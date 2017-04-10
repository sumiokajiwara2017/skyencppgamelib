#include "SkyBaseLib/Thread.h"
#include "ThreadCommonRes.h"

void ThreadCommonRes::Test1 ( s32 threadNo )
{
	sky::lib::thread::Synchronized g( pCs );
	{
		SKY_PRINTF( L"Test1:ThreadNo = %d \n" ,  threadNo );
	}
}

void ThreadCommonRes::Test2 ( s32 threadNo )
{
	sky::lib::thread::Synchronized g( pCs );
	{
		SKY_PRINTF( L"Test2:ThreadNo = %d \n" ,  threadNo );
	}
}
