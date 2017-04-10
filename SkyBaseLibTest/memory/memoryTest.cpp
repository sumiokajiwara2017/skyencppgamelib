#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_MEMORY

#include "SkyBaseLib/Memory.h"

static const u32 MINI_SIZE_ALLOC = 10000;

//typedef sky::lib::CList< u32 , void * > MemList;

TEST( memory , 1 ) 
{
	SKY_BASE_LIB_INIT();
/*

	//猛烈に小さいアロックをして開放する
	
	MemList memlist;
	for ( u32 i = 0 ; i < MINI_SIZE_ALLOC ; i++ )
	{
		memlist.AddTail( NEW u32 );
	}

	FOREACH( MemList , it , memlist )
	{
		DEL *it;
		it = memlist.Erases( it );
	}

*/
	SKY_BASE_LIB_TERM();
}

#endif