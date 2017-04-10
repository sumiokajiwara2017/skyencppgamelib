#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_INPUT

using namespace sky::lib;

#if ON_

static const skyString *SCREEN_SETUP_NAME                   = _T( "Test" ); 

//�J�����������
static const math::CBasicVector3 _CAMERA_DEFAULT_UP_VECTOR	= math::CBasicVector3( 0 , 1 , 0 );
static const dectype			 _CAMERA_DEFAULT_NEAR		= graphic::RENDERING_2D_PRIO_FAST;
static const dectype			 _CAMERA_DEFAULT_FAR		= graphic::RENDERING_2D_PRIO_MAX;

//Window�����ʒu�A�T�C�Y
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640;
static const u32  _WIN_H = 480;

//�P�b������̃t���[����
static const u32 _FRAME_NUM = 60;

static const u32 RENDER_QUEUE_SIZE = 3;

//�J����
static graphic::CCamera2DPtr  g_spCamera;

//�e�X�g�p���f��
static graphic::CSpritePtr     g_spSprite;

//�X�N���[��
static graphic::CScreenObjectPtr    g_spScreen;

TEST( input , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//�I�����m�R�[���o�b�N��o�^����
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//�t���X�N���[�����[�hOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//--------2DScreen�̃Z�b�g�A�b�v--------��

	//�J�����̍쐬�^�p�����[�^�[�ݒ�^������
	g_spCamera = Camera2DFixationCreate_();
	g_spCamera->SetUpVector( _CAMERA_DEFAULT_UP_VECTOR );
	g_spCamera->SetNear( _CAMERA_DEFAULT_NEAR );
	g_spCamera->SetFar( _CAMERA_DEFAULT_FAR );
	g_spCamera->SetLeftTopCenter( skyTrue );

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

	//�������o�͂���̂ŕ����Ǘ���������
	CFontManagerInitialize_();

	//�V�[���O���t�̃��[�g���쐬
	g_spScreen->GetScene()->m_spSceneGraph = graphic::CSceneGraphNodePtr( NEW graphic::CSceneGraphNode() );

	//��`���쐬
	g_spSprite = graphic::CSpriteUtil::CreateSquare( graphic::RENDERING_2D_PRIO_FAST , math::CBasicVector2( 64 , 64 ) );

	//�X�v���C�g���V�[���O���t�ɓo�^
	g_spScreen->GetScene()->m_spSceneGraph->AttachChild( g_spSprite );

	//�������쐬
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();

	//�������V�[���O���t�ɓo�^
	g_spScreen->GetScene()->m_spSceneGraph->AttachChild( spFont );

	//�}�E�X���𐶐�
	framework::CMouseArrowPtr spMouse( NEW framework::CMouseArrow() );

	//�}�E�X����ݒ�
	g_spScreen->GetScene()->m_spSceneGraph->AttachChild( spMouse->m_spSprite );

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

		//�I������
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) || bExit == 1 )
		{
			GAME_LOOP_EXIT();
		}

		//�J�����̍X�V
		g_spCamera->vUpdate( fpsCtrl.GetPassageTime() );

		//���f���̉�]
		g_spSprite->GetEntity2D().Pos.X = _WIN_W / 2;
		g_spSprite->GetEntity2D().Pos.Y = _WIN_H / 2;
	
		//�J�����ړ��A���f���ړ����͎�t
		math::CEntityUtil::DefaultInput_Entity2D( g_spCamera->GetEntity2D() , KEY_C );
		math::CEntityUtil::DefaultInput_Entity2D( g_spSprite->GetEntity2D() , KEY_X );

		//�J�����ړ��A���f���ړ��𔽉f
		graphic::CCameraUtil::SetEntity2D( g_spCamera );
		graphic::CSpriteUtil::SetEntity2D( g_spSprite );

		//�������o��
		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "%f ms " ) , fpsCtrl.GetPassageTime() );
		spFont->PrintfSentence( 16 , 32 , graphic::CColor_White , 1.0f , _T( "UITest" ) );

		//���Z�b�g
		if ( Ckeyboard_->IsKeyPress( KEY_HOME ) )
		{
			g_spSprite->GetProperty().ResetLocal();
			g_spSprite->GetProperty().ResetParent();
			g_spSprite->GetProperty().ResetWorld();
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

	//�����Ǘ��̔j��
	CFontManagerDispose_();

	g_spCamera.Delete();
	g_spSprite.Delete();
	spMouse.Delete();
	g_spScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif