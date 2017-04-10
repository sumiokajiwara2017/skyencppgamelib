#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_UI

#include "SkyFrameWorkLib/UI.h"
#include "SkyFrameWorkLib/Input.h"

using namespace sky::lib;

#if ON_

static const skyString *UI_SCREEN_NAME                   = _T( "UIScreen" ); 
static const skyString *CURSOR_SCREEN_NAME               = _T( "CursorScreen" ); 

//�J�����������
static const math::CBasicVector3 _CAMERA_DEFAULT_UP_VECTOR	= math::CBasicVector3( 0 , 1 , 0 );
static const dectype			 _CAMERA_DEFAULT_NEAR		= graphic::RENDERING_2D_PRIO_FAST;
static const dectype			 _CAMERA_DEFAULT_FAR		= graphic::RENDERING_2D_PRIO_MAX;

//Window�����ʒu�A�T�C�Y
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//�P�b������̃t���[����
static const u32 _FRAME_NUM = 60;

static const u32 RENDER_QUEUE_SIZE = 3;

//�J����
static graphic::CCamera2DPtr  g_spCamera;

//�X�N���[��
static graphic::CScreenObjectPtr    g_spUIScreen;
static graphic::CScreenObjectPtr    g_spCursorScreen;

TEST( ui , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//�I�����m�R�[���o�b�N��o�^����
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//�t���X�N���[�����[�hOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//--------UIScreen�̃Z�b�g�A�b�v--------��

	//�J�����̍쐬�^�p�����[�^�[�ݒ�^������
	g_spCamera = Camera2DFixationCreate_();
	g_spCamera->SetUpVector( _CAMERA_DEFAULT_UP_VECTOR );
	g_spCamera->SetNear( _CAMERA_DEFAULT_NEAR );
	g_spCamera->SetFar( _CAMERA_DEFAULT_FAR );
	g_spCamera->SetLeftTopCenter( skyTrue );

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( UI_SCREEN_NAME );             //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = g_spCamera;                           //�J����
	screenData.WindowRect.X()     = _WIN_X;                               //��ʈʒuX
	screenData.WindowRect.Y()     = _WIN_Y;                               //��ʈʒuY
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���
	screenData.RenderState        = IRenderStatCreate_();                 //�����_�[�X�e�[�g

	//�X�N���[���̃Z�b�g�A�b�v
	g_spUIScreen = CScreenObject_::Create( screenData );

	//�������o�͂���̂ŕ����Ǘ���������
	CFontManagerInitialize_();

	//�V�[���O���t�̃��[�g���쐬
	g_spUIScreen->GetScene()->m_spSceneGraph = graphic::CSceneGraphNodePtr( NEW graphic::CSceneGraphNode() );

	//�t�H�[���̍쐬
	framework::ui::CUITextListFormPtr spSampleForm = framework::ui::CUITextListForm::Create();
	spSampleForm->SetSize( math::CBasicVector2( 500 , 800 ) );
	spSampleForm->SetSize( math::CBasicVector2( 400 , 400 ) );

	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestTestLog1" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestTestTestTestTestTeTestTestTestTestTestTestTestTestTestTestTestTestTeststTestTestTestTestTestTestLog2" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestLog3" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestLog4" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestLog5" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestLog6" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestLog7" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestLog8" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestLog9" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestLog10" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestLog11" ) );
	spSampleForm->m_spTextList->PrintLine( graphic::CColor_Orangered , _T( ">TestLog12" ) );

	//UI�̃V�[���O���t�̃��[�g�ݒ肷��
	g_spUIScreen->GetScene()->m_spSceneGraph->AttachChild( CUIController_->GetSceneGraphRoot() );

//--------UIScreen�̃Z�b�g�A�b�v--------��

//--------CursorScreen�̃Z�b�g�A�b�v--------��

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	screenData.Name               = CHS( CURSOR_SCREEN_NAME );             //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = g_spCamera;                           //�J����

	g_spCursorScreen = g_spUIScreen->PushLayer( screenData );

	//�V�[���O���t�̃��[�g���쐬
	g_spCursorScreen->GetScene()->m_spSceneGraph = graphic::CSceneGraphNodePtr( NEW graphic::CSceneGraphNode() );

	//�}�E�X���𐶐�
	framework::CMouseArrowPtr spMouse( NEW framework::CMouseArrow() );

	//�}�E�X����ݒ�
	g_spCursorScreen->GetScene()->m_spSceneGraph->AttachChild( spMouse->m_spRootSprite );

//--------CursorScreen�̃Z�b�g�A�b�v--------��

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

		//�J�����ړ��A���f���ړ����͎�t
		math::CEntityUtil::DefaultInput_Entity2D( g_spCamera->GetEntity2D() , KEY_C );

		//�J�����ړ��A���f���ړ��𔽉f
		graphic::CCameraUtil::SetEntity2D( g_spCamera );

		//UI�R���g���[���[�̍X�V
		CUIController_->Update();

		//�`�撆��������R�}���h���s����
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//�R�}���h�o�b�t�@�̍쐬
			CRenderCommandManager_::CreateBuffer();

			//�`��R�}���h���s
			g_spUIScreen->Rendering();

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

	g_spCamera.Delete();
	spMouse.Delete();
	g_spUIScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif