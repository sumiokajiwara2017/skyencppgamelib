#include "StdAfx.h"
#include "SkyBaseLib/File.h"

namespace sky { namespace lib { namespace file { 

//ファイル情報
struct PoolFileInfo_
{
	u32                 category;
	stream::IStreamPtr  bindFile;
	FileLoadAsyncRs     retObject;               //このオブジェクトが存在しない場合、同期で読まれている。存在している場合は非同期で読まれていて、IsEnd()がskyTrueにならなければ使用できない。
	virtual ~PoolFileInfo_(){}
};
typedef PoolFileInfo_*                               FileInfoPtr_;
typedef CList        < u32 , FileInfoPtr_ >      FileInfoInfoPtrList_;
typedef CMemoryPool  < PoolFileInfo_ >			     FileInfoPool_;

struct CFilePoolContenaMember
{
	//ファイル情報リスト
	FileInfoInfoPtrList_    fileInfoList;
	FileInfoPool_           fileInfoPool;

	//コンストラクタ
    CFilePoolContenaMember() :
    fileInfoPool( pool::FILE_NUM_MAX ){}

	//デストラクタ
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
				//一つでも終わってなければ終わってないとする
				return skyFalse;
			}
			else if ( ( *it )->bindFile.IsEmpty() )
			{
				//非同期読み込みが完了したデータをセットアップする
				( *it )->bindFile = ( *it )->retObject->GetResult();                                                        //ファイル読み込み
			}
		}
	}
	return skyTrue;
}

void CFilePoolContena::Load( const hash::CHashStringPtr &pPath , u32 category )
{
	PoolFileInfo_ *pFileInfo = m_pMember->fileInfoPool.Create();

	pFileInfo->category = category;
	pFileInfo->bindFile = CFileUtil_::LoadFile( pPath );                                                        //ファイル読み込み

	m_pMember->fileInfoList.AddTail( pFileInfo );
}

void CFilePoolContena::Delete( const hash::CHashStringPtr &pPath )
{
	//普通にファイルを消す
	DeleteFile( pPath );
}

void CFilePoolContena::Delete( u32 category )
{
	FOREACH_NOINC( FileInfoInfoPtrList_ , it , m_pMember->fileInfoList )
	{
		if ( ( *it )->category == category )
		{//見つけたので消す
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
			return ( *it )->bindFile->vGetData(); //見つかったのでデータを返す
		}
	}
	return skyNull; //見つからなかったのnullを返す
}

skyBool CFilePoolContena::DeleteFile( const hash::CHashStringPtr &pPath )
{
	FOREACH_NOINC( FileInfoInfoPtrList_ , it , m_pMember->fileInfoList )
	{
		if ( CHS( ( *it )->bindFile->vGetName() ) == pPath )
		{//見つけたので消す
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