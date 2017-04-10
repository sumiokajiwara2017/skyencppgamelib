#include "StdAfx.h"
#include "SkyBaseLib/File.h"

namespace sky { namespace lib { namespace file { 

//�t�@�C�����
struct PoolFileInfo_
{
	u32                 category;
	stream::IStreamPtr  bindFile;
	FileLoadAsyncRs     retObject;               //���̃I�u�W�F�N�g�����݂��Ȃ��ꍇ�A�����œǂ܂�Ă���B���݂��Ă���ꍇ�͔񓯊��œǂ܂�Ă��āAIsEnd()��skyTrue�ɂȂ�Ȃ���Ύg�p�ł��Ȃ��B
	virtual ~PoolFileInfo_(){}
};
typedef PoolFileInfo_*                               FileInfoPtr_;
typedef CList        < u32 , FileInfoPtr_ >      FileInfoInfoPtrList_;
typedef CMemoryPool  < PoolFileInfo_ >			     FileInfoPool_;

struct CFilePoolContenaMember
{
	//�t�@�C����񃊃X�g
	FileInfoInfoPtrList_    fileInfoList;
	FileInfoPool_           fileInfoPool;

	//�R���X�g���N�^
    CFilePoolContenaMember() :
    fileInfoPool( pool::FILE_NUM_MAX ){}

	//�f�X�g���N�^
	virtual ~CFilePoolContenaMember(){}
};

CFilePoolContenaMember *CFilePoolContena::m_pMember = skyNull;

void CFilePoolContena::Initialize()
{
	m_pMember = NEW__( CFilePoolContenaMember , CFilePoolContenaMember() );
}

void CFilePoolContena::Dispose()
{
	SAFE_DELETE( m_pMember );
}

void CFilePoolContena::LoadAsync( const hash::CHashStringPtr &pPath , u32 category )
{
	FileInfoPtr_ pFileInfo = m_pMember->fileInfoPool.Create();

	pFileInfo->category = category;
	pFileInfo->retObject = CFileUtil_::LoadFileAsync( pPath );

	m_pMember->fileInfoList.AddTail( pFileInfo );
}

void CFilePoolContena::CancelAll()
{
	FOREACH( FileInfoInfoPtrList_ , it , m_pMember->fileInfoList )
	{
		CFileUtil_::LoadFileAsyncCancel( ( *it )->retObject );
	}
}

skyBool CFilePoolContena::IsLoadEnd()
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

void CFilePoolContena::Load( const hash::CHashStringPtr &pPath , u32 category )
{
	PoolFileInfo_ *pFileInfo = m_pMember->fileInfoPool.Create();

	pFileInfo->category = category;
	pFileInfo->bindFile = CFileUtil_::LoadFile( pPath );                                                        //�t�@�C���ǂݍ���

	m_pMember->fileInfoList.AddTail( pFileInfo );
}

void CFilePoolContena::Delete( const hash::CHashStringPtr &pPath )
{
	//���ʂɃt�@�C��������
	DeleteFile( pPath );
}

void CFilePoolContena::Delete( u32 category )
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

const u8 *CFilePoolContena::LoadFile( const hash::CHashStringPtr &pPath )
{
	FOREACH( FileInfoInfoPtrList_ , it , m_pMember->fileInfoList )
	{
		if ( CHS( ( *it )->bindFile->vGetName() ) == pPath )
		{
			return ( *it )->bindFile->vGetData(); //���������̂Ńf�[�^��Ԃ�
		}
	}
	return skyNull; //������Ȃ�������null��Ԃ�
}

skyBool CFilePoolContena::DeleteFile( const hash::CHashStringPtr &pPath )
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

void CFilePoolContena::Print()
{
	SKY_PRINTF( _T( "======< PoolFileList >====== \n" ) );
	FOREACH( FileInfoInfoPtrList_ , it , m_pMember->fileInfoList )
	{
		SKY_PRINTF( _T( "PoolFile:%s \n" ) , ( *it )->bindFile->vGetName() );
	}
}


} } }