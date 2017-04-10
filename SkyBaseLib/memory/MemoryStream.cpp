#include "StdAfx.h"
#include "SkyBaseLib/Memory.h"
#include "SkyBaseLib/SKYTL.h"

SkyImplementRTTI( sky::lib::memory::CMemoryStream , sky::lib::stream::AbsStream );

namespace sky { namespace lib { namespace memory {

skyBool CMemoryStream::vOpen()
{
	m_Seek = 0;
	return skyTrue;
}

u8* CMemoryStream::vCloneData()
{
	u8 *pResult = ( u8 * )SkyMalloc_( vGetSize() );
	CMemoryUtil_::Copy( pResult , vGetData() , vGetSize() );

	return pResult;
}

skyBool CMemoryStream::vClose()
{
	vDestroy();
	return skyTrue;
}
u32 CMemoryStream::vWriteStream( const skyString *format , ... )
{
	skyString	buf[ stream::WRITE_STREAM_BUF_SIZE ];
	VA_LIST( buf , stream::WRITE_STREAM_BUF_SIZE , format );

	u32 size = CStringUtil_::LenByte( buf );
	return vWriteStream( ( u8* )buf , size );
}

u32 CMemoryStream::vWriteStream( u8* pData , u32 size )
{
	SKY_ASSERT_MSG( !m_IsReadOnly , _T( "This data is not revokable. " ) ); //���̃f�[�^�͕ύX�ł��܂���B

	//�T�C�Y������Ȃ�������g������
	if ( m_Seek + size > m_Size )
	{
		SKY_ASSERT_MSG( m_IsEnableChengeSize , _T( "This data cannot change the size. " ) ); //���̃f�[�^�̓T�C�Y�ύX�ł��܂���B

		m_Size = m_Seek + size;
		m_pData = ( u8 * )SkyRealloc_( m_pData , m_Size );
	}

	CMemoryUtil_::Copy( ( void * )&m_pData[ m_Seek ] , ( void * )pData , size );

	//�X�V���t��ς���
//	m_nUpdateTime = time::CTimeUtil::GetTime(); //�������݂̓x�ɍX�V���t�����̂͏ꍇ�ɂ���Ă͏d�����̂ŁA�����ōD���Ȏ��ɃZ�b�g���Ă�������

	vSetSeek( m_Seek + size );

	return m_Size;
}
u32 CMemoryStream::vReadStream( u8  *pData , u32 size )
{
	u32 readSize = size;

	if (  m_Seek == m_Size ) return 0;

	//�T�C�Y��葽���ǂݍ������Ƃ�����T�C�Y�𒲐����ēǂ�
	if ( m_Seek + size > m_Size )
	{
		readSize = size - ( m_Seek + size - m_Size );
	}

	CMemoryUtil_::Copy( ( void * )pData , ( void * )&m_pData[ m_Seek ] , readSize );

	vSetSeek( m_Seek + readSize );

	return readSize;
}
CMemoryStream::CMemoryStream( const skyString *name , u32 size , skyBool isEnableDelete , skyBool isReadOnly , skyBool isEnableChengeSize )
: AbsStream( stream::eStreamKind_memory )
{
	vCreate( size );

	ComInit( name , isEnableDelete , isReadOnly , isEnableChengeSize );

}
CMemoryStream::CMemoryStream( const skyString *name , void* data , u32 size , skyBool isEnableDelete , skyBool isReadOnly , skyBool isEnableChengeSize )
: AbsStream( stream::eStreamKind_memory )
{
	m_pData = ( u8 * )data;
	m_Size = size;

	ComInit( name , isEnableDelete , isReadOnly , isEnableChengeSize );
}
void CMemoryStream::ComInit( const skyString *name , skyBool isEnableDelete , skyBool isReadOnly , skyBool isEnableChengeSize )
{
	//�X�V���t��ς���
	m_nUpdateTime = time::CTimeUtil::GetNowTime();

	m_Name = name;
	m_IsReadOnly = isReadOnly;
	m_IsEnableChengeSize = isEnableChengeSize;
	m_IsEnableDelete = isEnableDelete;
	m_IsOpen = skyTrue;

	m_pMarkInfoHash = NEW_() stream::MarkInfoHash( stream::MARK_INFO_HASH_SIZE );

}

CMemoryStream::~CMemoryStream()
{
	//����������f�[�^������
	vDestroy();

	DEL m_pMarkInfoHash;
}

void CMemoryStream::vCreate( u32 size )
{
	vDestroy();

	m_pData = ( u8* )SkyMalloc_( size );
	m_Size = size;
#ifdef SW_SKYLIB_EXEC_ZERO_CLEAR
	CMemoryUtil_::ClearZero( ( void * )m_pData , size );
#endif

	//�X�V���t��ς���
	m_nUpdateTime = time::CTimeUtil::GetNowTime();
}

void CMemoryStream::vDestroy()
{
	if ( m_pData != skyNull )
	{
		//������f�[�^�Ȃ����
		if ( m_IsEnableDelete == skyTrue )
		{
			SkyFree ( m_pData );
			m_pData = skyNull;
			m_Size = 0;
			m_Seek = 0;
		}
	}
}

u32 CMemoryStream::vWriteStream( u16 data )
{
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap16( ( u8 * )&data );
	}
	return vWriteStream( ( u8 * )&data , sizeof( u16 ) );
}

u32 CMemoryStream::vWriteStream( s16 data )
{
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap16( ( u8 * )&data );
	}
	return vWriteStream( ( u8 * )&data , sizeof( s16 ) );
}
u32 CMemoryStream::vWriteStream( u32 data )
{
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap32( ( u8 * )&data );
	}
	return vWriteStream( ( u8 * )&data , sizeof( u32 ) );
}
u32 CMemoryStream::vWriteStream( s32 data )
{
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap32( ( u8 * )&data );
	}
	return vWriteStream( ( u8 * )&data , sizeof( s32 ) );
}
u32 CMemoryStream::vWriteStream( f32 data )
{
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap32( ( u8 * )&data );
	}
	return vWriteStream( ( u8 * )&data , sizeof( f32 ) );
}
u32 CMemoryStream::vWriteStream( d64 data )
{
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap32( ( u8 * )&data );
	}
	return vWriteStream( ( u8 * )&data , sizeof( d64 ) );
}
u32 CMemoryStream::vReadStream(skyString  *data , u32 len )
{
	return vReadStream( ( u8 * )&data , sizeof( skyString ) * len );
}
u32 CMemoryStream::vReadStream( u16 &data )
{
	u32 ret = vReadStream( ( u8 * )&data , sizeof( u16 ) );
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap16( ( u8 * )&data );
	}
	return ret;
}
u32 CMemoryStream::vReadStream( s16 &data )
{
	u32 ret = vReadStream( ( u8 * )&data , sizeof( s16 ) );
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap16( ( u8 * )&data );
	}
	return ret;
}
u32 CMemoryStream::vReadStream( u32 &data )
{
	u32 ret = vReadStream( ( u8 * )&data , sizeof( u32 ) );
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap32( ( u8 * )&data );
	}
	return ret;
}
u32 CMemoryStream::vReadStream( s32 &data )
{
	u32 ret = vReadStream( ( u8 * )&data , sizeof( s32 ) );
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap32( ( u8 * )&data );
	}
	return ret;
}
u32 CMemoryStream::vReadStream( f32 &data )
{
	u32 ret = vReadStream( ( u8 * )&data , sizeof( f32 ) );
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap32( ( u8 * )&data );
	}
	return ret;
}
u32 CMemoryStream::vReadStream( d64 &data )
{
	u32 ret = vReadStream( ( u8 * )&data , sizeof( d64 ) );
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap64( ( u8 * )&data );
	}
	return ret;
}

u32	 CMemoryStream::vMarkWrite( const skyString *mark , u32 size )
{
	stream::MarkInfo markInfo;
	markInfo.seekPos = m_Seek;
	markInfo.size = size;

	m_pMarkInfoHash->Set( mark , markInfo );

	vSetSeek( m_Seek + size );

	return size;
}
u32	 CMemoryStream::vMarkReplace( const skyString *mark , u8* data , u32 size )
{
	stream::MarkInfo &markInfo = ( *m_pMarkInfoHash )[ mark ];

	SKY_ASSERT_MSG( markInfo.size == size , _T( "The size and the replacement size when Mark is done are different. " ) ); //Mark�������ۂ̃T�C�Y�ƒu�������T�C�Y���Ⴂ�܂��B

	//���݂̈ʒu��ۑ�
	u32 saveSeek = m_Seek;

	//�}�[�N�̈ʒu�Ɉړ�
	vSetSeek( markInfo.seekPos );

	//��������
	vWriteStream( data, size );

	//�ۑ������ʒu�ɖ߂�
	vSetSeek( saveSeek );

	//�}�[�N������
	m_pMarkInfoHash->Delete( mark );

	return size;
}

} } }
