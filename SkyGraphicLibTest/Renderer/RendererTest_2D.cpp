#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_RENDERER

#if ON_

#include "SkyBaseLib/GameLoop.h"
#include "SkyBaseLib/Input.h"
#include "SkyBaseLib/Fps.h"

#include "SkyGraphicLib/Camera.h"
#include "SkyGraphicLib/Model.h"
#include "SkyGraphicLib/Sprite.h"
#include "SkyGraphicLib/Scene.h"
#include "SkyGraphicLib/Renderer.h"
#include "SkyMathLib/Entity.h"

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

static const skyString *SCREEN_SETUP_NAME                   = _T( "Test" ); 

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
static const u32				_FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//カメラ
static graphic::Camera2DPtr  g_spCamera;

//テスト用モデル
static graphic::CSpritePtr     g_spSprite;

//スクリーン
static graphic::CScreenObjectPtr    g_spScreen;

TEST( Render , 1 ) 
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
	g_spCamera->vSetUp();
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
	screenData.RenderState        = CRenderStateCreate_();                 //レンダーステート

	//スクリーンのセットアップ
	g_spScreen = CScreenObject_::Create( screenData );

	//文字を出力するので文字管理を初期化
	CFontManagerInitialize_();

	//テクスチャ読み込み
	graphic::CTexturePtr spTexture0 = CTextureCreateName_( CHSC( TEST1_TEXTURE_PATH ) );
	spTexture0->Load(); //ロード

	//テクスチャ付きの矩形を作成
	g_spSprite = graphic::CSpriteUtil::CreateSquare( spTexture0 , graphic::RENDERING_2D_PRIO_FAST , math::CBasicVector2( 2 , 2 ) , graphic::CColor_White );
	g_spSprite->GetMesh()->CreateGpuMemory();
	g_spSprite->GetMesh()->WirteGpuMemory();

	//スプライトをシーングラフに登録
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_spSprite );

	graphic::CSpritePtr spLineSprite = graphic::CSpriteUtil::CreateSquareLine( 32 , 32 );
	spLineSprite->SetLockParentRotAndScale( skyTrue );

	//シェーダーを取り替える
	graphic::CMaterialDiffuseColorShaderObjectPtr effectPtr = graphic::CMaterialDiffuseColorShaderObject::Create();
	spLineSprite->LeafList().DeleteAll();
	spLineSprite->AttachLeaf( 0 , effectPtr );

	//法線を作る(上記シェーダーは使用するので）
	spLineSprite->GetMesh()->GetMaterial()->CreateNormalStream( spLineSprite->GetMesh()->GetVertexBuffer()->VertexNum() );

	//色を設定する
	spLineSprite->GetMesh()->SetDiffuse( graphic::CColor_Gold ); 

	//GPUメモリに書きこむ
	graphic::CRenderCommand::CreateMeshGpuMemory( spLineSprite->GetMesh() );
	graphic::CRenderCommand::WirteMeshGpuMemory( spLineSprite->GetMesh() );

	//CPUメモリは消す
	graphic::CRenderCommand::DeleteMeshCpuMemory( spLineSprite->GetMesh() );

	//シーングラフの先頭に置く
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spLineSprite );

	GET_BONE( spLineSprite )->SetScale_X( 2.0f );
	GET_BONE( spLineSprite )->SetScale_Y( 2.0f );
	GET_BONE( spLineSprite )->MulRotation_Z( 0.8f );
	GET_BONE( spLineSprite )->SetPosition_X( 256 );
	GET_BONE( spLineSprite )->SetPosition_Y( 256 );

	//文字を作成
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();
	spFont->SetUp();

	//文字をシーングラフに登録
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spFont );

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
		g_spSprite->GetEntity2D()->m_Entity2D.SetRot( math::util::KPI * ( ( u32 )CTimeUtil_::GetNowTime() % 3000 ) / 1500.0f );
		g_spSprite->GetEntity2D()->m_Entity2D.SetPos_X( _WIN_W / 2 );
		g_spSprite->GetEntity2D()->m_Entity2D.SetPos_Y(_WIN_H / 2 );
	
		//カメラ移動、モデル移動入力受付
		math::CEntityUtil::DefaultInput_Entity2D( g_spCamera->GetEntity2D() , KEY_C );
		math::CEntityUtil::DefaultInput_Entity2D( g_spSprite->GetEntity2D() , KEY_X );

		//文字を出す
		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "%f ms" ) , fpsCtrl.GetPassageTime() );
//		spFont->PrintfSentence( 16 , 32 , graphic::CColor_White , 1.0f , _T( "abcdefghijklmnopqrstuvwxyz" ) );

		//描画中だったらコマンド発行する
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//コマンドバッファの作成
			CRenderCommandManager_::CreateBuffer();

			//描画コマンド発行
			g_spScreen->Rendering( CTimeUtil_::GetNowTime() );

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
	g_spScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}


#endif

#endif