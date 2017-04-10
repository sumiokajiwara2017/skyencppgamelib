#pragma once

namespace sky { namespace lib { namespace file { namespace bind {

/// <summary>
/// ファイルヘッダ
/// </summary>
struct FileHeader
{
	u8		cCode[ FILE_CODE_LENGHT ];			//バイナリ識別コード
	u8		bIsBigEndian;						//ビッグエンディアンフラグ
	u32		nVersion;							//バージョン
	u32		nFileSize;							//ファイルサイズ
	u32		nFileBindCount;						//ファイルバインド数
};
//このヘッダーの次からデータ部に入る。


/// <summary>
/// データヘッダ
/// </summary>
struct DataHeader
{
	u32		nCompressFlg;						//圧縮フラグ(0:非圧縮 1:zip圧縮)
	u8		cFilePath[ PATH_LEN_MAX ];		//ファイルパス
	u32		fileSize;							//ファイルサイズ
};

} } } }