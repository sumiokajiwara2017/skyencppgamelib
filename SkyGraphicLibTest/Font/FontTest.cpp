#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_FONT

#if ON_

using namespace sky::lib;

#include "SkyGraphicLib/Font.h"
#include "SkyGraphicLib/Screen.h"
#include "SkyGraphicLib/Renderer.h"

using namespace sky::lib;

static const skyString *TEST1_OBJ_ROOT = _T( "data/obj/" );
static const skyString *TEST1_MTL_ROOT = _T( "data/mtl/" );
static const skyString *TEST1_TEX_ROOT = _T( "data/texture/" );

static const skyString *TEST1_OBJ_FILE_NAME_1 = _T( "cube.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_2 = _T( "kuma.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_3 = _T( "sphere.obj" );

//�e�X�g�p�e�N�X�`���t�@�C���p�X
static const skyString *TEST1_TEXTURE_PATH = _T( "data/test/texture/TestBmp8bit.bmp" );

static const skyString *SCREEN_SETUP_NAME_3D                   = _T( "Test_Screen3D" ); 
static const skyString *SCREEN_SETUP_NAME_2D                   = _T( "Test_Screen2D" ); 
static const skyString *SCREEN_SETUP_NAME_3D_L                 = _T( "Test_Screen3DL" ); 
static const skyString *SCREEN_SETUP_NAME_3D_R                 = _T( "Test_Screen3DR" ); 

//�J�������
static const math::CBasicVector3 _CAMERA3D_DEFAULT_UP_VECTOR	= math::CBasicVector3( 0 , 1 , 0 );
static const dectype			 _CAMERA3D_DEFAULT_NEAR		= 1;
static const dectype			 _CAMERA3D_DEFAULT_FAR		= 20;
static const dectype			 _CAMERA3D_DEFAULT_FOV		= math::CMathUtil::DegToRad( 30.0f );
static const math::CBasicVector3 _CAMERA3D_DEFAULT_EYE		= math::CBasicVector3( 0.0f, 5.0f,-5.0f );
static const math::CBasicVector3 _CAMERA3D_DEFAULT_TAG		= math::CBasicVector3( 0.0f, 0.0f, 0.0f );

//�J�����������
static const math::CBasicVector3 _CAMERA2D_DEFAULT_UP_VECTOR	= math::CBasicVector3( 0 , 1 , 0 );
static const dectype			 _CAMERA2D_DEFAULT_NEAR		= graphic::RENDERING_2D_PRIO_FAST;
static const dectype			 _CAMERA2D_DEFAULT_FAR		= graphic::RENDERING_2D_PRIO_MAX;

//Window�����ʒu�A�T�C�Y
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//�P�b������̃t���[����
static const u32				_FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//�J����
static graphic::Camera3DPtr  g_spCamera3D;
static graphic::Camera2DPtr  g_spCamera2D;

//�X�N���[��
static graphic::CScreenObjectPtr    g_spScreen3D;

//�e�X�g�p���f��
static graphic::CModelPtr     g_spModel;

TEST( Font , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//�I�����m�R�[���o�b�N��o�^����
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//�t���X�N���[�����[�hOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

	//�����Ǘ��̏�����
	CFontManagerInitialize_();

//--------3DScreen�̃Z�b�g�A�b�v--------��

	//�J�����̍쐬�^�p�����[�^�[�ݒ�^������
	g_spCamera3D = Camera3DFixationCreate_();
	g_spCamera3D->SetUpVector( _CAMERA3D_DEFAULT_UP_VECTOR );
	g_spCamera3D->SetNear( _CAMERA3D_DEFAULT_NEAR );
	g_spCamera3D->SetFar( _CAMERA3D_DEFAULT_FAR );
	g_spCamera3D->SetFov( _CAMERA3D_DEFAULT_FOV );
	g_spCamera3D->SetEye( _CAMERA3D_DEFAULT_EYE );
	g_spCamera3D->SetTag( _CAMERA3D_DEFAULT_TAG ); 

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( SCREEN_SETUP_NAME_3D );             //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = g_spCamera3D;                           //�J����
	screenData.WindowRect.X()     = _WIN_X;                               //��ʈʒuX
	screenData.WindowRect.Y()     = _WIN_Y;                               //��ʈʒuY
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���
	screenData.RenderState        = IRenderStatCreate_();                 //�����_�[�X�e�[�g

	//�X�N���[���̃Z�b�g�A�b�v
	g_spScreen3D = CScreenObject_::Create( screenData );

	//�t�@�C�����烂�f���f�[�^���\�z
	CWavefrontOBJLoader_::SetObjFilePathRoot( CHSC( TEST1_OBJ_ROOT ) );
	CWavefrontOBJLoader_::SetMtlFilePathRoot( CHSC( TEST1_MTL_ROOT ) );
	CWavefrontOBJLoader_::SetTextureFilePathRoot( CHSC( TEST1_TEX_ROOT ) );
	g_spModel = graphic::CModelUtil::CreateFromWavefrontOBJ( CHSC( TEST1_OBJ_FILE_NAME_1 ) );

	//���f�����V�[���O���t�ɓo�^
	g_spScreen3D->GetScene()->m_spSceneGraph = g_spModel;

//--------3DScreen�̃Z�b�g�A�b�v--------��
//--------2DScreen�̃Z�b�g�A�b�v--------��

	//�J�����̍쐬�^�p�����[�^�[�ݒ�^������
	g_spCamera2D = Camera2DFixationCreate_();
	g_spCamera2D->SetUpVector( _CAMERA2D_DEFAULT_UP_VECTOR );
	g_spCamera2D->SetNear( _CAMERA2D_DEFAULT_NEAR );
	g_spCamera2D->SetFar( _CAMERA2D_DEFAULT_FAR );
	g_spCamera2D->SetLeftTopCenter( skyTrue );

	//���C���[�̃Z�b�g�A�b�v�f�[�^
	screenData.Name               = CHS( SCREEN_SETUP_NAME_2D );          //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = g_spCamera2D;                           //�J����
	screenData.RenderState        = IRenderStatCreate_();                 //�����_�[�X�e�[�g

	//�X�N���[���̃Z�b�g�A�b�v
	g_spScreen3D->PushLayer( screenData );
    graphic::CScreenObjectPtr spScreen2D = CScreenObject_::Get( CHS( SCREEN_SETUP_NAME_2D ) );


	//�V�[���O���t�̃��[�g���쐬
	CScreenObject_::Get( CHS( SCREEN_SETUP_NAME_2D ) )->GetScene()->m_spSceneGraph = graphic::CSceneGraphNodePtr( NEW graphic::CSceneGraphNode() );

	//�e�N�X�`���ǂݍ���
	graphic::CTexturePtr spTexture0 = graphic::CTexture::CreateInstance( CHSC( TEST1_TEXTURE_PATH ) );
	spTexture0->Load(); //���[�h

	//�������쐬
	graphic::CFontSimplePtr spFont = CFontSimple_();

	//�������V�[���O���t�ɓo�^
	spScreen2D->GetScene()->m_spSceneGraph->AttachChild( spFont );

	//�}�E�X���𐶐�
	graphic::CMouseArrowPtr spMouse( NEW graphic::CMouseArrow() );

	//�}�E�X����ݒ�
	spScreen2D->GetScene()->m_spSceneGraph->AttachChild( spMouse->m_spSprite );

//--------2DScreen�̃Z�b�g�A�b�v--------��

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

		//�x�������ɕ�������
		if ( Ckeyboard_->IsKeyPress( KEY_Y ) )
		{
			g_spScreen3D->SplitYCenter( CHS( SCREEN_SETUP_NAME_3D_L ) , CHS( SCREEN_SETUP_NAME_3D_R ) );
			g_spScreen3D->GetScene()->m_spSceneGraph.Delete();
		}

		//�w�������ɕ�������
		if ( Ckeyboard_->IsKeyPress( KEY_X ) )
		{
			g_spScreen3D->SplitXCenter( CHS( SCREEN_SETUP_NAME_3D_L ) , CHS( SCREEN_SETUP_NAME_3D_R ) );
			g_spScreen3D->GetScene()->m_spSceneGraph.Delete();
		}

		//�I������
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) || bExit == 1 )
		{
			GAME_LOOP_EXIT();
		}

		//���f���̉�]
		g_spModel->GetEntity3D().Rot.Y = CMathUtil_::RadToDeg( math::util::KPI * ( ( u32 )CTimeUtil_::GetNowTime() % 3000 ) / 1500.0f );
		g_spModel->GetEntity3D().Rot.X = CMathUtil_::RadToDeg( math::util::KPI * ( ( u32 )CTimeUtil_::GetNowTime() % 1500 ) / 750.0f );

		//�J�����̍X�V
		g_spCamera2D->vUpdate( fpsCtrl.GetPassageTime() );

		//�J�����ړ��A���f���ړ����͎�t
		math::CEntityUtil::DefaultInput_Entity2D( g_spCamera2D->GetEntity2D() , KEY_C );

		//�J�����ړ��A���f���ړ��𔽉f
		graphic::CameraUtil::SetEntity2D( g_spCamera2D );
		graphic::CModelUtil::SetEntity3D( g_spModel );

		//�������o��
		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , 1.0f , _T( "%f ms " ) , fpsCtrl.GetPassageTime() );
		spFont->PrintfSentence( 16 , 32 , graphic::CColor_White , 1.0f , 0.0f , _T( "abcdefghijklmnopqrstuvwxyz\n������������������������������\n1234567890" ) );

		//�`�撆��������R�}���h���s����
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//�R�}���h�o�b�t�@�̍쐬
			CRenderCommandManager_::CreateBuffer();

			//�`��R�}���h���s
			g_spScreen3D->Rendering();

			//�R�}���h�o�b�t�@�̒ǉ�
			CRenderCommandManager_::PushBuffer();
		}

		//�t���[�����[�g�ێ��̂��߁A���Ԃ��]������҂�
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	//�`��p�X���b�h�̒�~
	CRenderCommandManager_::StopThread();

	//�����Ǘ��̗���
	CFontManagerDispose_();

	g_spCamera2D.Delete();
	g_spCamera3D.Delete();
	g_spModel.Delete();
	spMouse.Delete();
	spScreen2D.Delete();
	g_spScreen3D.Delete();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif