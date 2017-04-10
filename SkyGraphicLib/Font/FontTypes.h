#pragma once

namespace sky { namespace lib { namespace graphic {

namespace font {

//フォントの種類
enum eFontStyleType
{
	eFontStyleType_Gosic11 = 0 ,
	eFontStyleType_Gosic13 ,
//	eFontStyleType_Gosic16 ,
//	eFontStyleType_Mincho11 ,
//	eFontStyleType_Mincho13 ,
//	eFontStyleType_Mincho16 ,
};

static const u32        STYLE_HASH_SIZE   = 11; //スタイルの種類よりちょっと上の値の素数にする
static const u32        FONT_HASH_SIZE    = 691;//１スタイル毎の文字数よりちょっと上の値の素数にする

}

} } }