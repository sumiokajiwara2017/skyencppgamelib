#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_FILE

#include "SkyBaseLib/File.h"
#include "SkyBaseLib/Console.h"

using namespace sky::lib;

#if OFF_

#define TEST1_FILE_PATH			_T( "data/test/file/Test1Data.txt" )
#define TEST1_FILE_OUT_PATH			_T( "data/test/file/Test1DataOut.txt" )
#define TEST_BUF_LEN			256

//�t�@�C���f�[�^�̃e�X�g
TEST( file , 1 ) 
{	SKY_BASE_LIB_INIT();

	sky::lib::stream::IStreamPtr iStream = CFileUtil_::LoadFile( CHSC( TEST1_FILE_PATH ) );
	sky::lib::file::CFileDataPtr filePtr = iStream;

	sky::lib::stream::IStreamPtr oiSteam( NEW sky::lib::file::CFileData( TEST1_FILE_OUT_PATH , 32 , skyTrue , skyFalse , skyTrue ) );
	sky::lib::file::CFileDataPtr outFilePtr = oiSteam;

	skyString buf[ TEST_BUF_LEN ];
	//�ǂݍ���
#if defined( SKYLIB_PLATFORM_WIN32 ) && defined( _UNICODE )
	filePtr->vReadStream( (u8 * )buf , BOM_SIZE );
	//��������
	outFilePtr->vWriteStream( ( u16 )BOM );
#endif
	filePtr->ReadLine( buf , TEST_BUF_LEN );
	outFilePtr->WriteLine( buf );
	filePtr->ReadLine( buf , TEST_BUF_LEN );
	outFilePtr->WriteLine( buf );
	filePtr->ReadLine( buf , TEST_BUF_LEN );
	outFilePtr->WriteLine( buf );
	filePtr->ReadLine( buf , TEST_BUF_LEN );
	outFilePtr->WriteLine( buf );

	CFileUtil_::SaveFile( oiSteam );
	SKY_BASE_LIB_TERM();
}

#define BIND_SRC_PATH_FILE_1	_T( "data/test/file/BindSrcFile1.txt" )
#define BIND_SRC_PATH_FILE_2	_T( "data/test/file/BindSrcFile2.bmp" )
#define BIND_SRC_PATH_FILE_3	_T( "data/test/file/BindSrcFile3.txt" )
#define BIND_SRC_PATH_FILE_4	_T( "data/test/file/BindSrcFile4.txt" )
#define BIND_SRC_PATH_FILE_5	_T( "data/test/file/BindSrcFile5.bmp" )

#define BIND_DEST_PATH_FILE_1	_T( "data/test/file/BindDestFile1.txt" )
#define BIND_DEST_PATH_FILE_2	_T( "data/test/file/BindDestFile2.bmp" )
#define BIND_DEST_PATH_FILE_3	_T( "data/test/file/BindDestFile3.txt" )
#define BIND_DEST_PATH_FILE_4	_T( "data/test/file/BindDestFile4.txt" )
#define BIND_DEST_PATH_FILE_5	_T( "data/test/file/BindDestFile5.bmp" )

//�o�C���h�t�@�C���e�X�g
TEST( file , 2 ) 
{
	SKY_BASE_LIB_INIT();

	//�o�C���h���\�[�X��ǂݍ���ŁA�o�C���h�f�[�^���쐬����
	string::CStringList srcPathList;
	srcPathList.AddTail( BIND_SRC_PATH_FILE_1 );
	srcPathList.AddTail( BIND_SRC_PATH_FILE_2 );
	srcPathList.AddTail( BIND_SRC_PATH_FILE_3 );
	srcPathList.AddTail( BIND_SRC_PATH_FILE_4 );
	srcPathList.AddTail( BIND_SRC_PATH_FILE_5 );
	sky::lib::file::bind::FileHeader *pBindData = NULL;
	CBindUtil_::BindFiles( srcPathList , pBindData ); //������炿���Ə���

	//�o�C���h�f�[�^����e�t�@�C�������o���A�t�@�C���ɕۑ�����B
	sky::lib::file::bind::DataHeader *pFileData;

	CBindUtil_::LoadFile( pBindData , BIND_SRC_PATH_FILE_1 , pFileData );
	CFileUtil_::SaveBinFileForDisc( BIND_DEST_PATH_FILE_1 , ( ( u8 * )pFileData ) + sizeof ( sky::lib::file::bind::DataHeader ) , pFileData->fileSize );

	CBindUtil_::LoadFile( pBindData , BIND_SRC_PATH_FILE_2 , pFileData );
	CFileUtil_::SaveBinFileForDisc( BIND_DEST_PATH_FILE_2 , ( ( u8 * )pFileData ) + sizeof ( sky::lib::file::bind::DataHeader ) , pFileData->fileSize );

	CBindUtil_::LoadFile( pBindData , BIND_SRC_PATH_FILE_3 , pFileData );
	CFileUtil_::SaveBinFileForDisc( BIND_DEST_PATH_FILE_3 , ( ( u8 * )pFileData ) + sizeof ( sky::lib::file::bind::DataHeader ) , pFileData->fileSize );

	CBindUtil_::LoadFile( pBindData , BIND_SRC_PATH_FILE_4 , pFileData );
	CFileUtil_::SaveBinFileForDisc( BIND_DEST_PATH_FILE_4 , ( ( u8 * )pFileData ) + sizeof ( sky::lib::file::bind::DataHeader ) , pFileData->fileSize );

	CBindUtil_::LoadFile( pBindData , BIND_SRC_PATH_FILE_5 , pFileData );
	CFileUtil_::SaveBinFileForDisc( BIND_DEST_PATH_FILE_5 , ( ( u8 * )pFileData ) + sizeof ( sky::lib::file::bind::DataHeader ) , pFileData->fileSize );

	DEL pBindData;

	SKY_BASE_LIB_TERM();
}

#define TEST_STREAM_FILE_PATH_1	_T( "data/test/file/StreamTestFile.txt" )

//�f�B�X�N�f�[�^�X�g���[���̃e�X�g
TEST( file , 3 ) 
{
	SKY_BASE_LIB_INIT();

	file::CDiscFileStream fileStream( TEST_STREAM_FILE_PATH_1 , file::eOpenMode_Binary_RW_Create );
	file::CDiscFileStream fileStream2( TEST_STREAM_FILE_PATH_1 , file::eOpenMode_Binary_R_NoCreate );

	//�J��

	fileStream.vOpen();

	u16 src1 = 2;
	s16 src2 = -2;
	u32 src3 = 3;
	s32 src4 = -3;

	//��������
	fileStream.vWriteStream( src1 );
	fileStream.vWriteStream( src2 );
	fileStream.vWriteStream( src3 );
	fileStream.vWriteStream( src4 );

	_tprintf( _T( "src = %d %d %d %d \n" ) , src1 , src2 , src3 , src4 );

	//����
	fileStream.vClose();

	//�ʂ̃X�g���[���ŊJ��
	fileStream2.vOpen();

	u16 dest1 = 0;
	s16 dest2 = 0;
	u32 dest3 = 0;
	s32 dest4 = 0;
	s32 dest5 = 0;

	fileStream2.vReadStream( dest1 );
	fileStream2.vReadStream( dest2 );
	fileStream2.vReadStream( dest3 );
	fileStream2.vReadStream( dest4 );
	fileStream2.vReadStream( dest5 );
	fileStream2.vReadStream( dest5 );
	fileStream2.vReadStream( dest5 );

	_tprintf( _T( "dest = %d %d %d %d %d \n" ) , dest1 , dest2 , dest3 , dest4 , dest5 );

	//����
	fileStream2.vClose();

	SKY_BASE_LIB_TERM();
}

#define TEST4_STREAM_FILE_PATH_1	_T( "data/test/file/StreamTextTest.txt" )

//�f�B�X�N�f�[�^�X�g���[���̃e�X�g
TEST( file , 4 ) 
{
	SKY_BASE_LIB_INIT();

	file::CDiscFileStream fileStream( TEST4_STREAM_FILE_PATH_1 , file::eOpenMode_Binary_RW_Create );

	//�J��
	fileStream.vOpen();

	//��������
#if defined( SKYLIB_PLATFORM_WIN32 ) && defined( _UNICODE )
	fileStream.vWriteStream( ( u16 )BOM );
#endif
	fileStream.vWriteStream( _T( "aaaa\n" ) );
	fileStream.vWriteStream( _T( "bbbb\n" ) );
	fileStream.vWriteStream( _T( "cccc\n" ) );
	fileStream.vWriteStream( _T( "dddd\n" ) );

	//����
	fileStream.vClose();

	SKY_BASE_LIB_TERM();
}

#endif

#if ON_

#define TEST5_STREAM_FILE_PATH_1	_T( "data/test/file/PropertyTest.prop" )
#define TEST5_STREAM_FILE_PATH_2	_T( "data/test/file/PropertyTest.propb" )


//�f�B�X�N�f�[�^�X�g���[���̃e�X�g
TEST( file , 5 ) 
{
	SKY_BASE_LIB_INIT();

	CLocaleUtil_::SetAllLocale( locale::Country_English );
	CLocaleUtil_::SetCharacterLocale( locale::Country_English );

	debuglogic::CDebugStream debugStream;
	debugStream.vOpen();
	stream::IStreamPtr debugStreamPtr( &debugStream , skyTrue );

	CPropertyManager_::Dump( debugStreamPtr );
	SKY_PRINTF( _T( "SkyBaseLib �o�[�W���� %f \n" ) , CPropertyManager_::Getd64( CHS( _T( "SkyBaseLib.prop" ) ) , CHS( _T( "Version" ) ) , 0 ) );
	SKY_PRINTF( _T( "SkyBaseLib �쐬�J�n�N���� %s \n" ) , ( const skyString* )CPropertyManager_::GetString( CHS( _T( "SkyBaseLib.prop" ) ) , CHS( _T( "start" ) ) , 0 ) );
	SKY_PRINTF( _T( "SkyGraphicLib �o�[�W���� %f \n" ) , CPropertyManager_::Getd64( CHS( _T( "SkyGraphicLib.prop" ) ) , CHS( _T( "Version" ) ) , 0 ) );
	SKY_PRINTF( _T( "SkyGraphicLib �쐬�J�n�N���� %s \n" ) , ( const skyString* )CPropertyManager_::GetString( CHS( _T( "SkyGraphicLib.prop" ) ) , CHS( _T( "start" ) ) , 0 ) );

	//�e�L�X�g�̃v���p�e�B�t�@�C����ǂ�
	SKY_PRINTF( _T("�e�L�X�g�v���p�e�B�t�@�C����ǂ� \n") );
	stream::IStreamPtr fileStreamPtr = CFileUtil_::LoadFile( CHSC( TEST5_STREAM_FILE_PATH_1 ) );
	fileStreamPtr->vOpen();
	hash::CHashStringPtrListPtrHash propData( file::property::DEFAULt_HASH_SIZE );
	CPropertyFileUtil_::Load( fileStreamPtr , propData );
	CPropertyFileUtil_::Dump( propData , debugStreamPtr );

	//�V�[�N���t�@�C���̐擪�ɂ���
	fileStreamPtr->vSetSeek( 0 );

	//�R���p�C������
	SKY_PRINTF( _T("�e�L�X�g�v���p�e�B�t�@�C�����R���p�C�� \n") );
	stream::IStreamPtr outfileStream( NEW file::CDiscFileStream( TEST5_STREAM_FILE_PATH_2 , file::eOpenMode_Binary_RW_Create ) );
	outfileStream->vOpen();
	CPropertyFileUtil_::Compile( fileStreamPtr , outfileStream );
	outfileStream->vClose();

	//�R���p�C����̃t�@�C����ǂݍ��݃_���v����
	SKY_PRINTF( _T("�o�C�i���v���p�e�B�t�@�C����ǂ� \n") );
	stream::IStreamPtr compilefileStreamPtr = CFileUtil_::LoadFile( CHSC( TEST5_STREAM_FILE_PATH_2 ) );
	compilefileStreamPtr->vOpen();
	hash::CHashStringPtrListPtrHash comppropData( file::property::DEFAULt_HASH_SIZE );
	CPropertyFileUtil_::Load( compilefileStreamPtr , comppropData );
	CPropertyFileUtil_::Dump( comppropData , debugStreamPtr );


	//����
	fileStreamPtr->vClose();
	debugStream.vClose();

	SKY_BASE_LIB_TERM();
}

#endif

#if OFF_

#define TEST6_ASYNC_PATH	_T( "data/test/file/TestAsyncRead.txt" )

//�t�@�C���̔񓯊��ǂݍ���
TEST( file , 6 ) 
{
	SKY_BASE_LIB_INIT();

	//�񓯊��t�@�C�����[�h�J�n
	sky::lib::file::FileLoadAsyncRs loadworker = CFileUtil_::LoadFileAsync( CHSC( TEST6_ASYNC_PATH ) );

	//�t�@�C�����[�h�I���܂�
	while ( loadworker->IsEnd() == skyFalse )
	{
		SKY_PRINTF( _T( "�Ǎ����ł��B���΂炭���҂����������B\n" ) );
	}

	//�߂�l�擾
	sky::lib::stream::IStreamPtr loadfile = loadworker->GetResult();

	loadfile->vPrintf();
	SKY_PRINTF( _T( "����?��BOM�����������������̂�������v�I\n" ) );

	loadfile->vWriteStream( BOM );
	loadfile->vWriteStream( _T( "�n�j�I" ) );

	//�񓯊��t�@�C���������݊J�n
	sky::lib::file::FileSaveAsyncRs saveworker  = CFileUtil_::SaveFileAsync( loadfile );

	//�t�@�C���������݂܂�
	while ( saveworker->IsEnd() == skyFalse )
	{
		SKY_PRINTF( _T( "�������ݒ��ł��B���΂炭���҂����������B\n" ) );
	}

	SKY_BASE_LIB_TERM();
}

#define TEST7_ASYNC_PATH	_T( "data/test/SkyFileBind/binded.bn" )
#define TEST7_ACCESS_PATH_1   _T( "data/test/SkyFileBind/indata/Test1.txt" )
#define TEST7_ACCESS_PATH_2   _T( "data/test/SkyFileBind/indata/Test2.txt" )

//�����t�@�C���Ǘ��@�\�̃e�X�g
TEST( file , 7 ) 
{
	SKY_BASE_LIB_INIT();

	//�����t�@�C����񓯊��ǂݍ��݂���
	CBindFileManager_::LoadAsync( CHSC( TEST7_ASYNC_PATH ) );

	//�񓯊��ǂݍ��ݏI���҂�
	while( CBindFileManager_::IsLoadEnd() == skyFalse )
	{
		SKY_PRINTF( _T( "�����t�@�C���Ǎ����ł��B�B\n" ) );
	}

	//�t�@�C�������_���v
	CBindFileManager_::Print();

	//�������ꂽ�t�@�C���P��ǂ�
	const u8 *pData = CBindFileManager_::LoadFile( CHS( TEST7_ACCESS_PATH_1 ) );
	SKY_PRINTF( _T( "�ǂ߂��P=[%s] \n" ) , pData ); 
	pData = CBindFileManager_::LoadFile( CHS( TEST7_ACCESS_PATH_2 ) );
	SKY_PRINTF( _T( "�ǂ߂��Q=[%s] \n" ) , pData ); 

	//�������ꂽ�t�@�C���P�������i���ۂ͏����Ȃ���skyTrue��Ԃ�����
	CBindFileManager_::DeleteFile( CHS( TEST7_ACCESS_PATH_1 ) );
	CBindFileManager_::DeleteFile( CHS( TEST7_ACCESS_PATH_2 ) );

	//�t�@�C�������_���v
	CBindFileManager_::Print();

	//�����t�@�C����j������
	CBindFileManager_::Delete( CHSC( TEST7_ASYNC_PATH ) );

	SKY_BASE_LIB_TERM();
}

#define TEST8_POOL_PATH_1   _T( "data/test/SkyFileBind/indata/Test1.txt" )
#define TEST8_POOL_PATH_2   _T( "data/test/SkyFileBind/indata/Test2.txt" )

//�v�[���t�@�C���Ǘ��@�\�̃e�X�g
TEST( file , 8 ) 
{
	SKY_BASE_LIB_INIT();

	//�t�@�C�����v�[��
	CFilePoolContena_::LoadAsync( CHSC( TEST8_POOL_PATH_1 ) );
	CFilePoolContena_::LoadAsync( CHSC( TEST8_POOL_PATH_2 ) );

	//�񓯊��ǂݍ��ݏI���҂�
	while( CFilePoolContena_::IsLoadEnd() == skyFalse )
	{
		SKY_PRINTF( _T( "�v�[���t�@�C���Ǎ����ł��B�B\n" ) );
	}

	//�t�@�C�����v�����g
	CFilePoolContena_::Print();

	//�v�[�����ꂽ�t�@�C���P��ǂ�
	const u8 *pData = CFilePoolContena_::LoadFile( CHS( TEST8_POOL_PATH_1 ) );
	SKY_PRINTF( _T( "�ǂ߂��P=[%s] \n" ) , pData ); 
	pData = CFilePoolContena_::LoadFile( CHS( TEST8_POOL_PATH_2 ) );
	SKY_PRINTF( _T( "�ǂ߂��Q=[%s] \n" ) , pData ); 

	//�v�[�����ꂽ�t�@�C���P�������i���ۂ͏����Ȃ���skyTrue��Ԃ�����
	CFilePoolContena_::DeleteFile( CHS( TEST8_POOL_PATH_1 ) );
	CFilePoolContena_::DeleteFile( CHS( TEST8_POOL_PATH_2 ) );

	//�t�@�C�����v�����g
	CFilePoolContena_::Print();

	SKY_BASE_LIB_TERM();
}

#define TEST9_DEF_PATH_1   _T( "data/test/SkyFileBind/indata/Test1.txt" ) //.txt�Ƃ����g���q�̓ǂݍ��ݗv���ɂ͑S�Ă��̃t�@�C����Ԃ�
#define TEST9_READ_PATH_1   _T( "data/test/SkyFileBind/indata/Hoge.txt" )

//�f�t�H���g�t�@�C���Ǘ��@�\�̃e�X�g
TEST( file , 9 ) 
{
	SKY_BASE_LIB_INIT();

	//�f�t�H���g�t�@�C�����v�[��
	CDefaultFileManager_::LoadAsync( CHSC( TEST9_DEF_PATH_1 ) );

	//�񓯊��ǂݍ��ݏI���҂�
	while( CDefaultFileManager_::IsLoadEnd() == skyFalse )
	{
		SKY_PRINTF( _T( "�f�t�H���g�t�@�C���Ǎ����ł��B�B\n" ) );
	}

	//�t�@�C�����v�����g
	CDefaultFileManager_::Print();

	//�f�t�H���g�t�@�C���P��ǂ�
	const u8 *pData = CDefaultFileManager_::LoadFile( CHS( TEST9_READ_PATH_1 ) );
	SKY_PRINTF( _T( "�ǂ߂��P=[%s] \n" ) , pData ); 

	//�f�t�H���g�t�@�C���P�������i���ۂ͏����Ȃ���skyTrue��Ԃ�����
	CFilePoolContena_::DeleteFile( CHS( TEST9_READ_PATH_1 ) );

	//�t�@�C�����v�����g
	CFilePoolContena_::Print();

	SKY_BASE_LIB_TERM();
}

#endif

#if OFF_

#define TEST10_BIND_PATH	_T( "data/test/SkyFileBind/binded.bn" )
#define TEST10_POOL_PATH_1  _T( "data/test/file/TestAsyncRead.txt" )
#define TEST10_DEF_PATH_1   _T( "data/test/SkyFileBind/indata/Test1.txt" ) //.txt�Ƃ����g���q�̓ǂݍ��ݗv���ɂ͑S�Ă��̃t�@�C����Ԃ�

#define TEST10_LOAD_PATH1	_T( "data/test/SkyFileBind/indata/Test1.txt" ) //����̓o�C���h�t�@�C���Ŕ��������͂�
#define TEST10_LOAD_PATH2	_T( "data/test/file/TestAsyncRead.txt" )       //����̓v�[���t�@�C���Ŕ��������
#define TEST10_LOAD_PATH3	_T( "data/test/file/BindSrcFile1.txt" )        //�����Disc�Ŕ�������A�v�[������ĕԂ�͂�
//#define TEST10_LOAD_PATH4	_T( "data/test/file/HogeHoge.txt" )            //����͂ǂ��ɂ��������ꂸ�A�f�t�H���g�t�@�C�����Ԃ�͂��i�ł����ݑ��݂��Ȃ��p�X�̃��[�h���s����Assert���ł�̂Ŗ����j


//�f�t�H���g�t�@�C���Ǘ��@�\�̃e�X�g
TEST( file , 10 ) 
{
	SKY_BASE_LIB_INIT();

//-----------------����-----------------------��

	//�����t�@�C����񓯊��ǂݍ��݂���
	CBindFileManager_::LoadAsync( CHSC( TEST10_BIND_PATH ) );

	//�񓯊��ǂݍ��ݏI���҂�
	while( CBindFileManager_::IsLoadEnd() == skyFalse )
	{
		SKY_PRINTF( _T( "�����t�@�C���Ǎ����ł��B�B\n" ) );
	}

	//�v�[���t�@�C����ǂݍ���
	CFilePoolContena_::LoadAsync( CHSC( TEST10_POOL_PATH_1 ) );

	//�񓯊��ǂݍ��ݏI���҂�
	while( CFilePoolContena_::IsLoadEnd() == skyFalse )
	{
		SKY_PRINTF( _T( "�v�[���t�@�C���Ǎ����ł��B�B\n" ) );
	}

	//�f�t�H���g�t�@�C�����v�[��
	CDefaultFileManager_::LoadAsync( CHSC( TEST10_DEF_PATH_1 ) );

	//�񓯊��ǂݍ��ݏI���҂�
	while( CDefaultFileManager_::IsLoadEnd() == skyFalse )
	{
		SKY_PRINTF( _T( "�f�t�H���g�t�@�C���Ǎ����ł��B�B\n" ) );
	}

//-----------------���f�[�^�ǂݍ���---------------��

	const u8 *pData = CDiscFileController_::Load( CHS( TEST10_LOAD_PATH1 ) );
	SKY_PRINTF( _T( "�ǂ߂��P=[%s] \n" ) , pData ); 

	pData = CDiscFileController_::Load( CHS( TEST10_LOAD_PATH2 ) );
	SKY_PRINTF( _T( "�ǂ߂��Q=[%s] \n" ) , pData ); 

	pData = CDiscFileController_::Load( CHS( TEST10_LOAD_PATH3 ) );
	SKY_PRINTF( _T( "�ǂ߂��R=[%s] \n" ) , pData ); 

//	const u8 *pData = CDiscFileController_::Load( CHS( TEST10_LOAD_PATH4 ) );
//	SKY_PRINTF( _T( "�ǂ߂��P=[%s] \n" ) , pData ); 

	//�t�@�C�������_���v
	CBindFileManager_::Print();
	CFilePoolContena_::Print();
	CDefaultFileManager_::Print();

	CDiscFileController_::Delete( CHS( TEST10_LOAD_PATH1 ) );
	CDiscFileController_::Delete( CHS( TEST10_LOAD_PATH2 ) );
	CDiscFileController_::Delete( CHS( TEST10_LOAD_PATH3 ) );

	//�t�@�C�������_���v
	CBindFileManager_::Print();
	CFilePoolContena_::Print();
	CDefaultFileManager_::Print();

	SKY_BASE_LIB_TERM();
}

#endif

#endif