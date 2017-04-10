#pragma once

namespace sky { namespace lib { namespace graphic { namespace binary {

	/// <summary>
	/// バージョン情報
	/// </summary>
	static const u32 VERSION_NO = 1;

	/// <summary>
	/// バイナリ識別コード長さ
	/// </summary>
	static const u32 FILE_CODE_LENGHT = 3;

	/// <summary>
	/// ファイルヘッダーマーク
	/// </summary>
	static const skyString *FILE_HEADER_MARK = _T( "fileheadermark" );

} } } }
