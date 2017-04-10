#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_SPRITE

using namespace sky::lib;

//�e�X�g�p�e�N�X�`���t�@�C���p�X
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

//Window�����ʒu�A�T�C�Y
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//�P�b������̃t���[����
static const u32 _FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

#include "SkyGraphicLib/Sprite.h"

#if OFF_

TEST( sprite , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//�t���X�N���[�����[�hOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

	//�J�����̍쐬
	graphic::CameraPtr spCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spCamera->SetOrtho( skyTrue );
	spCamera->SetDefaultInput();

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

	//�e�N�X�`���ǂݍ���
	graphic::CTexturePtr spTexture0 = CTextureCreateName_( CHSC( TEST1_TEXTURE_PATH ) );
	spTexture0->Load(); //���[�h

	//�e�N�X�`���t���̋�`���쐬
	graphic::CSpritePtr spSprite = graphic::CSpriteUtil::CreateSquare( spTexture0 , graphic::RENDERING_2D_PRIO_FAST , math::CBasicVector2( 2 , 2 ) , graphic::CColor_White );

	//�X�v���C�g���V�[���O���t�ɓo�^
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spSprite );
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CSpriteUtil::CreateStandardLine2() );

	//�����Ǘ��̏������iDirectX11�̏ꍇ�AWindow����������ADevice���������ꂽ��łȂ��Ə������ł��Ȃ�
	CFontManagerInitialize_();

	//�����J����
	graphic::CameraPtr spFontCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spFontCamera->SetOrtho( skyTrue );
	spFontCamera->SetLeftTopCenter( skyTrue );

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

	//�Q�[�����[�v
	GAME_LOOP_START
	{
		//�t���[���J�n
		fpsCtrl.StartFrame();

		//���͊Ǘ��X�V
		Ckeyboard_->Update( fpsCtrl );

		if ( Ckeyboard_->IsKeyRepeat( KEY_R ) )
		{
			math::CBasicAngle angle;
			angle.FromRad( math::CMathUtil::DegToRad( 5 ) );
		}

		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "Sprite Test Draw." ) );

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

#if ON_

TEST( sprite , 2 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//�t���X�N���[�����[�hOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

	//�J�����̍쐬
	graphic::CameraPtr spCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spCamera->SetDefaultInput();
	spCamera->SetEye( math::CBasicVector3( 0 , 0 , 15 ) );

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( _T( "MainScreen" ) );             //�X�N���[����
	screenData.ClearColor         = graphic::CColor_Black;             //�X�N���[���N���A�J���[
	screenData.Camera             = spCamera;                             //�J����
	screenData.WindowRect.X()     = _WIN_X;                               //��ʈʒuX
	screenData.WindowRect.Y()     = _WIN_Y;                               //��ʈʒuY
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���
	screenData.RenderState        = CRenderStateCreate_();                 //�����_�[�X�e�[�g

	//�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectPtr spMainScreen = CScreenObject_::Create( screenData );

	//�e�N�X�`���ǂݍ���
	graphic::CTexturePtr spTexture0 = CTextureCreateName_( CHSC( _T( "data/test/texture/beam.bmp" ) ) );
	spTexture0->Load(); //���[�h

	graphic::CBillboardBeamPtr spEffect = graphic::CBillboardBeam::Create();
	spEffect->Initialize( 32 , 0.25f , spTexture0 ); 

	//�X�v���C�g���V�[���O���t�ɓo�^
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spEffect );
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CSpriteUtil::CreateStandardLine2() );

	//�����Ǘ��̏������iDirectX11�̏ꍇ�AWindow����������ADevice���������ꂽ��łȂ��Ə������ł��Ȃ�
	CFontManagerInitialize_();

	//�����J����
	graphic::CameraPtr spFontCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spFontCamera->SetOrtho( skyTrue );
	spFontCamera->SetLeftTopCenter( skyTrue );

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

	//�Q�[�����[�v
	GAME_LOOP_START
	{
		//�t���[���J�n
		fpsCtrl.StartFrame();

		//���͊Ǘ��X�V
		Ckeyboard_->Update( fpsCtrl );

		if ( Ckeyboard_->IsKeyRepeat( KEY_R ) )
		{
			math::CBasicAngle angle;
			angle.FromRad( math::CMathUtil::DegToRad( 5 ) );
		}

		//������
		graphic::CBillboardBeamUtil::Move_Thunder( spEffect );
//		graphic::CBillboardBeamUtil::Move_Rot( spEffect );

		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "Sprite Test Draw." ) );

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

#endif