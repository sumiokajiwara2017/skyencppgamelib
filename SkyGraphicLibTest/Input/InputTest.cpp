#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_INPUT

#if ON_

using namespace sky::lib;

#include "SkyGraphicLib/Input.h"

TEST( input , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif