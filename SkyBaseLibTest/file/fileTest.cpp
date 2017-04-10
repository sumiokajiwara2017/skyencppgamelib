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

//ファイルデータのテスト
TEST( file , 1 ) 
{	SKY_BASE_LIB_INIT();

	sky::lib::stream::IStreamPtr iStream = CFileUtil_::LoadFile( CHSC( TEST1_FILE_PATH ) );
	sky::lib::file::CFileDataPtr filePtr = iStream;

	sky::lib::stream::IStreamPtr oiSteam( NEW sky::lib::file::CFileData( TEST1_FILE_OUT_PATH , 32 , skyTrue , skyFalse , skyTrue ) );
	sky::lib::file::CFileDataPtr outFilePtr = oiSteam;

	skyString buf[ TEST_BUF_LEN ];
	//読み込む
#if defined( SKYLIB_PLATFORM_WIN32 ) && defined( _UNICODE )
	filePtr->vReadStream( (u8 * )buf , BOM_SIZE );
	//書き込む
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

//バインドファイルテスト
TEST( file , 2 ) 
{
	SKY_BASE_LIB_INIT();

	//バインド元ソースを読み込んで、バインドデータを作成する
	string::CStringList srcPathList;
	srcPathList.AddTail( BIND_SRC_PATH_FILE_1 );
	srcPathList.AddTail( BIND_SRC_PATH_FILE_2 );
	srcPathList.AddTail( BIND_SRC_PATH_FILE_3 );
	srcPathList.AddTail( BIND_SRC_PATH_FILE_4 );
	srcPathList.AddTail( BIND_SRC_PATH_FILE_5 );
	sky::lib::file::bind::FileHeader *pBindData = NULL;
	CBindUtil_::BindFiles( srcPathList , pBindData ); //作ったらちゃんと消す

	//バインドデータから各ファイルを取り出し、ファイルに保存する。
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

//ディスクデータストリームのテスト
TEST( file , 3 ) 
{
	SKY_BASE_LIB_INIT();

	file::CDiscFileStream fileStream( TEST_STREAM_FILE_PATH_1 , file::eOpenMode_Binary_RW_Create );
	file::CDiscFileStream fileStream2( TEST_STREAM_FILE_PATH_1 , file::eOpenMode_Binary_R_NoCreate );

	//開く

	fileStream.vOpen();

	u16 src1 = 2;
	s16 src2 = -2;
	u32 src3 = 3;
	s32 src4 = -3;

	//書き込む
	fileStream.vWriteStream( src1 );
	fileStream.vWriteStream( src2 );
	fileStream.vWriteStream( src3 );
	fileStream.vWriteStream( src4 );

	_tprintf( _T( "src = %d %d %d %d \n" ) , src1 , src2 , src3 , src4 );

	//閉じる
	fileStream.vClose();

	//別のストリームで開く
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

	//閉じる
	fileStream2.vClose();

	SKY_BASE_LIB_TERM();
}

#define TEST4_STREAM_FILE_PATH_1	_T( "data/test/file/StreamTextTest.txt" )

//ディスクデータストリームのテスト
TEST( file , 4 ) 
{
	SKY_BASE_LIB_INIT();

	file::CDiscFileStream fileStream( TEST4_STREAM_FILE_PATH_1 , file::eOpenMode_Binary_RW_Create );

	//開く
	fileStream.vOpen();

	//書き込む
#if defined( SKYLIB_PLATFORM_WIN32 ) && defined( _UNICODE )
	fileStream.vWriteStream( ( u16 )BOM );
#endif
	fileStream.vWriteStream( _T( "aaaa\n" ) );
	fileStream.vWriteStream( _T( "bbbb\n" ) );
	fileStream.vWriteStream( _T( "cccc\n" ) );
	fileStream.vWriteStream( _T( "dddd\n" ) );

	//閉じる
	fileStream.vClose();

	SKY_BASE_LIB_TERM();
}

#endif

#if ON_

#define TEST5_STREAM_FILE_PATH_1	_T( "data/test/file/PropertyTest.prop" )
#define TEST5_STREAM_FILE_PATH_2	_T( "data/test/file/PropertyTest.propb" )


//ディスクデータストリームのテスト
TEST( file , 5 ) 
{
	SKY_BASE_LIB_INIT();

	CLocaleUtil_::SetAllLocale( locale::Country_English );
	CLocaleUtil_::SetCharacterLocale( locale::Country_English );

	debuglogic::CDebugStream debugStream;
	debugStream.vOpen();
	stream::IStreamPtr debugStreamPtr( &debugStream , skyTrue );

	CPropertyManager_::Dump( debugStreamPtr );
	SKY_PRINTF( _T( "SkyBaseLib バージョン %f \n" ) , CPropertyManager_::Getd64( CHS( _T( "SkyBaseLib.prop" ) ) , CHS( _T( "Version" ) ) , 0 ) );
	SKY_PRINTF( _T( "SkyBaseLib 作成開始年月日 %s \n" ) , ( const skyString* )CPropertyManager_::GetString( CHS( _T( "SkyBaseLib.prop" ) ) , CHS( _T( "start" ) ) , 0 ) );
	SKY_PRINTF( _T( "SkyGraphicLib バージョン %f \n" ) , CPropertyManager_::Getd64( CHS( _T( "SkyGraphicLib.prop" ) ) , CHS( _T( "Version" ) ) , 0 ) );
	SKY_PRINTF( _T( "SkyGraphicLib 作成開始年月日 %s \n" ) , ( const skyString* )CPropertyManager_::GetString( CHS( _T( "SkyGraphicLib.prop" ) ) , CHS( _T( "start" ) ) , 0 ) );

	//テキストのプロパティファイルを読む
	SKY_PRINTF( _T("テキストプロパティファイルを読む \n") );
	stream::IStreamPtr fileStreamPtr = CFileUtil_::LoadFile( CHSC( TEST5_STREAM_FILE_PATH_1 ) );
	fileStreamPtr->vOpen();
	hash::CHashStringPtrListPtrHash propData( file::property::DEFAULt_HASH_SIZE );
	CPropertyFileUtil_::Load( fileStreamPtr , propData );
	CPropertyFileUtil_::Dump( propData , debugStreamPtr );

	//シークをファイルの先頭にする
	fileStreamPtr->vSetSeek( 0 );

	//コンパイルする
	SKY_PRINTF( _T("テキストプロパティファイルをコンパイル \n") );
	stream::IStreamPtr outfileStream( NEW file::CDiscFileStream( TEST5_STREAM_FILE_PATH_2 , file::eOpenMode_Binary_RW_Create ) );
	outfileStream->vOpen();
	CPropertyFileUtil_::Compile( fileStreamPtr , outfileStream );
	outfileStream->vClose();

	//コンパイル後のファイルを読み込みダンプする
	SKY_PRINTF( _T("バイナリプロパティファイルを読む \n") );
	stream::IStreamPtr compilefileStreamPtr = CFileUtil_::LoadFile( CHSC( TEST5_STREAM_FILE_PATH_2 ) );
	compilefileStreamPtr->vOpen();
	hash::CHashStringPtrListPtrHash comppropData( file::property::DEFAULt_HASH_SIZE );
	CPropertyFileUtil_::Load( compilefileStreamPtr , comppropData );
	CPropertyFileUtil_::Dump( comppropData , debugStreamPtr );


	//閉じる
	fileStreamPtr->vClose();
	debugStream.vClose();

	SKY_BASE_LIB_TERM();
}

#endif

#if OFF_

#define TEST6_ASYNC_PATH	_T( "data/test/file/TestAsyncRead.txt" )

//ファイルの非同期読み込み
TEST( file , 6 ) 
{
	SKY_BASE_LIB_INIT();

	//非同期ファイルロード開始
	sky::lib::file::FileLoadAsyncRs loadworker = CFileUtil_::LoadFileAsync( CHSC( TEST6_ASYNC_PATH ) );

	//ファイルロード終了まち
	while ( loadworker->IsEnd() == skyFalse )
	{
		SKY_PRINTF( _T( "読込中です。しばらくお待ちください。\n" ) );
	}

	//戻り値取得
	sky::lib::stream::IStreamPtr loadfile = loadworker->GetResult();

	loadfile->vPrintf();
	SKY_PRINTF( _T( "頭の?はBOMが文字化けしたものだから大丈夫！\n" ) );

	loadfile->vWriteStream( BOM );
	loadfile->vWriteStream( _T( "ＯＫ！" ) );

	//非同期ファイル書き込み開始
	sky::lib::file::FileSaveAsyncRs saveworker  = CFileUtil_::SaveFileAsync( loadfile );

	//ファイル書き込みまち
	while ( saveworker->IsEnd() == skyFalse )
	{
		SKY_PRINTF( _T( "書き込み中です。しばらくお待ちください。\n" ) );
	}

	SKY_BASE_LIB_TERM();
}

#define TEST7_ASYNC_PATH	_T( "data/test/SkyFileBind/binded.bn" )
#define TEST7_ACCESS_PATH_1   _T( "data/test/SkyFileBind/indata/Test1.txt" )
#define TEST7_ACCESS_PATH_2   _T( "data/test/SkyFileBind/indata/Test2.txt" )

//結合ファイル管理機能のテスト
TEST( file , 7 ) 
{
	SKY_BASE_LIB_INIT();

	//結合ファイルを非同期読み込みする
	CBindFileManager_::LoadAsync( CHSC( TEST7_ASYNC_PATH ) );

	//非同期読み込み終了待ち
	while( CBindFileManager_::IsLoadEnd() == skyFalse )
	{
		SKY_PRINTF( _T( "結合ファイル読込中です。。\n" ) );
	}

	//ファイル情報をダンプ
	CBindFileManager_::Print();

	//結合されたファイル１個を読む
	const u8 *pData = CBindFileManager_::LoadFile( CHS( TEST7_ACCESS_PATH_1 ) );
	SKY_PRINTF( _T( "読めた１=[%s] \n" ) , pData ); 
	pData = CBindFileManager_::LoadFile( CHS( TEST7_ACCESS_PATH_2 ) );
	SKY_PRINTF( _T( "読めた２=[%s] \n" ) , pData ); 

	//結合されたファイル１個を消す（実際は消さないがskyTrueを返すこと
	CBindFileManager_::DeleteFile( CHS( TEST7_ACCESS_PATH_1 ) );
	CBindFileManager_::DeleteFile( CHS( TEST7_ACCESS_PATH_2 ) );

	//ファイル情報をダンプ
	CBindFileManager_::Print();

	//結合ファイルを破棄する
	CBindFileManager_::Delete( CHSC( TEST7_ASYNC_PATH ) );

	SKY_BASE_LIB_TERM();
}

#define TEST8_POOL_PATH_1   _T( "data/test/SkyFileBind/indata/Test1.txt" )
#define TEST8_POOL_PATH_2   _T( "data/test/SkyFileBind/indata/Test2.txt" )

//プールファイル管理機能のテスト
TEST( file , 8 ) 
{
	SKY_BASE_LIB_INIT();

	//ファイルをプール
	CFilePoolContena_::LoadAsync( CHSC( TEST8_POOL_PATH_1 ) );
	CFilePoolContena_::LoadAsync( CHSC( TEST8_POOL_PATH_2 ) );

	//非同期読み込み終了待ち
	while( CFilePoolContena_::IsLoadEnd() == skyFalse )
	{
		SKY_PRINTF( _T( "プールファイル読込中です。。\n" ) );
	}

	//ファイルをプリント
	CFilePoolContena_::Print();

	//プールされたファイル１個を読む
	const u8 *pData = CFilePoolContena_::LoadFile( CHS( TEST8_POOL_PATH_1 ) );
	SKY_PRINTF( _T( "読めた１=[%s] \n" ) , pData ); 
	pData = CFilePoolContena_::LoadFile( CHS( TEST8_POOL_PATH_2 ) );
	SKY_PRINTF( _T( "読めた２=[%s] \n" ) , pData ); 

	//プールされたファイル１個を消す（実際は消さないがskyTrueを返すこと
	CFilePoolContena_::DeleteFile( CHS( TEST8_POOL_PATH_1 ) );
	CFilePoolContena_::DeleteFile( CHS( TEST8_POOL_PATH_2 ) );

	//ファイルをプリント
	CFilePoolContena_::Print();

	SKY_BASE_LIB_TERM();
}

#define TEST9_DEF_PATH_1   _T( "data/test/SkyFileBind/indata/Test1.txt" ) //.txtという拡張子の読み込み要請には全てこのファイルを返す
#define TEST9_READ_PATH_1   _T( "data/test/SkyFileBind/indata/Hoge.txt" )

//デフォルトファイル管理機能のテスト
TEST( file , 9 ) 
{
	SKY_BASE_LIB_INIT();

	//デフォルトファイルをプール
	CDefaultFileManager_::LoadAsync( CHSC( TEST9_DEF_PATH_1 ) );

	//非同期読み込み終了待ち
	while( CDefaultFileManager_::IsLoadEnd() == skyFalse )
	{
		SKY_PRINTF( _T( "デフォルトファイル読込中です。。\n" ) );
	}

	//ファイルをプリント
	CDefaultFileManager_::Print();

	//デフォルトファイル１個を読む
	const u8 *pData = CDefaultFileManager_::LoadFile( CHS( TEST9_READ_PATH_1 ) );
	SKY_PRINTF( _T( "読めた１=[%s] \n" ) , pData ); 

	//デフォルトファイル１個を消す（実際は消さないがskyTrueを返すこと
	CFilePoolContena_::DeleteFile( CHS( TEST9_READ_PATH_1 ) );

	//ファイルをプリント
	CFilePoolContena_::Print();

	SKY_BASE_LIB_TERM();
}

#endif

#if OFF_

#define TEST10_BIND_PATH	_T( "data/test/SkyFileBind/binded.bn" )
#define TEST10_POOL_PATH_1  _T( "data/test/file/TestAsyncRead.txt" )
#define TEST10_DEF_PATH_1   _T( "data/test/SkyFileBind/indata/Test1.txt" ) //.txtという拡張子の読み込み要請には全てこのファイルを返す

#define TEST10_LOAD_PATH1	_T( "data/test/SkyFileBind/indata/Test1.txt" ) //これはバインドファイルで発見されるはず
#define TEST10_LOAD_PATH2	_T( "data/test/file/TestAsyncRead.txt" )       //これはプールファイルで発見される
#define TEST10_LOAD_PATH3	_T( "data/test/file/BindSrcFile1.txt" )        //これはDiscで発見され、プールされて返るはず
//#define TEST10_LOAD_PATH4	_T( "data/test/file/HogeHoge.txt" )            //これはどこにも発見されず、デフォルトファイルが返るはず（でも現在存在しないパスのロードを行うとAssertがでるので無効）


//デフォルトファイル管理機能のテスト
TEST( file , 10 ) 
{
	SKY_BASE_LIB_INIT();

//-----------------準備-----------------------▽

	//結合ファイルを非同期読み込みする
	CBindFileManager_::LoadAsync( CHSC( TEST10_BIND_PATH ) );

	//非同期読み込み終了待ち
	while( CBindFileManager_::IsLoadEnd() == skyFalse )
	{
		SKY_PRINTF( _T( "結合ファイル読込中です。。\n" ) );
	}

	//プールファイルを読み込む
	CFilePoolContena_::LoadAsync( CHSC( TEST10_POOL_PATH_1 ) );

	//非同期読み込み終了待ち
	while( CFilePoolContena_::IsLoadEnd() == skyFalse )
	{
		SKY_PRINTF( _T( "プールファイル読込中です。。\n" ) );
	}

	//デフォルトファイルをプール
	CDefaultFileManager_::LoadAsync( CHSC( TEST10_DEF_PATH_1 ) );

	//非同期読み込み終了待ち
	while( CDefaultFileManager_::IsLoadEnd() == skyFalse )
	{
		SKY_PRINTF( _T( "デフォルトファイル読込中です。。\n" ) );
	}

//-----------------実データ読み込み---------------▽

	const u8 *pData = CDiscFileController_::Load( CHS( TEST10_LOAD_PATH1 ) );
	SKY_PRINTF( _T( "読めた１=[%s] \n" ) , pData ); 

	pData = CDiscFileController_::Load( CHS( TEST10_LOAD_PATH2 ) );
	SKY_PRINTF( _T( "読めた２=[%s] \n" ) , pData ); 

	pData = CDiscFileController_::Load( CHS( TEST10_LOAD_PATH3 ) );
	SKY_PRINTF( _T( "読めた３=[%s] \n" ) , pData ); 

//	const u8 *pData = CDiscFileController_::Load( CHS( TEST10_LOAD_PATH4 ) );
//	SKY_PRINTF( _T( "読めた１=[%s] \n" ) , pData ); 

	//ファイル情報をダンプ
	CBindFileManager_::Print();
	CFilePoolContena_::Print();
	CDefaultFileManager_::Print();

	CDiscFileController_::Delete( CHS( TEST10_LOAD_PATH1 ) );
	CDiscFileController_::Delete( CHS( TEST10_LOAD_PATH2 ) );
	CDiscFileController_::Delete( CHS( TEST10_LOAD_PATH3 ) );

	//ファイル情報をダンプ
	CBindFileManager_::Print();
	CFilePoolContena_::Print();
	CDefaultFileManager_::Print();

	SKY_BASE_LIB_TERM();
}

#endif

#endif