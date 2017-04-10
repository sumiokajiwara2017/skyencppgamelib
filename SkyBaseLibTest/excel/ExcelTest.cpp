#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_EXCEL

#include "SkyBaseLib/File.h"

#define EXCEL_FILE_PATH _T( "data/test/excel/TestExcel.xls" )

TEST( excel , 1 ) 
{
	SKY_BASE_LIB_INIT();

	//Excel�N���X���쐬
	sky::lib::file::IExcelPtr excelPtr = sky::lib::file::IExcel::CreateInstance();

	//Excel���J��
	excelPtr->Open( EXCEL_FILE_PATH );

	//�Z����I��
	excelPtr->SelCell( 3 , 3 );

	//�Z���ɕ���������
	excelPtr->SetValue_String( _T( "Test Kajiwara Success !!" ) );

	//�Z�[�u
	excelPtr->Save();

	//Excel�����
	excelPtr->Close();

	SKY_BASE_LIB_TERM();
}

#endif