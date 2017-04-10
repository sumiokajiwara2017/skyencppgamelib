#include "StdAfx.h"
#include "SkyBaseLib/File.h"

namespace sky { namespace lib { namespace file { 

//�t�@�C�����
struct BindFileInfo_
{
	u32                 category;
	stream::IStreamPtr  bindFile;
	CBindFilesHash      fileHash;
	FileLoadAsyncRs     retObject;               //���̃I�u�W�F�N�g�����݂��Ȃ��ꍇ�A�����œǂ܂�Ă���B���݂��Ă���ꍇ�͔񓯊��œǂ܂�Ă��āAIsEnd()��skyTrue�ɂȂ�Ȃ���Ύg�p�ł��Ȃ��B
	BindFileInfo_() : fileHash( bind::FILE_HASH_SIZE ){}
	virtual ~BindFileInfo_(){}
};
typedef BindFileInfo_*                               FileInfoPtr_;
typedef CList        < u32 , FileInfoPtr_ >      FileInfoInfoPtrList_;
typedef CMemoryPool  < BindFileInfo_ >			     FileInfoPool_;

struct CBindFileManagerMember
{
	//�t�@�C����񃊃X�g
	FileInfoInfoPtrList_    fileInfoList;
	FileInfoPool_           fileInfoPool;

	//�R���X�g���N�^
    CBindFileManagerMember() :
    fileInfoPool( bind::FILE_NUM_MAX ){}

	//�f�X�g���N�^
	virtual ~CBindFileManagerMember(){}
};

CBindFileManagerMember *CBindFileManager::m_pMember = skyNull;

void CBindFileManager::Initialize()
{
	m_pMember = NEW__( CBindFileManagerMember , CBindFileManagerMember() );
}

void CBindFileManager::Dispose()
{
	SAFE_DELETE( m_pMember );
}

void CBindFileManager::LoadAsync( const hash::CHashStringPtr &pPath , u32 category )
{
	FileInfoPtr_ pFileInfo = m_pMember->fileInfoPool.Create();

	pFileInfo->category = category;
	pFileInfo->retObject = CFileUtil_::LoadFileAsync( pPath );

	m_pMember->fileInfoList.AddTail( pFileInfo );
}

void CBindFileManager::CancelAll()
{
	FOREACH( FileInfoInfoPtrList_ , it , m_pMember->fileInfoList )
	{
		CFileUtil_::LoadFileAsyncCancel( ( *it )->retObject );
	}
}

skyBool CBindFileManager::IsLoadEnd()
{
	FOREACH( FileInfoInfoPtrList_ , it , m_pMember->fileInfoList )
	{
		if ( ( *it )->retObject.IsEmpty() == skyFalse )
		{
			if ( ( *it )->retObject->IsEnd() == skyFalse )
			{
				//��ł��I����ĂȂ���ΏI����ĂȂ��Ƃ���
				return skyFalse;
			}
			else if ( ( *it )->bindFile.IsEmpty() )
			{
				//�񓯊��ǂݍ��݂����������f�[�^���Z�b�g�A�b�v����
				( *it )->bindFile = ( *it )->retObject->GetResult();                                                        //�t�@�C���ǂݍ���
				CBindUtil::CreateBindFilesHash( ( bind::FileHeader * )( *it )->bindFile->vGetData() , ( *it )->fileHash );   //�ǂݍ��񂾌����t�@�C�����e�t�@�C���ɃA�N�Z�X���₷���悤�ɂ���
			}
		}
	}
	return skyTrue;
}

void CBindFileManager::Load( const hash::CHashStringPtr &pPath , u32 category )
{
	BindFileInfo_ *pFileInfo = m_pMember->fileInfoPool.Create();

	pFileInfo->category = category;
	pFileInfo->bindFile = CFileUtil_::LoadFile( pPath );                                                        //�t�@�C���ǂݍ���
	CBindUtil::CreateBindFilesHash( ( bind::FileHeader * )pFileInfo->bindFile.GetPtr() , pFileInfo->fileHash ); //�ǂݍ��񂾌����t�@�C�����e�t�@�C���ɃA�N�Z�X���₷���悤�ɂ���

	m_pMember->fileInfoList.AddTail( pFileInfo );
}

void CBindFileManager::Delete( const hash::CHashStringPtr &pPath )
{
	FOREACH_NOINC( FileInfoInfoPtrList_ , it , m_pMember->fileInfoList )
	{
		if ( CHS( ( *it )->bindFile->vGetName() ) == pPath )
		{//�������̂ŏ���
			m_pMember->fileInfoPool.Delete( *it );
			it = m_pMember->fileInfoList.Erases( it );
			break;
		}
		else
		{
			it++;
		}
	}
}

void CBindFileManager::Delete( u32 category )
{
	FOREACH_NOINC( FileInfoInfoPtrList_ , it , m_pMember->fileInfoList )
	{
		if ( ( *it )->category == category )
		{//�������̂ŏ���
			m_pMember->fileInfoPool.Delete( *it );
			it = m_pMember->fileInfoList.Erases( it );
		}
		else
		{
			it++;
		}
	}
}

const u8 *CBindFileManager::LoadFile( const hash::CHashStringPtr &pPath )
{
	FOREACH( FileInfoInfoPtrList_ , it , m_pMember->fileInfoList )
	{
		FOREACH( CBindFilesHash , ith ,  ( *it )->fileHash )
		{
			if ( CHS( ( const skyString *)( *ith )->cFilePath ) == pPath )
			{
				return CBindUtil::GetFileData( ( *ith ) );
			}
		}
	}
	return skyNull;
}

skyBool CBindFileManager::DeleteFile( const hash::CHashStringPtr &pPath )
{
	FOREACH( FileInfoInfoPtrList_ , it , m_pMember->fileInfoList )
	{
		FOREACH( CBindFilesHash , ith ,  ( *it )->fileHash )
		{
			if ( CHS( ( const skyString *)( *ith )->cFilePath ) == pPath )
			{
				return skyTrue;
			}
		}
	}
	return skyFalse;
}

void CBindFileManager::Print()
{
	SKY_PRINTF( _T( "======< BindFileList >====== \n" ) );
	FOREACH( FileInfoInfoPtrList_ , it , m_pMember->fileInfoList )
	{
		FOREACH( CBindFilesHash , ith ,  ( *it )->fileHash )
		{
			SKY_PRINTF( _T( "BindFile:%s \n" ) , ( *ith )->cFilePath );
		}
	}
}

} } }