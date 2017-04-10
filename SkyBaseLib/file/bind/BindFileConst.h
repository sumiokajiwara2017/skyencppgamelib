#pragma once

namespace sky { namespace lib { namespace file { namespace bind {

	/// <summary>
	/// バージョン情報
	/// </summary>
	const static u32 VERSION_NO = 1;

	/// <summary>
	/// バイナリ識別コード長
	/// </summary>
	const static u32 FILE_CODE_LENGHT = 3;

	/// <summary>
	/// バイナリ識別コード
	/// </summary>
	const static u8	g_cFileCode[ FILE_CODE_LENGHT ] = { 'b', 'n', 'd' };

	/// <summary>
	/// 結合ファイル最大ファイル数
	/// </summary>
	const static u32 FILE_NUM_MAX = 256;

	/// <summary>
	/// 結合ファイルアクセスハッシュのサイズ
	/// </summary>
	const static u32 FILE_HASH_SIZE = 91;

	/// <summary>
	/// デフォルトカテゴリ番号
	/// </summary>
	const static u32 DEFAULT_CATEGORY_NO = 0;

} } } }