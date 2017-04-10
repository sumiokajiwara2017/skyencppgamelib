#include "StdAfx.h"
#include "SkyBaseLib/File.h"
#include "SkyBaseLib/Directory.h"

namespace sky { namespace lib { namespace file {

hash::CHashStringPtrListPtrHashPtrHash	*CPropertyManager::m_pFileDataHash = skyNull;

void CPropertyManager::Initialize()
{
	//ファイル名コンテナを作成
	m_pFileDataHash = NEW__( hash::CHashStringPtrListPtrHashPtrHash , hash::CHashStringPtrListPtrHashPtrHash( property::PROPERTY_FILE_HASH_SIZE ) );

	//Directory内のファイルリストを作る
	skyString idir[ DIR_LEN_MAX ];
	skyString ifn[ FILE_LEN_MAX ];

	CPathUtil_::GetDirPath( property::PROPERTY_SEARCH_PATH , idir , DIR_LEN_MAX );
	CPathUtil_::GetFileName( property::PROPERTY_SEARCH_PATH , ifn , FILE_LEN_MAX );

	lib::directory::CDirectoryPtr dirPtr = CDirectoryFactory_::Create( idir , ifn );

	//ディレクトリクラスからファイルリストをもらう]
	lib::hash::CHashStringPtrList fileList;
	dirPtr->GetFilePathList( fileList ,skyFalse );

	//ファイルリストと出力ディレクトリでファイルを出力する
	FOREACH( lib::hash::CHashStringPtrList , it , fileList )
	{
		//ファイル名毎にコンテナをつくってデータを格納する
		hash::CHashStringPtrListPtrHashPtr propDataPtr( NEW__( hash::CHashStringPtrListPtrHash , hash::CHashStringPtrListPtrHash( property::DEFAULt_HASH_SIZE ) ) );

		//ファイルを読む
		sky::lib::stream::IStreamPtr spSrcStream = CFileUtil_::LoadFile( *it );

		//ストリームを開く
		spSrcStream->vOpen();

		//プロパティファイルをロード
		CPropertyFileUtil_::Load( spSrcStream , *propDataPtr.GetPtrNoConst() );

		//ストリームを閉じる
		spSrcStream->vClose();

		//データをファイル名コンテナに入れる
		skyString key[ FILE_LEN_MAX ];
		CPathUtil_::GetFileName( ( *it )->GetString() , key , FILE_LEN_MAX );
		m_pFileDataHash->Set( CHS( key ) , propDataPtr );
	}
}

void CPropertyManager::Dispose()
{
	//ファイル名コンテナを削除
	SAFE_DELETE( m_pFileDataHash );
	m_pFileDataHash = skyNull;
}

const hash::CHashStringPtr &CPropertyManager::GetString( const hash::CHashStringPtr &fileName , const hash::CHashStringPtr &key , u32 index )
{
	const hash::CHashStringPtrListPtrHashPtr &hashPtr = ( *m_pFileDataHash )[ fileName ];
	return CPropertyFileUtil_::GetString( key , *hashPtr.GetPtrNoConst() , index );
}

s32 CPropertyManager::Gets32( const hash::CHashStringPtr &fileName , const hash::CHashStringPtr &key , u32 index )
{
	const hash::CHashStringPtrListPtrHashPtr &hashPtr = ( *m_pFileDataHash )[ fileName ];
	return CPropertyFileUtil_::Gets32( key , *hashPtr.GetPtrNoConst() , index );
}

d64 CPropertyManager::Getd64( const hash::CHashStringPtr &fileName , const hash::CHashStringPtr &key , u32 index )
{
	const hash::CHashStringPtrListPtrHashPtr &hashPtr = ( *m_pFileDataHash )[ fileName ];
	return CPropertyFileUtil_::Getd64( key , *hashPtr.GetPtrNoConst() , index );
}

void CPropertyManager::Dump( stream::IStreamPtr &dest )
{
	FOREACH( hash::CHashStringPtrListPtrHashPtrHash , it , *m_pFileDataHash )
	{
		CPropertyFileUtil_::Dump( *( *it ).GetPtrNoConst() , dest );
	}
}

} } }