#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_TEXTURE

using namespace sky::lib;

#include "SkyGraphicLib/Texture.h"

#if OFF_

//static const skyString *TEST1_TEXTURE_PATH = _T( "data/test/texture/Bmp4bitFont.bmp" );
static const skyString *TEST1_TEXTURE_PATH = _T( "data/test/texture/TestBmp8bit.bmp" );
//static const skyString *TEST1_TEXTURE_PATH = _T( "data/test/texture/Bmp24bit.bmp" );
//static const skyString *TEST1_TEXTURE_PATH = _T( "data/test/texture/Bmp32bit.bmp" );
//static const skyString *TEST1_TEXTURE_PATH = _T( "data/test/texture/TestJpg.jpg" );
//static const skyString *TEST1_TEXTURE_PATH = _T( "data/test/texture/TestPng.png" );
//static const skyString *TEST1_TEXTURE_PATH = _T( "data/test/texture/TestTga.tga" );

//Window��
static const skyString *_TEST_WINDOW_NAME			  = _T( "TestWindow" );
static const skyString *_TEST_SURFACE_NAME			  = _T( "TestCRenderTarget" );

//��ʃN���A�J���[
static const graphic::CColor BACK_BUFFER_CLEAR_COLOR = graphic::CColor_Silver;

//Window�����ʒu�A�T�C�Y
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640;
static const u32  _WIN_H = 480;

TEST( texture , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//�t���X�N���[�����[�hOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

	//�E�B���h�E�̍쐬�^�p�����[�^�[�ݒ�^������
	graphic::CWindowPtr spWindow = CWindowCreate_( CHS( _TEST_WINDOW_NAME ) );								//�쐬
	spWindow->SetRect(  math::CBasicRect( _WIN_X , _WIN_Y , _WIN_W , _WIN_H ) );							//�p�����[�^�[�ݒ�
	spWindow->SetUp();																					//����������

	//�T�[�t�F�[�X�̍쐬�^�p�����[�^�[�ݒ�^������
	graphic::CRenderTargetPtr spCRenderTarget = CRenderTargetCreate_( CHS( _TEST_SURFACE_NAME ) );							//�쐬
	spCRenderTarget->SetSize( math::CSize_u32( _WIN_W , _WIN_H ) );												//�p�����[�^�[�ݒ�
	spCRenderTarget->SetUpFrameBuffer( spWindow );																		//����
	spCRenderTarget->SetClearColor( BACK_BUFFER_CLEAR_COLOR );

	graphic::CTexturePtr spTexture1 = graphic::CTexture::CreateInstance( CHSC( TEST1_TEXTURE_PATH ) );
	graphic::CTexturePtr spTexture2 = graphic::CTexture::CreateInstance( CHSC( TEST1_TEXTURE_PATH ) );
	graphic::CTexturePtr spTexture3 = graphic::CTexture::CreateInstance( CHSC( TEST1_TEXTURE_PATH ) );

	//�����@�\�Ń��[�h�i�{�Ԃł͂܂��g�p���Ȃ��B�ėp�̃��[�h�œǂݍ���ł���e�N�X�`���悤�ɂ��邾�낤�j
	spTexture1->Load();

	//���O�Ƀ��[�h���ăX�g���[���œn��
	stream::IStreamPtr strm = CFileUtil_::LoadFile( spTexture2->GetPath() );
	spTexture2->Load( strm );

	//��������Ƀe�N�X�`�����쐬����
	graphic::texture::CTextureInfo info;
	info.Width        = graphic::texture::eTexture_with_128;     //��
	info.Height       = graphic::texture::eTexture_height_128;   //����
	info.MipLevels    = 1;                                       //�~�b�v�}�b�v�̖���
	info.Format       = graphic::texture::eTextureFormat_R8G8B8;
	spTexture3->Create( info );

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#if ON_

//��ʃN���A�J���[
static const graphic::CColor BACK_BUFFER_CLEAR_COLOR = graphic::CColor_Silver;

//Window�����ʒu�A�T�C�Y
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640;
static const u32  _WIN_H = 480;

static const skyString *_TEST_WINDOW_NAME = _T( "TestWindow" );

TEST( texture , 2 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//�t���X�N���[�����[�hOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

	//�E�B���h�E�̍쐬�^�p�����[�^�[�ݒ�^������
	graphic::CWindowPtr spWindow = CWindowCreate_( CHS( _TEST_WINDOW_NAME ) );								//�쐬
	graphic::CRenderCommand::SetRadyWindow( spWindow , math::CBasicRect( _WIN_X , _WIN_Y , _WIN_W , _WIN_H ) , skyFalse );

	//�f�o�C�X�̏�����
	graphic::CRenderCommand::InitializeDevice( CDevice_ , spWindow );

	graphic::GLTextureLoader texture;

	//�@�e�N�X�`���̃��[�h
	graphic::CreateTextureFromFileA( "data/test/texture/lena.dds", texture );

	graphic::CreateTextureFromFileA( "data/test/texture/TestBmp8bit.bmp", texture );

	graphic::CreateTextureFromFileA( "data/test/texture/TestBmp8bit.bmp", texture );


	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif