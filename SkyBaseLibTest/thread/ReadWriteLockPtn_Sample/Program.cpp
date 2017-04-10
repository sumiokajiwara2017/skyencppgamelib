#include "SkyBaseLib/Controller.h"
#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "CommonResource/CommonResource.h"
#include "WriterThread/WriterThread.h"
#include "ReaderThread/ReaderThread.h"

/// <summary>
/// メイン関数
/// </summary>
int _tmain( int argc , _TCHAR *argv[] )
{
	argc;
	argv;

	//ライブラリ初期化
	SkyLibController_->Init();

	sky::lib::thread::ReadWriteLock	rwl;
	CommonResource					comres;

	IThreadManager_->CreateThread( NEW WriterThread( &rwl , &comres , L"書き込み担当" ) );

	IThreadManager_->CreateThread( NEW ReaderThread( &rwl , &comres , L"読み込み担当１" ) );
	IThreadManager_->CreateThread( NEW ReaderThread( &rwl , &comres , L"読み込み担当２" ) );
	IThreadManager_->CreateThread( NEW ReaderThread( &rwl , &comres , L"読み込み担当３" ) );

	IThreadManager_->WaitAllThreadEnded(); //全てのスレッドの終了を待つ

	//プリント
	PrintMemory( skyTrue );

	//ライブラリ末期化
	SkyLibController_->Term();
}