#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_DOS

#include "SkyBaseLib/Dos.h"

static skyString *TEST1_BAT_FILE_PATH = _T( "data\\test\\dos\\Test.bat" ); //パスは必ず\\区切り
static skyString *TEST1_BAT_FILE_PATH_2 = _T( "notepad" );
static skyString *TEST1_BAT_FILE_RESULT_PATH = _T( "data\\test\\dos\\TestRs.txt" ); //パスは必ず\\区切り

TEST( dos , 1 ) 
{
	SKY_BASE_LIB_INIT();

	skyString ioBuf[ 1024 ];
	CStringUtil_::Sprintf( ioBuf , 1024 , TEST1_BAT_FILE_PATH );

	//Ｄｏｓでバッチを実行する
	CDosUtil_::ExecDosCommand( ioBuf , TEST1_BAT_FILE_RESULT_PATH );

	CStringUtil_::Sprintf( ioBuf , 1024 , TEST1_BAT_FILE_PATH_2 );

	//Ｄｏｓでノートパッドを起動する
	CDosUtil_::ExecDosCommand( ioBuf , TEST1_BAT_FILE_RESULT_PATH );
	
	SKY_BASE_LIB_TERM();
}

#endif