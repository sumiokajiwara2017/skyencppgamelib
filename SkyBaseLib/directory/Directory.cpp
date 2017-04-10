#include "StdAfx.h"
#include "SkyBaseLib/Directory.h"

SkyImplementRTTI( sky::lib::directory::CDirectory , sky::lib::directory::DirectoryBase );
template<>
SkyImplementRTTI( sky::lib::directory::DirectoryBase , sky::lib::base::SkyRefObject );

namespace sky { namespace lib { namespace directory {

//-------------------------------< ファクトリー >----------------------------------------//

CDirectoryPtr	CDirectoryFactory::Create( const skyString *pPath , const skyString *pWildCard )
{
	return CDirectoryPtr( NEW__( CDirectory , CDirectory( pPath , skyNull , pWildCard ) ) );
}

//-------------------------------------< 実装 >---------------------------------------------//

void CDirectory::SearchDir( const skyString* pRootDir, const skyString *pWildCard )
{
	HANDLE searchHndl;
	skyString  path[ SKY_PATH_LENGTH ];
	WIN32_FIND_DATA findFile;

	// ファイル検索条件合成
	CStringUtil_::Copy(path, SKY_PATH_LENGTH , pRootDir);
	CStringUtil_::Cat(path, pWildCard);

	// ファイル検索
	if ( (searchHndl = FindFirstFile(path, &findFile) ) != INVALID_HANDLE_VALUE)
	{
		if ( (findFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
		{//ファイルが見つかった
			CStringUtil_::Copy(path, SKY_PATH_LENGTH , pRootDir );
			AttachChild( NEW_() CDirectory( CStringUtil_::Cat( path, findFile.cFileName ) , this ) );
		}
		while(FindNextFile(searchHndl , &findFile) != 0)
		{//さらに存在するだけ別のファイルを探す
			if ( (findFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
			{//ファイルが見つかった
				CStringUtil_::Copy(path, SKY_PATH_LENGTH , pRootDir );
				AttachChild( NEW_() CDirectory( CStringUtil_::Cat( path, findFile.cFileName ) , this ) );
			}
		}
		FindClose(searchHndl);
	}

	// ディレクトリ検索条件合成
	CStringUtil_::Copy(path, SKY_PATH_LENGTH , pRootDir);
	CStringUtil_::Cat(path, _T( "*" ) );

	// ディレクトリ検索
	if ( (searchHndl = FindFirstFile(path, &findFile) ) == INVALID_HANDLE_VALUE) return;

	if ( (findFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
	{//ディレクトリが見つかった

		// カレント、親ディレクトリスキップ
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
		{//ディレクトリが見つかった

			// カレント、親ディレクトリスキップ
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

	// リターン
	return;
}

CDirectory::CDirectory( const skyString *pPath , CDirectory *pParent , const skyString *pWildCard ) :
DirectoryBase( pParent ) ,
m_bIsInitialized( skyFalse )
{
	SKY_ASSERT_MSG( CPathUtil_::IsSlashEnd( pPath ) , _T( "It should become the directory path is whether the end is \\" ) ); //ディレクトリパスは最後が/か\\にならなければなりません。

	m_Path = pPath;
	m_WildCard = pWildCard;
	m_eType = Type_Directory;

	SearchDir( m_Path, m_WildCard );
}

CDirectory::CDirectory( const skyString *pPath , CDirectory *pParent ) :
DirectoryBase( pParent ) ,
m_bIsInitialized( skyFalse )
{
	SKY_ASSERT_MSG( !CPathUtil_::IsSlashEnd( pPath ) , _T( "It tries to store the file as a directory." ) ); //何らかの理由でファイルをディレクトリとして格納しようとしています。

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
		//検索に行く
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
		//再帰で子のノードをすべて削除
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
	//ファイルを探してテキスト出力。ディレクトリを見つけたらその下に行きファイルを探す
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
	//ディレクトリを探してテキスト出力。ディレクトリを見つけたらその下に行きファイルを探す
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
	//ファイルを探して文字列をリストに格納。ディレクトリを見つけたらその下に行きファイルを探す
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
	//ディレクトリを探して文字列をリストに格納。ディレクトリを見つけたらその下に行きファイルを探す
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
