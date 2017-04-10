#pragma once

namespace sky { namespace lib { namespace state {

/// <summary>
/// ステート管理クラス
/// アプリケーションの根本の状態管理はこれで行う。
/// ここから起動し、ゲーム起動後はスクリプトに制御を任せるもよし。
/// その場合はスクリプトを管理するステートをＰＵＳＨしておく。
/// ステート切り替えは基本キューに一旦登録する形で行われる。
/// 登録されるので１フレーム後実行となる。
/// ステート管理はスタックで行われるので、ステートが積み上がっていく。
/// なぜキューなのか。それはステート自身が他のステートをPUSHしたり自分自身をPOPしたりするので。
/// </summary>
class CStateManager : public base::SkyRefObject
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CStateManager(){};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CStateManager();

	/// <summary>
	/// ステートの登録
	/// </summary>
	void Register( const hash::CHashStringPtr &name , const IStatePtr &spState );

	/// <summary>
	/// ステートの登録解除
	/// </summary>
	void UnRegister( const hash::CHashStringPtr &name );

	/// <summary>
	/// ステートのＰＵＳＨ
	/// </summary>
	void Push( const hash::CHashStringPtr &name );

	/// <summary>
	/// ステートのＰＯＰ
	/// </summary>
	void Pop();

	/// <summary>
	/// ステートの切り替え（既存のスタックをクリアし、新しいステートを積む）
	/// </summary>
	void Change( const hash::CHashStringPtr &name );

	/// <summary>
	/// ステートの全破棄
	/// </summary>
	void Clear();

	/// <summary>
	/// 更新
	/// </summary>
	void Update( const fps::CFpsController &fps );

	/// <summary>
	/// 指定のステートのスタック上の上に指定のステートが乗っているかチェックする
	/// </summary>
	skyBool IsUpState( const hash::CHashStringPtr &name , const hash::CHashStringPtr &upname );

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize	();

	/// <summary>
	/// 末期化
	/// </summary>
	void Dispose		();

//---static

	/// <summary>
	/// インスタンス生成
	/// EventManagerはシステムに1つ存在するが、インスタンスを作成してローカルで使用することもできる
	/// </summary>
    static CStateManagerPtr Create();

	//シングルトン機能を付属
	friend class SingletonPtr< CStateManager >;
	static SingletonPtr< CStateManager > Singleton;

private:

	/// <summary>
	/// 命令種別
	/// </summary>
	enum eOrder
	{
		eOrder_Push = 0,
		eOrder_Pop,
		eOrder_Change,
		eOrder_Clear,
	};

	/// <summary>
	/// 命令情報
	/// </summary>
	struct COrderInfo : public base::SkyRefObject
	{
		eOrder     eId;
		IStatePtr  spState;
	};
	typedef CMemoryPool< COrderInfo >   COrderInfoPool;
	typedef COrderInfo *				COrderInfoPtr;
	typedef CFifo< COrderInfoPtr >	    COrderInfoPtrFifo;

	/// <summary>
	/// 登録ステートハッシュ
	/// </summary>
	IStatePtrHash		m_RegisterStateHash;

	/// <summary>
	/// ステートスタック
	/// </summary>
	IStatePtrStack		m_ActiveStateStack;

	/// <summary>
	/// 命令キュー
	/// </summary>
	COrderInfoPtrFifo   m_OrderFifo;

	/// <summary>
	/// 命令キュー情報プール
	/// </summary>
	COrderInfoPool		m_OrderInfoPool;

	/// <summary>
	/// 命令をキューに積む
	/// </summary>
	void PushOrder( eOrder eId , const IStatePtr  &spState = IStatePtr() );

	/// <summary>
	/// 命令キューを実行する
	/// </summary>
	void ExecOrderFifo();

	/// <summary>
	/// 命令を実行する
	/// </summary>
	void ExecOrder( COrderInfoPtr pOrderInfo );
};

//インスタンスアクセス文字列
#define CStateManagerCreate_()	sky::lib::state::CStateManager::Singleton.CreateInstance()
#define CStateManager_			sky::lib::state::CStateManager::Singleton.GetInstance()
#define CStateManagerDelete_()	sky::lib::state::CStateManager::Singleton.DeleteInstance()

} } }
