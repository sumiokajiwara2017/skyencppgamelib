#include "StdAfx.h"
#include "SkyBaseLib/Stream.h"
#include "SkyBaseLib/Debug.h"

SkyImplementRTTI( sky::lib::stream::IStream   , sky::lib::base::SkyRefObject );
SkyImplementRTTI( sky::lib::stream::AbsStream , sky::lib::stream::IStream );

namespace sky { namespace lib { namespace stream {

//-------é¿ëïÇ≈Ç´ÇÈÇ‡ÇÃÇÕé¿ëïÇ∑ÇÈÅ§
const u8*   AbsStream::vGetSeekData(){ SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyNull; };
const u32   AbsStream::vGetSeek(){  SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyNull; }
const u8*   AbsStream::vGetData(){ SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyNull; };
const u8**  AbsStream::vGetDataPtr(){ SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyNull; };
u8*         AbsStream::vCloneData(){ SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyNull; };
u32		    AbsStream::vGetSize(){ SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyNull; };
u32			AbsStream::vWriteStream( u8* pData , u32 size ){ SKY_UNUSED_ARG( pData ); SKY_UNUSED_ARG( size ); SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyNull; };
u32			AbsStream::vWriteStream( u16 data ){ SKY_UNUSED_ARG( data ); SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyNull; };
u32			AbsStream::vWriteStream( s16 data ){ SKY_UNUSED_ARG( data ); SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyNull; };
u32			AbsStream::vWriteStream( u32 data ){ SKY_UNUSED_ARG( data ); SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyNull; };
u32			AbsStream::vWriteStream( s32 data ){ SKY_UNUSED_ARG( data ); SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyNull; };
u32			AbsStream::vWriteStream( f32 data ){ SKY_UNUSED_ARG( data ); SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyNull; };
u32			AbsStream::vWriteStream( d64 data ){ SKY_UNUSED_ARG( data ); SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyNull; };
u32		    AbsStream::vReadStream( u8  *pData , u32 size ){ SKY_UNUSED_ARG( pData ); SKY_UNUSED_ARG( size ); SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyNull; };
u32			AbsStream::vReadStream( u16 &data ){ SKY_UNUSED_ARG( data ); SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyNull; };
u32			AbsStream::vReadStream( s16 &data ){ SKY_UNUSED_ARG( data ); SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyNull; };
u32			AbsStream::vReadStream( u32 &data ){ SKY_UNUSED_ARG( data ); SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyNull; };
u32			AbsStream::vReadStream( s32 &data ){ SKY_UNUSED_ARG( data ); SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyNull; };
u32			AbsStream::vReadStream( f32 &data ){ SKY_UNUSED_ARG( data ); SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyNull; };
u32			AbsStream::vReadStream( d64 &data ){ SKY_UNUSED_ARG( data ); SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyNull; };
u32         AbsStream::vWriteReadStream( u8* pData , u32 size , skyBool isWrite )
{
	u32 result = 0;
	if ( isWrite == skyTrue )
	{
		result = vWriteStream( pData , size );
	}
	else
	{
		result = vReadStream( pData , size );
	}
	return result;
};

void	    AbsStream::vCreate( u32 size ){ SKY_UNUSED_ARG( size ); SKY_PANIC_MSG( _T( "This function is not supported. " ) ); };
void 	    AbsStream::vDestroy(){ SKY_PANIC_MSG( _T( "This function is not supported. " ) ); };
u32			AbsStream::vMarkWrite( const skyString *mark , u32 size ){ SKY_UNUSED_ARG( mark ); SKY_UNUSED_ARG( size ); SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyNull; };
u32			AbsStream::vMarkReplace( const skyString *mark , u8* data , u32 size ){ SKY_UNUSED_ARG( mark ); SKY_UNUSED_ARG( data ); SKY_UNUSED_ARG( size ); SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyNull; };
skyBool     AbsStream::vReadLine( skyString *pBuf , u32 bufLen , u32 offset ){ SKY_UNUSED_ARG( pBuf ); SKY_UNUSED_ARG( bufLen ); SKY_UNUSED_ARG( offset ); SKY_PANIC_MSG( _T( "This function is not supported. " ) ); return skyFalse; }
void        AbsStream::vWriteLine( const skyString *pBuf , u32 len ){ SKY_UNUSED_ARG( pBuf ); SKY_UNUSED_ARG( len ); SKY_PANIC_MSG( _T( "This function is not supported. " ) ); }

//-------é¿ëïÇ≈Ç´ÇÈÇ‡ÇÃÇÕé¿ëïÇ∑ÇÈÅ¢


} } }