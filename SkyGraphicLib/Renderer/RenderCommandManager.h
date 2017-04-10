#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// レンダリングコマンド管理
/// </summary>
class CRenderCommandManager : public base::SkyObject
{

friend class CRenderThread;

public:

	/// <summary>
	/// ステート
	/// </summary>
	enum eState
	{
		eState_Stop = 0 ,
		eState_Rendering ,
		eState_RenderingStopRequest ,
		eState_RendringStop ,
	};

	/// <summary>
	/// 初期化
	/// </summary>
	static void    Initialize();

	/// <summary>
	/// 末期化
	/// </summary>
	static void    Dispose();

	/// <summary>
	/// キューの作成（なぜ初期化で作成しないかは、どれだけの長さのキューをつくるかを各ゲームで決定してもらいたいから）
	/// </summary>
	static void    CreateQueue( u32 queueSize );

	/// <summary>
	/// コマンドバッファの作成
	/// </summary>
	static void    CreateBuffer();

	/// <summary>
	/// コマンドバッファにコマンドを追加する
	/// </summary>
	static void    InvokeCommand( const CRenderingCommandPtr &command );

	/// <summary>
	/// コマンドバッファを実行してしまう。
	/// </summary>
	static void    FlashBuffer();

	/// <summary>
	/// コマンドバッファをキューに追加する
	/// </summary>
	static void    PushBuffer();

	/// <summary>
	/// コマンドバッファをキューから取得する
	/// </summary>
	static CRenderingCommandPtrList*  PopBuffer();

	/// <summary>
	/// コマンド実行
	/// </summary>
	static void ExecCommand( CRenderingCommandPtrList *pBuffer );

	/// <summary>
	/// 描画停止要求
	/// </summary>
	static void RequestRenderingStop();

	/// <summary>
	/// 描画再開要求
	/// </summary>
	static void RequestRenderingReStart();

	/// <summary>
	/// 描画停止待ち
	/// </summary>
	static void WaiteRenderingStop();

	/// <summary>
	/// 描画中判定
	/// </summary>
	static skyBool IsRenderingExec();

	/// <summary>
	/// スレッド実行
	/// </summary>
	static void StartThread();

	/// <summary>
	/// スレッド停止
	/// </summary>
	static void StopThread();

	/// <summary>
	/// レンダリング直前コールデリゲート
	/// </summary>
	static delegate::CDelegateHandler& PreRendaringEvent();

	/// <summary>
	/// レンダリング直後コールデリゲート
	/// </summary>
	static delegate::CDelegateHandler& PostRendaringEvent();

	/// <summary>
	/// レンダリング直前タスク実行タスク要求
	/// タスクは下記のように作成し、この関数の引数に渡す
	/// 例 ）FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::SetRenderTarget , spRenderTarget , skyFalse ) )
	/// </summary>
	static void AddPreRenderingExecEvent( s32 prio , const CRenderingCommandPtr &spFunc );

	/// <summary>
	/// レンダリング直後タスク実行タスク要求
	/// タスクは下記のように作成し、この関数の引数に渡す
	/// 例 ）FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::SetRenderTarget , spRenderTarget , skyFalse ) )
	/// </summary>
	static void AddPostRenderingExecEvent( s32 prio , const CRenderingCommandPtr &spFunc );

private:

	static eState m_eState;

	/// <summary>
	/// コマンドバッファ
	/// </summary>
	static CRenderingCommandPtrList*          m_pCmmandBuffer;

	/// <summary>
	/// コマンドバッファキュー
	/// </summary>
	static CRenderingCommandPtrListPtrFifo*   m_pCommandBufferQueue;

	/// <summary>
	/// レンダリング直前発生イベント
	/// </summary>
	static delegate::CDelegateHandler*        m_pPreRenderingEventHndl;

	/// <summary>
	/// レンダリング直後発生イベント
	/// </summary>
	static delegate::CDelegateHandler*        m_pPostRenderingEventHndl;

	/// <summary>
	/// レンダリング直前実行コマンドリスト
	/// </summary>
	static CRenderingCommandPtrListMT*          m_pPreRenderingCommand;

	/// <summary>
	/// レンダリング直後実行コマンドリスト
	/// </summary>
	static CRenderingCommandPtrListMT*          m_pPostRenderingCommand;

};

} } }

//インスタンスアクセス文字列
#define CRenderCommandManagerInitialize_()	sky::lib::graphic::CRenderCommandManager::Initialize()
#define CRenderCommandManager_				sky::lib::graphic::CRenderCommandManager
#define CRenderCommandManagerDispose_()		sky::lib::graphic::CRenderCommandManager::Dispose()
