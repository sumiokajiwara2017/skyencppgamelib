#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_CONST

#include "SkyBaseLib/Const.h"

TEST( Const , 1 ) 
{
	SKY_BASE_LIB_INIT();

	s32 value_s32 = C_S32( consol_MSG_LENGHT_WCHAR );
	value_s32 = C_S32( consol_MSG_LENGHT_SCHAR );
	const char *pChar1 = C_CHAR( debuglogic_DEBUG_OUT_PATH );
	const char *pChar2 = C_CHAR( debuglogic_DEBUG_OUT_MODE );
	const skyString *pStr1 = C_ST( debuglogic_DEBUG_OUT_PATH );
	const skyString *pStr2 = C_ST( debuglogic_DEBUG_OUT_MODE );
	SKY_UNUSED( pChar1 );
	SKY_UNUSED( pChar2 );
	SKY_UNUSED( pStr1 );
	SKY_UNUSED( pStr2 );
	value_s32 = C_S32( file_FILE_CTRL_HASH_SIZE );
	value_s32 = C_S32( hash_HASH_STR_MAN_HASH_SIZE );
	value_s32 = C_S32( input_DEF_REPEATE_INTVL );
	value_s32 = C_S32( input_DEF_ON_INTVL );
	value_s32 = C_S32( input_CONTROLLER_NUM_MAX );

	SKY_BASE_LIB_TERM();
}

#endif