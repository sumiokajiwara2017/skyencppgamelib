#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_CAMERA

using namespace sky::lib;

//Window�����ʒu�A�T�C�Y
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//�P�b������̃t���[����
static const u32 _FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

TEST( Camera , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//�t���X�N���[�����[�hOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

	//�J�����̍쐬
	graphic::CameraPtr spCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spCamera->SetUp();

	//�T�u�J����
	graphic::CameraPtr spCamera_Left =  CameraCreate_( graphic::eCameraKind_Satellite );
	spCamera_Left->SetUp();
	graphic::CameraUtil::SetDefaultCameraPos_Left( spCamera_Left );

	graphic::CameraPtr spCamera_Right =  CameraCreate_( graphic::eCameraKind_Satellite );
	spCamera_Right->SetUp();
	graphic::CameraUtil::SetDefaultCameraPos_Right( spCamera_Right );

	graphic::CameraPtr spCamera_Top =  CameraCreate_( graphic::eCameraKind_Satellite );
	spCamera_Top->SetUp();
	graphic::CameraUtil::SetDefaultCameraPos_Top( spCamera_Top );

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( _T( "MainScreen" ) );             //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = spCamera;                             //�J����
	screenData.WindowRect.X()     = _WIN_X;                               //��ʈʒuX
	screenData.WindowRect.Y()     = _WIN_Y;                               //��ʈʒuY
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���
	screenData.RenderState        = CRenderStateCreate_();                 //�����_�[�X�e�[�g

	//�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectPtr spMainScreen = CScreenObject_::Create( screenData );

	//�T�u�J�����̓o�^
	spMainScreen->GetScene()->GetCameraManager()->ResisterCamera( spCamera_Left );
	spMainScreen->GetScene()->GetCameraManager()->ResisterCamera( spCamera_Right );
	spMainScreen->GetScene()->GetCameraManager()->ResisterCamera( spCamera_Top );
	spMainScreen->GetScene()->GetCameraManager()->SetDefaultInputOn( skyTrue );

	//���f���̍쐬
	graphic::CModelPtr spModel = graphic::CModelUtil::CreateTestCube();

	//�V�F�[�_�[�̌���
//	spModel->ClearAndAttachShader( CPointLightShaderObjectCreate_() );

	//���f�����V�[���O���t�ɓo�^
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spModel );
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateStandardLine3() );

	//�����Ǘ��̏������iDirectX11�̏ꍇ�AWindow����������ADevice���������ꂽ��łȂ��Ə������ł��Ȃ�
	CFontManagerInitialize_();

	//�����J����
	graphic::CameraPtr spFontCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spFontCamera->SetUp();                         //�K�{
	spFontCamera->SetOrtho( skyTrue );             //���ˉe
	spFontCamera->SetLeftTopCenter( skyTrue ); //���㌴�_

	//�����X�N���[���̃Z�b�g�A�b�v
	screenData.Name               = CHS( _T( "FontScreen" ) );        //�X�N���[����
	screenData.Camera             = spFontCamera;                       //�J����

	//�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectPtr spFontScreen = spMainScreen->PushLayer( screenData );

	//�������쐬
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();
	spFont->SetUp();

	//�������V�[���O���t�ɓo�^
	spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spFont );

	//FPS�R���g���[���[�쐬
	sky::lib::fps::CFpsController fpsCtrl;

	//�`����L���[�̍쐬
	CRenderCommandManager_::CreateQueue( RENDER_QUEUE_SIZE );

	//�`��p�X���b�h�̍쐬
	CRenderCommandManager_::StartThread();

	fpsCtrl.SetFrameRate( _FRAME_NUM );

	SKY_PRINTF( _T( "===================== < RenderTest Start > ===================== \n" ) );

	math::CBasicAngleVector3 cameraAngle( 2 , 0 , 0 , 30 );

	//�Q�[�����[�v
	s32 degAngle = 0;
	spCamera->SetDistance( 80.0f );
	GAME_LOOP_START
	{
		//�t���[���J�n
		fpsCtrl.StartFrame();

		//���͊Ǘ��X�V
		Ckeyboard_->Update( fpsCtrl );

		degAngle++;
		cameraAngle.Y().FromDeg( degAngle );
		spCamera->SetAngle( cameraAngle );

		if ( Ckeyboard_->IsKeyRepeat( KEY_R ) )
		{
			math::CBasicAngle angle;
			angle.FromDeg( 5 );
//			spModel->GetEntity3D()->m_Entity3D.AddRot_Z( angle );
//			spModel->GetEntity3D()->m_Entity3D.AddPos_Z( 1 );
			spModel->GetEntity3D()->m_Entity3D.AddSc_Z( 1 );
		}

		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "Camera Test." ) );

		//�`�撆��������R�}���h���s����
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//�R�}���h�o�b�t�@�̍쐬
			CRenderCommandManager_::CreateBuffer();

			//�`��R�}���h���s
			spMainScreen->Rendering( fpsCtrl );

			//�R�}���h�o�b�t�@�̒ǉ�
			CRenderCommandManager_::PushBuffer();
		}

		//�t���[�����[�g�ێ��̂��߁A���Ԃ��]������҂�
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	SKY_LIB_GRAPHIC_TERM();
}

#endif