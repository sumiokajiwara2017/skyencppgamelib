#include "StdAfx.h"
#include "SkyBaseLib/Dos.h"
#include "SkyBaseLib/Debug.h"
#include "SkyBaseLib/Script.h"
#include "SkyBaseLib/Directory.h"

#ifdef SKYLIB_PLATFORM_WIN32

namespace sky { namespace lib { namespace dos {

u32 CDosUtil::ExecDosCommand( const skyString *cmd , const skyString *outFilePath , skyBool isDisplayWindow , const skyString *currentDir )
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
	BOOL hndlFlg = FALSE;
    DWORD dwExCode = 0;
    
    memset( &si , 0 , sizeof( STARTUPINFO ) );
    memset( &pi , 0 , sizeof( PROCESS_INFORMATION ) );

	if( isDisplayWindow == skyFalse )
	{
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
	}

	//結果をファイルに出力する
	FILE* fpredir;
	if ( outFilePath != skyNull )
	{
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
		if ( ( _tfreopen_s( &fpredir , outFilePath , _T( "w+" ), stdout ) ) != 0 )
#else
		if ( ( fpredir = _tfreopen( outFilePath , _T( "w+" ), stdout ) ) != 0 )
#endif
		{
			SKY_PANIC_MSG( _T( "It failed in the opening of passing for redirecting" ) ); //リダイレクト用パスのオープンに失敗しました。
		}
		hndlFlg = TRUE;
        si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
		si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	}

	//一旦バッファリングする。（エラーするため）
	skyString cmdBuf[ CMD_LEN_MAX ];
	CStringUtil_::Sprintf( cmdBuf , CMD_LEN_MAX , cmd );

	//実行
    CreateProcess(
		NULL ,
        cmdBuf ,
        NULL ,
        NULL ,
        hndlFlg ,
        0 ,
        NULL ,
        currentDir ,
        &si ,
        &pi );

	//終了待ち
	skyBool isLoop = skyTrue;
    while ( isLoop )
	{
        Sleep(100);
        GetExitCodeProcess( pi.hProcess , &dwExCode );
        if (dwExCode == STILL_ACTIVE)
		{
            continue;
        } 
		else
		{
            break;
        }
    }

    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

	if ( outFilePath != skyNull )
	{
		fclose( fpredir );
	}

	return dwExCode;
}

} } }

//-----スクリプトでも使う----▽
#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

static SQInteger ExecDosCommand( HSQUIRRELVM v )
{
	//引数を受け取る
	const SQChar *val;
	if ( !SQ_SUCCEEDED( sq_getstring( v , 2 , &val ) ) )
	{
		SKY_PANIC_MSG( _T( "The argument was not able to be acquired. " ) ); //引数を取得できませんでした。
	}

	//環境変数コンバート
	skyString conv[ sky::lib::dos::CMD_LEN_MAX ];
	CPathUtil_::ReplaceEnv( val , conv , sky::lib::dos::CMD_LEN_MAX );

	//コマンド実行
	CDosUtil_::ExecDosCommand( conv );

	return 1; //戻り値1個返したよ
};

sky::lib::squirrel::SqClassObject GetSqCDosUtil() //コピーコンストラクタが呼ばれるので毎フレームは呼ばないでください。おそらく一回の実行で数回しか呼ばないはずです。
{
	sky::lib::squirrel::SqClassObject classobj( CHS( _T( "CDosUtil" ) ) );
	classobj.AddMethod( CHS( _T( "ExecDosCommand" ) ) , ExecDosCommand , CHS( _T( "ys" ) ) );

	return classobj;
}

#endif

#endif