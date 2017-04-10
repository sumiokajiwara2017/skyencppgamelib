#pragma once

namespace sky { namespace lib { namespace comp {

/// <summary>
/// ■□■圧縮のためにzlibを使用しています■□■
/// デフォルト圧縮モード(fopenのモード + 圧縮レベル + 圧縮方式 )
///
/// 【fopenのモード】
/// rb : バイナリ読み込みモード
/// wb : バイナリ書き込みモード
///
/// 【圧縮レベル】
/// 0 : 圧縮なし
/// 1 : 処理速度重視
/// 9 : 圧縮効率重視
////
/// 【圧縮レベル】
/// f : フィルタ p
/// h : ハフマン符号化のみ
/// R : ランレングス
///
/// 解答後のファイルサイズを事前に取る方法が今のところありません。
/// データを解答する各アプリ処理において、最大値をそれぞれ仮定して使ってください。
/// </summary>
static const skyMString *DEFAULT_WRITE_GZ_MODE = "wb6f";
static const skyMString *DEFAULT_READ_GZ_MODE = "rb";

/// <summary>
/// Zlibユーティリティ関数群
/// </summary>
namespace CCompressUtil
{
	/// <summary>
	/// 指定のデータを圧縮
	/// </summary>
	skyBool CompressFile( const u8 *pSrcData , u32 srcSize , const hash::CHashStringPtr &pDestPath , const skyMString *pWriteOpenMode = DEFAULT_WRITE_GZ_MODE );
	skyBool CompressData( const u8 *pSrcData , u32 srcSize , u8* pDestData , u32 &destSize );

	/// <summary>
	/// 指定の圧縮データを解凍
	/// </summary>
	skyBool UnCompressFile( const u8 *pDestData , u32 destBufSize , u32 &destReadSize , const hash::CHashStringPtr &pSrcPath , const skyMString *pReadOpenMode = DEFAULT_READ_GZ_MODE );
	skyBool UnCompressData( const u8 *pSrcData , u32 srcSize , u8* pDestData , u32 &destSize );
}

#define CCompressUtil_					sky::lib::comp::CCompressUtil

} } }