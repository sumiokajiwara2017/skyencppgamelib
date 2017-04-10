#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_SHADER

#include "SkyFrameWorkLib/UI.h"
#include "SkyFrameWorkLib/Input.h"

using namespace sky::lib;

#if ON_

#ifdef SKY_GRAPHIC_LIB_OPENGL

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

#include "Cg/CgLightShaderObject.h"

static const skyString *TEST1_OBJ_ROOT = _T( "data/obj/" );
static const skyString *TEST1_MTL_ROOT = _T( "data/mtl/" );
static const skyString *TEST1_TEX_ROOT = _T( "data/texture/" );

static const skyString *TEX_CUBE_MODEL_FILE = _T( "cube.obj" );

static const skyString *TD_SCREEN_NAME                   = _T( "3DScreen" ); 
static const skyString *CURSOR_SCREEN_NAME               = _T( "CursorScreen" ); 
static const skyString *FONT_SCREEN_NAME                 = _T( "FontScreen" ); 
static const skyString *UI_SCREEN_NAME                   = _T( "UIScreen" ); 

//カメラ初期情報
static const math::CBasicVector3 _CAMERA_DEFAULT_UP_VECTOR	= math::CBasicVector3( 0 , 1 , 0 );
static const dectype			 _CAMERA2D_DEFAULT_NEAR		= graphic::RENDERING_2D_PRIO_FAST;
static const dectype			 _CAMERA2D_DEFAULT_FAR		= graphic::RENDERING_2D_PRIO_MAX;
static const dectype			 _CAMERA3D_DEFAULT_NEAR		= 1;
static const dectype			 _CAMERA3D_DEFAULT_FAR		= 1000;
static const dectype			 _CAMERA3D_DEFAULT_FOV		= math::CMathUtil::DegToRad( 30.0f );

//Window初期位置、サイズ
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//１秒あたりのフレーム数
static const u32 _FRAME_NUM = 60;

static const u32 RENDER_QUEUE_SIZE = 3;

//カメラ

static graphic::CCamera3DSatellitePtr  g_spCamera3D;
//static graphic::CCamera3DDebugPtr    g_spCamera3D;
static graphic::CCamera2DPtr           g_spCamera2D;

//スクリーン
static graphic::CScreenObjectPtr    g_sp3DScreen;
static graphic::CScreenObjectPtr    g_spFontScreen;
static graphic::CScreenObjectPtr    g_spUIScreen;
static graphic::CScreenObjectPtr    g_spCursorScreen;

//モデル
static graphic::CModelPtr     g_TestCube;
static graphic::CModelPtr     g_TestCone;
static graphic::CModelPtr     g_TestPlane;

TEST( shader , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//終了感知コールバックを登録する
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//フルスクリーンモードOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//--------カメラのセットアップ--------▽

	//カメラの作成／パラメーター設定／初期化
	g_spCamera3D = CCamera3DSatelliteCreate_();
//    g_spCamera3D = CCamera3DDebugCreate_();
	g_spCamera3D->SetUpVector( _CAMERA_DEFAULT_UP_VECTOR );
	g_spCamera3D->SetNear( _CAMERA3D_DEFAULT_NEAR );
	g_spCamera3D->SetFar( _CAMERA3D_DEFAULT_FAR );
	g_spCamera3D->SetFov( _CAMERA3D_DEFAULT_FOV );

	//カメラの作成／パラメーター設定／初期化
	g_spCamera2D = Camera2DFixationCreate_();
	g_spCamera2D->SetUpVector( _CAMERA_DEFAULT_UP_VECTOR );
	g_spCamera2D->SetNear( _CAMERA2D_DEFAULT_NEAR );
	g_spCamera2D->SetFar( _CAMERA2D_DEFAULT_FAR );
	g_spCamera2D->SetLeftTopCenter( skyTrue );

//--------3DScreenのセットアップ--------▽

	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( TD_SCREEN_NAME );                //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = g_spCamera3D;                         //カメラ
	screenData.WindowRect.X()     = _WIN_X;                               //画面位置X
	screenData.WindowRect.Y()     = _WIN_Y;                               //画面位置Y
	screenData.WindowRect.W()     = _WIN_W;                               //画面幅
	screenData.WindowRect.H()     = _WIN_H;                               //画面高さ
	screenData.RenderState        = IRenderStatCreate_();                 //レンダーステート

	//スクリーンのセットアップ
	g_sp3DScreen = CScreenObject_::Create( screenData );

	g_TestCube = graphic::CModelUtil::CreateFromWavefrontOBJ( CHSC( TEX_CUBE_MODEL_FILE ) );
	g_TestCone = graphic::CModelUtil::CreateTestCone();
	g_TestPlane = graphic::CModelUtil::CreateTestPlane();

	//シェーダーを取り替える
	g_TestCube->DetachShaderObjectAll();
//	g_TestCube->AttachShaderObject( 0 , graphic::CCgLightShaderObject::Create() );
    g_TestCube->AttachShaderObject( 0 , graphic::CBasicShaderObject::Create() );

	//モデルをシーングラフに登録
	g_sp3DScreen->GetScene()->m_spSceneGraph = CSceneGraphNodeCreate_();
	g_sp3DScreen->GetScene()->m_spSceneGraph->AttachChild( g_TestCube ); g_TestCube->GetEntity3D()->Pos.Y = 1.0f;
	g_sp3DScreen->GetScene()->m_spSceneGraph->AttachChild( g_TestCone ); g_TestCone->GetEntity3D()->Pos.X = 4.0f; g_TestCone->GetEntity3D()->Pos.Y = 1.0f;
	g_sp3DScreen->GetScene()->m_spSceneGraph->AttachChild( g_TestPlane );

//--------CursorScreenのセットアップ--------▽

	//ルートスクリーンのセットアップ
	screenData.Name               = CHS( CURSOR_SCREEN_NAME );             //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = g_spCamera2D;                           //カメラ

	g_spCursorScreen = g_sp3DScreen->PushLayer( screenData );

	//シーングラフのルートを作成
	g_spCursorScreen->GetScene()->m_spSceneGraph = graphic::CSceneGraphNodePtr( NEW graphic::CSceneGraphNode() );

	//マウス矢印を生成
	framework::CMouseArrowPtr spMouse( NEW framework::CMouseArrow() );

	//マウス矢印を設定
	g_spCursorScreen->GetScene()->m_spSceneGraph->AttachChild( spMouse->m_spRootSprite );

//--------FontScreenのセットアップ--------▽

	//ルートスクリーンのセットアップ
	screenData.Name               = CHS( FONT_SCREEN_NAME );             //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = g_spCamera2D;                           //カメラ

	g_spFontScreen = g_sp3DScreen->PushLayer( screenData );

	//シーングラフのルートを作成
	g_spFontScreen->GetScene()->m_spSceneGraph = graphic::CSceneGraphNodePtr( NEW graphic::CSceneGraphNode() );

	//文字を出力するので文字管理を初期化
	CFontManagerInitialize_();

	//文字を作成
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();

	//文字をシーングラフに登録
	g_spFontScreen->GetScene()->m_spSceneGraph->AttachChild( spFont );

//--------UIScreenのセットアップ--------▽

	//ルートスクリーンのセットアップ
	screenData.Name               = CHS( UI_SCREEN_NAME );             //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = g_spCamera2D;                           //カメラ
	screenData.RenderState        = IRenderStatCreate_();                 //レンダーステート

	//スクリーンのセットアップ
	g_spUIScreen = g_sp3DScreen->PushLayer( screenData );

	//シーングラフのルートを作成
	g_spUIScreen->GetScene()->m_spSceneGraph = graphic::CSceneGraphNodePtr( NEW graphic::CSceneGraphNode() );

	//フォームの作成
	framework::ui::CUITextListFormPtr spSampleForm = framework::ui::CUITextListForm::Create( CHS( _T( "" ) ) );
	spSampleForm->SetSize( math::CBasicVector2( 500 , 800 ) );
	spSampleForm->SetSize( math::CBasicVector2( 400 , 400 ) );

	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTest" ) );

	//UIのシーングラフのルート設定する
	g_spUIScreen->GetScene()->m_spSceneGraph->AttachChild( CUIController_->GetSceneGraphRoot() );

//--------アプリケーション開始--------▽

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

		//文字を出す
		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "%f ms " ) , fpsCtrl.GetPassageTime() );

		//カメラの更新
		g_spCamera3D->vUpdate( fpsCtrl.GetPassageTime() );

		//モデル移動位置反映
		graphic::CModelUtil::SetEntity3D( g_TestCube );
		graphic::CModelUtil::SetEntity3D( g_TestCone );

		//UIコントローラーの更新
		CUIController_->Update( fpsCtrl.GetPassageTime() );

		//描画中だったらコマンド発行する
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//コマンドバッファの作成
			CRenderCommandManager_::CreateBuffer();

			//描画コマンド発行
			g_sp3DScreen->Rendering();

			//コマンドバッファの追加
			CRenderCommandManager_::PushBuffer();
		}

		//フレームレート維持のため、時間が余ったら待つ
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	//描画用スレッドの停止
	CRenderCommandManager_::StopThread();

	//文字管理の破棄
	CFontManagerDispose_();

	g_spCamera2D.Delete();
	g_spCamera3D.Delete();
	spMouse.Delete();
	g_spUIScreen.Delete();
	g_sp3DScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif

#endif

#endif