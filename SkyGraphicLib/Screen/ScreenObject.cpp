#include "StdAfx.h"
#include "SkyGraphicLib/Screen.h"

#include "SkyGraphicLib/RenderTarget.h"
#include "SkyGraphicLib/ViewPort.h"
#include "SkyGraphicLib/Renderer.h"
#include "SkyGraphicLib/Light.h"

#include "SkyBaseLib/Event.h"

SkyImplementRTTI( sky::lib::graphic::CScreenObject     , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

static CScreenObjectPtrHash*  m_ppScreenHash = skyNull;

CScreenObjectPtr CScreenObject::AddChild( const CScreenObjectSetUpData &param )
{
	CScreenObjectPtr spResult = CreateSubScreen( param );

	//子スクリーンリストに追加
	m_ChildScreenList.AddFront( spResult );

	return spResult;
}

void CScreenObject::SetDisableOn( skyBool isDisable )
{
	m_IsDisable = isDisable;
}

skyBool CScreenObject::IsDisableOn()
{
	return m_IsDisable;
}

void CScreenObject::SetReSizeOn( skyBool isReSize )
{
	m_IsReSizeOn = isReSize;
}

skyBool CScreenObject::IsReSizeOn()
{
	return m_IsReSizeOn;
}

CScreenObjectPtr CScreenObject::PushLayer( const CScreenObjectSetUpData &param )
{
	CScreenObjectPtr spResult = CreateSubScreen( param );

	//レイヤースクリーンスタックにのせる
	m_LayerScreenStack.Push( spResult );

	return spResult;
}

CScreenObjectPtr CScreenObject::CreateSubScreen( const CScreenObjectSetUpData &param )
{
	if ( m_ppScreenHash->IsGet( param.Name ) )
	{
		SKY_PANIC_MSG( _T( "The screen[ %s ] already exists." ) , ( const skyString * )param.Name ); //%sという名前のスクリーンはすでに存在します。
		return CScreenObjectPtr();
	}

	if ( GetWindow().IsEmpty() )
	{
		SKY_PANIC_MSG( _T( "This screen is not a root." ) ); //このスクリーンはPushLayer関数を実行できません。Create関数で作成されたスクリーンで実行してください。
		return CScreenObjectPtr();
	}

	//スクリーンの作成
	CScreenObjectPtr screen = CScreenObjectPtr( NEW__( CScreenObject , CScreenObject() ) );

	//ＨＡＳＨに登録
	m_ppScreenHash->Set( param.Name , screen );

	//シーンの作成／パラメーター設定／初期化
	screen->AttachBranch( CScene::Create( param.Name ) );

	//シーンにカメラ管理を設定
	SKY_ASSERT_MSG( param.Camera.IsEmpty() == skyFalse , _T( "camera is isRequired." ) ); //カメラは必ず必要
	CameraManagerPtr spCameraManager = CameraManager::Create( param.Name );
	spCameraManager->ResisterCamera( param.Camera );
	spCameraManager->SetCurrentCameraRequest( param.Camera );
	screen->GetScene()->SetCameraManager( spCameraManager );

	//シーンライトを設定
	screen->GetScene()->SetLight( CLightCreate_() );

	//ビューポートの作成／パラメーター設定／初期化
	CViewPortPtr spViewPort = CViewPortCreateName_( param.Name );
	if ( param.IsSetParentWindowSize == skyTrue )
	{
		spViewPort->SetRect( math::CBasicRect( GetWindow()->GetRect().X() , GetWindow()->GetRect().Y() , GetWindow()->GetRect().W() , GetWindow()->GetRect().H() ) ); //渡された値は無視して親とおなじ大きさにする
	}
	else
	{
		spViewPort->SetRect( math::CBasicRect( param.WindowRect.X() , param.WindowRect.Y() , param.WindowRect.W() , param.WindowRect.H() ) ); //渡された値は無視して親とおなじ大きさにする
	}
	screen->GetScene()->SetViewPort( spViewPort );

	//シザリング矩形の作成／パラメーター設定／初期化
	screen->GetScene()->AttachLeaf( CScissorRect::Create() );
	screen->GetScene()->GetScissorRect()->SetRect( spViewPort->GetRect() );

	//レンダーターゲットがあれば設定する
	if ( param.RenderTarget.IsEmpty() == skyFalse )
	{
		screen->GetScene()->SetRenderTarget( param.RenderTarget );
	}

	//深度ステンシルバッファがあれば設定する
	if ( param.DepthStencilBuffer.IsEmpty() == skyFalse )
	{
		screen->GetScene()->SetDepthStencilBuffer( param.DepthStencilBuffer );
	}

	//シーンにレンダーステートがあれば設定する
	if ( param.RenderState.IsEmpty() == skyFalse )
	{
		screen->GetScene()->SetRenderState( param.RenderState );
	}

	//基本深度バッファはクリアする
	screen->GetScene()->m_IsDepthBufferClear = skyTrue;

	return screen;
}

void CScreenObject::ReSize( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args )
{
	SKY_UNUSED_ARG( spSender );

	//もしサイズが変わってたら処理する
	if ( GetWindow()->GetWindowId() == args.Gets32( 2 ) &&
		GetWindow().IsEmpty() == skyFalse &&
		GetWindow()->IsChangeSize( ( dectype )args.Gets32( 0 ) , ( dectype )args.Gets32( 1 ) ) &&
		m_IsReSizeOn == skyTrue )
	{
		//描画が停止するまで待つ
		CRenderCommandManager_::WaiteRenderingStop();

		//もともとのWindowのサイズと変更後の値を使用して変化率を求める
		dectype scw = ( dectype )args.Gets32( 0 ) / ( dectype )GetWindow()->GetRect().W();
		dectype sch = ( dectype )args.Gets32( 1 ) / ( dectype )GetWindow()->GetRect().H();
		u32 with = ( u32 )args.Gets32( 0 );
		u32 height = ( u32 )args.Gets32( 1 );

		//Windowのサイズを変更
		GetWindow()->ReSize( ( dectype )with , ( dectype )height );

		//レンダリングターゲットのサイズを変更
		GetRenderTarget()->ReSize( with , height );

		//ステンシルバッファのサイズを変更
		GetDepthStencilBuffer()->ReSize( with , height );

		//ビューポートのサイズを変更
		GetScene()->GetViewPort()->ReSize( with , height );

		//シザリング矩形のサイズを変更
		GetScene()->GetScissorRect()->ReSizeSc( ( dectype )scw , ( dectype )sch );

		//カメラのアスペクト比を変更
		GetScene()->GetCameraManager()->GetCurrentCamera()->SetDisplaySize( ( dectype )with , ( dectype )height );

		//レイヤースタックのビューポート、カメラも変更する
		FOREACH( CScreenObjectPtrStack , it , m_LayerScreenStack )
		{
			//ビューポートのサイズを変更
			( *it )->GetScene()->GetViewPort()->ReSize( with , height );

			//シザリング矩形のサイズを変更
			( *it )->GetScene()->GetScissorRect()->ReSizeSc( ( dectype )scw , ( dectype )sch );

			//カメラのアスペクト比を変更
			( *it )->GetScene()->GetCameraManager()->GetCurrentCamera()->SetDisplaySize( ( dectype )with , ( dectype )height ); 
		}

		//子のビューポートとカメラも変更する
		FOREACH( CScreenObjectPtrList , it , m_ChildScreenList )
		{
			if( ( * it )->m_IsReSizeOn )
			{
				( * it )->ReSizeChild( scw , sch );
			}
		}

		//描画を再開する
		CRenderCommandManager_::RequestRenderingReStart();
	}
}

void CScreenObject::ReSizeChild ( dectype scw , dectype sch )
{
	//ビューポートとカメラのサイズを変更（割合で変更する）
    GetScene()->GetViewPort()->ReSizeSc( scw , sch );

	//シザリング矩形のサイズ変更
	GetScene()->GetScissorRect()->ReSizeSc( scw , sch );

	//カメラのアスペクト比を変更
	GetScene()->GetCameraManager()->GetCurrentCamera()->SetDisplaySize( 
	GetScene()->GetViewPort()->GetRect().W() , 
	GetScene()->GetViewPort()->GetRect().H() );

	FOREACH( CScreenObjectPtrList , it , m_ChildScreenList )
	{
		( * it )->ReSizeChild( scw , sch );
	}
}

void CScreenObject::Constractor()
{
	m_IsDisable = skyFalse;
	m_IsReSizeOn = skyTrue;
}

CScreenObject::CScreenObject() : m_LayerScreenStack( SCREEN_LAYER_MAX_NUM )
{
	Constractor();
}

CScreenObject::CScreenObject( const CScreenObjectPtr &spParent ) : m_LayerScreenStack( SCREEN_LAYER_MAX_NUM )
{
	AttachParent( spParent );
	Constractor();
}

CScreenObject::~CScreenObject()
{
	if ( m_spReSizeDelegate.IsEmpty() == skyFalse )
	{
		CEventManager_->DetachEventListener( CHS( event::SYSTEM_EVENT_WINDOW_RESIZE ) , m_spReSizeDelegate );
	}

	//ステートを戻す
	if ( GetRenderState( skyFalse ).IsEmpty() == skyFalse )
	{
		GetRenderState()->ReverseRenderer();
	}
}


#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&m_IsDisable , sizeof( s32 ) , isWrite ); \

skyBool CScreenObject::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "screenobject bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN( skyTrue );

	return result;
}

skyBool CScreenObject::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "screenobject bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN( skyFalse );

	return result;
}

const serialize::CSerializeInterfacePtr &CScreenObject::Copy( const serialize::CSerializeInterfacePtr &spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CScreenObjectPtr )spContainer )->m_IsDisable = m_IsDisable;
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CScreenObject::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CScreenObject::Clone()
{
	return Copy( CScreenObjectPtr( NEW__( CScreenObject , CScreenObject() ) ) );
}

serialize::CSerializeInterfacePtr CScreenObject::CloneSharing()
{
	return Clone();
}

void CScreenObject::Activate()
{
	ACTIVATE_COMMON();

	//ＨＡＳＨに登録
	m_ppScreenHash->Set( Name.GetString() , thisPtr( CSceneGraphNodePtr ) );

	//非同期描画の場合はこの段階でコマンドバッファを作成する
#if IS_COMMAND

	//コマンドバッファの作成
	CRenderCommandManager_::CreateBuffer();

#endif

	//Windowの活性化
	GetWindow()->Activate();

	//デバイスの初期化
	CRenderCommand::InitializeDevice( CDevice_ );

	//レンダーターゲットの設定
	CRenderCommand::SetRadyFrameBufferRenderTarget( GetRenderTarget() , GetWindow() );

	//深度ステンシルバッファの初期化
	CRenderCommand::SetRadyDepthStencilBuffer( GetDepthStencilBuffer() , GetRenderTarget() );

	//全体のレンダーステートの活性化
	GetRenderState()->Activate(); 

	//レンダリングステップ作成
	CRenderStepPtr spRenderStep = CRenderStepCreate_( Name.GetName() );

	//レンダリングステップにWindowを登録する
	spRenderStep->m_spWindow = GetWindow();

	//レンダリングステップにレンダーターゲットを登録する
	spRenderStep->m_spRenderTarget = GetRenderTarget();

	//レンダリングステップに深度ステンシルバッファを登録する
	spRenderStep->m_spDepthStencilBuffer = GetDepthStencilBuffer();

	//レンダリングステップにレンダーステートを登録する
	spRenderStep->m_spRenderState = GetRenderState();

	//レンダリングステップ管理の作成
	m_RenderStepManager = CRenderStepManager_->Create();

	//レンダリングステップの登録
	m_RenderStepManager->AttachStep( RenderStep_Prio_1 , spRenderStep );

	//デリゲート設定（ルートだけデリゲートをもつ。ルートから分割された子はデリゲートを持たない）
	m_spReSizeDelegate.SetPtr( NEW delegate::CEventDelegateAdaptor< CScreenObject >( this , &CScreenObject::ReSize ) );
	CEventManager_->AttachEventListener( CHS( event::SYSTEM_EVENT_WINDOW_RESIZE ) , m_spReSizeDelegate );
}

void CScreenObject::Deactivate()
{
	DEACTIVATE_COMMON();
}

void CScreenObject::Initialize()
{
	m_ppScreenHash = NEW__( CScreenObjectPtrHash , CScreenObjectPtrHash( screen::HASH_SIZE ) );
}

void CScreenObject::Dispose()
{
	DEL m_ppScreenHash;
}

CScreenObjectPtr CScreenObject::Create( const CScreenObjectSetUpData &param )
{
	if ( m_ppScreenHash->IsGet( param.Name ) )
	{
		SKY_PANIC_MSG( _T( "The screen[ %s ] already exists." ) , ( const skyString * )param.Name ); //%sという名前のスクリーンはすでに存在します。
		return CScreenObjectPtr();
	}

	CScreenObjectPtr spScreen = CScreenObjectPtr( NEW__( CScreenObject , CScreenObject() ) );
	spScreen->Name.SetName( param.Name );

	//ＨＡＳＨに登録
	m_ppScreenHash->Set( param.Name , spScreen );

	//非同期描画の場合はこの段階でコマンドバッファを作成する
#if IS_COMMAND

	//コマンドバッファの作成
	CRenderCommandManager_::CreateBuffer();

#endif

	//ウィンドウの作成／パラメーター設定／初期化
	if ( param.Window.IsEmpty() == skyTrue )
	{//自動生成
		spScreen->AttachBranch( CWindow::Create( param.Name ) );
		CRenderCommand::SetRadyWindow( spScreen->GetWindow() , param.WindowRect , skyFalse );
	}
	else
	{//自作
		spScreen->AttachBranch( param.Window );
		param.Window->ReSize( param.WindowRect.W() , param.WindowRect.H() );
	}

	//カメラの幅と高さを設定
	param.Camera->SetDisplaySize( param.WindowRect.W() , param.WindowRect.H() );

	//デバイスの初期化
	CRenderCommand::InitializeDevice( CDevice_ );

	//シーンの作成／初期化
	spScreen->AttachBranch( CScene::Create( param.Name ) );
	spScreen->GetScene()->SetUp();

	//レンダリングターゲットの作成／パラメーター設定／初期化
	spScreen->AttachBranch( CRenderTarget::Create( param.Name ) );
	spScreen->GetRenderTarget()->SetClearColor( param.ClearColor );
	CRenderCommand::SetRadyFrameBufferRenderTarget( spScreen->GetRenderTarget() , spScreen->GetWindow() );

	//深度ステンシルバッファの作成／初期化
	spScreen->AttachBranch( CDepthStencilBuffer::Create( param.Name ) );
	CRenderCommand::SetRadyDepthStencilBuffer( spScreen->GetDepthStencilBuffer() , spScreen->GetRenderTarget() );

	//シザリング矩形の作成／パラメーター設定／初期化
	spScreen->GetScene()->AttachLeaf( CScissorRect::Create( param.Name ) );
	spScreen->GetScene()->GetScissorRect()->SetRect( spScreen->GetWindow()->GetRect() );

	//全体のレンダーステート作成／パラメーター設定／初期化
	spScreen->AttachBranch( graphic::CRenderState::Create( param.Name ) );
	spScreen->GetRenderState()->EnableDepthBuffer(); //深度ステンシルバッファステート作成
	spScreen->GetRenderState()->EnableScissorTest();   //ラスタライザステート作成

	//シーンにカメラ管理を設定
	SKY_ASSERT_MSG( param.Camera.IsEmpty() == skyFalse , _T( "camera is isRequired." ) ); //カメラは必ず必要
	CameraManagerPtr spCameraManager = CameraManager::Create( param.Name );
	spCameraManager->ResisterCamera( param.Camera );
	spCameraManager->SetCurrentCameraRequest( param.Camera );
	spScreen->GetScene()->SetCameraManager( spCameraManager );

	//シーンライトを設定
	spScreen->GetScene()->SetLight( CLightCreate_() );

	//ビューポートの作成／パラメーター設定／初期化
	CViewPortPtr spViewPort = CViewPortCreateName_( param.Name );
	spViewPort->SetRect( spScreen->GetWindow()->GetRect() );
	spScreen->GetScene()->SetViewPort( spViewPort );

	//レンダーターゲットがあれば設定する
	if ( param.RenderTarget.IsEmpty() == skyFalse )
	{
		spScreen->GetScene()->SetRenderTarget( param.RenderTarget );
	}

	//深度ステンシルバッファがあれば設定する
	if ( param.DepthStencilBuffer.IsEmpty() == skyFalse )
	{
		spScreen->GetScene()->SetDepthStencilBuffer( param.DepthStencilBuffer );
	}

	//レンダーステートがあれば設定する
	if ( param.RenderState.IsEmpty() == skyFalse )
	{
		spScreen->GetScene()->SetRenderState( param.RenderState );
	}

	//レンダリングステップ作成
	CRenderStepPtr spRenderStep = CRenderStepCreate_( param.Name );

	//レンダリングステップにWindowを登録する
	spRenderStep->m_spWindow = spScreen->GetWindow();

	//レンダリングステップにレンダーターゲットを登録する
	spRenderStep->m_spRenderTarget = spScreen->GetRenderTarget();

	//レンダリングステップに深度ステンシルバッファを登録する
	spRenderStep->m_spDepthStencilBuffer = spScreen->GetDepthStencilBuffer();

	//レンダリングステップにレンダーステートを登録する
	spRenderStep->m_spRenderState = spScreen->GetRenderState();

	//レンダリングステップ管理の作成
	spScreen->m_RenderStepManager = CRenderStepManager_->Create();

	//レンダリングステップの登録
	spScreen->m_RenderStepManager->AttachStep( RenderStep_Prio_1 , spRenderStep );

	//デリゲート設定（ルートだけデリゲートをもつ。ルートから分割された子はデリゲートを持たない）
	spScreen->m_spReSizeDelegate.SetPtr( NEW__( delegate::CEventDelegateAdaptor< CScreenObject > , delegate::CEventDelegateAdaptor< CScreenObject >( spScreen.GetPtrNoConst() , &CScreenObject::ReSize ) ) );
	CEventManager_->AttachEventListener( CHS( event::SYSTEM_EVENT_WINDOW_RESIZE ) , spScreen->m_spReSizeDelegate );

	return spScreen;
}

CScreenObjectPtr CScreenObject::Get( const hash::CHashStringPtr &name )
{
	if ( m_ppScreenHash->IsGet( name ) == skyFalse )
	{
		SKY_PANIC_MSG( _T( "The screen[ %s ] doesn't exist. " ) , ( const skyString * )name ); //%sという名前のスクリーンは存在しません。
		return CScreenObjectPtr();
	}

	return ( *m_ppScreenHash )[ name ];
}

void CScreenObject::Rendering( const fps::CFpsController &fps )
{
	//シーンのクリア＆設定
	m_RenderStepManager->DeleteAllScene();

	//レイヤースタックのシーンを最初に登録（３番目に描画）
	FOREACH_REVERS( CScreenObjectPtrStack , it , m_LayerScreenStack )
	{
		if ( ( *it )->IsDisableOn() == skyFalse )
		{
			m_RenderStepManager->AttachFrontScene( ( *it )->GetScene() );
		}
	}

	if ( IsDisableOn() == skyFalse )
	{
		//自分のシーンを登録（２番目に描画）
		m_RenderStepManager->AttachFrontScene( GetScene() );
	}

	//子のシーンを登録（１番目に描画）
	FOREACH( CScreenObjectPtrList , it , m_ChildScreenList )
	{
		if ( ( *it )->IsDisableOn() == skyFalse )
		{
			m_RenderStepManager->AttachFrontScene( ( * it )->GetScene() );
		}
	}

	//描画
	m_RenderStepManager->Rendering( fps );
}

CWindowPtr CScreenObject::GetWindow( skyBool isRequired )
{
	CWindowPtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachBranch< CWindow , CWindowPtr >( GetBranchRoot() , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "Window is not found." ) );
	}
#endif
	return spResult;
}

CScenePtr CScreenObject::GetScene( skyBool isRequired )
{
	CScenePtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachBranch< CScene , CScenePtr >( GetBranchRoot() , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "Scene is not found." ) );
	}
#endif
	return spResult;
}

CRenderTargetPtr CScreenObject::GetRenderTarget( skyBool isRequired )
{
	CRenderTargetPtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachBranch< CRenderTarget , CRenderTargetPtr >( GetBranchRoot() , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "RenderTarget is not found." ) );
	}
#endif
	return spResult;
}

CDepthStencilBufferPtr CScreenObject::GetDepthStencilBuffer( skyBool isRequired )
{
	CDepthStencilBufferPtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachBranch< CDepthStencilBuffer , CDepthStencilBufferPtr >( GetBranchRoot() , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "DepthStencilBuffer is not found." ) );
	}
#endif
	return spResult;
}

CRenderStatePtr CScreenObject::GetRenderState( skyBool isRequired )
{
	CRenderStatePtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachBranch< CRenderState , CRenderStatePtr >( GetBranchRoot() , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "RenderState is not found." ) );
	}
#endif
	return spResult;
}

} } }
