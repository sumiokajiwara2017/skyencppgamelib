#include "StdAfx.h"
#include "SkyBaseLib/File.h"

namespace sky { namespace lib { namespace file {

#ifdef SKYLIB_COMPILER_VC10

#include <stdio.h>
#include <windows.h>
#include <objbase.h>

//-------------------------------< インターフェース >----------------------------------------//

IExcelPtr IExcel::CreateInstance()
{
	//生成と同時にスマートポインタで管理。
	return IExcelPtr( ( IExcel * )NEW CExcelWin32() );
}

s32 CExcelWin32::getRow()
{
	s32 result = 0;

	HRESULT hr = m_pComExcel->get_Row( &result );
	SKY_UNUSED_ARG( hr );

	SKY_ASSERT_MSG( SUCCEEDED( hr ) , _T( "Processing failed. " ) );

	return result;
}
void CExcelWin32::setRow( s32 row )
{
	HRESULT hr = m_pComExcel->put_Row( row );
	SKY_UNUSED_ARG( hr );

	SKY_ASSERT_MSG( SUCCEEDED( hr ) , _T( "Processing failed. " ) );
}
s32 CExcelWin32::getCol()
{
	s32 result = 0;

	HRESULT hr = m_pComExcel->get_Col( &result );
	SKY_UNUSED_ARG( hr );

	SKY_ASSERT_MSG( SUCCEEDED( hr ) , _T( "Processing failed." ) );

	return result;
}
void CExcelWin32::setCol( s32 col )
{
	HRESULT hr = m_pComExcel->put_Col( col );
	SKY_UNUSED_ARG( hr );

	SKY_ASSERT_MSG( SUCCEEDED( hr ) , _T( "Processing failed." ) );
}
void CExcelWin32::Open( skyString *filePath)
{
	//BSTRに変換
	BSTR bfilePath = SysAllocString( filePath );

	HRESULT hr = m_pComExcel->Open( bfilePath );
	SKY_UNUSED_ARG( hr );

	//BSTRを開放
	SysFreeString( bfilePath );

	SKY_ASSERT_MSG( SUCCEEDED( hr ) , _T( "Processing failed." ) );
}
void CExcelWin32::Close()
{
	HRESULT hr = m_pComExcel->Close();
	SKY_UNUSED_ARG( hr );

	SKY_ASSERT_MSG( SUCCEEDED( hr ) , _T( "Processing failed." ) );
}
skyBool CExcelWin32::IsSheet( skyString *sheetName)
{
	s16 result = skyFalse;

	//BSTRに変換
	BSTR bsheetName = SysAllocString( sheetName );

	HRESULT hr = m_pComExcel->IsSheet( bsheetName , &result );
	SKY_UNUSED_ARG( hr );

	SKY_ASSERT_MSG( SUCCEEDED( hr ) , _T( "Processing failed." ) );

	//BSTRを開放
	SysFreeString( bsheetName );

	return result;
}
void CExcelWin32::SelSheet( skyString *sheetName )
{
	//BSTRに変換
	BSTR bsheetName = SysAllocString( sheetName );

	HRESULT hr = m_pComExcel->SelSheet_2(bsheetName );
	SKY_UNUSED_ARG( hr );

	SKY_ASSERT_MSG( SUCCEEDED( hr ) , _T( "Processing failed." ) );

	//BSTRを開放
	SysFreeString( bsheetName );

}
void CExcelWin32::SelSheet(s32 sheetNo)
{
	HRESULT hr = m_pComExcel->SelSheet( sheetNo );
	SKY_UNUSED_ARG( hr );

	SKY_ASSERT_MSG( SUCCEEDED( hr ) , _T( "Processing failed." ) );
}
void CExcelWin32::SelCell(s32 rowNo, s32 colNo)
{
	HRESULT hr = m_pComExcel->SelCell( rowNo , colNo );
	SKY_UNUSED_ARG( hr );

	SKY_ASSERT_MSG( SUCCEEDED( hr ) , _T( "Processing failed." ) );
}
skyBool CExcelWin32::EqualsCell( skyString *key)
{
	s16 result = skyFalse;

	//BSTRに変換
	BSTR bkey = SysAllocString( key );

	HRESULT hr = m_pComExcel->EqualsCell( bkey , &result );
	SKY_UNUSED_ARG( hr );

	SKY_ASSERT_MSG( SUCCEEDED( hr ) , _T( "Processing failed." ) );

	//BSTRを開放
	SysFreeString( bkey );

	return result;
}
skyBool CExcelWin32::IndexOfCell( skyString *key)
{
	s16 result = skyFalse;

	//BSTRに変換
	BSTR bkey = SysAllocString( key );

	HRESULT hr = m_pComExcel->IndexOfCell( bkey , &result );
	SKY_UNUSED_ARG( hr );

	SKY_ASSERT_MSG( SUCCEEDED( hr ) , _T( "Processing failed." ) );

	//BSTRを開放
	SysFreeString( bkey );

	return result;
}
skyBool CExcelWin32::IsCellEmpty()
{
	s16 result = skyFalse;

	HRESULT hr = m_pComExcel->IsCellEmpty( &result );
	SKY_UNUSED_ARG( hr );

	SKY_ASSERT_MSG( SUCCEEDED( hr ) , _T( "Processing failed." ) );

	return result;
}
void CExcelWin32::NextCol()
{
	HRESULT hr = m_pComExcel->NextCol();
	SKY_UNUSED_ARG( hr );

	SKY_ASSERT_MSG( SUCCEEDED( hr ) , _T( "Processing failed." ) );
}
void CExcelWin32::NextRow()
{
	HRESULT hr = m_pComExcel->NextRow();
	SKY_UNUSED_ARG( hr );

	SKY_ASSERT_MSG( SUCCEEDED( hr ) , _T( "Processing failed." ) );
}
const skyString *CExcelWin32::GetValue_String()
{
	skyString *result = 0;

	HRESULT hr = m_pComExcel->GetValue_String( &result );
	SKY_UNUSED_ARG( hr );

	SKY_ASSERT_MSG( SUCCEEDED( hr ) , _T( "Processing failed." ) );

	return result;
}
s32 CExcelWin32::GetValue_Int()
{
	s32 result = 0;
	return result;
}
void CExcelWin32::SetValue_String( skyString *value)
{
	//BSTRに変換
	BSTR bvalue = SysAllocString( value );

	HRESULT hr = m_pComExcel->SetValue_String( bvalue );
	SKY_UNUSED_ARG( hr );

	SKY_ASSERT_MSG( SUCCEEDED( hr ) , _T( "Processing failed." ) );

	//BSTRを開放
	SysFreeString( bvalue );
}
void CExcelWin32::SetValue_Int(s32 value)
{
	HRESULT hr = m_pComExcel->SetValue_Int( value );
	SKY_UNUSED_ARG( hr );

	SKY_ASSERT_MSG( SUCCEEDED( hr ) , _T( "Processing failed." ) );
}
void CExcelWin32::Save()
{
	HRESULT hr = m_pComExcel->Save();
	SKY_UNUSED_ARG( hr );

	SKY_ASSERT_MSG( SUCCEEDED( hr ) , _T( "Processing failed." ) );
}
void CExcelWin32::SaveAs( skyString *filePath )
{
	//BSTRに変換
	BSTR bfilePath = SysAllocString( filePath );

	HRESULT hr = m_pComExcel->SaveAs( bfilePath );
	SKY_UNUSED_ARG( hr );

	SKY_ASSERT_MSG( SUCCEEDED( hr ) , _T( "Processing failed." ) );

	//BSTRを開放
	SysFreeString( bfilePath );
}

CExcelWin32::CExcelWin32()
{
	//ComExcelの生成
	skycslib::IExcelPtr pComExcel( __uuidof( skycslib::CSExcel ) );

	//メンバに保存
	m_pComExcel = pComExcel;
}
CExcelWin32::~CExcelWin32	()
{
	//COMの開放
	m_pComExcel->Release();
}

#endif

} } }