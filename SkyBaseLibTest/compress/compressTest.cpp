#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_COMPRESS

#include "SkyBaseLib/Compress.h"

#define COMPRESS_SRC_FILE_PATH		_T( "data/test/compress/CompressSrcFile.txt" )
#define COMPRESS_FILE_PATH			_T( "data/test/compress/CompressFile.gz" )
#define UNCOMPRESS_FILE_PATH		_T( "data/test/compress/UnCompressFile.txt" )

TEST( compress , 1 ) 
{
	SKY_BASE_LIB_INIT();

//----------------データをファイルに圧縮して保存
	
	//ファイルを開く
	u8 *bbuf;
	u32 readSize;
	if ( CFileUtil_::LoadFileFromDisc( COMPRESS_SRC_FILE_PATH, &bbuf , &readSize ) == skyFalse )
	{
		SKY_PANIC_MSG( _T( "can't open file \n" ) );
		return;
	}

	//圧縮
	CCompressUtil_::CompressFile( bbuf , readSize , CHS( COMPRESS_FILE_PATH ) );

	//読み込みデータ破棄
	DEL bbuf;

//----------------ファイルから解凍しながらデータをロード

	//解凍
#define UNCOMPRESS_DATA_BUF_SIZE 256
	u8 uncompressDataBuf[ UNCOMPRESS_DATA_BUF_SIZE ];
	u32 uncompressDataSize = 0;
	CCompressUtil_::UnCompressFile( &uncompressDataBuf[ 0 ] , UNCOMPRESS_DATA_BUF_SIZE , uncompressDataSize , CHS( COMPRESS_FILE_PATH ) );

//----------------データを圧縮してデータに入れる

	//圧縮元データ
	u8 compressSrcData[ UNCOMPRESS_DATA_BUF_SIZE ] = "abcdefghijklmnbbbb";

	//圧縮データ格納バッファ
	u8 compressDataBuffer[ UNCOMPRESS_DATA_BUF_SIZE ];
	u32 compressSize = UNCOMPRESS_DATA_BUF_SIZE; //バッファのサイズを指定しておくのがコツ。処理終了後は実際の圧縮時のサイズがここに入っている。

	//圧縮
	CCompressUtil_::CompressData( compressSrcData , UNCOMPRESS_DATA_BUF_SIZE , compressDataBuffer , compressSize );

//----------------データを解凍してデータに入れる

	//解凍データ格納バッファ
	u8 uncompressDataBuffer[ UNCOMPRESS_DATA_BUF_SIZE ];
	u32 uncompressSize = UNCOMPRESS_DATA_BUF_SIZE; //バッファのサイズを指定しておくのがコツ。処理終了後は実際の圧縮時のサイズがここに入っている。

	//解凍
	CCompressUtil_::UnCompressData( compressDataBuffer , compressSize , uncompressDataBuffer , uncompressSize );


	SKY_BASE_LIB_TERM();
}

#endif