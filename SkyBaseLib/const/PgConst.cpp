#include "StdAfx.h"
#include "SkyBaseLib/Const.h"

#include "SkyBaseLib/File.h"

namespace sky { namespace lib { namespace cons {

/// <summary>
/// プログラム定数バイナリパス
/// </summary>
static const skyString *PgConstBinaryPath = _T( "Data/lib/const/pgconstdata.bin" );

namespace PgConst {

	u8 *g_pPgConstDataAdr = NULL;
	static u8 *g_pPgConstDataAdrOriginal = NULL;

void Load()
{
	u32 size = 0;
	if ( file::Native_FileRead( PgConstBinaryPath , &g_pPgConstDataAdrOriginal , &size ) == skyFalse )
	{
		SKY_PANIC_MSG( _T("The program constant binary( %s ) was not able to be loaded."), PgConstBinaryPath ); //プログラム定数バイナリをロードできませんでした。%sがあることを確認してください。
	}
	g_pPgConstDataAdr = g_pPgConstDataAdrOriginal + sizeof ( ExcelConvBinType2Header ); 
}

void Destroy()
{
	FREE__( g_pPgConstDataAdrOriginal );
}

void ReLoad()
{
	Destroy();
	Load();
}

}

} } }