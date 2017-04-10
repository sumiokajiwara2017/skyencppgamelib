#include "StdAfx.h"
#include "SkyBaseLib/File.h"
#include "SkyBaseLib/Locale.h"

SkyImplementRTTI( sky::lib::file::CDiscFileStream , sky::lib::stream::AbsStream );

namespace sky { namespace lib { namespace file { 

u32	CDiscFileStream::vGetSize()
{
	SKY_ASSERT_MSG( m_IsOpen , _T( "File %s has not been opened. " ) , ( const skyString * )m_Name );
	return m_spFileInfo->fileSize;
}
skyBool CDiscFileStream::vOpen()
{
	SKY_ASSERT_MSG( !m_IsOpen , _T( "File %s has already been opened. " ) , ( const skyString * )m_Name );
	CFileUtil::Open( m_Name , m_spFileInfo , m_eMode );
	m_IsOpen = skyTrue;
	return skyTrue;
}
skyBool CDiscFileStream::vClose()
{
	SKY_ASSERT_MSG( m_IsOpen , _T( "File %s has not been opened." ) , ( const skyString * )m_Name );

	CFileUtil::Close( m_spFileInfo );
	m_IsOpen = skyFalse;
	return skyTrue;
}
u32 CDiscFileStream::vWriteStream( const skyString *format , ... )
{
	SKY_ASSERT_MSG( m_IsOpen , _T( "File %s has not been opened." ) , ( const skyString * )m_Name );

	CLocaleUtil_::SetAllLocale( locale::Country_Japanese );

	skyString	buf[ stream::WRITE_STREAM_BUF_SIZE ];
	VA_LIST( buf , stream::WRITE_STREAM_BUF_SIZE , format );

	u32 size = CStringUtil_::LenByte( buf );
	return vWriteStream( ( u8* )buf , size );
}
u32 CDiscFileStream::vWriteStream( u8* pData , u32 size )
{
	SKY_ASSERT_MSG( m_IsOpen , _T( "File %s has not been opened." ) , ( const skyString * )m_Name );

	u32 writeSize = CFileUtil::Write( m_spFileInfo , pData , size , size , this->m_Seek );
	this->m_Seek += writeSize;
	return writeSize;
}
u32 CDiscFileStream::vReadStream( u8  *pData , u32 size )
{
	SKY_ASSERT_MSG( m_IsOpen , _T( "File %s has not been opened." ) , ( const skyString * )m_Name );

	//�V�[�N�ʒu�ƃt�@�C���T�C�Y�������l�̏ꍇ�Ō�܂ł݂��Ƃ݂Ȃ��ĂȂɂ����Ȃ��B
	if ( m_Seek == m_spFileInfo->fileSize ) return 0;

	u32 readSize = CFileUtil::Read( m_spFileInfo , pData, size , size , this->m_Seek );
	this->m_Seek += readSize;
	return readSize;
}

u32 CDiscFileStream::vWriteStream( u16 data )
{
	SKY_ASSERT_MSG( m_IsOpen , _T( "File %s has not been opened." ) , ( const skyString * )m_Name );
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap16( ( u8 * )&data );
	}
	return vWriteStream( ( u8 * )&data , sizeof( u16 ) );
}

u32 CDiscFileStream::vWriteStream( s16 data )
{
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap16( ( u8 * )&data );
	}
	return vWriteStream( ( u8 * )&data , sizeof( s16 ) );
}
u32 CDiscFileStream::vWriteStream( u32 data )
{
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap32( ( u8 * )&data );
	}
	return vWriteStream( ( u8 * )&data , sizeof( u32 ) );
}
u32 CDiscFileStream::vWriteStream( s32 data )
{
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap32( ( u8 * )&data );
	}
	return vWriteStream( ( u8 * )&data , sizeof( s32 ) );
}
u32 CDiscFileStream::vWriteStream( f32 data )
{
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap32( ( u8 * )&data );
	}
	return vWriteStream( ( u8 * )&data , sizeof( f32 ) );
}
u32 CDiscFileStream::vWriteStream( d64 data )
{
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap32( ( u8 * )&data );
	}
	return vWriteStream( ( u8 * )&data , sizeof( d64 ) );
}
u32 CDiscFileStream::vReadStream(skyString  *data , u32 len )
{
	return vReadStream( ( u8 * )&data , sizeof( skyString ) * len );
}
u32 CDiscFileStream::vReadStream( u16 &data )
{
	u32 ret = vReadStream( ( u8 * )&data , sizeof( u16 ) );
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap16( ( u8 * )&data );
	}
	return ret;
}
u32 CDiscFileStream::vReadStream( s16 &data )
{
	u32 ret = vReadStream( ( u8 * )&data , sizeof( s16 ) );
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap16( ( u8 * )&data );
	}
	return ret;
}
u32 CDiscFileStream::vReadStream( u32 &data )
{
	u32 ret = vReadStream( ( u8 * )&data , sizeof( u32 ) );
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap32( ( u8 * )&data );
	}
	return ret;
}
u32 CDiscFileStream::vReadStream( s32 &data )
{
	u32 ret = vReadStream( ( u8 * )&data , sizeof( s32 ) );
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap32( ( u8 * )&data );
	}
	return ret;
}
u32 CDiscFileStream::vReadStream( f32 &data )
{
	u32 ret = vReadStream( ( u8 * )&data , sizeof( f32 ) );
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap32( ( u8 * )&data );
	}
	return ret;
}
u32 CDiscFileStream::vReadStream( d64 &data )
{
	u32 ret = vReadStream( ( u8 * )&data , sizeof( d64 ) );
	if ( vIsSwap() ) 
	{
		endian::EndianUtil::Swap64( ( u8 * )&data );
	}
	return ret;
}

CDiscFileStream::CDiscFileStream( const skyString *name , eOpenMode eMode )
: stream::AbsStream( stream::eStreamKind_file )
{
	m_Name = name;
	m_eMode = eMode;
	m_pMarkInfoHash = NEW_() stream::MarkInfoHash( stream::MARK_INFO_HASH_SIZE );
}

CDiscFileStream::~CDiscFileStream()
{
	if ( m_IsOpen == skyTrue )
	{
		vClose();
	}
	DEL m_pMarkInfoHash;
}

u32	 CDiscFileStream::vMarkWrite( const skyString *mark , u32 size )
{
	stream::MarkInfo markInfo;
	markInfo.seekPos = m_Seek;
	markInfo.size = size;

	m_pMarkInfoHash->Set( mark , markInfo );

	vAddSeek( size );

	return size;
}
u32	 CDiscFileStream::vMarkReplace( const skyString *mark , u8* data , u32 size )
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