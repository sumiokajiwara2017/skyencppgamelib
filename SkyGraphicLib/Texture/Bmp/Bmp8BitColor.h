#pragma once

namespace sky { namespace lib { namespace graphic {

#ifdef SKYLIB_IDE_VC10

class CBmp8BitColor;
typedef SmartPointer< CBmp8BitColor >	CBmp8BitColorPtr;					//スマートポインタ版定義

/// <summary>
/// 定数
/// </summary>
namespace cbmp8bitcolor
{
	static const	u32	PALLET_NUM			= 256;
	static const	u32	PALLET_SIZE			= PALLET_NUM * sizeof(RGBQUAD);
	static const	u32	NO_PALLET_IDX		= PALLET_NUM;			//無効なパレットインデックス
	static const	d64	ONE_PIXEL_BIT_SIZE	= 8.0f;//下記で計算に使うためあえてd64
	static const	d64	ONE_PIXEL_BYTE_SIZE	= ONE_PIXEL_BIT_SIZE/8;
	static const	u32	LINE_ALIGN_SIZE		= 4;	//１ラインのアライメントサイズ
	static const	s32	NO_DRAW_PALLET_IDX_NONE	= -1;				
}

/// <summary>
/// ２５６色Ｂｍｐファイルクラス
/// </summary>
/*
+--------------------------------------------+
|                                            |
|             BITMAPFILEHEADER               |
|                                            |
+--------------------------------------------+
|                                            |
|               BITMAPINFO		             |
|                                            |
+--------------------------------------------+
|                                            |
|               COLOR_PALLET                 |
|                                            |
+--------------------------------------------+
|                                            |
|                                            |
|                                            |
|               COLOR_INDEX                  |
|                                            |
|                                            |
|                                            |
|                                            |
+--------------------------------------------+

COLOR_INDEX部データは画像の左下から右上へと伸びるデータが入っている。
COLOR_INDEX部データはライン毎にアライメントがとられている

*/
class CBmp8BitColor : public AbsBmpFile
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CBmp8BitColor() : AbsBmpFile( fontbmp::eBmpKind_bmp8bit ){}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CBmp8BitColor(){};

	/// <summary>
	/// 指定の色情報で初期化する
	/// </summary>
	void Clear( u32 palletNo );

	/// <summary>
	/// データ部のサイズを取得する(ヘッダー、パレット以外の場所）
	/// </summary>
	u32 GetDataSize();

	/// <summary>
	/// パレットアドレス取得
	/// </summary>
	RGBQUAD* GetPalletAdr();

	/// <summary>
	/// ヘッダーアドレス取得
	/// </summary>
	BITMAPFILEHEADER* CBmp8BitColor::GetHeaderAdr();

	/// <summary>
	/// ビットマップ情報アドレス取得
	/// </summary>
	BITMAPINFO* CBmp8BitColor::GetInfoAdr();

	/// <summary>
	/// 指定の位置に点を打つ
	/// </summary>
	void DrawPoint( const math::CBasicVector2 &pos , u32 palletNo );

	/// <summary>
	/// <summary>
	/// 指定の位置に点を打つ
	/// </summary>
	void DrawPoint( const math::CBasicVector2 &pos , u32 palletNo, u8 boldSize );

	/// <summary>
	/// 指定の位置に線を引く
	/// </summary>
	void DrawLine( const math::CBasicVector2& start, const math::CBasicVector2& Gole , u32 palletNo, u8 boldSize );

	/// <summary>
	/// 指定の位置に四角形をタイル状に全体に描く
	/// </summary>
	void DrawTile( const math::CBasicRect& rect, u32 palletNo );

	/// <summary>
	/// 指定の位置に四角形をタイル状に全体に描く
	/// </summary>
	void DrawTile( CBmp8BitColor &bmpFile , math::CBasicVector2 &point, s32 copyPalNo );

	/// <summary>
	/// 指定の位置に四角形を描く
	/// </summary>
	void DrawRect( const math::CBasicRect& rect, u32 palletNo );

	/// <summary>
	/// 指定の位置に三角形を描く
	/// </summary>
	void DrawTriangle( const math::CBasicTriangle3& tri, u32 palletNo );

	/// <summary>
	/// 指定の位置に円形を描く
	/// </summary>
	void DrawCircle( const math::CBasicCircle& circle , u32 palletNo );

	/// <summary>
	/// 指定の位置にBmpを描く
	/// </summary>
	void DrawBmp( CBmp8BitColor &bmpFile , math::CBasicVector2 &point, s32 copyPalNo );

	/// <summary>
	/// 指定の位置の点のカラー番号を取得する
	/// </summary>
	u32 GetPoint( const math::CBasicVector2& start );

	/// <summary>
	/// サイズ変更しつつ複製
	/// </summary>
	CBmp8BitColor * CloneBmp( u32 width, u32 height, u32 palletNo );

	/// <summary>
	/// 指定のBmpのヘッダーを上書きする
	/// </summary>
	void SetHeader( CBmp8BitColor &bmpFile );

	/// <summary>
	/// 指定のBmpのビットマップ情報を上書きする
	/// </summary>
	void SetInfo( CBmp8BitColor &bmpFile );

	/// <summary>
	/// 指定のBmpのパレットを上書きする
	/// </summary>
	void SetPallet( CBmp8BitColor &bmpFile );

	/// <summary>
	/// 塗りつぶし処理
	/// </summary>
	void PaintsOut( math::CBasicVector2& vec2, u32 palletNo, CBmp8BitColor& flgBmp );

	/// <summary>
	/// サイズ変更
	/// </summary>
	void ReSize( dectype sc );

	/// <summary>
	/// 純粋仮想関数の実装
	/// </summary>
    void vWriteFontInit   ( const CFontBmpInfo &info );
	void vWriteFont( const math::CBasicVector2 &pos , const CFontBmpInfo &info );
	void vCreate( u32 width, u32 height, u32 palletNo );
	void vMakeColorPallet ( const CFontBmpInfo &info );
	void vSave          ( stream::IStreamPtr &dest );
	void vLoad          ( stream::IStreamPtr &src );

private:

	/// <summary>
	/// 塗りつぶし処理下請け
	/// </summary>
	void PaintsOut_Child( math::CBasicVector2& vec2, u32 startPalNo, u32 paintsOutPalNo, CBmp8BitColor& flgBmp );

};

#endif

} } }
