#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_POINTER

#include "SkyBaseLib/Memory.h"

class Hoge : public sky::lib::base::SkyRefObject
{
	u32 mem1;
};
typedef sky::lib::SmartPointer< Hoge >		HogePtr;

TEST( pointer , 1 ) 
{
	SKY_BASE_LIB_INIT();

	HogePtr hogePtr( NEW Hoge() );
	HogePtr hoge2Ptr;

	hoge2Ptr = hogePtr;

	hogePtr.Delete();
	hoge2Ptr.Delete();

	SKY_BASE_LIB_TERM();
}

#endif