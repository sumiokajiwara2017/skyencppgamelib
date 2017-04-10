#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_THREAD

#include "SkyBaseLib/Thread.h"
#include "SkyBaseLib/GameLoop.h"
#include "SkyBaseLib/Input.h"
#include "SkyBaseLib/Fps.h"
static const u32 FRAME_RATE = 60;

using namespace sky::lib;

#if OFF_

static void HogeFunc1( u32 param1 )
{
	SKY_PRINTF( _T( "HogeFunc1 %d \n" ) , param1 );
}

class CHogeThread : public thread::CThread
{
    SkyTypeDefRTTI;

	CHogeThread() : CThread( 1024 )
	{
	}

	void ThreadMain()
	{
		SKY_PRINTF( _T( "CHogeThread \n" ) );
	}
};
SkyImplementRTTI( CHogeThread , sky::lib::thread::CThread );

TEST( tread2 , 1 ) 
{
	SKY_BASE_LIB_INIT();


	thread::CThreadManager manager;

	//スレッドを生成し管理に登録
	manager.RegisterThread( thread::CThread::Create( 1024 , FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &HogeFunc1 , 333 ) ) ) );
	manager.RegisterThread( thread::CThreadPtr( NEW CHogeThread() ) );
	manager.StartAll();
	manager.EndWaitAll();
	manager.UnRegisterThreadAll();

	SKY_BASE_LIB_TERM();
}

#endif

#if OFF_

static void HogeFunc1( u32 param1 )
{
	while( 1 )
	{
		SKY_PRINTF( _T( "HogeFunc1 %d \n" ) , param1 );
		CThreadUtil_::Sleep( 100 );
	}
}

class CHogeThread : public thread::CThread
{
    SkyTypeDefRTTI;

	CHogeThread() : CThread( 1024 )
	{
	}

	void ThreadMain()
	{
		while( 1 )
		{
			SKY_PRINTF( _T( "CHogeThread \n" ) );
			CThreadUtil_::Sleep( 100 );
		}
	}
};
SkyImplementRTTI( CHogeThread , sky::lib::thread::CThread );

TEST( tread2 , 2 ) 
{
	SKY_BASE_LIB_INIT();


	thread::CThreadManager manager;

	//スレッドを生成し管理に登録
	manager.RegisterThread( thread::CThread::Create( 1024 , FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &HogeFunc1 , 333 ) ) ) );
	manager.RegisterThread( thread::CThreadPtr( NEW CHogeThread() ) );
	manager.StartAll();

	//FPSコントローラー作成
	fps::CFpsController fpsCtrl;

	//フレームレートの設定
	fpsCtrl.SetFrameRate( FRAME_RATE );

	SKY_PRINTF( _T( "エスケープキーを押下してください。 \n" ) );

	//ゲームループ
	GAME_LOOP_START
	{
		//フレーム開始
		fpsCtrl.StartFrame();

		//入力管理更新
		Ckeyboard_->Update( fpsCtrl.GetPassageTime() );

		//停止
		if ( Ckeyboard_->IsKeyPress( KEY_S ) )
		{
			manager.StopAll();
		}

		//再開
		if ( Ckeyboard_->IsKeyPress( KEY_R ) )
		{
			manager.ReStartAll();
		}


		//終了判定
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) )
		{
			GAME_LOOP_EXIT();
		}

		//フレームレート維持のため、時間が余ったら待つ
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	manager.EndAll(); //強制停止
	manager.EndWaitAll();
	manager.UnRegisterThreadAll();

	SKY_BASE_LIB_TERM();
}

#endif

#if OFF_

static thread::CriticalSection *s_Cs;

static u32 s_Count = 0;
static void ExecFuncPrint( u32 param )
{
	thread::CriticalLock lock( s_Cs );

	SKY_PRINTF( "=======Start( %d )========\n" , param );
	SKY_PRINTF( "%d \n" , s_Count++ );
	SKY_PRINTF( "%d \n" , s_Count++ );
	SKY_PRINTF( "%d \n" , s_Count++ );
	SKY_PRINTF( "%d \n" , s_Count++ );
	SKY_PRINTF( "%d \n" , s_Count++ );
	SKY_PRINTF( "%d \n" , s_Count++ );
	SKY_PRINTF( "%d \n" , s_Count++ );
	SKY_PRINTF( "========End( %d )========\n" , param );
	s_Count = 0;
}

static skyBool s_ExecThread = skyTrue;

static void HogeFunc1( u32 param1 )
{
	while( s_ExecThread )
	{
		ExecFuncPrint( 1 );
		CThreadUtil_::Sleep( 100 );
	}
}

class CHogeThread : public thread::CThread
{
    SkyTypeDefRTTI;

	CHogeThread() : CThread( 1024 )
	{
	}

	void ThreadMain()
	{
		while( s_ExecThread )
		{
			ExecFuncPrint( 2 );
			CThreadUtil_::Sleep( 100 );
		}
	}
};
SkyImplementRTTI( CHogeThread , sky::lib::thread::CThread );


TEST( tread2 , 3 ) 
{
	SKY_BASE_LIB_INIT();

	s_Cs = thread::CriticalSection::Create();

	thread::CThreadManager manager;

	//スレッドを生成し管理に登録
	manager.RegisterThread( thread::CThread::Create( 1024 , FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &HogeFunc1 , 333 ) ) ) );
	manager.RegisterThread( thread::CThreadPtr( NEW CHogeThread() ) );
	manager.StartAll();

	//FPSコントローラー作成
	fps::CFpsController fpsCtrl;

	//フレームレートの設定
	fpsCtrl.SetFrameRate( FRAME_RATE );

	SKY_PRINTF( _T( "エスケープキーを押下してください。 \n" ) );

	//ゲームループ
	GAME_LOOP_START
	{
		//フレーム開始
		fpsCtrl.StartFrame();

		//入力管理更新
		Ckeyboard_->Update( fpsCtrl.GetPassageTime() );

		//終了判定
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) )
		{
			GAME_LOOP_EXIT();
		}

		//フレームレート維持のため、時間が余ったら待つ
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	s_ExecThread = skyFalse; //スレッドを停止に導く
	manager.EndWaitAll();
	manager.UnRegisterThreadAll();

	thread::CriticalSection::Delete( s_Cs );

	SKY_BASE_LIB_TERM();
}

#endif

#if ON_

static void ExecFuncPrint( u32 param )
{
	SKY_PRINTF( _T( "Work:%d \n" ) , param );
}

TEST( tread2 , 3 ) 
{
	SKY_BASE_LIB_INIT();

	thread::CWorkerThreadManager workerManager;
	workerManager.StartWorker( 5 , 1024 );

	//FPSコントローラー作成
	fps::CFpsController fpsCtrl;

	//フレームレートの設定
	fpsCtrl.SetFrameRate( FRAME_RATE );

	SKY_PRINTF( _T( "エスケープキーを押下してください。 \n" ) );

	//ゲームループ
	GAME_LOOP_START
	{
		//フレーム開始
		fpsCtrl.StartFrame();

		//入力管理更新
		Ckeyboard_->Update( fpsCtrl.GetPassageTime() );

		//仕事実行
		if ( Ckeyboard_->IsKeyPress( KEY_W ) )
		{
			static u32 count = 0;
			workerManager.RequestWork( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &ExecFuncPrint , count ) ) );
			count++;
		}

		//終了判定
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) )
		{
			GAME_LOOP_EXIT();
		}

		//フレームレート維持のため、時間が余ったら待つ
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	workerManager.EndWorkerRequest();
	workerManager.WaitEndWorker();

	SKY_BASE_LIB_TERM();
}

#endif

#endif