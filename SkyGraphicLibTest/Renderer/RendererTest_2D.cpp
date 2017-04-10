#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_RENDERER

#if ON_

#include "SkyBaseLib/GameLoop.h"
#include "SkyBaseLib/Input.h"
#include "SkyBaseLib/Fps.h"

#include "SkyGraphicLib/Camera.h"
#include "SkyGraphicLib/Model.h"
#include "SkyGraphicLib/Sprite.h"
#include "SkyGraphicLib/Scene.h"
#include "SkyGraphicLib/Renderer.h"
#include "SkyMathLib/Entity.h"

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

static const skyString *SCREEN_SETUP_NAME                   = _T( "Test" ); 

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
static const u32				_FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//�J����
static graphic::Camera2DPtr  g_spCamera;

//�e�X�g�p���f��
static graphic::CSpritePtr     g_spSprite;

//�X�N���[��
static graphic::CScreenObjectPtr    g_spScreen;

TEST( Render , 1 ) 
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
	g_spCamera->vSetUp();
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
	screenData.RenderState        = CRenderStateCreate_();                 //�����_�[�X�e�[�g

	//�X�N���[���̃Z�b�g�A�b�v
	g_spScreen = CScreenObject_::Create( screenData );

	//�������o�͂���̂ŕ����Ǘ���������
	CFontManagerInitialize_();

	//�e�N�X�`���ǂݍ���
	graphic::CTexturePtr spTexture0 = CTextureCreateName_( CHSC( TEST1_TEXTURE_PATH ) );
	spTexture0->Load(); //���[�h

	//�e�N�X�`���t���̋�`���쐬
	g_spSprite = graphic::CSpriteUtil::CreateSquare( spTexture0 , graphic::RENDERING_2D_PRIO_FAST , math::CBasicVector2( 2 , 2 ) , graphic::CColor_White );
	g_spSprite->GetMesh()->CreateGpuMemory();
	g_spSprite->GetMesh()->WirteGpuMemory();

	//�X�v���C�g���V�[���O���t�ɓo�^
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_spSprite );

	graphic::CSpritePtr spLineSprite = graphic::CSpriteUtil::CreateSquareLine( 32 , 32 );
	spLineSprite->SetLockParentRotAndScale( skyTrue );

	//�V�F�[�_�[�����ւ���
	graphic::CMaterialDiffuseColorShaderObjectPtr effectPtr = graphic::CMaterialDiffuseColorShaderObject::Create();
	spLineSprite->LeafList().DeleteAll();
	spLineSprite->AttachLeaf( 0 , effectPtr );

	//�@�������(��L�V�F�[�_�[�͎g�p����̂Łj
	spLineSprite->GetMesh()->GetMaterial()->CreateNormalStream( spLineSprite->GetMesh()->GetVertexBuffer()->VertexNum() );

	//�F��ݒ肷��
	spLineSprite->GetMesh()->SetDiffuse( graphic::CColor_Gold ); 

	//GPU�������ɏ�������
	graphic::CRenderCommand::CreateMeshGpuMemory( spLineSprite->GetMesh() );
	graphic::CRenderCommand::WirteMeshGpuMemory( spLineSprite->GetMesh() );

	//CPU�������͏���
	graphic::CRenderCommand::DeleteMeshCpuMemory( spLineSprite->GetMesh() );

	//�V�[���O���t�̐擪�ɒu��
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spLineSprite );

	GET_BONE( spLineSprite )->SetScale_X( 2.0f );
	GET_BONE( spLineSprite )->SetScale_Y( 2.0f );
	GET_BONE( spLineSprite )->MulRotation_Z( 0.8f );
	GET_BONE( spLineSprite )->SetPosition_X( 256 );
	GET_BONE( spLineSprite )->SetPosition_Y( 256 );

	//�������쐬
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();
	spFont->SetUp();

	//�������V�[���O���t�ɓo�^
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spFont );

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
		g_spSprite->GetEntity2D()->m_Entity2D.SetRot( math::util::KPI * ( ( u32 )CTimeUtil_::GetNowTime() % 3000 ) / 1500.0f );
		g_spSprite->GetEntity2D()->m_Entity2D.SetPos_X( _WIN_W / 2 );
		g_spSprite->GetEntity2D()->m_Entity2D.SetPos_Y(_WIN_H / 2 );
	
		//�J�����ړ��A���f���ړ����͎�t
		math::CEntityUtil::DefaultInput_Entity2D( g_spCamera->GetEntity2D() , KEY_C );
		math::CEntityUtil::DefaultInput_Entity2D( g_spSprite->GetEntity2D() , KEY_X );

		//�������o��
		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "%f ms" ) , fpsCtrl.GetPassageTime() );
//		spFont->PrintfSentence( 16 , 32 , graphic::CColor_White , 1.0f , _T( "abcdefghijklmnopqrstuvwxyz" ) );

		//�`�撆��������R�}���h���s����
		if ( CRenderCommandManager_::IsRenderingExec() )
		{
			//�R�}���h�o�b�t�@�̍쐬
			CRenderCommandManager_::CreateBuffer();

			//�`��R�}���h���s
			g_spScreen->Rendering( CTimeUtil_::GetNowTime() );

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
	g_spScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}


#endif

#endif