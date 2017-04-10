#pragma once

//�v���C���[�N���X
class CPlayer
{

public:

	//���f��
	graphic::CModelPtr m_spModel;

	//�X�s�[�h
	dectype m_Speed;

	//���݂̑��x
	math::CBasicVector3 m_Velocity;

	//�R���X�g���N�^
	CPlayer( GameManager *pGameManager );

	//�f�X�g���N�^
	virtual ~CPlayer();

	//�X�V
	void Update( const fps::CFpsController &fps );

private:

	//�Q�[���Ǘ��̎Q��
	GameManager *m_pGameManager;

	//�Փ˔�����
	math::CollisionData m_CollisionData;

	//�X�e�[�g
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
		return ( m_FunctionState.GetQueueNum() > 0 ); //�������߂�����܂ł��̃X�e�[�g�ɂƂǂ܂葱����
	}

	//�X�e�[�g�Ǘ�
	state::CFunctionStateManager< CPlayer , eState > m_FunctionState;

	//�v���Z�X�Ǘ�
	process::CProcessManager m_ProcessManager;

	/// <summary>
	/// �L�[�{�[�h�C�x���g�f���Q�[�g�i�L�[�{�[�h�C�x���g�n���h���ɓo�^����j
	/// </summary>
	delegate::IDelegateObjectPtr m_spKeyBordEventDelegate;
	void KeyBordEventDelegate( const CommonSmartPointer &spSender = CommonSmartPointer() , const delegate::CDelegateArgs &args = delegate::CDelegateArgs() )
	{
		math::CQuaternion rot;
		math::CBasicVector3   trans( 0 , 0 , 0 );
		skyBool isChange = skyFalse;

		//�L�[�{�[�h���͎�t
		if ( Ckeyboard_->IsKeyRepeat( KEY_PAGE_UP ) )
		{//�i�s����������]
			rot.SetToRotateAboutAxis( m_spModel->GetAxisX() , math::CMathUtil::DegToRad( -8 ) );
			isChange = skyTrue;
		}
		else if ( Ckeyboard_->IsKeyRepeat( KEY_PAGE_DOWN ) )
		{//�i�s�������E��]
			rot.SetToRotateAboutAxis( m_spModel->GetAxisX() , math::CMathUtil::DegToRad( 8 ) );
			isChange = skyTrue;
		}

		//�L�[�{�[�h���͎�t
		if ( Ckeyboard_->IsKeyRepeat( KEY_LEFT ) )
		{//�i�s����������]
			rot.SetToRotateAboutAxis( m_spModel->GetAxisY() , math::CMathUtil::DegToRad( -8 ) );
			isChange = skyTrue;
		}
		else if ( Ckeyboard_->IsKeyRepeat( KEY_RIGHT ) )
		{//�i�s�������E��]
			rot.SetToRotateAboutAxis( m_spModel->GetAxisY() , math::CMathUtil::DegToRad( 8 ) );
			isChange = skyTrue;
		}

		if ( Ckeyboard_->IsKeyRepeat( KEY_UP ) )
		{//�i�s�����ɉ��Z
			trans = m_spModel->GetAxisZ() * m_Speed;
			isChange = skyTrue;
		}
		else if ( Ckeyboard_->IsKeyRepeat( KEY_DOWN ) )
		{//�i�s�����Ɍ��Z
			trans = -( m_spModel->GetAxisZ() * m_Speed );
			isChange = skyTrue;
		}

		if ( isChange == skyTrue )
		{
			//��]�ƕ��s�ړ��𔽉f
			m_spModel->GetRotation() *= rot;
			m_spModel->AddPosition_XYZ( trans );

			//�␳
			math::CBasicVector3 nowPos;
			nowPos = m_spModel->GetPositionMatrix().GetCol3( nowPos );
			math::CBasicVector3 newPos = math::GeometryUtil::ConvertRoopPos( g_SpaceMax , g_SpaceMin , nowPos );
			m_spModel->SetPosition_XYZ( newPos );

			//���̃t���[���̑��x������
			m_Velocity = trans;
		}
	}

	/// <summary>
	/// �}�E�X�C�x���g�f���Q�[�g�i�}�E�X�C�x���g�n���h���ɓo�^����j
	/// </summary>
	delegate::IDelegateObjectPtr m_spMouseEventDelegate;
	void MouseEventDelegate( const CommonSmartPointer &spSender = CommonSmartPointer() , const delegate::CDelegateArgs &args = delegate::CDelegateArgs() )
	{
	}

	/// <summary>
	/// �􉽌`��擾�f���Q�[�g
	/// </summary>
	math::IGeometoryGetDelegateObjectPtr m_spGeometoryGetDelegate;
	const math::CGeometryBase* GeometoryGetDelegate()
	{
//		return &m_spModel->GetOBSComputeJoint();
		return &m_spModel->GetOBB3ComputeJoint();
//		return &m_spModel->GetAABB3ComputeJoint();
	}

	/// <summary>
	/// �Փ˒ʒm�f���Q�[�g
	/// </summary>
	math::ICollisionCBDelegateObjectPtr m_spCollisionCBDelegate;
	void CollisionCBDelegate( const math::CGeometryBase *pGeometry , const math::CollisionData *pData , math::CollisionResultInfo *pResultInfo = skyNull )
	{
		g_spFont->PrintfSentence( 32 , 48 , graphic::CColor_Green , 1.0f , _T( "Player:[ Hit %d ]" ) , pData->GetKindId() );
	}
};


//�����`�h�N���X
class CPlayerAI
{

public:

	//���f��
	graphic::CModelPtr m_spModel;

	//AI���
	framework::CAIMoveObject m_AIObject;

	//�X�V
	void Update( const fps::CFpsController &fps );

	//�R���X�g���N�^
	CPlayerAI( GameManager *pGameManager );

	//�f�X�g���N�^
	virtual ~CPlayerAI();

private:

	//�Q�[���Ǘ��̎Q��
	GameManager *m_pGameManager;

	//�Փ˔�����
	math::CollisionData m_CollisionData;

	//�X�e�[�g
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
		return ( m_FunctionState.GetQueueNum() > 0 ); //�������߂�����܂ł��̃X�e�[�g�ɂƂǂ܂葱����
	}

	//�X�e�[�g�Ǘ�
	state::CFunctionStateManager< CPlayerAI , eState > m_FunctionState;

	//�v���Z�X�Ǘ�
	process::CProcessManager m_ProcessManager;

	/// <summary>
	/// �􉽌`��擾�f���Q�[�g
	/// </summary>
	math::IGeometoryGetDelegateObjectPtr m_spGeometoryGetDelegate;
	const math::CGeometryBase* GeometoryGetDelegate()
	{
		return &m_spModel->GetOBSComputeJoint();
	}

	/// <summary>
	/// �Փ˒ʒm�f���Q�[�g
	/// </summary>
	math::ICollisionCBDelegateObjectPtr m_spCollisionCBDelegate;
	void CollisionCBDelegate( const math::CGeometryBase *pGeometry , const math::CollisionData *pData , math::CollisionResultInfo *pResultInfo = skyNull )
	{
		g_spFont->PrintfSentence( 32 , 80 , graphic::CColor_Green , 1.0f , _T( "AIPlayer:[ Hit ]" ) );
	}

};
