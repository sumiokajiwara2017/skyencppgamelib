#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_SPRITE

using namespace sky::lib;

//テスト用テクスチャファイルパス
#define TEX_NO ( 3 )
#if TEX_NO == 1
static const skyString *TEST1_TEXTURE_PATH = _T( "data/test/texture/TestBmp8bit.bmp" );
#elif TEX_NO == 2
static const skyString *TEST1_TEXTURE_PATH = _T( "data/test/texture/TestJpg.jpg" );
#elif TEX_NO == 3
static const skyString *TEST1_TEXTURE_PATH = _T( "data/test/texture/TestPng.png" );
#elif TEX_NO == 4
static const skyString *TEST1_TEXTURE_PATH = _T( "data/test/texture/TestTga.tga" );
#elif TEX_NO == 5
static const skyString *TEST1_TEXTURE_PATH = _T( "data/test/texture/Bmp4bit.bmp" );
#elif TEX_NO == 6
static const skyString *TEST1_TEXTURE_PATH = _T( "data/test/texture/Bmp24bit.bmp" );
#elif TEX_NO == 7
static const skyString *TEST1_TEXTURE_PATH = _T( "data/test/texture/Bmp32bit.bmp" );
#endif

//Window初期位置、サイズ
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//１秒あたりのフレーム数
static const u32 _FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

#include "SkyGraphicLib/Sprite.h"

#if OFF_

TEST( sprite , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//フルスクリーンモードOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

	//カメラの作成
	graphic::CameraPtr spCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spCamera->SetOrtho( skyTrue );
	spCamera->SetDefaultInput();

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

	//テクスチャ読み込み
	graphic::CTexturePtr spTexture0 = CTextureCreateName_( CHSC( TEST1_TEXTURE_PATH ) );
	spTexture0->Load(); //ロード

	//テクスチャ付きの矩形を作成
	graphic::CSpritePtr spSprite = graphic::CSpriteUtil::CreateSquare( spTexture0 , graphic::RENDERING_2D_PRIO_FAST , math::CBasicVector2( 2 , 2 ) , graphic::CColor_White );

	//スプライトをシーングラフに登録
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spSprite );
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CSpriteUtil::CreateStandardLine2() );

	//文字管理の初期化（DirectX11の場合、Windowが生成され、Deviceが生成された後でないと初期化できない
	CFontManagerInitialize_();

	//文字カメラ
	graphic::CameraPtr spFontCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spFontCamera->SetOrtho( skyTrue );
	spFontCamera->SetLeftTopCenter( skyTrue );

	//文字スクリーンのセットアップ
	screenData.Name               = CHS( _T( "FontScreen" ) );        //スクリーン名
	screenData.Camera             = spFontCamera;                       //カメラ

	//スクリーンのセットアップ
	graphic::CScreenObjectPtr spFontScreen = spMainScreen->PushLayer( screenData );

	//文字を作成
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();
	spFont->SetUp();

	//文字をシーングラフに登録
	spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spFont );

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

		if ( Ckeyboard_->IsKeyRepeat( KEY_R ) )
		{
			math::CBasicAngle angle;
			angle.FromRad( math::CMathUtil::DegToRad( 5 ) );
		}

		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "Sprite Test Draw." ) );

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

#endif

#if ON_

TEST( sprite , 2 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//フルスクリーンモードOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

	//カメラの作成
	graphic::CameraPtr spCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spCamera->SetDefaultInput();
	spCamera->SetEye( math::CBasicVector3( 0 , 0 , 15 ) );

	//ルートスクリーンのセットアップ
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( _T( "MainScreen" ) );             //スクリーン名
	screenData.ClearColor         = graphic::CColor_Black;             //スクリーンクリアカラー
	screenData.Camera             = spCamera;                             //カメラ
	screenData.WindowRect.X()     = _WIN_X;                               //画面位置X
	screenData.WindowRect.Y()     = _WIN_Y;                               //画面位置Y
	screenData.WindowRect.W()     = _WIN_W;                               //画面幅
	screenData.WindowRect.H()     = _WIN_H;                               //画面高さ
	screenData.RenderState        = CRenderStateCreate_();                 //レンダーステート

	//スクリーンのセットアップ
	graphic::CScreenObjectPtr spMainScreen = CScreenObject_::Create( screenData );

	//テクスチャ読み込み
	graphic::CTexturePtr spTexture0 = CTextureCreateName_( CHSC( _T( "data/test/texture/beam.bmp" ) ) );
	spTexture0->Load(); //ロード

	graphic::CBillboardBeamPtr spEffect = graphic::CBillboardBeam::Create();
	spEffect->Initialize( 32 , 0.25f , spTexture0 ); 

	//スプライトをシーングラフに登録
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spEffect );
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CSpriteUtil::CreateStandardLine2() );

	//文字管理の初期化（DirectX11の場合、Windowが生成され、Deviceが生成された後でないと初期化できない
	CFontManagerInitialize_();

	//文字カメラ
	graphic::CameraPtr spFontCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spFontCamera->SetOrtho( skyTrue );
	spFontCamera->SetLeftTopCenter( skyTrue );

	//文字スクリーンのセットアップ
	screenData.Name               = CHS( _T( "FontScreen" ) );        //スクリーン名
	screenData.Camera             = spFontCamera;                       //カメラ

	//スクリーンのセットアップ
	graphic::CScreenObjectPtr spFontScreen = spMainScreen->PushLayer( screenData );

	//文字を作成
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();
	spFont->SetUp();

	//文字をシーングラフに登録
	spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spFont );

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

		if ( Ckeyboard_->IsKeyRepeat( KEY_R ) )
		{
			math::CBasicAngle angle;
			angle.FromRad( math::CMathUtil::DegToRad( 5 ) );
		}

		//動かす
		graphic::CBillboardBeamUtil::Move_Thunder( spEffect );
//		graphic::CBillboardBeamUtil::Move_Rot( spEffect );

		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "Sprite Test Draw." ) );

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

#endif

#endif