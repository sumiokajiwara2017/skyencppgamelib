#pragma once

//�G�N���X
class CEnemmy
{

public:

	//���f��
	graphic::CModelPtr m_spModel;

	//AI���
	framework::CAIMoveObject m_AIObject;

	//�X�V
	void Update( const fps::CFpsController &fps );

	//�R���X�g���N�^
	CEnemmy( GameManager *pGameManager );

	//�f�X�g���N�^
	virtual ~CEnemmy();

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
	state::CFunctionStateManager< CEnemmy , eState > m_FunctionState;

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
		g_spFont->PrintfSentence( 32 , 64 , graphic::CColor_Green , 1.0f , _T( "Enemy:[ Hit ]" ) );
	}

};
