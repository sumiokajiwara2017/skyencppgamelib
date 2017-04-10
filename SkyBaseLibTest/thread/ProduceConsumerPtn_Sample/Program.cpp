#include "SkyBaseLib/Controller.h"
#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "Data/Data.h"
#include "Channel/Channel.h"
#include "ConsumerThread/ConsumerThread.h"
#include "ProducerThread/ProducerThread.h"

/// <summary>
/// メイン関数
/// </summary>
int _tmain( int argc , _TCHAR *argv[] )
{
	argc;
	argv;

	//ライブラリ初期化
	SkyLibController_->Init();

	Channel ch;
	IThreadManager_->CreateThread( NEW ConsumerThread( &ch ) );
	IThreadManager_->CreateThread( NEW ProducerThread( &ch , L"アマゾンで買った本" ) );
	IThreadManager_->CreateThread( NEW ProducerThread( &ch , L"テレビショッピングで買った家具" ) );

	IThreadManager_->WaitAllThreadEnded(); //全てのスレッドの終了を待つ

	//プリント
	PrintMemory( skyTrue );

	//ライブラリ末期化
	SkyLibController_->Term();
}