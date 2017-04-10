#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_SCREEN


using namespace sky::lib;

#include "SkyGraphicLib/Font.h"
#include "SkyGraphicLib/Screen.h"
#include "SkyGraphicLib/Renderer.h"

using namespace sky::lib;

#if ON_

static const skyString *RENDER_TARGET_SAVE_PATH = _T( "data/out/CRenderTarget.bmp" );
static const skyString *DEPTHSTENCIL_SAVE_PATH  = _T( "data/out/DepthStencil.dds" );

static const skyString *TEST1_TEX_SURFACE_NAME = _T( "texture_surface" );

static const skyString *TEST1_OBJ_ROOT = _T( "data/obj/" );
static const skyString *TEST1_MTL_ROOT = _T( "data/mtl/" );
static const skyString *TEST1_TEX_ROOT = _T( "data/texture/" );

static const skyString *TEST1_OBJ_FILE_NAME_1 = _T( "cube.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_2 = _T( "kuma.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_3 = _T( "sphere.obj" );

//テスト用テクスチャファイルパス
static const skyString *TEST1_TEXTURE_PATH = _T( "data/test/texture/TestBmp8bit.bmp" );

static const skyString *SCREEN_SETUP_NAME_3D                   = _T( "Test_Screen3D" ); 
static const skyString *SCREEN_SETUP_NAME_2D                   = _T( "Test_Screen2D" ); 
static const skyString *SCREEN_SETUP_NAME_3D_L                 = _T( "Test_Screen3DL" ); 
static const skyString *SCREEN_SETUP_NAME_3D_R                 = _T( "Test_Screen3DR" ); 

//カメラ情報
static const math::CBasicVector3 _CAMERA3D_DEFAULT_UP_VECTOR	= math::CBasicVector3( 0 , 1 , 0 );
static const dectype			 _CAMERA3D_DEFAULT_NEAR		= 1;
static const dectype			 _CAMERA3D_DEFAULT_FAR		= 20;
static const dectype			 _CAMERA3D_DEFAULT_FOV		= math::CMathUtil::DegToRad( 30.0f );
static const math::CBasicVector3 _CAMERA3D_DEFAULT_EYE		= math::CBasicVector3( 0.0f, 5.0f,-5.0f );
static const math::CBasicVector3 _CAMERA3D_DEFAULT_TAG		= math::CBasicVector3( 0.0f, 0.0f, 0.0f );

//カメラ初期情報
static const math::CBasicVector3 _CAMERA2D_DEFAULT_UP_VECTOR	= math::CBasicVector3( 0 , 1 , 0 );
static const dectype			 _CAMERA2D_DEFAULT_NEAR		= graphic::RENDERING_2D_PRIO_FAST;
static const dectype			 _CAMERA2D_DEFAULT_FAR		= graphic::RENDERING_2D_PRIO_MAX;

//Window初期位置、サイズ
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//１秒あたりのフレーム数
static const u32				_FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//カメラ
static graphic::Camera3DPtr  g_spCamera3D;
static graphic::Camera2DPtr  g_spCamera2D;

//スクリーン
static graphic::CScreenObjectPtr    g_spScreen3D;

//テスト用モデル
static graphic::CModelPtr     g_spModel;

TEST( Screen , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//終了感知コールバックを登録する
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//フルスクリーンモードOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

	//カメラの作成／パラメーター設定／初期化
	g_spCamera3D = Camera3DFixationCreate_();
	g_spCamera3D->SetUpVector( _CAMERA3D_DEFAULT_UP_VECTOR );
	g_spCamera3D->SetNear( _CAMERA3D_DEFAULT_NEAR );
	g_spCamera3D->SetFar( _CAMERA3D_DEFAULT_FAR );
	g_spCamera3D->SetFov( _CAMERA3D_DEFAULT_FOV );
	g_spCamera3D->SetEye( _CAMERA3D_DEFAULT_EYE );
	g_spCamera3D->SetTag( _CAMERA3D_DEFAULT_TAG ); 

	//カメラの作成／パラメーター設定／初期化
	g_spCamera2D = Camera2DFixationCreate_();
	g_spCamera2D->SetUpVector( _CAMERA2D_DEFAULT_UP_VECTOR );
	g_spCamera2D->SetNear( _CAMERA2D_DEFAULT_NEAR );
	g_spCamera2D->SetFar( _CAMERA2D_DEFAULT_FAR );
	g_spCamera2D->SetLeftTopCenter( skyTrue );

//--------3DScreenのセットアップ--------▽
	
	//ルートスクリーンのセットアップ
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( SCREEN_SETUP_NAME_3D );          //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = g_spCamera3D;                         //カメラ
	screenData.WindowRect.X()     = _WIN_X;                               //画面位置X
	screenData.WindowRect.Y()     = _WIN_Y;                               //画面位置Y
	screenData.WindowRect.W()     = _WIN_W;                               //画面幅
	screenData.WindowRect.H()     = _WIN_H;                               //画面高さ

	//スクリーンのセットアップ
	g_spScreen3D = CScreenObject_::Create( screenData );

	//ファイルからモデルデータを構築
	CWavefrontOBJLoader_::SetObjFilePathRoot( CHSC( TEST1_OBJ_ROOT ) );
	CWavefrontOBJLoader_::SetMtlFilePathRoot( CHSC( TEST1_MTL_ROOT ) );
	CWavefrontOBJLoader_::SetTextureFilePathRoot( CHSC( TEST1_TEX_ROOT ) );
	g_spModel = graphic::CModelUtil::CreateFromWavefrontOBJ( CHSC( TEST1_OBJ_FILE_NAME_1 ) );
	g_spModel->GetShaderList().DeleteAll();
	g_spModel->AttachLeaf( 0 , graphic::CTextureMappingShaderObject<>::Create() );

	//モデルをシーングラフに登録
	g_spScreen3D->GetScene()->m_spSceneGraph = g_spModel;

//-----TextureScreenのセットアップ------▽

	//ルートスクリーンのセットアップ
/*	graphic::CRenderTargetPtr spCRenderTarget = CRenderTargetCreate_( CHSC( TEST1_TEX_SURFACE_NAME ) );
	spCRenderTarget->SetUpTexture( _WIN_W , _WIN_H );

	screenData.Name               = CHS( SCREEN_SETUP_NAME_2D );          //スクリーン名
	screenData.ClearColor         = graphic::CColor_Green;                //スクリーンクリアカラー
	screenData.Camera             = g_spCamera2D;                         //カメラ
	screenData.RenderState        = IRenderStatCreate_();                 //レンダーステート

	g_spScreen3D->AddCRenderTargetScreen( screenData , spCRenderTarget );
*/
//--------2DScreenのセットアップ--------▽

	//レイヤーのセットアップデータ
	screenData.Name               = CHS( SCREEN_SETUP_NAME_2D );          //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = g_spCamera2D;                           //カメラ

	//スクリーンのセットアップ
	g_spScreen3D->PushLayer( screenData );
    graphic::CScreenObjectPtr spScreen2D = CScreenObject_::Get( CHS( SCREEN_SETUP_NAME_2D ) );

	//シーングラフのルートを作成
	CScreenObject_::Get( CHS( SCREEN_SETUP_NAME_2D ) )->GetScene()->m_spSceneGraph = graphic::CSceneGraphNodePtr( NEW graphic::CSceneGraphNode() );

	//文字管理の初期化
	CFontManagerInitialize_();

	//文字を作成
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();

	//文字をシーングラフに登録
	spScreen2D->GetScene()->m_spSceneGraph->AttachChild( spFont );

//--------3DScreenのセットアップ（上記レンダリング結果を利用して描画）--------▽

//--------描画開始--------▽

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

		//Ｙ軸方向に分割する
		if ( Ckeyboard_->IsKeyPress( KEY_Y ) )
		{
			g_spScreen3D->SplitYCenter( CHS( SCREEN_SETUP_NAME_3D_L ) , CHS( SCREEN_SETUP_NAME_3D_R ) );
			g_spScreen3D->GetScene()->m_spSceneGraph.Delete();
		}

		//Ｘ軸方向に分割する
		if ( Ckeyboard_->IsKeyPress( KEY_X ) )
		{
			g_spScreen3D->SplitXCenter( CHS( SCREEN_SETUP_NAME_3D_L ) , CHS( SCREEN_SETUP_NAME_3D_R ) );
			g_spScreen3D->GetScene()->m_spSceneGraph.Delete();
		}

		//終了判定
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) || bExit == 1 )
		{
			GAME_LOOP_EXIT();
		}

		//レンダリングターゲットをテクスチャとして保存
		if ( Ckeyboard_->IsKeyPress( KEY_P ) )
		{
			CRenderCommandManager_::AddPreRenderingExecEvent( 0 , 
				FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( 
				&graphic::CTexture<>::Save , 
				g_spScreen3D->m_spRenderingTarget->GetColorBufferTexture().GetPtrNoConst() , 
				graphic::texture::eTextureFileFormat_jpg , 
				CHSC( RENDER_TARGET_SAVE_PATH ) ) ) );
		}

		//深度ステンシルバッファをテクスチャとして保存
		if ( Ckeyboard_->IsKeyPress( KEY_D ) )
		{
			CRenderCommandManager_::AddPreRenderingExecEvent( 0 , 
				FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( 
				&graphic::CTexture<>::Save , 
				g_spScreen3D->m_spDepthStencilBuffer->GetDepthStencilTexture().GetPtrNoConst() , 
				graphic::texture::eTextureFileFormat_dds , 
				CHSC( DEPTHSTENCIL_SAVE_PATH ) ) ) );
		}

		//モデルの回転
		g_spModel->GetEntity3D()->Rot.Y = CMathUtil_::RadToDeg( math::util::KPI * ( ( u32 )CTimeUtil_::GetNowTime() % 3000 ) / 1500.0f );
		g_spModel->GetEntity3D()->Rot.X = CMathUtil_::RadToDeg( math::util::KPI * ( ( u32 )CTimeUtil_::GetNowTime() % 1500 ) / 750.0f );

		//カメラの更新
		g_spCamera2D->vUpdate( fpsCtrl.GetPassageTime() );

		//カメラ移動、モデル移動入力受付
		math::CEntityUtil::DefaultInput_Entity2D( g_spCamera2D->GetEntity2D() , KEY_C );

		//カメラ移動、モデル移動を反映
		graphic::CameraUtil::SetEntity2D( g_spCamera2D );
		graphic::CModelUtil::SetEntity3D( g_spModel );

		//文字を出す
		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "%f ms " ) , fpsCtrl.GetPassageTime() );
		spFont->PrintfSentence( 16 , 32 , graphic::CColor_White , 1.0f , _T( "abcdefghijklmnopqrstuvwxyz\nあいうえおかきくけこさしすせそ\n1234567890" ) );

		//描画中だったらコマンド発行する
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//コマンドバッファの作成
			CRenderCommandManager_::CreateBuffer();

			//描画コマンド発行
			g_spScreen3D->Rendering();

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

	g_spCamera2D.Delete();
	g_spCamera3D.Delete();
	g_spModel.Delete();
	spScreen2D.Delete();
	g_spScreen3D.Delete();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

TEST( Screen , 2 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	dectype value = graphic::CScreenUtil::GetCinemaScopeHeight( math::CBasicSize( 800.0f , 600.0f ) );
	dectype valid = graphic::CScreenUtil::GetValidScreenHeightRateInVirtualScreen( math::CBasicSize( 1280.0f , 720.0f ) );
	sky::lib::math::CBasicSize pixel = graphic::CScreenUtil::GetViewPortSpacedPixelSize( math::CBasicSize( 1280.0f , 720.0f ) );
	sky::lib::math::CBasicVector2 vec2 = graphic::CScreenUtil::PixelToPoint( math::CBasicVector2( 128 , 128 ) , math::CBasicSize( 1280.0f , 720.0f ) );
	pixel.W = pixel.W * 1280.0f;
	pixel.H = pixel.H * 720.0f;

	SKY_LIB_GRAPHIC_TERM();
}

#endif