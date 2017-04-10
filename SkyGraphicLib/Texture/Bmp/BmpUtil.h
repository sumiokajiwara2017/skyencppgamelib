#pragma once

namespace sky { namespace lib { namespace graphic {

class AbsBmpFile;

//文字スタイル
static const skyString* FONT_STYLE_MS_MINCHO = _T( "MS Mincho" );
static const skyString* FONT_STYLE_MS_GOSIC  = _T( "MS Gothic" );

/// <summary>
/// フォントビットマップ情報
/// </summary>
struct CFontBmpInfo
{
	CColor                  in_color;           //（入力）文字色
	CColor                  in_clearcolor;      //（入力）背景色
	u32                     in_lineNum;         //（入力）１行の文字数
	u32                     in_size;            //（入力）データサイズ
	fontbmp::eGradationType in_eGradation;      //（入力）色階調
	const skyString*        in_pStyle;          //（入力）スタイル
	u32                     in_isBold;          // (入力）太文字か？
	skyBool                 in_isAlphaGradation;//（入力）アルファ値でグラデーションさせるか？
	u32                     in_texWidth;        // (入力）ビットマップ横サイズ（２の累乗）
	u32                     in_texHeight;       // (入力）ビットマップ縦サイズ（２の累乗）
	math::CSize< d64 >      out_outLine;        //（出力）外枠の縦横サイズ
	math::CRect< d64 >      out_rect;           //（出力）位置
    fontbmp::CUvFileHeader* out_pUvData;        // (出力）ＵＶ情報
    u32                     out_uvDataSize;     // (出力）ＵＶ情報サイズ
	u8*                     w_pData;            //（ワーク）データ開始アドレス
	fontbmp::CUvData*       w_pUvData;          //（ワーク）ＵＶデータ開始アドレス

	/// <summary>
	/// コンストラクタ
	/// </summary>
    CFontBmpInfo()
    {
		in_color = graphic::CColor_White;
		in_clearcolor = graphic::CColor_Violetw;
		in_lineNum = 16;
		in_size = 16;
		in_eGradation = fontbmp::eGradationType_2;
		in_pStyle = FONT_STYLE_MS_GOSIC;
		in_isBold = skyFalse;
		w_pData = skyNull;
		in_isAlphaGradation = skyTrue;
	    in_texWidth = 512;
	    in_texHeight = 512;
    }
};

namespace BmpUtil {

/// <summary>
/// Bmpに指定の文字を書き出します。内部処理でBmpは初期化され、lineNumに合わせた横の長さ、全ての文字が収まる縦の長さのデータが生成されます。
/// </summary>
void WriteFontBmp(  AbsBmpFile *bmp , const skyString *data , CFontBmpInfo &bmpinfo );

}

} } }
