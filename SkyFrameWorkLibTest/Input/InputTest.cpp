#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_INPUT

using namespace sky::lib;

#if ON_

static const skyString *SCREEN_SETUP_NAME                   = _T( "Test" ); 

//カメラ初期情報
static const math::CBasicVector3 _CAMERA_DEFAULT_UP_VECTOR	= math::CBasicVector3( 0 , 1 , 0 );
static const dectype			 _CAMERA_DEFAULT_NEAR		= graphic::RENDERING_2D_PRIO_FAST;
static const dectype			 _CAMERA_DEFAULT_FAR		= graphic::RENDERING_2D_PRIO_MAX;

//Window初期位置、サイズ
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640;
static const u32  _WIN_H = 480;

//１秒あたりのフレーム数
static const u32 _FRAME_NUM = 60;

static const u32 RENDER_QUEUE_SIZE = 3;

//カメラ
static graphic::CCamera2DPtr  g_spCamera;

//テスト用モデル
static graphic::CSpritePtr     g_spSprite;

//スクリーン
static graphic::CScreenObjectPtr    g_spScreen;

TEST( input , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//終了感知コールバックを登録する
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//フルスクリーンモードOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//--------2DScreenのセットアップ--------▽

	//カメラの作成／パラメーター設定／初期化
	g_spCamera = Camera2DFixationCreate_();
	g_spCamera->SetUpVector( _CAMERA_DEFAULT_UP_VECTOR );
	g_spCamera->SetNear( _CAMERA_DEFAULT_NEAR );
	g_spCamera->SetFar( _CAMERA_DEFAULT_FAR );
	g_spCamera->SetLeftTopCenter( skyTrue );

	//ルートスクリーンのセットアップ
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( SCREEN_SETUP_NAME );             //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = g_spCamera;                           //カメラ
	screenData.WindowRect.X()     = _WIN_X;                               //画面位置X
	screenData.WindowRect.Y()     = _WIN_Y;                               //画面位置Y
	screenData.WindowRect.W()     = _WIN_W;                               //画面幅
	screenData.WindowRect.H()     = _WIN_H;                               //画面高さ
	screenData.RenderState        = IRenderStatCreate_();                 //レンダーステート

	//スクリーンのセットアップ
	g_spScreen = CScreenObject_::Create( screenData );

	//文字を出力するので文字管理を初期化
	CFontManagerInitialize_();

	//シーングラフのルートを作成
	g_spScreen->GetScene()->m_spSceneGraph = graphic::CSceneGraphNodePtr( NEW graphic::CSceneGraphNode() );

	//矩形を作成
	g_spSprite = graphic::CSpriteUtil::CreateSquare( graphic::RENDERING_2D_PRIO_FAST , math::CBasicVector2( 64 , 64 ) );

	//スプライトをシーングラフに登録
	g_spScreen->GetScene()->m_spSceneGraph->AttachChild( g_spSprite );

	//文字を作成
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();

	//文字をシーングラフに登録
	g_spScreen->GetScene()->m_spSceneGraph->AttachChild( spFont );

	//マウス矢印を生成
	framework::CMouseArrowPtr spMouse( NEW framework::CMouseArrow() );

	//マウス矢印を設定
	g_spScreen->GetScene()->m_spSceneGraph->AttachChild( spMouse->m_spSprite );

//--------2DScreenのセットアップ--------△

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

		//モデルの回転
		g_spSprite->GetEntity2D().Pos.X = _WIN_W / 2;
		g_spSprite->GetEntity2D().Pos.Y = _WIN_H / 2;
	
		//カメラ移動、モデル移動入力受付
		math::CEntityUtil::DefaultInput_Entity2D( g_spCamera->GetEntity2D() , KEY_C );
		math::CEntityUtil::DefaultInput_Entity2D( g_spSprite->GetEntity2D() , KEY_X );

		//カメラ移動、モデル移動を反映
		graphic::CCameraUtil::SetEntity2D( g_spCamera );
		graphic::CSpriteUtil::SetEntity2D( g_spSprite );

		//文字を出す
		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "%f ms " ) , fpsCtrl.GetPassageTime() );
		spFont->PrintfSentence( 16 , 32 , graphic::CColor_White , 1.0f , _T( "UITest" ) );

		//リセット
		if ( Ckeyboard_->IsKeyPress( KEY_HOME ) )
		{
			g_spSprite->GetProperty().ResetLocal();
			g_spSprite->GetProperty().ResetParent();
			g_spSprite->GetProperty().ResetWorld();
		}


		//描画中だったらコマンド発行する
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//コマンドバッファの作成
			CRenderCommandManager_::CreateBuffer();

			//描画コマンド発行
			g_spScreen->Rendering();

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
	g_spSprite.Delete();
	spMouse.Delete();
	g_spScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif