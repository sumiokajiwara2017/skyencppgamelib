#pragma once

namespace sky { namespace lib { namespace process {

//-------------------------------< インターフェース >----------------------------------------//

/// <summary>
/// プロセスクラス
/// 複数フレーム間にまたがる処理はすべてプロセスとして扱う。
/// 登録・実行・中断・キャンセルがどんなタイミングでも可能。
/// </summary>
class IProcess : virtual public base::SkyRefObject
{
    SkyTypeDefRTTI;

friend class CProcessManager;

public:

	/// <summary>
	/// 更新結果種別（マネージャーがこれを受け取り処理を分けます）
	/// </summary>
	enum eState
	{
		eState_Wait = 0,
		eState_Initialize,
		eState_Update,
		eState_Dispose, //ここまできたらプロセス管理から勝手に関連付け解除されます。
	};

	/// <summary>
	/// 更新結果種別（マネージャーがこれを受け取り処理を分けます）
	/// </summary>
	enum eResult
	{
		eResult_Continue = 0,	//継続
		eResult_End,			//終了
	};

	/// <summary>
	/// プロセス状態の取得／設定（マネージャーが操作）
	/// </summary>
	virtual eState	vGetState() = 0;
	virtual void	vSetState( eState state ) = 0;

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void	vInitialize() = 0;

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void	vDispose() = 0;

	/// <summary>
	/// 前更新
	/// </summary>
	virtual void	vPreUpdate( const fps::CFpsController &fps ) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual eResult vUpdate( const fps::CFpsController &fps ) = 0;

	/// <summary>
	/// 後更新
	/// </summary>
	virtual void	vPostUpdate( const fps::CFpsController &fps ) = 0;

	/// <summary>
	/// 名前取得
	/// </summary>
	virtual const hash::CHashStringPtr &vGetName() = 0;

	/// <summary>
	/// 優先順位取得／設定
	/// </summary>
	virtual void vSetPrio( u32 prio ) = 0;
	virtual u32	 vGetPrio() = 0;

	/// <summary>
	/// 管理の関連付け／関連付け解除／取得
	/// </summary>
	virtual void  vAttachManager( const CProcessManagerPtr &spManager ) = 0;
	virtual void  vDetachManager() = 0;
	virtual const CProcessManagerPtr &vGetManager() = 0;

	/// <summary>
	/// 子プロセスの関連付け／関連付け解除／再帰初期化・更新・末期化（このプロセスが更新されたら子プロセスも更新されます）
	/// </summary>
	virtual void vAttachChildProcess	( IProcessPtr &process ) = 0;
	virtual void vDetachChildProcess	( IProcessPtr &process ) = 0;
	virtual void vInitializeChildProcess() = 0;
	virtual void vDisposeChildProcess	() = 0;
	virtual void vPreUpdateChildProcess	( const fps::CFpsController &fps ) = 0;
	virtual void vUpdateChildProcess	( const fps::CFpsController &fps ) = 0;
	virtual void vPostUpdateChildProcess( const fps::CFpsController &fps ) = 0;

	/// <summary>
	/// 次のプロセスの関連付け／関連付け解除（このプロセスが終了した次のフレームより開始されます）
	/// </summary>
	virtual void vAttachNextProcess( IProcessPtr &process ) = 0;
	virtual void vDetachnextProcess( IProcessPtr &process ) = 0;

private:

	/// <summary>
	/// 次のプロセスリスト取得
	/// </summary>
	virtual IProcessPtrList	&GetNextProcessList() = 0;

};

//-----------------------------------< 抽象実装 >-------------------------------------------//

class AbsProcess : public IProcess
{

public:

	/// <summary>
	/// 実装
	/// </summary>
	virtual void	vInitialize()                               {  }; ///必要に応じてオーバーライド
	virtual void	vDispose()                                  {  }; //必要に応じてオーバーライド
	virtual void	vPreUpdate( updateTime time )               { SKY_UNUSED_ARG( time ); }; //必要に応じてオーバーライド
	virtual eResult vUpdate( updateTime time )                  { SKY_UNUSED_ARG( time ); return eResult_End;  }; //必要に応じてオーバーライド
	virtual void	vPostUpdate( updateTime time )              { SKY_UNUSED_ARG( time );  }; //必要に応じてオーバーライド

	void   vSetPrio( u32 prio )							        { m_Prio = prio; }
	u32    vGetPrio()										    { return m_Prio; }
	void   vAttachManager( const CProcessManagerPtr &spManager ){ m_spManager = spManager; }
	void   vDetachManager()                                     { m_spManager.Delete(); }
	const  CProcessManagerPtr &vGetManager()                    { return m_spManager; }

	eState vGetState()										    { return m_eState; }
	void   vSetState( eState state )						    { m_eState = state; }
	void   vAttachChildProcess	( IProcessPtr &process )        { m_ChildProcessList.AddTail( process ); };
	void   vDetachChildProcess	( IProcessPtr &process )        { m_ChildProcessList.Delete( process ); };
	void   vInitializeChildProcess()
	{
		FOREACH( IProcessPtrList , it , m_ChildProcessList )
		{
			( *it )->vInitialize();
			( *it )->vInitializeChildProcess();
		}
	}
	void   vDisposeChildProcess	()
	{
		FOREACH( IProcessPtrList , it , m_ChildProcessList )
		{
			( *it )->vDispose();
			( *it )->vDisposeChildProcess();
		}
	}
	void   vPreUpdateChildProcess  ( const fps::CFpsController &fps )		
	{
		FOREACH( IProcessPtrList , it , m_ChildProcessList )
		{
			( *it )->vPreUpdate( fps );
			( *it )->vPreUpdateChildProcess( fps );
		}
	}
	void   vUpdateChildProcess  ( const fps::CFpsController &fps )
	{
		FOREACH( IProcessPtrList , it , m_ChildProcessList )
		{
			( *it )->vUpdate( fps );
			( *it )->vUpdateChildProcess( fps );
		}
	}
	void   vPostUpdateChildProcess  ( const fps::CFpsController &fps )
	{
		FOREACH( IProcessPtrList , it , m_ChildProcessList )
		{
			( *it )->vPostUpdate( fps );
			( *it )->vPostUpdateChildProcess( fps );
		}
	}
	void   vAttachNextProcess	( IProcessPtr &process )	{ m_NextProcessList.AddTail( process ); };
	void   vDetachnextProcess	( IProcessPtr &process )	{ m_NextProcessList.Delete( process ); };
	IProcessPtrList	&GetNextProcessList(){ return m_NextProcessList; }

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~AbsProcess(){};

	/// <summary>
	/// 名前の取得
	/// </summary>
	const hash::CHashStringPtr &vGetName(){ return m_Name; }

private:

	/// <summary>
	/// 自分の管理者
	/// </summary>
	CProcessManagerPtr     m_spManager;

	/// <summary>
	/// 優先順位
	/// </summary>
	u32						m_Prio;

	/// <summary>
	/// 名前
	/// </summary>
	hash::CHashStringPtr	m_Name;

	/// <summary>
	/// 状態
	/// </summary>
	eState					m_eState;

	/// <summary>
	/// 子プロセス
	/// </summary>
	IProcessPtrList			m_ChildProcessList;

	/// <summary>
	/// 次のプロセス
	/// </summary>
	IProcessPtrList			m_NextProcessList;
};


} } }
