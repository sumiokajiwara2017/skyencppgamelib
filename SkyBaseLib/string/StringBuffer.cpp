#include "StdAfx.h"
#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Memory.h"

namespace sky { namespace lib { namespace string {

CStringBuffer::CStringBuffer() :
CString()
{
}

CStringBuffer::CStringBuffer( const skyString *pStr ) :
CString( pStr )
{
}

CStringBuffer::~CStringBuffer()
{
}

CStringBuffer& CStringBuffer::operator =( const skyString *pStr )
{
	if ( CStringUtil_::LenByte( pStr ) == 0 ) return *this;

	//新しい文字列のサイズでバッファを取り直してコピーする
	u32 size = CStringUtil_::LenByte( pStr );
	if ( size != m_nStrBufSize )
	{
		//既存を開放
		Free();

		//新しい領域を確保
		m_pStrBuf = ( skyString * )SkyMalloc( size );

		//新しい領域サイズを保存
		m_nStrBufSize = size;
		m_nStrBufLen = CStringUtil_::Len( pStr );
		*m_pStrBuf = '\0';
	}

	//新しい領域にコピー
	_Copy( pStr );

	return *this;
}
CStringBuffer& CStringBuffer::operator =( const CStringBuffer &str )
{
	if ( CStringUtil_::LenByte( str.m_pStrBuf ) == 0 ) return *this;

	//新しい文字列のサイズでバッファを取り直してコピーする
	u32 size = CStringUtil_::LenByte( str.m_pStrBuf );
	if ( size != m_nStrBufSize )
	{
		//既存を開放
		Free();

		//新しい領域を確保
		m_pStrBuf = ( skyString* )SkyMalloc( size );

		//新しい領域サイズを保存
		m_nStrBufSize = size;
		m_nStrBufLen = CStringUtil_::Len( str.m_pStrBuf );
		*m_pStrBuf = '\0';
	}

	//新しい領域にコピー
	_Copy( str.m_pStrBuf );

	return *this;
}

CStringBuffer& CStringBuffer::operator +=( const skyString *pStr )
{
	if ( CStringUtil_::LenByte( pStr ) == 0 ) return *this;

	//新しい文字列を足したイズでバッファを取り直してコピーする
	u32 catStrSize = CStringUtil_::LenByte( pStr );

	//新しいサイズを計算する
	u32 newSize = catStrSize + m_nStrBufSize;
	if ( m_nStrBufSize == 0 ) newSize += sizeof( skyString );
	u32 newLen	= CStringUtil_::Len( pStr ) + m_nStrBufLen;

	//新しいサイズで領域を確保する
	skyString *pNewBuffer = ( skyString* )SkyMalloc( newSize );

	//新しい領域をクリア
	CMemoryUtil_::ClearZero( pNewBuffer, newSize );

	//新しい領域に古い情報をコピー
	if ( m_pStrBuf != skyNull )
	{
		CStringUtil_::Copy( pNewBuffer, newLen, m_pStrBuf );
	}

	//古い領域をフリー
	Free();

	//新しい情報に更新
	m_nStrBufSize = newSize;
	m_nStrBufLen = newLen;
	m_pStrBuf = pNewBuffer;

	//新しい領域を連結
	_Cat( pStr ); 

	return *this;
}
CStringBuffer& CStringBuffer::operator +=( const CStringBuffer &str )
{
	if ( CStringUtil_::LenByte( str.m_pStrBuf ) == 0 ) return *this;

	//新しい文字列を足したイズでバッファを取り直してコピーする
	u32 catStrSize = CStringUtil_::LenByte( str.m_pStrBuf );

	//新しいサイズを計算する
	u32 newSize = catStrSize + m_nStrBufSize - sizeof( skyString ); //-sizeof( skyString )をするのは終端子が２重になっているため。
	u32 newLen	= CStringUtil_::Len( str.m_pStrBuf ) + m_nStrBufLen;

	//新しいサイズで領域を確保する
	skyString*pNewBuffer = ( skyString* )SkyMalloc( newSize );

	//新しい領域をクリア
	CMemoryUtil_::ClearZero( pNewBuffer, newSize );

	//新しい領域に古い情報をコピー
	if ( m_pStrBuf != skyNull )
	{
		CStringUtil_::Copy( pNewBuffer, newLen, m_pStrBuf );
	}

	//古い領域をフリー
	Free();

	//新しい情報に更新
	m_nStrBufSize = newSize;
	m_nStrBufLen = newLen;
	m_pStrBuf = pNewBuffer;

	//新しい領域を連結
	_Cat( str.m_pStrBuf ); 

	return *this;
}

CStringBuffer& CStringBuffer::operator +=( skyString str )
{
	*this += &str;
	return *this;
}

CStringBuffer& CStringBuffer::operator +=( s32 value )
{
	skyString str[ NUM_LEN_MAX ];
	CStringUtil_::ConvNumToString( value , str , NUM_LEN_MAX );
	*this += str;
	return *this;
}
CStringBuffer& CStringBuffer::operator +=( u32 value )
{
	skyString str[ NUM_LEN_MAX ];
	CStringUtil_::ConvNumToString( value , str , NUM_LEN_MAX );
	*this += str;
	return *this;
}
CStringBuffer& CStringBuffer::operator +=( f32 value )
{
	skyString str[ NUM_LEN_MAX ];
	CStringUtil_::ConvNumToString( value , str , NUM_LEN_MAX );
	*this += str;
	return *this;
}
CStringBuffer& CStringBuffer::operator +=( d64 value )
{
	skyString str[ NUM_LEN_MAX ];
	CStringUtil_::ConvNumToString( value , str , NUM_LEN_MAX );
	*this += str;
	return *this;
}
CStringBuffer& CStringBuffer::operator +=( skyBool value )
{
	skyString str[ NUM_LEN_MAX ];
	CStringUtil_::ConvNumToString( value , str , NUM_LEN_MAX );
	*this += str;
	return *this;
}

} } }