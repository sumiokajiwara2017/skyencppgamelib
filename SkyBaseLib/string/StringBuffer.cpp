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

	//�V����������̃T�C�Y�Ńo�b�t�@����蒼���ăR�s�[����
	u32 size = CStringUtil_::LenByte( pStr );
	if ( size != m_nStrBufSize )
	{
		//�������J��
		Free();

		//�V�����̈���m��
		m_pStrBuf = ( skyString * )SkyMalloc( size );

		//�V�����̈�T�C�Y��ۑ�
		m_nStrBufSize = size;
		m_nStrBufLen = CStringUtil_::Len( pStr );
		*m_pStrBuf = '\0';
	}

	//�V�����̈�ɃR�s�[
	_Copy( pStr );

	return *this;
}
CStringBuffer& CStringBuffer::operator =( const CStringBuffer &str )
{
	if ( CStringUtil_::LenByte( str.m_pStrBuf ) == 0 ) return *this;

	//�V����������̃T�C�Y�Ńo�b�t�@����蒼���ăR�s�[����
	u32 size = CStringUtil_::LenByte( str.m_pStrBuf );
	if ( size != m_nStrBufSize )
	{
		//�������J��
		Free();

		//�V�����̈���m��
		m_pStrBuf = ( skyString* )SkyMalloc( size );

		//�V�����̈�T�C�Y��ۑ�
		m_nStrBufSize = size;
		m_nStrBufLen = CStringUtil_::Len( str.m_pStrBuf );
		*m_pStrBuf = '\0';
	}

	//�V�����̈�ɃR�s�[
	_Copy( str.m_pStrBuf );

	return *this;
}

CStringBuffer& CStringBuffer::operator +=( const skyString *pStr )
{
	if ( CStringUtil_::LenByte( pStr ) == 0 ) return *this;

	//�V����������𑫂����C�Y�Ńo�b�t�@����蒼���ăR�s�[����
	u32 catStrSize = CStringUtil_::LenByte( pStr );

	//�V�����T�C�Y���v�Z����
	u32 newSize = catStrSize + m_nStrBufSize;
	if ( m_nStrBufSize == 0 ) newSize += sizeof( skyString );
	u32 newLen	= CStringUtil_::Len( pStr ) + m_nStrBufLen;

	//�V�����T�C�Y�ŗ̈���m�ۂ���
	skyString *pNewBuffer = ( skyString* )SkyMalloc( newSize );

	//�V�����̈���N���A
	CMemoryUtil_::ClearZero( pNewBuffer, newSize );

	//�V�����̈�ɌÂ������R�s�[
	if ( m_pStrBuf != skyNull )
	{
		CStringUtil_::Copy( pNewBuffer, newLen, m_pStrBuf );
	}

	//�Â��̈���t���[
	Free();

	//�V�������ɍX�V
	m_nStrBufSize = newSize;
	m_nStrBufLen = newLen;
	m_pStrBuf = pNewBuffer;

	//�V�����̈��A��
	_Cat( pStr ); 

	return *this;
}
CStringBuffer& CStringBuffer::operator +=( const CStringBuffer &str )
{
	if ( CStringUtil_::LenByte( str.m_pStrBuf ) == 0 ) return *this;

	//�V����������𑫂����C�Y�Ńo�b�t�@����蒼���ăR�s�[����
	u32 catStrSize = CStringUtil_::LenByte( str.m_pStrBuf );

	//�V�����T�C�Y���v�Z����
	u32 newSize = catStrSize + m_nStrBufSize - sizeof( skyString ); //-sizeof( skyString )������̂͏I�[�q���Q�d�ɂȂ��Ă��邽�߁B
	u32 newLen	= CStringUtil_::Len( str.m_pStrBuf ) + m_nStrBufLen;

	//�V�����T�C�Y�ŗ̈���m�ۂ���
	skyString*pNewBuffer = ( skyString* )SkyMalloc( newSize );

	//�V�����̈���N���A
	CMemoryUtil_::ClearZero( pNewBuffer, newSize );

	//�V�����̈�ɌÂ������R�s�[
	if ( m_pStrBuf != skyNull )
	{
		CStringUtil_::Copy( pNewBuffer, newLen, m_pStrBuf );
	}

	//�Â��̈���t���[
	Free();

	//�V�������ɍX�V
	m_nStrBufSize = newSize;
	m_nStrBufLen = newLen;
	m_pStrBuf = pNewBuffer;

	//�V�����̈��A��
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