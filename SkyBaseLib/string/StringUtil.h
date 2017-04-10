#pragma once

namespace sky { namespace lib { namespace string {

//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
/// 文字列処理群
/// inline化してもよかったんですが、他のライブラリにリンクができなくなるのでstaticにしました。
/// プラットフォーム毎の差は関数内で#defineで区切って解決します。
/// 結局そのほうが工数が低くて実行速度が出る。ただソースは汚くなる。
/// Unicode版とマルチバイト版の両方を用意しているのは、Unicodeプロジェクトでもマルチバイトにしか対応
/// していない関数等に対応するためです。_t系の処理なら両方やってくれるので普通それだけでいいんですが。
/// </summary>
class CStringUtil : public base::SkyObject
{

public:

	/// <summary>
	/// 文字列のコピー
	/// </summary>
	static skyWString* Copy( out__ skyWString *pDest , u32 destLen , in__ const skyWString *pSrc );
	static skyMString* Copy( out__ skyMString *pDest , u32 destLen , in__ const skyMString *pSrc );

	/// <summary>
	/// 文字列の長さを取得する(文字数）
	/// </summary>
	static u32 Len( in__ const skyWString *pStr );
	static u32 Len( in__ const skyMString *pStr );

	/// <summary>
	/// 文字列の長さを取得する(バイト数+終端子用byte(charなら1,wcharなら2)を勝手に換算）
	/// </summary>
	static u32 LenByte( in__ const skyWString *pStr );
	static u32 LenByte( in__ const skyMString *pStr );

	/// <summary>
	/// 指定の文字列が２バイト文字だった場合の長さを取得する
	/// </summary>
	static u32 LenChar( in__ const skyWString *pStr );

	/// <summary>
	/// 文字列を比較する
	/// </summary>
	static skyBool Comp( in__ const skyWString *pStrA, in__ const skyWString *pStrB );
	static skyBool Comp( in__ const skyMString *pStrA, in__ const skyMString *pStrB );
	static skyBool NComp( in__ const skyWString *pStrA, in__ const skyWString *pStrB , u32 num );
	static skyBool NComp( in__ const skyMString *pStrA, in__ const skyMString *pStrB , u32 num );

	/// <summary>
	/// pSrc文字列をpDest文字列に結合する
	/// </summary>
	static skyWString* Cat( inout__ skyWString *pDest , in__ const skyWString *pSrc );
	static skyMString* Cat( skyMString *pDest , u32 destByte , const skyMString *pSrc );

	/// <summary>
	/// pSrc文字列をpDest文字列に結合する(間にスペースをつける）
	/// </summary>
	static skyWString* CatSpace( inout__ skyWString *pDest , in__ const skyWString *pSrc );
	static skyMString* CatSpace( inout__ skyMString *pDest , u32 destByte , in__ const skyMString *pSrc );

	/// <summary>
	/// 指定の文字列において、指定の字句が最初に見つかった位置のポインターを返す。見つからなかったときNULLを返す
	/// </summary>
	static skyWString* Strchr( const in__ skyWString *pSrcString , in__ const skyWString pMark );
	static skyMString* Strchr( const in__ skyMString *pSrcString , in__ const skyMString pMark );

	/// <summary>
	/// 指定の文字列において、指定の字句が最初に見つかった位置までの距離を返す。見つからなければskyFalseを返し、最後までの距離を返す。
	/// </summary>
	static skyBool StrchrDistance( const in__ skyWString *pSrcString , in__ const skyWString pMark , u32 &size );
	static skyBool StrchrDistance( const in__ skyMString *pSrcString , in__ const skyMString pMark , u32 &size );

	/// <summary>
	/// 指定の文字列において、指定の字句が存在するかどうかチェックする
	/// </summary>
	static skyBool IsStrchrIn( const in__ skyWString *pSrcString , in__ const skyWString pMark );
	static skyBool IsStrchrIn( const in__ skyMString *pSrcString , in__ const skyMString pMark );

	/// <summary>
	/// 指定の文字列において、指定の字句が最後に見つかった位置のポインターを返す
	/// </summary>
	static skyWString* Strrchr( const in__ skyWString *pSrcString , in__ const skyWString pMark );
	static skyMString* Strrchr( const in__ skyMString *pSrcString , in__ const skyMString pMark );

	/// <summary>
	/// 指定の文字列において、指定の字句が最後に見つかった位置のポインターを返す
	/// </summary>
	static skyWString *Sprintf( inout__ skyWString *pBuf , s32 bufSize , in__ const skyWString *format , ... );
	static skyMString *Sprintf( inout__ skyMString *pBuf , s32 bufSize , in__ const skyMString *format , ... );

	/// <summary>
	/// 文字を置換します。
	/// </summary>
	static skyBool	Replace( const skyString *src , const skyString *srcKeyWord , const skyString *srcReplaceWord , skyString *destBuf , u32 destBufSize , const skyString *start = _T( "<##" ) , const skyString *end = _T( "#>" ) );

	/// <summary>
	/// 大文字を小文字にして返す
	/// </summary>
	static void ConvToLower( inout__ skyWString *pStr );
	static void ConvToLower( inout__ skyMString *pStr );

	/// <summary>
	/// １６進数文字列をu32型に変換して返す。
	/// </summary>
	static u32 ConvHexStrTou32( const skyWString *pSrc );
	static u32 ConvHexStrTou32( const skyMString *pSrc );

	/// <summary>
	/// 小文字を大文字にして返す
	/// </summary>
	static void ConvToUpper( inout__ skyWString *pStr );
	static void ConvToUpper( inout__ skyMString *pStr );

	/// <summary>
	/// 文字列をs32型に変換して返す。変換に失敗したらINT_MAXを返します
	/// </summary>
	static s32 ConvStringTos32( in__ const skyWString *pSrc , s32 baseNum = 10 );
	static s32 ConvStringTos32( in__ const skyMString *pSrc , s32 baseNum = 10 );

	/// <summary>
	/// 文字列をu32型に変換して返す。変換に失敗したらUINT_MAXを返します
	/// </summary>
	static u32 ConvStringTou32( in__ const skyWString *pSrc , s32 baseNum = 10 );
	static u32 ConvStringTou32( in__ const skyMString *pSrc , s32 baseNum = 10 );

	/// <summary>
	/// 文字列をf32型に変換して返す。変換に失敗したらFLT_MAXを返します
	/// </summary>
	static f32 ConvStringTof32( in__ const skyWString *pSrc );
	static f32 ConvStringTof32( in__ const skyMString *pSrc );

	/// <summary>
	/// 文字列をd64型に変換して返す。変換に失敗したらDBL_MAXを返します
	/// </summary>
	static d64 ConvStringTod64( in__ const skyWString *pSrc );
	static d64 ConvStringTod64( in__ const skyMString *pSrc );

	/// <summary>
	/// 文字列をskyBool型に変換して返す。変換に失敗したらskyFalseを返します
	/// </summary>
	static skyBool ConvStringToBool( in__ const skyWString *pSrc );
	static skyBool ConvStringToBool( in__ const skyMString *pSrc );

	/// <summary>
	/// 数字を文字にして返す
	/// </summary>
	static const skyString *ConvNumToString( s16 value , skyString *pBuf , s32 bufSize );
	static const skyString *ConvNumToString( u16 value , skyString *pBuf , s32 bufSize );
	static const skyString *ConvNumToString( s32 value , skyString *pBuf , s32 bufSize );
	static const skyString *ConvNumToString( u32 value , skyString *pBuf , s32 bufSize );
	static const skyString *ConvNumToString( f32 value , skyString *pBuf , s32 bufSize );
	static const skyString *ConvNumToString( d64 value , skyString *pBuf , s32 bufSize );
	static const skyString *ConvNumToString( skyBool value , skyString *pBuf , s32 bufSize );

	/// <summary>
	/// 引数の文字列を環境変数展開した文字列に置換して返す。
	/// </summary>
	static void ConvEnvValue( const in__ skyWString *pSrc , out__ skyWString *pDest , u32 destSize );
	static void ConvEnvValue( const in__ skyMString *pSrc , out__ skyMString *pDest , u32 destSize );

	/// <summary>
	/// 指定の文字列中の指定の文字を指定の文字に置き換える
	/// </summary>
	static void Replace( inout__ skyWString *pStr , const skyWString *pSrc , const skyWString *pDest );
	static void Replace( inout__ skyMString *pStr , const skyMString *pSrc , const skyMString *pDest );

	/// <summary>
	/// マルチバイト文字列をワイド文字列に変換する
	/// 実際このような文字変換をプログラム上で行うのよっぽどのことがない限り無いかと。
	/// この関数はシンプルな分危険です。仕組みを理解した上で使用して下さい。
	/// </summary>
	static u32 ConvMCharToWCharLen( const skyMString *pSrc );
	static u32 ConvMCharToWCharSize( const skyMString *pSrc );
	static u32 ConvMCharToWChar( in__ const skyMString *pSrc , inout__ skyWString *pDest , in__ u32 destSize );

	/// <summary>
	/// ワイド文字列をマルチバイト文字列に変換する
	/// 実際このような文字変換をプログラム上で行うのよっぽどのことがない限り無いかと。
	/// この関数はシンプルな分危険です。仕組みを理解した上で使用して下さい。
	/// </summary>
	static u32 ConvWCharToMCharSize( const skyWString *pSrc );
	static u32 ConvWCharToMChar( in__ const skyWString *pSrc , inout__ skyMString *pDest , in__ u32 destSize );

	/// <summary>
	/// 指定の文字列において、指定の文字列が存在するかチェックを行う
	/// </summary>
	static skyBool IsStrstrIn( const in__ skyWString *pSrcString , in__ const skyWString *pMark );
	static skyBool IsStrstrIn( const in__ skyMString *pSrcString , in__ const skyMString *pMark );

	/// <summary>
	/// 指定の文字列において指定の文字列が最初に現れるアドレスを返す
	/// </summary>
    static const skyWString *Strstr( const in__ skyWString *pSrcString , in__ const skyWString *pMark );
    static const skyMString *Strstr( const in__ skyMString *pSrcString , in__ const skyMString *pMark );

	/// <summary>
	/// 指定の文字列において、指定の文字列が最初に見つかった位置までの距離を返す。見つからなければ最後までの距離を返す。
	/// </summary>
	static u32 StrstrDistance( const in__ skyWString *pSrcString , in__ const skyWString *pMark );
	static u32 StrstrDistance( const in__ skyMString *pSrcString , in__ const skyMString *pMark );

	/// <summary>
	/// 文字列を変換し、出力する。変換後文字列はヒープを使用しているので必ず解放すること
	/// </summary>
	static skyMString	*CreateMString( const skyWString *pSrc );
	static skyMString	*CreateMString( const skyMString *pSrc );
	static void			DeleteMString( skyMString *pSrc );

	static skyWString	*CreateWString( const skyWString *pSrc );
	static skyWString	*CreateWString( const skyMString *pSrc );
	static void			DeleteWString( skyWString *pSrc );

	static skyString	*CreateString( const skyWString *pSrc );
	static skyString	*CreateString( const skyMString *pSrc );
	static void			DeleteString( skyString *pSrc );

	/// <summary>
	/// プログラムの文字コードがUnicodeかどうか判定する
	/// </summary>
	static skyBool IsProgramUnicode			();

	/// <summary>
	/// 文字が0から9までのいずれかの値であればゼロでない値を返すisdigitの機能を利用して１文字を数字かどうか判定する
	/// </summary>
	static skyBool      IsNum( const skyString c );

	/// <summary>
	/// 文字を数字に変換する
	/// </summary>
	static s32          ConvertNum( const skyString c );

	/// <summary>
	/// 指定のデータの末尾に終端子を付けつつ指定のバッファにコピーする
	/// </summary>
	static void         CopyAdde0( skyWString *pDest , u32 destLen , const skyWString *src , u32 srcLen );
	static void         CopyAdde0( skyMString *pDest , u32 destLen , const skyMString *src , u32 srcLen );

private:

	//各機能封印
	CStringUtil(){};													//コンストラクタ
	CStringUtil( const CStringUtil& ){};								//コピーコンストラクタ
	virtual ~CStringUtil(){};											//デストラクタ
	CStringUtil &operator=(const CStringUtil& ){ return *this; };		//代入演算子オーバーロード
};

#define CStringUtil_			sky::lib::string::CStringUtil

} } }

#include "StringUtil.inl"