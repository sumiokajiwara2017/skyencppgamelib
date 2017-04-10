#pragma once

namespace sky { namespace lib { namespace state {

/// <summary>
/// 関数ステートマシン（単一型用）
/// </summary>
template< class T , typename ID >
class CFunctionStateManager
{

public:

	/// <summary>
	/// 関数型
	/// </summary>
	typedef skyBool ( T::*FUNCTION )( const fps::CFpsController & );
	typedef void    ( T::*FUNCTIONSUB )();
	typedef skyBool ( T::*FUNCTIONLOAD )();
	typedef CFifo< ID >              StateQueue;
	typedef CList< u32 , ID >        StateList;

	/// <summary>
	/// ステート関数情報
	/// </summary>
	struct FUNCTIONData
	{
		/// <summary>
		/// ステート関数群
		/// </summary>
		FUNCTIONLOAD m_Load;   //ロードが完了したらskyTrueを返すGetNextState関数で次のステートのIDを得て次のステートのm_Loadを呼んでskyTrueが来るまで待って現在のステートを描画し、skyTrueが来てから遷移したらロードレスになる
		FUNCTIONSUB  m_Init;   //初めてステートに来た時に一回だけ
		FUNCTIONSUB  m_Start;  //ステートが来る度に一回だけ
		FUNCTION     m_Update; //毎フレーム更新
		FUNCTIONSUB  m_End;    //ステートが離れる度に一度だけ

		/// <summary>
		/// フラグ
		/// </summary>
		skyBool      m_IsInitExec;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		void Constractor()
		{
			m_Load   = skyNull;
			m_Init   = skyNull;
			m_Start  = skyNull;
			m_Update = skyNull;
			m_End    = skyNull;
			m_IsInitExec = skyFalse;
		}
		FUNCTIONData()
		{
			Constractor();
		}
		FUNCTIONData( FUNCTIONLOAD load , FUNCTIONSUB init , FUNCTIONSUB start , FUNCTION update , FUNCTIONSUB end )
		{
			Constractor();
			m_Load  = load;
			m_Init  = init;
			m_Start  = start;
			m_Update = update;
			m_End    = end;
		}
		FUNCTIONData( FUNCTIONSUB init , FUNCTIONSUB start , FUNCTION update , FUNCTIONSUB end )
		{
			Constractor();
			m_Init  = init;
			m_Start  = start;
			m_Update = update;
			m_End    = end;
		}
		FUNCTIONData( FUNCTIONSUB start , FUNCTION update , FUNCTIONSUB end )
		{
			Constractor();
			m_Start  = start;
			m_Update = update;
			m_End    = end;
		}
		FUNCTIONData( FUNCTIONSUB start , FUNCTION update )
		{
			Constractor();
			m_Start  = start;
			m_Update = update;
		}
		FUNCTIONData( FUNCTION update , FUNCTIONSUB end )
		{
			Constractor();
			m_Update = update;
			m_End    = end;
		}
		FUNCTIONData( FUNCTION update )
		{
			Constractor();
			m_Update = update;
		}
	};

	typedef CList< ID , FUNCTIONData >  FUNCTIONList;

	/// <summary>
	/// 関数を登録
	/// </summary>
	inline void RegisterState( ID id , FUNCTION func )
	{
		m_FunctionList.AddTail( id , FUNCTIONData( func ) );
	}
	inline void RegisterStateData( ID id , const FUNCTIONData &funcData )
	{
		m_FunctionList.AddTail( id , funcData );
	
	}

	/// <summary>
	/// 現在のフレームカウンターの値を取得
	/// </summary>
	inline u32 GetFrameCount()
	{
		return m_FrameCount;
	}

	/// <summary>
	/// 現在のステートを取得
	/// </summary>
	inline ID GetState()
	{
		return m_CurrentState;
	}

	/// <summary>
	/// 次のステートを取得(skyFalseで次が存在しない）
	/// </summary>
	inline skyBool GetNextState( ID &dest )
	{
		skyBool result = skyFalse;
		if ( m_StateQueue.GetNum() > 0 )
		{
			dest = m_StateQueue.GetFront();
		}
		return result;
	}

	/// <summary>
	/// 現在ステートのm_Loadを呼ぶ
	/// </summary>
	inline skyBool ExecCurrentStateLoad()
	{
		skyBool result = skyTrue; //次がなかったら完了とする
		if( m_CurrentFunction.m_Load != skyNull )
		{
			result = ( m_pObject->*m_CurrentFunction.m_Load )();
		}
		return result;
	}

	/// <summary>
	/// 次のステートのm_Loadを呼ぶ
	/// </summary>
	inline skyBool ExecNextStateLoad()
	{
		skyBool result = skyTrue; //次がなかったら完了とする

		if ( m_StateQueue.GetNum() > 0 )
		{
			typename FUNCTIONData& next = m_FunctionList.GetIdx( m_StateQueue.GetFront() );

			if( next.m_Load != skyNull )
			{
			    result = ( m_pObject->*next.m_Load )();
			}
		}

		return result;
	}

	/// <summary>
	/// 現在ステートに溜まっているキューの数を取得する
	/// </summary>
	inline u32 GetQueueNum()
	{
		return m_StateQueue.GetNum();
	}

	/// <summary>
	/// 現在のステートを終了させる
	/// </summary>
	inline void EndCurrentState()
	{
		m_IsStateEnd = skyTrue;
	}

	/// <summary>
	/// ステートキューの登録
	/// </summary>
	inline void PushState( ID state )
	{
		m_StateQueue.Push( state );
	}

	/// <summary>
	/// ステートキューのキャンセル
	/// </summary>
	inline void CancelState( ID state )
	{
		m_StateQueue.SetCancelFlag( state , skyTrue );
	}

	/// <summary>
	/// ステートキューの全キャンセル
	/// </summary>
	inline u32 CancelAllState()
	{
		m_StateQueue.AllPop();
	}

	/// <summary>
	/// グローバルステートの追加
	/// </summary>
	inline void AddGlobalState( ID state )
	{
		m_GrobalStateList.AddTail( state );
	}

	/// <summary>
	/// グローバルステートの削除
	/// </summary>
	inline void DeleteGlobalState( ID state )
	{
		m_GrobalStateList.Delete( state );
	}

	/// <summary>
	/// 更新
	/// </summary>
	inline skyBool Update( const fps::CFpsController &fps )
	{
		//実行中フラグ
		skyBool IsExec = skyTrue;

		if ( m_IsStateEnd == skyFalse )
		{
			//ステート関数実行（skyTrueが帰ってきた場合終了する）
			m_IsStateEnd = ( m_pObject->*m_CurrentFunction.m_Update )( fps );

			//フレームカウンターUP
			m_FrameCount++;
		}

		//上記でm_IsStateEndがTrueになった場合間髪いれずに次のステートを発進させる。
		//1フレ空くだけで様々な問題が発生する場合が多いから。
		if ( m_IsStateEnd == skyTrue )
		{
			//キャンセルされていないステートを先頭にする
			skyBool isSeach = skyTrue;
			while( m_StateQueue.GetNum() > 0 && isSeach )
			{
				if ( m_StateQueue.IsFrontCancel() )
				{
					m_StateQueue.Pop();
				}
				else
				{
					isSeach = skyFalse;
				}
			}

			//ステート取得
			if ( m_StateQueue.GetNum() > 0 )
			{
				if ( m_CurrentFunction.m_End != skyNull )
				{
					( m_pObject->*m_CurrentFunction.m_End )();
				}

				//前のステートＩＤを取っておく（何かの判定に使用する）
				m_PrevState    = m_CurrentState;

				m_StateQueue.Pop( m_CurrentState );

                SKY_ASSERT_MSG( m_CurrentState < ( s32 )m_FunctionList.GetNum() , _T( "This has the possibility of the state not registered. Please used RegisterState().\n" ) );

				m_CurrentFunction = m_FunctionList.GetIdx( m_CurrentState );
				m_FrameCount  = 0;
				m_IsStateEnd   = skyFalse;

				if ( m_CurrentFunction.m_Init != skyNull && m_CurrentFunction.m_IsInitExec == skyFalse )
				{
					( m_pObject->*m_CurrentFunction.m_Init )();
					m_CurrentFunction.m_IsInitExec = skyTrue;
				}

				if ( m_CurrentFunction.m_Start != skyNull )
				{
					( m_pObject->*m_CurrentFunction.m_Start )();
				}
			}
			else
			{
				//何も実行していない
				IsExec = skyFalse;
			}
		}

		//グローバルステートの実行
		FOREACH( StateList , it , m_GrobalStateList )
		{
			typename FUNCTIONData& function = m_FunctionList.GetIdx( *it );
		    ( m_pObject->*function.m_Update )( fps );
		}

		return IsExec;
	}

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CFunctionStateManager( T *pParent , u32 queueSize = fifo::DEFAULT_FIFO_SIZE ) : 
	m_FrameCount( 0 ) , m_IsStateEnd( skyTrue ) , m_StateQueue( queueSize ) , m_CurrentFunction( skyNull )
	{
		m_pObject = pParent;
	}

private:
	
	/// <summary>
	/// 親クラス
	/// </summary>
	T* m_pObject;

	/// <summary>
	/// 関数リスト
	/// </summary>
	FUNCTIONList m_FunctionList;

	/// <summary>
	/// 更新
	/// </summary>
	u32          m_FrameCount;

	/// <summary>
	/// ステート終了フラグ
	/// </summary>
	skyBool      m_IsStateEnd;

	/// <summary>
	/// ステート
	/// </summary>
	ID           m_PrevState;
	ID           m_CurrentState;
	FUNCTIONData m_CurrentFunction;

	/// <summary>
	/// ステートキュー
	/// </summary>
	StateQueue  m_StateQueue;

	/// <summary>
	/// グローバルステート（通常のステートの後に呼ばれる）
	/// </summary>
	StateList m_GrobalStateList;
};

class FUNCTIONDelegateDataBase;
//typedef SmartPointer< FUNCTIONDelegateDataBase >	FUNCTIONDelegateDataBasePtr;				//スマートポインタ版定義
typedef FUNCTIONDelegateDataBase*	                FUNCTIONDelegateDataBasePtr;				//ポインタ定義
typedef CList< u32 , FUNCTIONDelegateDataBasePtr >  FUNCTIONDelegateDataBasePtrList;

/// <summary>
/// ステート関数情報基本クラス
/// </summary>
struct FUNCTIONDelegateDataBase : public base::SkyRefObject
{
	/// <summary>
	/// ロードを呼ぶ
	/// </summary>
	virtual skyBool Load() = 0;

	/// <summary>
	/// 初期化を呼ぶ
	/// </summary>
	virtual void    Init() = 0; 

	/// <summary>
	/// 開始を呼ぶ
	/// </summary>
	virtual void    Start() = 0; 

	/// <summary>
	/// 更新を呼ぶ
	/// </summary>
	virtual skyBool Update( const fps::CFpsController & ) = 0; 

	/// <summary>
	/// 終了を呼ぶ
	/// </summary>
	virtual void    End() = 0; 
};

/// <summary>
/// ステート関数情報
/// </summary>
template< class T >
struct FUNCTIONDelegateData : public FUNCTIONDelegateDataBase
{
	typedef skyBool ( T::*FUNCTION )( const fps::CFpsController & );
	typedef void    ( T::*FUNCTIONSUB )();
	typedef skyBool ( T::*FUNCTIONLOAD )();

	/// <summary>
	/// デリゲート型
	/// </summary>
	T*           m_pObject;

	/// <summary>
	/// フラグ
	/// </summary>
	skyBool      m_IsInitExec;

	/// <summary>
	/// ステート関数群
	/// </summary>
	FUNCTIONLOAD m_Load;   //ロードが完了したらskyTrueを返すGetNextState関数で次のステートのIDを得て次のステートのm_Loadを呼んでskyTrueが来るまで待って現在のステートを描画し、skyTrueが来てから遷移したらロードレスになる
	FUNCTIONSUB  m_Init;   //初めてステートに来た時に一回だけ
	FUNCTIONSUB  m_Start;  //ステートが来る度に一回だけ
	FUNCTION     m_Update; //毎フレーム更新
	FUNCTIONSUB  m_End;    //ステートが離れる度に一度だけ

	/// <summary>
	/// コンストラクタ
	/// </summary>
	void Constractor()
	{
		m_pObject = skyNull;
		m_Load    = skyNull;
		m_Init    = skyNull;
		m_Start   = skyNull;
		m_Update  = skyNull;
		m_End     = skyNull;
		m_IsInitExec = skyFalse;
	}
	FUNCTIONDelegateData()
	{
		Constractor();
	}
	FUNCTIONDelegateData( T* pObject , FUNCTION update , FUNCTIONSUB init = skyNull , FUNCTIONLOAD load = skyNull , FUNCTIONSUB start = skyNull , FUNCTIONSUB end = skyNull )
	{
		Constractor();
		m_pObject = pObject;
		m_Load    = load;
		m_Init    = init;
		m_Start   = start;
		m_Update  = update;
		m_End     = end;
	}

	/// <summary>
	/// 実装
	/// </summary>
	skyBool Load()
	{
		if ( m_Load == skyNull ) return skyTrue;
		return ( m_pObject->*m_Load )();
	}
	void Init()
	{
		if ( m_Init == skyNull ) return;
		( m_pObject->*m_Init )();
	}
	void Start()
	{
		if ( m_Start == skyNull ) return;
		( m_pObject->*m_Start )();
	}
	skyBool Update( const fps::CFpsController &fps )
	{
		if ( m_Update == skyNull ) return skyTrue;
		return ( m_pObject->*m_Update )( fps );
	}
	void End()
	{
		if ( m_End == skyNull ) return;
		( m_pObject->*m_End )();
	}
};

template< typename ID >
class CFunctionDelgateStateManager
{

public:

	typedef CFifo< ID >              StateQueue;
	typedef CList< u32 , ID >        StateList;

	/// <summary>
	/// 現在ステートのm_Loadを呼ぶ
	/// </summary>
	inline skyBool ExecCurrentStateLoad()
	{
		if ( m_CurrentFunction != skyNull )
		{
			return m_CurrentFunction->Load();
		}
		else
		{
			return skyTrue;
		}
	}

	/// <summary>
	/// 次のステートのm_Loadを呼ぶ
	/// </summary>
	inline skyBool ExecNextStateLoad()
	{
		skyBool result = skyTrue; //次がなかったら完了とする

		if ( m_StateQueue.GetNum() > 0 )
		{
			typename FUNCTIONDelegateDataBasePtr &next = m_FunctionList.GetIdx( m_StateQueue.GetFront() );

		    result = next->Load();
		}

		return result;
	}

	/// <summary>
	/// ステートの登録
	/// </summary>
	inline void RegisterStateData( ID id , const FUNCTIONDelegateDataBasePtr &funcData )
	{
		m_FunctionList.AddTail( id , funcData );
	}

	/// <summary>
	/// 現在ステートに溜まっているキューの数を取得する
	/// </summary>
	inline u32 GetQueueNum()
	{
		return m_StateQueue.GetNum();
	}

	/// <summary>
	/// 現在のステートを終了させる
	/// </summary>
	inline void EndCurrentState()
	{
		m_IsStateEnd = skyTrue;
	}

	/// <summary>
	/// ステートキューの登録
	/// </summary>
	inline void PushState( ID state )
	{
		m_StateQueue.Push( state );
	}

	/// <summary>
	/// ステートキューのキャンセル
	/// </summary>
	inline void CancelState( ID state )
	{
		m_StateQueue.SetCancelFlag( state , skyTrue );
	}

	/// <summary>
	/// ステートキューの全キャンセル
	/// </summary>
	inline u32 CancelAllState()
	{
		m_StateQueue.AllPop();
	}

	/// <summary>
	/// グローバルステートの追加
	/// </summary>
	inline void AddGlobalState( ID state )
	{
		m_GrobalStateList.AddTail( state );
	}

	/// <summary>
	/// グローバルステートの削除
	/// </summary>
	inline void DeleteGlobalState( ID state )
	{
		m_GrobalStateList.Delete( state );
	}

	/// <summary>
	/// 更新
	/// </summary>
	inline skyBool Update( const fps::CFpsController &fps )
	{
		//実行中フラグ
		skyBool IsExec = skyTrue;

		if ( m_IsStateEnd == skyFalse )
		{
			//ステート関数実行（skyTrueが帰ってきた場合終了する）
			m_IsStateEnd = m_CurrentFunction->Update( fps );

			//ステートが終わったら終了
			if( m_IsStateEnd == skyTrue )
			{
				m_CurrentFunction->End();
			}

			//フレームカウンターUP
			m_FrameCount++;
		}

		//上記でm_IsStateEndがTrueになった場合間髪いれずに次のステートを発進させる。
		//1フレ空くだけで様々な問題が発生する場合が多いから。
		if ( m_IsStateEnd == skyTrue )
		{
			//キャンセルされていないステートを先頭にする
			skyBool isSeach = skyTrue;
			while( m_StateQueue.GetNum() > 0 && isSeach )
			{
				if ( m_StateQueue.IsFrontCancel() )
				{
					m_StateQueue.Pop();
				}
				else
				{
					isSeach = skyFalse;
				}
			}

			//ステート取得
			if ( m_StateQueue.GetNum() > 0 )
			{
				//前のステートＩＤを取っておく（何かの判定に使用する）
				m_PrevState    = m_CurrentState;

				m_StateQueue.Pop( m_CurrentState );

                SKY_ASSERT_MSG( m_CurrentState < ( s32 )m_FunctionList.GetNum() , _T( "This has the possibility of the state not registered. Please used RegisterState().\n" ) );

				m_CurrentFunction = m_FunctionList.GetIdx( m_CurrentState );
				m_FrameCount  = 0;
				m_IsStateEnd   = skyFalse;

				m_CurrentFunction->Init();

				m_CurrentFunction->Start();
			}
			else
			{
				//何も実行していない
				IsExec = skyFalse;
			}
		}

		//グローバルステートの実行
		FOREACH( StateList , it , m_GrobalStateList )
		{
			typename FUNCTIONDelegateDataBasePtr& function = m_FunctionList.GetIdx( *it );
			function->Update( fps );
		}

		return IsExec;
	}

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CFunctionDelgateStateManager( u32 queueSize = fifo::DEFAULT_FIFO_SIZE ) : 
	m_FrameCount( 0 ) , m_IsStateEnd( skyTrue ) , m_StateQueue( queueSize ) , m_CurrentFunction( skyNull )
	{
	}

private:
	
	/// <summary>
	/// 関数リスト
	/// </summary>
	FUNCTIONDelegateDataBasePtrList m_FunctionList;

	/// <summary>
	/// 更新
	/// </summary>
	u32          m_FrameCount;

	/// <summary>
	/// ステート終了フラグ
	/// </summary>
	skyBool      m_IsStateEnd;

	/// <summary>
	/// ステート
	/// </summary>
	ID           m_PrevState;
	ID           m_CurrentState;
	FUNCTIONDelegateDataBasePtr m_CurrentFunction;

	/// <summary>
	/// ステートキュー
	/// </summary>
	StateQueue  m_StateQueue;

	/// <summary>
	/// グローバルステート（通常のステートの後に呼ばれる）
	/// </summary>
	StateList m_GrobalStateList;
};

} } }
