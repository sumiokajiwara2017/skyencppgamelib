#pragma once

namespace sky { namespace lib { namespace file { namespace property {

/// <summary>
/// バイナリ識別コード
/// </summary>
const static u8	g_cFileCode[ FILE_CODE_LENGHT ] = { 'p', 'r', 'o' };

/// <summary>
/// ファイルヘッダ
/// </summary>
struct FileHeader
{
	u8		cCode[ FILE_CODE_LENGHT ];			//バイナリ識別コード
	u8		bIsBigEndian;						//ビッグエンディアンフラグ
	u32		nVersion;							//バージョン
	u32		nFileSize;							//ファイルサイズ
	u32		nPropertyCount;						//プロパティ数
};
//このヘッダーの次からデータ部に入る。


/// <summary>
/// データヘッダ
/// データは文字終端で区切られた文字列。※イメージ→『KEY 値１ value2』
/// 最初の文字はキーとなり、以後の文字は値となる。読み込みサイズがvalueSizeをこえるまで読み込む
/// </summary>
struct DataHeader
{
	u32		valueSize;
};

} } } }