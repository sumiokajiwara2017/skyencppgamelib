#pragma once

namespace sky { namespace lib { namespace comp {

static const u32 VERSION		 = 1;		//バージョン
static const u32 BIN_CODE_LENGHT = 4;		//バイナリ識別コード長

/// <summary>
/// バイナリ識別コード
/// </summary>
const static u8	g_cFileCode[ BIN_CODE_LENGHT ] = { 'c', 'o', 'm' , 'p' };

/// <summary>
/// ファイルヘッダ
/// </summary>
struct FileHeader
{
	u8		cCode[ BIN_CODE_LENGHT ];			//バイナリ識別コード
	u8		al[3];								//アライメント
	u32		nVersion;							//バージョン
	u32		nOrgDataSize;						//圧縮前データサイズ
};

} } }