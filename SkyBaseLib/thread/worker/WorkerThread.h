#pragma once

namespace sky { namespace lib { namespace thread {

class CWorkerThread;
typedef SmartPointer< CWorkerThread >	CWorkerThreadPtr;
typedef CList< u32 , CWorkerThreadPtr >	CWorkerThreadPtrList;

/// <summary>
/// ワーカースレッド
/// </summary>
class CWorkerThread : public thread::CThread
{

friend class CWorkerThreadManager;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CWorkerThread();

private:

	/// <summary>
	/// 待ちオブジェクト
	/// </summary>
	CWaitObjectPtr m_spWaitObject;

	/// <summary>
	/// スレッド蓄積キューの参照
	/// </summary>
	CWorkQueuePtr m_spQueue;

	/// <summary>
	/// 有効フラグ
	/// </summary>
	skyBool m_IsValid;

	/// <summary>
	/// 実行
	/// </summary>
	virtual void ThreadMain();

	/// <summary>
	/// 有効化設定
	/// </summary>
	void SetValid( skyBool isValid );

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CWorkerThread( const CWorkQueuePtr& spQueue , u32 stackSize );
};

} } }