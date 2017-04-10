#pragma once

//プレイヤークラス
class CPlayer
{

public:

	//モデル
	graphic::CModelPtr m_spModel;

	//スピード
	dectype m_Speed;

	//現在の速度
	math::CBasicVector3 m_Velocity;

	//コンストラクタ
	CPlayer( GameManager *pGameManager );

	//デストラクタ
	virtual ~CPlayer();

	//更新
	void Update( const fps::CFpsController &fps );

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
	state::CFunctionStateManager< CPlayer , eState > m_FunctionState;

	//プロセス管理
	process::CProcessManager m_ProcessManager;

	/// <summary>
	/// キーボードイベントデリゲート（キーボードイベントハンドラに登録する）
	/// </summary>
	delegate::IDelegateObjectPtr m_spKeyBordEventDelegate;
	void KeyBordEventDelegate( const CommonSmartPointer &spSender = CommonSmartPointer() , const delegate::CDelegateArgs &args = delegate::CDelegateArgs() )
	{
		math::CQuaternion rot;
		math::CBasicVector3   trans( 0 , 0 , 0 );
		skyBool isChange = skyFalse;

		//キーボード入力受付
		if ( Ckeyboard_->IsKeyRepeat( KEY_PAGE_UP ) )
		{//進行方向を左回転
			rot.SetToRotateAboutAxis( m_spModel->GetAxisX() , math::CMathUtil::DegToRad( -8 ) );
			isChange = skyTrue;
		}
		else if ( Ckeyboard_->IsKeyRepeat( KEY_PAGE_DOWN ) )
		{//進行方向を右回転
			rot.SetToRotateAboutAxis( m_spModel->GetAxisX() , math::CMathUtil::DegToRad( 8 ) );
			isChange = skyTrue;
		}

		//キーボード入力受付
		if ( Ckeyboard_->IsKeyRepeat( KEY_LEFT ) )
		{//進行方向を左回転
			rot.SetToRotateAboutAxis( m_spModel->GetAxisY() , math::CMathUtil::DegToRad( -8 ) );
			isChange = skyTrue;
		}
		else if ( Ckeyboard_->IsKeyRepeat( KEY_RIGHT ) )
		{//進行方向を右回転
			rot.SetToRotateAboutAxis( m_spModel->GetAxisY() , math::CMathUtil::DegToRad( 8 ) );
			isChange = skyTrue;
		}

		if ( Ckeyboard_->IsKeyRepeat( KEY_UP ) )
		{//進行方向に加算
			trans = m_spModel->GetAxisZ() * m_Speed;
			isChange = skyTrue;
		}
		else if ( Ckeyboard_->IsKeyRepeat( KEY_DOWN ) )
		{//進行方向に減算
			trans = -( m_spModel->GetAxisZ() * m_Speed );
			isChange = skyTrue;
		}

		if ( isChange == skyTrue )
		{
			//回転と並行移動を反映
			m_spModel->GetRotation() *= rot;
			m_spModel->AddPosition_XYZ( trans );

			//補正
			math::CBasicVector3 nowPos;
			nowPos = m_spModel->GetPositionMatrix().GetCol3( nowPos );
			math::CBasicVector3 newPos = math::GeometryUtil::ConvertRoopPos( g_SpaceMax , g_SpaceMin , nowPos );
			m_spModel->SetPosition_XYZ( newPos );

			//このフレームの速度を決定
			m_Velocity = trans;
		}
	}

	/// <summary>
	/// マウスイベントデリゲート（マウスイベントハンドラに登録する）
	/// </summary>
	delegate::IDelegateObjectPtr m_spMouseEventDelegate;
	void MouseEventDelegate( const CommonSmartPointer &spSender = CommonSmartPointer() , const delegate::CDelegateArgs &args = delegate::CDelegateArgs() )
	{
	}

	/// <summary>
	/// 幾何形状取得デリゲート
	/// </summary>
	math::IGeometoryGetDelegateObjectPtr m_spGeometoryGetDelegate;
	const math::CGeometryBase* GeometoryGetDelegate()
	{
//		return &m_spModel->GetOBSComputeJoint();
		return &m_spModel->GetOBB3ComputeJoint();
//		return &m_spModel->GetAABB3ComputeJoint();
	}

	/// <summary>
	/// 衝突通知デリゲート
	/// </summary>
	math::ICollisionCBDelegateObjectPtr m_spCollisionCBDelegate;
	void CollisionCBDelegate( const math::CGeometryBase *pGeometry , const math::CollisionData *pData , math::CollisionResultInfo *pResultInfo = skyNull )
	{
		g_spFont->PrintfSentence( 32 , 48 , graphic::CColor_Green , 1.0f , _T( "Player:[ Hit %d ]" ) , pData->GetKindId() );
	}
};


//味方ＡＩクラス
class CPlayerAI
{

public:

	//モデル
	graphic::CModelPtr m_spModel;

	//AI情報
	framework::CAIMoveObject m_AIObject;

	//更新
	void Update( const fps::CFpsController &fps );

	//コンストラクタ
	CPlayerAI( GameManager *pGameManager );

	//デストラクタ
	virtual ~CPlayerAI();

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
	state::CFunctionStateManager< CPlayerAI , eState > m_FunctionState;

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
		g_spFont->PrintfSentence( 32 , 80 , graphic::CColor_Green , 1.0f , _T( "AIPlayer:[ Hit ]" ) );
	}

};
