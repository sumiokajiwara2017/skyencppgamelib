#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#include "SkyMathLib/Collision.h"
#include "SkyFrameWorkLib/AI.h"

#include "TestGameManager.h"
#include "TestEnemy.h"
#include "TestPlayer.h"

#ifdef TEST_AI

using namespace sky::lib;

#if OFF_

static const skyString *SCREEN_SETUP_NAME       = _T( "Test" ); 
static const skyString *FONT_SCREEN_SETUP_NAME  = _T( "font" ); 

//Window�����ʒu�A�T�C�Y
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//�P�b������̃t���[����
static const u32				_FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//�J����
static graphic::Camera3DFixationPtr  g_spCamera;
static graphic::Camera2DFixationPtr  g_spFontCamera;

//�e�X�g�p���f��
static graphic::CModelPtr     g_TestPlane;
static graphic::CModelPtr     g_TestObstacle_Wall;
static graphic::CModelPtr     g_TestObstacle_Sphere1;
static graphic::CModelPtr     g_TestObstacle_Sphere2;
static graphic::CModelPtr     g_TestObstacle_Sphere3;

//�X�N���[��
static graphic::CScreenObjectPtr    g_spFontScreen;

//��Q�p�Փ˔�����
math::CollisionData g_ObstacleWallCollisionData;
math::CollisionData g_ObstacleSphere1CollisionData;
math::CollisionData g_ObstacleSphere2CollisionData;
math::CollisionData g_ObstacleSphere3CollisionData;

//---------------------------------------------------------------

TEST( Ai , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//�I�����m_�o�^����
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//�t���X�N���[�����[�hOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//--------3DScreen�̃Z�b�g�A�b�v--------��

	g_spCamera = Camera3DFixationCreate_();
	g_spCamera->vSetUp();
	g_spCamera->SetDefaultInputOn( skyTrue );
	graphic::CameraUtil::SetInitPos_Top( g_spCamera.GetPtrNoConst() , 80 );

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( SCREEN_SETUP_NAME );             //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = g_spCamera;                           //�J����
	screenData.WindowRect.X()     = _WIN_X;                               //��ʈʒuX
	screenData.WindowRect.Y()     = _WIN_Y;                               //��ʈʒuY
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���
	screenData.RenderState        = CRenderStateCreate_();                 //�����_�[�X�e�[�g

	//�X�N���[���̃Z�b�g�A�b�v
	g_spScreen = CScreenObject_::Create( screenData );

	//���E�Ǘ��쐬
	GameManager gameManager;

	//�v���C���[�쐬
	CPlayer player( &gameManager );

	//�v���C���[�`�h�쐬
	CPlayerAI playerAI( &gameManager );

	//�G�쐬
	CEnemmy ennemy( &gameManager );

	//���쐬
	g_TestPlane = graphic::CModelUtil::CreateTestPlane(); 
	g_TestPlane->SetPosition_Y( -1.0f ); 
	g_TestPlane->SetScale_XYZ( math::CBasicVector3( 4 , 4 , 4 ) );

	//��Q���쐬
	g_TestObstacle_Wall = graphic::CModelUtil::CreateTestCube( graphic::CColor_Dimgray , math::CBasicVector3( 4 , 1 , 1 ) );
	GET_BONE( g_TestObstacle_Wall )->SetPosition_X( 10 );

	g_TestObstacle_Sphere1 = graphic::CModelUtil::CreateTestSphere( graphic::CColor_Dimgray , math::CBasicVector3( 6 , 6 , 6 ) , math::CBasicVector3( 0 , 0 , 0 ) , math::CBasicVector3( 10  , 0 , 10 ) );
	g_TestObstacle_Sphere2 = graphic::CModelUtil::CreateTestSphere( graphic::CColor_Dimgray , math::CBasicVector3( 7 , 7 , 7 ) , math::CBasicVector3( 0 , 0 , 0 ) , math::CBasicVector3( 10  , 0 , -10 ) );
	g_TestObstacle_Sphere3 = graphic::CModelUtil::CreateTestSphere( graphic::CColor_Dimgray , math::CBasicVector3( 8 , 8 , 8 ) , math::CBasicVector3( 0 , 0 , 0 ) , math::CBasicVector3( -10 , 0 , 10 ) );

	//���f�����V�[���O���t�ɓo�^
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestPlane );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestObstacle_Wall );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestObstacle_Sphere1 );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestObstacle_Sphere2 );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestObstacle_Sphere3 );

	//�Փ˔���
//	g_TestObstacle_Wall->ComputeAABB3(); g_TestObstacle_Wall->AttachChild( graphic::CModelUtil::CreateAABBDisp( g_TestObstacle_Wall->GetAABB3() , graphic::CColor_Mistyrose ) ); 
	g_TestObstacle_Wall->ComputeOBB3(); g_TestObstacle_Wall->AttachChild( graphic::CModelUtil::CreateOBBDisp( g_TestObstacle_Wall->GetOBB3() , graphic::CColor_Mistyrose ) ); 

	g_TestObstacle_Sphere1->ComputeBS(); g_TestObstacle_Sphere1->AttachChild( graphic::CModelUtil::CreateLine3DSphere( g_TestObstacle_Sphere1->GetOBSComputeBone() , graphic::CColor_Mistyrose ) );
	g_TestObstacle_Sphere2->ComputeBS(); g_TestObstacle_Sphere2->AttachChild( graphic::CModelUtil::CreateLine3DSphere( g_TestObstacle_Sphere2->GetOBSComputeBone() , graphic::CColor_Mistyrose ) );
	g_TestObstacle_Sphere3->ComputeBS(); g_TestObstacle_Sphere3->AttachChild( graphic::CModelUtil::CreateLine3DSphere( g_TestObstacle_Sphere3->GetOBSComputeBone() , graphic::CColor_Mistyrose ) );
/*
	g_TestObstacle_Sphere1->ComputeOBB3(); g_TestObstacle_Sphere1->AttachChild( graphic::CModelUtil::CreateOBBDisp( g_TestObstacle_Sphere1->GetOBB3ComputeBone() , graphic::CColor_Mistyrose ) );
	g_TestObstacle_Sphere2->ComputeOBB3(); g_TestObstacle_Sphere2->AttachChild( graphic::CModelUtil::CreateOBBDisp( g_TestObstacle_Sphere2->GetOBB3ComputeBone() , graphic::CColor_Mistyrose ) );
	g_TestObstacle_Sphere3->ComputeOBB3(); g_TestObstacle_Sphere3->AttachChild( graphic::CModelUtil::CreateOBBDisp( g_TestObstacle_Sphere3->GetOBB3ComputeBone() , graphic::CColor_Mistyrose ) );

	g_TestObstacle_Sphere1->ComputeAABB3(); g_TestObstacle_Sphere1->AttachChild( graphic::CModelUtil::CreateAABBDisp( g_TestObstacle_Sphere1->GetAABB3ComputeBone() , graphic::CColor_Mistyrose ) );
	g_TestObstacle_Sphere2->ComputeAABB3(); g_TestObstacle_Sphere2->AttachChild( graphic::CModelUtil::CreateAABBDisp( g_TestObstacle_Sphere2->GetAABB3ComputeBone() , graphic::CColor_Mistyrose ) );
	g_TestObstacle_Sphere3->ComputeAABB3(); g_TestObstacle_Sphere3->AttachChild( graphic::CModelUtil::CreateAABBDisp( g_TestObstacle_Sphere3->GetAABB3ComputeBone() , graphic::CColor_Mistyrose ) );
*/
	gameManager.m_spWall = g_TestObstacle_Wall;
	gameManager.m_spSphere1 = g_TestObstacle_Sphere1;
	gameManager.m_spSphere2 = g_TestObstacle_Sphere2;
	gameManager.m_spSphere3 = g_TestObstacle_Sphere3;

	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateStandardLine3() );

//------------------------------------------------------------

	//�����J����
	g_spFontCamera = Camera2DFixationCreate_();
	g_spFontCamera->vSetUp();
	g_spFontCamera->SetLeftTopCenter( skyTrue );

	//�����X�N���[���̃Z�b�g�A�b�v
	screenData.Name               = CHS( FONT_SCREEN_SETUP_NAME );        //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = g_spFontCamera;                       //�J����

	//�X�N���[���̃Z�b�g�A�b�v
	g_spFontScreen = g_spScreen->PushLayer( screenData );

	//�������o�͂���̂ŕ����Ǘ���������
	CFontManagerInitialize_();

	//�������쐬
	g_spFont = CFontSimpleCreate_();
	g_spFont->SetUp();

	//�������V�[���O���t�ɓo�^
	g_spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_spFont );

//--------3DScreen�̃Z�b�g�A�b�v--------��

	//�Փ˔�����o�^
//	g_ObstacleWallCollisionData.Initialize( eObjectKind_Box , skyTrue , &g_TestObstacle_Wall->GetAABB3ComputeBone() );
	g_ObstacleWallCollisionData.Initialize( eObjectKind_Box , skyTrue , &g_TestObstacle_Wall->GetOBB3ComputeBone() );

	g_ObstacleSphere1CollisionData.Initialize( eObjectKind_Sphere , skyTrue , &g_TestObstacle_Sphere1->GetOBSComputeBone() );
	g_ObstacleSphere2CollisionData.Initialize( eObjectKind_Sphere , skyTrue , &g_TestObstacle_Sphere2->GetOBSComputeBone() );
	g_ObstacleSphere3CollisionData.Initialize( eObjectKind_Sphere , skyTrue , &g_TestObstacle_Sphere3->GetOBSComputeBone() );
/*
	g_ObstacleSphere1CollisionData.Initialize( eObjectKind_Sphere , skyTrue , &g_TestObstacle_Sphere1->GetOBB3ComputeBone() );
	g_ObstacleSphere2CollisionData.Initialize( eObjectKind_Sphere , skyTrue , &g_TestObstacle_Sphere2->GetOBB3ComputeBone() );
	g_ObstacleSphere3CollisionData.Initialize( eObjectKind_Sphere , skyTrue , &g_TestObstacle_Sphere3->GetOBB3ComputeBone() );

	g_ObstacleSphere1CollisionData.Initialize( eObjectKind_Sphere , skyTrue , &g_TestObstacle_Sphere1->GetAABB3ComputeBone() );
	g_ObstacleSphere2CollisionData.Initialize( eObjectKind_Sphere , skyTrue , &g_TestObstacle_Sphere2->GetAABB3ComputeBone() );
	g_ObstacleSphere3CollisionData.Initialize( eObjectKind_Sphere , skyTrue , &g_TestObstacle_Sphere3->GetAABB3ComputeBone() );
*/
	g_CollisionManager.ResisterDataRequest( &g_ObstacleWallCollisionData );
	g_CollisionManager.ResisterDataRequest( &g_ObstacleSphere1CollisionData );
	g_CollisionManager.ResisterDataRequest( &g_ObstacleSphere2CollisionData );
	g_CollisionManager.ResisterDataRequest( &g_ObstacleSphere3CollisionData );

	//FPS�R���g���[���[�쐬
	sky::lib::fps::CFpsController fpsCtrl;

	//�`����L���[�̍쐬
	CRenderCommandManager_::CreateQueue( RENDER_QUEUE_SIZE );

	//�`��p�X���b�h�̍쐬
	CRenderCommandManager_::StartThread();

	fpsCtrl.SetFrameRate( _FRAME_NUM );

	SKY_PRINTF( _T( "===================== < RenderTest Start > ===================== \n" ) );

	//�Q�[�����[�v
	GAME_LOOP_START
	{
		//�t���[���J�n
		fpsCtrl.StartFrame();

		//���͊Ǘ��X�V
		Ckeyboard_->Update( fpsCtrl );

		//�Փ˔�����{
		g_CollisionManager.Update();
		g_CollisionManager.Detection();

		//�X�V
		player.Update( fpsCtrl );
		playerAI.Update( fpsCtrl );
		ennemy.Update( fpsCtrl );

		//�����`��
		g_spFont->PrintfSentence( 32 , 32 , graphic::CColor_Green , 1.0f , _T( "[ AIDemo ]" ) );

		//�`�撆��������R�}���h���s����
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//�R�}���h�o�b�t�@�̍쐬
			CRenderCommandManager_::CreateBuffer();

			//�`��R�}���h���s
			g_spScreen->Rendering( fpsCtrl );

			//�R�}���h�o�b�t�@�̒ǉ�
			CRenderCommandManager_::PushBuffer();
		}

		//�t���[�����[�g�ێ��̂��߁A���Ԃ��]������҂�
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	//�`��p�X���b�h�̒�~
	CRenderCommandManager_::StopThread();

	//����
	g_CollisionManager.UnResisterDataRequest( &g_ObstacleWallCollisionData );

	//�����Ǘ��̗���
	CFontManagerDispose_();

	g_spCamera.Delete();
	g_spFont.Delete();
	g_spScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#if ON_

static const skyString *SCREEN_SETUP_NAME       = _T( "Test" ); 
static const skyString *FONT_SCREEN_SETUP_NAME  = _T( "font" ); 

//Window�����ʒu�A�T�C�Y
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//�P�b������̃t���[����
static const u32				_FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//�J����
static graphic::CameraPtr  g_spCamera;
static graphic::CameraPtr  g_spFontCamera;

//�e�X�g�p���f��
static graphic::CModelPtr     g_TestPlane;
static graphic::CModelPtr     g_TestObstacle_Wall;
static graphic::CModelPtr     g_TestObstacle_Sphere1;
static graphic::CModelPtr     g_TestObstacle_Sphere2;
static graphic::CModelPtr     g_TestObstacle_Sphere3;

//�X�N���[��
static graphic::CScreenObjectPtr    g_spFontScreen;

//��Q�p�Փ˔�����
math::CollisionData g_ObstacleWallCollisionData;
math::CollisionData g_ObstacleSphere1CollisionData;
math::CollisionData g_ObstacleSphere2CollisionData;
math::CollisionData g_ObstacleSphere3CollisionData;

//---------------------------------------------------------------

skyBool s_CollisionFunc( const math::CBasicLine3 &line3 )
{
	skyBool result = skyFalse;

	math::CollisionData colData;
	colData.Initialize( eObjectKind_Edge , skyTrue , &line3 );

	result = g_CollisionManager.Detection( &colData , eObjectKind_Sphere );
//	result = result || g_CollisionManager.Detection( &colData , eObjectKind_Box ); //��������ƌo�H��������Ȃ��Ȃ��ăo�O��

	return result;
}

TEST( Ai , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//�I�����m_�o�^����
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//�t���X�N���[�����[�hOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//--------3DScreen�̃Z�b�g�A�b�v--------��

	g_spCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	g_spCamera->SetDefaultInput();

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( SCREEN_SETUP_NAME );             //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = g_spCamera;                           //�J����
	screenData.WindowRect.X()     = _WIN_X;                               //��ʈʒuX
	screenData.WindowRect.Y()     = _WIN_Y;                               //��ʈʒuY
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���
	screenData.RenderState        = CRenderStateCreate_();                 //�����_�[�X�e�[�g

	//�X�N���[���̃Z�b�g�A�b�v
	g_spScreen = CScreenObject_::Create( screenData );

	//���E�Ǘ��쐬
	GameManager gameManager;

	//�v���C���[�쐬
	CPlayer player( &gameManager );

	//�v���C���[�`�h�쐬
	CPlayerAI playerAI( &gameManager );

	//�G�쐬
	CEnemmy ennemy( &gameManager );

	//���쐬
	g_TestPlane = graphic::CModelUtil::CreateTestPlane(); 
	g_TestPlane->SetPosition_Y( -1.0f ); 
	g_TestPlane->SetScale_XYZ( math::CBasicVector3( 4 , 4 , 4 ) );

	//��Q���쐬
	g_TestObstacle_Wall = graphic::CModelUtil::CreateTestCube( graphic::CColor_Dimgray , math::CBasicVector3( 4 , 1 , 1 ) );
	g_TestObstacle_Wall->SetPosition_X( 10 );

	g_TestObstacle_Sphere1 = graphic::CModelUtil::CreateTestSphere( graphic::CColor_Dimgray , math::CBasicVector3( 6 , 6 , 6 ) , math::CBasicAngleVector3_ZERO , math::CBasicVector3( 10  , 0 , 10 ) );
	g_TestObstacle_Sphere2 = graphic::CModelUtil::CreateTestSphere( graphic::CColor_Dimgray , math::CBasicVector3( 7 , 7 , 7 ) , math::CBasicAngleVector3_ZERO , math::CBasicVector3( 10  , 0 , -10 ) );
	g_TestObstacle_Sphere3 = graphic::CModelUtil::CreateTestSphere( graphic::CColor_Dimgray , math::CBasicVector3( 8 , 8 , 8 ) , math::CBasicAngleVector3_ZERO , math::CBasicVector3( -10 , 0 , 10 ) );

	//���f�����V�[���O���t�ɓo�^
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestPlane );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestObstacle_Wall );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestObstacle_Sphere1 );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestObstacle_Sphere2 );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestObstacle_Sphere3 );

	//�Փ˔���
	g_TestObstacle_Wall->ComputeOBB3();  g_TestObstacle_Wall->AttachChild( graphic::CModelUtil::CreateOBBDisp( g_TestObstacle_Wall->GetOBB3() , graphic::CColor_Mistyrose ) ); 
	g_TestObstacle_Sphere1->ComputeBS(); g_TestObstacle_Sphere1->AttachChild( graphic::CModelUtil::CreateLine3DSphere( g_TestObstacle_Sphere1->GetOBSComputeJoint() , graphic::CColor_Mistyrose ) );
	g_TestObstacle_Sphere2->ComputeBS(); g_TestObstacle_Sphere2->AttachChild( graphic::CModelUtil::CreateLine3DSphere( g_TestObstacle_Sphere2->GetOBSComputeJoint() , graphic::CColor_Mistyrose ) );
	g_TestObstacle_Sphere3->ComputeBS(); g_TestObstacle_Sphere3->AttachChild( graphic::CModelUtil::CreateLine3DSphere( g_TestObstacle_Sphere3->GetOBSComputeJoint() , graphic::CColor_Mistyrose ) );

	gameManager.m_spWall = g_TestObstacle_Wall;
	gameManager.m_spSphere1 = g_TestObstacle_Sphere1;
	gameManager.m_spSphere2 = g_TestObstacle_Sphere2;
	gameManager.m_spSphere3 = g_TestObstacle_Sphere3;

	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateStandardLine3() );

//------------------------------------------------------------

	//�����J����
	g_spFontCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	g_spFontCamera->SetOrtho( skyTrue );         //���ˉe
	g_spFontCamera->SetLeftTopCenter( skyTrue ); //���㌴�_

	//�����X�N���[���̃Z�b�g�A�b�v
	screenData.Name               = CHS( FONT_SCREEN_SETUP_NAME );        //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = g_spFontCamera;                       //�J����

	//�X�N���[���̃Z�b�g�A�b�v
	g_spFontScreen = g_spScreen->PushLayer( screenData );

	//�������o�͂���̂ŕ����Ǘ���������
	CFontManagerInitialize_();

	//�������쐬
	g_spFont = CFontSimpleCreate_();
	g_spFont->SetUp();

	g_spFontTitle = CFontCreate_();
	g_spFontTitle->SetUp();

	//�������V�[���O���t�ɓo�^
	g_spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_spFont );
	g_spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_spFontTitle );

//--------3DScreen�̃Z�b�g�A�b�v--------��

	//�Փ˔�����o�^
	g_ObstacleWallCollisionData.Initialize( eObjectKind_Box , skyTrue , &g_TestObstacle_Wall->GetOBB3ComputeJoint() );

	g_ObstacleSphere1CollisionData.Initialize( eObjectKind_Sphere , skyTrue , &g_TestObstacle_Sphere1->GetOBSComputeJoint() );
	g_ObstacleSphere2CollisionData.Initialize( eObjectKind_Sphere , skyTrue , &g_TestObstacle_Sphere2->GetOBSComputeJoint() );
	g_ObstacleSphere3CollisionData.Initialize( eObjectKind_Sphere , skyTrue , &g_TestObstacle_Sphere3->GetOBSComputeJoint() );

	g_CollisionManager.ResisterDataRequest( &g_ObstacleWallCollisionData );
	g_CollisionManager.ResisterDataRequest( &g_ObstacleSphere1CollisionData );
	g_CollisionManager.ResisterDataRequest( &g_ObstacleSphere2CollisionData );
	g_CollisionManager.ResisterDataRequest( &g_ObstacleSphere3CollisionData );

//--------�͂�--------------------------��

	math::CBasicVector3Vector fence;

#define FENCE_LENGHT 38
	fence.Add( math::CBasicVector3( -FENCE_LENGHT, 0,  FENCE_LENGHT ) );
	fence.Add( math::CBasicVector3(  FENCE_LENGHT, 0,  FENCE_LENGHT ) );
	fence.Add( math::CBasicVector3(  FENCE_LENGHT, 0, -FENCE_LENGHT ) );
	fence.Add( math::CBasicVector3( -FENCE_LENGHT, 0, -FENCE_LENGHT ) );
	fence.Add( math::CBasicVector3( -FENCE_LENGHT, 0,  FENCE_LENGHT ) );

	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateLine3D( fence , graphic::CColor_Blue ) );

	//Update���ăf�[�^���m�肳����
	g_CollisionManager.Update();

//--------�͂�--------------------------��

//--------�O���t�����-------------------��

	//�m�[�h��z�u����
	math::CEuclidGraph graph( skyFalse );
	math::CEuclidGraph_Create8DirectionsGraph( graph , math::CBasicVector3( 0 , 0 , 0 ) , fence , 3 , skyTrue , &s_CollisionFunc );

	//�ړ��J�n�m�[�h�ƏI���m�[�h��T��
	graphIndex startNodeIndex = graph.GetNearNodeIndex( math::CBasicVector3(  40 , 0 ,  -40 ) , 3 ); //�X�^�[�g
	graphIndex goleNodeIndex  = graph.GetNearNodeIndex( math::CBasicVector3( -40 , 0 , 40 ) , 3 );   //�S�[��

	//�o�H�T���i�S��ށj
	math::CGraphSearchAStar< math::CEuclidGraph , math::CEuclidHeuristic > search( graph , startNodeIndex , goleNodeIndex );
//	math::CGraphSearchBFS< math::CEuclidGraph > search( graph , startNodeIndex , goleNodeIndex );
//	math::CGraphSearchDFS< math::CEuclidGraph > search( graph , startNodeIndex , goleNodeIndex , 4096 );
//	math::CGraphSearchDijkstra< math::CEuclidGraph > search( graph , startNodeIndex , goleNodeIndex );

	//�T�����ʂ���p�X�𐶐�����
	framework::CAIMovePath path;
	path.SetPathEndType( framework::eAIPathEndType_Back );
	math::GraphIndexList pathList;
	path.AddGraph( graph , search.GetPathToTarget( pathList ) );

	//�o�H��`��
	math::CBasicLine3List edgeList;
	graph.GetEdgeLineList( edgeList );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateLine3D( edgeList , graphic::CColor_Gold ) );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( path.CreateModel( graphic::CColor_Black ) );

	//�`�h�Ɍo�H��ݒ�
	ennemy.m_AIObject.m_Path = path;

//--------�O���t�����-------------------��

	//FPS�R���g���[���[�쐬
	sky::lib::fps::CFpsController fpsCtrl;

	//�`����L���[�̍쐬
	CRenderCommandManager_::CreateQueue( RENDER_QUEUE_SIZE );

	//�`��p�X���b�h�̍쐬
	CRenderCommandManager_::StartThread();

	fpsCtrl.SetFrameRate( _FRAME_NUM );

	SKY_PRINTF( _T( "===================== < RenderTest Start > ===================== \n" ) );

	//�Q�[�����[�v
	GAME_LOOP_START
	{
		//�t���[���J�n
		fpsCtrl.StartFrame();

		//���͊Ǘ��X�V
		Ckeyboard_->Update( fpsCtrl );

		//�Փ˔�����{
		g_CollisionManager.Update();
		g_CollisionManager.Detection();

		//�X�V
		player.Update( fpsCtrl );
		playerAI.Update( fpsCtrl );
		ennemy.Update( fpsCtrl );

		//�����`��
		g_spFont->PrintfSentence( 32 , 32 , graphic::CColor_Green , 1.0f , _T( "[ AIDemo ]" ) );

		//�`�撆��������R�}���h���s����
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//�R�}���h�o�b�t�@�̍쐬
			CRenderCommandManager_::CreateBuffer();

			//�`��R�}���h���s
			g_spScreen->Rendering( fpsCtrl );

			//�R�}���h�o�b�t�@�̒ǉ�
			CRenderCommandManager_::PushBuffer();
		}

		//�t���[�����[�g�ێ��̂��߁A���Ԃ��]������҂�
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	//�`��p�X���b�h�̒�~
	CRenderCommandManager_::StopThread();

	//����
	g_CollisionManager.UnResisterDataRequest( &g_ObstacleWallCollisionData );

	//�����Ǘ��̗���
	CFontManagerDispose_();

	g_spCamera.Delete();
	g_spFont.Delete();
	g_spScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif