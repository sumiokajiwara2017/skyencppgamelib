#include "StdAfx.h"
#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Memory.h"

#include <locale.h>
#include <stdio.h>
#include <string.h> 

#pragma warning(disable: 4793)	//マネージでは使わないので問題ない

namespace sky { namespace lib { namespace string {


//-------------------------------------< 実装 >---------------------------------------------//

skyWString* CStringUtil::Copy( skyWString *pDest , u32 destLen , const skyWString *pSrc )
{
	setlocale( LC_ALL , "ja" );  /* ロケールを日本語に設定 */

#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	wcscpy_s( pDest , destLen , pSrc );
#else
	wcscpy( pDest , pSrc );
#endif

	return pDest;
}
skyMString* CStringUtil::Copy( skyMString *pDest , u32 destLen , const skyMString *pSrc )
{
	setlocale( LC_ALL , "ja" );  /* ロケールを日本語に設定 */
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	strcpy_s( pDest , destLen , pSrc );
#else
	strcpy( pDest , pSrc );
#endif
	return pDest;
}

u32 CStringUtil::Len( const skyWString *pStr )
{
	setlocale( LC_ALL , "ja" );  /* ロケールを日本語に設定 */
	return wcslen( pStr );
}
u32 CStringUtil::Len( const skyMString *pStr )
{
	return strlen( pStr );
}

u32 CStringUtil::LenByte( in__ const skyWString *pStr )
{
	return Len( pStr ) * sizeof( skyWString );
}
u32 CStringUtil::LenByte( in__ const skyMString *pStr )
{
	return Len( pStr ) * sizeof( skyMString );
}

u32 CStringUtil::LenChar( const skyWString *pStr )
{
	return WideCharToMultiByte( CP_THREAD_ACP , 0 , pStr , -1 , skyNull , 0 , skyNull , skyNull );
}

skyBool CStringUtil::Comp( const skyWString *pStrA , const skyWString *pStrB )
{
	return ( wcscmp( pStrA , pStrB ) == 0 );
}
skyBool CStringUtil::Comp( const skyMString *pStrA, const skyMString *pStrB )
{
	return ( strcmp( pStrA , pStrB ) == 0 );
}

skyBool CStringUtil::NComp( const skyWString *pStrA , const skyWString *pStrB , u32 num )
{
	return ( wcsncmp( pStrA , pStrB , num ) == 0 );
}
skyBool CStringUtil::NComp( const skyMString *pStrA, const skyMString *pStrB , u32 num )
{
	return ( strncmp( pStrA , pStrB , num ) == 0 );
}

skyWString* CStringUtil::Cat( skyWString *pDest , const skyWString *pSrc )
{
	return lstrcat( pDest , pSrc );
}
skyMString* CStringUtil::Cat( skyMString *pDest , u32 destByte , const skyMString *pSrc )
{
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	strcat_s( pDest , destByte , pSrc );
#else
	strcat( pDest , pSrc );
#endif
	return pDest;
}

skyWString* CStringUtil::CatSpace( skyWString *pDest , const skyWString *pSrc )
{
	if ( Len( pDest ) > 0 )
	{
		Cat( pDest , L" " );
	}
	return Cat( pDest , pSrc );
}
skyMString* CStringUtil::CatSpace( skyMString *pDest , u32 destByte , const skyMString *pSrc )
{
	if ( Len( pDest ) > 0 )
	{
		Cat( pDest , destByte , " " );
	}
	return Cat( pDest , destByte , pSrc );
}

skyWString *CStringUtil::Sprintf( skyWString *pBuf , s32 bufSize , const skyWString *format , ... )
{
	va_list	va;									
	va_start( va , format );						
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	vswprintf_s( pBuf , bufSize , format , va );	
#else
	vswprintf( pBuf , format , va );
#endif
	va_end( va );

	return pBuf;
}
skyMString *CStringUtil::Sprintf( skyMString *pBuf , s32 bufSize , const skyMString *format , ... )
{
	va_list	va;									
	va_start( va , format );						
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	vsprintf_s( pBuf , bufSize , format, va );	
#else
	vsprintf( pBuf , format, va );	
#endif
	va_end( va );								

	return pBuf;
}

skyBool CStringUtil::IsStrchrIn( const in__ skyWString *pSrcString , in__ const skyWString pMark )
{
	skyBool result = skyFalse;

	setlocale( LC_ALL , "ja" );  /* ロケールを日本語に設定 */

	skyWString *pRs = ( skyWString * )wcschr( pSrcString , pMark );

	if ( pRs != skyNull )
	{
		result = skyTrue;
	}

	return result;
}

skyBool CStringUtil::IsStrchrIn( const in__ skyMString *pSrcString , in__ const skyMString pMark )
{
	skyBool result = skyFalse;

	skyMString *pRs = ( skyMString * )strchr( pSrcString , pMark );

	if ( pRs != skyNull )
	{
		result = skyTrue;
	}

	return result;
}

skyWString* CStringUtil::Strrchr( const skyWString *pSrcString , const skyWString pMark )
{
	return ( skyWString * )wcsrchr( pSrcString , pMark );
}
skyMString* CStringUtil::Strrchr( const skyMString *pSrcString , const skyMString pMark )
{
	return ( skyMString* )strrchr( pSrcString , pMark );
}

skyWString* CStringUtil::Strchr( const skyWString *pSrcString , const skyWString pMark )
{
	return ( skyWString * )wcschr( pSrcString , pMark );
}
skyMString* CStringUtil::Strchr( const skyMString *pSrcString , const skyMString pMark )
{
	return ( skyMString * )strchr( pSrcString , pMark );
}

skyBool CStringUtil::StrchrDistance( const in__ skyWString *pSrcString , in__ const skyWString pMark , u32 &size )
{
	setlocale( LC_ALL , "ja" );  /* ロケールを日本語に設定 */

	skyWString *pRs = ( skyWString * )wcschr( pSrcString , pMark );

	if ( pRs != skyNull )
	{
		size = ( u32 )( pRs - pSrcString );
		return skyTrue;
	}
	else
	{
		size = Len( pSrcString );
		return skyFalse;
	}
}
skyBool CStringUtil::StrchrDistance( const in__ skyMString *pSrcString , in__ const skyMString pMark , u32 &size )
{
	skyMString *pRs = ( skyMString * )strchr( pSrcString , pMark );

	if ( pRs != skyNull )
	{
		size = pSrcString - pRs;
		return skyTrue;
	}
	else
	{
		size = Len( pSrcString );
		return skyFalse;
	}
}

const skyString *CStringUtil::ConvNumToString( s16 value , skyString *pBuf , s32 bufSize )
{
	Sprintf( pBuf , bufSize , _T( "%d" ) , value );
	return pBuf;
}
const skyString *CStringUtil::ConvNumToString( u16 value , skyString *pBuf , s32 bufSize )
{
	Sprintf( pBuf , bufSize , _T( "%d" ) , value );
	return pBuf;
}
const skyString *CStringUtil::ConvNumToString( s32 value , skyString *pBuf , s32 bufSize )
{
	Sprintf( pBuf , bufSize , _T( "%d" ) , value );
	return pBuf;
}
const skyString *CStringUtil::ConvNumToString( u32 value , skyString *pBuf , s32 bufSize )
{
	Sprintf( pBuf , bufSize , _T( "%d" ) , value );
	return pBuf;
}
const skyString *CStringUtil::ConvNumToString( f32 value , skyString *pBuf , s32 bufSize )
{
	Sprintf( pBuf , bufSize , _T( "%f" ) , value );
	return pBuf;
}
const skyString *CStringUtil::ConvNumToString( d64 value , skyString *pBuf , s32 bufSize )
{
	Sprintf( pBuf , bufSize , _T( "%f" ) , value );
	return pBuf;
}
const skyString *CStringUtil::ConvNumToString( skyBool value , skyString *pBuf , s32 bufSize )
{
	Sprintf( pBuf , bufSize , _T( "%d" ) , value );
	return pBuf;
}

u32 CStringUtil::ConvHexStrTou32( const skyMString *pSrc )
{
    s16 i = 0;        /* 配列の添字として使用 */
    s16 n = 0;
    u32 x = 0;
    u8 c;

	//頭2文字が0xでなかった場合エラー
        SKY_ASSERT_MSG( IsStrstrIn( pSrc , "0x" ) , L"The character of 0x is not included." ); //0xという文字が含まれていません。
	SKY_ASSERT_MSG( StrstrDistance( pSrc , "0x" ) == 0 , _T( "There is no character of 0x in the head." ) ); //0xという文字が先頭にはありません。

	const skyMString *pParseStr = pSrc + 2;

    while ( pParseStr[ i ] != MEOT )
	{//文字列の末尾でなければ

        //'0' から '9' の文字なら
        if ( '0' <= pParseStr[ i ] && pParseStr[ i ] <= '9' )
		{
            n = pParseStr[ i ] - '0';        //数字に変換
		}
        // 'a' から 'f' の文字なら
        else if ( 'a' <= ( c = ( u8 )tolower( pParseStr[ i ] ) ) && c <= 'f' )
		{
            n = c - 'a' + 10;//数字に変換
		}
        else
		{// それ以外の文字なら
            SKY_PANIC_MSG( _T("This character is not a hexadecimal number. %c") , pParseStr[ i ] ); //16進数文字列を変換中、想定外の文字が現れました
        }
        i++;			//次の文字を指す

        x = x * 16 + n; // 桁上がり
    }

	return x;
}

u32 CStringUtil::ConvHexStrTou32( const skyWString *pSrc )
{
	//一旦charにしてしまう。
	u32 bufSize = ConvWCharToMCharSize( pSrc );
	skyMString *pBuf = NEW_ARRAY__( skyMString , bufSize );
	ConvWCharToMChar( pSrc , pBuf , bufSize );
	u32 result = ConvHexStrTou32( pBuf );
	DEL pBuf;
	return result;
}

s32 CStringUtil::ConvStringTos32( in__ const skyWString *pSrc , s32 baseNum )
{//errnoはなんとスレッドセーフらしい！
	errno = 0;	//大域変数errnoに０を設定。この変数の実体は<errno.h>にある。
	skyWString *pEndptr = skyNull;

	s32 result = wcstol( pSrc , &pEndptr , baseNum ); 
	if ( errno != 0 || *pEndptr != MEOT )
	{
		//コンバートエラー
		result = INT_MAX;
	}
	return result;
}
s32 CStringUtil::ConvStringTos32( in__ const skyMString *pSrc , s32 baseNum )
{//errnoはなんとスレッドセーフらしい！
	errno = 0;	//大域変数errnoに０を設定。この変数の実体は<errno.h>にある。
	skyMString *pEndptr = skyNull;

	s32 result = strtol( pSrc , &pEndptr , baseNum ); 
	if ( errno != 0 || *pEndptr != MEOT )
	{
		//コンバートエラー
		result = INT_MAX;
	}
	return result;
}

u32 CStringUtil::ConvStringTou32( in__ const skyWString *pSrc , s32 baseNum )
{//errnoはなんとスレッドセーフらしい！
	errno = 0;	//大域変数errnoに０を設定。この変数の実体は<errno.h>にある。
	skyWString *pEndptr = skyNull;

	u32 result = wcstoul( pSrc , &pEndptr , baseNum ); 
	if ( errno != 0 || *pEndptr != MEOT )
	{
		//コンバートエラー
		result = UINT_MAX;
	}
	return result;
}

u32 CStringUtil::ConvStringTou32( in__ const skyMString *pSrc , s32 baseNum )
{//errnoはなんとスレッドセーフらしい！
	errno = 0;	//大域変数errnoに０を設定。この変数の実体は<errno.h>にある。
	skyMString *pEndptr = skyNull;

	u32 result = strtoul( pSrc , &pEndptr , baseNum ); 
	if ( errno != 0 || *pEndptr != MEOT )
	{
		//コンバートエラー
		result = UINT_MAX;
	}
	return result;
}

f32 CStringUtil::ConvStringTof32( in__ const skyWString *pSrc )
{
	return ( f32 )ConvStringTod64( pSrc );
}
f32 CStringUtil::ConvStringTof32( in__ const skyMString *pSrc )
{
	return ( f32 )ConvStringTod64( pSrc );
}

d64 CStringUtil::ConvStringTod64( in__ const skyWString *pSrc )
{//errnoはなんとスレッドセーフらしい！
	errno = 0;	//大域変数errnoに０を設定。この変数の実体は<errno.h>にある。
	skyWString *pEndptr = skyNull;

	d64 result = wcstod( pSrc , &pEndptr ); 
	if ( errno != 0 || *pEndptr != MEOT )
	{
		//コンバートエラー
		result = DBL_MAX;
	}
	return result;
}
d64 CStringUtil::ConvStringTod64( in__ const skyMString *pSrc )
{//errnoはなんとスレッドセーフらしい！

	errno = 0;	//大域変数errnoに０を設定。この変数の実体は<errno.h>にある。
	skyMString *pEndptr = skyNull;

	d64 result = strtod( pSrc , &pEndptr ); 
	if ( errno != 0 || *pEndptr != MEOT )
	{
		//コンバートエラー
		result = DBL_MAX;
	}
	return result;
}

skyBool CStringUtil::ConvStringToBool( in__ const skyWString *pSrc )
{
	u32 value = ConvStringTou32( pSrc );

	if( value == 0 || value == UINT_MAX )
	{
		return skyFalse;
	}
	else
	{
		return skyTrue;
	}
}
skyBool CStringUtil::ConvStringToBool( in__ const skyMString *pSrc )
{
	u32 value = ConvStringTou32( pSrc );

	if( value == 0 || value == UINT_MAX )
	{
		return skyFalse;
	}
	else
	{
		return skyTrue;
	}
}

void CStringUtil::ConvToLower( inout__ skyWString *pStr )
{
	u32 len = Len( pStr );
	for ( u32 i = 0 ; i < len ; i++ )
	{
		pStr[ i ] = towlower( pStr[ i ] );
	}
}
void CStringUtil::ConvToLower( inout__ skyMString *pStr )
{
	u32 len = Len( pStr );
	for ( u32 i = 0 ; i < len ; i++ )
	{
		pStr[ i ] = ( skyMString )tolower( pStr[ i ] );
	}
}

void CStringUtil::ConvToUpper( inout__ skyWString *pStr )
{
	u32 len = Len( pStr );
	for ( u32 i = 0 ; i < len ; i++ )
	{
		pStr[ i ] = _totupper( pStr[ i ] );
	}
}
void CStringUtil::ConvToUpper( inout__ skyMString *pStr )
{
	u32 len = Len( pStr );
	for ( u32 i = 0 ; i < len ; i++ )
	{
		pStr[ i ] = ( skyMString )toupper( pStr[ i ] );
	}
}

void CStringUtil::ConvEnvValue( const in__ skyWString *pSrc , out__ skyWString *pDest , u32 destSize )
{
	ExpandEnvironmentStringsW( pSrc,pDest , destSize );
}
void CStringUtil::ConvEnvValue( const in__ skyMString *pSrc , out__ skyMString *pDest , u32 destSize )
{
	ExpandEnvironmentStringsA( pSrc,pDest , destSize );
}

void CStringUtil::Replace( inout__ skyWString *pStr , const skyWString *pSrc , const skyWString *pDest )
{
	for ( u32 i = 0 ; i < Len( pStr ) ; i++ )
	{
		if ( pStr[ i ] == *pSrc )
		{
			pStr[ i ] = *pDest;
		}
	}
}
void CStringUtil::Replace( inout__ skyMString *pStr , const skyMString *pSrc , const skyMString *pDest )
{
	for ( u32 i = 0 ; i < Len( pStr ) ; i++ )
	{
		if ( pStr[ i ] == *pSrc )
		{
			pStr[ i ] = *pDest;
		}
	}
}

u32 CStringUtil::ConvMCharToWCharLen( const skyMString *pSrc )
{
	return MultiByteToWideChar( CP_ACP , 0 , pSrc , -1 , skyNull , skyNull );
}
u32 CStringUtil::ConvMCharToWCharSize( const skyMString *pSrc )
{
	return ConvMCharToWCharLen( pSrc ) * sizeof( skyWString );
}
u32 CStringUtil::ConvMCharToWChar( const skyMString *pSrc , skyWString *pDest , u32 destSize )
{
	u32 size = ConvMCharToWCharLen( pSrc );

	if ( destSize < size )
	{
		SKY_PANIC_MSG( _T( "The buffer where the value after it converts it is stored doesn't suffice." ) ); //変換後の値を格納するバッファが足りません。
	}

	// Char → Unicode
	MultiByteToWideChar(
		CP_ACP ,					// ANSI code page
		0 ,							// 既定の変換方法
		pSrc ,						// 変換元文字列
		-1 ,						// 変換元文字列サイズ(-1指定で自動計算)
		pDest ,						// 変換先バッファ
		destSize);					// 変換先バッファサイズ
	return size;
}

u32 CStringUtil::ConvWCharToMCharSize( const skyWString *pSrc )
{
	//変換後のサイズを取得
	return WideCharToMultiByte( CP_THREAD_ACP , 0 , pSrc , -1 , skyNull , 0 , skyNull , skyNull );
}

u32 CStringUtil::ConvWCharToMChar( const skyWString *pSrc , skyMString *pDest , u32 destSize )
{
	//変換後のサイズを取得
	u32 size = ConvWCharToMCharSize( ( skyWString * )pSrc );

	if ( destSize < size )
	{
		SKY_PANIC_MSG( _T( "The buffer where the value after it converts it is stored doesn't suffice." ) ); //変換後の値を格納するバッファが足りません。
	}

	// Unicode → UTF8
	WideCharToMultiByte(
		CP_THREAD_ACP ,				// コードページ
		0 ,							// 処理速度とマッピング方法を決定するフラグ
		pSrc ,						// ワイド文字列のアドレス
		-1 ,						// ワイド文字列の文字数(-1指定で自動計算)
		(skyMString *)pDest ,				// 新しい文字列を受け取るバッファのアドレス
		size ,						// 新しい文字列を受け取るバッファのサイズ
		skyNull ,					// マップできない文字の既定値のアドレス
		skyNull						// 既定の文字を使ったときにセットするフラグのアドレス
	);
	return size;
}

skyBool CStringUtil::IsStrstrIn( const in__ skyWString *pSrcString , in__ const skyWString *pMark )
{
	return ( wcsstr( pSrcString , pMark ) != skyNull );
}
skyBool CStringUtil::IsStrstrIn( const in__ skyMString *pSrcString , in__ const skyMString *pMark )
{
	return ( strstr( pSrcString , pMark ) != skyNull );
}
const skyWString *CStringUtil::Strstr( const in__ skyWString *pSrcString , in__ const skyWString *pMark )
{
	return wcsstr( pSrcString , pMark  );
}
const skyMString *CStringUtil::Strstr( const in__ skyMString *pSrcString , in__ const skyMString *pMark )
{
	return strstr( pSrcString , pMark );
}
u32 CStringUtil::StrstrDistance( const in__ skyWString *pSrcString , in__ const skyWString *pMark )
{
	setlocale( LC_ALL , "ja" );  /* ロケールを日本語に設定 */

	skyWString *pRs = ( skyWString * )wcsstr( pSrcString , pMark );

	u32 size = 0;
	if ( pRs != skyNull )
	{
		size = ( u32 )( pRs - pSrcString );
	}
	else
	{
		size = Len( pSrcString );
	}

	return size;
}
u32 CStringUtil::StrstrDistance( const in__ skyMString *pSrcString , in__ const skyMString *pMark )
{
	skyMString *pRs = ( skyMString * )strstr( pSrcString , pMark );

	u32 size = 0;
	if ( pRs != skyNull )
	{
		size = ( u32 )( pRs - pSrcString );
	}
	else
	{
		size = Len( pSrcString );
	}

	return size;
}

skyMString *CStringUtil::CreateMString( const skyWString *pSrc )
{
	skyMString *pResult = NULL;
	u32			size = 0;

	//コンバート後のサイズを取得
	size = ConvWCharToMCharSize( pSrc );

	//領域を確保
	pResult = ( skyMString * )SkyMalloc( size );
	CMemoryUtil_::ClearZero( pResult , size );

	//コンバート
	ConvWCharToMChar( pSrc , pResult , size );

	//値を返す
	return pResult;
}

skyMString *CStringUtil::CreateMString( const skyMString *pSrc )
{
	skyMString *pResult = NULL;
	u32			size = 0;

	//サイズを取得
	size = LenByte( pSrc );

	//領域を確保
	pResult = ( skyMString * )SkyMalloc( size );
	CMemoryUtil_::ClearZero( pResult , size );

	//コピー
	Copy( pResult , size , pSrc );

	//値を返す
	return pResult;
}

void CStringUtil::DeleteMString( skyMString *pSrc )
{
	SkyFree( pSrc );
}

skyWString *CStringUtil::CreateWString( const skyWString *pSrc )
{
	skyWString *pResult = NULL;
	u32			size = 0;

	//サイズを取得
	size = LenByte( pSrc );

	//領域を確保
	pResult = ( skyWString * )SkyMalloc( size );
	CMemoryUtil_::ClearZero( pResult , size );

	//コピー
	CMemoryUtil_::Copy( pResult , pSrc , size );

	//値を返す
	return pResult;
}

skyWString *CStringUtil::CreateWString( const skyMString *pSrc )
{
	skyWString *pResult = NULL;
	u32			size = 0;

	//コンバート後のサイズを取得
	size = ConvMCharToWCharSize( pSrc );

	//領域を確保
	pResult = ( skyWString * )SkyMalloc( size );
	CMemoryUtil_::ClearZero( pResult , size );

	//コンバート
	ConvMCharToWChar( pSrc , pResult , size );

	//値を返す
	return pResult;
}


void CStringUtil::DeleteWString( skyWString *pSrc )
{
	SkyFree( pSrc );
}

skyString *CStringUtil::CreateString( const skyWString *pSrc )
{
#ifdef  SW_SKYLIB_MEMORY_TEXT_UNICODE
	return CreateWString( pSrc );
#else
	return CreateMString( pSrc )
#endif
}
skyString *CStringUtil::CreateString( const skyMString *pSrc )
{
#ifdef  SW_SKYLIB_MEMORY_TEXT_UNICODE
	return CreateWString( pSrc );
#else
	return CreateMString( pSrc )
#endif
}

void CStringUtil::DeleteString( skyString *pSrc )
{
	SkyFree( pSrc );
}

skyBool	CStringUtil::Replace( const skyString *src , const skyString *srcKeyWord , const skyString *srcReplaceWord , skyString *destBuf , u32 destBufSize , const skyString *start , const skyString *end )
{
	SKY_ASSERT_MSG( src != skyNull				, _T( "There is no required parameter. " ) );
	SKY_ASSERT_MSG( srcKeyWord != skyNull		, _T( "There is no required parameter. " ) );
	SKY_ASSERT_MSG( srcReplaceWord != skyNull	, _T( "There is no required parameter. " ) );
	SKY_ASSERT_MSG( destBuf != skyNull			, _T( "There is no required parameter. " ) );
	SKY_ASSERT_MSG( destBufSize > 0				, _T( "The size of the buffer is not good in 0." ) );

	//srcの文字数を取得
	u32 srclen = Len( src );

	SKY_ASSERT_MSG( destBufSize > srclen		, _T( "The size of the buffer doesn't suffice." ) ); //バッファサイズが変換前の文字列以下です。絶対に足りません。

	//開始文字の長さをとる
	u32 startlen = 0;
	if ( start != skyNull )
	{
		startlen = Len( start );
	}

	//終了文字の長さをとる
	u32 endlen = 0;
	if ( end != skyNull )
	{
		endlen = Len( end );
	}

	//キーワードの長さを取る
	u32 keyLen = Len( srcKeyWord );
	
	//置換後の文字の長さを取る
	u32 repLen = Len( srcReplaceWord );

	//srcを1文字ずつ回す
	skyBool startFound = skyFalse;
	skyBool endFound = skyFalse;
	skyBool keyFound = skyFalse;
	u32 setDestBufSize = 0;
	for ( u32 i = 0 ; i < srclen ; i++ )
	{
		//区切り文字があるか？
		if ( start != skyNull && end != skyNull )
		{
			if ( startFound == skyFalse )
			{
				//開始文字を探す
				if ( src[ i ] == start[ 0 ] )
				{//開始文字のようなものを見つけた
				
					startFound = skyTrue;
					for ( u32 j = 0 ; j < startlen ; j ++ )
					{
						if ( src[ i + j ] != start[ j ] )
						{
							startFound = skyFalse;
						}
					}

					//開始文字が見つかったらiを開始文字の文字数分進める
					if ( startFound == skyTrue )
					{
						i += startlen - 1; //for文で一個加算されるので-1しておく
						continue;
					}
				}
			}

			if ( startFound == skyTrue )
			{
				//終了文字を探す
				if ( src[ i ] == end[ 0 ] )
				{//終了文字のようなものを見つけた

					endFound = skyTrue;
					for ( u32 j = 0 ; j < endlen ; j ++ )
					{
						if ( src[ i + j ] != end[ j ] )
						{
							endFound = skyFalse;
						}
					}
					//開始文字が見つかったらiを開始文字の文字数分進める
					if ( endFound == skyTrue )
					{
						i += endlen - 1; //for文で一個加算されるので-1しておく
						endFound   = skyFalse;
						startFound = skyFalse;
						continue;
					}
				}
			}
		}
		
		//キーワードらしき値を探す
		if ( src[ i ] == srcKeyWord[ 0 ] )
		{
			keyFound = skyTrue;
			for ( u32 j = 0 ; j < keyLen ; j++ )
			{
				if ( src[ i + j ] != srcKeyWord[ j ] )
				{
					keyFound = skyFalse;
				}
			}

			if ( keyFound == skyTrue )
			{//キーワードが見つかった

				//キーの後ろが終了文字になっていなかったらエラー
				for ( u32 j = 0 ; j < endlen ; j ++ )
				{
					if ( src[ i + j + keyLen ] != end[ j ] )
					{
                        SKY_PANIC_MSG( _T( "Please write the terminal character after the key word( %s ). " ) , end ); //キーワードの後ろは終了文字%sを書いてください
					}
				}

				//destBufに置換後の文字を書きこむ
				for ( u32 j = 0 ; j < repLen ; j++ )
				{
					SKY_ASSERT_MSG( destBufSize > setDestBufSize , _T( "The output buffer doesn't suffice. " ) ); //出力バッファをオーバーしました。置換後の文字の長さが長すぎます。 
					destBuf[ setDestBufSize ] = srcReplaceWord[ j ];
					setDestBufSize++;
				}

				//キーワードの文字数分iを進める
				i += keyLen - 1; //for文で一個加算されるので-1しておく

				continue;
			}
		}

		SKY_ASSERT_MSG( destBufSize > setDestBufSize , _T( "The output buffer doesn't suffice. " ) ); //出力バッファをオーバーしました。置換後の文字の長さが長すぎます。
		destBuf[ setDestBufSize ] = src[ i ];
		setDestBufSize++;
	}

	SKY_ASSERT_MSG( destBufSize > setDestBufSize , _T( "The output buffer doesn't suffice. " ) ); //出力バッファをオーバーしました。置換後の文字の長さが長すぎます。
	destBuf[ setDestBufSize ] = EOT;
	return skyTrue; 
}

skyBool CStringUtil::IsProgramUnicode()
{
#ifdef UNICODE
	return skyTrue;
#else
	return skyFalse;
#endif
}

skyBool CStringUtil::IsNum( const skyString c )
{
	return isdigit( c );
}

s32 CStringUtil::ConvertNum( const skyString c )
{
	return _ttoi( &c );
}

void CStringUtil::CopyAdde0( skyWString *pDest , u32 destLen , const skyWString *src , u32 srcLen )
{
	SKY_ASSERT_MSG( destLen > srcLen + 1 , "dest size too small." );

	CMemoryUtil_::ClearValue( pDest , EOT , ( srcLen + 1 ) * sizeof( skyWString ) );
	CMemoryUtil_::Copy( pDest , src , srcLen * sizeof( skyWString ) );
}

void CStringUtil::CopyAdde0( skyMString *pDest , u32 destLen , const skyMString *src , u32 srcLen )
{
	SKY_ASSERT_MSG( destLen > srcLen + 1 , "dest size too small." );

	CMemoryUtil_::ClearValue( pDest , MEOT , srcLen + 1 );
	CMemoryUtil_::Copy( pDest , src , srcLen );
}

} } }
