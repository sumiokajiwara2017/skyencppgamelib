#include "StdAfx.h"
#include "SkyGraphicLib/Texture.h"

namespace sky { namespace lib { namespace graphic {

#ifdef SKYLIB_IDE_VC10

void CBmp32BitColor::Clear( u32 abgr )
{
	DrawRect( m_Rect, abgr );
}

u32 CBmp32BitColor::GetDataSize()
{
	return m_pBmpInfo->bmiHeader.biWidth * m_pBmpInfo->bmiHeader.biHeight * cbmp32bitcolor::ONE_PIXEL_BYTE_SIZE;
}

void CBmp32BitColor::DrawPoint( const math::CBasicVector2 &pos , u32 abgr )
{
	//データが下から始まっているのでY座標を反転させる
	u32 dotPosY = ( u32 )( (m_Rect.GetSize().H-1)-pos.Y);

	//点内外判定
	if ( m_Rect.DotHitCheck( math::CBasicVector2( pos.X , ( dectype )dotPosY ) ) == skyFalse ) return;

	u32 Tgt = ( u32 )( ( m_LineWidth * dotPosY + pos.X ) );
	stdPixelInfo*pData = &( (stdPixelInfo*)m_pDataAdr)[ Tgt ];
	u8*	pInputData = ( u8 * )&abgr;
	pData->r = pInputData[ 0 ];
	pData->g = pInputData[ 1 ];
	pData->b = pInputData[ 2 ];
	pData->a = pInputData[ 3 ];
}

void CBmp32BitColor::DrawPoint( const math::CBasicVector2 &pos , u32 palletNo, u8 boldSize )
{
	//マイナス座標の場合リターン
	if ( pos.X < 0 || pos.Y < 0 ) return;

	// 描画の開始座標から横幅、縦幅を決める
	s32 Width	= (s32)(pos.X+boldSize);
	s32 Height	= (s32)(pos.Y+boldSize);
	
	// 点を打ち込んでいく
	u16 texDotX = ( u16 )pos.X;
	u16 texDotY = ( u16 )pos.Y;
	for ( ; texDotX < Width; texDotX++ )
	{
		//幅がテクスチャのインデックスを超えたら無効
		if ( texDotX >= m_Rect.GetSize().W ) continue;

		for ( ; texDotY < Height; texDotY++ )
		{
			//高さがテクスチャのインデックスを超えたら無効
			if ( texDotY >= m_Rect.GetSize().H ) continue;
	
			DrawPoint( math::CBasicVector2( texDotX , texDotY ) , palletNo );
		}
		texDotY = ( u16 )pos.Y;
	}
}

void CBmp32BitColor::DrawLine( const math::CBasicVector2& start, math::CBasicVector2& Gole , u32 abgr, u8 boldSize )
{
	s32 offsetX ,offsetY,distNum ,drawBasePosX ,drawBasePosY;
	u32 twoPointDist;
	d64	test1,test2,testX ,testY;

	// ２点間距離の２乗を算出
	offsetX = (s32)(start.X - Gole.X);
	offsetY = (s32)(start.Y - Gole.Y);
	twoPointDist = ( u32 )( (offsetX*offsetX) + (offsetY*offsetY) );
	
	// ２点間距離を元に、等間隔で何回塗るかを算出
	distNum = (s32)( (twoPointDist / 2) + 1 );
	
	// 補完して塗っていく
	for ( s32 i = 1; i <= distNum; i++ )
	{
		test1 = ( ( d64 )offsetX/( d64 )distNum);
		test2 = ( ( d64 )offsetY/( d64 )distNum);
	
		testX = test1*i;
		testY = test2*i;
	
		drawBasePosX = (s32)(Gole.X + testX);
		drawBasePosY = (s32)(Gole.Y + testY);
		
		DrawPoint( math::CBasicVector2( ( dectype )drawBasePosX , ( dectype )drawBasePosY ) , abgr, boldSize );
	}
}

void CBmp32BitColor::DrawTile( CBmp32BitColor &bmpFile , math::CBasicVector2 &point, u32 nocpyColor )
{
	dectype width	= bmpFile.GetRect().GetSize().W;
	dectype height	= bmpFile.GetRect().GetSize().H;
	dectype posX	= point.X;
	dectype posY	= point.Y;
	s32 palNo	= 0;
	d64 tileWNum = m_Rect.GetSize().W / bmpFile.GetRect().GetSize().W;
	d64 tileHNum = m_Rect.GetSize().H / bmpFile.GetRect().GetSize().H;
	
	for( u32 tw = 0; tw<tileWNum; tw++ )
	{
		for( u32 th = 0; th<tileHNum; th++ )
		{
			for ( dectype x=0; x<width; x++ )
			{
				for ( dectype y=0; y<height; y++ )
				{
					palNo = bmpFile.GetPoint( math::CBasicVector2(x , y) );
					if( ( s32 )nocpyColor == cbmp4bitcolor::NO_DRAW_PALLET_IDX_NONE || ( s32 )nocpyColor != palNo )
					{
						DrawPoint( math::CBasicVector2( x+posX+(tw*width) , y+posY+(th*height) ) , palNo );
					}
				}
			}
		}
	}
}

void CBmp32BitColor::DrawRect( const math::CBasicRect& rect, u32 abgr )
{
	u32 width	= ( u32 )rect.W();
	u32 height	= ( u32 )rect.H();
	u32 posX	= ( u32 )rect.X();
	u32 posY	= ( u32 )rect.Y();
	for ( u32 x=0; x<width; x++ )
	{
		for ( u32 y=0; y<height; y++ )
		{
			DrawPoint( math::CBasicVector2( ( dectype )(x+posX) , ( dectype )(y+posY) ) , abgr, 1 );
		}
	}
}

void CBmp32BitColor::DrawTriangle( const math::CBasicTriangle3& tri, u32 abgr )
{
	abgr;
	tri;
}

void CBmp32BitColor::DrawCircle( const math::CBasicCircle& circle , u32 abgr )
{
	u32 width	= ( u32 )m_Rect.GetSize().W;
	u32 height	= ( u32 )m_Rect.GetSize().H;
	u32 posX	= ( u32 )circle.X();
	u32 posY	= ( u32 )circle.Y();
	u32 radius_2 = ( u32 )(circle.R() * circle.R() );

	for ( u32 y=0; y<height; y++ )
	{
		for ( u32 x=0; x<width; x++ )
		{
			if ( ( (y-posY)*(y-posY) + (x-posX)*(x-posX) ) < radius_2 )
			{
				DrawPoint( math::CBasicVector2( ( dectype )x , ( dectype )y) , abgr, 1 );
			}
		}
	}
}

u32 CBmp32BitColor::GetPoint( const math::CBasicVector2 &pos )
{
	//点内外判定
	if ( m_Rect.DotHitCheck( math::CBasicVector2( pos.X , pos.Y ) ) == skyFalse )
	{
		SKY_PANIC_MSG( _T( "It is beyond the limits of the bit map." ) ); //Ｂｍｐの外の点の色情報を取得しようとしました。
	}

	u32 Tgt = ( u32 )( ( m_pBmpInfo->bmiHeader.biWidth *pos.Y + pos.X ) );
	stdPixelInfo*pData = &( (stdPixelInfo*)m_pDataAdr)[ Tgt ];
	return MK_4BYTE_ID( pData->r, pData->g , pData->b, 255 );
}

void CBmp32BitColor::vLoad( stream::IStreamPtr &src )
{
	//リセット
	Reset();

	//データを全コピーする
	m_pData = src->vCloneData();

	//データサイズを取得
	m_nDataSize = src->vGetSize();

	//ファイルヘッダーを取る
	m_pHed = ( BITMAPFILEHEADER * )m_pData;

	//念のためにファイルサイズを上書きする（この処理は・・？）
	m_pHed->bfSize = m_nDataSize;

	//基本情報を得る
	m_pBmpInfo	= ( BITMAPINFO * )( m_pData + sizeof( BITMAPFILEHEADER ) );

	// データ部先頭アドレス取得
	m_pDataAdr = m_pData + m_pHed->bfOffBits;

	//矩形情報の構築
	m_Rect.Set( math::CBasicVector2( 0.0f , 0.0f ) , math::CBasicSize( ( dectype )m_pBmpInfo->bmiHeader.biWidth, ( dectype )m_pBmpInfo->bmiHeader.biHeight ) );

	//幅はアライメント取る
	m_LineWidth = PAGE_ALIGN( m_Rect.GetSize().W, cbmp32bitcolor::LINE_ALIGN_SIZE );
}

void CBmp32BitColor::vSave( stream::IStreamPtr &dest )
{
	//すべてストリームに書きだす
	dest->vWriteStream( m_pData , m_nDataSize );
}

void CBmp32BitColor::vCreate( u32 width, u32 height , u32 abgr )
{
	//幅はアライメント取る
	m_LineWidth = PAGE_ALIGN( width, cbmp32bitcolor::LINE_ALIGN_SIZE );

	u32 dwSizeImage = ( m_LineWidth * height * cbmp32bitcolor::ONE_PIXEL_BYTE_SIZE );

	//サイズを取得
	m_nDataSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + dwSizeImage;

	//DIB用バッファ確保(BITMAPFILEHEADER
	m_pData = ( u8 * )SkyMalloc( m_nDataSize );

	//ファイルヘッダーを作成
	m_pHed = ( BITMAPFILEHEADER * )m_pData;
	CMemoryUtil_::ClearZero( m_pHed , sizeof( BITMAPFILEHEADER ) );
	m_pHed->bfType    = *( LPWORD )"BM";
	m_pHed->bfSize    = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + dwSizeImage;
	m_pHed->bfOffBits = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER );

	//基本情報を作成
	m_pBmpInfo	= ( BITMAPINFO * )( m_pData + sizeof( BITMAPFILEHEADER ) );
	CMemoryUtil_::ClearZero( m_pBmpInfo , sizeof( BITMAPINFOHEADER ) );
	m_pBmpInfo->bmiHeader.biSize		= sizeof( BITMAPINFOHEADER );
	m_pBmpInfo->bmiHeader.biWidth		= width;
	m_pBmpInfo->bmiHeader.biHeight		= height;
	m_pBmpInfo->bmiHeader.biPlanes		= 1;
	m_pBmpInfo->bmiHeader.biBitCount	=  cbmp32bitcolor::ONE_PIXEL_BIT_SIZE;				//1ピクセルあたりの情報量4bitつまり1バイトにつき2ピクセル
	m_pBmpInfo->bmiHeader.biCompression = BI_RGB;

	// データ部先頭アドレス取得
	m_pDataAdr = ( u8 * )( m_pData + m_pHed->bfOffBits );
	CMemoryUtil_::ClearZero( m_pDataAdr , dwSizeImage );

	//矩形情報の構築
	m_Rect.Set( math::CBasicVector2( 0.0f , 0.0f ) , math::CBasicSize( ( dectype )width, ( dectype )height ) );

	Clear( abgr );
}

static u32 GetDataColor( u32 data , const CFontBmpInfo &info )
{
	u32 result = 0xFFFFFF;
	u8* pResult = ( u8 * )&result;

	f32 X = ( f32 )data / info.in_eGradation;

	if ( info.in_isAlphaGradation == skyTrue )
	{
		//データが０の場合は背景色
		if ( data == 0 )
		{
			pResult[ 0 ] = CFTOI( info.in_clearcolor.b );
			pResult[ 1 ] = CFTOI( info.in_clearcolor.g );
			pResult[ 2 ] = CFTOI( info.in_clearcolor.r );
			pResult[ 3 ] = 0; //完全透明
		}
		else
		{
			pResult[ 0 ] = CFTOI( info.in_color.b );
			pResult[ 1 ] = CFTOI( info.in_color.g );
			pResult[ 2 ] = CFTOI( info.in_color.r );
			pResult[ 3 ] = CFTOI( X );
		}
	}
	else
	{
		pResult[ 0 ] = CFTOI( ( ( info.in_color.b - info.in_clearcolor.b ) * X + info.in_clearcolor.b ) );
		pResult[ 1 ] = CFTOI( ( ( info.in_color.g - info.in_clearcolor.g ) * X + info.in_clearcolor.g ) );
		pResult[ 2 ] = CFTOI( ( ( info.in_color.r - info.in_clearcolor.r ) * X + info.in_clearcolor.r ) );
		pResult[ 3 ] = 0xFF; //色で階調をもたせているので完全不透明にしている。
	}

	return *( ( u32 * )&result );
}

void CBmp32BitColor::vWriteFontInit( const CFontBmpInfo &info )
{
	//背景色でクリア（インデックスカラーはパレット０が背景色）
	Clear( info.in_clearcolor.GetAsABGR() );
}

void CBmp32BitColor::vWriteFont( const math::CBasicVector2 &pos , const CFontBmpInfo &info )
{
	info.w_pUvData->x = ( u32 )( info.out_outLine.W * pos.X );
	info.w_pUvData->y = ( u32 )( info.out_outLine.H * pos.Y );
	info.w_pUvData->w = ( u32 )( info.out_pUvData->fontWidth );
	info.w_pUvData->h = ( u32 )( info.out_pUvData->fontHeight );

	info.w_pUvData->ult = info.w_pUvData->x / info.out_pUvData->fileWidth;                                       //左上Ｕ
	info.w_pUvData->vlt = info.w_pUvData->y / info.out_pUvData->fileHeight;                                      //左上Ｖ
	info.w_pUvData->urt = ( info.w_pUvData->x + info.out_pUvData->fontWidth ) / info.out_pUvData->fileWidth;     //右上Ｕ
	info.w_pUvData->vrt = info.w_pUvData->vlt;                                                                   //右上Ｖ
	info.w_pUvData->ulb = info.w_pUvData->ult;                                                                   //左下Ｕ
	info.w_pUvData->vlb = ( info.w_pUvData->y + info.out_pUvData->fontHeight ) / info.out_pUvData->fileHeight;   //左下Ｖ
	info.w_pUvData->urb = info.w_pUvData->urt;
	info.w_pUvData->vrb = info.w_pUvData->vlb;

	//ビットマップの幅とＸ座標と文字書き出しＸ座標からテクスチャ書き出しＸ座標を算出する
	u32 sx = ( u32 )( info.out_outLine.W * pos.X + info.out_rect.X() );

	//ビットマップの幅とＹ座標と文字書き出しＹ座標からテクスチャ書き出しＹ座標を算出する
	u32 sy = ( u32 )( info.out_outLine.H * pos.Y + info.out_rect.Y() );

	if ( info.in_eGradation == fontbmp::eGradationType_2 )
	{//２階調限定ビットマップ書き込み処理

		// ビットマップ一文字の横の長さをバイト換算
		//（理屈：2階調データはビットで文字を表現している。しかしビットマップは4byteアライメントなので使わなくても
		//  最低4バイトある。4バイトは32bitなので文字幅が32までは最低の4byteで表現できる）
		u32 aw = PAGE_ALIGN( 1 + ( info.out_rect.W() / 32 ) , 4 );
		for ( u32 y = 0 ; y < info.out_rect.H() ; y++ )
		{
            for ( u32 x = 0 ; x < info.out_rect.W() ; x++ )
			{
				//データからビット情報を取得
				DWORD num = x / 8;                                    // 現在のxが1行の何BYTE目かを算出
				BYTE bit  = x % 8;                                    // 現在のxが1バイト内の何ビット目かを算出
				BYTE mask = ( ( BYTE ) 1 ) << ( 7 - bit );            // 現在のビット数のマスク作成
				BYTE Cur  = *( ( BYTE * )info.w_pData + aw * y + num ); // 現在のバイト位置にあるビットマップをを取得
				Cur      &= mask;                                     // 作成したマスクと現在のバイト位置とで論理積演算
				u32 p     = ( Cur >> ( 7-bit ) );                     // Curに立ったビットフラグを最下位ビットまでシフトしてアルファ値に変換
				DrawPoint( math::CBasicVector2( ( dectype )( x + sx ) , ( dectype )( y + sy ) ) , GetDataColor( p , info ) );
			}
		}
	}
	else
	{//２階調より上の階調のビットマップ書き込み処理

		// ビットマップ一文字の横の長さを計算
		u32 aw = PAGE_ALIGN( info.out_rect.W() , 4 );
		for ( u32 y = 0 ; y < info.out_rect.H() ; y++ )
		{
			for ( u32 x = 0 ; x < aw ; x++ )
			{
				//データからビット情報を取得
				u32 p = ( info.w_pData[ x + aw * y ] );
				DrawPoint( math::CBasicVector2( ( dectype )( x + sx ) , ( dectype )( y + sy ) ) , GetDataColor( p , info ) );
			}
		}
	}
}

#endif

} } }
