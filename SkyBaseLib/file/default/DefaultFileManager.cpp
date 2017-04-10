#include "StdAfx.h"
#include "SkyBaseLib/File.h"

#include "SkyBaseLib/Directory.h"

namespace sky { namespace lib { namespace file { 

//�t�@�C�����
struct DefaultFileInfo_
{
	u32                 category;
	stream::IStreamPtr  bindFile;
	FileLoadAsyncRs     retObject;               //���̃I�u�W�F�N�g�����݂��Ȃ��ꍇ�A�����œǂ܂�Ă���B���݂��Ă���ꍇ�͔񓯊��œǂ܂�Ă��āAIsEnd()��skyTrue�ɂȂ�Ȃ���Ύg�p�ł��Ȃ��B
	virtual ~DefaultFileInfo_(){}
};
typedef DefaultFileInfo_*                               FileInfoPtr_;
typedef CList        < u32 , FileInfoPtr_ >      FileInfoInfoPtrList_;
typedef CMemoryPool  < DefaultFileInfo_ >			     FileInfoPool_;

struct CDefaultFileManagerMember
{
	//�t�@�C����񃊃X�g
	FileInfoInfoPtrList_    fileInfoList;
	FileInfoPool_           fileInfoPool;

	//�R���X�g���N�^
    CDefaultFileManagerMember() :
    fileInfoPool( pool::FILE_NUM_MAX ){}

	//�f�X�g���N�^
	virtual ~CDefaultFileManagerMember(){}
};

CDefaultFileManagerMember *CDefaultFileManager::m_pMember = skyNull;

void CDefaultFileManager::Initialize()
{
	m_pMember = NEW__( CDefaultFileManagerMember , CDefaultFileManagerMember() );
}

void CDefaultFileManager::Dispose()
{
	SAFE_DELETE( m_pMember );
}

void CDefaultFileManager::LoadAsync( const hash::CHashStringPtr &pPath , u32 category )
{
	FileInfoPtr_ pFileInfo = m_pMember->fileInfoPool.Create();

	pFileInfo->category = category;
	pFileInfo->retObject = CFileUtil_::LoadFileAsync( pPath );

	m_pMember->fileInfoList.AddTail( pFileInfo );
}

void CDefaultFileManager::CancelAll()
{
	FOREACH( FileInfoInfoPtrList_ , it , m_pMember->fileInfoList )
	{
		CFileUtil_::LoadFileAsyncCancel( ( *it )->retObject );
	}
}

skyBool CDefaultFileManager::IsLoadEnd()
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
			}
		}
	}
	return skyTrue;
}

void CDefaultFileManager::Load( const hash::CHashStringPtr &pPath , u32 category )
{
	DefaultFileInfo_ *pFileInfo = m_pMember->fileInfoPool.Create();

	pFileInfo->category = category;
	pFileInfo->bindFile = CFileUtil_::LoadFile( pPath );                                                        //�t�@�C���ǂݍ���

	m_pMember->fileInfoList.AddTail( pFileInfo );
}

void CDefaultFileManager::Delete( const hash::CHashStringPtr &pPath )
{
	//���ʂɃt�@�C��������
	DeleteFile( pPath );
}

void CDefaultFileManager::Delete( u32 category )
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

const u8 *CDefaultFileManager::LoadFile( const hash::CHashStringPtr &pPath )
{
	FOREACH( FileInfoInfoPtrList_ , it , m_pMember->fileInfoList )
	{
		//�g���q���m���r����
		skyString extA[ EXT_LEN_MAX ];
		skyString extB[ EXT_LEN_MAX ];
		CPathUtil_::GetFileExt( ( *it )->bindFile->vGetName() , extA , EXT_LEN_MAX );
		CPathUtil_::GetFileExt( pPath , extB , EXT_LEN_MAX );
		if ( CStringUtil_::Comp( extA ,extB ) == skyTrue )
		{
			return ( *it )->bindFile->vGetData(); //���������̂Ńf�[�^��Ԃ�
		}
	}
	return skyNull; //������Ȃ�������null��Ԃ�
}

skyBool CDefaultFileManager::DeleteFile( const hash::CHashStringPtr &pPath )
{
	FOREACH_NOINC( FileInfoInfoPtrList_ , it , m_pMember->fileInfoList )
	{
		if ( CHS( ( *it )->bindFile->vGetName() ) == pPath )
		{//�������̂ŏ���
			m_pMember->fileInfoPool.Delete( *it );
			it = m_pMember->fileInfoList.Erases( it );
			return skyTrue;
		}
		else
		{
			it++;
		}
	}
	return skyFalse;
}

void CDefaultFileManager::Print()
{
	SKY_PRINTF( _T( "======< DefaultFileList >====== \n" ) );
	FOREACH( FileInfoInfoPtrList_ , it , m_pMember->fileInfoList )
	{
		SKY_PRINTF( _T( "DefaultFile:%s \n" ) , ( *it )->bindFile->vGetName() );
	}
}


} } }