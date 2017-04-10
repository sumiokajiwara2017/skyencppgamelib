#include "SkyBaseLib/Controller.h"
#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "RequestData/RequestData.h"
#include "RequestQueue/RequestQueue.h"
#include "ClientThread/ClientThread.h"
#include "ServerThread/ServerThread.h"

/// <summary>
/// メイン関数
/// </summary>
int _tmain( int argc , _TCHAR *argv[] )
{
argc;
argv;

	//ライブラリ初期化
	SkyLibController_->Init();

	RequestQueuePtr rq( NEW RequestQueue() );
	IThreadManager_->CreateThread( NEW ClientThread( rq ) );
	IThreadManager_->CreateThread( NEW ServerThread( rq ) );

	IThreadManager_->WaitAllThreadEnded(); //全てのスレッドの終了を待つ

	//ライブラリ末期化
	SkyLibController_->Term();
}