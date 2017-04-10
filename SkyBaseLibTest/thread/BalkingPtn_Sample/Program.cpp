#include "SkyBaseLib/Controller.h"
#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "DrawCommand/DrawCommand.h"
#include "DrawCommand/DrawCommandManager.h"
#include "DrawExecEvent/DrawExecEvent.h"
#include "DrawThread/DrawThread.h"
#include "UpdateThread/UpdateThread.h"

/// <summary>
/// メイン関数
/// </summary>
int _tmain( int argc , _TCHAR *argv[] )
{
argc;
argv;
	//ライブラリ初期化
	SkyLibController_->Init();

	DrawExecEventPtr ev( NEW DrawExecEvent() );
	IThreadManager_->CreateThread( NEW DrawThread( ev ) );
	IThreadManager_->CreateThread( NEW UpdateThread( ev ) );

	IThreadManager_->WaitAllThreadEnded(); //全てのスレッドの終了を待つ

	//ライブラリ末期化
	SkyLibController_->Term();
}