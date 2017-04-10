#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_TEXTURE

using namespace sky::lib;

#include "SkyGraphicLib/Texture.h"

#if OFF_

static const skyString *TEST1_BMP4_PATH = _T( "data/test/texture/Bmp4bit.bmp" );
static const skyString *TEST1_BMP8_PATH = _T( "data/test/texture/Bmp8bit.bmp" );
static const skyString *TEST1_BMP24_PATH = _T( "data/test/texture/Bmp24bit.bmp" );
static const skyString *TEST1_BMP32_PATH = _T( "data/test/texture/Bmp32bit.bmp" );

//�r�b�g�}�b�v�T�C�Y
static const u32 BMP_W = 128;
static const u32 BMP_H = 128;

TEST( bmp , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	graphic::CBmp4BitColor bmp4;
	graphic::CBmp8BitColor bmp8;
	graphic::CBmp24BitColor bmp24;
	graphic::CBmp32BitColor bmp32;

	//���������Bmp���쐬����
	bmp4.vCreate( BMP_W , BMP_H , 0 );
	bmp8.vCreate( BMP_W , BMP_H , 0 );
	bmp24.vCreate( BMP_W , BMP_H , 0x00FFFFFF );
	bmp32.vCreate( BMP_W , BMP_H , 0x00FFFFFF );

	//�l�p������
	bmp4.DrawRect( math::CBasicRect( 0 , 0 , BMP_W/2 , BMP_H/2 ) , 1 );
	bmp8.DrawRect( math::CBasicRect( 0 , 0 , BMP_W/2 , BMP_H/2 ) , 1 );
	bmp24.DrawRect( math::CBasicRect( 0 , 0 , BMP_W/2 , BMP_H/2 ) , 0x00FF0000 );
	bmp32.DrawRect( math::CBasicRect( 0 , 0 , BMP_W/2 , BMP_H/2 ) , 0x00FF0000 );

	//�o�̓X�g���[���쐬
	stream::IStreamPtr outStream4( NEW file::CFileStream( CHS( TEST1_BMP4_PATH ) , 32 , skyTrue , skyFalse , skyTrue ) );
	stream::IStreamPtr outStream8( NEW file::CFileStream( CHS( TEST1_BMP8_PATH ) , 32 , skyTrue , skyFalse , skyTrue ) );
	stream::IStreamPtr outStream24( NEW file::CFileStream( CHS( TEST1_BMP24_PATH ) , 32 , skyTrue , skyFalse , skyTrue ) );
	stream::IStreamPtr outStream32( NEW file::CFileStream( CHS( TEST1_BMP32_PATH ) , 32 , skyTrue , skyFalse , skyTrue ) );

	//�X�g���[���o��
	bmp4.vSave( outStream4 );
	bmp8.vSave( outStream8 );
	bmp24.vSave( outStream24 );
	bmp32.vSave( outStream32 );

	//�X�g���[�����t�@�C���ɏo��
	CFileUtil_::SaveFile( outStream4 );
	CFileUtil_::SaveFile( outStream8 );
	CFileUtil_::SaveFile( outStream24 );
	CFileUtil_::SaveFile( outStream32 );

	//�t�@�C����ǂݍ���
	stream::IStreamPtr inStream4 = CFileUtil_::LoadFile( CHS( TEST1_BMP4_PATH ) );
	stream::IStreamPtr inStream8 = CFileUtil_::LoadFile( CHS( TEST1_BMP8_PATH ) );
	stream::IStreamPtr inStream24 = CFileUtil_::LoadFile( CHS( TEST1_BMP24_PATH ) );
	stream::IStreamPtr inStream32 = CFileUtil_::LoadFile( CHS( TEST1_BMP32_PATH ) );

	//���̓X�g���[������f�[�^���擾
	bmp4.vLoad( inStream4 );
	bmp8.vLoad( inStream8 );
	bmp24.vLoad( inStream24 );
	bmp32.vLoad( inStream32 );

	//�l�p������
	bmp4.DrawRect( math::CBasicRect( BMP_W/2 , BMP_H/2 , BMP_W/2 , BMP_H/2 ) , 2 );
	bmp8.DrawRect( math::CBasicRect( BMP_W/2 , BMP_H/2 , BMP_W/2 , BMP_H/2 ) , 2 );
	bmp24.DrawRect( math::CBasicRect( BMP_W/2 , BMP_H/2 , BMP_W/2 , BMP_H/2 ) , 0x0000FF00 );
	bmp32.DrawRect( math::CBasicRect( BMP_W/2 , BMP_H/2 , BMP_W/2 , BMP_H/2 ) , 0x0000FF00 );

	//�X�g���[���o��
	outStream4->vDestroy();
	outStream8->vDestroy();
	outStream24->vDestroy();
	outStream32->vDestroy();
	bmp4.vSave( outStream4 );
	bmp8.vSave( outStream8 );
	bmp24.vSave( outStream24 );
	bmp32.vSave( outStream32 );

	//�X�g���[�����t�@�C���ɏo��
	CFileUtil_::SaveFile( outStream4 );
	CFileUtil_::SaveFile( outStream8 );
	CFileUtil_::SaveFile( outStream24 );
	CFileUtil_::SaveFile( outStream32 );
	
	SKY_LIB_GRAPHIC_TERM();
}

#endif

#if OFF_

static const skyString *TEST2_BMP4_PATH = _T( "data/test/texture/Bmp4bitFont.bmp" );
static const skyString *TEST2_BMP8_PATH = _T( "data/test/texture/Bmp8bitFont.bmp" );
static const skyString *TEST2_BMP24_PATH = _T( "data/test/texture/Bmp24bitFont.bmp" );
static const skyString *TEST2_BMP32_PATH = _T( "data/test/texture/Bmp32bitFont.bmp" );

TEST( bmp , 2 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	graphic::CBmp4BitColor  bmp4;
	graphic::CBmp8BitColor  bmp8;
	graphic::CBmp24BitColor bmp24;
	graphic::CBmp32BitColor bmp32;

	graphic::CFontBmpInfo info;
	info.in_eGradation = graphic::fontbmp::eGradationType_17;
	info.in_size = 32;
//	info.in_isBold = skyTrue;
	info.in_isAlphaGradation = skyFalse;
	graphic::BmpUtil::WriteFontBmp( &bmp4 , _T( "��" ) , info ); SkyFree( info.out_pUvData ); //UV���͎g��Ȃ��̂ő����j��
	graphic::BmpUtil::WriteFontBmp( &bmp8 , _T( "abcdefghijklmnopqrstuvwxyz" ) , info ); SkyFree( info.out_pUvData );
	graphic::BmpUtil::WriteFontBmp( &bmp24 , _T( "abcdefghijklmnopqrstuvwxyz" ) , info ); SkyFree( info.out_pUvData );
	graphic::BmpUtil::WriteFontBmp( &bmp32 , _T( "abcdefghijklmnopqrstuvwxyz" ) , info ); SkyFree( info.out_pUvData );

	//�o�̓X�g���[���쐬
	stream::IStreamPtr outStream4( NEW file::CFileStream( CHS( TEST2_BMP4_PATH ) , 32 , skyTrue , skyFalse , skyTrue ) );
	stream::IStreamPtr outStream8( NEW file::CFileStream( CHS( TEST2_BMP8_PATH ) , 32 , skyTrue , skyFalse , skyTrue ) );
	stream::IStreamPtr outStream24( NEW file::CFileStream( CHS( TEST2_BMP24_PATH ) , 32 , skyTrue , skyFalse , skyTrue ) );
	stream::IStreamPtr outStream32( NEW file::CFileStream( CHS( TEST2_BMP32_PATH ) , 32 , skyTrue , skyFalse , skyTrue ) );

	//�X�g���[���o��
	bmp4.vSave( outStream4 );
	bmp8.vSave( outStream8 );
	bmp24.vSave( outStream24 );
	bmp32.vSave( outStream32 );

	//�X�g���[�����t�@�C���ɏo��
	CFileUtil_::SaveFile( outStream4 );
	CFileUtil_::SaveFile( outStream8 );
	CFileUtil_::SaveFile( outStream24 );
	CFileUtil_::SaveFile( outStream32 );

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif