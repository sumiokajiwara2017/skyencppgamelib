#pragma once

//敵クラス
class CEnemmy
{

public:

	//モデル
	graphic::CModelPtr m_spModel;

	//AI情報
	framework::CAIMoveObject m_AIObject;

	//更新
	void Update( const fps::CFpsController &fps );

	//コンストラクタ
	CEnemmy( GameManager *pGameManager );

	//デストラクタ
	virtual ~CEnemmy();

private:

	//ゲーム管理の参照
	GameManager *m_pGameManager;

	//衝突判定情報
	math::CollisionData m_CollisionData;

	//ステート
	enum eState
	{
		eState_Start ,
		eState_Idle ,
	};

	skyBool State_Start( const fps::CFpsController &fps )
	{
		return skyTrue;
	}

	skyBool State_Idle( const fps::CFpsController &fps )
	{
		return ( m_FunctionState.GetQueueNum() > 0 ); //何か命令がくるまでこのステートにとどまり続ける
	}

	//ステート管理
	state::CFunctionStateManager< CEnemmy , eState > m_FunctionState;

	//プロセス管理
	process::CProcessManager m_ProcessManager;

	/// <summary>
	/// 幾何形状取得デリゲート
	/// </summary>
	math::IGeometoryGetDelegateObjectPtr m_spGeometoryGetDelegate;
	const math::CGeometryBase* GeometoryGetDelegate()
	{
		return &m_spModel->GetOBSComputeJoint();
	}

	/// <summary>
	/// 衝突通知デリゲート
	/// </summary>
	math::ICollisionCBDelegateObjectPtr m_spCollisionCBDelegate;
	void CollisionCBDelegate( const math::CGeometryBase *pGeometry , const math::CollisionData *pData , math::CollisionResultInfo *pResultInfo = skyNull )
	{
		g_spFont->PrintfSentence( 32 , 64 , graphic::CColor_Green , 1.0f , _T( "Enemy:[ Hit ]" ) );
	}

};
