#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_COLLISION

#if ON_

using namespace sky::lib;

//Window初期位置、サイズ
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//１秒あたりのフレーム数
static const u32 _FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//２Ｄジオメトリ表示
TEST( collision , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//フルスクリーンモードOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

	//カメラの作成
	graphic::Camera2DPtr spCamera = Camera2DFixationCreate_();
	spCamera->vSetUp();
	spCamera->SetDefaultInputOn( skyTrue );

	//ルートスクリーンのセットアップ
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( _T( "MainScreen" ) );             //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = spCamera;                             //カメラ
	screenData.WindowRect.X()     = _WIN_X;                               //画面位置X
	screenData.WindowRect.Y()     = _WIN_Y;                               //画面位置Y
	screenData.WindowRect.W()     = _WIN_W;                               //画面幅
	screenData.WindowRect.H()     = _WIN_H;                               //画面高さ
	screenData.RenderState        = CRenderStateCreate_();                 //レンダーステート

	//スクリーンのセットアップ
	graphic::CScreenObjectPtr spMainScreen = CScreenObject_::Create( screenData );

	//文字カメラ
	graphic::Camera2DPtr spFontCamera = Camera2DFixationCreate_();
	spFontCamera->vSetUp();
	spFontCamera->SetLeftTopCenter( skyTrue );

	//文字スクリーンのセットアップ
	screenData.Name               = CHS( _T( "FontScreen" ) );        //スクリーン名
	screenData.Camera             = spFontCamera;                       //カメラ

	//スクリーンのセットアップ
	graphic::CScreenObjectPtr spFontScreen = spMainScreen->PushLayer( screenData );

	//文字を出力するので文字管理を初期化
	CFontManagerInitialize_();

	//文字を作成
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();
	spFont->SetUp();

	//文字をシーングラフに登録
	spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spFont );

//-----------------------------------------------------------------------
//スプライトの作成

	//基準座標
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CSpriteUtil::CreateStandardLine2() );

	//円の描画

//-----------------------------------------------------------------------


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

		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "Geometory 2D Draw." ) );

		//描画中だったらコマンド発行する
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//コマンドバッファの作成
			CRenderCommandManager_::CreateBuffer();

			//描画コマンド発行
			spMainScreen->Rendering( fpsCtrl );

			//コマンドバッファの追加
			CRenderCommandManager_::PushBuffer();
		}

		//フレームレート維持のため、時間が余ったら待つ
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	SKY_LIB_GRAPHIC_TERM();
}

//３Ｄジオメトリ表示
TEST( collision , 2 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif