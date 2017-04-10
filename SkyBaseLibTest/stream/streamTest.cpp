#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_STREAM

#include "SkyBaseLib/Stream.h"

using namespace sky::lib;

TEST( stream , 1 ) 
{
	SKY_BASE_LIB_INIT();

	memory::CMemoryStream memStream( _T( "Test" ) , 32 , skyTrue , skyFalse , skyTrue );

	memStream.vWriteStream( _T( "hogeTest write ok? \n" ) );

	SKY_PRINTF( _T( "%s" ) , memStream.vGetData() );

	SKY_BASE_LIB_TERM();
}

TEST( stream , 2 ) 
{
	SKY_BASE_LIB_INIT();

	memory::CMemoryStream memStream( _T( "hoge" ) , 1 , skyTrue , skyFalse , skyTrue );

	u16 src1 = 2;
	s16 src2 = -2;
	u32 src3 = 3;
	s32 src4 = -3;

	memStream.vWriteStream( src1 );
	memStream.vWriteStream( src2 );
	memStream.vWriteStream( src3 );
	memStream.vWriteStream( src4 );

	SKY_PRINTF( _T( "src = %d %d %d %d \n" ) , src1 , src2 , src3 , src4 );

	memStream.vSetSeek( 0 );

	u16 dest1 = 0;
	s16 dest2 = 0;
	u32 dest3 = 0;
	s32 dest4 = 0;
	u32 dest5 = 0;

	memStream.vReadStream( dest1 );
	memStream.vReadStream( dest2 );
	memStream.vReadStream( dest3 );
	memStream.vReadStream( dest4 );
	memStream.vReadStream( dest5 );
	memStream.vReadStream( dest5 );
	memStream.vReadStream( dest5 );
	memStream.vReadStream( dest5 );

	SKY_PRINTF( _T( "dest = %d %d %d %d %d \n" ) , dest1 , dest2 , dest3 , dest4 , dest5 );

	SKY_BASE_LIB_TERM();
}

#endif