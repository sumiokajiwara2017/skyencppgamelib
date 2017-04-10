#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_DIRECTORY

#include "SkyBaseLib/Directory.h"
#include "SkyBaseLib/File.h"

#define _TEST_PATH			_T( "../include/SkyBaseLib/" ) //★★最後に/がついてないとこける。後で治そう★★
#define _TEST_TEXT_PATH		_T( "data/test/directory/test.txt" )

//ディレクトリ巡回テスト
TEST( Directory , 1 ) 
{
	SKY_BASE_LIB_INIT();

	//ディレクトリ作成
	sky::lib::directory::CDirectoryPtr dirPtr = CDirectoryFactory_::Create( _TEST_PATH , _T( "*" ) );
	dirPtr->Initialize();

	//適当な大きのメモリデータを作る
	u32 size = 32;
	u8* pBuf = ( u8 * )SkyMalloc( size );
	sky::lib::file::CFileDataPtr filePtr( NEW sky::lib::file::CFileData( _TEST_TEXT_PATH , pBuf , size , skyTrue , skyFalse , skyTrue ) );

	//ファイルオブジェクトに書き出す
	dirPtr->WriteFileList( filePtr );

	//ファイルに書き出す
	sky::lib::stream::IStreamPtr strm = filePtr;
	CFileUtil_::SaveFile( strm , skyTrue );

	SKY_BASE_LIB_TERM();
}

static const skyString *TEST_PATH = _T( "C:/hoge/test/test.txt" );

//Path操作機能確認
TEST( Directory , 2 ) 
{
	SKY_BASE_LIB_INIT();

	skyString drive[ DRIVE_LEN_MAX ];
	skyString dir[ DIR_LEN_MAX ];
	skyString file[ FILE_LEN_MAX ];
	skyString ext[ EXT_LEN_MAX ];

	//ドライブを取得する
	CPathUtil_::GetDrive( TEST_PATH , drive , DRIVE_LEN_MAX );

	//ディレクトリを取得する
	CPathUtil_::GetDirPath( TEST_PATH , dir , DIR_LEN_MAX );

	//ファイル名を取得する
	CPathUtil_::GetFileName( TEST_PATH , file , FILE_LEN_MAX );

	//拡張子を取得する
	CPathUtil_::GetFileExt( TEST_PATH , ext , EXT_LEN_MAX );

	SKY_BASE_LIB_TERM();
}

//環境変数操作機能
TEST( Directory , 3 ) 
{
	SKY_BASE_LIB_INIT();

	u32 envlen;
	skyString env[ 1024 ];

	//ドライブを取得する
	CPathUtil_::GetEnv( envlen , _T( "SKYLIB_ROOT" ) , env , 1024 );

	const skyString *testPath = _T( "%SKYLIB_ROOT%\\hoge\\test\\hoge.txt" );

	skyString rep[ 1024 ];
	CPathUtil_::ReplaceEnv( testPath , rep , 1024 );

	SKY_BASE_LIB_TERM();
}

#endif