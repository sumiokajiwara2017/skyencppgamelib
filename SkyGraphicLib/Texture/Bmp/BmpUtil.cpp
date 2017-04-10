#include "StdAfx.h"
#include "SkyGraphicLib/Texture.h"

namespace sky { namespace lib { namespace graphic {

namespace BmpUtil {

static u32 ConvertGradationType_( fontbmp::eGradationType eType )
{
	switch ( eType )
	{
		case fontbmp::eGradationType_2 :
			return GGO_BITMAP;
			break;
		case fontbmp::eGradationType_5 :
			return GGO_GRAY2_BITMAP;
			break;
		case fontbmp::eGradationType_17 :
			return GGO_GRAY4_BITMAP;
			break;
		case fontbmp::eGradationType_65 :
			return GGO_GRAY8_BITMAP;
			break;
		default:
			SKY_PANIC_MSG( _T( "It is a bit map of the uncorrespondence." ) ); //未対応のビットマップです。
	}

	return GGO_GRAY2_BITMAP;
}

void WriteFontBmp(  AbsBmpFile *bmp , const skyString *data , CFontBmpInfo &bmpinfo )
{
	u32 fontLen = CStringUtil_::Len( data );

	// フォント情報
	HFONT hFont = CreateFont(
		   bmpinfo.in_size,             // フォントの高さ
           0,                        // 平均文字幅
           0,                        // 文字送り方向の角度
           0,                        // ベースラインの角度
		   ( bmpinfo.in_isBold )? 700 : 400,                      // フォントの太さ(フォントの太さを表す 0 から 1000 までの範囲内の値を指定します。たとえば、400 を指定すると標準の太さになり、700 を指定すると太字になります。0 を指定すると、既定の太さが選択されます。)
           FALSE,                    // 斜体にするかどうか
           FALSE,                    // 下線を付けるかどうか
		   FALSE,                    // 取り消し線を付けるかどうか
           SHIFTJIS_CHARSET,         // 文字セットの識別子
           OUT_TT_ONLY_PRECIS,       // 出力精度(出力精度を指定します。出力精度は、実際の出力が、要求されたフォントの高さ、幅、文字の向き、文字送りの方向、ピッチ、およびフォント名にどの程度一致していなければならないかを示します。)
           CLIP_DEFAULT_PRECIS,      // クリッピング精度(クリッピング精度を指定します。クリッピング精度とは、文字の一部がクリッピング領域の外にはみ出たときに、その文字をクリップする方法を定義するものです)
           PROOF_QUALITY,            // 出力品質(出力品質を指定します。出力品質とは、GDI が、論理フォントの属性と実際の物理フォントの属性とをどの程度まで一致させなければならないかを定義するものです)
           FIXED_PITCH | FF_MODERN,  // ピッチとファミリ
           bmpinfo.in_pStyle               // フォント名
	);

	math::CBasicVector2 vec( 0 , 0 );
	for ( u32 i = 0 ; i < fontLen ; i++ )
	{
		// デバイスコンテキスト取得
		// フォントを持たせないとGetGlyphOutline関数はエラーとなる
		HDC hdc = GetDC( NULL );
		HFONT oldFont = ( HFONT )SelectObject( hdc , hFont );

		UINT code;

#ifdef SW_SKYLIB_MEMORY_TEXT_UNICODE
	   // unicodeの場合、文字コードは単純にワイド文字のUINT変換です
       code = (UINT)data[ i ];
#else
	   // マルチバイト文字の場合、
	   // 1バイト文字のコードは1バイト目のUINT変換、
	   // 2バイト文字のコードは[先導コード]*256 + [文字コード]です
	  if( IsDBCSLeadByte( *c ) )
          code = ( BYTE )data[ i ] << 8 | ( BYTE )data[ i ];
	  else
		  code = data[ i ];
#endif

		//改行コードの場合は次の行へ
		if ( i > 0 && ( code == EC || bmpinfo.in_lineNum == vec.X ) )
		{
			vec.Y += 1;
			vec.X =  0;
			continue;
		}

		// ビットマップ取得
		TEXTMETRIC TM;
		GetTextMetrics( hdc, &TM );
		GLYPHMETRICS GM;
		CONST MAT2 Mat = { { 0 , 1 } , { 0 , 0 } , { 0 , 0 } , { 0 , 1 } };
		u32 d3dGradType = ConvertGradationType_( bmpinfo.in_eGradation );
		DWORD size = GetGlyphOutline( hdc , code , d3dGradType , &GM , 0 , NULL , &Mat );
		BYTE *ptr = NEW_ARRAY__( BYTE , size );
		GetGlyphOutline( hdc, code, d3dGradType, &GM, size, ptr, &Mat );

		//文字ビットマップ情報設定
		bmpinfo.w_pData        = ptr;
		bmpinfo.out_outLine.W  = GM.gmCellIncX;
		bmpinfo.out_outLine.H  = ( dectype )TM.tmHeight;
		bmpinfo.out_rect.X()   = ( dectype )GM.gmptGlyphOrigin.x;
		bmpinfo.out_rect.Y()   = ( dectype )TM.tmAscent - GM.gmptGlyphOrigin.y;
		bmpinfo.out_rect.W()   = ( dectype )GM.gmBlackBoxX;
		bmpinfo.out_rect.H()   = ( dectype )GM.gmBlackBoxY;

		//最初の１文字描画の際、全文字を収められてかつ２の２乗の縦横サイズを計算して、そのサイズ分Bmpを作成する
		if ( i == 0 )
		{
			//bmpは初期化
			bmp->Reset();

			//ビットマップ作成(とりあえず黒で）
			bmp->vCreate( bmpinfo.in_texWidth, bmpinfo.in_texHeight , 0 );

			//階調に合わせてパレットを作る（インデックスカラー以外はスルー）
			bmp->vMakeColorPallet( bmpinfo );

			//初期化
			bmp->vWriteFontInit( bmpinfo );

			//UV情報を作成
			bmpinfo.out_uvDataSize = sizeof( fontbmp::CUvFileHeader ) + ( sizeof( fontbmp::CUvData ) * fontLen );
			bmpinfo.out_pUvData = ( fontbmp::CUvFileHeader * )SkyMalloc( bmpinfo.out_uvDataSize );
            CMemoryUtil_::Copy( ( *bmpinfo.out_pUvData ).code , fontbmp::CODE , sizeof( fontbmp::CODE ) );
			bmpinfo.out_pUvData->num = fontLen;
			bmpinfo.out_pUvData->version = fontbmp::VERSION;
			bmpinfo.out_pUvData->num = fontLen;
			bmpinfo.out_pUvData->eKind = bmp->GetKind();
			bmpinfo.out_pUvData->eGrad = bmpinfo.in_eGradation;
			bmpinfo.out_pUvData->isAlphaGrad = bmpinfo.in_isAlphaGradation;
			bmpinfo.out_pUvData->fileWidth  = ( dectype )bmpinfo.in_texWidth;
			bmpinfo.out_pUvData->fileHeight = ( dectype )bmpinfo.in_texHeight;
			bmpinfo.out_pUvData->fontWidth  = ( dectype )bmpinfo.out_outLine.W; //これはどの文字も同じはず
			bmpinfo.out_pUvData->fontHeight = ( dectype )bmpinfo.out_outLine.H; //これはどの文字も同じはず
		}

		//UV情報を構築する
		fontbmp::CUvData *pUvData = ( ( fontbmp::CUvData * )( bmpinfo.out_pUvData + 1 ) ) + i;
		pUvData->code = code;

		//ビットマップに書きだす
		bmpinfo.w_pUvData = pUvData;
		bmp->vWriteFont( vec , bmpinfo );

		//１文字すすめる
		vec.X += 1;

		// オブジェクトの開放
		SelectObject( hdc , oldFont );
		ReleaseDC( NULL , hdc );

		// 後片付け
		delete[] ptr;
	}

	//文字定義を破棄
	DeleteObject( hFont );

}

}

} } }