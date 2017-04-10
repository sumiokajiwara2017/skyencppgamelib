#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_FPS

#include "SkyBaseLib/Fps.h"

TEST( fps , 1 ) 
{
	SKY_BASE_LIB_INIT();

	SKY_BASE_LIB_TERM();
}

#endif