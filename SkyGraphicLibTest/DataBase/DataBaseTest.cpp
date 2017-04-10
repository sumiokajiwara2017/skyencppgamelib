#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_DATABASE

#if ON_

using namespace sky::lib;

#include "SkyGraphicLib/DataBase.h"

TEST( DataBase , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif