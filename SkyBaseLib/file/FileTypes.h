#pragma once

/// <summary>
/// file機能共通型(enum、struct、functor、delegate、関数ポインタ型等の定義）
/// </summary>
namespace sky { namespace lib { namespace file { 

/// <summary>
/// ファイルの種類
/// </summary>
enum eFileType
{
	eFileType_Binary = 0,
	eFileType_Text,
	eFileType_Bind,
};

/// <summary>
/// ファイルオープンモード
/// </summary>
enum eOpenMode
{
	eOpenMode_Text_R_NoCreate = 0, //テキストモード読込専用。ファイルが無ければエラー（テキストモードだと勝手に変換が入るので注意）
	eOpenMode_Text_RW_Create,	   //テキストモード読み書き可能。ファイルが無ければ作成、あれば上書き（テキストモードだと勝手に変換が入るので注意） //Openの時に０バイトのファイルにしてしまいます。
	eOpenMode_Binary_R_NoCreate,   //バイナリモード読込専用。ファイルが無ければエラー（テキストモードだと勝手に変換が入るので注意）
	eOpenMode_Binary_RW_Create,	   //バイナリモード読み書き可能。ファイルが無ければ作成、あれば上書き（テキストモードだと勝手に変換が入るので注意） //Openの時に０バイトのファイルにしてしまいます。
};

} } }