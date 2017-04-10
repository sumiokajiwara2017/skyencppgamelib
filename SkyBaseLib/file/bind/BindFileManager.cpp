#include "StdAfx.h"
#include "SkyBaseLib/File.h"

namespace sky { namespace lib { namespace file { 

//ファイル情報
struct BindFileInfo_
{
	u32                 category;
	stream::IStreamPtr  bindFile;
	CBindFilesHash      fileHash;
	FileLoadAsyncRs     retObject;               //このオブジェクトが存在しない場合、同期で読まれている。存在している場合は非同期で読まれていて、IsEnd()がskyTrueにならなければ使用できない。
	BindFileInfo_() : fileHash( bind::FILE_HASH_SIZE ){}
	virtual ~BindFileInfo_(){}
};
typedef BindFileInfo_*                               FileInfoPtr_;
typedef CList        < u32 , FileInfoPtr_ >      FileInfoInfoPtrList_;
typedef CMemoryPool  < BindFileInfo_ >			     FileInfoPool_;

struct CBindFileManagerMember
{
	//ファイル情報リスト
	FileInfoInfoPtrList_    fileInfoList;
	FileInfoPool_           fileInfoPool;

	//コンストラクタ
    CBindFileManagerMember() :
    fileInfoPool( bind::FILE_NUM_MAX ){}

	//デストラクタ
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
				//一つでも終わってなければ終わってないとする
				return skyFalse;
			}
			else if ( ( *it )->bindFile.IsEmpty() )
			{
				//非同期読み込みが完了したデータをセットアップする
				( *it )->bindFile = ( *it )->retObject->GetResult();                                                        //ファイル読み込み
				CBindUtil::CreateBindFilesHash( ( bind::FileHeader * )( *it )->bindFile->vGetData() , ( *it )->fileHash );   //読み込んだ結合ファイルを各ファイルにアクセスしやすいようにする
			}
		}
	}
	return skyTrue;
}

void CBindFileManager::Load( const hash::CHashStringPtr &pPath , u32 category )
{
	BindFileInfo_ *pFileInfo = m_pMember->fileInfoPool.Create();

	pFileInfo->category = category;
	pFileInfo->bindFile = CFileUtil_::LoadFile( pPath );                                                        //ファイル読み込み
	CBindUtil::CreateBindFilesHash( ( bind::FileHeader * )pFileInfo->bindFile.GetPtr() , pFileInfo->fileHash ); //読み込んだ結合ファイルを各ファイルにアクセスしやすいようにする

	m_pMember->fileInfoList.AddTail( pFileInfo );
}

void CBindFileManager::Delete( const hash::CHashStringPtr &pPath )
{
	FOREACH_NOINC( FileInfoInfoPtrList_ , it , m_pMember->fileInfoList )
	{
		if ( CHS( ( *it )->bindFile->vGetName() ) == pPath )
		{//見つけたので消す
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