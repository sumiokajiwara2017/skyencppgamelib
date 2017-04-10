#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_DIRECTORY

#include "SkyBaseLib/Directory.h"
#include "SkyBaseLib/File.h"

#define _TEST_PATH			_T( "../include/SkyBaseLib/" ) //�����Ō��/�����ĂȂ��Ƃ�����B��Ŏ���������
#define _TEST_TEXT_PATH		_T( "data/test/directory/test.txt" )

//�f�B���N�g������e�X�g
TEST( Directory , 1 ) 
{
	SKY_BASE_LIB_INIT();

	//�f�B���N�g���쐬
	sky::lib::directory::CDirectoryPtr dirPtr = CDirectoryFactory_::Create( _TEST_PATH , _T( "*" ) );
	dirPtr->Initialize();

	//�K���ȑ傫�̃������f�[�^�����
	u32 size = 32;
	u8* pBuf = ( u8 * )SkyMalloc( size );
	sky::lib::file::CFileDataPtr filePtr( NEW sky::lib::file::CFileData( _TEST_TEXT_PATH , pBuf , size , skyTrue , skyFalse , skyTrue ) );

	//�t�@�C���I�u�W�F�N�g�ɏ����o��
	dirPtr->WriteFileList( filePtr );

	//�t�@�C���ɏ����o��
	sky::lib::stream::IStreamPtr strm = filePtr;
	CFileUtil_::SaveFile( strm , skyTrue );

	SKY_BASE_LIB_TERM();
}

static const skyString *TEST_PATH = _T( "C:/hoge/test/test.txt" );

//Path����@�\�m�F
TEST( Directory , 2 ) 
{
	SKY_BASE_LIB_INIT();

	skyString drive[ DRIVE_LEN_MAX ];
	skyString dir[ DIR_LEN_MAX ];
	skyString file[ FILE_LEN_MAX ];
	skyString ext[ EXT_LEN_MAX ];

	//�h���C�u���擾����
	CPathUtil_::GetDrive( TEST_PATH , drive , DRIVE_LEN_MAX );

	//�f�B���N�g�����擾����
	CPathUtil_::GetDirPath( TEST_PATH , dir , DIR_LEN_MAX );

	//�t�@�C�������擾����
	CPathUtil_::GetFileName( TEST_PATH , file , FILE_LEN_MAX );

	//�g���q���擾����
	CPathUtil_::GetFileExt( TEST_PATH , ext , EXT_LEN_MAX );

	SKY_BASE_LIB_TERM();
}

//���ϐ�����@�\
TEST( Directory , 3 ) 
{
	SKY_BASE_LIB_INIT();

	u32 envlen;
	skyString env[ 1024 ];

	//�h���C�u���擾����
	CPathUtil_::GetEnv( envlen , _T( "SKYLIB_ROOT" ) , env , 1024 );

	const skyString *testPath = _T( "%SKYLIB_ROOT%\\hoge\\test\\hoge.txt" );

	skyString rep[ 1024 ];
	CPathUtil_::ReplaceEnv( testPath , rep , 1024 );

	SKY_BASE_LIB_TERM();
}

#endif