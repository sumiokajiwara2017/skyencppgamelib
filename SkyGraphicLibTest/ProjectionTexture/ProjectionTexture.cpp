#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_PROJECTION_TEXTURE

#if OFF_

using namespace sky::lib;

#include "SkyGraphicLib/Loader.h"

static const skyString *TEST1_OBJ_FILE_NAME_1 = _T( "testcube.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_2 = _T( "testcone.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_3 = _T( "testplane10x10.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_4 = _T( "testsphere.obj" );

static const skyString *SCREEN_SETUP_NAME                   = _T( "Test" ); 
static const skyString *FONT_SCREEN_SETUP_NAME              = _T( "font" ); 

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

//�e�X�g�p���f��
static graphic::CModelPtr     g_TestCube_1;
static graphic::CModelPtr     g_TestCube_2;
static graphic::CModelPtr     g_TestSphere;
static graphic::CModelPtr     g_TestCubeShadow;
static graphic::CModelPtr     g_TestPlane;

//�X�N���[��
static graphic::CScreenObjectPtr    g_spScreen;
static graphic::CScreenObjectPtr    g_spFontScreen;

TEST( ProjectionTexture , 1 ) 
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
	g_spCamera = Camera3DFixationCreate_();
	g_spCamera->vSetUp();
	g_spCamera->SetDefaultInputOn( skyTrue );
	g_spCamera->SetEye( math::CBasicVector3( 0.0f , 8.5f , -35.0f ) );
	g_spCamera->SetInitEye( g_spCamera->GetEye() );
	g_spCamera->SetTag( math::CBasicVector3( 0.0f , 8.5f , -25.0f ) );
	g_spCamera->SetInitTag( g_spCamera->GetTag() );

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

	//���f�����쐬
	g_TestCube_1 = graphic::CModelUtil::CreateTestCube();
	GET_BONE( g_TestCube_1 )->SetPosition_X( 1.0f );
	GET_BONE( g_TestCube_1 )->SetPosition_Y( 5.0f );
	GET_BONE( g_TestCube_1 )->SetPosition_Z( 0.0f );

	g_TestCube_2 = graphic::CModelUtil::CreateTestCube();
	GET_BONE( g_TestCube_2 )->SetPosition_X( 5.0f );
	GET_BONE( g_TestCube_2 )->SetPosition_Y( 8.0f );
	GET_BONE( g_TestCube_2 )->SetPosition_Z( 4.0f );

	g_TestSphere = graphic::CModelUtil::CreateTestSphere( graphic::CColor_Red );
	GET_BONE( g_TestSphere )->MulScale_XYZ( math::CBasicVector3( 0.3f , 0.3 , 0.3 ) );
	GET_BONE( g_TestSphere )->SetPosition_X( -3.0f );
	GET_BONE( g_TestSphere )->SetPosition_Y( 8.0f );
	GET_BONE( g_TestSphere )->SetPosition_Z( 0.0f );
	 
	g_TestCubeShadow = g_TestCube_1->CloneSharingTraverse();
	g_TestCubeShadow->ActivateTraverse();

	g_TestPlane = graphic::CModelUtil::CreateTestPlane();
	GET_BONE( g_TestPlane )->SetPosition_Y( -0.4f );

	//���t���N�V����
//	GET_BONE( g_TestConeShadow )->MulReflect( math::eAxis_x );
//	GET_BONE( g_TestConeShadow )->MulReflect( math::eAxis_y );
//	GET_BONE( g_TestConeShadow )->MulReflect( math::eAxis_z );
//	GET_BONE( g_TestConeShadow )->MulReflect( math::CBasicVector3( 1.0f , 1.0f , 0.0f ).Normalize() );

	//����f
//	GET_BONE( g_TestConeShadow )->MulShear( math::eAxis_x , 10.0f , 3.0f );

	//���ʓ���
//	GET_BONE( g_TestConeShadow )->MulProjection( math::CBasicVector3( 1.0f , 0.0f , 0.0f ).Normalize() ); //YZ����
//	GET_BONE( g_TestConeShadow )->MulProjection( math::CBasicVector3( 0.0f , 1.0f , 0.0f ).Normalize() ); //XZ����
//	GET_BONE( g_TestConeShadow )->MulProjection( math::CBasicVector3( 0.0f , 0.0f , 1.0f ).Normalize() ); //XY����
	GET_BONE( g_TestCubeShadow )->MulProjection( 
		math::CBasicPlane( math::CBasicVector3( 0.0f , 0.0f , 0.0f ) , math::CBasicVector3( 0.0f , 0.0f , 1.0f ) , math::CBasicVector3( 1.0f , 0.0f , 0.0f ) ) , 
		GET_BONE( g_TestSphere )->GetPosition_XYZ() - GET_BONE( g_TestCube_1 )->GetPosition_XYZ() );

	//�O���[�o���V�F�[�_�[���쐬
	graphic::CPointLightShaderObjectPtr pointLight = graphic::CPointLightShaderObject::Create();
	pointLight->SetLightPosition( math::CBasicVector3( 10.0f , 10.0f , -10.0f ) );
	pointLight->SetLightPower( 5.0f );

	//���[�J���V�F�[�_�[�̍폜
	g_TestCube_1->DetachShader();
	g_TestCube_2->DetachShader();
	g_TestCubeShadow->DetachShader();
	g_TestPlane->DetachShader();

	//���f�����V�[���O���t�ɓo�^
	pointLight->AttachChild( g_TestCube_1 );
	pointLight->AttachChild( g_TestCube_2 );
	pointLight->AttachChild( g_TestCubeShadow );
	pointLight->AttachChild( g_TestPlane );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( pointLight );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestSphere );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateStandardLine3() );

	//�������쐬
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();
	spFont->SetUp();

	//�������V�[���O���t�ɓo�^
	g_spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spFont );

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

		spFont->PrintfSentence( 16 , 16 * 1 , graphic::CColor_Tomato , 1.0f , _T( "cameraPos = [ %f %f %f ]" ) , g_spCamera->GetEye().X , g_spCamera->GetEye().Y , g_spCamera->GetEye().Z );
		spFont->PrintfSentence( 16 , 16 * 2 , graphic::CColor_Tomato , 1.0f , _T( "cameraTag = [ %f %f %f ]" ) , g_spCamera->GetTag().X , g_spCamera->GetTag().Y , g_spCamera->GetTag().Z );

		//�J�����̍X�V
		g_spCamera->vUpdate( fpsCtrl.GetPassageTime() );

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

	g_spCamera.Delete();
	g_spScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}
#endif

#if ON_

using namespace sky::lib;

#include "SkyGraphicLib/Loader.h"

static const skyString *TEST1_OBJ_FILE_NAME_1 = _T( "testcube.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_2 = _T( "testcone.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_3 = _T( "testplane10x10.obj" );
static const skyString *TEST1_OBJ_FILE_NAME_4 = _T( "testsphere.obj" );

static const skyString *SCREEN_SETUP_NAME                   = _T( "Test" ); 
static const skyString *FONT_SCREEN_SETUP_NAME              = _T( "font" ); 

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

//�e�X�g�p���f��
static graphic::CModelPtr     g_TestCube_1;
static graphic::CModelPtr     g_TestCube_2;
static graphic::CModelPtr     g_TestSphere;
static graphic::CModelPtr     g_TestCubeShadow;
static graphic::CModelPtr     g_TestPlane;

//�X�N���[��
static graphic::CScreenObjectPtr    g_spScreen;
static graphic::CScreenObjectPtr    g_spFontScreen;

TEST( ProjectionTexture , 2 ) 
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
	g_spCamera = Camera3DFixationCreate_();
	g_spCamera->vSetUp();
	g_spCamera->SetDefaultInputOn( skyTrue );
	g_spCamera->SetEye( math::CBasicVector3( 0.0f , 8.5f , -35.0f ) );
	g_spCamera->SetInitEye( g_spCamera->GetEye() );
	g_spCamera->SetTag( math::CBasicVector3( 0.0f , 8.5f , -25.0f ) );
	g_spCamera->SetInitTag( g_spCamera->GetTag() );

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

	//���f�����쐬
	g_TestCube_1 = graphic::CModelUtil::CreateTestCube();
	GET_BONE( g_TestCube_1 )->SetPosition_X( 1.0f );
	GET_BONE( g_TestCube_1 )->SetPosition_Y( 5.0f );
	GET_BONE( g_TestCube_1 )->SetPosition_Z( 0.0f );

	g_TestCube_2 = graphic::CModelUtil::CreateTestCube();
	GET_BONE( g_TestCube_2 )->SetPosition_X( 5.0f );
	GET_BONE( g_TestCube_2 )->SetPosition_Y( 8.0f );
	GET_BONE( g_TestCube_2 )->SetPosition_Z( 4.0f );

	g_TestSphere = graphic::CModelUtil::CreateTestSphere( graphic::CColor_Red );
	GET_BONE( g_TestSphere )->MulScale_XYZ( math::CBasicVector3( 0.3f , 0.3 , 0.3 ) );
	GET_BONE( g_TestSphere )->SetPosition_X( -3.0f );
	GET_BONE( g_TestSphere )->SetPosition_Y( 8.0f );
	GET_BONE( g_TestSphere )->SetPosition_Z( 0.0f );
	 
	g_TestCubeShadow = g_TestCube_1->CloneSharingTraverse();
	g_TestCubeShadow->ActivateTraverse();

	g_TestPlane = graphic::CModelUtil::CreateTestPlane();
	GET_BONE( g_TestPlane )->SetPosition_Y( -0.4f );

	//���t���N�V����
//	GET_BONE( g_TestConeShadow )->MulReflect( math::eAxis_x );
//	GET_BONE( g_TestConeShadow )->MulReflect( math::eAxis_y );
//	GET_BONE( g_TestConeShadow )->MulReflect( math::eAxis_z );
//	GET_BONE( g_TestConeShadow )->MulReflect( math::CBasicVector3( 1.0f , 1.0f , 0.0f ).Normalize() );

	//����f
//	GET_BONE( g_TestConeShadow )->MulShear( math::eAxis_x , 10.0f , 3.0f );

	//���ʓ���
//	GET_BONE( g_TestConeShadow )->MulProjection( math::CBasicVector3( 1.0f , 0.0f , 0.0f ).Normalize() ); //YZ����
//	GET_BONE( g_TestConeShadow )->MulProjection( math::CBasicVector3( 0.0f , 1.0f , 0.0f ).Normalize() ); //XZ����
//	GET_BONE( g_TestConeShadow )->MulProjection( math::CBasicVector3( 0.0f , 0.0f , 1.0f ).Normalize() ); //XY����
	GET_BONE( g_TestCubeShadow )->MulProjection( 
		math::CBasicPlane( math::CBasicVector3( 0.0f , 0.0f , 0.0f ) , math::CBasicVector3( 0.0f , 0.0f , 1.0f ) , math::CBasicVector3( 1.0f , 0.0f , 0.0f ) ) , 
		GET_BONE( g_TestSphere )->GetPosition_XYZ() - GET_BONE( g_TestCube_1 )->GetPosition_XYZ() );

	//�O���[�o���V�F�[�_�[���쐬
	graphic::CPointLightShaderObjectPtr pointLight = graphic::CPointLightShaderObject::Create();
	pointLight->SetLightPosition( math::CBasicVector3( 10.0f , 10.0f , -10.0f ) );
	pointLight->SetLightPower( 5.0f );

	//���[�J���V�F�[�_�[�̍폜
	g_TestCube_1->DetachShader();
	g_TestCube_2->DetachShader();
	g_TestCubeShadow->DetachShader();
	g_TestPlane->DetachShader();

	//���f�����V�[���O���t�ɓo�^
	pointLight->AttachChild( g_TestCube_1 );
	pointLight->AttachChild( g_TestCube_2 );
	pointLight->AttachChild( g_TestCubeShadow );
	pointLight->AttachChild( g_TestPlane );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( pointLight );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestSphere );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateStandardLine3() );

	//�������쐬
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();
	spFont->SetUp();

	//�������V�[���O���t�ɓo�^
	g_spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spFont );

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

		spFont->PrintfSentence( 16 , 16 * 1 , graphic::CColor_Tomato , 1.0f , _T( "cameraPos = [ %f %f %f ]" ) , g_spCamera->GetEye().X , g_spCamera->GetEye().Y , g_spCamera->GetEye().Z );
		spFont->PrintfSentence( 16 , 16 * 2 , graphic::CColor_Tomato , 1.0f , _T( "cameraTag = [ %f %f %f ]" ) , g_spCamera->GetTag().X , g_spCamera->GetTag().Y , g_spCamera->GetTag().Z );

		//�J�����̍X�V
		g_spCamera->vUpdate( fpsCtrl.GetPassageTime() );

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

	g_spCamera.Delete();
	g_spScreen.Delete();

	SKY_LIB_GRAPHIC_TERM();
}
#endif


#endif