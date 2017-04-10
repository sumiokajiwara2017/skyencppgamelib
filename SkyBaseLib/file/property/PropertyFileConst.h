#pragma once

namespace sky { namespace lib { namespace file { namespace property {

	/// <summary>
	/// バージョン情報
	/// </summary>
	static const u32 VERSION_NO = 1;

	/// <summary>
	/// バイナリ識別コード長
	/// </summary>
	static const u32 FILE_CODE_LENGHT = 3;

	/// <summary>
	/// キー値最大サイズ
	/// </summary>
	static const u32 KEY_SIZE_MAX = 64;

	/// <summary>
	/// ハッシュデフォルトサイズ
	/// </summary>
	static const u32 DEFAULt_HASH_SIZE = 31;

	/// <summary>
	/// ディレクトリ検索パス
	/// </summary>
	static const skyString *PROPERTY_SEARCH_PATH = _T( "data/property/*" );

	/// <summary>
	/// プロパティファイルＨＡＳＨサイズ
	/// </summary>
	static const u32 PROPERTY_FILE_HASH_SIZE = 61;


} } } }