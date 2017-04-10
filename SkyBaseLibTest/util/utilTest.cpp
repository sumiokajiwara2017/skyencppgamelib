#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_UTIL

#include "SkyBaseLib/Util.h"

TEST( util , 1 ) 
{
	SKY_BASE_LIB_INIT();

	//Crc’l‚ðŽæ“¾
	u32 crc = 0;
	skyString	*pTestString_1 = _T( "Test1" );
	skyString	*pTestString_2 = _T( "Test2" );
	skyString	*pTestString_3 = _T( "Test3" );

	crc = sky::lib::util::Crc::Create( pTestString_1 );
	crc = sky::lib::util::Crc::Create( pTestString_2 );
	crc = sky::lib::util::Crc::Create( pTestString_3 );

	u32 murmurHash = 0;

	murmurHash = sky::lib::util::MurmurHash::Create( pTestString_1 , CStringUtil_::LenByte( pTestString_1 ) , 1 );
	murmurHash = sky::lib::util::MurmurHash::Create( pTestString_2 , CStringUtil_::LenByte( pTestString_2 ) , 1 );
	murmurHash = sky::lib::util::MurmurHash::Create( pTestString_3 , CStringUtil_::LenByte( pTestString_3 ) , 1 );


	SKY_BASE_LIB_TERM();
}

#endif