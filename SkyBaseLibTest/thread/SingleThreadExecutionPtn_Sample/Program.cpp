#include "SkyBaseLib/Controller.h"
#include "SkyBaseLib/Thread.h"
#include "ThreadCommonRes/ThreadCommonRes.h"
#include "UserThread/UserThread.h"

/// <summary>
/// メイン関数
/// </summary>
int _tmain( int argc , _TCHAR *argv[] )
{
	argc;
	argv;

	//ライブラリ初期化
	SkyLibController_->Init();

	sky::lib::SmartPointerMT< ThreadCommonRes > res( NEW ThreadCommonRes() );
	IThreadManager_->CreateThread( NEW UserThread( res , 111 ) );
	IThreadManager_->CreateThread( NEW UserThread( res , 222 ) );

	IThreadManager_->WaitAllThreadEnded(); //全てのスレッドの終了を待つ

	//プリント
	PrintMemory( skyTrue );

	//ライブラリ末期化
	SkyLibController_->Term();
}