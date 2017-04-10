#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"
#include "SkyGraphicLib/Model.h"
#include "SkyGraphicLib/Loader.h"

#ifdef TEST_MODEL

#if OFF_

using namespace sky::lib;

//Window�����ʒu�A�T�C�Y
static const dectype  _WIN_X = 0;
static const dectype  _WIN_Y = 0;
static const dectype  _WIN_W = 1024;
static const dectype  _WIN_H = 768;

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
	graphic::CameraPtr spModel = CameraCreate_( graphic::eCameraKind_Satellite );
	spModel->SetOrtho( skyTrue );         //���ˉe

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( _T( "MainScreen" ) );            //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = spModel;                       //�J����
	screenData.WindowRect.X()     = _WIN_X;                               //��ʈʒuX
	screenData.WindowRect.Y()     = _WIN_Y;                               //��ʈʒuY
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���

	//�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectPtr spMainScreen = CScreenObject_::Create( screenData );

//------�q�X�N���[��

	//�J�����̍쐬
	graphic::CameraPtr spCamera = CameraCreate_( graphic::eCameraKind_Satellite );
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

//	spModel_1->ClearAndAttachShader( CPhongShaderObjectCreate_() );
//	spModel_2->ClearAndAttachShader( CPhongShaderObjectCreate_() );
//	spModel_3->ClearAndAttachShader( CPhongShaderObjectCreate_() );
//	spModel_4->ClearAndAttachShader( CPhongShaderObjectCreate_() );

	spModel_1->GetMeshContena()->GetMesh()->GetMaterialContena()->GetMaterial()->Reset();
	spModel_2->GetMeshContena()->GetMesh()->GetMaterialContena()->GetMaterial()->Reset();
	spModel_3->GetMeshContena()->GetMesh()->GetMaterialContena()->GetMaterial()->Reset();
	spModel_4->GetMeshContena()->GetMesh()->GetMaterialContena()->GetMaterial()->Reset();

//	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spModel_1 );
//	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spModel_2 );
//	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spModel_3 );
//	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spModel_4 );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateStandardLine3() );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateGrid_XZ( 10 ) );

	//�����`���n��
	graphic::CModelPtr cameraFrustum = graphic::CModelUtil::CreateFrustum( math::CBasicVector3( 0 , 0 , 0 ) , spCamera );
	spPostEffectScreen->GetScene()->GetSceneGraphRoot()->AttachChild( cameraFrustum );


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
		spFont->PrintfSentence( 16 , 32 , graphic::CColor_Tomato , 1.0f , _T( "CameraPos( %f %f %f )" ) , spCamera->GetData().m_Eye.X, spCamera->GetData().m_Eye.Y , spCamera->GetData().m_Eye.Z );
		spFont->PrintfSentence( 16 , 48 , graphic::CColor_Tomato , 1.0f , _T( "CameraAngle( %f %f %f )" ) , spCamera->GetData().m_Angle.X().ToDeg() , spCamera->GetData().m_Angle.Y().ToDeg() , spCamera->GetData().m_Angle.Z().ToDeg() );

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
	graphic::CameraPtr spMainCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	spMainCamera->SetDefaultInput();

	//���[�g�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectSetUpData screenData;
	screenData.Name               = CHS( _T( "MainScreen" ) );            //�X�N���[����
	screenData.ClearColor         = graphic::CColor_DeepBlue;             //�X�N���[���N���A�J���[
	screenData.Camera             = spMainCamera;                       //�J����
	screenData.WindowRect.X()     = _WIN_X;                               //��ʈʒuX
	screenData.WindowRect.Y()     = _WIN_Y;                               //��ʈʒuY
	screenData.WindowRect.W()     = _WIN_W;                               //��ʕ�
	screenData.WindowRect.H()     = _WIN_H;                               //��ʍ���

	//�X�N���[���̃Z�b�g�A�b�v
	graphic::CScreenObjectPtr spMainScreen = CScreenObject_::Create( screenData );

	//���[�h�p�X����
	graphic::CGraphicFilePathManager::SetColladaFilePathRoot( CHS( "data/lib/shader/testdata/" ) );
	graphic::CGraphicFilePathManager::SetTextureFilePathRoot( CHS( "data/lib/shader/testdata/" ) );

	//���f���̓ǂݍ���
	graphic::ColladaLoader::LoadFile( CHS( _T( "shaderTestModel5.dae" ) ) );

	graphic::CModelPtr spModel_1 = graphic::CDataBase::GetModel( CHS( "Cone" ) );
	spModel_1->ActivateTraverse();
	CDataBase_::Clear();

	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( spModel_1 );

	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateStandardLine3() );
	spMainScreen->GetScene()->GetSceneGraphRoot()->AttachChild( graphic::CModelUtil::CreateGrid_XZ( 10 ) );

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

		spFont->PrintfSentence( 16 , 16 , graphic::CColor_Tomato , 1.0f , _T( "Shader Test 3D." ) );
		spFont->PrintfSentence( 16 , 32 , graphic::CColor_Tomato , 1.0f , _T( "CameraPos( %f %f %f )" ) , spMainCamera->GetData().m_Eye.X, spMainCamera->GetData().m_Eye.Y , spMainCamera->GetData().m_Eye.Z );
		spFont->PrintfSentence( 16 , 48 , graphic::CColor_Tomato , 1.0f , _T( "CameraTag( %f %f %f )" ) , spMainCamera->GetData().m_Tag.X, spMainCamera->GetData().m_Tag.Y , spMainCamera->GetData().m_Tag.Z );

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