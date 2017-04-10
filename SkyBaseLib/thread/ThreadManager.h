#pragma once

namespace sky { namespace lib { namespace thread {

/// <summary>
/// スレッド管理
/// スレッド管理にスレッドを登録すれば一括での終了要求、終了待ちができる。
/// </summary>
class CThreadManager : public base::SkyObject
{

public:

	/// <summary>
	/// スレッドを登録する
	/// </summary>
	void RegisterThread( const CThreadPtr &spThread );

	/// <summary>
	/// スレッドを登録解除する
	/// </summary>
	void UnRegisterThread( const CThreadPtr &spThread );
	void UnRegisterThreadAll();

	/// <summary>
	/// スレッド開始
	/// </summary>
	void StartAll();

	/// <summary>
	/// スレッド終了要求
	/// </summary>
	void EndAll();

	/// <summary>
	/// スレッド終了判定
	/// </summary>
	skyBool IsEndAll();

	/// <summary>
	/// スレッド終了待ち(内部でループして終了を待ちます)
	/// </summary>
	void EndWaitAll();

	/// <summary>
	/// スレッド一時停止
	/// </summary>
	void StopAll();

	/// <summary>
	/// スレッド一時停止再開
	/// </summary>
	void ReStartAll();

private:

	/// <summary>
	/// 登録スレッドリスト
	/// </summary>
	CThreadPtrList m_RegisterThreadList;

};

} } }

/*
使用例

using namespace sky::lib;

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

*/