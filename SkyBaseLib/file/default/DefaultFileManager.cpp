#include "StdAfx.h"
#include "SkyBaseLib/File.h"

#include "SkyBaseLib/Directory.h"

namespace sky { namespace lib { namespace file { 

//ファイル情報
struct DefaultFileInfo_
{
	u32                 category;
	stream::IStreamPtr  bindFile;
	FileLoadAsyncRs     retObject;               //このオブジェクトが存在しない場合、同期で読まれている。存在している場合は非同期で読まれていて、IsEnd()がskyTrueにならなければ使用できない。
	virtual ~DefaultFileInfo_(){}
};
typedef DefaultFileInfo_*                               FileInfoPtr_;
typedef CList        < u32 , FileInfoPtr_ >      FileInfoInfoPtrList_;
typedef CMemoryPool  < DefaultFileInfo_ >			     FileInfoPool_;

struct CDefaultFileManagerMember
{
	//ファイル情報リスト
	FileInfoInfoPtrList_    fileInfoList;
	FileInfoPool_           fileInfoPool;

	//コンストラクタ
    CDefaultFileManagerMember() :
    fileInfoPool( pool::FILE_NUM_MAX ){}

	//デストラクタ
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

void CDefaultFileManager::Load( const hash::CHashStringPtr &pPath , u32 category )
{
	DefaultFileInfo_ *pFileInfo = m_pMember->fileInfoPool.Create();

	pFileInfo->category = category;
	pFileInfo->bindFile = CFileUtil_::LoadFile( pPath );                                                        //ファイル読み込み

	m_pMember->fileInfoList.AddTail( pFileInfo );
}

void CDefaultFileManager::Delete( const hash::CHashStringPtr &pPath )
{
	//普通にファイルを消す
	DeleteFile( pPath );
}

void CDefaultFileManager::Delete( u32 category )
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

const u8 *CDefaultFileManager::LoadFile( const hash::CHashStringPtr &pPath )
{
	FOREACH( FileInfoInfoPtrList_ , it , m_pMember->fileInfoList )
	{
		//拡張子同士を比較する
		skyString extA[ EXT_LEN_MAX ];
		skyString extB[ EXT_LEN_MAX ];
		CPathUtil_::GetFileExt( ( *it )->bindFile->vGetName() , extA , EXT_LEN_MAX );
		CPathUtil_::GetFileExt( pPath , extB , EXT_LEN_MAX );
		if ( CStringUtil_::Comp( extA ,extB ) == skyTrue )
		{
			return ( *it )->bindFile->vGetData(); //見つかったのでデータを返す
		}
	}
	return skyNull; //見つからなかったのnullを返す
}

skyBool CDefaultFileManager::DeleteFile( const hash::CHashStringPtr &pPath )
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

void CDefaultFileManager::Print()
{
	SKY_PRINTF( _T( "======< DefaultFileList >====== \n" ) );
	FOREACH( FileInfoInfoPtrList_ , it , m_pMember->fileInfoList )
	{
		SKY_PRINTF( _T( "DefaultFile:%s \n" ) , ( *it )->bindFile->vGetName() );
	}
}


} } }