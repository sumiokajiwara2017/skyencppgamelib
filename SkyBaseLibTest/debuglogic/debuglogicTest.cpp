#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_DEBUG_LOGIC

#include "SkyBaseLib/Debug.h"

using namespace sky::lib;

//スタックトレース出力
TEST( debuglogic , 1 ) 
{
	SKY_BASE_LIB_INIT();

	SKY_PRINT_STACK_TRACE_MSG( _T( "==============スタックトレース=================== \n" ) );

	SKY_BASE_LIB_TERM();
}

//出力
TEST( debuglogic , 2 ) 
{
	SKY_BASE_LIB_INIT();

	//コンソール出力
	SKY_PRINTF( _T( "コンソール出力\n" ) );

	//ファイル出力
	SKY_OPEN_FILE_STREAM( _T( "debug.log" ) );
	SKY_PRINTF( _T( "ファイル出力２\n" ) );

	//メモリ出力
	const void *debugsymbol;
	SKY_OPEN_MEM_STREAM( &debugsymbol );
	SKY_PRINTF( _T( "メモリ出力２\n" ) );
	SKY_BASE_LIB_TERM();
}

//メモリダンプ
TEST( debuglogic , 3 ) 
{
	SKY_BASE_LIB_INIT();

	u32 i = 1;

	//コンソール出力
	SKY_MEM_DUMP( ( u8 * )&i, 64 , 64, 15 );

	SKY_BASE_LIB_TERM();
}

static const u32 LOG_CATEGORY_TEST_1 = 1 << 2;
static const u32 LOG_CATEGORY_TEST_2 = 1 << 3;
static const u32 LOG_CATEGORY_TEST_3 = 1 << 4;

//ログカテゴリ
TEST( debuglogic , 4 ) 
{
	SKY_BASE_LIB_INIT();

	SKY_DEL_LOG_CATEGORY( sky::lib::debuglogic::LOG_CATEGORY_ALL );

	SKY_PRINTF_CATEGORY( sky::lib::debuglogic::eLogLevel_Error , LOG_CATEGORY_TEST_1 , _T( "TEST1 このログはでない。\n" ) );
	SKY_PRINTF_CATEGORY( sky::lib::debuglogic::eLogLevel_Error , LOG_CATEGORY_TEST_2 , _T( "TEST2 このログはでない。\n" ) );
	SKY_PRINTF_CATEGORY( sky::lib::debuglogic::eLogLevel_Error , LOG_CATEGORY_TEST_3 , _T( "TEST3 このログはでない。\n" ) );

	SKY_SET_LOG_CATEGORY( LOG_CATEGORY_TEST_1 );

	SKY_PRINTF_CATEGORY( sky::lib::debuglogic::eLogLevel_Error , LOG_CATEGORY_TEST_1 , _T( "TEST1 \n" ) );
	SKY_PRINTF_CATEGORY( sky::lib::debuglogic::eLogLevel_Error , LOG_CATEGORY_TEST_2 , _T( "TEST2 このログはでない。\n" ) );
	SKY_PRINTF_CATEGORY( sky::lib::debuglogic::eLogLevel_Error , LOG_CATEGORY_TEST_3 , _T( "TEST3 このログはでない。\n" ) );

	SKY_SET_LOG_CATEGORY( LOG_CATEGORY_TEST_2 );

	SKY_PRINTF_CATEGORY( sky::lib::debuglogic::eLogLevel_Error , LOG_CATEGORY_TEST_1 , _T( "TEST1 \n" ) );
	SKY_PRINTF_CATEGORY( sky::lib::debuglogic::eLogLevel_Error , LOG_CATEGORY_TEST_2 , _T( "TEST2 \n" ) );
	SKY_PRINTF_CATEGORY( sky::lib::debuglogic::eLogLevel_Error , LOG_CATEGORY_TEST_3 , _T( "TEST3 このログはでない。\n" ) );

	SKY_SET_LOG_CATEGORY( LOG_CATEGORY_TEST_3 );

	SKY_PRINTF_CATEGORY( sky::lib::debuglogic::eLogLevel_Error , LOG_CATEGORY_TEST_1 , _T( "TEST1 \n" ) );
	SKY_PRINTF_CATEGORY( sky::lib::debuglogic::eLogLevel_Error , LOG_CATEGORY_TEST_2 , _T( "TEST2 \n" ) );
	SKY_PRINTF_CATEGORY( sky::lib::debuglogic::eLogLevel_Error , LOG_CATEGORY_TEST_3 , _T( "TEST3 \n" ) );

	SKY_SET_LOG_CATEGORY( sky::lib::debuglogic::LOG_CATEGORY_ALL );

	SKY_BASE_LIB_TERM();
}

//ログレベル
TEST( debuglogic , 5 ) 
{
	SKY_BASE_LIB_INIT();

	SKY_SET_LOG_LEVEL( sky::lib::debuglogic::eLogLevel_Error );
	SKY_PRINTF_CATEGORY( sky::lib::debuglogic::eLogLevel_Error , sky::lib::debuglogic::LOG_CATEGORY_ALL , _T( "TEST1 \n" ) );
	SKY_PRINTF_CATEGORY( sky::lib::debuglogic::eLogLevel_Warning , sky::lib::debuglogic::LOG_CATEGORY_ALL , _T( "TEST2 \n" ) );
	SKY_PRINTF_CATEGORY( sky::lib::debuglogic::eLogLevel_Dump , sky::lib::debuglogic::LOG_CATEGORY_ALL , _T( "TEST3 \n" ) );

	SKY_SET_LOG_LEVEL( sky::lib::debuglogic::eLogLevel_Warning );
	SKY_PRINTF_CATEGORY( sky::lib::debuglogic::eLogLevel_Error , sky::lib::debuglogic::LOG_CATEGORY_ALL , _T( "TEST1 このログはでない。 \n" ) );
	SKY_PRINTF_CATEGORY( sky::lib::debuglogic::eLogLevel_Warning , sky::lib::debuglogic::LOG_CATEGORY_ALL , _T( "TEST2 \n" ) );
	SKY_PRINTF_CATEGORY( sky::lib::debuglogic::eLogLevel_Dump , sky::lib::debuglogic::LOG_CATEGORY_ALL , _T( "TEST3 \n" ) );

	SKY_SET_LOG_LEVEL( sky::lib::debuglogic::eLogLevel_Dump );
	SKY_PRINTF_CATEGORY( sky::lib::debuglogic::eLogLevel_Error , sky::lib::debuglogic::LOG_CATEGORY_ALL , _T( "TEST1 このログはでない。 \n" ) );
	SKY_PRINTF_CATEGORY( sky::lib::debuglogic::eLogLevel_Warning , sky::lib::debuglogic::LOG_CATEGORY_ALL , _T( "TEST2 このログはでない。 \n" ) );
	SKY_PRINTF_CATEGORY( sky::lib::debuglogic::eLogLevel_Dump , sky::lib::debuglogic::LOG_CATEGORY_ALL , _T( "TEST3 \n" ) );

	SKY_SET_LOG_LEVEL( sky::lib::debuglogic::eLogLevel_Error );

	SKY_BASE_LIB_TERM();
}


#endif