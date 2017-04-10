#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_SCENEGRAPH

#if OFF_

using namespace sky::lib;

#include "SkyGraphicLib/Loader.h"

static const skyString *TEST1_OBJ_ROOT = _T( "data/obj/" );
static const skyString *TEST1_MTL_ROOT = _T( "data/mtl/" );
static const skyString *TEST1_TEX_ROOT = _T( "data/texture/" );

static const skyString *TEST1_OBJ_FILE_NAME_1 = _T( "cube.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_2 = _T( "kuma.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_3 = _T( "sphere.obj" );

static const skyString *SCREEN_SETUP_NAME                   = _T( "Test" ); 

//カメラ情報
static const math::CBasicVector3 _CAMERA_DEFAULT_UP_VECTOR	= math::CBasicVector3( 0 , 1 , 0 );
static const dectype			 _CAMERA_DEFAULT_NEAR		= 1;
static const dectype			 _CAMERA_DEFAULT_FAR		= 20;
static const dectype			 _CAMERA_DEFAULT_FOV		= math::CMathUtil::DegToRad( 30.0f );
static const math::CBasicVector3 _CAMERA_DEFAULT_EYE		= math::CBasicVector3( 0.0f, 5.0f,-5.0f );
static const math::CBasicVector3 _CAMERA_DEFAULT_TAG		= math::CBasicVector3( 0.0f, 0.0f, 0.0f );

//Window初期位置、サイズ
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//１秒あたりのフレーム数
static const u32				_FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//カメラ
static graphic::Camera3DPtr  g_spCamera;

//テスト用モデル
static graphic::CModelPtr     g_spModel_1;
static graphic::CModelPtr     g_spModel_2;

//スクリーン
static graphic::CScreenObjectPtr    g_spScreen;

TEST( scenegraph , 4 ) 
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

	//カメラの作成／パラメーター設定／初期化
	g_spCamera = Camera3DFixationCreate_();
	g_spCamera->SetUpVector( _CAMERA_DEFAULT_UP_VECTOR );
	g_spCamera->SetNear( _CAMERA_DEFAULT_NEAR );
	g_spCamera->SetFar( _CAMERA_DEFAULT_FAR );
	g_spCamera->SetFov( _CAMERA_DEFAULT_FOV );
	g_spCamera->SetEye( _CAMERA_DEFAULT_EYE );
	g_spCamera->SetTag( _CAMERA_DEFAULT_TAG ); 

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

	//ファイルからモデルデータを構築
	CWavefrontOBJLoader_::SetObjFilePathRoot( CHSC( TEST1_OBJ_ROOT ) );
	CWavefrontOBJLoader_::SetMtlFilePathRoot( CHSC( TEST1_MTL_ROOT ) );
	CWavefrontOBJLoader_::SetTextureFilePathRoot( CHSC( TEST1_TEX_ROOT ) );
	g_spModel_1 = graphic::CModelUtil::CreateFromWavefrontOBJ( CHSC( TEST1_OBJ_FILE_NAME_1 ) );
	g_spModel_2 = graphic::CModelUtil::CreateFromWavefrontOBJ( CHSC( TEST1_OBJ_FILE_NAME_1 ) );

	//ルートを空のシーングラフで作成
	g_spScreen->GetScene()->m_spSceneGraph = CSceneGraphNodeCreate_();

	//ステンシルテスト用レンダーステートノード作成
	graphic::CRenderStatePtr spStencilTestRenderState = IRenderStatCreate_();
	spStencilTestRenderState->EnableStencilTest();

	//ステンシルマスク用レンダーステートノード作成
	graphic::CRenderStatePtr spStencilMaskRenderState = IRenderStatCreate_();
	spStencilMaskRenderState->EnableStencilMask();

	//深度ステンシルバッファノード作成
	graphic::CDepthStencilBufferPtr spStencilTestBuffer = CDepthStencilBufferCreate_();
	spStencilTestBuffer->SetUp( _WIN_W , _WIN_H );

	//ステンシルマスク書き込み用
	graphic::CDepthStencilBufferPtr spStencilMaskBuffer = spStencilTestBuffer->CloneNode();

	//シーングラフを構築
	g_spScreen->GetScene()->m_spSceneGraph->AttachChild( spStencilTestRenderState );
	g_spScreen->GetScene()->m_spSceneGraph->AttachChild( spStencilMaskRenderState );
	spStencilTestRenderState->AttachChild( spStencilTestBuffer );
	spStencilMaskRenderState->AttachChild( spStencilMaskBuffer );
	spStencilTestBuffer->AttachChild( g_spModel_1 );
	spStencilMaskRenderState->AttachChild( g_spModel_2 );

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

		//カメラの更新
		g_spCamera->vUpdate( fpsCtrl.GetPassageTime() );

		//モデルの回転
		g_spModel_1->GetEntity3D().Rot.Y = CMathUtil_::RadToDeg( math::util::KPI * ( ( u32 )CTimeUtil_::GetNowTime() % 3000 ) / 1500.0f );
		g_spModel_1->GetEntity3D().Rot.X = CMathUtil_::RadToDeg( math::util::KPI * ( ( u32 )CTimeUtil_::GetNowTime() % 1500 ) / 750.0f );
		g_spModel_2->GetEntity3D().Rot.Y = CMathUtil_::RadToDeg( math::util::KPI * ( ( u32 )CTimeUtil_::GetNowTime() % 3000 ) / 1500.0f );
		g_spModel_2->GetEntity3D().Rot.X = CMathUtil_::RadToDeg( math::util::KPI * ( ( u32 )CTimeUtil_::GetNowTime() % 1500 ) / 750.0f );
	
		//カメラ移動、モデル移動入力受付
		math::CEntityUtil::DefaultInput_Entity3D( g_spCamera->GetEntity3D() , KEY_C );
		math::CEntityUtil::DefaultInput_Entity3D( g_spModel_1->GetEntity3D() , KEY_X );
		math::CEntityUtil::DefaultInput_Entity3D( g_spModel_2->GetEntity3D() , KEY_X );

		//カメラ移動、モデル移動を反映
		graphic::CameraUtil::SetEntity3D( g_spCamera );
		graphic::CModelUtil::SetEntity3D( g_spModel_1 );
		graphic::CModelUtil::SetEntity3D( g_spModel_2 );

		//リセット
		if ( Ckeyboard_->IsKeyPress( KEY_HOME ) )
		{
			g_spModel_1->GetProperty().ResetLocal();
			g_spModel_1->GetProperty().ResetParent();
			g_spModel_1->GetProperty().ResetWorld();

			g_spModel_2->GetProperty().ResetLocal();
			g_spModel_2->GetProperty().ResetParent();
			g_spModel_2->GetProperty().ResetWorld();
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

	g_spCamera.Delete();
	g_spModel_1.Delete();
	g_spModel_2.Delete();
	g_spScreen.Delete();
	spStencilTestRenderState.Delete();
	spStencilMaskRenderState.Delete();
	spStencilTestBuffer.Delete();
	spStencilMaskBuffer.Delete();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif