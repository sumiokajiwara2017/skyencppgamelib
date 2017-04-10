#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_EXCEL

#include "SkyBaseLib/File.h"

#define EXCEL_FILE_PATH _T( "data/test/excel/TestExcel.xls" )

TEST( excel , 1 ) 
{
	SKY_BASE_LIB_INIT();

	//Excelクラスを作成
	sky::lib::file::IExcelPtr excelPtr = sky::lib::file::IExcel::CreateInstance();

	//Excelを開く
	excelPtr->Open( EXCEL_FILE_PATH );

	//セルを選択
	excelPtr->SelCell( 3 , 3 );

	//セルに文字を書く
	excelPtr->SetValue_String( _T( "Test Kajiwara Success !!" ) );

	//セーブ
	excelPtr->Save();

	//Excelを閉じる
	excelPtr->Close();

	SKY_BASE_LIB_TERM();
}

#endif