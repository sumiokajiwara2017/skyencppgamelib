#include "StdAfx.h"
#include "SkyBaseLib/Compress.h"
#include "SkyBaseLib/Debug.h"

namespace sky { namespace lib { namespace comp {

//-------------------------------< 実装 >----------------------------------------------

skyBool CCompressUtil::CompressFile( const u8 *pSrcData , u32 srcSize , const hash::CHashStringPtr &pDestPath , const skyMString *pOpenMode )
{
	skyMString *pPath = CStringUtil_::CreateMString( pDestPath->GetString() );

	// 出力先圧縮ファイルをオープンする.
	gzFile outFile = gzopen( pPath , pOpenMode );

	if ( outFile == NULL )
	{
		SKY_PANIC_MSG( _T( "The compression file did not open it at the output destination." ) ); //出力先圧縮ファイルが開けませんでした。
		return skyFalse;
	}

	// データを圧縮して出力ファイルに書き込む
	u32 writeLen = gzwrite( outFile, ( void * )pSrcData , srcSize );

	if ( writeLen != srcSize )
	{
		SKY_PANIC_MSG( _T( "Writing failed in the data compressed file. " ) ); //圧縮ファイルへ書き込みが失敗しました。
		return skyFalse; 
	}

	// 出力先圧縮ファイルを閉じる
	s32 ret = gzclose(outFile);

	if ( ret != Z_OK )
	{
		if ( ret == Z_MEM_ERROR )
		{
			SKY_PANIC_MSG( _T( "The data compressed file was not able to be shut." ) ); //圧縮ファイルを閉じれませんでした。メモリ不足で malloc() できなかった
		}
		else if ( ret == Z_MEM_ERROR )
		{
			SKY_PANIC_MSG( _T( "The data compressed file was not able to be shut." ) ); //圧縮ファイルを閉じれませんでした。出力バッファのサイズ不足
		}
		return skyFalse;
	}

	CStringUtil_::DeleteMString( pPath );

	return skyTrue;
}
skyBool CCompressUtil::CompressData( const u8 *pSrcData , u32 srcSize , u8* pDestData , u32 &destSize )
{
	//圧縮する
	s32 ret = compress( pDestData , &destSize , pSrcData , srcSize );

	if ( ret != Z_OK )
	{
		if ( ret == Z_MEM_ERROR )
		{
			SKY_PANIC_MSG( _T( "The data compressed file was not able to be shut." ) ); //圧縮に失敗しました。メモリ不足で malloc() できなかった
		}
		else if ( ret == Z_MEM_ERROR )
		{
			SKY_PANIC_MSG( _T( "The data compressed file was not able to be shut." ) ); //圧縮に失敗しました。出力バッファのサイズ不足
		}
		return skyFalse;
	}

	return skyTrue;
}
skyBool CCompressUtil::UnCompressFile( const u8 *pDestData , u32 destBufSize , u32 &destReadSize , const hash::CHashStringPtr &pSrcPath , const skyMString *pOpenMode )
{
	skyMString *pPath = CStringUtil_::CreateMString( pSrcPath->GetString() );

	// 出力先圧縮ファイルをオープンする.
	gzFile inFile = gzopen( pPath , pOpenMode );

	if ( inFile == NULL )
	{
		SKY_PANIC_MSG( _T( "The compression file did not open it at the output destination." ) );
		return skyFalse;
	}

	s32 ret;
	while( (ret = gzread( inFile, ( void *)pDestData , destBufSize ) ) != 0 && ret != -1)
	{
		//読み込みサイズを計算
		destReadSize += ret;
	}

	if(ret == -1)
	{
		// gzerrorでエラーメッセージを取得する
		const skyMString *gzerr = gzerror( inFile, ( int * )&ret );
		skyString *errMsg;

		skyString *msg = _T( "圧縮ファイルの解凍読込に失敗しました。 errorMsg=%s" );
		SKY_UNUSED_ARG( msg );
		//エラーメッセージとれた？
		if ( ret == Z_ERRNO )
		{
			//標準エラーを出す
			#define ERR_MSG_SIZE	256
			skyString errMsgBuf[ ERR_MSG_SIZE ];
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
			_tcserror_s( errMsgBuf , ERR_MSG_SIZE );
			SKY_PANIC_MSG( msg , errMsgBuf );
#else
			SKY_PANIC_MSG( msg );
#endif
		}
		else
		{
			errMsg = CStringUtil_::CreateString( gzerr );
			SKY_PANIC_MSG( msg , errMsg );
			CStringUtil_::DeleteString( errMsg );
		}
		return skyFalse;
	}

	if ( ( ret = gzclose( inFile ) ) != Z_OK )
	{
		if ( ret == Z_MEM_ERROR )
		{
			SKY_PANIC_MSG( _T( "The data compressed file was not able to be shut." ) ); //圧縮ファイルを閉じれませんでした。メモリ不足で malloc() できなかった
		}
		else if ( ret == Z_MEM_ERROR )
		{
			SKY_PANIC_MSG( _T( "The data compressed file was not able to be shut." ) ); //圧縮ファイルを閉じれませんでした。出力バッファのサイズ不足
		}
		return skyFalse;
	}

	return skyTrue;
}
skyBool CCompressUtil::UnCompressData( const u8 *pSrcData , u32 srcSize , u8* pDestData , u32 &destSize )
{
	s32 ret = uncompress( pDestData , &destSize , pSrcData , srcSize );

	if ( ret != Z_OK )
	{
		if ( ret == Z_MEM_ERROR )
		{
			SKY_PANIC_MSG( _T( "It failed in the decompression." ) ); //解凍に失敗しました。メモリ不足で malloc() できなかった
		}
		else if ( ret == Z_MEM_ERROR )
		{
			SKY_PANIC_MSG( _T( "It failed in the decompression." ) ); //解凍に失敗しました。出力バッファのサイズ不足
		}
		return skyFalse;
	}

	return skyTrue;
}

} } }