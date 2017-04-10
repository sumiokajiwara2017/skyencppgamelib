#include "StdAfx.h"
#include "SkyGraphicLib/Texture.h"

namespace sky { namespace lib { namespace graphic {

#ifdef SKYLIB_IDE_VC10

using namespace cbmp4bitcolor;

CBmp4BitColor::CBmp4BitColor() :
AbsBmpFile( fontbmp::eBmpKind_bmp4bit ) ,
m_NoDrawPalletIdx( cbmp4bitcolor::NO_DRAW_PALLET_IDX_NONE )
{
}

u32 CBmp4BitColor::GetDataSize()
{
	return ( u32 )(m_pBmpInfo->bmiHeader.biWidth * m_pBmpInfo->bmiHeader.biHeight * cbmp4bitcolor::ONE_PIXEL_BYTE_SIZE);
}

void CBmp4BitColor::Clear( u32 palletNo )
{
	DrawRect( m_Rect, palletNo );
}

void CBmp4BitColor::DrawPoint( const math::CBasicVector2 &pos , u32 palletNo )
{
	//データが下から始まっているのでY座標を反転させる
	dectype dotPosY = (m_Rect.GetSize().H-1)-pos.Y;

	//点内外判定
	if ( m_Rect.DotHitCheck( math::CBasicVector2( pos.X , dotPosY ) ) == skyFalse ) return;

	//非書き込み判定
	if ( m_NoDrawPalletIdx != cbmp4bitcolor::NO_DRAW_PALLET_IDX_NONE )
	{
		if ( m_NoDrawPalletIdx == ( s32 )GetPoint( math::CBasicVector2( pos.X , pos.Y ) ) )
		{
			return;
		}
	}

	//インデックスの先頭ポインタ取得
	u32 Tgt = ( u32 )( ( m_LineWidth * dotPosY + pos.X ) / 2 );
	if( ( u32 )( pos.X ) & 0x1 )
	{
		m_pDataAdr[ Tgt ] = ( u8 )( ( m_pDataAdr[ Tgt ] & 0xF0 ) | palletNo );
	}
	else
	{
		m_pDataAdr[ Tgt ] = ( u8 )( ( palletNo << 4 ) | ( m_pDataAdr[ Tgt ] & 0xF ) );
	}
}

void CBmp4BitColor::DrawPoint( const math::CBasicVector2 &pos , u32 palletNo, u8 boldSize )
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

void CBmp4BitColor::DrawLine( const math::CBasicVector2& start, const math::CBasicVector2& Gole , u32 palletNo, u8 boldSize )
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
		
		DrawPoint( math::CBasicVector2( ( dectype )drawBasePosX , ( dectype )drawBasePosY ) , palletNo, boldSize );
	}
}

void CBmp4BitColor::DrawTile( CBmp4BitColor &bmpFile , math::CBasicVector2 &point, s32 copyPalNo )
{
	dectype width	= bmpFile.GetRect().GetSize().W;
	dectype height	= bmpFile.GetRect().GetSize().H;
	dectype posX	= point.X;
	dectype posY	= point.Y;
	s32 fromPalNo	= 0;
	s32 toPalNo	= 0;
	dectype tileWNum = m_Rect.GetSize().W / bmpFile.GetRect().GetSize().W;
	dectype tileHNum = m_Rect.GetSize().H / bmpFile.GetRect().GetSize().H;
	
	for( u32 tw = 0; tw<tileWNum; tw++ )
	{
		for( u32 th = 0; th<tileHNum; th++ )
		{
			for ( dectype x=0; x<width; x++ )
			{
				for ( dectype y=0; y<height; y++ )
				{
					fromPalNo = bmpFile.GetPoint( math::CBasicVector2(x , y) );
					toPalNo	  = GetPoint( math::CBasicVector2( x+posX+(tw*width) , y+posY+(th*height) ) );
					if( copyPalNo == NO_DRAW_PALLET_IDX_NONE || copyPalNo == toPalNo )
					{
						DrawPoint( math::CBasicVector2( x+posX+(tw*width) , y+posY+(th*height) ) , fromPalNo );
					}
				}
			}
		}
	}
}

void CBmp4BitColor::DrawRect( const math::CBasicRect& rect, u32 palletNo )
{
	dectype width	= rect.W();
	dectype height	= rect.H();
	dectype posX	= rect.X();
	dectype posY	= rect.Y();
	for ( dectype x=0; x<width; x++ )
	{
		for ( dectype y=0; y<height; y++ )
		{
			DrawPoint( math::CBasicVector2( x+posX , y+posY ) , palletNo, 1 );
		}
	}
}

void CBmp4BitColor::DrawTriangle( const math::CBasicTriangle3& tri, u32 palletNo )
{
	tri;
	palletNo;
}

void CBmp4BitColor::DrawCircle( const math::CBasicCircle& circle , u32 palletNo )
{
	dectype width	= m_Rect.GetSize().W;
	dectype height	= m_Rect.GetSize().H;
	dectype posX	= circle.X();
	dectype posY	= circle.Y();
	dectype radius_2 = circle.R() * circle.R();

	for ( dectype y=0; y<height; y++ )
	{
		for ( dectype x=0; x<width; x++ )
		{
			if ( ( (y-posY)*(y-posY) + (x-posX)*(x-posX) ) < radius_2 )
			{
				DrawPoint( math::CBasicVector2( x , y) , palletNo );
			}
		}
	}
}

void CBmp4BitColor::DrawBmp( CBmp4BitColor &bmpFile , math::CBasicVector2 &point, s32 copyPalNo )
{
	dectype width	= bmpFile.GetRect().GetSize().W;
	dectype height	= bmpFile.GetRect().GetSize().H;
	dectype posX	= point.X;
	dectype posY	= point.Y;
	s32 fromPalNo	= 0;
	s32 toPalNo	= 0;
	
	for ( dectype x=0; x<width; x++ )
	{
		for ( dectype y=0; y<height; y++ )
		{
			fromPalNo = bmpFile.GetPoint( math::CBasicVector2(x , y) );
			toPalNo	  = GetPoint( math::CBasicVector2( x+posX , y+posY ) );
			if( copyPalNo == NO_DRAW_PALLET_IDX_NONE || copyPalNo == toPalNo )
			{
				DrawPoint( math::CBasicVector2( x+posX , y+posY ) , fromPalNo );
			}
		}
	}
}

u32 CBmp4BitColor::GetPoint( const math::CBasicVector2 &pos )
{
	//データが下から始まっているのでY座標を反転させる
	dectype dotPosY = ( (m_Rect.GetSize().H-1)-pos.Y);

	//点内外判定
	if ( m_Rect.DotHitCheck( math::CBasicVector2( pos.X , dotPosY ) ) == skyFalse )
	{
		return cbmp4bitcolor::NO_PALLET_IDX;
	}

	u32 Tgt = ( u32 )( ( m_LineWidth * dotPosY + pos.X ) / 2 );
	if ( ( u32 )pos.X & 0x1 )
	{
		return ( u8 )( m_pDataAdr[ Tgt ] & 0x0F );
	}
	else
	{
		return ( u8 )( (m_pDataAdr[ Tgt ] >> 4) & 0x0F );
	}
}

void CBmp4BitColor::ReSize( dectype sc )
{
	//新しい縦横サイズを計算する
	u32 new_Width  = ( u32 )(m_Rect.GetSize().W * sc);
	u32 new_Height = ( u32 )(m_Rect.GetSize().H * sc);

	//新しいBmpを作成する
	CBmp4BitColor *pNew_Bmp = CloneBmp( new_Width, new_Height , 0 );

	//画素位置をスケールしながらコピーしていく
	math::CMatrix3x2	scCMatrix3x2;
	scCMatrix3x2.SetupScale( sky::lib::math::CBasicVector2( sc, sc ) );
	math::CBasicVector2 fromPos , toPos;
	for ( dectype w=0; w<m_Rect.GetSize().W; w++ )
	{
		for ( dectype h=0; h<m_Rect.GetSize().H; h++ )
		{
			fromPos.Set( w, h );
			toPos.Set( w, h );
			toPos *= scCMatrix3x2;
			toPos.Floor();
			pNew_Bmp->DrawPoint( toPos , GetPoint( fromPos ) );
		}
	}

	//古い領域を新しいサイズでリアロックする
	m_pData = ( u8 * )SkyRealloc( m_pData , pNew_Bmp->m_nDataSize );

	//リアロックされた領域に新しい情報を書き込む
	CMemoryUtil_::Copy( m_pData , pNew_Bmp->m_pData , pNew_Bmp->m_nDataSize );

	//各種情報の更新
	m_pHed			= ( BITMAPFILEHEADER * )m_pData;
	m_pBmpInfo		= ( BITMAPINFO * )( m_pData + sizeof( BITMAPFILEHEADER ) );
	m_pPalletAdr	= ( RGBQUAD * )m_pBmpInfo->bmiColors;
	m_pDataAdr		= ( u8 * )( m_pData + m_pHed->bfOffBits );
	m_LineWidth		= PAGE_ALIGN( new_Width, cbmp4bitcolor::LINE_ALIGN_SIZE );
	m_Rect.Set( math::CBasicVector2( 0.0f , 0.0f ) , math::CBasicSize( ( dectype )new_Width, ( dectype )new_Height ) );

	//新しい情報を破棄する
	SkyFree( pNew_Bmp );
}


RGBQUAD* CBmp4BitColor::GetPalletAdr()
{
	return m_pPalletAdr;
}

void CBmp4BitColor::SetPallet( CBmp4BitColor &bmpFile )
{
	CMemoryUtil_::Copy( m_pPalletAdr, bmpFile.GetPalletAdr() , cbmp4bitcolor::PALLET_SIZE );
}

void CBmp4BitColor::PaintsOut_Child( math::CBasicVector2& vec2, u32 startPalNo, u32 paintsOutPalNo, CBmp4BitColor& flgBmp )
{
	CStack<math::CBasicVector2*>	st( ( u32 )(m_Rect.GetSize().W * m_Rect.GetSize().H) );

	//最初の一つをPush
	st.Push( NEW__( math::CBasicVector2 , math::CBasicVector2( vec2.X , vec2.Y ) ) );

	math::CBasicVector2*pCheckVec2;
	while (st.GetNum() > 0) //スタックから値がなくなったら終了
	{
		//スタックの最上段の座標を判定
		st.GetItem( st.GetNum() - 1 , pCheckVec2 );

		//点を打ち、フラグを立てる
		u32 pos = GetPoint( * pCheckVec2 );
		DrawPoint( * pCheckVec2 , paintsOutPalNo );
		pos = GetPoint( * pCheckVec2 );
		flgBmp.DrawPoint( * pCheckVec2 , 1 );

		//左方向
		if ( GetPoint( math::CBasicVector2( pCheckVec2->X - 1 , pCheckVec2->Y ) ) == startPalNo && 
			 flgBmp.GetPoint( math::CBasicVector2( pCheckVec2->X - 1 , pCheckVec2->Y ) ) == 0 &&
			 pCheckVec2->X-1 < m_Rect.GetSize().W &&
			 pCheckVec2->Y < m_Rect.GetSize().H )
		{
			st.Push( NEW__( math::CBasicVector2 , math::CBasicVector2( pCheckVec2->X-1, pCheckVec2->Y ) ) );
			continue;
		}

		//下方向
		if ( GetPoint( math::CBasicVector2( pCheckVec2->X , pCheckVec2->Y+1 ) ) == startPalNo && 
			 flgBmp.GetPoint( math::CBasicVector2( pCheckVec2->X , pCheckVec2->Y+1 ) ) == 0 &&
			 pCheckVec2->X   < m_Rect.GetSize().W &&
			 pCheckVec2->Y+1 < m_Rect.GetSize().H )
		{
			st.Push( NEW__( math::CBasicVector2 , math::CBasicVector2( pCheckVec2->X , pCheckVec2->Y+1 ) ) );
			continue;
		}

		//右方向
		if ( GetPoint( math::CBasicVector2( pCheckVec2->X+1, pCheckVec2->Y ) ) == startPalNo && 
			 flgBmp.GetPoint( math::CBasicVector2(pCheckVec2->X+1, pCheckVec2->Y ) ) == 0 &&
			 pCheckVec2->X+1 < m_Rect.GetSize().W &&
			 pCheckVec2->Y   < m_Rect.GetSize().H )
		{
			st.Push( NEW__( math::CBasicVector2 , math::CBasicVector2( pCheckVec2->X+1, pCheckVec2->Y ) ) );
			continue;
		}

		//上方向
		if ( GetPoint( math::CBasicVector2(pCheckVec2->X , pCheckVec2->Y-1 ) ) == startPalNo && 
			 flgBmp.GetPoint( math::CBasicVector2(pCheckVec2->X , pCheckVec2->Y-1 ) ) == 0 &&
			 pCheckVec2->X   < m_Rect.GetSize().W &&
			 pCheckVec2->Y-1 < m_Rect.GetSize().H )
		{
			st.Push( NEW__( math::CBasicVector2 , math::CBasicVector2( pCheckVec2->X , pCheckVec2->Y-1 ) ) );
			continue;
		}

		//判定の終わった座標はPop
		math::CBasicVector2 *pVec;

		if ( st.Pop( pVec ) == skyTrue )
		{
			DEL pVec;
		}
	}
}
void CBmp4BitColor::PaintsOut( math::CBasicVector2& vec2, u32 palletNo, CBmp4BitColor& flgBmp )
{
	//実際の塗りつぶし処理。
	PaintsOut_Child( vec2, GetPoint( vec2 ) , palletNo, flgBmp );
}

void CBmp4BitColor::vLoad( stream::IStreamPtr &src )
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

	// パレット部先頭アドレス取得
	m_pPalletAdr = ( RGBQUAD * )m_pBmpInfo->bmiColors;

	// データ部先頭アドレス取得
	m_pDataAdr = m_pData + m_pHed->bfOffBits;

	//矩形情報の構築
	m_Rect.Set( math::CBasicVector2( 0.0f , 0.0f ) , math::CBasicSize( ( dectype )m_pBmpInfo->bmiHeader.biWidth, ( dectype )m_pBmpInfo->bmiHeader.biHeight ) );

	//幅はアライメント取る
	m_LineWidth = PAGE_ALIGN( m_Rect.GetSize().W, cbmp4bitcolor::LINE_ALIGN_SIZE );
}

void CBmp4BitColor::vSave( stream::IStreamPtr &dest )
{
	//すべてストリームに書きだす
	dest->vWriteStream( m_pData , m_nDataSize );
}


void CBmp4BitColor::vCreate( u32 width, u32 height, u32 palletNo )
{
	//幅はアライメント取る
	m_LineWidth = PAGE_ALIGN( width, cbmp4bitcolor::LINE_ALIGN_SIZE );

	u32 dwSizeImage = ( u32 )( m_LineWidth * height * cbmp4bitcolor::ONE_PIXEL_BYTE_SIZE );

	//ファイル全体サイズ
	m_nDataSize = sizeof(BITMAPFILEHEADER) +	
				 sizeof(BITMAPINFOHEADER) +
				 cbmp4bitcolor::PALLET_SIZE + 
				 dwSizeImage;

	//DIB用バッファ確保(BITMAPFILEHEADER
	m_pData = ( u8 * )SkyMalloc( m_nDataSize );

	//ファイルヘッダーを作成
	m_pHed = ( BITMAPFILEHEADER * )m_pData;
	CMemoryUtil_::ClearZero( m_pHed , sizeof( BITMAPFILEHEADER ) );
	m_pHed->bfType    = *( LPWORD )"BM";
	m_pHed->bfSize    = m_nDataSize;
	m_pHed->bfOffBits = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + cbmp4bitcolor::PALLET_SIZE;

	//基本情報を作成
	m_pBmpInfo	= ( BITMAPINFO * )( m_pData + sizeof( BITMAPFILEHEADER ) );
	CMemoryUtil_::ClearZero( m_pBmpInfo , sizeof( BITMAPINFOHEADER ) );
	m_pBmpInfo->bmiHeader.biSize		= sizeof( BITMAPINFOHEADER );
	m_pBmpInfo->bmiHeader.biWidth		= width;
	m_pBmpInfo->bmiHeader.biHeight		= height;
	m_pBmpInfo->bmiHeader.biPlanes		= 1;
	m_pBmpInfo->bmiHeader.biBitCount	= (WORD)cbmp4bitcolor::ONE_PIXEL_BIT_SIZE;				//1ピクセルあたりの情報量4bitつまり1バイトにつき2ピクセル
	m_pBmpInfo->bmiHeader.biCompression = BI_RGB;

	// パレット部先頭アドレス取得
	m_pPalletAdr = ( RGBQUAD * )m_pBmpInfo->bmiColors;
	CMemoryUtil_::ClearZero( m_pPalletAdr, cbmp4bitcolor::PALLET_SIZE );

	//デフォルトパレット（適当）
	RGBQUAD pal[] = 
	{
		{ 255 , 255 , 255 , 100 }, //0:白
		{ 255 , 0   , 0   , 100 }, //1:青
		{ 0   , 255 , 0   , 100 }, //2:緑
		{ 0   , 0   , 255 , 100 }, //3:赤
		{ 255 , 255 , 255 , 100 }, //4:黒
	};
	u32 palNum = array_sizeof( pal );
	for ( u32 i = 0; i < palNum ; i++ )
	{
		m_pPalletAdr[ i ] = pal[ i ];
	}

	// データ部先頭アドレス取得
	m_pDataAdr = ( u8 * )( m_pData + m_pHed->bfOffBits );
	CMemoryUtil_::ClearZero( m_pDataAdr , dwSizeImage );

	//矩形情報の構築
	m_Rect.Set( math::CBasicVector2( 0.0f , 0.0f ) , math::CBasicSize( ( dectype )width, ( dectype )height ) ); //矩形情報は配列のインデックスとしたいので、高さは-1する

	Clear( palletNo );
}

CBmp4BitColor * CBmp4BitColor::CloneBmp( u32 width, u32 height, u32 palletNo )
{
	//新しいBmpを作成する
	CBmp4BitColor *pNew_Bmp = NEW__( CBmp4BitColor , CBmp4BitColor() );
	pNew_Bmp->vCreate( width, height, palletNo );

	//パレットをコピー
	pNew_Bmp->SetPallet( *this );

	return pNew_Bmp;
}

void CBmp4BitColor::SetNoDrawPalletNo( u32 palletNo )
{
	m_NoDrawPalletIdx = palletNo;
}

void CBmp4BitColor::SetNoDrawPalletNoClear()
{
	m_NoDrawPalletIdx = cbmp4bitcolor::NO_DRAW_PALLET_IDX_NONE;
}

//階調データをパレット番号に変換する
static u32 GetDataPallet( fontbmp::eGradationType eType , u32 data )
{
	u32 result = 0;
	switch( eType )
	{//２と５階調はデータをそのまま使用できる
		case fontbmp::eGradationType_2:
		case fontbmp::eGradationType_5:
			result = data;
			break;
		case fontbmp::eGradationType_17:
		case fontbmp::eGradationType_65:
			result = COL_HIRITU_1( eType , data , cbmp4bitcolor::PALLET_NUM );
			break;
	}
	return result;
}

void CBmp4BitColor::vWriteFontInit( const CFontBmpInfo &info )
{
	SKY_UNUSED_ARG( info );

	//背景色でクリア（インデックスカラーはパレット０が背景色）
	Clear( 0 );
}

void CBmp4BitColor::vWriteFont( const math::CBasicVector2 &pos , const CFontBmpInfo &info )
{
	info.w_pUvData->x = ( u32 )( info.out_outLine.W * pos.X );
	info.w_pUvData->y = ( u32 )( info.out_outLine.H * pos.Y );
	info.w_pUvData->w = ( u32 )( info.out_outLine.W );
	info.w_pUvData->h = ( u32 )( info.out_outLine.H );

	info.w_pUvData->ult = info.w_pUvData->x / info.out_pUvData->fileWidth;                                       //左上Ｕ
	info.w_pUvData->vlt = info.w_pUvData->y / info.out_pUvData->fileHeight;                                      //左上Ｖ
	info.w_pUvData->urt = ( info.w_pUvData->x + info.out_outLine.W ) / info.out_pUvData->fileWidth;     //右上Ｕ
	info.w_pUvData->vrt = info.w_pUvData->vlt;                                                                   //右上Ｖ
	info.w_pUvData->ulb = info.w_pUvData->ult;                                                                   //左下Ｕ
	info.w_pUvData->vlb = ( info.w_pUvData->y + info.out_outLine.H ) / info.out_pUvData->fileHeight;   //左下Ｖ
	info.w_pUvData->urb = info.w_pUvData->urt;
	info.w_pUvData->vrb = info.w_pUvData->vlb;

	//ビットマップの幅とＸ座標と文字書き出しＸ座標からテクスチャ書き出しＸ座標を算出する
	u32 sx = ( u32 )( info.w_pUvData->x + info.out_rect.X() );

	//ビットマップの幅とＹ座標と文字書き出しＹ座標からテクスチャ書き出しＹ座標を算出する
	u32 sy = ( u32 )( info.w_pUvData->y + info.out_rect.Y() );

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
				DrawPoint( math::CBasicVector2( ( dectype )( x + sx ) , ( dectype )( y + sy ) ) , p );
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
				DrawPoint( math::CBasicVector2( ( dectype )( x + sx ) , ( dectype )( y + sy ) ) , GetDataPallet( info.in_eGradation , p ) );
			}
		}
	}
}

void CBmp4BitColor::vMakeColorPallet ( const CFontBmpInfo &info )
{
	u32 colornum = ( cbmp4bitcolor::PALLET_NUM > info.in_eGradation ) ? info.in_eGradation : cbmp4bitcolor::PALLET_NUM;
	colornum--; //計算のためひとつ減らす。
	for( u32 i = 0 ; i <= colornum ; i++ ) //パレット０から敷き詰めていく
	{
		f32 X = ( f32 )i / colornum;

		//アルファグラデーションの場合はアルファ値で階調を表現する
		if ( info.in_isAlphaGradation )
		{
			if ( i == 0 ) //パレット０は背景色
			{
				m_pPalletAdr[ i ].rgbBlue     = CFTOI( info.in_clearcolor.b );
				m_pPalletAdr[ i ].rgbGreen    = CFTOI( info.in_clearcolor.g );
				m_pPalletAdr[ i ].rgbRed      = CFTOI( info.in_clearcolor.r );
				m_pPalletAdr[ i ].rgbReserved = 0; //完全透明
			}
			else
			{
				m_pPalletAdr[ i ].rgbBlue     = CFTOI( info.in_color.b );
				m_pPalletAdr[ i ].rgbGreen    = CFTOI( info.in_color.g );
				m_pPalletAdr[ i ].rgbRed      = CFTOI( info.in_color.r );
				m_pPalletAdr[ i ].rgbReserved = CFTOI( X );
			}
		}
		else
		{
			m_pPalletAdr[ i ].rgbBlue     = CFTOI( ( ( info.in_color.b - info.in_clearcolor.b ) * X + info.in_clearcolor.b ) );
			m_pPalletAdr[ i ].rgbGreen    = CFTOI( ( ( info.in_color.g - info.in_clearcolor.g ) * X + info.in_clearcolor.g ) );
			m_pPalletAdr[ i ].rgbRed      = CFTOI( ( ( info.in_color.r - info.in_clearcolor.r ) * X + info.in_clearcolor.r ) );
			m_pPalletAdr[ i ].rgbReserved = 255;
		}
	}
}

#endif

} } }
