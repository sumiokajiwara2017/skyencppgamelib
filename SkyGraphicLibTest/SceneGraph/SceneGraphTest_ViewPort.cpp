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

//�J�������
static const math::CBasicVector3 _CAMERA_DEFAULT_UP_VECTOR	= math::CBasicVector3( 0 , 1 , 0 );
static const dectype			 _CAMERA_DEFAULT_NEAR		= 1;
static const dectype			 _CAMERA_DEFAULT_FAR		= 20;
static const dectype			 _CAMERA_DEFAULT_FOV		= math::CMathUtil::DegToRad( 30.0f );
static const math::CBasicVector3 _CAMERA_DEFAULT_EYE		= math::CBasicVector3( 0.0f, 5.0f,-5.0f );
static const math::CBasicVector3 _CAMERA_DEFAULT_TAG		= math::CBasicVector3( 0.0f, 0.0f, 0.0f );

//Window�����ʒu�A�T�C�Y
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//�P�b������̃t���[����
static const u32				_FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//�J����
static graphic::Camera3DPtr  g_spCamera;

//�e�X�g�p���f��
static graphic::CModelPtr     g_spModel;

//�X�N���[��
static graphic::CScreenObjectPtr    g_spScreen;

TEST( scenegraph , 2 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//�I�����m�R�[���o�b�N��o�^����
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//�t���X�N���[�����[�hOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//--------3DScreen�̃Z�b�g�A�b�v--------��

	//�J�����̍쐬�^�p�����[�^�[�ݒ�^������
	g_spCamera = Camera3DFixationCreate_();
	g_spCamera->SetUpVector( _CAMERA_DEFAULT_UP_VECTOR );
	g_spCamera->SetNear( _CAMERA_DEFAULT_NEAR );
	g_spCamera->SetFar( _CAMERA_DEFAULT_FAR );
	g_spCamera->SetFov( _CAMERA_DEFAULT_FOV );
	g_spCamera->SetEye( _CAMERA_DEFAULT_EYE );
	g_spCamera->SetTag( _CAMERA_DEFAULT_TAG ); 

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( SCREEN_SETUP_NAME );             //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = g_spCamera;                           //�J����
	screenData.WindowRect.X()     = _WIN_X;                               //��ʈʒuX
	screenData.WindowRect.Y()     = _WIN_Y;                               //��ʈʒuY
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���
	screenData.RenderState        = IRenderStatCreate_();                 //�����_�[�X�e�[�g

	//�X�N���[���̃Z�b�g�A�b�v
	g_spScreen = CScreenObject_::Create( screenData );

	//�t�@�C�����烂�f���f�[�^���\�z
	CWavefrontOBJLoader_::SetObjFilePathRoot( CHSC( TEST1_OBJ_ROOT ) );
	CWavefrontOBJLoader_::SetMtlFilePathRoot( CHSC( TEST1_MTL_ROOT ) );
	CWavefrontOBJLoader_::SetTextureFilePathRoot( CHSC( TEST1_TEX_ROOT ) );
	g_spModel = graphic::CModelUtil::CreateFromWavefrontOBJ( CHSC( TEST1_OBJ_FILE_NAME_1 ) );

	graphic::CDepthStencilBufferPtr spCloneBuffer = g_spScreen->m_spDepthStencilBuffer->CloneNode();

	//���[�g�͋�̃m�[�h�ɂ���
	g_spScreen->GetScene()->m_spSceneGraph = CSceneGraphNodeCreate_();

	//���f�����q�Ƃ��ēo�^
	g_spScreen->GetScene()->m_spSceneGraph->AttachChild( g_spModel );

	//�r���[�|�[�g���V�[���O���t�̃��[�g�ɓo�^
	graphic::CViewPortPtr spViewPort = CViewPortCreate_();
	spViewPort->SetRect(  math::CBasicRectPtr( NEW math::CBasicRect( 0 , 0 , _WIN_W / 2 , _WIN_H / 2 ) ) );
	spViewPort->SetDepthBufferClear( skyTrue ); //�[�x�o�b�t�@�̓N���A����
	g_spScreen->GetScene()->m_spSceneGraph->AttachChild( spViewPort );

	//���f�����V�[���O���t�̎q�ƃr���[�|�[�g�̎q�ɓo�^
	spViewPort->AttachChild( g_spModel ); 

//--------3DScreen�̃Z�b�g�A�b�v--------��

	//FPS�R���g���[���[�쐬
	sky::lib::fps::CFpsController fpsCtrl;

	//�`����L���[�̍쐬
	CRenderCommandManager_::CreateQueue( RENDER_QUEUE_SIZE );

	//�`��p�X���b�h�̍쐬
	CRenderCommandManager_::StartThread();

	fpsCtrl.SetFrameRate( _FRAME_NUM );

	SKY_PRINTF( _T( "===================== < RenderTest Start > ===================== \n" ) );

	//�Q�[�����[�v
	GAME_LOOP_START
	{
		//�t���[���J�n
		fpsCtrl.StartFrame();

		//���͊Ǘ��X�V
		Ckeyboard_->Update( fpsCtrl.GetPassageTime() );

		//�I������
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) || bExit == 1 )
		{
			GAME_LOOP_EXIT();
		}

		//�J�����̍X�V
		g_spCamera->vUpdate( fpsCtrl.GetPassageTime() );

		//���f���̉�]
		g_spModel->GetEntity3D().Rot.Y = CMathUtil_::RadToDeg( math::util::KPI * ( ( u32 )CTimeUtil_::GetNowTime() % 3000 ) / 1500.0f );
		g_spModel->GetEntity3D().Rot.X = CMathUtil_::RadToDeg( math::util::KPI * ( ( u32 )CTimeUtil_::GetNowTime() % 1500 ) / 750.0f );
	
		//�J�����ړ��A���f���ړ����͎�t
		math::CEntityUtil::DefaultInput_Entity3D( g_spCamera->GetEntity3D() , KEY_C );
		math::CEntityUtil::DefaultInput_Entity3D( g_spModel->GetEntity3D() , KEY_X );

		//�J�����ړ��A���f���ړ��𔽉f
		graphic::CameraUtil::SetEntity3D( g_spCamera );
		graphic::CModelUtil::SetEntity3D( g_spModel );

		//���Z�b�g
		if ( Ckeyboard_->IsKeyPress( KEY_HOME ) )
		{
			g_spModel->GetProperty().ResetLocal();
			g_spModel->GetProperty().ResetParent();
			g_spModel->GetProperty().ResetWorld();
		}

		//�`�撆��������R�}���h���s����
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//�R�}���h�o�b�t�@�̍쐬
			CRenderCommandManager_::CreateBuffer();

			//�`��R�}���h���s
			g_spScreen->Rendering();

			//�R�}���h�o�b�t�@�̒ǉ�
			CRenderCommandManager_::PushBuffer();
		}

		//�t���[�����[�g�ێ��̂��߁A���Ԃ��]������҂�
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	//�`��p�X���b�h�̒�~
	CRenderCommandManager_::StopThread();

	g_spCamera.Delete();
	g_spModel.Delete();
	g_spScreen.Delete();
	spCloneBuffer.Delete();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif