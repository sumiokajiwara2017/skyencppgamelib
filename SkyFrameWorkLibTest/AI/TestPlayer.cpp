#include "StdAfx.h"
#include "TestGameManager.h"
#include "TestPlayer.h"

#include "TestEnemy.h"

//�R���X�g���N�^
CPlayer::CPlayer( GameManager *pGameManager ) : m_FunctionState( this ) , m_Speed( 1.0f ) , m_pGameManager( pGameManager )
{
	//�Q�[���Ǘ��Ɏ����̎Q�Ƃ�o�^
	m_pGameManager->m_pPlayer = this;

	//�L�[�{�[�h�̃C�x���g�o�^
	m_spKeyBordEventDelegate.SetPtr( NEW delegate::CEventDelegateAdaptor< CPlayer >( this , &CPlayer::KeyBordEventDelegate ) );
	Ckeyboard_->AttachInputListener( m_spKeyBordEventDelegate );

	//�}�E�X�̃C�x���g�o�^
	m_spMouseEventDelegate.SetPtr( NEW delegate::CEventDelegateAdaptor< CPlayer >( this , &CPlayer::MouseEventDelegate ) );
	CMouse_->AttachInputListener( m_spMouseEventDelegate );

	//�􉽌`��擾�f���Q�[�g�쐬
	m_spGeometoryGetDelegate.SetPtr( NEW math::CGeometoryGetDelegateAdaptor< CPlayer >( this , &CPlayer::GeometoryGetDelegate ) );

	//�Փ˒ʒm�f���Q�[�g
	m_spCollisionCBDelegate.SetPtr( NEW math::CollisionCBDelegateAdaptor< CPlayer >( this , &CPlayer::CollisionCBDelegate ) );

	//�X�e�[�g�֐���o�^
	m_FunctionState.RegisterState( eState_Start , &CPlayer::State_Start );
	m_FunctionState.RegisterState( eState_Idle  , &CPlayer::State_Idle );

	//�X�e�[�g���L���[�ɓ���
	m_FunctionState.PushState( eState_Start );
	m_FunctionState.PushState( eState_Idle );

	//�`��p���f�����쐬
	m_spModel = graphic::CModelUtil::CreateTestCone( graphic::CColor_Powderblue );
	m_spModel->AttachChild( graphic::CModelUtil::CreateStandardLine3( 2 ) );
	m_spModel->SetPosition_XYZ( math::CBasicVector3( 0 , 0 , 0 ) ); 
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( m_spModel );
//	m_spModel->ComputeBS();  
	m_spModel->ComputeOBB3();
//	m_spModel->ComputeAABB3();
//	m_spModel->AttachChild( graphic::CModelUtil::CreateLine3DSphere( m_spModel->GetOBS() , graphic::CColor_Mistyrose ) ); 
	m_spModel->AttachChild( graphic::CModelUtil::CreateOBBDisp( m_spModel->GetOBB3() , graphic::CColor_Mistyrose ) ); 
//	m_spModel->AttachChild( graphic::CModelUtil::CreateAABBDisp( m_spModel->GetAABB3() , graphic::CColor_Mistyrose ) ); 

	//�Փ˔�����o�^
	m_CollisionData.Initialize( eObjectKind_Player , skyTrue , m_spGeometoryGetDelegate , m_spCollisionCBDelegate );
	g_CollisionManager.ResisterDataRequest( &m_CollisionData );
}

//�f�X�g���N�^
CPlayer::~CPlayer()
{
	//�Փ˔���o�^����
	g_CollisionManager.UnResisterDataRequest( &m_CollisionData );

	//�Q�[���Ǘ�����o�^����
	m_pGameManager->m_pPlayer = skyNull;
}

//�X�V
void CPlayer::Update( const fps::CFpsController &fps )
{
	//�v���Z�X���X�V
	m_ProcessManager.Update( fps );

	//�X�e�[�g���X�V
	m_FunctionState.Update( fps );
}

//�X�V
void CPlayerAI::Update( const fps::CFpsController &fps )
{
	//�v���Z�X���X�V
	m_ProcessManager.Update( fps );

	//�X�e�[�g���X�V
	m_FunctionState.Update( fps );

	//�`�h�s��
#define ENEMY_MOVE 7
#if ENEMY_MOVE == 0
	framework::CAISteeringBehaviorsUtil::Seek_Move( fps , GET_BONE( m_pGameManager->m_pPlayer->m_spModel )->GetPosition_XYZ() , m_AIObject , 16 );
#elif ENEMY_MOVE == 1
	framework::CAISteeringBehaviorsUtil::Flee_Move( fps , GET_BONE( m_pGameManager->m_pPlayer->m_spModel )->GetPosition_XYZ() , m_AIObject , 16 );
#elif ENEMY_MOVE == 2
	framework::CAISteeringBehaviorsUtil::Arrive_Move( fps , GET_BONE( m_pGameManager->m_pPlayer->m_spModel )->GetPosition_XYZ() , m_AIObject , 1 );
#elif ENEMY_MOVE == 3
	framework::CAISteeringBehaviorsUtil::Pursuit_Move( fps , m_pGameManager->m_pEnemy->m_AIObject , m_AIObject );
#elif ENEMY_MOVE == 4
	framework::CAISteeringBehaviorsUtil::Evade_Move( fps , m_pGameManager->m_pEnemy->m_AIObject , m_AIObject );
#elif ENEMY_MOVE == 5
	framework::CAISteeringBehaviorsUtil::Wander_Move( fps , m_AIObject , m_spModel->GetMatrix() , math::eAxis_y );
#elif ENEMY_MOVE == 6
	framework::CAISteeringBehaviorsUtil::FollowPath_Move( fps , m_AIObject );
#elif ENEMY_MOVE == 7
	math::CollisionDataPtrList obstacleList;
	g_CollisionManager.GetKindDataList( eObjectKind_Sphere , obstacleList );
	framework::CAISteeringBehaviorsUtil::Hide_Move( fps , m_pGameManager->m_pPlayer->m_spModel->GetPosition_XYZ() , obstacleList , m_AIObject );
#endif

	//�s�����ʂ��{�[���ɔ��f
	m_spModel->GetRotation() = m_AIObject.GetDirectionQuaternion();
	m_spModel->SetPosition_XYZ( m_AIObject.m_Position ); 

	//�␳
	math::CBasicVector3 nowPos;
	nowPos = m_spModel->GetPositionMatrix().GetCol3( nowPos );
	math::CBasicVector3 newPos = math::GeometryUtil::ConvertRoopPos( g_SpaceMax , g_SpaceMin , nowPos );
	m_spModel->SetPosition_XYZ( newPos );
	m_AIObject.m_Position = newPos;

	//�����␳�����������i�g�̊O�ɍs�����j�ꍇ�́A���S�ɖ߂��Ă���
	if ( nowPos != newPos )
	{
		m_AIObject.m_Position = math::CBasicVector3_ZERO;
		m_spModel->SetPosition_XYZ( math::CBasicVector3_ZERO );
	}
}

//�R���X�g���N�^
CPlayerAI::CPlayerAI( GameManager *pGameManager ) : m_FunctionState( this ) , m_pGameManager( pGameManager )
{
	//�Q�[���Ǘ��ɓo�^
	m_pGameManager->m_pAIPlayer = this;

	//�����ʒu�Ə���������ݒ�
	m_AIObject.m_Position = math::CBasicVector3( -10 , 0 , 0 );
	m_AIObject.m_Direction = math::CBasicVector3( 0 , 0 , -1 ).Normalize();

	//�􉽌`��擾�f���Q�[�g�쐬
	m_spGeometoryGetDelegate.SetPtr( NEW math::CGeometoryGetDelegateAdaptor< CPlayerAI >( this , &CPlayerAI::GeometoryGetDelegate ) );

	//�Փ˒ʒm�f���Q�[�g
	m_spCollisionCBDelegate.SetPtr( NEW math::CollisionCBDelegateAdaptor< CPlayerAI >( this , &CPlayerAI::CollisionCBDelegate ) );

	//�X�e�[�g�֐���o�^
	m_FunctionState.RegisterState( eState_Start , &CPlayerAI::State_Start );
	m_FunctionState.RegisterState( eState_Idle  , &CPlayerAI::State_Idle );

	//�X�e�[�g���L���[�ɓ���
	m_FunctionState.PushState( eState_Start );
	m_FunctionState.PushState( eState_Idle );

	//���f���̍쐬
	m_spModel = graphic::CModelUtil::CreateTestCube( graphic::CColor_Tomato );
	m_spModel->AttachChild( graphic::CModelUtil::CreateStandardLine3( 2 ) );

	m_spModel->GetRotation() = m_AIObject.GetDirectionQuaternion();
	m_spModel->SetPosition_XYZ( m_AIObject.m_Position ); 

	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( m_spModel );
	m_spModel->ComputeBS();   
	m_spModel->AttachChild( graphic::CModelUtil::CreateLine3DSphere( m_spModel->GetOBS() , graphic::CColor_Mistyrose ) ); 

	//�Փ˔�����o�^
	m_CollisionData.Initialize( eObjectKind_Enemy , skyTrue , m_spGeometoryGetDelegate , m_spCollisionCBDelegate );
	g_CollisionManager.ResisterDataRequest( &m_CollisionData );

	//�������x�̐ݒ�i���x�������Ɠ����悤���Ȃ��j
	m_AIObject.m_Velocity = math::CBasicVector3( 1 , 0 , 0 );

	//�p�j�s���p�p�����[�^�[�ݒ�
	m_AIObject.m_Path.AddPath( math::CBasicVector3( 0   , 0 , 20 ) );
	m_AIObject.m_Path.AddPath( math::CBasicVector3( 20  , 0 , 0 ) );
	m_AIObject.m_Path.AddPath( math::CBasicVector3( -20 , 0 , 0 ) );

	m_AIObject.m_Path.SetPathEndType( framework::eAIPathEndType_Back );
}

//�f�X�g���N�^
CPlayerAI::~CPlayerAI()
{
	//�Փ˔���o�^����
	g_CollisionManager.UnResisterDataRequest( &m_CollisionData );

	//�Q�[���Ǘ�����o�^����
	m_pGameManager->m_pAIPlayer = skyNull;
}
