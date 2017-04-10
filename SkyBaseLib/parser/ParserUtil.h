#pragma once

namespace sky { namespace lib { namespace parser {

static const u32 TOKEN_NUM_MAX    = 64; //トークンの最大数
static const u32 TOKEN_LENGTH_MAX = 64; //１トークンの最大長さ
static const u32 KEY_LENGHT_MAX   = 32;
static const u32 VALUE_LENGHT_MAX = TOKEN_LENGTH_MAX - KEY_LENGHT_MAX;


/// <summary>
/// 文字列解析便利処理群
/// </summary>
class CParserUtil : public base::SkyObject
{

public:

	/// <summary>
	/// 文字列解析パターン０
	/// 『 a b c d e 』この文字列を
	/// 『 a , b , c , d 』というCString型のリストにする。
	/// </summary>
	static void ParseStringPtn0( const skyWString *src , string::CStringList &destlist );
	static void ParseStringPtn0( const skyMString *src , string::CStringList &destlist );

	/// <summary>
	/// 文字列解析パターン１
	/// 『 hoge011_mog99_g100m_ll99m 』この文字列を
	/// 『 hoge=1.0f , mog=99.0f , g=-100.0f , ll=-99.0f 』というd64型のマップにする。数字の末尾にmが付いていたらマイナス
	/// </summary>
	static void ParseStringPtn1( const skyWString *src , CValues32Map &destmap );
	static void ParseStringPtn1( const skyMString *src , CValues32Map &destmap );

	/// <summary>
	/// 文字列解析パターン２
	/// 『 _bit0100_ 』この文字列を
	/// 『 list[0]=skyTrue , list[1]=skyTrue , list[2]=skyFalse , list[3]=skyFalse 』というskyBool型のリストにする
	/// 『bit』の部分はkeyWordで任意の文字列を指定できる
	/// </summary>
	static void ParseStringPtn2( const skyWString *src , const skyWString *keyWord , CValueBoolList &destlist );
	static void ParseStringPtn2( const skyMString *src , const skyMString *keyWord , CValueBoolList &destlist );

	/// <summary>
	/// 文字列解析パターン３
	/// 『 _flg9185_ 』この文字列を
	/// 『 list[0]=9 , list[1]=1 , list[2]=8 , list[3]=5 』というu8型のリストにする
	/// 『flg』の部分はkeyWordで任意の文字列を指定できる
	/// </summary>
	static void ParseStringPtn3( const skyWString *src , const skyWString *keyWord , CValueu8List &destlist );
	static void ParseStringPtn3( const skyMString *src , const skyMString *keyWord , CValueu8List &destlist );

};

#define CParserUtil_			sky::lib::parser::CParserUtil

} } }