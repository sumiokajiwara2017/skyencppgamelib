#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_UI

#include "SkyFrameWorkLib/UI.h"
#include "SkyFrameWorkLib/Input.h"

using namespace sky::lib;

#if ON_

static const skyString *UI_SCREEN_NAME                   = _T( "UIScreen" ); 
static const skyString *CURSOR_SCREEN_NAME               = _T( "CursorScreen" ); 

//カメラ初期情報
static const math::CBasicVector3 _CAMERA_DEFAULT_UP_VECTOR	= math::CBasicVector3( 0 , 1 , 0 );
static const dectype			 _CAMERA_DEFAULT_NEAR		= graphic::RENDERING_2D_PRIO_FAST;
static const dectype			 _CAMERA_DEFAULT_FAR		= graphic::RENDERING_2D_PRIO_MAX;

//Window初期位置、サイズ
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//１秒あたりのフレーム数
static const u32 _FRAME_NUM = 60;

static const u32 RENDER_QUEUE_SIZE = 3;

//カメラ
static graphic::CCamera2DPtr  g_spCamera;

//スクリーン
static graphic::CScreenObjectPtr    g_spUIScreen;
static graphic::CScreenObjectPtr    g_spCursorScreen;

TEST( ui , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//終了感知コールバックを登録する
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//フルスクリーンモードOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//--------UIScreenのセットアップ--------▽

	//カメラの作成／パラメーター設定／初期化
	g_spCamera = Camera2DFixationCreate_();
	g_spCamera->SetUpVector( _CAMERA_DEFAULT_UP_VECTOR );
	g_spCamera->SetNear( _CAMERA_DEFAULT_NEAR );
	g_spCamera->SetFar( _CAMERA_DEFAULT_FAR );
	g_spCamera->SetLeftTopCenter( skyTrue );

	//ルートスクリーンのセットアップ
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( UI_SCREEN_NAME );             //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = g_spCamera;                           //カメラ
	screenData.WindowRect.X()     = _WIN_X;                               //画面位置X
	screenData.WindowRect.Y()     = _WIN_Y;                               //画面位置Y
	screenData.WindowRect.W()     = _WIN_W;                               //画面幅
	screenData.WindowRect.H()     = _WIN_H;                               //画面高さ
	screenData.RenderState        = IRenderStatCreate_();                 //レンダーステート

	//スクリーンのセットアップ
	g_spUIScreen = CScreenObject_::Create( screenData );

	//文字を出力するので文字管理を初期化
	CFontManagerInitialize_();

	//シーングラフのルートを作成
	g_spUIScreen->GetScene()->m_spSceneGraph = graphic::CSceneGraphNodePtr( NEW graphic::CSceneGraphNode() );

	//フォームの作成
	framework::ui::CUITextListFormPtr spSampleForm = framework::ui::CUITextListForm::Create();
	spSampleForm->SetSize( math::CBasicVector2( 500 , 800 ) );
	spSampleForm->SetSize( math::CBasicVector2( 400 , 400 ) );

	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestLog1" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestTestTestTestTestTeTestTestTestTestTestTestTestTestTestTestTestTestTeststTestTestTestTestTestTestLog2" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestLog3" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestLog4" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestLog5" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestLog6" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestLog7" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestLog8" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestLog9" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestLog10" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestLog11" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestLog12" ) );

	//UIのシーングラフのルート設定する
	g_spUIScreen->GetScene()->m_spSceneGraph->AttachChild( CUIController_->GetSceneGraphRoot() );

//--------UIScreenのセットアップ--------△

//--------CursorScreenのセットアップ--------▽

	//ルートスクリーンのセットアップ
	screenData.Name               = CHS( CURSOR_SCREEN_NAME );             //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = g_spCamera;                           //カメラ

	g_spCursorScreen = g_spUIScreen->PushLayer( screenData );

	//シーングラフのルートを作成
	g_spCursorScreen->GetScene()->m_spSceneGraph = graphic::CSceneGraphNodePtr( NEW graphic::CSceneGraphNode() );

	//マウス矢印を生成
	framework::CMouseArrowPtr spMouse( NEW framework::CMouseArrow() );

	//マウス矢印を設定
	g_spCursorScreen->GetScene()->m_spSceneGraph->AttachChild( spMouse->m_spRootSprite );

//--------CursorScreenのセットアップ--------△

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

		//カメラの更新
		g_spCamera->vUpdate( fpsCtrl.GetPassageTime() );

		//カメラ移動、モデル移動入力受付
		math::CEntityUtil::DefaultInput_Entity2D( g_spCamera->GetEntity2D() , KEY_C );

		//カメラ移動、モデル移動を反映
		graphic::CCameraUtil::SetEntity2D( g_spCamera );

		//UIコントローラーの更新
		CUIController_->Update();

		//描画中だったらコマンド発行する
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//コマンドバッファの作成
			CRenderCommandManager_::CreateBuffer();

			//描画コマンド発行
			g_spUIScreen->Rendering();

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

	g_spCamera.Delete();
	spMouse.Delete();
	g_spUIScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif