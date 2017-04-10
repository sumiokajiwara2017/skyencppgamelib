#pragma once

namespace sky { namespace lib { namespace cons {

/// <summary>
/// プログラム定数の操作
/// プログラムの定数をExcelで記述し、バイナリ化してこのプログラム
/// 読み込みをしています。\lib\skycpplib\media\const以下にExcelが存在します。
/// Excelを使用するとWindows依存なので、いずれ別のファイル形式にするかもしれません。
/// </summary>
namespace PgConst
{
	extern u8 *g_pPgConstDataAdr;

	/// <summary>
	/// プログラム定数の読込
	/// </summary>
	void Load			();

	/// <summary>
	/// プログラム定数の破棄
	/// </summary>
	void Destroy		();

	/// <summary>
	/// プログラム定数の再読込
	/// </summary>
	void ReLoad			();

	/// <summary>
	/// プログラム定数の取得
	/// </summary>
	inline u8 *Get_u8	( pgconstdata id )
	{
		return g_pPgConstDataAdr + id;
	}
	inline s8 Get_s8	( pgconstdata id )
	{
		return *( s8 * )( g_pPgConstDataAdr + id );
	}
	inline u16 Get_u16	( pgconstdata id )
	{
		return *( u16 * )( g_pPgConstDataAdr + id );
	}
	inline s16 Get_s16	( pgconstdata id )
	{
		return *( s16 * )( g_pPgConstDataAdr + id );
	}
	inline u32 Get_u32	( pgconstdata id )
	{
		return *( u32 * )( g_pPgConstDataAdr + id );
	}
	inline s32 Get_s32	( pgconstdata id )
	{
		return *( s32 * )( g_pPgConstDataAdr + id );
	}
	inline const char *Get_Char	( pgconstdata id )
	{
		return ( const char * )( g_pPgConstDataAdr + id );
	}
	inline const skyString *Get_String	( pgconstdata id )
	{
		return ( skyString * )( g_pPgConstDataAdr + id );
	}
}

} } }

//LCはライブラリコンストの略
#define C_S32( key ) sky::lib::cons::PgConst::Get_s32( key )
#define C_ST( key )  sky::lib::cons::PgConst::Get_String( key )
#define C_CHAR( key )  sky::lib::cons::PgConst::Get_Char( key )
