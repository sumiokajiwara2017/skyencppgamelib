#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_SCRIPT

#include "SkyMathLib/Script.h"

static const skyString *TEST1_NUTFILE_PATH = _T( "data/test/script/ClassBind2.nut" );

TEST( Script , 1 ) 
{
	SKY_MATH_LIB_INIT();

#ifdef SKYLIB_SCRIPT_USE_SQUIRREL

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );
	sky::lib::math::squirrel::CSqUtil::RegisterSkyMathLibClass( spSqVm->Vm() );
	spSqVm->RegisterScriptFile( CHSC( TEST1_NUTFILE_PATH ) );

	sky::lib::squirrel::SqFuncParam inParam;		//à¯êî
	sky::lib::squirrel::SqFuncParam outParam;		//ñﬂÇËíl

	inParam.SetParam( (s32)50 );
	inParam.SetParam( (s32)100 );
	spSqVm->ExecuteFuncSync( CHS( _T( "ClassExec" ) ) , inParam , outParam ); //ä÷êîé¿çs

#endif

	SKY_MATH_LIB_TERM();
}

#endif