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

//Window名
static const skyString *_TEST_WINDOW_NAME			  = _T( "TestWindow" );
static const skyString *_TEST_SURFACE_NAME			  = _T( "TestCRenderTarget" );

//画面クリアカラー
static const graphic::CColor BACK_BUFFER_CLEAR_COLOR = graphic::CColor_Silver;

//Window初期位置、サイズ
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640;
static const u32  _WIN_H = 480;

TEST( texture , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//フルスクリーンモードOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

	//ウィンドウの作成／パラメーター設定／初期化
	graphic::CWindowPtr spWindow = CWindowCreate_( CHS( _TEST_WINDOW_NAME ) );								//作成
	spWindow->SetRect(  math::CBasicRect( _WIN_X , _WIN_Y , _WIN_W , _WIN_H ) );							//パラメーター設定
	spWindow->SetUp();																					//準備をする

	//サーフェースの作成／パラメーター設定／初期化
	graphic::CRenderTargetPtr spCRenderTarget = CRenderTargetCreate_( CHS( _TEST_SURFACE_NAME ) );							//作成
	spCRenderTarget->SetSize( math::CSize_u32( _WIN_W , _WIN_H ) );												//パラメーター設定
	spCRenderTarget->SetUpFrameBuffer( spWindow );																		//準備
	spCRenderTarget->SetClearColor( BACK_BUFFER_CLEAR_COLOR );

	graphic::CTexturePtr spTexture1 = graphic::CTexture::CreateInstance( CHSC( TEST1_TEXTURE_PATH ) );
	graphic::CTexturePtr spTexture2 = graphic::CTexture::CreateInstance( CHSC( TEST1_TEXTURE_PATH ) );
	graphic::CTexturePtr spTexture3 = graphic::CTexture::CreateInstance( CHSC( TEST1_TEXTURE_PATH ) );

	//内部機能でロード（本番ではまず使用しない。汎用のロードで読み込んでからテクスチャようにするだろう）
	spTexture1->Load();

	//事前にロードしてストリームで渡す
	stream::IStreamPtr strm = CFileUtil_::LoadFile( spTexture2->GetPath() );
	spTexture2->Load( strm );

	//メモリ上にテクスチャを作成する
	graphic::texture::CTextureInfo info;
	info.Width        = graphic::texture::eTexture_with_128;     //幅
	info.Height       = graphic::texture::eTexture_height_128;   //高さ
	info.MipLevels    = 1;                                       //ミップマップの枚数
	info.Format       = graphic::texture::eTextureFormat_R8G8B8;
	spTexture3->Create( info );

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#if ON_

//画面クリアカラー
static const graphic::CColor BACK_BUFFER_CLEAR_COLOR = graphic::CColor_Silver;

//Window初期位置、サイズ
static const u32  _WIN_X = 0;
static const u32  _WIN_Y = 0;
static const u32  _WIN_W = 640;
static const u32  _WIN_H = 480;

static const skyString *_TEST_WINDOW_NAME = _T( "TestWindow" );

TEST( texture , 2 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	//フルスクリーンモードOFF
	SkyGraphicLibController_::SetFullScreen( skyFalse );

	//ウィンドウの作成／パラメーター設定／初期化
	graphic::CWindowPtr spWindow = CWindowCreate_( CHS( _TEST_WINDOW_NAME ) );								//作成
	graphic::CRenderCommand::SetRadyWindow( spWindow , math::CBasicRect( _WIN_X , _WIN_Y , _WIN_W , _WIN_H ) , skyFalse );

	//デバイスの初期化
	graphic::CRenderCommand::InitializeDevice( CDevice_ , spWindow );

	graphic::GLTextureLoader texture;

	//　テクスチャのロード
	graphic::CreateTextureFromFileA( "data/test/texture/lena.dds", texture );

	graphic::CreateTextureFromFileA( "data/test/texture/TestBmp8bit.bmp", texture );

	graphic::CreateTextureFromFileA( "data/test/texture/TestBmp8bit.bmp", texture );


	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif