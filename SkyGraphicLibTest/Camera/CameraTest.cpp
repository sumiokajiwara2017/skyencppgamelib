#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_CAMERA

using namespace sky::lib;

//Window初期位置、サイズ
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//１秒あたりのフレーム数
static const u32 _FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

TEST( Camera , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//フルスクリーンモードOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

	//カメラの作成
	graphic::CameraPtr spCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spCamera->SetUp();

	//サブカメラ
	graphic::CameraPtr spCamera_Left =  CameraCreate_( graphic::eCameraKind_Satellite );
	spCamera_Left->SetUp();
	graphic::CameraUtil::SetDefaultCameraPos_Left( spCamera_Left );

	graphic::CameraPtr spCamera_Right =  CameraCreate_( graphic::eCameraKind_Satellite );
	spCamera_Right->SetUp();
	graphic::CameraUtil::SetDefaultCameraPos_Right( spCamera_Right );

	graphic::CameraPtr spCamera_Top =  CameraCreate_( graphic::eCameraKind_Satellite );
	spCamera_Top->SetUp();
	graphic::CameraUtil::SetDefaultCameraPos_Top( spCamera_Top );

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

	//サブカメラの登録
	spMainScreen->GetScene()->GetCameraManager()->ResisterCamera( spCamera_Left );
	spMainScreen->GetScene()->GetCameraManager()->ResisterCamera( spCamera_Right );
	spMainScreen->GetScene()->GetCameraManager()->ResisterCamera( spCamera_Top );
	spMainScreen->GetScene()->GetCameraManager()->SetDefaultInputOn( skyTrue );

	//モデルの作成
	graphic::CModelPtr spModel = graphic::CModelUtil::CreateTestCube();

	//シェーダーの交換
//	spModel->ClearAndAttachShader( CPointLightShaderObjectCreate_() );

	//モデルをシーングラフに登録
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spModel );
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateStandardLine3() );

	//文字管理の初期化（DirectX11の場合、Windowが生成され、Deviceが生成された後でないと初期化できない
	CFontManagerInitialize_();

	//文字カメラ
	graphic::CameraPtr spFontCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spFontCamera->SetUp();                         //必須
	spFontCamera->SetOrtho( skyTrue );             //正射影
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

	//FPSコントローラー作成
	sky::lib::fps::CFpsController fpsCtrl;

	//描画情報キューの作成
	CRenderCommandManager_::CreateQueue( RENDER_QUEUE_SIZE );

	//描画用スレッドの作成
	CRenderCommandManager_::StartThread();

	fpsCtrl.SetFrameRate( _FRAME_NUM );

	SKY_PRINTF( _T( "===================== < RenderTest Start > ===================== \n" ) );

	math::CBasicAngleVector3 cameraAngle( 2 , 0 , 0 , 30 );

	//ゲームループ
	s32 degAngle = 0;
	spCamera->SetDistance( 80.0f );
	GAME_LOOP_START
	{
		//フレーム開始
		fpsCtrl.StartFrame();

		//入力管理更新
		Ckeyboard_->Update( fpsCtrl );

		degAngle++;
		cameraAngle.Y().FromDeg( degAngle );
		spCamera->SetAngle( cameraAngle );

		if ( Ckeyboard_->IsKeyRepeat( KEY_R ) )
		{
			math::CBasicAngle angle;
			angle.FromDeg( 5 );
//			spModel->GetEntity3D()->m_Entity3D.AddRot_Z( angle );
//			spModel->GetEntity3D()->m_Entity3D.AddPos_Z( 1 );
			spModel->GetEntity3D()->m_Entity3D.AddSc_Z( 1 );
		}

		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "Camera Test." ) );

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