#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_LOADER

#define DISP_TYPE ( 2 )

#include "SkyGraphicLib/DataBase.h"

#if DISP_TYPE == 0

using namespace sky::lib;

//Window初期位置、サイズ
static const dectype  _WIN_X = 0;
static const dectype  _WIN_Y = 0;
static const dectype  _WIN_W = 1024;
static const dectype  _WIN_H = 768;

//１秒あたりのフレーム数
static const u32 _FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//ポストエフェクトを使用
//#define USE_POST_EFFECT

TEST( Loader , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//フルスクリーンモードOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

	//カメラの作成
	graphic::CameraPtr spSpriteCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spSpriteCamera->SetUp();
	spSpriteCamera->SetOrtho( skyTrue );         //正射影

	//ルートスクリーンのセットアップ
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( _T( "MainScreen" ) );            //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = spSpriteCamera;                       //カメラ
	screenData.WindowRect.X()     = _WIN_X;                               //画面位置X
	screenData.WindowRect.Y()     = _WIN_Y;                               //画面位置Y
	screenData.WindowRect.W()     = _WIN_W;                               //画面幅
	screenData.WindowRect.H()     = _WIN_H;                               //画面高さ

	//スクリーンのセットアップ
	graphic::CScreenObjectPtr spMainScreen = CScreenObject_::Create( screenData );

//------ポストエフェクトスクリーンオブジェクト関連

	//カメラの作成
	graphic::CameraPtr spCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spCamera->SetUp();
	spCamera->SetOrtho( skyTrue );         //正射影

	//オフラインレンダリング用のスクリーンのセットアップ
	screenData.Name               = CHS( _T( "PostEffectScreen" ) );    //スクリーン名
	screenData.Camera             = spCamera;                           //カメラ
	screenData.WindowRect.W()     = _WIN_W;                               //画面幅
	screenData.WindowRect.H()     = _WIN_H;                               //画面高さ

	//ポストエフェクト用のスクリーンを作成
	graphic::CScreenObjectPtr spPostEffectScreen = spMainScreen->AddChild( screenData );
	spPostEffectScreen->SetReSizeOn( skyFalse );

	//テクスチャのロード
	graphic::CTexturePtr spTexture1 = graphic::CTexture::Create( CHS( "data/test/posteffect/haruhi.bmp" ) );
	graphic::CTexturePtr spTexture2 = graphic::CTexture::Create( CHS( "data/test/posteffect/lena.dds" ) ); //これはOpenGLでバグる。ローダーのせい
	spTexture1->Load();
	spTexture2->Load();

	//スプライトの作成
	graphic::CSpritePtr spSprite1 = graphic::CSpriteUtil::CreateSquare( spTexture1 );
	spSprite1->MulScale_XYZ( 2.0f );
	graphic::CSpritePtr spSprite2 = graphic::CSpriteUtil::CreateSquare( spTexture2 );

	//スプライトをシーングラフに登録
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spSprite1 );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spSprite2 );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CSpriteUtil::CreateStandardLine2() );

	//レンダーターゲットの作成
	graphic::CRenderTargetPtr spSamplerRenderTarget = graphic::CRenderTarget::Create();
	spSamplerRenderTarget->SetUpTexture( math::CBasicSize( _WIN_W , _WIN_H ) );
	spSamplerRenderTarget->SetClearColor( graphic::CColor_White );

	//シーンにレンダーターゲットを設置
	spPostEffectScreen->GetScene()->SetRenderTarget( spSamplerRenderTarget );

	//ポストエフェクトをシーンにかける
	spPostEffectScreen->GetScene()->SetUpPostEffect();

	//ポストエフェクトを積む
	spPostEffectScreen->GetScene()->GetPostEffectManager()->PushPostEffect( CPostEffectTextureMappingCreate_() );
	spPostEffectScreen->GetScene()->GetPostEffectManager()->PushPostEffect( CPostEffectGaussianFilterCreate_() );

//-----文字スクリーン関連

	//文字管理の初期化（DirectX11の場合、Windowが生成され、Deviceが生成された後でないと初期化できない
	CFontManagerInitialize_();

	//文字カメラ
	graphic::CameraPtr spFontCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spFontCamera->SetUp();                     //必須
	spFontCamera->SetOrtho( skyTrue );         //正射影
	spFontCamera->SetLeftTopCenter( skyTrue ); //左上原点

	//文字スクリーンのセットアップ
	screenData.Name                  = CHS( _T( "FontScreen" ) );        //スクリーン名
	screenData.Camera                = spFontCamera;                     //カメラ
	screenData.IsSetParentWindowSize = skyTrue;

	//スクリーンのセットアップ
	graphic::CScreenObjectPtr spFontScreen = spMainScreen->PushLayer( screenData );

	//文字を作成
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();
	spFont->SetUp();

	//文字をシーングラフに登録
	spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spFont );

//-----最終描画のためのスプライト

	//スプライトの作成
	graphic::CSpritePtr spDisplaySprite = graphic::CSpriteUtil::CreateSquare( spSamplerRenderTarget->GetColorBufferTexture() );

	//スプライトをメインのスクリーンに登録する
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spDisplaySprite );

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

		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "Shader Test 2D." ) );

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

#if DISP_TYPE == 1

using namespace sky::lib;

//Window初期位置、サイズ
static const dectype  _WIN_X = 0;
static const dectype  _WIN_Y = 0;
static const dectype  _WIN_W = 1024;
static const dectype  _WIN_H = 768;

//１秒あたりのフレーム数
static const u32 _FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;


TEST( Loader , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//フルスクリーンモードOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//------親スクリーン

	//カメラの作成
	graphic::CameraPtr spSpriteCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spSpriteCamera->SetUp();
	spSpriteCamera->SetOrtho( skyTrue );         //正射影

	//ルートスクリーンのセットアップ
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( _T( "MainScreen" ) );            //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = spSpriteCamera;                       //カメラ
	screenData.WindowRect.X()     = _WIN_X;                               //画面位置X
	screenData.WindowRect.Y()     = _WIN_Y;                               //画面位置Y
	screenData.WindowRect.W()     = _WIN_W;                               //画面幅
	screenData.WindowRect.H()     = _WIN_H;                               //画面高さ

	//スクリーンのセットアップ
	graphic::CScreenObjectPtr spMainScreen = CScreenObject_::Create( screenData );

//------子スクリーン

	//カメラの作成
	graphic::CameraPtr spCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spCamera->SetUp();
	spCamera->SetDefaultInput();

	//オフラインレンダリング用のスクリーンのセットアップ
	screenData.Name               = CHS( _T( "PostEffectScreen" ) );    //スクリーン名
	screenData.Camera             = spCamera;                           //カメラ
	screenData.WindowRect.W()     = _WIN_W;                               //画面幅
	screenData.WindowRect.H()     = _WIN_H;                               //画面高さ

	//ポストエフェクト用のスクリーンを作成
	graphic::CScreenObjectPtr spPostEffectScreen = spMainScreen->AddChild( screenData );
	spPostEffectScreen->SetReSizeOn( skyFalse );

	//レンダーターゲットの作成
	graphic::CRenderTargetPtr spSamplerRenderTarget = graphic::CRenderTarget::Create();
	spSamplerRenderTarget->SetUpTexture( math::CBasicSize( ( dectype )_WIN_W , ( dectype )_WIN_H ) );
	spSamplerRenderTarget->SetClearColor( graphic::CColor_White );

	//シーンにレンダーターゲットを設置
	spPostEffectScreen->GetScene()->SetRenderTarget( spSamplerRenderTarget );

	//ポストエフェクトをシーンにかける
	spPostEffectScreen->GetScene()->SetUpPostEffect();

	//ポストエフェクトを積む
//	spPostEffectScreen->GetScene()->GetPostEffectManager()->PushPostEffect( CPostEffectTextureMappingCreate_() );
//	spPostEffectScreen->GetScene()->GetPostEffectManager()->PushPostEffect( CPostEffectGaussianFilterCreate_() );

//------文字スクリーン

	//文字管理の初期化（DirectX11の場合、Windowが生成され、Deviceが生成された後でないと初期化できない
	CFontManagerInitialize_();

	//文字カメラ
	graphic::CameraPtr spFontCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spFontCamera->SetUp();                     //必須
	spFontCamera->SetOrtho( skyTrue );         //正射影
	spFontCamera->SetLeftTopCenter( skyTrue ); //左上原点

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

//-----------------------------------------------------------------------

	//ロードパス調整
	graphic::CGraphicFilePathManager::SetColladaFilePathRoot( CHS( "data/lib/shader/testdata/" ) );
	graphic::CGraphicFilePathManager::SetTextureFilePathRoot( CHS( "data/lib/shader/testdata/" ) );

	//モデルの読み込み
#if 1
	graphic::ColladaLoader::LoadFile( CHS( _T( "shaderTestModel2.dae" ) ) );
	graphic::CModelPtr spModel_1 = graphic::CDataBase::GetModel( CHS( "Monkey" ) );
#elif 0 
	graphic::ColladaLoader::LoadFile( CHS( _T( "shaderTestModel3.dae" ) ) );
	graphic::CModelPtr spModel_1 = graphic::CDataBase::GetModel( CHS( "Cube" ) );
#elif 0
	graphic::ColladaLoader::LoadFile( CHS( _T( "shaderTestModel4.dae" ) ) );
	graphic::CModelPtr spModel_1 = graphic::CDataBase::GetModel( CHS( "Sphere" ) );
#endif
	spModel_1->ActivateTraverse();
	CDataBase_::Clear();

//	spModel_1->ClearAndAttachShader( CBlinnPhongShaderObjectCreate_() );

	graphic::CModelPtr spConeModel = graphic::CModelUtil::CreateTestCone();
	spConeModel->SetPosition_X( 3 );
	graphic::CModelPtr spCubeModel = graphic::CModelUtil::CreateTestCube();
	spCubeModel->SetPosition_X( -3 );

	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spConeModel );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spCubeModel );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spModel_1 );
//	spModel_1->SetDebugDisplayNormalOn( skyTrue , 1.0f );
	spModel_1->GetMeshContena()->GetMaterial()->Reset();
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateStandardLine3() );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateGrid_XZ( 10 ) );

//-----最終描画のためのスプライト

	//スプライトの作成
	graphic::CSpritePtr spDisplaySprite = graphic::CSpriteUtil::CreateSquare( spSamplerRenderTarget->GetColorBufferTexture() );

	//スプライトをメインのスクリーンに登録する
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spDisplaySprite );

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
	spCamera->SetDistance( 40.0f );
	math::CBasicAngleVector3 cameraAngle( 0 , 0 , 0 , 0 );
	GAME_LOOP_START
	{
		//フレーム開始
		fpsCtrl.StartFrame();

		//入力管理更新
		Ckeyboard_->Update( fpsCtrl );

//		degAngle++;
//		cameraAngle.Y().FromDeg( degAngle );
//		spCamera->SetAngle( cameraAngle );

		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "Shader Test 3D." ) );
		spFont->PrintfSentence( 16 , 32 , graphic::CColor_Tomato , 1.0f , _T( "CameraPos( %f %f %f )" ) , spCamera->GetData().m_Eye.X, spCamera->GetData().m_Eye.Y , spCamera->GetData().m_Eye.Z );
		spFont->PrintfSentence( 16 , 48 , graphic::CColor_Tomato , 1.0f , _T( "CameraAngle( %f %f %f )" ) , spCamera->GetData().m_Angle.X().ToDeg() , spCamera->GetData().m_Angle.Y().ToDeg() , spCamera->GetData().m_Angle.Z().ToDeg() );

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