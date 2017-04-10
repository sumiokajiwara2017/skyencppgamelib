#include "StdAfx.h"
#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Memory.h"

namespace sky { namespace lib { namespace string {

CString::CString() :
m_bIsFix( skyFalse ) ,
m_nStrBufLen( 0 ) ,
m_nStrBufSize( 0 ) ,
m_pStrBuf( skyNull )
{
}

CString::CString( const skyWString *pStr) :
m_bIsFix( skyTrue ) ,
m_nStrBufLen( 0 ) ,
m_nStrBufSize( 0 ) ,
m_pStrBuf( skyNull )
{
#ifdef  SW_SKYLIB_MEMORY_TEXT_UNICODE
	_Create( pStr );
#else
	skyMString *cValue = CStringUtil_::CreateMString( src );
	_Create( pStr );
	CStringUtil_::DeleteMString( cValue );
#endif

}

CString::CString( const skyMString *pStr) :
m_bIsFix( skyTrue ) ,
m_nStrBufLen( 0 ) ,
m_nStrBufSize( 0 ) ,
m_pStrBuf( skyNull )
{
#ifdef  SW_SKYLIB_MEMORY_TEXT_UNICODE
	skyWString *cValue = CStringUtil_::CreateWString( pStr );
	_Create( cValue );
	CStringUtil_::DeleteWString( cValue );
#else
	_Create( pStr );
#endif
}

void CString::_Create( const skyString *pStr )
{
	if ( pStr == skyNull ) return;
	if ( CStringUtil_::LenByte( pStr ) == 0 ) return;

	m_nStrBufSize = CStringUtil_::LenByte( pStr ) + sizeof( skyString ); //文字終端分プラス
	m_nStrBufLen = CStringUtil_::Len( pStr );
	_MallocStrBuf( m_nStrBufSize );
	Clear();
	_Copy( pStr );
}

CString::~CString()
{
	Free();
}

void CString::Clear()
{
	CMemoryUtil_::ClearZero( m_pStrBuf , m_nStrBufSize );
}

const skyString* CString::Get() const
{
	return m_pStrBuf;
}

void CString::_MallocStrBuf( u32 size )
{
	m_pStrBuf = ( skyString* )SkyMalloc( size );
}

void CString::Free()
{
	if ( m_pStrBuf != skyNull )
	{
		SkyFree( m_pStrBuf );
	}
	m_bIsFix		= skyFalse;
	m_nStrBufLen	= 0;
	m_nStrBufSize	= 0;
	m_pStrBuf		= skyNull;

}

void CString::_Copy( const skyString *pStr )
{
	if ( CStringUtil_::LenByte( pStr ) == 0 ) return;

	u32 len = CStringUtil_::Len( pStr );
    SKY_ASSERT_MSG( m_nStrBufLen >= len , _T( "The character is too large. BufLen=[ %d ] , DataLen=[ %d ] , data=[ %s ]" ) , m_nStrBufLen , len , pStr );

	CStringUtil_::Copy( m_pStrBuf , len + 1 , pStr ); //末尾の空白も含めた文字数が第二引数なので、文字数と空白の+1でこれ。
}

void CString::_Cat( const skyString *pStr )
{
	if ( CStringUtil_::LenByte( pStr ) == 0 ) return;

	u32 nowLen  = CStringUtil_::Len( m_pStrBuf );
	u32 freeLen = m_nStrBufLen - nowLen;

	if ( freeLen == 0 ) return;

	CStringUtil_::Cat( m_pStrBuf , pStr );
}

CString& CString::operator =( const skyWString *pStr )
{
	//元々のエリアの削除
	Free();

#ifdef  SW_SKYLIB_MEMORY_TEXT_UNICODE
	//作成
	_Create( pStr );
#else
	skyMString *cValue = CStringUtil_::CreateMString( src );
	_Create( pStr );
	CStringUtil_::DeleteMString( cValue );
#endif

	return *this;
}

CString& CString::operator =( const skyMString *pStr )
{
#ifdef  SW_SKYLIB_MEMORY_TEXT_UNICODE
	skyWString *cValue = CStringUtil_::CreateWString( pStr );
	_Create( cValue );
	CStringUtil_::DeleteWString( cValue );
#else
	//作成
	_Create( pStr );
#endif

	return *this;
}
CString& CString::operator =( const CString& str )
{
	//元々のエリアの削除
	Free();

	//作成
	_Create( str.Get() );

	return *this;
}

skyBool CString::operator ==( const skyString c ) const
{
	//空白文字の比較
	if ( m_pStrBuf == skyNull && CStringUtil_::LenByte( &c ) == 0 ) return skyTrue;
	if ( CStringUtil_::LenByte( m_pStrBuf ) == 0 && CStringUtil_::LenByte( &c ) == 0 ) return skyTrue;

	return CStringUtil_::Comp( ( skyString * )m_pStrBuf , &c );
}
skyBool CString::operator ==( const skyString *pStr ) const
{
	//空白文字の比較
	if ( m_pStrBuf == skyNull && CStringUtil_::LenByte( pStr ) == 0 ) return skyTrue;
	if ( CStringUtil_::LenByte( m_pStrBuf ) == 0 && CStringUtil_::LenByte( pStr ) == 0 ) return skyTrue;

	return CStringUtil_::Comp( ( skyString * )m_pStrBuf , ( skyString * )pStr );
}
skyBool CString::operator ==( const CString& str ) const
{
	//空白文字の比較
	if ( m_pStrBuf == skyNull && CStringUtil_::LenByte( ( skyString * )str.m_pStrBuf ) == 0 ) return skyTrue;
	if ( CStringUtil_::LenByte( m_pStrBuf ) == 0 && CStringUtil_::LenByte( ( skyString * )str.m_pStrBuf ) == 0 ) return skyTrue;

	return CStringUtil_::Comp( ( skyString * )m_pStrBuf , ( skyString * )str.m_pStrBuf );
}
skyBool CString::operator !=( const skyString c ) const
{
	//空白文字の比較
	if ( m_pStrBuf == skyNull && CStringUtil_::LenByte( &c ) == 0 ) return skyFalse;
	if ( CStringUtil_::LenByte( m_pStrBuf ) == 0 && CStringUtil_::LenByte( &c ) == 0 ) return skyFalse;

	return !CStringUtil_::Comp( ( skyString * )m_pStrBuf , &c );
}
skyBool CString::operator !=( const skyString *pStr ) const
{
	//空白文字の比較
	if ( m_pStrBuf == skyNull && CStringUtil_::LenByte( pStr ) == 0 ) return skyFalse;
	if ( CStringUtil_::LenByte( m_pStrBuf ) == 0 && CStringUtil_::LenByte( pStr ) == 0 ) return skyFalse;

	return !CStringUtil_::Comp( ( skyString * )m_pStrBuf , ( skyString * )pStr );
}
skyBool CString::operator !=( const CString& str ) const
{
	//空白文字の比較
	if ( m_pStrBuf == skyNull && CStringUtil_::LenByte( ( skyString * )str.m_pStrBuf ) == 0 ) return skyFalse;
	if ( CStringUtil_::LenByte( m_pStrBuf ) == 0 && CStringUtil_::LenByte( ( skyString * )str.m_pStrBuf ) == 0 ) return skyFalse;

	return !CStringUtil_::Comp( ( skyString * )m_pStrBuf , ( skyString * )str.m_pStrBuf );
}

CString& CString::operator +=( const skyString *pStr )
{
	_Cat( pStr );
	return *this;
}
CString& CString::operator +=( const CString& str )
{
	_Cat( str.m_pStrBuf );
	return *this;
}

CString::operator const skyString*() const
{
	return Get();
}

void CString::ToLower()
{
	CStringUtil_::ConvToLower( m_pStrBuf );
}

} } }
