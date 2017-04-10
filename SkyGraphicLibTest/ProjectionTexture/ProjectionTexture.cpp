#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_PROJECTION_TEXTURE

#if OFF_

using namespace sky::lib;

#include "SkyGraphicLib/Loader.h"

static const skyString *TEST1_OBJ_FILE_NAME_1 = _T( "testcube.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_2 = _T( "testcone.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_3 = _T( "testplane10x10.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_4 = _T( "testsphere.obj" );

static const skyString *SCREEN_SETUP_NAME                   = _T( "Test" ); 
static const skyString *FONT_SCREEN_SETUP_NAME              = _T( "font" ); 

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
static graphic::CModelPtr     g_TestCube_1;
static graphic::CModelPtr     g_TestCube_2;
static graphic::CModelPtr     g_TestSphere;
static graphic::CModelPtr     g_TestCubeShadow;
static graphic::CModelPtr     g_TestPlane;

//スクリーン
static graphic::CScreenObjectPtr    g_spScreen;
static graphic::CScreenObjectPtr    g_spFontScreen;

TEST( ProjectionTexture , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//終了感知コールバックを登録する
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
	g_spCamera->SetEye( math::CBasicVector3( 0.0f , 8.5f , -35.0f ) );
	g_spCamera->SetInitEye( g_spCamera->GetEye() );
	g_spCamera->SetTag( math::CBasicVector3( 0.0f , 8.5f , -25.0f ) );
	g_spCamera->SetInitTag( g_spCamera->GetTag() );

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

	//モデルを作成
	g_TestCube_1 = graphic::CModelUtil::CreateTestCube();
	GET_BONE( g_TestCube_1 )->SetPosition_X( 1.0f );
	GET_BONE( g_TestCube_1 )->SetPosition_Y( 5.0f );
	GET_BONE( g_TestCube_1 )->SetPosition_Z( 0.0f );

	g_TestCube_2 = graphic::CModelUtil::CreateTestCube();
	GET_BONE( g_TestCube_2 )->SetPosition_X( 5.0f );
	GET_BONE( g_TestCube_2 )->SetPosition_Y( 8.0f );
	GET_BONE( g_TestCube_2 )->SetPosition_Z( 4.0f );

	g_TestSphere = graphic::CModelUtil::CreateTestSphere( graphic::CColor_Red );
	GET_BONE( g_TestSphere )->MulScale_XYZ( math::CBasicVector3( 0.3f , 0.3 , 0.3 ) );
	GET_BONE( g_TestSphere )->SetPosition_X( -3.0f );
	GET_BONE( g_TestSphere )->SetPosition_Y( 8.0f );
	GET_BONE( g_TestSphere )->SetPosition_Z( 0.0f );
	 
	g_TestCubeShadow = g_TestCube_1->CloneSharingTraverse();
	g_TestCubeShadow->ActivateTraverse();

	g_TestPlane = graphic::CModelUtil::CreateTestPlane();
	GET_BONE( g_TestPlane )->SetPosition_Y( -0.4f );

	//リフレクション
//	GET_BONE( g_TestConeShadow )->MulReflect( math::eAxis_x );
//	GET_BONE( g_TestConeShadow )->MulReflect( math::eAxis_y );
//	GET_BONE( g_TestConeShadow )->MulReflect( math::eAxis_z );
//	GET_BONE( g_TestConeShadow )->MulReflect( math::CBasicVector3( 1.0f , 1.0f , 0.0f ).Normalize() );

	//せん断
//	GET_BONE( g_TestConeShadow )->MulShear( math::eAxis_x , 10.0f , 3.0f );

	//平面投射
//	GET_BONE( g_TestConeShadow )->MulProjection( math::CBasicVector3( 1.0f , 0.0f , 0.0f ).Normalize() ); //YZ平面
//	GET_BONE( g_TestConeShadow )->MulProjection( math::CBasicVector3( 0.0f , 1.0f , 0.0f ).Normalize() ); //XZ平面
//	GET_BONE( g_TestConeShadow )->MulProjection( math::CBasicVector3( 0.0f , 0.0f , 1.0f ).Normalize() ); //XY平面
	GET_BONE( g_TestCubeShadow )->MulProjection( 
		math::CBasicPlane( math::CBasicVector3( 0.0f , 0.0f , 0.0f ) , math::CBasicVector3( 0.0f , 0.0f , 1.0f ) , math::CBasicVector3( 1.0f , 0.0f , 0.0f ) ) , 
		GET_BONE( g_TestSphere )->GetPosition_XYZ() - GET_BONE( g_TestCube_1 )->GetPosition_XYZ() );

	//グローバルシェーダーを作成
	graphic::CPointLightShaderObjectPtr pointLight = graphic::CPointLightShaderObject::Create();
	pointLight->SetLightPosition( math::CBasicVector3( 10.0f , 10.0f , -10.0f ) );
	pointLight->SetLightPower( 5.0f );

	//ローカルシェーダーの削除
	g_TestCube_1->DetachShader();
	g_TestCube_2->DetachShader();
	g_TestCubeShadow->DetachShader();
	g_TestPlane->DetachShader();

	//モデルをシーングラフに登録
	pointLight->AttachChild( g_TestCube_1 );
	pointLight->AttachChild( g_TestCube_2 );
	pointLight->AttachChild( g_TestCubeShadow );
	pointLight->AttachChild( g_TestPlane );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( pointLight );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestSphere );
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

		spFont->PrintfSentence( 16 , 16 * 1 , graphic::CColor_Tomato , 1.0f , _T( "cameraPos = [ %f %f %f ]" ) , g_spCamera->GetEye().X , g_spCamera->GetEye().Y , g_spCamera->GetEye().Z );
		spFont->PrintfSentence( 16 , 16 * 2 , graphic::CColor_Tomato , 1.0f , _T( "cameraTag = [ %f %f %f ]" ) , g_spCamera->GetTag().X , g_spCamera->GetTag().Y , g_spCamera->GetTag().Z );

		//カメラの更新
		g_spCamera->vUpdate( fpsCtrl.GetPassageTime() );

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

#include "SkyGraphicLib/Loader.h"

static const skyString *TEST1_OBJ_FILE_NAME_1 = _T( "testcube.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_2 = _T( "testcone.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_3 = _T( "testplane10x10.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_4 = _T( "testsphere.obj" );

static const skyString *SCREEN_SETUP_NAME                   = _T( "Test" ); 
static const skyString *FONT_SCREEN_SETUP_NAME              = _T( "font" ); 

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
static graphic::CModelPtr     g_TestCube_1;
static graphic::CModelPtr     g_TestCube_2;
static graphic::CModelPtr     g_TestSphere;
static graphic::CModelPtr     g_TestCubeShadow;
static graphic::CModelPtr     g_TestPlane;

//スクリーン
static graphic::CScreenObjectPtr    g_spScreen;
static graphic::CScreenObjectPtr    g_spFontScreen;

TEST( ProjectionTexture , 2 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//終了感知コールバックを登録する
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
	g_spCamera->SetEye( math::CBasicVector3( 0.0f , 8.5f , -35.0f ) );
	g_spCamera->SetInitEye( g_spCamera->GetEye() );
	g_spCamera->SetTag( math::CBasicVector3( 0.0f , 8.5f , -25.0f ) );
	g_spCamera->SetInitTag( g_spCamera->GetTag() );

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

	//モデルを作成
	g_TestCube_1 = graphic::CModelUtil::CreateTestCube();
	GET_BONE( g_TestCube_1 )->SetPosition_X( 1.0f );
	GET_BONE( g_TestCube_1 )->SetPosition_Y( 5.0f );
	GET_BONE( g_TestCube_1 )->SetPosition_Z( 0.0f );

	g_TestCube_2 = graphic::CModelUtil::CreateTestCube();
	GET_BONE( g_TestCube_2 )->SetPosition_X( 5.0f );
	GET_BONE( g_TestCube_2 )->SetPosition_Y( 8.0f );
	GET_BONE( g_TestCube_2 )->SetPosition_Z( 4.0f );

	g_TestSphere = graphic::CModelUtil::CreateTestSphere( graphic::CColor_Red );
	GET_BONE( g_TestSphere )->MulScale_XYZ( math::CBasicVector3( 0.3f , 0.3 , 0.3 ) );
	GET_BONE( g_TestSphere )->SetPosition_X( -3.0f );
	GET_BONE( g_TestSphere )->SetPosition_Y( 8.0f );
	GET_BONE( g_TestSphere )->SetPosition_Z( 0.0f );
	 
	g_TestCubeShadow = g_TestCube_1->CloneSharingTraverse();
	g_TestCubeShadow->ActivateTraverse();

	g_TestPlane = graphic::CModelUtil::CreateTestPlane();
	GET_BONE( g_TestPlane )->SetPosition_Y( -0.4f );

	//リフレクション
//	GET_BONE( g_TestConeShadow )->MulReflect( math::eAxis_x );
//	GET_BONE( g_TestConeShadow )->MulReflect( math::eAxis_y );
//	GET_BONE( g_TestConeShadow )->MulReflect( math::eAxis_z );
//	GET_BONE( g_TestConeShadow )->MulReflect( math::CBasicVector3( 1.0f , 1.0f , 0.0f ).Normalize() );

	//せん断
//	GET_BONE( g_TestConeShadow )->MulShear( math::eAxis_x , 10.0f , 3.0f );

	//平面投射
//	GET_BONE( g_TestConeShadow )->MulProjection( math::CBasicVector3( 1.0f , 0.0f , 0.0f ).Normalize() ); //YZ平面
//	GET_BONE( g_TestConeShadow )->MulProjection( math::CBasicVector3( 0.0f , 1.0f , 0.0f ).Normalize() ); //XZ平面
//	GET_BONE( g_TestConeShadow )->MulProjection( math::CBasicVector3( 0.0f , 0.0f , 1.0f ).Normalize() ); //XY平面
	GET_BONE( g_TestCubeShadow )->MulProjection( 
		math::CBasicPlane( math::CBasicVector3( 0.0f , 0.0f , 0.0f ) , math::CBasicVector3( 0.0f , 0.0f , 1.0f ) , math::CBasicVector3( 1.0f , 0.0f , 0.0f ) ) , 
		GET_BONE( g_TestSphere )->GetPosition_XYZ() - GET_BONE( g_TestCube_1 )->GetPosition_XYZ() );

	//グローバルシェーダーを作成
	graphic::CPointLightShaderObjectPtr pointLight = graphic::CPointLightShaderObject::Create();
	pointLight->SetLightPosition( math::CBasicVector3( 10.0f , 10.0f , -10.0f ) );
	pointLight->SetLightPower( 5.0f );

	//ローカルシェーダーの削除
	g_TestCube_1->DetachShader();
	g_TestCube_2->DetachShader();
	g_TestCubeShadow->DetachShader();
	g_TestPlane->DetachShader();

	//モデルをシーングラフに登録
	pointLight->AttachChild( g_TestCube_1 );
	pointLight->AttachChild( g_TestCube_2 );
	pointLight->AttachChild( g_TestCubeShadow );
	pointLight->AttachChild( g_TestPlane );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( pointLight );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestSphere );
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

		spFont->PrintfSentence( 16 , 16 * 1 , graphic::CColor_Tomato , 1.0f , _T( "cameraPos = [ %f %f %f ]" ) , g_spCamera->GetEye().X , g_spCamera->GetEye().Y , g_spCamera->GetEye().Z );
		spFont->PrintfSentence( 16 , 16 * 2 , graphic::CColor_Tomato , 1.0f , _T( "cameraTag = [ %f %f %f ]" ) , g_spCamera->GetTag().X , g_spCamera->GetTag().Y , g_spCamera->GetTag().Z );

		//カメラの更新
		g_spCamera->vUpdate( fpsCtrl.GetPassageTime() );

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


#endif