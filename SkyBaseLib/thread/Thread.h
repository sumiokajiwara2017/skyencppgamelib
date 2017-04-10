#pragma once

namespace sky { namespace lib { namespace thread {

class CThread;
typedef SmartPointer< CThread >	CThreadPtr;
typedef CList< u32 , CThreadPtr >	CThreadPtrList;

/// <summary>
/// スレッドクラス
/// 使い方は２種類
/// ①このクラスを継承してThreadMain関数をオーバーライドする。
/// ②関数オブジェクトを登録してそれをコールする。
///
/// スレッドクラスはシンプルで最低限便利な機能を実装する。
/// ミューテックス、セマフォ、イベントはあえて使用しない。
/// 使用したい場合はその時実装する。シンプルならばどの環境でも即実装できるはず。
/// 使用する時も、あまり複雑な問題を引き起こさない、シンプルなシチュエーションで使用すること。
/// 例外処理とコラボするとカオスになるのでなるべく避けること。
/// </summary>
class CThread : public base::SkyRefObject
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CThread();

	/// <summary>
	/// スレッドメイン
	/// </summary>
	virtual void ThreadMain();

	/// <summary>
	/// スレッド開始
	/// </summary>
	void Start();

	/// <summary>
	/// スレッド終了（この処理は外部スレッドからスレッドを強制停止するため、非常に危険です。）
	/// </summary>
	void End();

	/// <summary>
	/// スレッド終了判定
	/// </summary>
	skyBool IsEnd();

	/// <summary>
	/// スレッド終了待ち(内部でループして終了を待ちます)
	/// </summary>
	void EndWait();

	/// <summary>
	/// スレッド一時停止
	/// </summary>
	void Stop();

	/// <summary>
	/// スレッド一時停止再開
	/// </summary>
	void ReStart();

	/// <summary>
	/// スレッドID取得
	/// </summary>
	tread_id GetId();

	/// <summary>
	/// スレッドハンドル取得
	/// </summary>
	tread_hndl GetHndl();

	/// <summary>
	/// スレッド作成
	/// </summary>
	static CThreadPtr Create( u32 stackSize , const FunctionCallBackAsyncPtr &spFunction = FunctionCallBackAsyncPtr() );

protected:

	/// <summary>
	/// スレッドステート
	/// </summary>
	enum eState
	{
		eState_Exec = 0 ,
		eState_End ,
		eState_Suspend ,
		eState_Error ,
	} m_State;

	/// <summary>
	/// ファンクションオブジェクト
	/// </summary>
	FunctionCallBackAsyncPtr m_spFunction;

	/// <summary>
	/// スレッドID
	/// </summary>
	tread_id m_ThreadId;

	/// <summary>
	/// スレッドハンドル
	/// </summary>
	tread_hndl m_ThreadHndl;

	/// <summary>
	/// 全スレッド共通エントリーポイント
	/// </summary>
	static unsigned __stdcall ThreadEntry_( void* pParam );

	/// <summary>
	/// コンストラクタ（作成した時点ではスレッドは実行されません。
	/// </summary>
	CThread( u32 stackSize );

private:

	CThread( u32 stackSize , const FunctionCallBackAsyncPtr &spFunction );
	void Constractor( u32 stackSize );
};

} } }