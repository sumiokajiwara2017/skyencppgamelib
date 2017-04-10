#pragma once

namespace sky { namespace lib { namespace locale {

/// <summary>
/// 定数
/// </summary>
enum Country
{
	Country_English = 0,	//英語
	Country_French,			//フランス語
	Country_Japanese,		//日本
	Country_Max,
};


//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
/// ロケール設定＆取得
/// </summary>
/*
setlocale関数の仕様

LC_ALL
以下に示すすべてのカテゴリ。
LC_COLLATE
strcoll、_stricoll、wcscoll、_wcsicoll、strxfrm、_strncoll、_strnicoll、_wcsncoll、_wcsnicoll、および wcsxfrm の各関数。
LC_CTYPE
文字処理関数 (isdigit、isxdigit、mbstowcs、および mbtowc の各関数は除く)。
LC_MONETARY
localeconv 関数から返される通貨形式の情報。
LC_NUMERIC
書式付き出力ルーチン (printf など) とデータ変換ルーチンの小数点文字、および localeconv が返す非通貨形式の情報の小数点文字。LC_NUMERIC は、小数点文字の他に、千単位の桁区切り記号と localeconv 関数が返すグループ化制御文字列も設定します。
LC_TIME
strftime 関数と wcsftime 関数。

*/
class CLocaleUtil : public base::SkyObject
{
public:

	/// <summary>
	/// 文字ロケールを設定
	/// </summary>
	static void SetCharacterLocale	( Country country );

	/// <summary>
	/// 通貨ロケールを設定
	/// </summary>
	static void SetMoneyLocale		( Country country );

	/// <summary>
	/// 数字ロケールを取得
	/// </summary>
	static void SetNumberLocale		( Country country );

	/// <summary>
	/// 時間ロケールを取得
	/// </summary>
	static void SetTimeLocale			( Country country );

	/// <summary>
	/// すべてのロケール
	/// </summary>
	static void SetAllLocale			( Country country );

private:

	static const char *s_ccLocalTbl[ Country_Max ];

	//各機能封印
	CLocaleUtil(){};													//コンストラクタ
	CLocaleUtil( const CLocaleUtil& ){};							//コピーコンストラクタ
	virtual ~CLocaleUtil(){};											//デストラクタ
	CLocaleUtil &operator=(const CLocaleUtil& ){ return *this; };	//代入演算子オーバーロード

};

#define CLocaleUtil_							sky::lib::locale::CLocaleUtil

} } }