#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_SHADER

#define DISP_TYPE ( 4 )

#include "SkyGraphicLib/DataBase.h"

#if DISP_TYPE == 0

using namespace sky::lib;

//Window�����ʒu�A�T�C�Y
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 1024;
static const u32  _WIN_H = 768;

//�P�b������̃t���[����
static const u32 _FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

//�|�X�g�G�t�F�N�g���g�p
//#define USE_POST_EFFECT

TEST( Shader , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//�t���X�N���[�����[�hOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

	//�J�����̍쐬
	graphic::CameraPtr spSpriteCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spSpriteCamera->SetUp();
	spSpriteCamera->SetOrtho( skyTrue );         //���ˉe

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( _T( "MainScreen" ) );            //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = spSpriteCamera;                       //�J����
	screenData.WindowRect.X()     = _WIN_X;                               //��ʈʒuX
	screenData.WindowRect.Y()     = _WIN_Y;                               //��ʈʒuY
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���

	//�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectPtr spMainScreen = CScreenObject_::Create( screenData );

//------�|�X�g�G�t�F�N�g�X�N���[���I�u�W�F�N�g�֘A

	//�J�����̍쐬
	graphic::CameraPtr spCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spCamera->SetUp();
	spCamera->SetOrtho( skyTrue );         //���ˉe

	//�I�t���C�������_�����O�p�̃X�N���[���̃Z�b�g�A�b�v
	screenData.Name               = CHS( _T( "PostEffectScreen" ) );    //�X�N���[����
	screenData.Camera             = spCamera;                           //�J����
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���

	//�|�X�g�G�t�F�N�g�p�̃X�N���[�����쐬
	graphic::CScreenObjectPtr spPostEffectScreen = spMainScreen->AddChild( screenData );
	spPostEffectScreen->SetReSizeOn( skyFalse );

	//�e�N�X�`���̃��[�h
	graphic::CTexturePtr spTexture1 = graphic::CTexture::Create( CHS( "data/test/posteffect/haruhi.bmp" ) );
	graphic::CTexturePtr spTexture2 = graphic::CTexture::Create( CHS( "data/test/posteffect/lena.dds" ) ); //�����OpenGL�Ńo�O��B���[�_�[�̂���
	spTexture1->Load();
	spTexture2->Load();

	//�X�v���C�g�̍쐬
	graphic::CSpritePtr spSprite1 = graphic::CSpriteUtil::CreateSquare( spTexture1 );
	GET_BONE( spSprite1 )->MulScale_XYZ( 2.0f );
	graphic::CSpritePtr spSprite2 = graphic::CSpriteUtil::CreateSquare( spTexture2 );

	//�X�v���C�g���V�[���O���t�ɓo�^
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spSprite1 );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spSprite2 );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CSpriteUtil::CreateStandardLine2() );

	//�����_�[�^�[�Q�b�g�̍쐬
	graphic::CRenderTargetPtr spSamplerRenderTarget = graphic::CRenderTarget::Create();
	spSamplerRenderTarget->SetUpTexture( math::CBasicSize( _WIN_W , _WIN_H ) );
	spSamplerRenderTarget->SetClearColor( graphic::CColor_White );

	//�V�[���Ƀ����_�[�^�[�Q�b�g��ݒu
	spPostEffectScreen->GetScene()->SetRenderTarget( spSamplerRenderTarget );

	//�|�X�g�G�t�F�N�g���V�[���ɂ�����
	spPostEffectScreen->GetScene()->SetUpPostEffect();

	//�|�X�g�G�t�F�N�g��ς�
	spPostEffectScreen->GetScene()->GetPostEffectManager()->PushPostEffect( CPostEffectTextureMappingCreate_() );
	spPostEffectScreen->GetScene()->GetPostEffectManager()->PushPostEffect( CPostEffectGaussianFilterCreate_() );

//-----�����X�N���[���֘A

	//�����Ǘ��̏������iDirectX11�̏ꍇ�AWindow����������ADevice���������ꂽ��łȂ��Ə������ł��Ȃ�
	CFontManagerInitialize_();

	//�����J����
	graphic::CameraPtr spFontCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spFontCamera->SetUp();                     //�K�{
	spFontCamera->SetOrtho( skyTrue );         //���ˉe
	spFontCamera->SetLeftTopCenter( skyTrue ); //���㌴�_

	//�����X�N���[���̃Z�b�g�A�b�v
	screenData.Name                  = CHS( _T( "FontScreen" ) );        //�X�N���[����
	screenData.Camera                = spFontCamera;                     //�J����
	screenData.IsSetParentWindowSize = skyTrue;

	//�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectPtr spFontScreen = spMainScreen->PushLayer( screenData );

	//�������쐬
	graphic::CFontSimplePtr spFont = CFontSimpleCreate_();
	spFont->SetUp();

	//�������V�[���O���t�ɓo�^
	spFontScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spFont );

//-----�ŏI�`��̂��߂̃X�v���C�g

	//�X�v���C�g�̍쐬
	graphic::CSpritePtr spDisplaySprite = graphic::CSpriteUtil::CreateSquare( spSamplerRenderTarget->GetColorBufferTexture() );

	//�X�v���C�g�����C���̃X�N���[���ɓo�^����
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spDisplaySprite );

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

		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "Shader Test 2D." ) );

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

#if DISP_TYPE == 1

using namespace sky::lib;

//Window�����ʒu�A�T�C�Y
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 1024;
static const u32  _WIN_H = 768;

//�P�b������̃t���[����
static const u32 _FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;


TEST( Shader , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//�t���X�N���[�����[�hOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//------�e�X�N���[��

	//�J�����̍쐬
	graphic::CameraPtr spSpriteCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spSpriteCamera->SetUp();
	spSpriteCamera->SetOrtho( skyTrue );         //���ˉe

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( _T( "MainScreen" ) );            //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = spSpriteCamera;                       //�J����
	screenData.WindowRect.X()     = _WIN_X;                               //��ʈʒuX
	screenData.WindowRect.Y()     = _WIN_Y;                               //��ʈʒuY
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���

	//�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectPtr spMainScreen = CScreenObject_::Create( screenData );

//------�q�X�N���[��

	//�J�����̍쐬
	graphic::CameraPtr spCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spCamera->SetUp();
	spCamera->SetDefaultInput();

	//�I�t���C�������_�����O�p�̃X�N���[���̃Z�b�g�A�b�v
	screenData.Name               = CHS( _T( "PostEffectScreen" ) );    //�X�N���[����
	screenData.Camera             = spCamera;                           //�J����
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���

	//�|�X�g�G�t�F�N�g�p�̃X�N���[�����쐬
	graphic::CScreenObjectPtr spPostEffectScreen = spMainScreen->AddChild( screenData );
	spPostEffectScreen->SetReSizeOn( skyFalse );

	//�����_�[�^�[�Q�b�g�̍쐬
	graphic::CRenderTargetPtr spSamplerRenderTarget = graphic::CRenderTarget::Create();
	spSamplerRenderTarget->SetUpTexture( math::CBasicSize( _WIN_W , _WIN_H ) );
	spSamplerRenderTarget->SetClearColor( graphic::CColor_White );

	//�V�[���Ƀ����_�[�^�[�Q�b�g��ݒu
	spPostEffectScreen->GetScene()->SetRenderTarget( spSamplerRenderTarget );

	//�|�X�g�G�t�F�N�g���V�[���ɂ�����
	spPostEffectScreen->GetScene()->SetUpPostEffect();

	//�|�X�g�G�t�F�N�g��ς�
//	spPostEffectScreen->GetScene()->GetPostEffectManager()->PushPostEffect( CPostEffectTextureMappingCreate_() );
//	spPostEffectScreen->GetScene()->GetPostEffectManager()->PushPostEffect( CPostEffectGaussianFilterCreate_() );

//------�����X�N���[��

	//�����Ǘ��̏������iDirectX11�̏ꍇ�AWindow����������ADevice���������ꂽ��łȂ��Ə������ł��Ȃ�
	CFontManagerInitialize_();

	//�����J����
	graphic::CameraPtr spFontCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spFontCamera->SetUp();                     //�K�{
	spFontCamera->SetOrtho( skyTrue );         //���ˉe
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

//-----------------------------------------------------------------------

	//���[�h�p�X����
	graphic::CGraphicFilePathManager::SetColladaFilePathRoot( CHS( "data/lib/shader/testdata/" ) );
	graphic::CGraphicFilePathManager::SetTextureFilePathRoot( CHS( "data/lib/shader/testdata/" ) );

	graphic::CTexturePtr spTexture1 = graphic::CTexture::Create( CHS( "data/test/posteffect/haruhi.bmp" ) );
//	graphic::CTexturePtr spTexture1 = graphic::CTexture::Create( CHS( "data/test/posteffect/lena.dds" ) );
	spTexture1->Load();
	spTexture1->SetTextureWrapBorder();
	spTexture1->SetTextureWrapBorderColor( graphic::CColor_White );

	//���f���̓ǂݍ���
	graphic::ColladaLoader::LoadFile( CHS( _T( "shaderTestModel1.dae" ) ) );

	graphic::CModelPtr spModel_1 = graphic::CDataBase::GetModel( CHS( "Monkey" ) );
	spModel_1->ActivateTraverse();
	graphic::CModelPtr spModel_2 = graphic::CDataBase::GetModel( CHS( "Sphere" ) );
	spModel_2->ActivateTraverse();
	graphic::CModelPtr spModel_3 = graphic::CDataBase::GetModel( CHS( "Cube" ) );
	spModel_3->ActivateTraverse();
	graphic::CModelPtr spModel_4 = graphic::CDataBase::GetModel( CHS( "Plane" ) );
	spModel_4->ActivateTraverse();
	CDataBase_::Clear();

	spModel_1->ClearAndAttachShader( CPhongShaderObjectCreate_() );
	spModel_2->ClearAndAttachShader( CPhongShaderObjectCreate_() );
	spModel_3->ClearAndAttachShader( CPhongShaderObjectCreate_() );
	spModel_4->ClearAndAttachShader( CPhongShaderObjectCreate_() );

	spModel_1->GetMesh()->GetMaterial()->Reset();
	spModel_2->GetMesh()->GetMaterial()->Reset();
	spModel_3->GetMesh()->GetMaterial()->Reset();
	spModel_4->GetMesh()->GetMaterial()->Reset();

	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spModel_1 );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spModel_2 );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spModel_3 );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spModel_4 );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateStandardLine3() );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateGrid_XZ( 10 ) );

//-----�ŏI�`��̂��߂̃X�v���C�g

	//�X�v���C�g�̍쐬
	graphic::CSpritePtr spDisplaySprite = graphic::CSpriteUtil::CreateSquare( spSamplerRenderTarget->GetColorBufferTexture() );

	//�X�v���C�g�����C���̃X�N���[���ɓo�^����
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spDisplaySprite );

//-----------------------------------------------------------------------


	//FPS�R���g���[���[�쐬
	sky::lib::fps::CFpsController fpsCtrl;

	//�`����L���[�̍쐬
	CRenderCommandManager_::CreateQueue( RENDER_QUEUE_SIZE );

	//�`��p�X���b�h�̍쐬
	CRenderCommandManager_::StartThread();

	fpsCtrl.SetFrameRate( _FRAME_NUM );

	SKY_PRINTF( _T( "===================== < RenderTest Start > ===================== \n" ) );

	//�Q�[�����[�v
	spCamera->SetDistance( 40.0f );
	math::CBasicAngleVector3 cameraAngle( 0 , 0 , 0 , 0 );
	GAME_LOOP_START
	{
		//�t���[���J�n
		fpsCtrl.StartFrame();

		//���͊Ǘ��X�V
		Ckeyboard_->Update( fpsCtrl );

//		degAngle++;
//		cameraAngle.Y().FromDeg( degAngle );
//		spCamera->SetAngle( cameraAngle );

		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "Shader Test 3D." ) );
		spFont->PrintfSentence( 16 , 32 , graphic::CColor_Tomato , 1.0f , _T( "CameraPos( %f %f %f )" ) , spCamera->GetEye().X, spCamera->GetEye().Y , spCamera->GetEye().Z );
		spFont->PrintfSentence( 16 , 48 , graphic::CColor_Tomato , 1.0f , _T( "CameraAngle( %f %f %f )" ) , spCamera->GetAngle().X().ToDeg() , spCamera->GetAngle().Y().ToDeg() , spCamera->GetAngle().Z().ToDeg() );

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

#if DISP_TYPE == 2

using namespace sky::lib;

//Window�����ʒu�A�T�C�Y
static const dectype  _WIN_X = 0;
static const dectype  _WIN_Y = 0;
static const dectype  _WIN_W = 1024;
static const dectype  _WIN_H = 768;

//�P�b������̃t���[����
static const u32 _FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;


TEST( Shader , 2 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//�t���X�N���[�����[�hOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//------�e�X�N���[��

	//�J�����̍쐬
	graphic::CameraPtr spSpriteCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spSpriteCamera->SetUp();
	spSpriteCamera->SetOrtho( skyTrue );         //���ˉe

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( _T( "MainScreen" ) );            //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = spSpriteCamera;                       //�J����
	screenData.WindowRect.X()     = _WIN_X;                               //��ʈʒuX
	screenData.WindowRect.Y()     = _WIN_Y;                               //��ʈʒuY
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���

	//�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectPtr spMainScreen = CScreenObject_::Create( screenData );

//------�q�X�N���[��

	//�J�����̍쐬
	graphic::CameraPtr spCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spCamera->SetUp();
	spCamera->SetDefaultInput();
	spCamera->SetAngle( math::CBasicAngleVector3( DEGtoANG( -151 ) , DEGtoANG( -24 ) , DEGtoANG( 0 ) ) );
	spCamera->SetDistance( 18 );

	//�I�t���C�������_�����O�p�̃X�N���[���̃Z�b�g�A�b�v
	screenData.Name               = CHS( _T( "PostEffectScreen" ) );    //�X�N���[����
	screenData.Camera             = spCamera;                           //�J����
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���

	//�|�X�g�G�t�F�N�g�p�̃X�N���[�����쐬
	graphic::CScreenObjectPtr spPostEffectScreen = spMainScreen->AddChild( screenData );
	spPostEffectScreen->SetReSizeOn( skyFalse );

	//�����_�[�^�[�Q�b�g�̍쐬
	graphic::CRenderTargetPtr spSamplerRenderTarget = graphic::CRenderTarget::Create();
	spSamplerRenderTarget->SetUpTexture( math::CBasicSize( _WIN_W , _WIN_H ) );
	spSamplerRenderTarget->SetClearColor( graphic::CColor_White );

	//�V�[���Ƀ����_�[�^�[�Q�b�g��ݒu
	spPostEffectScreen->GetScene()->SetRenderTarget( spSamplerRenderTarget );

	//�|�X�g�G�t�F�N�g���V�[���ɂ�����
	spPostEffectScreen->GetScene()->SetUpPostEffect();

	//�|�X�g�G�t�F�N�g��ς�
	spPostEffectScreen->GetScene()->GetPostEffectManager()->PushPostEffect( CPostEffectTextureMappingCreate_() );
//	spPostEffectScreen->GetScene()->GetPostEffectManager()->PushPostEffect( CPostEffectGaussianFilterCreate_() );

//------�����X�N���[��

	//�����Ǘ��̏������iDirectX11�̏ꍇ�AWindow����������ADevice���������ꂽ��łȂ��Ə������ł��Ȃ�
	CFontManagerInitialize_();

	//�����J����
	graphic::CameraPtr spFontCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spFontCamera->SetUp();                     //�K�{
	spFontCamera->SetOrtho( skyTrue );         //���ˉe
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

//-----------------------------------------------------------------------

	//���[�h�p�X����
	graphic::CGraphicFilePathManager::SetXFilePathRoot( CHS( "data/test/shadow/" ) );
	graphic::CGraphicFilePathManager::SetTextureFilePathRoot( CHS( "data/test/shadow/" ) );

	//���f���̓ǂݍ���
	graphic::CXFileLoader::LoadFile( CHS( _T( "dosei.x" ) ) );
	graphic::CModelPtr spDoseiModel = graphic::CDataBase::GetModelIndex( 0 );
	spDoseiModel->ActivateTraverse();
	CDataBase_::Clear();

	graphic::CXFileLoader::LoadFile( CHS( _T( "tree.x" ) ) );
	graphic::CModelPtr spTreeModel1 = graphic::CDataBase::GetModelIndex( 0 );
	spTreeModel1->ActivateTraverse();
	graphic::CModelPtr spTreeModel2 = graphic::CDataBase::GetModelIndex( 0 , skyTrue , skyTrue );
	spTreeModel2->ActivateTraverse();
	graphic::CModelPtr spTreeModel3 = graphic::CDataBase::GetModelIndex( 0 , skyTrue , skyTrue );
	spTreeModel3->ActivateTraverse();
	CDataBase_::Clear();

	graphic::CXFileLoader::LoadFile( CHS( _T( "ground.x" ) ) );
	graphic::CModelPtr spGroundModel = graphic::CDataBase::GetModelIndex( 0 );
	spGroundModel->ActivateTraverse();
	CDataBase_::Clear();

	spDoseiModel->SetPosition_Y( spDoseiModel->GetLocalYUnderLength() );

	spTreeModel1->SetPosition_X( 5.0f );
	spTreeModel1->MulScale_XYZ( 3.0f );
	spTreeModel1->SetPosition_Y( spTreeModel1->GetLocalYUnderLength() );

	spTreeModel2->SetPosition_X( -5.0f );
	spTreeModel2->MulScale_XYZ( 3.0f );
	spTreeModel2->SetPosition_Y( spTreeModel2->GetLocalYUnderLength() );

	spTreeModel3->SetPosition_Z( 5.0f );
	spTreeModel3->MulScale_XYZ( 3.0f );
	spTreeModel3->SetPosition_Y( spTreeModel3->GetLocalYUnderLength() );

	spGroundModel->SetScale_XYZ( 16.0f );
	spGroundModel->SetPosition_Y( spGroundModel->GetLocalYUnderLength() );

	//���f���̃V�F�[�_�[��ς��Ă��܂�
	spDoseiModel->ClearAndAttachShader( CMaterialDiffuseColorShaderObjectCreate_() );
	spTreeModel1->ClearAndAttachShader( CMaterialDiffuseColorShaderObjectCreate_() );
	spTreeModel2->ClearAndAttachShader( CMaterialDiffuseColorShaderObjectCreate_() );
	spTreeModel3->ClearAndAttachShader( CMaterialDiffuseColorShaderObjectCreate_() );
	spGroundModel->ClearAndAttachShader( CMaterialDiffuseColorShaderObjectCreate_() );

	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spDoseiModel );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spTreeModel1 );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spTreeModel2 );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spTreeModel3 );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spGroundModel );

	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateStandardLine3() );

//-----�ŏI�`��̂��߂̃X�v���C�g

	//�X�v���C�g�̍쐬
	graphic::CSpritePtr spDisplaySprite = graphic::CSpriteUtil::CreateSquare( spSamplerRenderTarget->GetColorBufferTexture() );

	//�X�v���C�g�����C���̃X�N���[���ɓo�^����
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spDisplaySprite );

//-----------------------------------------------------------------------

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

		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "Shader Test 3D." ) );
		spFont->PrintfSentence( 16 , 32 , graphic::CColor_Tomato , 1.0f , _T( "CameraPos( %f %f %f )" ) , spCamera->GetData().m_Eye.X, spCamera->GetData().m_Eye.Y , spCamera->GetData().m_Eye.Z );
		spFont->PrintfSentence( 16 , 48 , graphic::CColor_Tomato , 1.0f , _T( "CameraAngle( %f %f %f )" ) , spCamera->GetData().m_Angle.X().ToDeg() , spCamera->GetData().m_Angle.Y().ToDeg() , spCamera->GetData().m_Angle.Z().ToDeg() );
		spFont->PrintfSentence( 16 , 64 , graphic::CColor_Tomato , 1.0f , _T( "CameraTargetDistance( %f )" ) , spCamera->GetData().m_Distance );

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


#if DISP_TYPE == 3

using namespace sky::lib;

//Window�����ʒu�A�T�C�Y
static const dectype  _WIN_X = 0;
static const dectype  _WIN_Y = 0;
static const dectype  _WIN_W = 1024;
static const dectype  _WIN_H = 1024;

//�P�b������̃t���[����
static const u32 _FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;


TEST( Shader , 3 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//�t���X�N���[�����[�hOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//------�e�X�N���[��

	//�J�����̍쐬
	graphic::CameraPtr spCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spCamera->SetDefaultInput();
	spCamera->SetAngle( math::CBasicAngleVector3( DEGtoANG( -151 ) , DEGtoANG( -24 ) , DEGtoANG( 0 ) ) );
	spCamera->SetDistance( 18 );

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( _T( "MainScreen" ) );            //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = spCamera;                             //�J����
	screenData.WindowRect.X()     = _WIN_X;                               //��ʈʒuX
	screenData.WindowRect.Y()     = _WIN_Y;                               //��ʈʒuY
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���

	//�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectPtr spMainScreen = CScreenObject_::Create( screenData );

//------�q�X�N���[��

	//�I�t���C�������_�����O�p�̃X�N���[���̃Z�b�g�A�b�v
	screenData.Name               = CHS( _T( "ShadowMapScreen" ) );       //�X�N���[����
	screenData.Camera             = spCamera;                             //�J����
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���

	//�V���h�E�}�b�v�p�̃X�N���[�����쐬
	graphic::CScreenObjectPtr spShadowMapScreen = spMainScreen->AddChild( screenData );

	//�V���h�E�}�b�v�p�����_�[�^�[�Q�b�g�̍쐬
	graphic::CRenderTargetPtr spSamplerRenderTarget = graphic::CRenderTarget::Create();
	spSamplerRenderTarget->SetUpTexture( math::CBasicSize( _WIN_W , _WIN_H ) );
	spSamplerRenderTarget->SetClearColor( graphic::CColor_White );
	spShadowMapScreen->GetScene()->SetRenderTarget( spSamplerRenderTarget );

//------���C���[�X�N���[��

	//�����Ǘ��̏������iDirectX11�̏ꍇ�AWindow����������ADevice���������ꂽ��łȂ��Ə������ł��Ȃ�
	CFontManagerInitialize_();

	//�����J����
	graphic::CameraPtr spFontCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spFontCamera->SetOrtho( skyTrue );         //���ˉe
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

//-----------------------------------------------------------------------

	//���[�h�p�X����
	graphic::CGraphicFilePathManager::SetXFilePathRoot( CHS( "data/test/shadow/" ) );
	graphic::CGraphicFilePathManager::SetTextureFilePathRoot( CHS( "data/test/shadow/" ) );

	//���f���̓ǂݍ���
	graphic::CXFileLoader::LoadFile( CHS( _T( "dosei.x" ) ) );
	graphic::CModelPtr spDoseiModel = graphic::CDataBase::GetModelIndex( 0 );
	spDoseiModel->ActivateTraverse();
	CDataBase_::Clear();

	graphic::CXFileLoader::LoadFile( CHS( _T( "tree.x" ) ) );
	graphic::CModelPtr spTreeModel1 = graphic::CDataBase::GetModelIndex( 0 );
	spTreeModel1->ActivateTraverse();
	graphic::CModelPtr spTreeModel2 = graphic::CDataBase::GetModelIndex( 0 , skyTrue , skyTrue );
	spTreeModel2->ActivateTraverse();
	graphic::CModelPtr spTreeModel3 = graphic::CDataBase::GetModelIndex( 0 , skyTrue , skyTrue );
	spTreeModel3->ActivateTraverse();
	CDataBase_::Clear();

	graphic::CXFileLoader::LoadFile( CHS( _T( "ground.x" ) ) );
	graphic::CModelPtr spGroundModel = graphic::CDataBase::GetModelIndex( 0 );
	spGroundModel->ActivateTraverse();
	CDataBase_::Clear();

	spDoseiModel->SetPosition_Y( spDoseiModel->GetLocalYUnderLength() );

	spTreeModel1->SetPosition_X( 5.0f );
	spTreeModel1->MulScale_XYZ( 3.0f );
	spTreeModel1->SetPosition_Y( spTreeModel1->GetLocalYUnderLength() );

	spTreeModel2->SetPosition_X( -5.0f );
	spTreeModel2->MulScale_XYZ( 3.0f );
	spTreeModel2->SetPosition_Y( spTreeModel2->GetLocalYUnderLength() );

	spTreeModel3->SetPosition_Z( 5.0f );
	spTreeModel3->MulScale_XYZ( 3.0f );
	spTreeModel3->SetPosition_Y( spTreeModel3->GetLocalYUnderLength() );

	spGroundModel->SetScale_XYZ( 16.0f );
	spGroundModel->SetPosition_Y( spGroundModel->GetLocalYUnderLength() );

	//�V���h�E�}�b�v�쐬�p�V�F�[�_�[�쐬
	graphic::CPerspectiveShadowMapCreateShaderObjectPtr spShadowMapCreate = CPerspectiveShadowMapCreateShaderObjectCreate_();

	//�V���h�E�}�b�v�g�p�`��p�V�F�[�_�[�쐬
	graphic::CPerspectiveShadowMapDrawShaderObjectPtr spShadowMapDraw = CPerspectiveShadowMapDrawShaderObjectCreate_();

	//�e���o�����C�g�̈ʒu��ݒ�
	spShadowMapCreate->SetShadowLightPosition( math::CBasicVector4( 0.0f, 10.0f, 10.0f, 0.0f ) );

	//�֘A�t����
	spShadowMapDraw->AttachShadowMapCreater( spShadowMapCreate );

	//�V���h�E�}�b�v��ݒ肷��
	spShadowMapDraw->AttachShadowMapRenderTarget( spSamplerRenderTarget );

	//���f�����[�g��n��
	graphic::CSceneGraphNodePtr spModelRoot = CSceneGraphNodeCreate_(); 
	spModelRoot->SetJointUse();

	//�V�F�[�_�[�Ƀ��[�g���Ԃ牺����
	spShadowMapDraw->AttachChild( spModelRoot );
	spShadowMapCreate->AttachChild( spModelRoot );

	//���f�������[�g�ɂԂ牺����
	spModelRoot->AttachChild( spDoseiModel );
	spModelRoot->AttachChild( spTreeModel1 );
	spModelRoot->AttachChild( spTreeModel2 );
	spModelRoot->AttachChild( spTreeModel3 );

	//�������͕\���̎������ɂ���
	spShadowMapDraw->AttachChild( spGroundModel );

	//�e�X�N���[���̃��[�g�ɂԂ牺����
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spShadowMapDraw );
	spShadowMapScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spShadowMapCreate );

//-----------------------------------------------------------------------

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

		spModelRoot->MulRotation_Y( math::CMathUtil::DegToRad( 2 ) );
		spGroundModel->MulRotation_Y( math::CMathUtil::DegToRad( 2 ) );

		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "Shader Test 3D." ) );
		spFont->PrintfSentence( 16 , 32 , graphic::CColor_Tomato , 1.0f , _T( "CameraPos( %f %f %f )" ) , spCamera->GetData().m_Eye.X, spCamera->GetData().m_Eye.Y , spCamera->GetData().m_Eye.Z );
		spFont->PrintfSentence( 16 , 48 , graphic::CColor_Tomato , 1.0f , _T( "CameraAngle( %f %f %f )" ) , spCamera->GetData().m_Angle.X().ToDeg() , spCamera->GetData().m_Angle.Y().ToDeg() , spCamera->GetData().m_Angle.Z().ToDeg() );
		spFont->PrintfSentence( 16 , 64 , graphic::CColor_Tomato , 1.0f , _T( "CameraTargetDistance( %f )" ) , spCamera->GetData().m_Distance );

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

#if DISP_TYPE == 4

using namespace sky::lib;

//Window�����ʒu�A�T�C�Y
static const dectype  _WIN_X = 0;
static const dectype  _WIN_Y = 0;
static const dectype  _WIN_W = 1024;
static const dectype  _WIN_H = 768;

//�P�b������̃t���[����
static const u32 _FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

static void printfMatrix( const math::CMatrix4x3 &a , char *pMark , char *in )
{
	MyOutputDebugStringA( pMark );
	MyOutputDebugStringA( "%s.M11 = %f; \n" , in , a.M11 );
	MyOutputDebugStringA( "%s.M12 = %f; \n" , in , a.M12 );
	MyOutputDebugStringA( "%s.M13 = %f; \n" , in , a.M13 );
	MyOutputDebugStringA( "%s.M14 = %f; \n" , in , a.M14 );
	MyOutputDebugStringA( "%s.M21 = %f; \n" , in , a.M21 );
	MyOutputDebugStringA( "%s.M22 = %f; \n" , in , a.M22 );
	MyOutputDebugStringA( "%s.M23 = %f; \n" , in , a.M23 );
	MyOutputDebugStringA( "%s.M24 = %f; \n" , in , a.M24 );
	MyOutputDebugStringA( "%s.M31 = %f; \n" , in , a.M31 );
	MyOutputDebugStringA( "%s.M32 = %f; \n" , in , a.M32 );
	MyOutputDebugStringA( "%s.M33 = %f; \n" , in , a.M33 );
	MyOutputDebugStringA( "%s.M34 = %f; \n" , in , a.M34 );
	MyOutputDebugStringA( "%s.Tx = %f; \n" , in , a.Tx );
	MyOutputDebugStringA( "%s.Ty = %f; \n" , in , a.Ty );
	MyOutputDebugStringA( "%s.Tz = %f; \n" , in , a.Tz );
	MyOutputDebugStringA( "%s.Tw = %f; \n" , in , a.Tw );
}

TEST( Loader , 2 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//�t���X�N���[�����[�hOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//------�e�X�N���[��

	//�J�����̍쐬
	graphic::CameraPtr spSpriteCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spSpriteCamera->SetOrtho( skyTrue );         //���ˉe

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( _T( "MainScreen" ) );            //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = spSpriteCamera;                       //�J����
	screenData.WindowRect.X()     = _WIN_X;                               //��ʈʒuX
	screenData.WindowRect.Y()     = _WIN_Y;                               //��ʈʒuY
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���

	//�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectPtr spMainScreen = CScreenObject_::Create( screenData );

//------�q�X�N���[��

	//�J�����̍쐬
	graphic::CameraPtr spCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spCamera->GetData().m_Far = 100000.0f;
	spCamera->SetDefaultInput();
	spCamera->SetAngle( math::CBasicAngleVector3( DEGtoANG( -179 ) , DEGtoANG( -17 ) , DEGtoANG( 0 ) ) );
	spCamera->SetDistance( 728 );

	//�I�t���C�������_�����O�p�̃X�N���[���̃Z�b�g�A�b�v
	screenData.Name               = CHS( _T( "PostEffectScreen" ) );    //�X�N���[����
	screenData.Camera             = spCamera;                           //�J����
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���

	//�|�X�g�G�t�F�N�g�p�̃X�N���[�����쐬
	graphic::CScreenObjectPtr spPostEffectScreen = spMainScreen->AddChild( screenData );
	spPostEffectScreen->SetReSizeOn( skyFalse );

	//�����_�[�^�[�Q�b�g�̍쐬
	graphic::CRenderTargetPtr spSamplerRenderTarget = graphic::CRenderTarget::Create();
	spSamplerRenderTarget->SetUpTexture( math::CBasicSize( _WIN_W , _WIN_H ) );
	spSamplerRenderTarget->SetClearColor( graphic::CColor_White );

	//�V�[���Ƀ����_�[�^�[�Q�b�g��ݒu
	spPostEffectScreen->GetScene()->SetRenderTarget( spSamplerRenderTarget );

	//�|�X�g�G�t�F�N�g���V�[���ɂ�����
	spPostEffectScreen->GetScene()->SetUpPostEffect();

	//�|�X�g�G�t�F�N�g��ς�
//	spPostEffectScreen->GetScene()->GetPostEffectManager()->PushPostEffect( CPostEffectTextureMappingCreate_() );
//	spPostEffectScreen->GetScene()->GetPostEffectManager()->PushPostEffect( CPostEffectGaussianFilterCreate_() );

//------�����X�N���[��

	//�����Ǘ��̏������iDirectX11�̏ꍇ�AWindow����������ADevice���������ꂽ��łȂ��Ə������ł��Ȃ�
	CFontManagerInitialize_();

	//�����J����
	graphic::CameraPtr spFontCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spFontCamera->SetOrtho( skyTrue );         //���ˉe
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

//-----------------------------------------------------------------------

	//���[�h�p�X����
	graphic::CGraphicFilePathManager::SetXFilePathRoot( CHS( "data/test/skinmesh/" ) );
	graphic::CGraphicFilePathManager::SetTextureFilePathRoot( CHS( "data/test/skinmesh/" ) );

	//���f���̓ǂݍ���
#define XFileType 3
#if XFileType == 1
	graphic::CXFileLoader::LoadFile( CHS( _T( "tiny.x" ) ) );
	spCamera->SetDistance( 728 );
#elif XFileType == 2 
	graphic::CXFileLoader::LoadFile( CHS( _T( "tiny_4anim.x" ) ) );
	spCamera->SetDistance( 728 );
#elif XFileType == 3 
	graphic::CXFileLoader::LoadFile( CHS( _T( "hand_tex.x" ) ) );
	spCamera->SetDistance( 20 );
#elif XFileType == 4 
	graphic::CXFileLoader::LoadFile( CHS( _T( "hand.x" ) ) );
	spCamera->SetDistance( 20 );
#endif
	graphic::CModelPtr spModel_1 = graphic::CDataBase::GetModelIndex( 0 );
	spModel_1->ActivateTraverse();

//	spModel_1->ClearAndAttachShader( CTextureMappingShaderType2ObjectCreate_() );
	spModel_1->ClearAndAttachShader( CSkinMeshAnimationShaderObjectCreate_() );

	graphic::CModelPtr spLine3 = graphic::CModelUtil::CreateStandardLine3( 300.0f );
//	spLine3->SetRotation_X( math::CMathUtil::DegToRad( 45 ) );

	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spModel_1 );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spLine3 );

//-----�ŏI�`��̂��߂̃X�v���C�g

	//�X�v���C�g�̍쐬
	graphic::CSpritePtr spDisplaySprite = graphic::CSpriteUtil::CreateSquare( spSamplerRenderTarget->GetColorBufferTexture() );

	//�X�v���C�g�����C���̃X�N���[���ɓo�^����
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spDisplaySprite );

//-----------------------------------------------------------------------

	//�A�j���[�V�����̐ݒ�
	math::CMotionController motionController;
	motionController.AttachSkeleton( spModel_1->GetMeshContena()->GetMesh()->GetSkeleton() );
	motionController.SetAnimationContena( graphic::CDataBase::GetMotionDataContenaIndex( 0 ) );

#if XFileType == 1
	math::CJointPtr spJoint = spModel_1->GetMeshContena()->GetMesh()->GetSkeleton()->m_spJointPallet->GetJoint( CHS( "Bip01_Head" ) );   //��
//	math::CJointPtr spJoint = spModel_1->GetMeshContena()->GetMesh()->GetSkeleton()->m_spJointPallet->GetJoint( CHS( "Bip01_Spine1" ) ); //��
//	math::CJointPtr spJoint = spModel_1->GetMeshContena()->GetMesh()->GetSkeleton()->m_spJointPallet->GetJoint( CHS( "Bip01_Pelvis" ) ); //�S�g
//	math::CJointPtr spJoint = spModel_1->GetMeshContena()->GetMesh()->GetSkeleton()->m_spJointPallet->GetJoint( CHS( "Bip01_L_Foot" ) ); //������
//	math::CJointPtr spJoint = spModel_1->GetMeshContena()->GetMesh()->GetSkeleton()->m_spJointPallet->GetJoint( CHS( "Bip01_R_Foot" ) ); //�E����
	spJoint->MulScale_XYZ( 2.0f ); //����傫���i���ʂ��ł����Ȃ�j
	spJoint->SetRotation_X( math::CMathUtil::DegToRad( 60 ) ); //������]�i���ʂ���]����j
	spJoint->SetPosition_X( 60 ); //���𒷂��i���ʂ��L�т�j

	motionController.Play( 0 );
	motionController.Update( 0.1333f );
	graphic::CMotionJointDebugDisplayNodePtr spMotionDebug = CMotionJointDebugDisplayNodeCreate_( motionController );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spMotionDebug );
	spMotionDebug->SetDrawOn( skyTrue );
#elif XFileType == 2 
	motionController.Play( 3 );
	motionController.Update( 0.1333f );
	graphic::CMotionJointDebugDisplayNodePtr spMotionDebug = CMotionJointDebugDisplayNodeCreate_( motionController );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spMotionDebug );
	spMotionDebug->SetDrawOn( skyTrue );
#elif XFileType == 3 
	motionController.Play( 0 );
	motionController.Update( 0.1333f );
	graphic::CMotionJointDebugDisplayNodePtr spMotionDebug = CMotionJointDebugDisplayNodeCreate_( motionController );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spMotionDebug );
	spMotionDebug->SetBaseScale( math::CBasicVector3( 1 , 2 , 1 ) );
	spMotionDebug->SetBaseRot( math::eAxis_x , math::CMathUtil::DegToRad( 90 ) );
	spMotionDebug->SetDrawOn( skyTrue );
#elif XFileType == 4 
	motionController.Play( 0 );
	motionController.Update( 0.1333f , skyFalse );
#endif


	//�f�[�^�x�[�X�̃N���A
	CDataBase_::Clear();

	//FPS�R���g���[���[�쐬
	sky::lib::fps::CFpsController fpsCtrl;

	//�`����L���[�̍쐬
	CRenderCommandManager_::CreateQueue( RENDER_QUEUE_SIZE );

	//�`��p�X���b�h�̍쐬
	CRenderCommandManager_::StartThread();

	fpsCtrl.SetFrameRate( _FRAME_NUM );

	SKY_PRINTF( _T( "===================== < RenderTest Start > ===================== \n" ) );

	GAME_LOOP_START
	{
		//�t���[���J�n
		fpsCtrl.StartFrame();

		//���͊Ǘ��X�V
		Ckeyboard_->Update( fpsCtrl );

		//�A�j���[�V�����̍X�V
		updateTime time = fpsCtrl.GetPassageTime_Second() * motionController.GetCurrentAnimationData()->GetMaxTime();


#if XFileType == 1
		motionController.Update( time );
#elif XFileType == 2 
		motionController.Update( time );
#elif XFileType == 3 
		motionController.Update( time );
#elif XFileType == 4 
		motionController.Update( time );
#endif

		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "Shader Test 3D." ) );
		spFont->PrintfSentence( 16 , 32 , graphic::CColor_Tomato , 1.0f , _T( "CameraPos( %f %f %f )" ) , spCamera->GetData().m_Eye.X, spCamera->GetData().m_Eye.Y , spCamera->GetData().m_Eye.Z );
		spFont->PrintfSentence( 16 , 48 , graphic::CColor_Tomato , 1.0f , _T( "CameraAngle( %f %f %f )" ) , spCamera->GetData().m_Angle.X().ToDeg() , spCamera->GetData().m_Angle.Y().ToDeg() , spCamera->GetData().m_Angle.Z().ToDeg() );
		spFont->PrintfSentence( 16 , 64 , graphic::CColor_Tomato , 1.0f , _T( "CameraDistance( %f )" ) , spCamera->GetData().m_Distance );

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

#if DISP_TYPE == 5

using namespace sky::lib;

//Window�����ʒu�A�T�C�Y
static const dectype  _WIN_X = 0;
static const dectype  _WIN_Y = 0;
static const dectype  _WIN_W = 1024;
static const dectype  _WIN_H = 1024;

//�P�b������̃t���[����
static const u32 _FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

TEST( Shader , 5 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//�t���X�N���[�����[�hOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//------�e�X�N���[��

	//�J�����̍쐬
	graphic::CameraPtr spCamera = CameraCreate_( graphic::eCameraKind_Satellite );
//	spCamera->SetDefaultInput();
	spCamera->SetAngle( math::CBasicAngleVector3( DEGtoANG( 180 ) , DEGtoANG( -0 ) , DEGtoANG( 0 ) ) );
	spCamera->SetDistance( 50 );

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( _T( "MainScreen" ) );            //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = spCamera;                             //�J����
	screenData.WindowRect.X()     = _WIN_X;                               //��ʈʒuX
	screenData.WindowRect.Y()     = _WIN_Y;                               //��ʈʒuY
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���

	//�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectPtr spMainScreen = CScreenObject_::Create( screenData );

//------���C���[�X�N���[��

	//�����Ǘ��̏������iDirectX11�̏ꍇ�AWindow����������ADevice���������ꂽ��łȂ��Ə������ł��Ȃ�
	CFontManagerInitialize_();

	//�����J����
	graphic::CameraPtr spFontCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spFontCamera->SetOrtho( skyTrue );         //���ˉe
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

//-----------------------------------------------------------------------

	//���[�h�p�X����
	graphic::CGraphicFilePathManager::SetXFilePathRoot( CHS( "data/test/ik/" ) );
	graphic::CGraphicFilePathManager::SetTextureFilePathRoot( CHS( "data/test/ik/" ) );

	//���f���̓ǂݍ���
	graphic::CXFileLoader::LoadFile( CHS( _T( "sphere400.x" ) ) );
	graphic::CModelPtr spLink0Model = graphic::CDataBase::GetModelIndex( 0 );
	spLink0Model->ActivateTraverse();
	graphic::CModelPtr spLink1Model = graphic::CDataBase::GetModelIndex( 0 , skyTrue , skyTrue );
	spLink1Model->ActivateTraverse();
	graphic::CModelPtr spLink2Model = graphic::CDataBase::GetModelIndex( 0 , skyTrue , skyTrue );
	spLink2Model->ActivateTraverse();
	graphic::CModelPtr spLink3Model = graphic::CDataBase::GetModelIndex( 0 , skyTrue , skyTrue );
	spLink3Model->ActivateTraverse();
	graphic::CModelPtr spLink4Model = graphic::CDataBase::GetModelIndex( 0 , skyTrue , skyTrue );
	spLink4Model->ActivateTraverse();
	graphic::CModelPtr spLink5Model = graphic::CDataBase::GetModelIndex( 0 , skyTrue , skyTrue );
	spLink5Model->ActivateTraverse();
	graphic::CModelPtr spEffector = graphic::CDataBase::GetModelIndex( 0 , skyTrue , skyTrue );
	spEffector->ActivateTraverse();
	CDataBase_::Clear();

	spLink0Model->AttachChild( spLink1Model , math::CBasicVector3( 0 , 2 , 0 ) );
	spLink1Model->AttachChild( spLink2Model , math::CBasicVector3( 0 , 2 , 0 ) );
	spLink2Model->AttachChild( spLink3Model , math::CBasicVector3( 0 , 2 , 0 ) );
	spLink3Model->AttachChild( spLink4Model , math::CBasicVector3( 0 , 2 , 0 ) );
	spLink4Model->AttachChild( spLink5Model , math::CBasicVector3( 0 , 2 , 0 ) );

	spLink0Model->SetDiffuse( graphic::CColor_Gold );
	spLink1Model->SetDiffuse( graphic::CColor_Green );
	spLink2Model->SetDiffuse( graphic::CColor_Green );
	spLink3Model->SetDiffuse( graphic::CColor_Green );
	spLink4Model->SetDiffuse( graphic::CColor_Green );
	spLink5Model->SetDiffuse( graphic::CColor_Green );
	spEffector->SetDiffuse( graphic::CColor_Red );

	spLink0Model->SetComputeOffsetMatrixToLocalMatrixOn( skyTrue );
	spLink1Model->SetComputeOffsetMatrixToLocalMatrixOn( skyTrue );
	spLink2Model->SetComputeOffsetMatrixToLocalMatrixOn( skyTrue );
	spLink3Model->SetComputeOffsetMatrixToLocalMatrixOn( skyTrue );
	spLink4Model->SetComputeOffsetMatrixToLocalMatrixOn( skyTrue );
	spLink5Model->SetComputeOffsetMatrixToLocalMatrixOn( skyTrue );

	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spLink0Model );
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spEffector );
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateGrid_XZ( 10.0f ) );
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateStandardLine3() );

	spLink0Model->SetNodeTreePrint( skyTrue );

	math::CSkeletonPtr spSkeleton = CSkeletonCreate_( spLink0Model );

//-----------------------------------------------------------------------

	//FPS�R���g���[���[�쐬
	sky::lib::fps::CFpsController fpsCtrl;

	//�`����L���[�̍쐬
	CRenderCommandManager_::CreateQueue( RENDER_QUEUE_SIZE );

	//�`��p�X���b�h�̍쐬
	CRenderCommandManager_::StartThread();

	fpsCtrl.SetFrameRate( _FRAME_NUM );

	SKY_PRINTF( _T( "===================== < RenderTest Start > ===================== \n" ) );

	math::CBasicVector3 effectorPos;
	effectorPos.X = 0;
	effectorPos.Y = -12;
	effectorPos.Z = 0;

	//�Q�[�����[�v
	GAME_LOOP_START
	{
		//�t���[���J�n
		fpsCtrl.StartFrame();

		//���͊Ǘ��X�V
		Ckeyboard_->Update( fpsCtrl );

#define SPEED ( 0.3f )

		//XY���ʂŃG�t�F�N�^�𑀍�
		if ( Ckeyboard_->IsKeyOn( KEY_LEFT ) )
		{
			effectorPos.X += -SPEED;
		}
		else if ( Ckeyboard_->IsKeyOn( KEY_RIGHT ) )
		{
			effectorPos.X += SPEED;
		}
		else if ( Ckeyboard_->IsKeyOn( KEY_UP ) )
		{
			effectorPos.Y += SPEED;
		}
		else if ( Ckeyboard_->IsKeyOn( KEY_DOWN ) )
		{
			effectorPos.Y += -SPEED;
		}

		spEffector->SetPosition_XYZ( effectorPos );

		//IK
//		math::InverseKinematicsUtil::SolveCcd( spSkeleton , spSkeleton->m_spJointPallet->GetJoint( spSkeleton->m_spJointPallet->GetJointNum() - 1 ) , effectorPos , 5, 1.0e-4f );
//		math::CBasicVector3List posList;
//		math::InverseKinematicsUtil::SolveParticle( posList , spSkeleton , spSkeleton->m_spJointPallet->GetJoint( spSkeleton->m_spJointPallet->GetJointNum() - 1 ) , effectorPos, 5 );
		math::InverseKinematicsUtil::SolveEulerJacobian( spSkeleton , spSkeleton->m_spJointPallet->GetJoint( spSkeleton->m_spJointPallet->GetJointNum() - 1 ) , effectorPos,  10, 1.0f );

		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "Shader Test 3D." ) );
		spFont->PrintfSentence( 16 , 32 , graphic::CColor_Tomato , 1.0f , _T( "CameraPos( %f %f %f )" ) , spCamera->GetData().m_Eye.X, spCamera->GetData().m_Eye.Y , spCamera->GetData().m_Eye.Z );
		spFont->PrintfSentence( 16 , 48 , graphic::CColor_Tomato , 1.0f , _T( "CameraAngle( %f %f %f )" ) , spCamera->GetData().m_Angle.X().ToDeg() , spCamera->GetData().m_Angle.Y().ToDeg() , spCamera->GetData().m_Angle.Z().ToDeg() );
		spFont->PrintfSentence( 16 , 64 , graphic::CColor_Tomato , 1.0f , _T( "CameraTargetDistance( %f )" ) , spCamera->GetData().m_Distance );

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

#if DISP_TYPE == 6

using namespace sky::lib;

//Window�����ʒu�A�T�C�Y
static const dectype  _WIN_X = 0;
static const dectype  _WIN_Y = 0;
static const dectype  _WIN_W = 1024;
static const dectype  _WIN_H = 1024;

//�P�b������̃t���[����
static const u32 _FRAME_NUM = 60;

static const u32  RENDER_QUEUE_SIZE = 3;

TEST( Shader , 5 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//�t���X�N���[�����[�hOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

//------�e�X�N���[��

	//�J�����̍쐬
	graphic::CameraPtr spCamera = CameraCreate_( graphic::eCameraKind_Satellite );
//	spCamera->SetDefaultInput();
	spCamera->SetAngle( math::CBasicAngleVector3( DEGtoANG( 180 ) , DEGtoANG( -0 ) , DEGtoANG( 0 ) ) );
	spCamera->SetDistance( 50 );

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( _T( "MainScreen" ) );            //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = spCamera;                             //�J����
	screenData.WindowRect.X()     = _WIN_X;                               //��ʈʒuX
	screenData.WindowRect.Y()     = _WIN_Y;                               //��ʈʒuY
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���

	//�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectPtr spMainScreen = CScreenObject_::Create( screenData );

//------���C���[�X�N���[��

	//�����Ǘ��̏������iDirectX11�̏ꍇ�AWindow����������ADevice���������ꂽ��łȂ��Ə������ł��Ȃ�
	CFontManagerInitialize_();

	//�����J����
	graphic::CameraPtr spFontCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spFontCamera->SetOrtho( skyTrue );         //���ˉe
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

//-----------------------------------------------------------------------

	//���[�h�p�X����
	graphic::CGraphicFilePathManager::SetXFilePathRoot( CHS( "data/test/ik/" ) );
	graphic::CGraphicFilePathManager::SetTextureFilePathRoot( CHS( "data/test/ik/" ) );

	//���f���̓ǂݍ���
	graphic::CXFileLoader::LoadFile( CHS( _T( "sphere400.x" ) ) );
	graphic::CModelPtr spLink0Model = graphic::CDataBase::GetModelIndex( 0 );
	spLink0Model->ActivateTraverse();
	graphic::CModelPtr spLink1Model = graphic::CDataBase::GetModelIndex( 0 , skyTrue , skyTrue );
	spLink1Model->ActivateTraverse();
	graphic::CModelPtr spLink2Model = graphic::CDataBase::GetModelIndex( 0 , skyTrue , skyTrue );
	spLink2Model->ActivateTraverse();
	graphic::CModelPtr spLink3Model = graphic::CDataBase::GetModelIndex( 0 , skyTrue , skyTrue );
	spLink3Model->ActivateTraverse();
	graphic::CModelPtr spLink4Model = graphic::CDataBase::GetModelIndex( 0 , skyTrue , skyTrue );
	spLink4Model->ActivateTraverse();
	graphic::CModelPtr spLink5Model = graphic::CDataBase::GetModelIndex( 0 , skyTrue , skyTrue );
	spLink5Model->ActivateTraverse();
	graphic::CModelPtr spEffector = graphic::CDataBase::GetModelIndex( 0 , skyTrue , skyTrue );
	spEffector->ActivateTraverse();
	CDataBase_::Clear();

	spLink0Model->AttachChild( spLink1Model , math::CBasicVector3( 0 , 2 , 0 ) );
	spLink1Model->AttachChild( spLink2Model , math::CBasicVector3( 0 , 2 , 0 ) );
	spLink2Model->AttachChild( spLink3Model , math::CBasicVector3( 0 , 2 , 0 ) );
	spLink3Model->AttachChild( spLink4Model , math::CBasicVector3( 0 , 2 , 0 ) );
	spLink4Model->AttachChild( spLink5Model , math::CBasicVector3( 0 , 2 , 0 ) );

	spLink0Model->SetDiffuse( graphic::CColor_Gold );
	spLink1Model->SetDiffuse( graphic::CColor_Green );
	spLink2Model->SetDiffuse( graphic::CColor_Green );
	spLink3Model->SetDiffuse( graphic::CColor_Green );
	spLink4Model->SetDiffuse( graphic::CColor_Green );
	spLink5Model->SetDiffuse( graphic::CColor_Green );
	spEffector->SetDiffuse( graphic::CColor_Red );

	spLink0Model->SetComputeOffsetMatrixToLocalMatrixOn( skyTrue );
	spLink1Model->SetComputeOffsetMatrixToLocalMatrixOn( skyTrue );
	spLink2Model->SetComputeOffsetMatrixToLocalMatrixOn( skyTrue );
	spLink3Model->SetComputeOffsetMatrixToLocalMatrixOn( skyTrue );
	spLink4Model->SetComputeOffsetMatrixToLocalMatrixOn( skyTrue );
	spLink5Model->SetComputeOffsetMatrixToLocalMatrixOn( skyTrue );

	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spLink0Model );
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spEffector );
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateGrid_XZ( 10.0f ) );
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateStandardLine3() );

	spLink0Model->SetNodeTreePrint( skyTrue );

	math::CSkeletonPtr spSkeleton = CSkeletonCreate_( spLink0Model );

//-----------------------------------------------------------------------

	//FPS�R���g���[���[�쐬
	sky::lib::fps::CFpsController fpsCtrl;

	//�`����L���[�̍쐬
	CRenderCommandManager_::CreateQueue( RENDER_QUEUE_SIZE );

	//�`��p�X���b�h�̍쐬
	CRenderCommandManager_::StartThread();

	fpsCtrl.SetFrameRate( _FRAME_NUM );

	SKY_PRINTF( _T( "===================== < RenderTest Start > ===================== \n" ) );

	math::CBasicVector3 effectorPos;
	effectorPos.X = 0;
	effectorPos.Y = -12;
	effectorPos.Z = 0;

	//�Q�[�����[�v
	GAME_LOOP_START
	{
		//�t���[���J�n
		fpsCtrl.StartFrame();

		//���͊Ǘ��X�V
		Ckeyboard_->Update( fpsCtrl );

#define SPEED ( 0.3f )

		//XY���ʂŃG�t�F�N�^�𑀍�
		if ( Ckeyboard_->IsKeyOn( KEY_LEFT ) )
		{
			effectorPos.X += -SPEED;
		}
		else if ( Ckeyboard_->IsKeyOn( KEY_RIGHT ) )
		{
			effectorPos.X += SPEED;
		}
		else if ( Ckeyboard_->IsKeyOn( KEY_UP ) )
		{
			effectorPos.Y += SPEED;
		}
		else if ( Ckeyboard_->IsKeyOn( KEY_DOWN ) )
		{
			effectorPos.Y += -SPEED;
		}

		spEffector->SetPosition_XYZ( effectorPos );

		//IK
		math::InverseKinematicsUtil::SolveCcd( spSkeleton , spSkeleton->m_spJointPallet->GetJoint( spSkeleton->m_spJointPallet->GetJointNum() - 1 ) , effectorPos , 5, 1.0e-4f );
//		math::CBasicVector3List posList;
//		math::InverseKinematicsUtil::SolveParticle( posList , spSkeleton , spSkeleton->m_spJointPallet->GetJoint( spSkeleton->m_spJointPallet->GetJointNum() - 1 ) , effectorPos, 5 );
//		math::InverseKinematicsUtil::SolveEulerJacobian( spSkeleton , spSkeleton->m_spJointPallet->GetJoint( spSkeleton->m_spJointPallet->GetJointNum() - 1 ) , effectorPos,  10, 1.0f );

		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "Shader Test 3D." ) );
		spFont->PrintfSentence( 16 , 32 , graphic::CColor_Tomato , 1.0f , _T( "CameraPos( %f %f %f )" ) , spCamera->GetData().m_Eye.X, spCamera->GetData().m_Eye.Y , spCamera->GetData().m_Eye.Z );
		spFont->PrintfSentence( 16 , 48 , graphic::CColor_Tomato , 1.0f , _T( "CameraAngle( %f %f %f )" ) , spCamera->GetData().m_Angle.X().ToDeg() , spCamera->GetData().m_Angle.Y().ToDeg() , spCamera->GetData().m_Angle.Z().ToDeg() );
		spFont->PrintfSentence( 16 , 64 , graphic::CColor_Tomato , 1.0f , _T( "CameraTargetDistance( %f )" ) , spCamera->GetData().m_Distance );

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