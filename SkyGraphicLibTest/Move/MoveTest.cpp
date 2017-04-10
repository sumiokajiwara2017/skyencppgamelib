#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_MOVE

#if OFF_

using namespace sky::lib;

#include "SkyBaseLib/Kfm.h"
#include "SkyMathLib/Graph.h"
#include "SkyGraphicLib/Loader.h"

static const skyString *TEST1_OBJ_FILE_NAME_1 = _T( "testcube.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_2 = _T( "testcone.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_3 = _T( "testplane10x10.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_4 = _T( "testsphere.obj" );

static const skyString *SCREEN_SETUP_NAME                   = _T( "Test" ); 
static const skyString *FONT_SCREEN_SETUP_NAME                   = _T( "font" ); 

static const skyString *TEST1_XML_PATH = _T( "data/test/skyxml/TestSaveModels.xml" );
static const skyString *TEST1_BIN_PATH = _T( "data/test/skybin/TestSaveModels.bin" );
static const skyString *TEST1_TEXTURE_PATH_1 = _T( "data/test/texture/TestPng.png" );
static const skyString *TEST1_TEXTURE_PATH_2 = _T( "data/test/texture/TestJpg.jpg" );

//Window初期位置、サイズ
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//１秒あたりのフレーム数
static const u32				_FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//カメラ
static graphic::Camera2DFixationPtr  g_spCamera;
static graphic::Camera2DFixationPtr  g_spFontCamera;

//テスト用スプライト
static graphic::CModelPtr      g_spGraph;
static graphic::CSpritePtr     g_spPoint;

//スクリーン
static graphic::CScreenObjectPtr    g_spScreen;
static graphic::CScreenObjectPtr    g_spFontScreen;

TEST( Move , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	math::CBasicVector3Vector vectorContena;
//	math::CGraphUtil::DotsCubicCurveCurve( vectorContena , -500 , 500 );
//	math::CGraphUtil::DotsCircle( vectorContena , 500 );
//	math::CGraphUtil::DotsElliptical( vectorContena , 400 , 300  );
//	math::CGraphUtil::DotsHyperbola( vectorContena , 300 , 400  );
//	math::CGraphUtil::DotsTrochoid( vectorContena , 500 , 500  );
	math::CGraphUtil::DotsClothoid( vectorContena , 500 );

	//終了感知コールバックを登録する
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//フルスクリーンモードOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//--------3DScreenのセットアップ--------▽

	g_spCamera = Camera2DFixationCreate_();
	g_spCamera->vSetUp();
	g_spCamera->SetDefaultInputOn( skyTrue );

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

	//グラフの作成
	g_spGraph = graphic::CModelUtil::CreateLine3D( vectorContena , graphic::CColor_Green );

	//ポインターの作成
	g_spPoint = graphic::CSpriteUtil::CreateTestCircle( graphic::RENDERING_2D_PRIO_FAST , graphic::CColor_Red );
	GET_BONE( g_spPoint )->SetScale_XYZ( 10.0f );

	//シーングラフに登録
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_spGraph );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_spPoint );

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

	//基準線の作成
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CSpriteUtil::CreateStandardLine2( 1000 ) );

	//文字を作成
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();
	spFont->SetUp();

	//文字をシーングラフに登録
	g_spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spFont );

//--------3DScreenのセットアップ--------△

//--------アニメーションのセットアップ--------▽

	//再生機の生成
	kfm::CFCurvePlayer fcPlayer;
	
	//再生するファンクションカーブの生成
	kfm::CFCurveBasePtr fcurve = kfm::FCurveUtil::CreateFCurve< math::CBasicVector3 >( vectorContena );

	//登録
	fcPlayer.PlayFCurveRequest( fcurve , 0.0f , 0.0f , 1.0f , 0.0f , kfm::ePlayEndType_Back );

//--------アニメーションのセットアップ--------△

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
		Ckeyboard_->Update( fpsCtrl.GetPassageTime() );

		//終了判定
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) || bExit == 1 )
		{
			GAME_LOOP_EXIT();
		}

		//再生
		fcPlayer.Play( 1.0f );

		//描画中だったらコマンド発行する
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//コマンドバッファの作成
			CRenderCommandManager_::CreateBuffer();

			//描画コマンド発行
			g_spScreen->Rendering( CTimeUtil_::GetNowTime() );

			//コマンドバッファの追加
			CRenderCommandManager_::PushBuffer();
		}

		//フレームレート維持のため、時間が余ったら待つ
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	//描画用スレッドの停止
	CRenderCommandManager_::StopThread();

	g_spCamera.Delete();
	g_spScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}
#endif

#if ON_

using namespace sky::lib;

#include "SkyMathLib/Graph.h"
#include "SkyGraphicLib/Loader.h"

static const skyString *SCREEN_SETUP_NAME                   = _T( "Test" ); 
static const skyString *FONT_SCREEN_SETUP_NAME                   = _T( "font" ); 

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
static graphic::CModelPtr     g_TestCone;
static graphic::CModelPtr     g_TestPlane;

//スクリーン
static graphic::CScreenObjectPtr    g_spScreen;
static graphic::CScreenObjectPtr    g_spFontScreen;

TEST( Move , 2 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	math::CBasicSphere sphere5( 0 , 0 , 0 , 5 );
	math::CBasicLine3  line( math::CBasicVector3( 6 , 0 , 0 ) , math::CBasicVector3( 6 , 10 , 0 ) );

	skyBool result = math::GeometryCollisionUtil::SphereToLine3< dectype >( sphere5 , line );

	math::CBasicCapsule3 capsule( 9 , 0 , 0 , 9 , 10 , 0 , 3 );
	result = math::GeometryCollisionUtil::SphereToCapsule( sphere5 , capsule );

	math::CBasicCapsule3 capsule2( 9 , 0 , 2 , 9 , 10 , 2 , 3 );
	result = math::GeometryCollisionUtil::CapsuleToCapsule( capsule , capsule2 );

	math::CBasicSphere sp1( 0 , 0 , 0 , 5 );
	math::CBasicSphere sp2( 0 , 0 , 0 , 0.5 );

	math::CBasicVector3 points[ 7 ];
	points[ 0 ].Set( 0  , 0  , 0 );
	points[ 1 ].Set( 1  , 0  , 0 );
	points[ 2 ].Set( 0  , 1  , 0 );
	points[ 3 ].Set( 0  , 0  , 1 );
	points[ 4 ].Set( -1 , 0  , 0 );
	points[ 5 ].Set( 0  , -1 , 0 );
	points[ 6 ].Set( 0  , 0  , -1 );
	sp1.CreateFromPoints( points , array_sizeof( points ) );

	sp1.Merge( sp2 );
	sp1.Merge( math::CBasicVector3( 0 , 7 , 0 ) );
	sp1.Append( math::CBasicVector3( 0 , 7 , 0 ) );

	math::CBasicSphere   sphere( 5 , 1 , 0 , 3 );
	math::CBasiTriangle triangle( math::CBasicVector3( 15 , 0 , 0.1 ) , math::CBasicVector3( 0 , 15 , 0.1 ) , math::CBasicVector3( -15 , 0 , 0.1 ) );
	result = math::GeometryCollisionUtil::SphereToTriangle< dectype >( sphere , triangle );

	math::CBasicPlane plane( math::CBasicVector3( 0 , 1 , 0 ) , math::CBasicVector3( 0 , 0 , 0 ) );
	result = math::GeometryCollisionUtil::SphereToPlane< dectype >( sphere , plane );

	math::CQuaternion q;
	q.SetToRotateAboutAxis( math::CBasicVector3( 0 , 0 , 1 ) , math::CMathUtil::DegToRad( 45 ) );
	math::CBasicVector3 vec( 0 , 1 , 0 );
	vec *= q;
	SKY_PRINTF( _T( "%f %f %f \n" ) , vec.X , vec.Y , vec.Z );


	graphic::CBonePtr spBone = CBoneCreate_();

	spBone->MulScale_X( 2 );
	spBone->MulScale_Y( 3 );
	spBone->MulScale_Z( 4 );

	spBone->MulRotation_X( math::CMathUtil::DegToRad( 60 ) );
	spBone->MulRotation_Y( math::CMathUtil::DegToRad( 80 ) );
	spBone->MulRotation_Z( math::CMathUtil::DegToRad( 90 ) );

	//拡縮して回転して移動する
	spBone->SetPosition_X( 5 );
	spBone->SetPosition_Y( 10 );
	spBone->SetPosition_Z( 15 );

	SKY_PRINTF( _T( "pos[ %f %f %f ] sc[ %f %f %f ]" ) , spBone->GetPosition_X() , spBone->GetPosition_Y() , spBone->GetPosition_Z() , spBone->GetScale_X() , spBone->GetScale_Y() , spBone->GetScale_Z() );

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

	g_TestCone = graphic::CModelUtil::CreateTestCone();
	g_TestPlane = graphic::CModelUtil::CreateTestPlane();

	GET_BONE( g_TestCone )->SetPosition_XYZ( math::CBasicVector3( 1 , 5 , 1 ) );

	//モデルをシーングラフに登録
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestCone );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestPlane );

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

	//基準線の作成
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateStandardLine3() );

	//文字を作成
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();
	spFont->SetUp();

	//文字をシーングラフに登録
	g_spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spFont );

//--------3DScreenのセットアップ--------△

	//FPSコントローラー作成
	sky::lib::fps::CFpsController fpsCtrl;

	//描画情報キューの作成
	CRenderCommandManager_::CreateQueue( RENDER_QUEUE_SIZE );

	//描画用スレッドの作成
	CRenderCommandManager_::StartThread();

	fpsCtrl.SetFrameRate( _FRAME_NUM );

	SKY_PRINTF( _T( "===================== < RenderTest Start > ===================== \n" ) );

	//球面座標によりコーンを回転する
	dectype r     = 3;                            //半径
	dectype theta = CMathUtil_::DegToRad( 0 );    //XY平面角度
	dectype phi   = CMathUtil_::DegToRad( 0 );    //仰角

	//ゲームループ
	GAME_LOOP_START
	{
		//フレーム開始
		fpsCtrl.StartFrame();

		//入力管理更新
		Ckeyboard_->Update( fpsCtrl );

		//カメラ管理を更新
		g_spScreen->GetScene()->GetCameraManager()->Update( fpsCtrl );

		//終了判定
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) || bExit == 1 )
		{
			GAME_LOOP_EXIT();
		}

		if ( Ckeyboard_->IsKeyOn( KEY_R ) && Ckeyboard_->IsKeyRepeat( KEY_UP ) )
		{
			r += 0.5f;
		}
		else if ( Ckeyboard_->IsKeyOn( KEY_R ) && Ckeyboard_->IsKeyRepeat( KEY_DOWN ) )
		{
			r -= 0.5f;
		}

		if ( Ckeyboard_->IsKeyOn( KEY_T ) && Ckeyboard_->IsKeyRepeat( KEY_UP ) )
		{
			theta += CMathUtil_::DegToRad( 1 );
		}
		else if ( Ckeyboard_->IsKeyOn( KEY_T ) && Ckeyboard_->IsKeyRepeat( KEY_DOWN ) )
		{
			theta -= CMathUtil_::DegToRad( 1 );
		}

		if ( Ckeyboard_->IsKeyOn( KEY_P ) && Ckeyboard_->IsKeyRepeat( KEY_UP ) )
		{
			phi += CMathUtil_::DegToRad( 1 );
		}
		else if ( Ckeyboard_->IsKeyOn( KEY_P ) && Ckeyboard_->IsKeyRepeat( KEY_DOWN ) )
		{
			phi -= CMathUtil_::DegToRad( 1 );
		}

		spFont->PrintfSentence( 32 , 32 , graphic::CColor_Tomato , 1.0f , _T( "DebugPrint." ) );

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

	//文字管理の履き
	CFontManagerDispose_();

	g_spCamera.Delete();
	spFont.Delete();
	g_spScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif