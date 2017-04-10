#include "StdAfx.h"
#include "TestGameManager.h"
#include "TestEnemy.h"

#include "TestPlayer.h"

//更新
void CEnemmy::Update( const fps::CFpsController &fps )
{
	//プロセスを更新
	m_ProcessManager.Update( fps );

	//ステートを更新
	m_FunctionState.Update( fps );

	//ＡＩ行動
#define ENEMY_MOVE 6
#if ENEMY_MOVE == 0
	framework::CAISteeringBehaviorsUtil::Seek_Move( fps , GET_BONE( m_pGameManager->m_pPlayer->m_spModel )->GetPosition_XYZ() , m_AIObject , 16 );
#elif ENEMY_MOVE == 1
	framework::CAISteeringBehaviorsUtil::Flee_Move( fps , GET_BONE( m_pGameManager->m_pAIPlayer->m_spModel )->GetPosition_XYZ() , m_AIObject , 16 );
#elif ENEMY_MOVE == 2
	framework::CAISteeringBehaviorsUtil::Arrive_Move( fps , GET_BONE( m_pGameManager->m_pPlayer->m_spModel )->GetPosition_XYZ() , m_AIObject , 1 );
#elif ENEMY_MOVE == 3
	framework::CAISteeringBehaviorsUtil::Pursuit_Move( fps , GET_BONE( m_pGameManager->m_pPlayer->m_spModel )->GetPosition_XYZ() , m_pGameManager->m_pPlayer->m_Velocity , m_pGameManager->m_pPlayer->m_Speed * 8 , GET_BONE( m_pGameManager->m_pPlayer->m_spModel )->GetAxisZ() , m_AIObject );
#elif ENEMY_MOVE == 4
	framework::CAISteeringBehaviorsUtil::Evade_Move( fps , m_pGameManager->m_pAIPlayer->m_AIObject , m_AIObject );
#elif ENEMY_MOVE == 5
	framework::CAISteeringBehaviorsUtil::Wander_Move( fps , m_AIObject , m_spModel->GetMatrix() , math::eAxis_y );
#elif ENEMY_MOVE == 6
	framework::CAISteeringBehaviorsUtil::FollowPath_Move( fps , m_AIObject );
#elif ENEMY_MOVE == 7
	math::CollisionDataPtrList obstacleList;
	g_CollisionManager.GetKindDataList( eObjectKind_Sphere , obstacleList );
	framework::CAISteeringBehaviorsUtil::Hide_Move( fps , GET_BONE( m_pGameManager->m_pPlayer->m_spModel )->GetPosition_XYZ() , obstacleList , m_AIObject );
#endif

	//行動結果をボーンに反映
	m_spModel->GetRotation() = m_AIObject.GetDirectionQuaternion();
	m_spModel->SetPosition_XYZ( m_AIObject.m_Position ); 

	//補正
	math::CBasicVector3 nowPos;
	nowPos = m_spModel->GetPositionMatrix().GetCol3( nowPos );
	math::CBasicVector3 newPos = math::GeometryUtil::ConvertRoopPos( g_SpaceMax , g_SpaceMin , nowPos );
	m_spModel->SetPosition_XYZ( newPos );
	m_AIObject.m_Position = newPos;

	//もし補正がかかった（枠の外に行った）場合は、中心に戻ってくる
	if ( nowPos != newPos )
	{
		m_AIObject.m_Position = math::CBasicVector3_ZERO;
		m_spModel->SetPosition_XYZ( math::CBasicVector3_ZERO );
	}
}

//コンストラクタ
CEnemmy::CEnemmy( GameManager *pGameManager ) : m_FunctionState( this ) , m_pGameManager( pGameManager )
{
	//ゲーム管理に登録
	m_pGameManager->m_pEnemy = this;

	//初期位置と初期向きを設定
	m_AIObject.m_Position = math::CBasicVector3( 0 , 0 , 10 );
	m_AIObject.m_WanderTarget = m_AIObject.m_Position;
	m_AIObject.m_Direction = math::CBasicVector3( 0 , 0 , -1 ).Normalize();

	//幾何形状取得デリゲート作成
	m_spGeometoryGetDelegate.SetPtr( NEW math::CGeometoryGetDelegateAdaptor< CEnemmy >( this , &CEnemmy::GeometoryGetDelegate ) );

	//衝突通知デリゲート
	m_spCollisionCBDelegate.SetPtr( NEW math::CollisionCBDelegateAdaptor< CEnemmy >( this , &CEnemmy::CollisionCBDelegate ) );

	//ステート関数を登録
	m_FunctionState.RegisterState( eState_Start , &CEnemmy::State_Start );
	m_FunctionState.RegisterState( eState_Idle  , &CEnemmy::State_Idle );

	//ステートをキューに投入
	m_FunctionState.PushState( eState_Start );
	m_FunctionState.PushState( eState_Idle );

	//モデルの作成
	m_spModel = graphic::CModelUtil::CreateTestCube( graphic::CColor_Tomato );
	m_spModel->AttachChild( graphic::CModelUtil::CreateStandardLine3( 2 ) );

	m_spModel->GetRotation() = m_AIObject.GetDirectionQuaternion();
	m_spModel->SetPosition_XYZ( m_AIObject.m_Position ); 

	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( m_spModel );
	m_spModel->ComputeBS();   
	m_spModel->AttachChild( graphic::CModelUtil::CreateLine3DSphere( m_spModel->GetOBS() , graphic::CColor_Mistyrose ) ); 

	//衝突判定情報登録
	m_CollisionData.Initialize( eObjectKind_Enemy , skyTrue , m_spGeometoryGetDelegate , m_spCollisionCBDelegate );
	g_CollisionManager.ResisterDataRequest( &m_CollisionData );

	//徘徊行動用パラメーター設定
	m_AIObject.m_Path.AddPath( math::CBasicVector3( 0   , 0 , -20 ) );
	m_AIObject.m_Path.AddPath( math::CBasicVector3( -20  , 0 , 0 ) );
	m_AIObject.m_Path.AddPath( math::CBasicVector3( 20 , 0 , 0 ) );

	m_AIObject.m_Path.SetPathEndType( framework::eAIPathEndType_Back );
}

//デストラクタ
CEnemmy::~CEnemmy()
{
	//衝突判定登録解除
	g_CollisionManager.UnResisterDataRequest( &m_CollisionData );

	//ゲーム管理から登録解除
	m_pGameManager->m_pEnemy = skyNull;
}
