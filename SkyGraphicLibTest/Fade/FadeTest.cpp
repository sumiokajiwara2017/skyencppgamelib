#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_FADE

using namespace sky::lib;

#include "SkyGraphicLib/Shader.h"

#if ON_

static const skyString *SCREEN_SETUP_NAME       = _T( "Test" ); 
static const skyString *FONT_SCREEN_SETUP_NAME  = _T( "font" ); 
static const skyString *FADE_SCREEN_SETUP_NAME  = _T( "fade" ); 

//Window�����ʒu�A�T�C�Y
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//�P�b������̃t���[����
static const u32				_FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//�J����
static graphic::Camera3DFixationPtr  g_spCamera;
static graphic::Camera2DFixationPtr  g_spFontCamera;
static graphic::Camera2DFixationPtr  g_spFadeCamera;

//�e�X�g�p���f��
static graphic::CModelPtr     g_TestCone;
static graphic::CModelPtr     g_TestPlane;

//�X�N���[��
static graphic::CScreenObjectPtr    g_spScreen;
static graphic::CScreenObjectPtr    g_spFontScreen;
static graphic::CScreenObjectPtr    g_spFadeScreen;

TEST( Fade , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	graphic::CBonePtr spBone = CBoneCreate_();

	//�I�����m_�o�^����
	static skyBool bExit = skyFalse;
	CEventManager_->AttachEventListener( 
		CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) , 
		sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptorBool( bExit ) ) );

	//�t���X�N���[�����[�hOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//--------3DScreen�̃Z�b�g�A�b�v--------��

	g_spCamera = Camera3DFixationCreate_();
	g_spCamera->vSetUp();
	g_spCamera->SetDefaultInputOn( skyTrue );

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( SCREEN_SETUP_NAME );             //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = g_spCamera;                           //�J����
	screenData.WindowRect.X()     = _WIN_X;                               //��ʈʒuX
	screenData.WindowRect.Y()     = _WIN_Y;                               //��ʈʒuY
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���
	screenData.RenderState        = CRenderStateCreate_();                 //�����_�[�X�e�[�g

	//�X�N���[���̃Z�b�g�A�b�v
	g_spScreen = CScreenObject_::Create( screenData );

	//���f���쐬
	g_TestCone = graphic::CModelUtil::CreateTestCone();
	g_TestPlane = graphic::CModelUtil::CreateTestPlane();

	//���f�����V�[���O���t�ɓo�^
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestCone );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestPlane );

	//���f���ɃA�j���[�V����������
	graphic::CKeyFrameEntity3DAnimationPtr spAnim = CKeyFrameEntity3DAnimationCreate_();
	spAnim->GetFCurve().AddKey( 2 , math::CEntity3D(
		math::CBasicVector3( 5 , 5 , 5 ) , //�ʒu
		math::CBasicVector3( 360 , 360 , 360 ) , //��]
		math::CBasicVector3( 2 , 2 , 2 ) ) , eInterpolateType_Linear );
	spAnim->Play( 0 , 1.0f , 0.0f , kfm::ePlayEndType_Back );
	g_TestCone->SetAlpha( skyTrue );
	g_TestCone->GetMesh()->GetMaterial()->Alpha = 0.5f;
	GET_BONE( g_TestCone )->AttachKeyFrameEntity3DAnimation( spAnim );

//------------------------------------------------------------

	//�����J����
	g_spFontCamera = Camera2DFixationCreate_();
	g_spFontCamera->vSetUp();
	g_spFontCamera->SetLeftTopCenter( skyTrue );

	//�����X�N���[���̃Z�b�g�A�b�v
	screenData.Name               = CHS( FONT_SCREEN_SETUP_NAME );        //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = g_spFontCamera;                       //�J����

	//�X�N���[���̃Z�b�g�A�b�v
	g_spFontScreen = g_spScreen->PushLayer( screenData );

	//�������o�͂���̂ŕ����Ǘ���������
	CFontManagerInitialize_();

	//����̍쐬
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateStandardLine3() );

	//�������쐬
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();
	spFont->SetUp();

	//�������V�[���O���t�ɓo�^
	g_spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spFont );

//------------------------------------------------------------

	//�����J����
	g_spFadeCamera = Camera2DFixationCreate_();
	g_spFadeCamera->vSetUp();

	//�����X�N���[���̃Z�b�g�A�b�v
	screenData.Name               = CHS( FADE_SCREEN_SETUP_NAME );        //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = g_spFadeCamera;                       //�J����

	//�X�N���[���̃Z�b�g�A�b�v
	g_spFadeScreen = g_spScreen->PushLayer( screenData );

	//�t�F�[�h�p�X�v���C�g�̐���
	CFadeManager_->CreateFadeSprite( g_spScreen->GetWindow()->GetRect().W() , g_spScreen->GetWindow()->GetRect().H() );
	CFadeManager_->SetFadeColor( graphic::CColor_Red );

	//�t�F�[�h�X�v���C�g���V�[���O���t�ɓo�^
	g_spFadeScreen->GetScene()->GetSceneGraphRoot()->AttachChild( CFadeManager_->GetSceneGraphRoot() );

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
		Ckeyboard_->Update( fpsCtrl );

		//�t�F�[�h�Ǘ��̍X�V
		CFadeManager_->Update( fpsCtrl );

		//�J�����Ǘ����X�V
		g_spScreen->GetScene()->GetCameraManager()->Update( fpsCtrl );

		spFont->PrintfSentence( 32 , 32 , graphic::CColor_Tomato , 1.0f , _T( "DebugPrint." ) );

		//�t�F�[�h�C��
		if ( Ckeyboard_->IsKeyPress( KEY_I ) )
		{
			CFadeManager_->ExecFadeIn();
		}

		//�t�F�[�h�A�E�g
		if ( Ckeyboard_->IsKeyPress( KEY_O ) )
		{
			CFadeManager_->ExecFadeOut();
		}

		//�t�F�[�h�C��
		if ( Ckeyboard_->IsKeyPress( KEY_S ) )
		{
			CFadeManager_->SetFadeInEnd();
		}

		//�t�F�[�h�A�E�g
		if ( Ckeyboard_->IsKeyPress( KEY_E ) )
		{
			CFadeManager_->SetFadeOutEnd();
		}


		//�`�撆��������R�}���h���s����
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//�R�}���h�o�b�t�@�̍쐬
			CRenderCommandManager_::CreateBuffer();

			//�`��R�}���h���s
			g_spScreen->Rendering( fpsCtrl );

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

	g_spCamera.Delete();
	spFont.Delete();
	g_spScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif