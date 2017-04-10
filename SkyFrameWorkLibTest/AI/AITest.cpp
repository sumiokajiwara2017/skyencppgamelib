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

//Window初期位置、サイズ
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//１秒あたりのフレーム数
static const u32				_FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//カメラ
static graphic::Camera3DFixationPtr  g_spCamera;
static graphic::Camera2DFixationPtr  g_spFontCamera;

//テスト用モデル
static graphic::CModelPtr     g_TestPlane;
static graphic::CModelPtr     g_TestObstacle_Wall;
static graphic::CModelPtr     g_TestObstacle_Sphere1;
static graphic::CModelPtr     g_TestObstacle_Sphere2;
static graphic::CModelPtr     g_TestObstacle_Sphere3;

//スクリーン
static graphic::CScreenObjectPtr    g_spFontScreen;

//障害用衝突判定情報
math::CollisionData g_ObstacleWallCollisionData;
math::CollisionData g_ObstacleSphere1CollisionData;
math::CollisionData g_ObstacleSphere2CollisionData;
math::CollisionData g_ObstacleSphere3CollisionData;

//---------------------------------------------------------------

TEST( Ai , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//終了感知_登録する
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//フルスクリーンモードOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//--------3DScreenのセットアップ--------▽

	g_spCamera = Camera3DFixationCreate_();
	g_spCamera->vSetUp();
	g_spCamera->SetDefaultInputOn( skyTrue );
	graphic::CameraUtil::SetInitPos_Top( g_spCamera.GetPtrNoConst() , 80 );

	//ルートスクリーンのセットアップ
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( SCREEN_SETUP_NAME );             //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = g_spCamera;                           //カメラ
	screenData.WindowRect.X()     = _WIN_X;                               //画面位置X
	screenData.WindowRect.Y()     = _WIN_Y;                               //画面位置Y
	screenData.WindowRect.W()     = _WIN_W;                               //画面幅
	screenData.WindowRect.H()     = _WIN_H;                               //画面高さ
	screenData.RenderState        = CRenderStateCreate_();                 //レンダーステート

	//スクリーンのセットアップ
	g_spScreen = CScreenObject_::Create( screenData );

	//世界管理作成
	GameManager gameManager;

	//プレイヤー作成
	CPlayer player( &gameManager );

	//プレイヤーＡＩ作成
	CPlayerAI playerAI( &gameManager );

	//敵作成
	CEnemmy ennemy( &gameManager );

	//床作成
	g_TestPlane = graphic::CModelUtil::CreateTestPlane(); 
	g_TestPlane->SetPosition_Y( -1.0f ); 
	g_TestPlane->SetScale_XYZ( math::CBasicVector3( 4 , 4 , 4 ) );

	//障害物作成
	g_TestObstacle_Wall = graphic::CModelUtil::CreateTestCube( graphic::CColor_Dimgray , math::CBasicVector3( 4 , 1 , 1 ) );
	GET_BONE( g_TestObstacle_Wall )->SetPosition_X( 10 );

	g_TestObstacle_Sphere1 = graphic::CModelUtil::CreateTestSphere( graphic::CColor_Dimgray , math::CBasicVector3( 6 , 6 , 6 ) , math::CBasicVector3( 0 , 0 , 0 ) , math::CBasicVector3( 10  , 0 , 10 ) );
	g_TestObstacle_Sphere2 = graphic::CModelUtil::CreateTestSphere( graphic::CColor_Dimgray , math::CBasicVector3( 7 , 7 , 7 ) , math::CBasicVector3( 0 , 0 , 0 ) , math::CBasicVector3( 10  , 0 , -10 ) );
	g_TestObstacle_Sphere3 = graphic::CModelUtil::CreateTestSphere( graphic::CColor_Dimgray , math::CBasicVector3( 8 , 8 , 8 ) , math::CBasicVector3( 0 , 0 , 0 ) , math::CBasicVector3( -10 , 0 , 10 ) );

	//モデルをシーングラフに登録
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestPlane );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestObstacle_Wall );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestObstacle_Sphere1 );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestObstacle_Sphere2 );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestObstacle_Sphere3 );

	//衝突判定
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

	//文字カメラ
	g_spFontCamera = Camera2DFixationCreate_();
	g_spFontCamera->vSetUp();
	g_spFontCamera->SetLeftTopCenter( skyTrue );

	//文字スクリーンのセットアップ
	screenData.Name               = CHS( FONT_SCREEN_SETUP_NAME );        //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = g_spFontCamera;                       //カメラ

	//スクリーンのセットアップ
	g_spFontScreen = g_spScreen->PushLayer( screenData );

	//文字を出力するので文字管理を初期化
	CFontManagerInitialize_();

	//文字を作成
	g_spFont = CFontSimpleCreate_();
	g_spFont->SetUp();

	//文字をシーングラフに登録
	g_spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_spFont );

//--------3DScreenのセットアップ--------△

	//衝突判定情報登録
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

	//FPSコントローラー作成
	sky::lib::fps::CFpsController fpsCtrl;

	//描画情報キューの作成
	CRenderCommandManager_::CreateQueue( RENDER_QUEUE_SIZE );

	//描画用スレッドの作成
	CRenderCommandManager_::StartThread();

	fpsCtrl.SetFrameRate( _FRAME_NUM );

	SKY_PRINTF( _T( "===================== < RenderTest Start > ===================== \n" ) );

	//ゲームループ
	GAME_LOOP_START
	{
		//フレーム開始
		fpsCtrl.StartFrame();

		//入力管理更新
		Ckeyboard_->Update( fpsCtrl );

		//衝突判定実施
		g_CollisionManager.Update();
		g_CollisionManager.Detection();

		//更新
		player.Update( fpsCtrl );
		playerAI.Update( fpsCtrl );
		ennemy.Update( fpsCtrl );

		//文字描画
		g_spFont->PrintfSentence( 32 , 32 , graphic::CColor_Green , 1.0f , _T( "[ AIDemo ]" ) );

		//描画中だったらコマンド発行する
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//コマンドバッファの作成
			CRenderCommandManager_::CreateBuffer();

			//描画コマンド発行
			g_spScreen->Rendering( fpsCtrl );

			//コマンドバッファの追加
			CRenderCommandManager_::PushBuffer();
		}

		//フレームレート維持のため、時間が余ったら待つ
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	//描画用スレッドの停止
	CRenderCommandManager_::StopThread();

	//解除
	g_CollisionManager.UnResisterDataRequest( &g_ObstacleWallCollisionData );

	//文字管理の履き
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

//Window初期位置、サイズ
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//１秒あたりのフレーム数
static const u32				_FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//カメラ
static graphic::CameraPtr  g_spCamera;
static graphic::CameraPtr  g_spFontCamera;

//テスト用モデル
static graphic::CModelPtr     g_TestPlane;
static graphic::CModelPtr     g_TestObstacle_Wall;
static graphic::CModelPtr     g_TestObstacle_Sphere1;
static graphic::CModelPtr     g_TestObstacle_Sphere2;
static graphic::CModelPtr     g_TestObstacle_Sphere3;

//スクリーン
static graphic::CScreenObjectPtr    g_spFontScreen;

//障害用衝突判定情報
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
//	result = result || g_CollisionManager.Detection( &colData , eObjectKind_Box ); //これを取ると経路がしかれなくなってバグる

	return result;
}

TEST( Ai , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//終了感知_登録する
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//フルスクリーンモードOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//--------3DScreenのセットアップ--------▽

	g_spCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	g_spCamera->SetDefaultInput();

	//ルートスクリーンのセットアップ
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( SCREEN_SETUP_NAME );             //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = g_spCamera;                           //カメラ
	screenData.WindowRect.X()     = _WIN_X;                               //画面位置X
	screenData.WindowRect.Y()     = _WIN_Y;                               //画面位置Y
	screenData.WindowRect.W()     = _WIN_W;                               //画面幅
	screenData.WindowRect.H()     = _WIN_H;                               //画面高さ
	screenData.RenderState        = CRenderStateCreate_();                 //レンダーステート

	//スクリーンのセットアップ
	g_spScreen = CScreenObject_::Create( screenData );

	//世界管理作成
	GameManager gameManager;

	//プレイヤー作成
	CPlayer player( &gameManager );

	//プレイヤーＡＩ作成
	CPlayerAI playerAI( &gameManager );

	//敵作成
	CEnemmy ennemy( &gameManager );

	//床作成
	g_TestPlane = graphic::CModelUtil::CreateTestPlane(); 
	g_TestPlane->SetPosition_Y( -1.0f ); 
	g_TestPlane->SetScale_XYZ( math::CBasicVector3( 4 , 4 , 4 ) );

	//障害物作成
	g_TestObstacle_Wall = graphic::CModelUtil::CreateTestCube( graphic::CColor_Dimgray , math::CBasicVector3( 4 , 1 , 1 ) );
	g_TestObstacle_Wall->SetPosition_X( 10 );

	g_TestObstacle_Sphere1 = graphic::CModelUtil::CreateTestSphere( graphic::CColor_Dimgray , math::CBasicVector3( 6 , 6 , 6 ) , math::CBasicAngleVector3_ZERO , math::CBasicVector3( 10  , 0 , 10 ) );
	g_TestObstacle_Sphere2 = graphic::CModelUtil::CreateTestSphere( graphic::CColor_Dimgray , math::CBasicVector3( 7 , 7 , 7 ) , math::CBasicAngleVector3_ZERO , math::CBasicVector3( 10  , 0 , -10 ) );
	g_TestObstacle_Sphere3 = graphic::CModelUtil::CreateTestSphere( graphic::CColor_Dimgray , math::CBasicVector3( 8 , 8 , 8 ) , math::CBasicAngleVector3_ZERO , math::CBasicVector3( -10 , 0 , 10 ) );

	//モデルをシーングラフに登録
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestPlane );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestObstacle_Wall );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestObstacle_Sphere1 );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestObstacle_Sphere2 );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestObstacle_Sphere3 );

	//衝突判定
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

	//文字カメラ
	g_spFontCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	g_spFontCamera->SetOrtho( skyTrue );         //正射影
	g_spFontCamera->SetLeftTopCenter( skyTrue ); //左上原点

	//文字スクリーンのセットアップ
	screenData.Name               = CHS( FONT_SCREEN_SETUP_NAME );        //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = g_spFontCamera;                       //カメラ

	//スクリーンのセットアップ
	g_spFontScreen = g_spScreen->PushLayer( screenData );

	//文字を出力するので文字管理を初期化
	CFontManagerInitialize_();

	//文字を作成
	g_spFont = CFontSimpleCreate_();
	g_spFont->SetUp();

	g_spFontTitle = CFontCreate_();
	g_spFontTitle->SetUp();

	//文字をシーングラフに登録
	g_spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_spFont );
	g_spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_spFontTitle );

//--------3DScreenのセットアップ--------△

	//衝突判定情報登録
	g_ObstacleWallCollisionData.Initialize( eObjectKind_Box , skyTrue , &g_TestObstacle_Wall->GetOBB3ComputeJoint() );

	g_ObstacleSphere1CollisionData.Initialize( eObjectKind_Sphere , skyTrue , &g_TestObstacle_Sphere1->GetOBSComputeJoint() );
	g_ObstacleSphere2CollisionData.Initialize( eObjectKind_Sphere , skyTrue , &g_TestObstacle_Sphere2->GetOBSComputeJoint() );
	g_ObstacleSphere3CollisionData.Initialize( eObjectKind_Sphere , skyTrue , &g_TestObstacle_Sphere3->GetOBSComputeJoint() );

	g_CollisionManager.ResisterDataRequest( &g_ObstacleWallCollisionData );
	g_CollisionManager.ResisterDataRequest( &g_ObstacleSphere1CollisionData );
	g_CollisionManager.ResisterDataRequest( &g_ObstacleSphere2CollisionData );
	g_CollisionManager.ResisterDataRequest( &g_ObstacleSphere3CollisionData );

//--------囲い--------------------------▽

	math::CBasicVector3Vector fence;

#define FENCE_LENGHT 38
	fence.Add( math::CBasicVector3( -FENCE_LENGHT, 0,  FENCE_LENGHT ) );
	fence.Add( math::CBasicVector3(  FENCE_LENGHT, 0,  FENCE_LENGHT ) );
	fence.Add( math::CBasicVector3(  FENCE_LENGHT, 0, -FENCE_LENGHT ) );
	fence.Add( math::CBasicVector3( -FENCE_LENGHT, 0, -FENCE_LENGHT ) );
	fence.Add( math::CBasicVector3( -FENCE_LENGHT, 0,  FENCE_LENGHT ) );

	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateLine3D( fence , graphic::CColor_Blue ) );

	//Updateしてデータを確定させる
	g_CollisionManager.Update();

//--------囲い--------------------------△

//--------グラフを作る-------------------▽

	//ノードを配置する
	math::CEuclidGraph graph( skyFalse );
	math::CEuclidGraph_Create8DirectionsGraph( graph , math::CBasicVector3( 0 , 0 , 0 ) , fence , 3 , skyTrue , &s_CollisionFunc );

	//移動開始ノードと終了ノードを探す
	graphIndex startNodeIndex = graph.GetNearNodeIndex( math::CBasicVector3(  40 , 0 ,  -40 ) , 3 ); //スタート
	graphIndex goleNodeIndex  = graph.GetNearNodeIndex( math::CBasicVector3( -40 , 0 , 40 ) , 3 );   //ゴール

	//経路探索（４種類）
	math::CGraphSearchAStar< math::CEuclidGraph , math::CEuclidHeuristic > search( graph , startNodeIndex , goleNodeIndex );
//	math::CGraphSearchBFS< math::CEuclidGraph > search( graph , startNodeIndex , goleNodeIndex );
//	math::CGraphSearchDFS< math::CEuclidGraph > search( graph , startNodeIndex , goleNodeIndex , 4096 );
//	math::CGraphSearchDijkstra< math::CEuclidGraph > search( graph , startNodeIndex , goleNodeIndex );

	//探索結果からパスを生成する
	framework::CAIMovePath path;
	path.SetPathEndType( framework::eAIPathEndType_Back );
	math::GraphIndexList pathList;
	path.AddGraph( graph , search.GetPathToTarget( pathList ) );

	//経路を描画
	math::CBasicLine3List edgeList;
	graph.GetEdgeLineList( edgeList );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateLine3D( edgeList , graphic::CColor_Gold ) );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( path.CreateModel( graphic::CColor_Black ) );

	//ＡＩに経路を設定
	ennemy.m_AIObject.m_Path = path;

//--------グラフを作る-------------------△

	//FPSコントローラー作成
	sky::lib::fps::CFpsController fpsCtrl;

	//描画情報キューの作成
	CRenderCommandManager_::CreateQueue( RENDER_QUEUE_SIZE );

	//描画用スレッドの作成
	CRenderCommandManager_::StartThread();

	fpsCtrl.SetFrameRate( _FRAME_NUM );

	SKY_PRINTF( _T( "===================== < RenderTest Start > ===================== \n" ) );

	//ゲームループ
	GAME_LOOP_START
	{
		//フレーム開始
		fpsCtrl.StartFrame();

		//入力管理更新
		Ckeyboard_->Update( fpsCtrl );

		//衝突判定実施
		g_CollisionManager.Update();
		g_CollisionManager.Detection();

		//更新
		player.Update( fpsCtrl );
		playerAI.Update( fpsCtrl );
		ennemy.Update( fpsCtrl );

		//文字描画
		g_spFont->PrintfSentence( 32 , 32 , graphic::CColor_Green , 1.0f , _T( "[ AIDemo ]" ) );

		//描画中だったらコマンド発行する
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//コマンドバッファの作成
			CRenderCommandManager_::CreateBuffer();

			//描画コマンド発行
			g_spScreen->Rendering( fpsCtrl );

			//コマンドバッファの追加
			CRenderCommandManager_::PushBuffer();
		}

		//フレームレート維持のため、時間が余ったら待つ
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	//描画用スレッドの停止
	CRenderCommandManager_::StopThread();

	//解除
	g_CollisionManager.UnResisterDataRequest( &g_ObstacleWallCollisionData );

	//文字管理の履き
	CFontManagerDispose_();

	g_spCamera.Delete();
	g_spFont.Delete();
	g_spScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif