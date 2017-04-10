#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_CONTROLLER

#include "SkyBaseLib/Controller.h"

TEST( controller , 1 ) 
{
	SKY_BASE_LIB_INIT();

	SKY_BASE_LIB_TERM();
}

#endif