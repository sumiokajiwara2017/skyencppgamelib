#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_ANIMATION

using namespace sky::lib;

#if OFF_

static const skyString *SCREEN_SETUP_NAME       = _T( "Test" ); 
static const skyString *FONT_SCREEN_SETUP_NAME  = _T( "font" ); 

//Window初期位置、サイズ
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//１秒あたりのフレーム数
static const u32 _FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//カメラ
static graphic::Camera3DFixationPtr  g_spCamera;
static graphic::Camera2DFixationPtr  g_spFontCamera;

//テスト用モデル
static graphic::CModelPtr     g_TestCone;
static graphic::CModelPtr     g_TestPlane;

//スクリーン
static graphic::CScreenObjectPtr    g_spScreen;
static graphic::CScreenObjectPtr    g_spFontScreen;

math::CBasicVector3 m_Transrate( 0 , 1 , 0 );

static void ColValueEventDelegate_Vector3( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args )
{
	SKY_UNUSED_ARG( spSender );
	f32 *time                  = ( f32 * )args.Getu32( 0 );
	math::CBasicVector3 *value = ( math::CBasicVector3 * )args.Getu32( 1 );
	f32 *weight                = ( f32 * )args.Getu32( 2 );

	m_Transrate = *value;
}

TEST( Animation , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	graphic::CBonePtr spBone = CBoneCreate_();

	//終了感知_登録する
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//フルスクリーンモードOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//--------3DScreenのセットアップ--------▽

	g_spCamera = Camera3DFixationCreate_();
	g_spCamera->vSetUp();
	g_spCamera->SetDefaultInputOn( skyTrue );

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

	//モデル作成
	g_TestCone = graphic::CModelUtil::CreateTestCone();
	g_TestPlane = graphic::CModelUtil::CreateTestPlane();

	GET_BONE( g_TestCone )->SetPosition_XYZ( m_Transrate );

	//モデルをシーングラフに登録
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestCone );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestPlane );

	//ファンクションカーブを生成
	kfm::CFCurve< math::CBasicVector3 > fcVector3( m_Transrate );
	kfm::CFCurve< math::CBasicVector3 > fcVector3_next( m_Transrate );
	kfm::CFCurve< math::CBasicVector3 > fcVector3_next2( m_Transrate );

	//値補間スケジュールを立てる
	fcVector3.AddKey( 1.0f , math::CBasicVector3( 0 , 20 , 0 ) , eInterpolateType_Deceleration ); 
	fcVector3.AddKey( 1.0f , m_Transrate                       , eInterpolateType_Deceleration ); 
	fcVector3.AddKey( 1.0f , math::CBasicVector3( 20 , 1 , 0 ) , eInterpolateType_Deceleration ); 
	fcVector3.AddKey( 1.0f , m_Transrate                       , eInterpolateType_Deceleration );

	fcVector3_next.AddKey( 1.0f , math::CBasicVector3( 0 , 20 , 0 ) , eInterpolateType_Deceleration ); 
	fcVector3_next.AddKey( 1.0f , m_Transrate                       , eInterpolateType_Deceleration ); 

	fcVector3_next2.AddKey( 1.0f , math::CBasicVector3( -20 , 1 , 0 ) , eInterpolateType_Deceleration ); 

	kfm::CFCurvePlayer< math::CBasicVector3 > fcurvePlayer_Vector3;
	fcurvePlayer_Vector3.GetColValueEventHndl() += delegate::IDelegateObjectPtr( NEW delegate::CEventDelegateAdaptorFunc( ColValueEventDelegate_Vector3 ) ); 
	fcurvePlayer_Vector3.PlayFCurveRequest( &fcVector3 , 0 , 0 , 1.0f , 0 , kfm::ePlayEndType_End );
//	fcurvePlayer_Vector3.PlayFCurveRequest( &fcVector3 , 0 , 0 , 1.0f , 0 , kfm::ePlayEndType_Repetition );
//	fcurvePlayer_Vector3.PlayFCurveRequest( &fcVector3 , 0 , 0 , 1.0f , 0 , kfm::ePlayEndType_Back );

	fcurvePlayer_Vector3.PlayFCurveRequest( &fcVector3_next );
	fcurvePlayer_Vector3.PlayFCurveRequest( &fcVector3_next2 );

//------------------------------------------------------------

	//文字カメラ
	g_spFontCamera = Camera2DFixationCreate_();
	g_spFontCamera->vSetUp();
	g_spFontCamera->SetLeftTopCenter( skyTrue );

	//文字スクリーンのセットアップ
	screenData.Name               = CHS( FONT_SCREEN_SETUP_NAME );        //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = g_spFontCamera;                       //カメラ

	//スクリーンのセットアップ
	g_spFontScreen = g_spScreen->PushLayer( screenData );

	//文字を出力するので文字管理を初期化
	CFontManagerInitialize_();

	//基準線の作成
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateStandardLine3() );

	//文字を作成
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();
	spFont->SetUp();

	//文字をシーングラフに登録
	g_spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spFont );

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
		Ckeyboard_->Update( fpsCtrl );

		spFont->PrintfSentence( 32 , 32 , graphic::CColor_Tomato , 1.0f , _T( "DebugPrint." ) );

		//再生
		fcurvePlayer_Vector3.Play( fpsCtrl.GetPassageTime_Second() );

		//値を反映
		GET_BONE( g_TestCone )->SetPosition_XYZ( m_Transrate );

		//描画中だったらコマンド発行する
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//コマンドバッファの作成
			CRenderCommandManager_::CreateBuffer();

			//描画コマンド発行
			g_spScreen->Rendering( fpsCtrl );

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

	g_spCamera.Delete();
	spFont.Delete();
	g_spScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#if OFF_

static const skyString *SCREEN_SETUP_NAME       = _T( "Test" ); 
static const skyString *FONT_SCREEN_SETUP_NAME  = _T( "font" ); 

//Window初期位置、サイズ
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//１秒あたりのフレーム数
static const u32				_FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//カメラ
static graphic::Camera3DFixationPtr  g_spCamera;
static graphic::Camera2DFixationPtr  g_spFontCamera;

//テスト用モデル
static graphic::CModelPtr     g_TestCone;
static graphic::CModelPtr     g_TestPlane;

//スクリーン
static graphic::CScreenObjectPtr    g_spScreen;
static graphic::CScreenObjectPtr    g_spFontScreen;

TEST( Animation , 2 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	graphic::CBonePtr spBone = CBoneCreate_();

	//終了感知_登録する
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//フルスクリーンモードOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//--------3DScreenのセットアップ--------▽

	g_spCamera = Camera3DFixationCreate_();
	g_spCamera->vSetUp();
	g_spCamera->SetDefaultInputOn( skyTrue );

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

	//モデル作成
	g_TestCone = graphic::CModelUtil::CreateTestCone();
	g_TestPlane = graphic::CModelUtil::CreateTestPlane();

	//モデルをシーングラフに登録
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestCone );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestPlane );

	//モデルにアニメーションをつける
	graphic::CKeyFrameEntity3DAnimationPtr spAnim = CKeyFrameEntity3DAnimationCreate_();
	spAnim->GetFCurve().AddKey( 2 , math::CEntity3D(
		math::CBasicVector3( 5 , 5 , 5 ) , //位置
		math::CBasicVector3( 360 , 360 , 360 ) , //回転
		math::CBasicVector3( 2 , 2 , 2 ) ) , eInterpolateType_Linear );
	spAnim->Play( 0 , 1.0f , 0.0f , kfm::ePlayEndType_Back );
	g_TestCone->SetAlpha( skyTrue );
	g_TestCone->GetMesh()->GetMaterial()->SetAlpha( 0.5 );
	GET_BONE( g_TestCone )->AttachKeyFrameEntity3DAnimation( spAnim );

//------------------------------------------------------------

	//文字カメラ
	g_spFontCamera = Camera2DFixationCreate_();
	g_spFontCamera->vSetUp();
	g_spFontCamera->SetLeftTopCenter( skyTrue );

	//文字スクリーンのセットアップ
	screenData.Name               = CHS( FONT_SCREEN_SETUP_NAME );        //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = g_spFontCamera;                       //カメラ

	//スクリーンのセットアップ
	g_spFontScreen = g_spScreen->PushLayer( screenData );

	//文字を出力するので文字管理を初期化
	CFontManagerInitialize_();

	//基準線の作成
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateStandardLine3() );

	//文字を作成
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();
	spFont->SetUp();

	//文字をシーングラフに登録
	g_spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spFont );

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
		Ckeyboard_->Update( fpsCtrl );

		spFont->PrintfSentence( 32 , 32 , graphic::CColor_Tomato , 1.0f , _T( "DebugPrint." ) );

		//描画中だったらコマンド発行する
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//コマンドバッファの作成
			CRenderCommandManager_::CreateBuffer();

			//描画コマンド発行
			g_spScreen->Rendering( fpsCtrl );

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

	g_spCamera.Delete();
	spFont.Delete();
	g_spScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#if OFF_

static const skyString *SCREEN_SETUP_NAME       = _T( "Test" ); 
static const skyString *FONT_SCREEN_SETUP_NAME  = _T( "font" ); 

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
static const u32				_FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//カメラ
static graphic::Camera2DFixationPtr  g_spCamera;
static graphic::Camera2DFixationPtr  g_spFontCamera;

//スクリーン
static graphic::CScreenObjectPtr    g_spScreen;
static graphic::CScreenObjectPtr    g_spFontScreen;

TEST( Animation , 3 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	graphic::CBonePtr spBone = CBoneCreate_();

	//終了感知_登録する
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//フルスクリーンモードOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//--------3DScreenのセットアップ--------▽

	g_spCamera = Camera2DFixationCreate_();
	g_spCamera->vSetUp();
	g_spCamera->SetDefaultInputOn( skyTrue );

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

	//テクスチャ読み込み
	graphic::CTexturePtr spTexture0 = CTextureCreateName_( CHSC( TEST1_TEXTURE_PATH ) );
	spTexture0->Load(); //ロード

	//テクスチャ付きの矩形を作成
	graphic::CSpritePtr spSprite = graphic::CSpriteUtil::CreateSquare( spTexture0 , graphic::RENDERING_2D_PRIO_FAST , math::CBasicVector2( 2 , 2 ) , graphic::CColor_White );

	//スプライトをシーングラフに登録
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spSprite );

	//モデルにアニメーションをつける
	graphic::CKeyFrameEntity2DAnimationPtr spAnim = CKeyFrameEntity2DAnimationCreate_();
	spAnim->GetFCurve().AddKey( 2 , math::CEntity2D( math::CBasicVector2( 300 , 300 ) , //位置
	                                                 360 , //回転
		                                             math::CBasicVector2( 0.5 , 0.5 ) ) , eInterpolateType_Deceleration );
	spAnim->Play( 0 , 1.0f , 0.0f , kfm::ePlayEndType_Back );
	GET_BONE( spSprite )->AttachKeyFrameEntity2DAnimation( spAnim );

//------------------------------------------------------------

	//文字カメラ
	g_spFontCamera = Camera2DFixationCreate_();
	g_spFontCamera->vSetUp();
	g_spFontCamera->SetLeftTopCenter( skyTrue );

	//文字スクリーンのセットアップ
	screenData.Name               = CHS( FONT_SCREEN_SETUP_NAME );        //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = g_spFontCamera;                       //カメラ

	//スクリーンのセットアップ
	g_spFontScreen = g_spScreen->PushLayer( screenData );

	//文字を出力するので文字管理を初期化
	CFontManagerInitialize_();

	//基準線の作成
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateStandardLine3() );

	//文字を作成
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();
	spFont->SetUp();

	//文字をシーングラフに登録
	g_spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spFont );

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
		Ckeyboard_->Update( fpsCtrl );

		spFont->PrintfSentence( 32 , 32 , graphic::CColor_Tomato , 1.0f , _T( "DebugPrint." ) );

		//描画中だったらコマンド発行する
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//コマンドバッファの作成
			CRenderCommandManager_::CreateBuffer();

			//描画コマンド発行
			g_spScreen->Rendering( fpsCtrl );

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

	g_spCamera.Delete();
	spFont.Delete();
	g_spScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#if OFF_

static const skyString *SCREEN_SETUP_NAME       = _T( "Test" ); 
static const skyString *FONT_SCREEN_SETUP_NAME  = _T( "font" ); 

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
static const u32				_FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//カメラ
static graphic::Camera2DFixationPtr  g_spCamera;
static graphic::Camera2DFixationPtr  g_spFontCamera;

//スクリーン
static graphic::CScreenObjectPtr    g_spScreen;
static graphic::CScreenObjectPtr    g_spFontScreen;

TEST( Animation , 4 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	graphic::CBonePtr spBone = CBoneCreate_();

	//終了感知_登録する
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//フルスクリーンモードOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//--------3DScreenのセットアップ--------▽

	g_spCamera = Camera2DFixationCreate_();
	g_spCamera->vSetUp();
	g_spCamera->SetDefaultInputOn( skyTrue );

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

	//テクスチャ読み込み
	graphic::CTexturePtr spTexture0 = CTextureCreateName_( CHSC( TEST1_TEXTURE_PATH ) );
	spTexture0->Load(); //ロード

	//テクスチャ付きの矩形を作成
	graphic::CSpritePtr spSprite = graphic::CSpriteUtil::CreateSquare( spTexture0 , graphic::RENDERING_2D_PRIO_FAST , math::CBasicVector2( 2 , 2 ) , graphic::CColor_White );

	//スプライトにアニメーションをつける
	graphic::CKeyFrameEntity2DAnimationPtr spAnim = CKeyFrameEntity2DAnimationCreate_();
	spAnim->GetFCurve().AddKey( 2 , math::CEntity2D( math::CBasicVector2( 300 , 300 ) , //位置
	                                                 360 , //回転
		                                             math::CBasicVector2( 0.5 , 0.5 ) ) , eInterpolateType_Deceleration );
	spAnim->Play( 0 , 1.0f , 0.0f , kfm::ePlayEndType_Back );
	GET_BONE( spSprite )->AttachKeyFrameEntity2DAnimation( spAnim );

	//スプライトのマテリアルにアニメーションをつける
	graphic::CKeyFrameMaterialAnimationPtr spMaterialAnim = CKeyFrameMaterialAnimationCreate_();

	spMaterialAnim->GetFCurve().AddKey( 2 , graphic::CMaterial( 0.0f ) , eInterpolateType_Acceleration );

	spMaterialAnim->Play( 0 , 1.0f , 0 , kfm::ePlayEndType_Back );
	spSprite->GetMesh()->GetMeshGroup()->AttachMaterialAnimation( spMaterialAnim );

	//スプライトをシーングラフに登録
	spSprite->SetAlpha( skyTrue );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spSprite );

//------------------------------------------------------------

	//文字カメラ
	g_spFontCamera = Camera2DFixationCreate_();
	g_spFontCamera->vSetUp();
	g_spFontCamera->SetLeftTopCenter( skyTrue );

	//文字スクリーンのセットアップ
	screenData.Name               = CHS( FONT_SCREEN_SETUP_NAME );        //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = g_spFontCamera;                       //カメラ

	//スクリーンのセットアップ
	g_spFontScreen = g_spScreen->PushLayer( screenData );

	//文字を出力するので文字管理を初期化
	CFontManagerInitialize_();

	//基準線の作成
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateStandardLine3() );

	//文字を作成
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();
	spFont->SetUp();

	//文字をシーングラフに登録
	g_spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spFont );

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
		Ckeyboard_->Update( fpsCtrl );

		spFont->PrintfSentence( 32 , 32 , graphic::CColor_Tomato , 1.0f , _T( "DebugPrint." ) );

		//描画中だったらコマンド発行する
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//コマンドバッファの作成
			CRenderCommandManager_::CreateBuffer();

			//描画コマンド発行
			g_spScreen->Rendering( fpsCtrl );

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

	g_spCamera.Delete();
	spFont.Delete();
	g_spScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#if OFF_

static const skyString *SCREEN_SETUP_NAME       = _T( "Test" ); 
static const skyString *FONT_SCREEN_SETUP_NAME  = _T( "font" ); 

//Window初期位置、サイズ
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//１秒あたりのフレーム数
static const u32				_FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//カメラ
static graphic::Camera3DFixationPtr  g_spCamera;
static graphic::Camera2DFixationPtr  g_spFontCamera;

//テスト用モデル
static graphic::CModelPtr     g_TestCone;
static graphic::CModelPtr     g_TestPlane;

//スクリーン
static graphic::CScreenObjectPtr    g_spScreen;
static graphic::CScreenObjectPtr    g_spFontScreen;

math::CBasicVector3 m_Transrate( 0 , 1 , 0 );

static void ColValueEventDelegate_Vector3( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args )
{
	SKY_UNUSED_ARG( spSender );
	f32 *time                  = ( f32 * )args.Getu32( 0 );
	math::CBasicVector3 *value = ( math::CBasicVector3 * )args.Getu32( 1 );

	m_Transrate = *value;
}

TEST( Animation , 5 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	graphic::CBonePtr spBone = CBoneCreate_();

	//終了感知_登録する
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//フルスクリーンモードOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//--------3DScreenのセットアップ--------▽

	g_spCamera = Camera3DFixationCreate_();
	g_spCamera->vSetUp();
	g_spCamera->SetDefaultInputOn( skyTrue );

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

	//モデル作成
	g_TestCone = graphic::CModelUtil::CreateTestCone();
	g_TestPlane = graphic::CModelUtil::CreateTestPlane();

	//モデルをシーングラフに登録
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestCone );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestPlane );

//------------------------------------------------------------

	//文字カメラ
	g_spFontCamera = Camera2DFixationCreate_();
	g_spFontCamera->vSetUp();
	g_spFontCamera->SetLeftTopCenter( skyTrue );

	//文字スクリーンのセットアップ
	screenData.Name               = CHS( FONT_SCREEN_SETUP_NAME );        //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = g_spFontCamera;                       //カメラ

	//スクリーンのセットアップ
	g_spFontScreen = g_spScreen->PushLayer( screenData );

	//文字を出力するので文字管理を初期化
	CFontManagerInitialize_();

	//基準線の作成
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateStandardLine3() );

	//文字を作成
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();
	spFont->SetUp();

	//文字をシーングラフに登録
	g_spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spFont );

//--------3DScreenのセットアップ--------△

//--------アニメーションタスクのセットアップ-------▽

	interp::CInterpolateTask< math::CBasicVector3 > task;
	interp::CInterpolateTaskPlayer player;
	task.m_GetValueEventHndl += delegate::IDelegateObjectPtr( NEW delegate::CEventDelegateAdaptorFunc( ColValueEventDelegate_Vector3 ) ); 
	task.Start( math::CBasicVector3( 0.0f , 0.0f , 0.0f ) , math::CBasicVector3( 0.0f , 10.0f , 0.0f ) , 1 , &player , interp::ePlayEndType_End , eInterpolateType_Linear , 1.0f );

//--------アニメーションタスクのセットアップ-------△

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

		spFont->PrintfSentence( 32 , 32 , graphic::CColor_Tomato , 1.0f , _T( "DebugPrint." ) );

		//アニメーション更新
		player.Update( fpsCtrl.GetPassageTime_Second() );

		GET_BONE( g_TestCone )->SetPosition_XYZ( m_Transrate );

		//終了判定
		if ( Ckeyboard_->IsKeyPress( KEY_LEFT ) )
		{
//			task.Start( math::CBasicVector3( -10.0f , 0.0f , 0.0f ) , 1 , &player );
			task.RequestEndValue( math::CBasicVector3( -20.0f , 0.0f , 0.0f ) , 1.0f , 0.8f );
		}

		if ( Ckeyboard_->IsKeyPress( KEY_RIGHT ) )
		{
//			task.Start( math::CBasicVector3( 10.0f , 0.0f , 0.0f ) , 1 , &player );
			task.RequestEndValue( math::CBasicVector3( 20.0f , 0.0f , 0.0f ) , 1.0f , 0.8f );
		}

		if ( Ckeyboard_->IsKeyPress( KEY_UP ) )
		{
//			task.Start( math::CBasicVector3( 0.0f , 10.0f , 0.0f ) , 1 , &player );
			task.RequestEndValue( math::CBasicVector3( 0.0f , 20.0f , 0.0f ) , 1.0f , 0.8f );
		}

		if ( Ckeyboard_->IsKeyPress( KEY_DOWN ) )
		{
//			task.Start( math::CBasicVector3( 0.0f , -10.0f , 0.0f ) , 1 , &player );
			task.RequestEndValue( math::CBasicVector3( 0.0f , -20.0f , 0.0f ) , 1.0f , 0.8f );
		}

		//描画中だったらコマンド発行する
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//コマンドバッファの作成
			CRenderCommandManager_::CreateBuffer();

			//描画コマンド発行
			g_spScreen->Rendering( fpsCtrl );

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

	g_spCamera.Delete();
	spFont.Delete();
	g_spScreen.Delete();

	task.End();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#if ON_

static const skyString *SCREEN_SETUP_NAME       = _T( "Test" ); 
static const skyString *FONT_SCREEN_SETUP_NAME  = _T( "font" ); 

//Window初期位置、サイズ
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//１秒あたりのフレーム数
static const u32				_FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//カメラ
static graphic::Camera3DFixationPtr  g_spCamera;
static graphic::Camera2DFixationPtr  g_spFontCamera;

//スクリーン
static graphic::CScreenObjectPtr    g_spScreen;
static graphic::CScreenObjectPtr    g_spFontScreen;

math::CBasicVector3 m_Transrate( 0 , 1 , 0 );

static void ColValueEventDelegate_Vector3( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args )
{
	SKY_UNUSED_ARG( spSender );
	f32 *time                  = ( f32 * )args.Getu32( 0 );
	math::CBasicVector3 *value = ( math::CBasicVector3 * )args.Getu32( 1 );

	m_Transrate = *value;
}

TEST( Animation , 6 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	math::CBasicLine2 line2( math::CBasicVector2( 0 , 0 ) , math::CBasicVector2( 10 , 10 ) );
	math::CBasicVector2 vector2 = line2.Interpolate( 0.5f );

	graphic::CBonePtr spBone = CBoneCreate_();

	//終了感知_登録する
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//フルスクリーンモードOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//--------3DScreenのセットアップ--------▽

	g_spCamera = Camera3DFixationCreate_();
	g_spCamera->vSetUp();
	g_spCamera->SetDefaultInputOn( skyTrue );

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

//	graphic::CModelPtr spModel = graphic::CModelUtil::CreateTestSphere();
//	graphic::CModelPtr spModel = graphic::CModelUtil::CreateTestCube();
	graphic::CModelPtr spModel = graphic::CModelUtil::CreateTestCone();
	graphic::CModelPtr spModel_Hit = graphic::CModelUtil::CreateTestCone(); GET_BONE( spModel_Hit )->SetPosition_X( 2 ); 
	spModel->ComputeAABB3();
	spModel->ComputeOBB3();
	spModel->ComputeBS();

	graphic::CModelPtr spAABB = graphic::CModelUtil::CreateAABBDisp( spModel->GetAABB3() , graphic::CColor_Mistyrose );
	graphic::CModelPtr spOBB  = graphic::CModelUtil::CreateOBBDisp( spModel->GetOBB3() , graphic::CColor_Mistyrose );
	graphic::CModelPtr spOBS  = graphic::CModelUtil::CreateLine3DSphere( spModel->GetOBS() , graphic::CColor_Mistyrose );

//	SKY_PRINTF( "%f %f %f \n" , spModel->GetAABB3()->Center().X , spModel->GetAABB3()->Center().Y , spModel->GetAABB3()->Center().Z );
//	SKY_PRINTF( "%f %f %f \n" , spModel->GetOBB3()->m_Pos.X , spModel->GetOBB3()->m_Pos.Y , spModel->GetOBB3()->m_Pos.Z );
	SKY_PRINTF( "%f %f %f \n" , spModel->GetOBS().GetPos().X , spModel->GetOBS().GetPos().Y , spModel->GetOBS().GetPos().Z );

	//モデルをシーングラフに登録
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spModel );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spModel_Hit );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spAABB );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spOBB );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spOBS );

//------------------------------------------------------------

	//文字カメラ
	g_spFontCamera = Camera2DFixationCreate_();
	g_spFontCamera->vSetUp();
	g_spFontCamera->SetLeftTopCenter( skyTrue );

	//文字スクリーンのセットアップ
	screenData.Name               = CHS( FONT_SCREEN_SETUP_NAME );        //スクリーン名
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //スクリーンクリアカラー
	screenData.Camera             = g_spFontCamera;                       //カメラ

	//スクリーンのセットアップ
	g_spFontScreen = g_spScreen->PushLayer( screenData );

	//文字を出力するので文字管理を初期化
	CFontManagerInitialize_();

	//基準線の作成
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateStandardLine3() );

	//文字を作成
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();
	spFont->SetUp();

	//文字をシーングラフに登録
	g_spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spFont );

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
		Ckeyboard_->Update( fpsCtrl );

#if ON_
//-----Lineとの衝突
		dectype distance;
		math::CBasicVector3 pos;
		math::CBasicVector3 vec;
		if ( math::GeometryCollisionUtil::SphereCrossLine3( spModel->GetOBS() , g_spCamera->GetEyeLine( 100 ) ) == skyTrue )
		{
			spFont->PrintfSentence( 32 , 16 , graphic::CColor_Tomato , 1.0f , _T( "Line:Sphere Hit." ) );
		}
		if ( math::GeometryCollisionUtil::AABB3CrossLine3( spModel->GetAABB3() , g_spCamera->GetEyeLine( 100 ) , pos , vec ) == skyTrue )
		{
			spFont->PrintfSentence( 32 , 32 , graphic::CColor_Tomato , 1.0f , _T( "Line:AABB Hit. x=[ %f ] y=[ %f ]" ) , pos.X , pos.Y );
		}
		if ( math::GeometryCollisionUtil::OBB3CrossLine3( spModel->GetOBB3() , g_spCamera->GetEyeLine( 100 ) ) == skyTrue )
		{
			spFont->PrintfSentence( 32 , 48 , graphic::CColor_Tomato , 1.0f , _T( "Line:OBB Hit." ) );
		}
#endif

		//描画中だったらコマンド発行する
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//コマンドバッファの作成
			CRenderCommandManager_::CreateBuffer();

			//描画コマンド発行
			g_spScreen->Rendering( fpsCtrl );

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

	g_spCamera.Delete();
	spFont.Delete();
	g_spScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif