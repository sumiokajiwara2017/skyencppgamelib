#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_HASH

#include "SkyBaseLib/Hash.h"

#define HASH_ID_1 _T( "HashTestKey1" )
#define HASH_ID_2 _T( "HashTestKey2" )
#define HASH_ID_3 _T( "HashTestKey3" )

TEST( Hash , 1 ) 
{
	SKY_BASE_LIB_INIT();

	//HashStringの取得
	sky::lib::hash::CHashStringPtr hash1 = CHSC( HASH_ID_1 );
	sky::lib::hash::CHashStringPtr hash2 = CHSC( HASH_ID_2 );
	sky::lib::hash::CHashStringPtr hash3 = CHSC( HASH_ID_3 );

	//HashStringの取得
	sky::lib::hash::CHashStringPtr hash4 = CHSC( HASH_ID_1 );
	sky::lib::hash::CHashStringPtr hash5 = CHSC( HASH_ID_2 );
	sky::lib::hash::CHashStringPtr hash6 = CHSC( HASH_ID_3 );

	if ( hash1 == hash4 )
	{
		SKY_PRINTF( _T( "正解！ \n" ) );
	}

	if ( hash2 == hash5 )
	{
		SKY_PRINTF( _T( "正解！ \n" ) );
	}

	if ( hash3 == hash6 )
	{
		SKY_PRINTF( _T( "正解！ \n" ) );
	}

	if ( hash1 != hash2 )
	{
		SKY_PRINTF( _T( "正解！ \n" ) );
	}

	//HashStringの全破棄
	CHashStringManager_::Clear();
	CHashStringManager_::GC();

	SKY_BASE_LIB_TERM();
}

#endif