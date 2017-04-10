#include "SkyBaseLib/Controller.h"
#include "SkyBaseLib/Thread.h"
#include "SkyBaseLib/String.h"
#include "Helper/Helper.h"
#include "Host/Host.h"

/// <summary>
/// メイン関数
/// </summary>
int _tmain( int argc , _TCHAR *argv[] )
{
	argc;
	argv;

	//ライブラリ初期化
	SkyLibController_->Init();

	Host	host;
	host.DoWork( L"草刈をしました。\n" );
	host.DoWork( L"農薬をまきました。\n" );
	host.DoWork( L"水をやりました。\n" );
	SKY_PRINTF( L"------指示はすべて出した------\n" );

	IThreadManager_->WaitAllThreadEnded(); //全てのスレッドの終了を待つ

	//ライブラリ末期化
	SkyLibController_->Term();
}