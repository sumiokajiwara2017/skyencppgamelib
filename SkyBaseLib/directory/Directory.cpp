#include "StdAfx.h"
#include "SkyBaseLib/Directory.h"

SkyImplementRTTI( sky::lib::directory::CDirectory , sky::lib::directory::DirectoryBase );
template<>
SkyImplementRTTI( sky::lib::directory::DirectoryBase , sky::lib::base::SkyRefObject );

namespace sky { namespace lib { namespace directory {

//-------------------------------< �t�@�N�g���[ >----------------------------------------//

CDirectoryPtr	CDirectoryFactory::Create( const skyString *pPath , const skyString *pWildCard )
{
	return CDirectoryPtr( NEW__( CDirectory , CDirectory( pPath , skyNull , pWildCard ) ) );
}

//-------------------------------------< ���� >---------------------------------------------//

void CDirectory::SearchDir( const skyString* pRootDir, const skyString *pWildCard )
{
	HANDLE searchHndl;
	skyString  path[ SKY_PATH_LENGTH ];
	WIN32_FIND_DATA findFile;

	// �t�@�C��������������
	CStringUtil_::Copy(path, SKY_PATH_LENGTH , pRootDir);
	CStringUtil_::Cat(path, pWildCard);

	// �t�@�C������
	if ( (searchHndl = FindFirstFile(path, &findFile) ) != INVALID_HANDLE_VALUE)
	{
		if ( (findFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
		{//�t�@�C������������
			CStringUtil_::Copy(path, SKY_PATH_LENGTH , pRootDir );
			AttachChild( NEW_() CDirectory( CStringUtil_::Cat( path, findFile.cFileName ) , this ) );
		}
		while(FindNextFile(searchHndl , &findFile) != 0)
		{//����ɑ��݂��邾���ʂ̃t�@�C����T��
			if ( (findFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
			{//�t�@�C������������
				CStringUtil_::Copy(path, SKY_PATH_LENGTH , pRootDir );
				AttachChild( NEW_() CDirectory( CStringUtil_::Cat( path, findFile.cFileName ) , this ) );
			}
		}
		FindClose(searchHndl);
	}

	// �f�B���N�g��������������
	CStringUtil_::Copy(path, SKY_PATH_LENGTH , pRootDir);
	CStringUtil_::Cat(path, _T( "*" ) );

	// �f�B���N�g������
	if ( (searchHndl = FindFirstFile(path, &findFile) ) == INVALID_HANDLE_VALUE) return;

	if ( (findFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
	{//�f�B���N�g������������

		// �J�����g�A�e�f�B���N�g���X�L�b�v
		if ( !( CStringUtil_::Comp(findFile.cFileName , _T( "." ) ) || 
			CStringUtil_::Comp(findFile.cFileName , _T( ".." ) ) ) )
		{
			CStringUtil_::Copy(path, SKY_PATH_LENGTH , pRootDir );
			CDirectory *pChild = NEW_() CDirectory( CStringUtil_::Cat( CStringUtil_::Cat( path, findFile.cFileName ) , _T( "/" ) ) , this , pWildCard );
			pChild->Initialize();
			AttachChild( pChild );
		}
	}
	while(FindNextFile(searchHndl , &findFile) != 0)
	{
		if ( (findFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
		{//�f�B���N�g������������

			// �J�����g�A�e�f�B���N�g���X�L�b�v
			if ( !( CStringUtil_::Comp(findFile.cFileName , _T( "." ) ) || 
				CStringUtil_::Comp(findFile.cFileName , _T( ".." ) ) ) )
			{
				CStringUtil_::Copy(path, SKY_PATH_LENGTH , pRootDir );
				CDirectory *pChild = NEW_() CDirectory( CStringUtil_::Cat( CStringUtil_::Cat( path, findFile.cFileName ) , _T( "/" ) ) , this , pWildCard );
				pChild->Initialize();
				AttachChild( pChild );
			}
		}
	}
	FindClose(searchHndl);

	// ���^�[��
	return;
}

CDirectory::CDirectory( const skyString *pPath , CDirectory *pParent , const skyString *pWildCard ) :
DirectoryBase( pParent ) ,
m_bIsInitialized( skyFalse )
{
	SKY_ASSERT_MSG( CPathUtil_::IsSlashEnd( pPath ) , _T( "It should become the directory path is whether the end is \\" ) ); //�f�B���N�g���p�X�͍Ōオ/��\\�ɂȂ�Ȃ���΂Ȃ�܂���B

	m_Path = pPath;
	m_WildCard = pWildCard;
	m_eType = Type_Directory;

	SearchDir( m_Path, m_WildCard );
}

CDirectory::CDirectory( const skyString *pPath , CDirectory *pParent ) :
DirectoryBase( pParent ) ,
m_bIsInitialized( skyFalse )
{
	SKY_ASSERT_MSG( !CPathUtil_::IsSlashEnd( pPath ) , _T( "It tries to store the file as a directory." ) ); //���炩�̗��R�Ńt�@�C�����f�B���N�g���Ƃ��Ċi�[���悤�Ƃ��Ă��܂��B

	m_Path = pPath;
	m_eType = Type_File;
}

CDirectory::~CDirectory()
{
	_DeleteChild();
}

const skyString* CDirectory::GetPath()
{
	return m_Path.Get();
}

skyBool CDirectory::IsFile()
{
	return ( m_eType == Type_File );
}

void CDirectory::Initialize()
{
	if ( m_bIsInitialized == skyFalse )
	{
		//�����ɍs��
		SearchDir( m_Path.Get(), m_WildCard.Get() );

		m_bIsInitialized = skyTrue;
	}
}

void CDirectory::_DeleteChild()
{
	FOREACH( DirectoryList , it , m_ChildList )
	{
		DEL *it;
	}
	m_ChildList.DeleteAll();
}

void CDirectory::Dispose()
{
	if ( m_bIsInitialized == skyTrue )
	{
		//�ċA�Ŏq�̃m�[�h�����ׂč폜
		while( skyTrue )
		{
			if ( this->IsChildOn() == skyTrue )
			{
				FOREACH( DirectoryList , it , m_ChildList )
				{
					( *it )->Dispose();
				}
			}
			else
			{
				_DeleteChild();
				break;
			}
		}

		m_bIsInitialized = skyFalse;
	}
}

void CDirectory::WriteFileList( file::CFileStreamPtr &textFilePtr , skyBool isR , const skyString *wildCard )
{
	//�t�@�C����T���ăe�L�X�g�o�́B�f�B���N�g�����������炻�̉��ɍs���t�@�C����T��
	FOREACH( DirectoryList , it , m_ChildList )
	{
		if ( ( *it )->IsFile() )
		{
			if ( wildCard != skyNull )
			{
				if ( CPathUtil_::IsWildCardMatch( wildCard , ( *it )->GetPath() ) )
				{
					textFilePtr->vWriteLine( ( *it )->GetPath() , CStringUtil_::LenChar( ( *it )->GetPath() ) );
				}
			}
			else
			{
				textFilePtr->vWriteLine( ( *it )->GetPath() , CStringUtil_::LenChar( ( *it )->GetPath() ) );
			}
		}
		else
		{
			if ( isR == skyTrue )
			{
				( *it )->WriteFileList( textFilePtr );
			}
		}
	}
}

void CDirectory::WriteDirList( file::CFileStreamPtr &textFilePtr , skyBool isR )
{
	//�f�B���N�g����T���ăe�L�X�g�o�́B�f�B���N�g�����������炻�̉��ɍs���t�@�C����T��
	FOREACH( DirectoryList , it , m_ChildList )
	{
		if ( !( *it )->IsFile() )
		{
			textFilePtr->vWriteLine( ( *it )->GetPath() , CStringUtil_::LenChar( ( *it )->GetPath() ) );

			if ( isR == skyTrue )
			{
				( *it )->WriteFileList( textFilePtr );
			}
		}
	}
}

void CDirectory::GetFilePathList( hash::CHashStringPtrList &filePathList , skyBool isR , const skyString *wildCard )
{
	//�t�@�C����T���ĕ���������X�g�Ɋi�[�B�f�B���N�g�����������炻�̉��ɍs���t�@�C����T��
	FOREACH( DirectoryList , it , m_ChildList )
	{
		if ( ( *it )->IsFile() )
		{
			if ( wildCard != skyNull )
			{
				if ( CPathUtil_::IsWildCardMatch( wildCard , ( *it )->GetPath() ) )
				{
					filePathList.AddTail( CHS( ( *it )->GetPath() ) );
				}
			}
			else
			{
				filePathList.AddTail( CHS( ( *it )->GetPath() ) );
			}
		}
		else
		{
			if ( isR == skyTrue )
			{
				( *it )->GetFilePathList( filePathList );
			}
		}
	}
}

void CDirectory::GetDirPathList( hash::CHashStringPtrList &dirPathList , skyBool isR )
{
	//�f�B���N�g����T���ĕ���������X�g�Ɋi�[�B�f�B���N�g�����������炻�̉��ɍs���t�@�C����T��
	FOREACH( DirectoryList , it , m_ChildList )
	{
		if ( !( *it )->IsFile() )
		{
			dirPathList.AddTail( CHS( ( *it )->GetPath() ) );

			if ( isR == skyTrue )
			{
				( *it )->GetDirPathList( dirPathList );
			}
		}
	}
}

} } }
