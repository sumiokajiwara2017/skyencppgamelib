#pragma once

namespace sky { namespace lib { namespace graphic {

#ifdef SKYLIB_IDE_VC10

class CBmp4BitColor;
typedef SmartPointer< CBmp4BitColor >	CBmp4BitColorPtr;					//スマートポインタ版定義

/// <summary>
/// 定数
/// </summary>
namespace cbmp4bitcolor
{
	static const	u32	PALLET_NUM				= 16;
	static const	u32	PALLET_SIZE				= PALLET_NUM * sizeof(RGBQUAD);
	static const	u32	NO_PALLET_IDX			= PALLET_NUM;			//無効なパレットインデックス
	static const	d64	ONE_PIXEL_BIT_SIZE		= 4.0f;					//下記で計算に使うためあえてd64
	static const	d64	ONE_PIXEL_BYTE_SIZE		= ONE_PIXEL_BIT_SIZE/8;
	static const	u32	LINE_ALIGN_SIZE			= 4;					//１ラインのアライメントサイズ
	static const	s32	NO_DRAW_PALLET_IDX_NONE	= -1;				
}

/// <summary>
/// １６色Ｂｍｐファイルクラス
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

＜ポイント＞
COLOR_INDEX部データは画像の左下から右上へと伸びるデータが入っている。
COLOR_INDEX部データはライン毎にアライメントがとられている

*/
class CBmp4BitColor : public AbsBmpFile
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CBmp4BitColor();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CBmp4BitColor(){};

	/// <summary>
	/// データ部のサイズを取得する(ヘッダー、パレット以外の場所）
	/// </summary>
	u32 GetDataSize();

	/// <summary>
	/// 指定の色情報で初期化する
	/// </summary>
	void Clear( u32 colorNo );

	/// <summary>
	/// 指定の色情報で初期化する
	/// </summary>
	void SetNoDrawPalletNo( u32 palletNo );
	void SetNoDrawPalletNoClear();

	/// <summary>
	/// 指定の位置に点を打つ
	/// </summary>
	void DrawPoint( const math::CBasicVector2 &pos , u32 palletNo );

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
	void DrawTile( CBmp4BitColor &bmpFile , math::CBasicVector2 &point, s32 copyPalNo );

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
	void DrawBmp( CBmp4BitColor &bmpFile , math::CBasicVector2 &point, s32 copyPalNo );

	/// <summary>
	/// 指定の位置の点のカラー番号を取得する
	/// </summary>
	u32 GetPoint( const math::CBasicVector2 &pos );

	/// <summary>
	/// サイズ変更しつつ複製
	/// </summary>
	CBmp4BitColor * CloneBmp( u32 width, u32 height, u32 palletNo );

	/// <summary>
	/// パレットアドレスの取得
	/// </summary>
	RGBQUAD*			GetPalletAdr();

	/// <summary>
	/// サイズ変更
	/// </summary>
	void ReSize( dectype sc );

	/// <summary>
	/// 指定のBmpのパレットを上書きする
	/// </summary>
	void SetPallet( CBmp4BitColor &bmpFile );

	/// <summary>
	/// 指定のテクスチャ座標から再帰で指定の色で塗りつぶす
	/// </summary>
	void PaintsOut( math::CBasicVector2& vec2, u32 palletNo, CBmp4BitColor& flgBmp );

	/// <summary>
	/// 仮想関数の実装
	/// </summary>
	void vWriteFontInit   ( const CFontBmpInfo &info );
	void vWriteFont       ( const math::CBasicVector2 &pos , const CFontBmpInfo &info );
	void vCreate          ( u32 width, u32 height, u32 palletNo );
	void vMakeColorPallet ( const CFontBmpInfo &info );
	void vSave            ( stream::IStreamPtr &dest );
	void vLoad            ( stream::IStreamPtr &src );

private:

	/// <summary>
	/// 書き込み禁止パレットインデックス
	/// </summary>
	s32	m_NoDrawPalletIdx;

	/// <summary>
	/// 塗りつぶし再帰
	/// </summary>
	void PaintsOut_Child( math::CBasicVector2& vec2, u32 startPalNo, u32 paintsOutPalNo, CBmp4BitColor& flgBmp );
};

#endif

} } }
