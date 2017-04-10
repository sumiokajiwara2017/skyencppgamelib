#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_ANIMATION

using namespace sky::lib;

#if OFF_

static const skyString *SCREEN_SETUP_NAME       = _T( "Test" ); 
static const skyString *FONT_SCREEN_SETUP_NAME  = _T( "font" ); 

//Window�����ʒu�A�T�C�Y
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640*2;
static const u32  _WIN_H = 480*2;

//�P�b������̃t���[����
static const u32 _FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//�J����
static graphic::Camera3DFixationPtr  g_spCamera;
static graphic::Camera2DFixationPtr  g_spFontCamera;

//�e�X�g�p���f��
static graphic::CModelPtr     g_TestCone;
static graphic::CModelPtr     g_TestPlane;

//�X�N���[��
static graphic::CScreenObjectPtr    g_spScreen;
static graphic::CScreenObjectPtr    g_spFontScreen;

math::CBasicVector3 m_Transrate( 0 , 1 , 0 );

static void ColValueEventDelegate_Vector3( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args )
{
	SKY_UNUSED_ARG( spSender );
	f32 *time                  = ( f32 * )args.Getu32( 0 );
	math::CBasicVector3 *value = ( math::CBasicVector3 * )args.Getu32( 1 );
	f32 *weight                = ( f32 * )args.Getu32( 2 );

	m_Transrate = *value;
}

TEST( Animation , 1 ) 
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

	GET_BONE( g_TestCone )->SetPosition_XYZ( m_Transrate );

	//���f�����V�[���O���t�ɓo�^
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestCone );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( g_TestPlane );

	//�t�@���N�V�����J�[�u�𐶐�
	kfm::CFCurve< math::CBasicVector3 > fcVector3( m_Transrate );
	kfm::CFCurve< math::CBasicVector3 > fcVector3_next( m_Transrate );
	kfm::CFCurve< math::CBasicVector3 > fcVector3_next2( m_Transrate );

	//�l��ԃX�P�W���[���𗧂Ă�
	fcVector3.AddKey( 1.0f , math::CBasicVector3( 0 , 20 , 0 ) , eInterpolateType_Deceleration ); 
	fcVector3.AddKey( 1.0f , m_Transrate                       , eInterpolateType_Deceleration ); 
	fcVector3.AddKey( 1.0f , math::CBasicVector3( 20 , 1 , 0 ) , eInterpolateType_Deceleration ); 
	fcVector3.AddKey( 1.0f , m_Transrate                       , eInterpolateType_Deceleration );

	fcVector3_next.AddKey( 1.0f , math::CBasicVector3( 0 , 20 , 0 ) , eInterpolateType_Deceleration ); 
	fcVector3_next.AddKey( 1.0f , m_Transrate                       , eInterpolateType_Deceleration ); 

	fcVector3_next2.AddKey( 1.0f , math::CBasicVector3( -20 , 1 , 0 ) , eInterpolateType_Deceleration ); 

	kfm::CFCurvePlayer< math::CBasicVector3 > fcurvePlayer_Vector3;
	fcurvePlayer_Vector3.GetColValueEventHndl() += delegate::IDelegateObjectPtr( NEW delegate::CEventDelegateAdaptorFunc( ColValueEventDelegate_Vector3 ) ); 
	fcurvePlayer_Vector3.PlayFCurveRequest( &fcVector3 , 0 , 0 , 1.0f , 0 , kfm::ePlayEndType_End );
//	fcurvePlayer_Vector3.PlayFCurveRequest( &fcVector3 , 0 , 0 , 1.0f , 0 , kfm::ePlayEndType_Repetition );
//	fcurvePlayer_Vector3.PlayFCurveRequest( &fcVector3 , 0 , 0 , 1.0f , 0 , kfm::ePlayEndType_Back );

	fcurvePlayer_Vector3.PlayFCurveRequest( &fcVector3_next );
	fcurvePlayer_Vector3.PlayFCurveRequest( &fcVector3_next2 );

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

		spFont->PrintfSentence( 32 , 32 , graphic::CColor_Tomato , 1.0f , _T( "DebugPrint." ) );

		//�Đ�
		fcurvePlayer_Vector3.Play( fpsCtrl.GetPassageTime_Second() );

		//�l�𔽉f
		GET_BONE( g_TestCone )->SetPosition_XYZ( m_Transrate );

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

#if OFF_

static const skyString *SCREEN_SETUP_NAME       = _T( "Test" ); 
static const skyString *FONT_SCREEN_SETUP_NAME  = _T( "font" ); 

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
static graphic::CModelPtr     g_TestCone;
static graphic::CModelPtr     g_TestPlane;

//�X�N���[��
static graphic::CScreenObjectPtr    g_spScreen;
static graphic::CScreenObjectPtr    g_spFontScreen;

TEST( Animation , 2 ) 
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
	g_TestCone->GetMesh()->GetMaterial()->SetAlpha( 0.5 );
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

		spFont->PrintfSentence( 32 , 32 , graphic::CColor_Tomato , 1.0f , _T( "DebugPrint." ) );

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

#if OFF_

static const skyString *SCREEN_SETUP_NAME       = _T( "Test" ); 
static const skyString *FONT_SCREEN_SETUP_NAME  = _T( "font" ); 

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
static const u32				_FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//�J����
static graphic::Camera2DFixationPtr  g_spCamera;
static graphic::Camera2DFixationPtr  g_spFontCamera;

//�X�N���[��
static graphic::CScreenObjectPtr    g_spScreen;
static graphic::CScreenObjectPtr    g_spFontScreen;

TEST( Animation , 3 ) 
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

	g_spCamera = Camera2DFixationCreate_();
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

	//�e�N�X�`���ǂݍ���
	graphic::CTexturePtr spTexture0 = CTextureCreateName_( CHSC( TEST1_TEXTURE_PATH ) );
	spTexture0->Load(); //���[�h

	//�e�N�X�`���t���̋�`���쐬
	graphic::CSpritePtr spSprite = graphic::CSpriteUtil::CreateSquare( spTexture0 , graphic::RENDERING_2D_PRIO_FAST , math::CBasicVector2( 2 , 2 ) , graphic::CColor_White );

	//�X�v���C�g���V�[���O���t�ɓo�^
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spSprite );

	//���f���ɃA�j���[�V����������
	graphic::CKeyFrameEntity2DAnimationPtr spAnim = CKeyFrameEntity2DAnimationCreate_();
	spAnim->GetFCurve().AddKey( 2 , math::CEntity2D( math::CBasicVector2( 300 , 300 ) , //�ʒu
	                                                 360 , //��]
		                                             math::CBasicVector2( 0.5 , 0.5 ) ) , eInterpolateType_Deceleration );
	spAnim->Play( 0 , 1.0f , 0.0f , kfm::ePlayEndType_Back );
	GET_BONE( spSprite )->AttachKeyFrameEntity2DAnimation( spAnim );

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

		spFont->PrintfSentence( 32 , 32 , graphic::CColor_Tomato , 1.0f , _T( "DebugPrint." ) );

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

#if OFF_

static const skyString *SCREEN_SETUP_NAME       = _T( "Test" ); 
static const skyString *FONT_SCREEN_SETUP_NAME  = _T( "font" ); 

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
static const u32				_FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//�J����
static graphic::Camera2DFixationPtr  g_spCamera;
static graphic::Camera2DFixationPtr  g_spFontCamera;

//�X�N���[��
static graphic::CScreenObjectPtr    g_spScreen;
static graphic::CScreenObjectPtr    g_spFontScreen;

TEST( Animation , 4 ) 
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

	g_spCamera = Camera2DFixationCreate_();
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

	//�e�N�X�`���ǂݍ���
	graphic::CTexturePtr spTexture0 = CTextureCreateName_( CHSC( TEST1_TEXTURE_PATH ) );
	spTexture0->Load(); //���[�h

	//�e�N�X�`���t���̋�`���쐬
	graphic::CSpritePtr spSprite = graphic::CSpriteUtil::CreateSquare( spTexture0 , graphic::RENDERING_2D_PRIO_FAST , math::CBasicVector2( 2 , 2 ) , graphic::CColor_White );

	//�X�v���C�g�ɃA�j���[�V����������
	graphic::CKeyFrameEntity2DAnimationPtr spAnim = CKeyFrameEntity2DAnimationCreate_();
	spAnim->GetFCurve().AddKey( 2 , math::CEntity2D( math::CBasicVector2( 300 , 300 ) , //�ʒu
	                                                 360 , //��]
		                                             math::CBasicVector2( 0.5 , 0.5 ) ) , eInterpolateType_Deceleration );
	spAnim->Play( 0 , 1.0f , 0.0f , kfm::ePlayEndType_Back );
	GET_BONE( spSprite )->AttachKeyFrameEntity2DAnimation( spAnim );

	//�X�v���C�g�̃}�e���A���ɃA�j���[�V����������
	graphic::CKeyFrameMaterialAnimationPtr spMaterialAnim = CKeyFrameMaterialAnimationCreate_();

	spMaterialAnim->GetFCurve().AddKey( 2 , graphic::CMaterial( 0.0f ) , eInterpolateType_Acceleration );

	spMaterialAnim->Play( 0 , 1.0f , 0 , kfm::ePlayEndType_Back );
	spSprite->GetMesh()->GetMeshGroup()->AttachMaterialAnimation( spMaterialAnim );

	//�X�v���C�g���V�[���O���t�ɓo�^
	spSprite->SetAlpha( skyTrue );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spSprite );

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

		spFont->PrintfSentence( 32 , 32 , graphic::CColor_Tomato , 1.0f , _T( "DebugPrint." ) );

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

#if OFF_

static const skyString *SCREEN_SETUP_NAME       = _T( "Test" ); 
static const skyString *FONT_SCREEN_SETUP_NAME  = _T( "font" ); 

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
static graphic::CModelPtr     g_TestCone;
static graphic::CModelPtr     g_TestPlane;

//�X�N���[��
static graphic::CScreenObjectPtr    g_spScreen;
static graphic::CScreenObjectPtr    g_spFontScreen;

math::CBasicVector3 m_Transrate( 0 , 1 , 0 );

static void ColValueEventDelegate_Vector3( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args )
{
	SKY_UNUSED_ARG( spSender );
	f32 *time                  = ( f32 * )args.Getu32( 0 );
	math::CBasicVector3 *value = ( math::CBasicVector3 * )args.Getu32( 1 );

	m_Transrate = *value;
}

TEST( Animation , 5 ) 
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

//--------3DScreen�̃Z�b�g�A�b�v--------��

//--------�A�j���[�V�����^�X�N�̃Z�b�g�A�b�v-------��

	interp::CInterpolateTask< math::CBasicVector3 > task;
	interp::CInterpolateTaskPlayer player;
	task.m_GetValueEventHndl += delegate::IDelegateObjectPtr( NEW delegate::CEventDelegateAdaptorFunc( ColValueEventDelegate_Vector3 ) ); 
	task.Start( math::CBasicVector3( 0.0f , 0.0f , 0.0f ) , math::CBasicVector3( 0.0f , 10.0f , 0.0f ) , 1 , &player , interp::ePlayEndType_End , eInterpolateType_Linear , 1.0f );

//--------�A�j���[�V�����^�X�N�̃Z�b�g�A�b�v-------��

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

		spFont->PrintfSentence( 32 , 32 , graphic::CColor_Tomato , 1.0f , _T( "DebugPrint." ) );

		//�A�j���[�V�����X�V
		player.Update( fpsCtrl.GetPassageTime_Second() );

		GET_BONE( g_TestCone )->SetPosition_XYZ( m_Transrate );

		//�I������
		if ( Ckeyboard_->IsKeyPress( KEY_LEFT ) )
		{
//			task.Start( math::CBasicVector3( -10.0f , 0.0f , 0.0f ) , 1 , &player );
			task.RequestEndValue( math::CBasicVector3( -20.0f , 0.0f , 0.0f ) , 1.0f , 0.8f );
		}

		if ( Ckeyboard_->IsKeyPress( KEY_RIGHT ) )
		{
//			task.Start( math::CBasicVector3( 10.0f , 0.0f , 0.0f ) , 1 , &player );
			task.RequestEndValue( math::CBasicVector3( 20.0f , 0.0f , 0.0f ) , 1.0f , 0.8f );
		}

		if ( Ckeyboard_->IsKeyPress( KEY_UP ) )
		{
//			task.Start( math::CBasicVector3( 0.0f , 10.0f , 0.0f ) , 1 , &player );
			task.RequestEndValue( math::CBasicVector3( 0.0f , 20.0f , 0.0f ) , 1.0f , 0.8f );
		}

		if ( Ckeyboard_->IsKeyPress( KEY_DOWN ) )
		{
//			task.Start( math::CBasicVector3( 0.0f , -10.0f , 0.0f ) , 1 , &player );
			task.RequestEndValue( math::CBasicVector3( 0.0f , -20.0f , 0.0f ) , 1.0f , 0.8f );
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

	task.End();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#if ON_

static const skyString *SCREEN_SETUP_NAME       = _T( "Test" ); 
static const skyString *FONT_SCREEN_SETUP_NAME  = _T( "font" ); 

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

//�X�N���[��
static graphic::CScreenObjectPtr    g_spScreen;
static graphic::CScreenObjectPtr    g_spFontScreen;

math::CBasicVector3 m_Transrate( 0 , 1 , 0 );

static void ColValueEventDelegate_Vector3( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args )
{
	SKY_UNUSED_ARG( spSender );
	f32 *time                  = ( f32 * )args.Getu32( 0 );
	math::CBasicVector3 *value = ( math::CBasicVector3 * )args.Getu32( 1 );

	m_Transrate = *value;
}

TEST( Animation , 6 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	math::CBasicLine2 line2( math::CBasicVector2( 0 , 0 ) , math::CBasicVector2( 10 , 10 ) );
	math::CBasicVector2 vector2 = line2.Interpolate( 0.5f );

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

//	graphic::CModelPtr spModel = graphic::CModelUtil::CreateTestSphere();
//	graphic::CModelPtr spModel = graphic::CModelUtil::CreateTestCube();
	graphic::CModelPtr spModel = graphic::CModelUtil::CreateTestCone();
	graphic::CModelPtr spModel_Hit = graphic::CModelUtil::CreateTestCone(); GET_BONE( spModel_Hit )->SetPosition_X( 2 ); 
	spModel->ComputeAABB3();
	spModel->ComputeOBB3();
	spModel->ComputeBS();

	graphic::CModelPtr spAABB = graphic::CModelUtil::CreateAABBDisp( spModel->GetAABB3() , graphic::CColor_Mistyrose );
	graphic::CModelPtr spOBB  = graphic::CModelUtil::CreateOBBDisp( spModel->GetOBB3() , graphic::CColor_Mistyrose );
	graphic::CModelPtr spOBS  = graphic::CModelUtil::CreateLine3DSphere( spModel->GetOBS() , graphic::CColor_Mistyrose );

//	SKY_PRINTF( "%f %f %f \n" , spModel->GetAABB3()->Center().X , spModel->GetAABB3()->Center().Y , spModel->GetAABB3()->Center().Z );
//	SKY_PRINTF( "%f %f %f \n" , spModel->GetOBB3()->m_Pos.X , spModel->GetOBB3()->m_Pos.Y , spModel->GetOBB3()->m_Pos.Z );
	SKY_PRINTF( "%f %f %f \n" , spModel->GetOBS().GetPos().X , spModel->GetOBS().GetPos().Y , spModel->GetOBS().GetPos().Z );

	//���f�����V�[���O���t�ɓo�^
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spModel );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spModel_Hit );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spAABB );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spOBB );
	g_spScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spOBS );

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

#if ON_
//-----Line�Ƃ̏Փ�
		dectype distance;
		math::CBasicVector3 pos;
		math::CBasicVector3 vec;
		if ( math::GeometryCollisionUtil::SphereCrossLine3( spModel->GetOBS() , g_spCamera->GetEyeLine( 100 ) ) == skyTrue )
		{
			spFont->PrintfSentence( 32 , 16 , graphic::CColor_Tomato , 1.0f , _T( "Line:Sphere Hit." ) );
		}
		if ( math::GeometryCollisionUtil::AABB3CrossLine3( spModel->GetAABB3() , g_spCamera->GetEyeLine( 100 ) , pos , vec ) == skyTrue )
		{
			spFont->PrintfSentence( 32 , 32 , graphic::CColor_Tomato , 1.0f , _T( "Line:AABB Hit. x=[ %f ] y=[ %f ]" ) , pos.X , pos.Y );
		}
		if ( math::GeometryCollisionUtil::OBB3CrossLine3( spModel->GetOBB3() , g_spCamera->GetEyeLine( 100 ) ) == skyTrue )
		{
			spFont->PrintfSentence( 32 , 48 , graphic::CColor_Tomato , 1.0f , _T( "Line:OBB Hit." ) );
		}
#endif

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