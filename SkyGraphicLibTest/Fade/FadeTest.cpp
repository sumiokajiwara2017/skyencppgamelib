#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_FADE

using namespace sky::lib;

#include "SkyGraphicLib/Shader.h"

#if ON_

static const skyString *SCREEN_SETUP_NAME       = _T( "Test" ); 
static const skyString *FONT_SCREEN_SETUP_NAME  = _T( "font" ); 
static const skyString *FADE_SCREEN_SETUP_NAME  = _T( "fade" ); 

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
static graphic::Camera2DFixationPtr  g_spFadeCamera;

//テスト用モデル
static graphic::CModelPtr     g_TestCone;
static graphic::CModelPtr     g_TestPlane;

//スクリーン
static graphic::CScreenObjectPtr    g_spScreen;
static graphic::CScreenObjectPtr    g_spFontScreen;
static graphic::CScreenObjectPtr    g_spFadeScreen;

TEST( Fade , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	graphic::CBonePtr spBone = CBoneCreate_();

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

	//モデル作成
	g_TestCone = graphic::CModelUtil::CreateTestCone();
	g_TestPlane = graphic::CModelUtil::CreateTestPlane();

	//モデルをシーングラフに登録
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestCone );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestPlane );

	//モデルにアニメーションをつける
	graphic::CKeyFrameEntity3DAnimationPtr spAnim = CKeyFrameEntity3DAnimationCreate_();
	spAnim->GetFCurve().AddKey( 2 , math::CEntity3D(
		math::CBasicVector3( 5 , 5 , 5 ) , //位置
		math::CBasicVector3( 360 , 360 , 360 ) , //回転
		math::CBasicVector3( 2 , 2 , 2 ) ) , eInterpolateType_Linear );
	spAnim->Play( 0 , 1.0f , 0.0f , kfm::ePlayEndType_Back );
	g_TestCone->SetAlpha( skyTrue );
	g_TestCone->GetMesh()->GetMaterial()->Alpha = 0.5f;
	GET_BONE( g_TestCone )->AttachKeyFrameEntity3DAnimation( spAnim );

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

//------------------------------------------------------------

	//文字カメラ
	g_spFadeCamera = Camera2DFixationCreate_();
	g_spFadeCamera->vSetUp();

	//文字スクリーンのセットアップ
	screenData.Name               = CHS( FADE_SCREEN_SETUP_NAME );        //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = g_spFadeCamera;                       //カメラ

	//スクリーンのセットアップ
	g_spFadeScreen = g_spScreen->PushLayer( screenData );

	//フェード用スプライトの生成
	CFadeManager_->CreateFadeSprite( g_spScreen->GetWindow()->GetRect().W() , g_spScreen->GetWindow()->GetRect().H() );
	CFadeManager_->SetFadeColor( graphic::CColor_Red );

	//フェードスプライトをシーングラフに登録
	g_spFadeScreen->GetScene()->GetSceneGraphRoot()->AttachChild( CFadeManager_->GetSceneGraphRoot() );

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
		Ckeyboard_->Update( fpsCtrl );

		//フェード管理の更新
		CFadeManager_->Update( fpsCtrl );

		//カメラ管理を更新
		g_spScreen->GetScene()->GetCameraManager()->Update( fpsCtrl );

		spFont->PrintfSentence( 32 , 32 , graphic::CColor_Tomato , 1.0f , _T( "DebugPrint." ) );

		//フェードイン
		if ( Ckeyboard_->IsKeyPress( KEY_I ) )
		{
			CFadeManager_->ExecFadeIn();
		}

		//フェードアウト
		if ( Ckeyboard_->IsKeyPress( KEY_O ) )
		{
			CFadeManager_->ExecFadeOut();
		}

		//フェードイン
		if ( Ckeyboard_->IsKeyPress( KEY_S ) )
		{
			CFadeManager_->SetFadeInEnd();
		}

		//フェードアウト
		if ( Ckeyboard_->IsKeyPress( KEY_E ) )
		{
			CFadeManager_->SetFadeOutEnd();
		}


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