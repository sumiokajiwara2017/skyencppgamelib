#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_GRAPHIC

using namespace sky::lib;

#include "SkyBaseLib/Graphic.h"

TEST( graphic , 1 ) 
{
	SKY_BASE_LIB_INIT();

	u32 makeId_1 = MK_4BYTE_ID( 255 , 255 , 0 , 0 );
	SKY_PRINTF( _T( "MAKEID_1 = 0x%X \n" ) , makeId_1 );

	graphic::CColor red = graphic::CColor( 1.0f , 1.0f , 0.0f , 0.0f );
	u32 redRGBA = red.GetAs();
	SKY_PRINTF( _T( "redRGBA = 0x%X \n" ) , redRGBA );
	u32 redARGB = red.GetAsARGB();
	SKY_PRINTF( _T( "redARGB = 0x%X \n" ) , redARGB );

	SKY_BASE_LIB_TERM();
}

#endif