#include "SkyBaseLib/Controller.h"
#include "SkyBaseLib/Thread.h"

static void Work( s32 n )
{
	SKY_PRINTF( L"1から%dまでの和を求めます。スレッド番号:%d \n" , n , IThreadAdptr_->GetCurrentThreadId() );

	s32 total = 0;
	for ( s32 i = 0 ; i <= n ; i++ )
	{
		total += i;
		::Sleep( 10 );		//計算に時間がかかることを表現
	}

	SKY_PRINTF( L"1から%dまでの和は%dです。スレッド番号%d \n" , n , total , IThreadAdptr_->GetCurrentThreadId() );
}

/// <summary>
/// メイン関数
/// </summary>
int _tmain( int argc , _TCHAR *argv[] )
{
	argc;
	argv;

	//ライブラリ初期化
	SkyLibController_->Init();

	sky::lib::thread::ThreadPool	threadPool;
	threadPool.StartThread( 3 );				// スレッドを作成して待機。

	for ( s32 i = 0 ; i < 10 ; i++ )
	{
		sky::lib::FunctionCallBack *fn( sky::lib::FunctionCallBack_v::Create( Work , i * 100 + 10 ) );
		threadPool.Invoke( fn );				// 待機させているスレッドに処理させる。
	}

	threadPool.WaitAllWorkEnded();							// すべての仕事の実行が完了するのを待つ。
	threadPool.ClearAllThread();							// すべてのスレッドを破棄する。

	SKY_PRINTF( L"全部の仕事が終わりました。  \n" );

	//プリント
	PrintMemory( skyTrue );

	//ライブラリ末期化
	SkyLibController_->Term();
}