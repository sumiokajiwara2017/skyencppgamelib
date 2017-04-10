#pragma once

namespace sky { namespace lib { namespace file { 

/// <summary>
/// バインド元データリスト
/// </summary>
struct CBindSrcData : public base::SkyRefObject
{
	string::CString			path;
	u8						*pData;
	u32						dataSize;
};
typedef SmartPointer< CBindSrcData >		CBindSrcDataPtr;
typedef CList< u32 , CBindSrcDataPtr >		CBindSrcDataPtrPtrList;	//リスト定義
typedef CHash< bind::DataHeader* >			CBindFilesHash;	//リスト定義

/// <summary>
/// バインドユーティリティ関数群
/// </summary>
namespace CBindUtil
{
	/// <summary>
	/// bind::DataHeaderアドレスからファイルデータを取得する
	/// </summary>
	inline u8 *GetFileData( bind::DataHeader *pData );

	/// <summary>
	/// 指定のファイル群を連結してデータを返す
	/// pCutPathFrontを指定すると、ファイル結合時に保存されるファイルパスの先頭と指定した文字列が一致した場合、
	/// ファイルパスから消されて結合ファイルに保存される。Windows上の絶対パスを保持されても困ることのほうが多いはずなのでこの機能で絶対パス分をカットする
	/// </summary>
	inline skyBool BindFiles( string::CStringList &srcPathList , bind::FileHeader* &pDestBindData , skyBool isRepSlash = skyFalse , const skyString *pCutPathFront = _T( "" ) );

	/// <summary>
	/// 指定のデータ群を連結。
	/// </summary>
	inline skyBool BindDatas( CBindSrcDataPtrPtrList &srcDataList , bind::FileHeader* &pDestBindData , skyBool isRepSlash = skyFalse , const skyString *pCutPathFront = _T( "" ) );

	/// <summary>
	/// 指定の連結ファイルからパスを指定してファイルを読み込む
	/// 読み込んだデータはあくまで結合ファイルのデータ中のポインタなので利用し終わっても消さないでください。
	/// </summary>
	inline skyBool LoadFile( bind::FileHeader *pSrcBindData , const skyString *pSrcPath , bind::DataHeader* &pDestData );

	/// <summary>
	/// 指定の連結ファイルからファイルパスをキーにしたHashコンテナを作成する。ファイルパスでファイルデータに高速にアクセス可能
	/// </summary>
	inline void CreateBindFilesHash( bind::FileHeader *pSrcBindData , CBindFilesHash &destHash );
}

#define CBindUtil_					sky::lib::file::CBindUtil

//-------------------------------< 実装 >----------------------------------------------

inline skyBool CBindUtil::BindFiles( string::CStringList &srcPathList , bind::FileHeader* &pDestBindData , skyBool isRepSlash , const skyString *pCutPathFront )
{
	//データリスト
	CBindSrcDataPtrPtrList srcDataList;

	//パスリストからデータリストを作成する
	FOREACH( string::CStringList , it , srcPathList )
	{
		u8 *bbuf;
		u32 readSize;
		if ( CFileUtil::LoadFileFromDisc( ( *it ) , &bbuf , &readSize ) == skyFalse )
		{
            SKY_PANIC_MSG( _T( "file can't open. %s \n" ) , ( *it ) );
			continue;
		}

		CBindSrcDataPtr dataPtr = CBindSrcDataPtr( NEW__( CBindSrcData , CBindSrcData() ) );
		dataPtr->path = ( *it );
		dataPtr->pData = bbuf;
		dataPtr->dataSize = readSize;

		srcDataList.AddTail( dataPtr );
	}

	//結合する
	BindDatas( srcDataList , pDestBindData , isRepSlash , pCutPathFront );

	FOREACH( CBindSrcDataPtrPtrList , it , srcDataList )
	{
		DEL( ( *it )->pData ); //ヒープに確保したファイルデータを消す
	}

	return skyFalse;
}

inline u8 *CBindUtil::GetFileData( bind::DataHeader *pData )
{
	//バインドされているデータを一つ一つたどっていく
	u8* pCursor = ( u8* )pData;

	//ヘッダーサイズ分進めたアドレスがファイルデータアドレスとなる。
	return pCursor + sizeof ( bind::DataHeader );
}

inline skyBool CBindUtil::BindDatas( CBindSrcDataPtrPtrList &srcDataList , bind::FileHeader* &pDestBindData , skyBool isRepSlash , const skyString *pCutPathFront )
{
	//まずはサイズを計算（最初にファイルヘッダーのサイズを加える）
	memSize bindDataSize = sizeof( bind::FileHeader );

	//データのサイズとデータヘッダーを足したサイズを全データ分加算する
	FOREACH( CBindSrcDataPtrPtrList , it , srcDataList )
	{
		//データヘッダーとデータサイズを加算
		bindDataSize += sizeof ( bind::DataHeader ) + ( *it )->dataSize;
	}

	//結合ファイル分の領域確保
	pDestBindData = ( bind::FileHeader * )SkyMalloc( bindDataSize );
	u8* pCursor = ( u8 * )pDestBindData;

	//全体を0クリア
	CMemoryUtil_::ClearZero( pDestBindData, bindDataSize );

	//ヘッダー情報を格納
	CMemoryUtil_::Copy( pDestBindData->cCode , bind::g_cFileCode, sizeof( bind::g_cFileCode ) );
	pDestBindData->bIsBigEndian = endian::EndianUtil::GetEndian();
	pDestBindData->nFileBindCount = srcDataList.GetNum();
	pDestBindData->nVersion = bind::VERSION_NO;
	pDestBindData->nFileSize = bindDataSize;
	pCursor = pCursor + sizeof ( bind::FileHeader );

	bind::DataHeader *pData;
	FOREACH( CBindSrcDataPtrPtrList , it , srcDataList )
	{
		//データの先頭をデータヘッダーにする
		pData = ( bind::DataHeader * )pCursor;

		//カット指定が着ていたらパスをカットする
		const skyString *savePath = ( *it )->path;
		if ( CStringUtil_::Len( pCutPathFront ) > 0 )
		{
			//カットパスと同じ文字列がパスの先頭にあるか？（パスの先頭以外にあったらカット対象とみなさない）
			if ( CStringUtil_::StrstrDistance( savePath , pCutPathFront ) == 0 )
			{//あるのでカットする（ポインタの先頭をカットパス分進める）
				savePath = savePath + CStringUtil_::Len( pCutPathFront );
			}
		}

		u32 dataPathSize = CStringUtil_::LenByte( savePath );
        SKY_ASSERT_MSG( dataPathSize <= PATH_LEN_MAX , _T( "file length over. %s \n" ) , ( *it )->path );

		//データヘッダーを作成する
		CMemoryUtil_::Copy( pData->cFilePath , savePath ,  dataPathSize );

		if ( isRepSlash == skyTrue )
		{
			//スラッシュ置き換え指定がきていたら\マークをスラッシュに置き換える
			CStringUtil_::Replace( ( skyString *)pData->cFilePath , _T( "\\" ) , _T( "/" ) );
		}

		pData->fileSize = ( *it )->dataSize;
		pData->nCompressFlg = 0;

		//カーソルをヘッダーからデータ部へ
		pCursor = pCursor + sizeof ( bind::DataHeader );

		//データをコピー
		CMemoryUtil_::Copy( pCursor , ( *it )->pData , ( *it )->dataSize );

		//カーソルを次のデータの先頭へ
		pCursor = pCursor + ( *it )->dataSize;
	}

	return skyFalse;
}

inline skyBool CBindUtil::LoadFile( bind::FileHeader *pSrcBindData , const skyString *pSrcPath , bind::DataHeader* &pDestData )
{
	//バインドされているデータを一つ一つたどっていく
	u8* pCursor = ( u8* )pSrcBindData;

	pCursor = pCursor + sizeof ( bind::FileHeader );
	bind::DataHeader *pData;
	for( u32 i = 0 ; i < pSrcBindData->nFileBindCount ; i++ )
	{
		//データの先頭をデータヘッダーにする
		pData = ( bind::DataHeader * )pCursor;

		if (  CStringUtil_::Comp( pSrcPath , ( skyString * )pData->cFilePath ) )
		{
			//データを発見したのでこのデータを返す
			pDestData = pData;
			return skyTrue; //見つかったので強制終了
		}
		else
		{
			//次のデータへ
			pCursor = pCursor + sizeof ( bind::DataHeader ) + pData->fileSize; 
		}
	}

	return skyFalse;
}

inline void CBindUtil::CreateBindFilesHash( bind::FileHeader *pSrcBindData , CBindFilesHash &destHash )
{
	//バインドされているデータを一つ一つたどっていく
	u8* pCursor = ( u8* )pSrcBindData;

	pCursor = pCursor + sizeof ( bind::FileHeader );
	bind::DataHeader *pData;
	for( u32 i = 0 ; i < pSrcBindData->nFileBindCount ; i++ )
	{
		//データの先頭をデータヘッダーにする
		pData = ( bind::DataHeader * )pCursor;

		//Hashに格納する
		destHash.Set( CHS( ( skyString * )pData->cFilePath ) , pData );

		//次のデータへ
		pCursor = pCursor + sizeof ( bind::DataHeader ) + pData->fileSize; 
	}
}


} } }
