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

	//�q�X�N���[�����X�g�ɒǉ�
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

	//���C���[�X�N���[���X�^�b�N�ɂ̂���
	m_LayerScreenStack.Push( spResult );

	return spResult;
}

CScreenObjectPtr CScreenObject::CreateSubScreen( const CScreenObjectSetUpData &param )
{
	if ( m_ppScreenHash->IsGet( param.Name ) )
	{
		SKY_PANIC_MSG( _T( "The screen[ %s ] already exists." ) , ( const skyString * )param.Name ); //%s�Ƃ������O�̃X�N���[���͂��łɑ��݂��܂��B
		return CScreenObjectPtr();
	}

	if ( GetWindow().IsEmpty() )
	{
		SKY_PANIC_MSG( _T( "This screen is not a root." ) ); //���̃X�N���[����PushLayer�֐������s�ł��܂���BCreate�֐��ō쐬���ꂽ�X�N���[���Ŏ��s���Ă��������B
		return CScreenObjectPtr();
	}

	//�X�N���[���̍쐬
	CScreenObjectPtr screen = CScreenObjectPtr( NEW__( CScreenObject , CScreenObject() ) );

	//�g�`�r�g�ɓo�^
	m_ppScreenHash->Set( param.Name , screen );

	//�V�[���̍쐬�^�p�����[�^�[�ݒ�^������
	screen->AttachBranch( CScene::Create( param.Name ) );

	//�V�[���ɃJ�����Ǘ���ݒ�
	SKY_ASSERT_MSG( param.Camera.IsEmpty() == skyFalse , _T( "camera is isRequired." ) ); //�J�����͕K���K�v
	CameraManagerPtr spCameraManager = CameraManager::Create( param.Name );
	spCameraManager->ResisterCamera( param.Camera );
	spCameraManager->SetCurrentCameraRequest( param.Camera );
	screen->GetScene()->SetCameraManager( spCameraManager );

	//�V�[�����C�g��ݒ�
	screen->GetScene()->SetLight( CLightCreate_() );

	//�r���[�|�[�g�̍쐬�^�p�����[�^�[�ݒ�^������
	CViewPortPtr spViewPort = CViewPortCreateName_( param.Name );
	if ( param.IsSetParentWindowSize == skyTrue )
	{
		spViewPort->SetRect( math::CBasicRect( GetWindow()->GetRect().X() , GetWindow()->GetRect().Y() , GetWindow()->GetRect().W() , GetWindow()->GetRect().H() ) ); //�n���ꂽ�l�͖������Đe�Ƃ��Ȃ��傫���ɂ���
	}
	else
	{
		spViewPort->SetRect( math::CBasicRect( param.WindowRect.X() , param.WindowRect.Y() , param.WindowRect.W() , param.WindowRect.H() ) ); //�n���ꂽ�l�͖������Đe�Ƃ��Ȃ��傫���ɂ���
	}
	screen->GetScene()->SetViewPort( spViewPort );

	//�V�U�����O��`�̍쐬�^�p�����[�^�[�ݒ�^������
	screen->GetScene()->AttachLeaf( CScissorRect::Create() );
	screen->GetScene()->GetScissorRect()->SetRect( spViewPort->GetRect() );

	//�����_�[�^�[�Q�b�g������ΐݒ肷��
	if ( param.RenderTarget.IsEmpty() == skyFalse )
	{
		screen->GetScene()->SetRenderTarget( param.RenderTarget );
	}

	//�[�x�X�e���V���o�b�t�@������ΐݒ肷��
	if ( param.DepthStencilBuffer.IsEmpty() == skyFalse )
	{
		screen->GetScene()->SetDepthStencilBuffer( param.DepthStencilBuffer );
	}

	//�V�[���Ƀ����_�[�X�e�[�g������ΐݒ肷��
	if ( param.RenderState.IsEmpty() == skyFalse )
	{
		screen->GetScene()->SetRenderState( param.RenderState );
	}

	//��{�[�x�o�b�t�@�̓N���A����
	screen->GetScene()->m_IsDepthBufferClear = skyTrue;

	return screen;
}

void CScreenObject::ReSize( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args )
{
	SKY_UNUSED_ARG( spSender );

	//�����T�C�Y���ς���Ă��珈������
	if ( GetWindow()->GetWindowId() == args.Gets32( 2 ) &&
		GetWindow().IsEmpty() == skyFalse &&
		GetWindow()->IsChangeSize( ( dectype )args.Gets32( 0 ) , ( dectype )args.Gets32( 1 ) ) &&
		m_IsReSizeOn == skyTrue )
	{
		//�`�悪��~����܂ő҂�
		CRenderCommandManager_::WaiteRenderingStop();

		//���Ƃ��Ƃ�Window�̃T�C�Y�ƕύX��̒l���g�p���ĕω��������߂�
		dectype scw = ( dectype )args.Gets32( 0 ) / ( dectype )GetWindow()->GetRect().W();
		dectype sch = ( dectype )args.Gets32( 1 ) / ( dectype )GetWindow()->GetRect().H();
		u32 with = ( u32 )args.Gets32( 0 );
		u32 height = ( u32 )args.Gets32( 1 );

		//Window�̃T�C�Y��ύX
		GetWindow()->ReSize( ( dectype )with , ( dectype )height );

		//�����_�����O�^�[�Q�b�g�̃T�C�Y��ύX
		GetRenderTarget()->ReSize( with , height );

		//�X�e���V���o�b�t�@�̃T�C�Y��ύX
		GetDepthStencilBuffer()->ReSize( with , height );

		//�r���[�|�[�g�̃T�C�Y��ύX
		GetScene()->GetViewPort()->ReSize( with , height );

		//�V�U�����O��`�̃T�C�Y��ύX
		GetScene()->GetScissorRect()->ReSizeSc( ( dectype )scw , ( dectype )sch );

		//�J�����̃A�X�y�N�g���ύX
		GetScene()->GetCameraManager()->GetCurrentCamera()->SetDisplaySize( ( dectype )with , ( dectype )height );

		//���C���[�X�^�b�N�̃r���[�|�[�g�A�J�������ύX����
		FOREACH( CScreenObjectPtrStack , it , m_LayerScreenStack )
		{
			//�r���[�|�[�g�̃T�C�Y��ύX
			( *it )->GetScene()->GetViewPort()->ReSize( with , height );

			//�V�U�����O��`�̃T�C�Y��ύX
			( *it )->GetScene()->GetScissorRect()->ReSizeSc( ( dectype )scw , ( dectype )sch );

			//�J�����̃A�X�y�N�g���ύX
			( *it )->GetScene()->GetCameraManager()->GetCurrentCamera()->SetDisplaySize( ( dectype )with , ( dectype )height ); 
		}

		//�q�̃r���[�|�[�g�ƃJ�������ύX����
		FOREACH( CScreenObjectPtrList , it , m_ChildScreenList )
		{
			if( ( * it )->m_IsReSizeOn )
			{
				( * it )->ReSizeChild( scw , sch );
			}
		}

		//�`����ĊJ����
		CRenderCommandManager_::RequestRenderingReStart();
	}
}

void CScreenObject::ReSizeChild ( dectype scw , dectype sch )
{
	//�r���[�|�[�g�ƃJ�����̃T�C�Y��ύX�i�����ŕύX����j
    GetScene()->GetViewPort()->ReSizeSc( scw , sch );

	//�V�U�����O��`�̃T�C�Y�ύX
	GetScene()->GetScissorRect()->ReSizeSc( scw , sch );

	//�J�����̃A�X�y�N�g���ύX
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

	//�X�e�[�g��߂�
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

	//�g�`�r�g�ɓo�^
	m_ppScreenHash->Set( Name.GetString() , thisPtr( CSceneGraphNodePtr ) );

	//�񓯊��`��̏ꍇ�͂��̒i�K�ŃR�}���h�o�b�t�@���쐬����
#if IS_COMMAND

	//�R�}���h�o�b�t�@�̍쐬
	CRenderCommandManager_::CreateBuffer();

#endif

	//Window�̊�����
	GetWindow()->Activate();

	//�f�o�C�X�̏�����
	CRenderCommand::InitializeDevice( CDevice_ );

	//�����_�[�^�[�Q�b�g�̐ݒ�
	CRenderCommand::SetRadyFrameBufferRenderTarget( GetRenderTarget() , GetWindow() );

	//�[�x�X�e���V���o�b�t�@�̏�����
	CRenderCommand::SetRadyDepthStencilBuffer( GetDepthStencilBuffer() , GetRenderTarget() );

	//�S�̂̃����_�[�X�e�[�g�̊�����
	GetRenderState()->Activate(); 

	//�����_�����O�X�e�b�v�쐬
	CRenderStepPtr spRenderStep = CRenderStepCreate_( Name.GetName() );

	//�����_�����O�X�e�b�v��Window��o�^����
	spRenderStep->m_spWindow = GetWindow();

	//�����_�����O�X�e�b�v�Ƀ����_�[�^�[�Q�b�g��o�^����
	spRenderStep->m_spRenderTarget = GetRenderTarget();

	//�����_�����O�X�e�b�v�ɐ[�x�X�e���V���o�b�t�@��o�^����
	spRenderStep->m_spDepthStencilBuffer = GetDepthStencilBuffer();

	//�����_�����O�X�e�b�v�Ƀ����_�[�X�e�[�g��o�^����
	spRenderStep->m_spRenderState = GetRenderState();

	//�����_�����O�X�e�b�v�Ǘ��̍쐬
	m_RenderStepManager = CRenderStepManager_->Create();

	//�����_�����O�X�e�b�v�̓o�^
	m_RenderStepManager->AttachStep( RenderStep_Prio_1 , spRenderStep );

	//�f���Q�[�g�ݒ�i���[�g�����f���Q�[�g�����B���[�g���番�����ꂽ�q�̓f���Q�[�g�������Ȃ��j
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
		SKY_PANIC_MSG( _T( "The screen[ %s ] already exists." ) , ( const skyString * )param.Name ); //%s�Ƃ������O�̃X�N���[���͂��łɑ��݂��܂��B
		return CScreenObjectPtr();
	}

	CScreenObjectPtr spScreen = CScreenObjectPtr( NEW__( CScreenObject , CScreenObject() ) );
	spScreen->Name.SetName( param.Name );

	//�g�`�r�g�ɓo�^
	m_ppScreenHash->Set( param.Name , spScreen );

	//�񓯊��`��̏ꍇ�͂��̒i�K�ŃR�}���h�o�b�t�@���쐬����
#if IS_COMMAND

	//�R�}���h�o�b�t�@�̍쐬
	CRenderCommandManager_::CreateBuffer();

#endif

	//�E�B���h�E�̍쐬�^�p�����[�^�[�ݒ�^������
	if ( param.Window.IsEmpty() == skyTrue )
	{//��������
		spScreen->AttachBranch( CWindow::Create( param.Name ) );
		CRenderCommand::SetRadyWindow( spScreen->GetWindow() , param.WindowRect , skyFalse );
	}
	else
	{//����
		spScreen->AttachBranch( param.Window );
		param.Window->ReSize( param.WindowRect.W() , param.WindowRect.H() );
	}

	//�J�����̕��ƍ�����ݒ�
	param.Camera->SetDisplaySize( param.WindowRect.W() , param.WindowRect.H() );

	//�f�o�C�X�̏�����
	CRenderCommand::InitializeDevice( CDevice_ );

	//�V�[���̍쐬�^������
	spScreen->AttachBranch( CScene::Create( param.Name ) );
	spScreen->GetScene()->SetUp();

	//�����_�����O�^�[�Q�b�g�̍쐬�^�p�����[�^�[�ݒ�^������
	spScreen->AttachBranch( CRenderTarget::Create( param.Name ) );
	spScreen->GetRenderTarget()->SetClearColor( param.ClearColor );
	CRenderCommand::SetRadyFrameBufferRenderTarget( spScreen->GetRenderTarget() , spScreen->GetWindow() );

	//�[�x�X�e���V���o�b�t�@�̍쐬�^������
	spScreen->AttachBranch( CDepthStencilBuffer::Create( param.Name ) );
	CRenderCommand::SetRadyDepthStencilBuffer( spScreen->GetDepthStencilBuffer() , spScreen->GetRenderTarget() );

	//�V�U�����O��`�̍쐬�^�p�����[�^�[�ݒ�^������
	spScreen->GetScene()->AttachLeaf( CScissorRect::Create( param.Name ) );
	spScreen->GetScene()->GetScissorRect()->SetRect( spScreen->GetWindow()->GetRect() );

	//�S�̂̃����_�[�X�e�[�g�쐬�^�p�����[�^�[�ݒ�^������
	spScreen->AttachBranch( graphic::CRenderState::Create( param.Name ) );
	spScreen->GetRenderState()->EnableDepthBuffer(); //�[�x�X�e���V���o�b�t�@�X�e�[�g�쐬
	spScreen->GetRenderState()->EnableScissorTest();   //���X�^���C�U�X�e�[�g�쐬

	//�V�[���ɃJ�����Ǘ���ݒ�
	SKY_ASSERT_MSG( param.Camera.IsEmpty() == skyFalse , _T( "camera is isRequired." ) ); //�J�����͕K���K�v
	CameraManagerPtr spCameraManager = CameraManager::Create( param.Name );
	spCameraManager->ResisterCamera( param.Camera );
	spCameraManager->SetCurrentCameraRequest( param.Camera );
	spScreen->GetScene()->SetCameraManager( spCameraManager );

	//�V�[�����C�g��ݒ�
	spScreen->GetScene()->SetLight( CLightCreate_() );

	//�r���[�|�[�g�̍쐬�^�p�����[�^�[�ݒ�^������
	CViewPortPtr spViewPort = CViewPortCreateName_( param.Name );
	spViewPort->SetRect( spScreen->GetWindow()->GetRect() );
	spScreen->GetScene()->SetViewPort( spViewPort );

	//�����_�[�^�[�Q�b�g������ΐݒ肷��
	if ( param.RenderTarget.IsEmpty() == skyFalse )
	{
		spScreen->GetScene()->SetRenderTarget( param.RenderTarget );
	}

	//�[�x�X�e���V���o�b�t�@������ΐݒ肷��
	if ( param.DepthStencilBuffer.IsEmpty() == skyFalse )
	{
		spScreen->GetScene()->SetDepthStencilBuffer( param.DepthStencilBuffer );
	}

	//�����_�[�X�e�[�g������ΐݒ肷��
	if ( param.RenderState.IsEmpty() == skyFalse )
	{
		spScreen->GetScene()->SetRenderState( param.RenderState );
	}

	//�����_�����O�X�e�b�v�쐬
	CRenderStepPtr spRenderStep = CRenderStepCreate_( param.Name );

	//�����_�����O�X�e�b�v��Window��o�^����
	spRenderStep->m_spWindow = spScreen->GetWindow();

	//�����_�����O�X�e�b�v�Ƀ����_�[�^�[�Q�b�g��o�^����
	spRenderStep->m_spRenderTarget = spScreen->GetRenderTarget();

	//�����_�����O�X�e�b�v�ɐ[�x�X�e���V���o�b�t�@��o�^����
	spRenderStep->m_spDepthStencilBuffer = spScreen->GetDepthStencilBuffer();

	//�����_�����O�X�e�b�v�Ƀ����_�[�X�e�[�g��o�^����
	spRenderStep->m_spRenderState = spScreen->GetRenderState();

	//�����_�����O�X�e�b�v�Ǘ��̍쐬
	spScreen->m_RenderStepManager = CRenderStepManager_->Create();

	//�����_�����O�X�e�b�v�̓o�^
	spScreen->m_RenderStepManager->AttachStep( RenderStep_Prio_1 , spRenderStep );

	//�f���Q�[�g�ݒ�i���[�g�����f���Q�[�g�����B���[�g���番�����ꂽ�q�̓f���Q�[�g�������Ȃ��j
	spScreen->m_spReSizeDelegate.SetPtr( NEW__( delegate::CEventDelegateAdaptor< CScreenObject > , delegate::CEventDelegateAdaptor< CScreenObject >( spScreen.GetPtrNoConst() , &CScreenObject::ReSize ) ) );
	CEventManager_->AttachEventListener( CHS( event::SYSTEM_EVENT_WINDOW_RESIZE ) , spScreen->m_spReSizeDelegate );

	return spScreen;
}

CScreenObjectPtr CScreenObject::Get( const hash::CHashStringPtr &name )
{
	if ( m_ppScreenHash->IsGet( name ) == skyFalse )
	{
		SKY_PANIC_MSG( _T( "The screen[ %s ] doesn't exist. " ) , ( const skyString * )name ); //%s�Ƃ������O�̃X�N���[���͑��݂��܂���B
		return CScreenObjectPtr();
	}

	return ( *m_ppScreenHash )[ name ];
}

void CScreenObject::Rendering( const fps::CFpsController &fps )
{
	//�V�[���̃N���A���ݒ�
	m_RenderStepManager->DeleteAllScene();

	//���C���[�X�^�b�N�̃V�[�����ŏ��ɓo�^�i�R�Ԗڂɕ`��j
	FOREACH_REVERS( CScreenObjectPtrStack , it , m_LayerScreenStack )
	{
		if ( ( *it )->IsDisableOn() == skyFalse )
		{
			m_RenderStepManager->AttachFrontScene( ( *it )->GetScene() );
		}
	}

	if ( IsDisableOn() == skyFalse )
	{
		//�����̃V�[����o�^�i�Q�Ԗڂɕ`��j
		m_RenderStepManager->AttachFrontScene( GetScene() );
	}

	//�q�̃V�[����o�^�i�P�Ԗڂɕ`��j
	FOREACH( CScreenObjectPtrList , it , m_ChildScreenList )
	{
		if ( ( *it )->IsDisableOn() == skyFalse )
		{
			m_RenderStepManager->AttachFrontScene( ( * it )->GetScene() );
		}
	}

	//�`��
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
