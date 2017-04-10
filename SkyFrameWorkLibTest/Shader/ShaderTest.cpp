#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_SHADER

#include "SkyFrameWorkLib/UI.h"
#include "SkyFrameWorkLib/Input.h"

using namespace sky::lib;

#if ON_

#ifdef SKY_GRAPHIC_LIB_OPENGL

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

#include "Cg/CgLightShaderObject.h"

static const skyString *TEST1_OBJ_ROOT = _T( "data/obj/" );
static const skyString *TEST1_MTL_ROOT = _T( "data/mtl/" );
static const skyString *TEST1_TEX_ROOT = _T( "data/texture/" );

static const skyString *TEX_CUBE_MODEL_FILE = _T( "cube.obj" );

static const skyString *TD_SCREEN_NAME                   = _T( "3DScreen" ); 
static const skyString *CURSOR_SCREEN_NAME               = _T( "CursorScreen" ); 
static const skyString *FONT_SCREEN_NAME                 = _T( "FontScreen" ); 
static const skyString *UI_SCREEN_NAME                   = _T( "UIScreen" ); 

//�J�����������
static const math::CBasicVector3 _CAMERA_DEFAULT_UP_VECTOR	= math::CBasicVector3( 0 , 1 , 0 );
static const dectype			 _CAMERA2D_DEFAULT_NEAR		= graphic::RENDERING_2D_PRIO_FAST;
static const dectype			 _CAMERA2D_DEFAULT_FAR		= graphic::RENDERING_2D_PRIO_MAX;
static const dectype			 _CAMERA3D_DEFAULT_NEAR		= 1;
static const dectype			 _CAMERA3D_DEFAULT_FAR		= 1000;
static const dectype			 _CAMERA3D_DEFAULT_FOV		= math::CMathUtil::DegToRad( 30.0f );

//Window�����ʒu�A�T�C�Y
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//�P�b������̃t���[����
static const u32 _FRAME_NUM = 60;

static const u32 RENDER_QUEUE_SIZE = 3;

//�J����

static graphic::CCamera3DSatellitePtr  g_spCamera3D;
//static graphic::CCamera3DDebugPtr    g_spCamera3D;
static graphic::CCamera2DPtr           g_spCamera2D;

//�X�N���[��
static graphic::CScreenObjectPtr    g_sp3DScreen;
static graphic::CScreenObjectPtr    g_spFontScreen;
static graphic::CScreenObjectPtr    g_spUIScreen;
static graphic::CScreenObjectPtr    g_spCursorScreen;

//���f��
static graphic::CModelPtr     g_TestCube;
static graphic::CModelPtr     g_TestCone;
static graphic::CModelPtr     g_TestPlane;

TEST( shader , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//�I�����m�R�[���o�b�N��o�^����
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//�t���X�N���[�����[�hOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//--------�J�����̃Z�b�g�A�b�v--------��

	//�J�����̍쐬�^�p�����[�^�[�ݒ�^������
	g_spCamera3D = CCamera3DSatelliteCreate_();
//    g_spCamera3D = CCamera3DDebugCreate_();
	g_spCamera3D->SetUpVector( _CAMERA_DEFAULT_UP_VECTOR );
	g_spCamera3D->SetNear( _CAMERA3D_DEFAULT_NEAR );
	g_spCamera3D->SetFar( _CAMERA3D_DEFAULT_FAR );
	g_spCamera3D->SetFov( _CAMERA3D_DEFAULT_FOV );

	//�J�����̍쐬�^�p�����[�^�[�ݒ�^������
	g_spCamera2D = Camera2DFixationCreate_();
	g_spCamera2D->SetUpVector( _CAMERA_DEFAULT_UP_VECTOR );
	g_spCamera2D->SetNear( _CAMERA2D_DEFAULT_NEAR );
	g_spCamera2D->SetFar( _CAMERA2D_DEFAULT_FAR );
	g_spCamera2D->SetLeftTopCenter( skyTrue );

//--------3DScreen�̃Z�b�g�A�b�v--------��

	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( TD_SCREEN_NAME );                //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = g_spCamera3D;                         //�J����
	screenData.WindowRect.X()     = _WIN_X;                               //��ʈʒuX
	screenData.WindowRect.Y()     = _WIN_Y;                               //��ʈʒuY
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���
	screenData.RenderState        = IRenderStatCreate_();                 //�����_�[�X�e�[�g

	//�X�N���[���̃Z�b�g�A�b�v
	g_sp3DScreen = CScreenObject_::Create( screenData );

	g_TestCube = graphic::CModelUtil::CreateFromWavefrontOBJ( CHSC( TEX_CUBE_MODEL_FILE ) );
	g_TestCone = graphic::CModelUtil::CreateTestCone();
	g_TestPlane = graphic::CModelUtil::CreateTestPlane();

	//�V�F�[�_�[�����ւ���
	g_TestCube->DetachShaderObjectAll();
//	g_TestCube->AttachShaderObject( 0 , graphic::CCgLightShaderObject::Create() );
    g_TestCube->AttachShaderObject( 0 , graphic::CBasicShaderObject::Create() );

	//���f�����V�[���O���t�ɓo�^
	g_sp3DScreen->GetScene()->m_spSceneGraph = CSceneGraphNodeCreate_();
	g_sp3DScreen->GetScene()->m_spSceneGraph->AttachChild( g_TestCube ); g_TestCube->GetEntity3D()->Pos.Y = 1.0f;
	g_sp3DScreen->GetScene()->m_spSceneGraph->AttachChild( g_TestCone ); g_TestCone->GetEntity3D()->Pos.X = 4.0f; g_TestCone->GetEntity3D()->Pos.Y = 1.0f;
	g_sp3DScreen->GetScene()->m_spSceneGraph->AttachChild( g_TestPlane );

//--------CursorScreen�̃Z�b�g�A�b�v--------��

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	screenData.Name               = CHS( CURSOR_SCREEN_NAME );             //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = g_spCamera2D;                           //�J����

	g_spCursorScreen = g_sp3DScreen->PushLayer( screenData );

	//�V�[���O���t�̃��[�g���쐬
	g_spCursorScreen->GetScene()->m_spSceneGraph = graphic::CSceneGraphNodePtr( NEW graphic::CSceneGraphNode() );

	//�}�E�X���𐶐�
	framework::CMouseArrowPtr spMouse( NEW framework::CMouseArrow() );

	//�}�E�X����ݒ�
	g_spCursorScreen->GetScene()->m_spSceneGraph->AttachChild( spMouse->m_spRootSprite );

//--------FontScreen�̃Z�b�g�A�b�v--------��

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	screenData.Name               = CHS( FONT_SCREEN_NAME );             //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = g_spCamera2D;                           //�J����

	g_spFontScreen = g_sp3DScreen->PushLayer( screenData );

	//�V�[���O���t�̃��[�g���쐬
	g_spFontScreen->GetScene()->m_spSceneGraph = graphic::CSceneGraphNodePtr( NEW graphic::CSceneGraphNode() );

	//�������o�͂���̂ŕ����Ǘ���������
	CFontManagerInitialize_();

	//�������쐬
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();

	//�������V�[���O���t�ɓo�^
	g_spFontScreen->GetScene()->m_spSceneGraph->AttachChild( spFont );

//--------UIScreen�̃Z�b�g�A�b�v--------��

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	screenData.Name               = CHS( UI_SCREEN_NAME );             //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = g_spCamera2D;                           //�J����
	screenData.RenderState        = IRenderStatCreate_();                 //�����_�[�X�e�[�g

	//�X�N���[���̃Z�b�g�A�b�v
	g_spUIScreen = g_sp3DScreen->PushLayer( screenData );

	//�V�[���O���t�̃��[�g���쐬
	g_spUIScreen->GetScene()->m_spSceneGraph = graphic::CSceneGraphNodePtr( NEW graphic::CSceneGraphNode() );

	//�t�H�[���̍쐬
	framework::ui::CUITextListFormPtr spSampleForm = framework::ui::CUITextListForm::Create( CHS( _T( "" ) ) );
	spSampleForm->SetSize( math::CBasicVector2( 500 , 800 ) );
	spSampleForm->SetSize( math::CBasicVector2( 400 , 400 ) );

	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTest" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">Log OutPutTest" ) );

	//UI�̃V�[���O���t�̃��[�g�ݒ肷��
	g_spUIScreen->GetScene()->m_spSceneGraph->AttachChild( CUIController_->GetSceneGraphRoot() );

//--------�A�v���P�[�V�����J�n--------��

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

		//�������o��
		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "%f ms " ) , fpsCtrl.GetPassageTime() );

		//�J�����̍X�V
		g_spCamera3D->vUpdate( fpsCtrl.GetPassageTime() );

		//���f���ړ��ʒu���f
		graphic::CModelUtil::SetEntity3D( g_TestCube );
		graphic::CModelUtil::SetEntity3D( g_TestCone );

		//UI�R���g���[���[�̍X�V
		CUIController_->Update( fpsCtrl.GetPassageTime() );

		//�`�撆��������R�}���h���s����
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//�R�}���h�o�b�t�@�̍쐬
			CRenderCommandManager_::CreateBuffer();

			//�`��R�}���h���s
			g_sp3DScreen->Rendering();

			//�R�}���h�o�b�t�@�̒ǉ�
			CRenderCommandManager_::PushBuffer();
		}

		//�t���[�����[�g�ێ��̂��߁A���Ԃ��]������҂�
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	//�`��p�X���b�h�̒�~
	CRenderCommandManager_::StopThread();

	//�����Ǘ��̔j��
	CFontManagerDispose_();

	g_spCamera2D.Delete();
	g_spCamera3D.Delete();
	spMouse.Delete();
	g_spUIScreen.Delete();
	g_sp3DScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif

#endif

#endif