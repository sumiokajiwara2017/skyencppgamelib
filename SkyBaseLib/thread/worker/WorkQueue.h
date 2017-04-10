#pragma once

namespace sky { namespace lib { namespace thread {

class CWorkQueue;
typedef SmartPointer< CWorkQueue >	CWorkQueuePtr;

/// <summary>
/// ワークキュー
/// </summary>
class CWorkQueue : public base::SkyRefObject
{

friend class CWorkerThreadManager;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CWorkQueue();
	
	/// <summary>
	/// スレッドカウントアップ
	/// </summary>
	void IncThreadCount();

	/// <summary>
	/// スレッドカウントダウン
	/// </summary>
	void DecThreadCount();

	/// <summary>
	/// スレッドカウント取得
	/// </summary>
	u32 GetThreadCount();

	/// <summary>
	/// 要求を設定（排他）
	/// </summary>
	void PushRequest( const FunctionCallBackAsyncPtr &spFunc );

	/// <summary>
	/// 要求を取得（排他）
	/// </summary>
	FunctionCallBackAsyncPtr PopRequest();

	/// <summary>
	/// 要求を破棄
	/// </summary>
	void CanselRequest( const FunctionCallBackAsyncPtr &spFunc );
	void CanselAll();

	/// <summary>
	/// 要求数を取得（排他）
	/// </summary>
	u32 GetRequestNum();

	/// <summary>
	/// 実行数をカウントアップ
	/// </summary>
	void IncExecNum();

	/// <summary>
	/// 実行数をカウントダウン
	/// </summary>
	void DecExecNum();

	/// <summary>
	/// 実行数を取得
	/// </summary>
	u32 GetExecNum();

private:

	/// <summary>
	/// クリティカルセクション
	/// </summary>
	CriticalSection *m_pCs;

	/// <summary>
	/// 実行数
	/// </summary>
	u32 m_ExecNum;

	/// <summary>
	/// スレッド数
	/// </summary>
	u32 m_ThreadNum;

	/// <summary>
	/// ワークリスト
	/// </summary>
	FunctionCallBackAsyncPtrList m_WorkList;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CWorkQueue();
};

} } }