#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_RENDER

#if ON_

using namespace sky::lib;

#include "SkyGraphicLib/Render.h"

TEST( render , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif