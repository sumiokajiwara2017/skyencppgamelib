#pragma once

namespace sky { namespace lib { namespace graphic { namespace binary {

	/// <summary>
	/// バイナリ識別コード
	/// </summary>
	const static u8	g_cFileCode[ FILE_CODE_LENGHT ] = { 's', 'g', 'b' };

	/// <summary>
	/// ファイルヘッダ
	/// </summary>
	struct FileHeader
	{
		u8		cCode[ FILE_CODE_LENGHT ];			//バイナリ識別コード
		u8		bIsBigEndian;						//ビッグエンディアンフラグ
		u32		nVersion;							//バージョン
        u32	    nFileSize;							//ファイルサイズ
	};

} } } }
