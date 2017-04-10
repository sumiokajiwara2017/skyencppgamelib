#pragma once

namespace sky { namespace lib { namespace thread {

class CWorkerThreadManager;
typedef SmartPointer< CWorkerThreadManager >	CWorkerThreadManagerPtr;	

/// <summary>
/// ワーカースレッド管理
/// FunctionCallBackAsyncPtrを渡せば余裕のある時実行してくれる。
/// 終了判定はFunctionCallBackAsyncPtrのIsEnd()関数で行う。
/// </summary>
class CWorkerThreadManager : public base::SkyRefObject
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CWorkerThreadManager();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CWorkerThreadManager();

	/// <summary>
	/// ワーカースレッド起動
	/// </summary>
	void StartWorker( u32 workerNum = DEFAULT_WORKER_NUM , u32 stackSize = DEFAULT_STACK_SIZE );

	/// <summary>
	/// ワーカースレッド全体を起こす
	/// ワーカースレッドは仕事が無いと自動で待ち状態に入るため、仕事が来たときに起こさないといけない。
	/// </summary>
	void WakeUpWorker();

	/// <summary>
	/// ワーカースレッド終了要求(仕事中の場合は仕事が終わってから終了します）
	/// </summary>
	void EndWorkerRequest();

	/// <summary>
	/// ワーカースレッド終了待ち
	/// </summary>
	void WaitEndWorker();

	/// <summary>
	/// ワーカースレッド終了判定
	/// </summary>
	skyBool IsEndWoker();

	/// <summary>
	/// 依頼完結判定（実行待ちと実行中の依頼が存在しない場合skyTrue )
	/// </summary>
	skyBool IsEndWork();

	/// <summary>
	/// 依頼完結待ち
	/// </summary>
	void WaitEndWork();

	/// <summary>
	/// 仕事依頼
	/// </summary>
	void RequestWork( const FunctionCallBackAsyncPtr &spWork );

	/// <summary>
	/// 仕事依頼キャンセル（キューの実行依頼からクリアします。既に実行されてしまった場合はユーザーで停止してください。共通処理で強制停止をするのは危険なので。）
	/// </summary>
	void CancelWork( const FunctionCallBackAsyncPtr &spWork );
	void CancelWorkAll();

//---static

	//シングルトン機能を付属
	friend class SingletonPtr< CWorkerThreadManager >;
	static SingletonPtr< CWorkerThreadManager > Singleton;

private:

	/// <summary>
	/// ワーカースレッドリスト
	/// </summary>
	CWorkerThreadPtrList m_WorkerList;

	/// <summary>
	/// 仕事依頼キュー（しっかり排他処理を行う）
	/// </summary>
	CWorkQueuePtr m_spQueue;
};

} } }


//インスタンスアクセス文字列
#define CWorkerThreadManagerCreate_()	sky::lib::thread::CWorkerThreadManager::Singleton.CreateInstance()
#define CWorkerThreadManager_			sky::lib::thread::CWorkerThreadManager::Singleton.GetInstance()
#define CWorkerThreadManagerDelete_()	sky::lib::thread::CWorkerThreadManager::Singleton.DeleteInstance()
