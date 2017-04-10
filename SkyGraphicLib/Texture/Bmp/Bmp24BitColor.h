#pragma once

namespace sky { namespace lib { namespace graphic {

class AbsBmpFile;

#ifdef SKYLIB_IDE_VC10

class CBmp24BitColor;
typedef SmartPointer< CBmp24BitColor >	CBmp24BitColorPtr;					//スマートポインタ版定義

/// <summary>
/// 定数
/// </summary>
namespace cbmp24bitcolor
{
	static const	u32	ONE_PIXEL_BIT_SIZE	= 24;
	static const	u32	ONE_PIXEL_BYTE_SIZE	= ONE_PIXEL_BIT_SIZE/8;
	static const	u32	LINE_ALIGN_SIZE		= 4;	//１ラインのアライメントサイズ
}

/// <summary>
/// ２４ビット色Ｂｍｐファイルクラス
/// </summary>
/*
+--------------------------------------------+
|                                            |
|             BMP_FILE_HEADER                |
|                                            |
+--------------------------------------------+
|                                            |
|             BMP_FILE_INFO                  |
|                                            |
+--------------------------------------------+
|                                            |
|                                            |
|                                            |
|             BMP_FILE_DATA                  |
|                                            |
|                                            |
|                                            |
|                                            |
+--------------------------------------------+

BMP_FILE_DATA部データは画像の左下から右上へと伸びるデータが入っている。
BMP_FILE_DATA部データはライン毎にアライメントがとられている

*/
class CBmp24BitColor : public AbsBmpFile
{
public:

	/// <summary>
	/// １ピクセル情報
	/// </summary>
	typedef struct
	{
		u8 b;
		u8 g;
		u8 r;
	} stdPixelInfo;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CBmp24BitColor() : AbsBmpFile( fontbmp::eBmpKind_bmp24bit ){}

	/// <summary>
	/// デスストラクタ
	/// </summary>
	virtual ~CBmp24BitColor(){};

	/// <summary>
	/// 指定の色情報で初期化する
	/// </summary>
	void Clear( u32 abgr );

	/// <summary>
	/// データ部のサイズを取得する(ヘッダー、パレット以外の場所）
	/// </summary>
	u32 GetDataSize();

	/// <summary>
	/// 指定の位置に点を打つ( abgr:0x000000FF(赤) abgr:0x0000FF00(緑) abgr:0x00FF0000(青)
	/// </summary>
	void DrawPoint( const math::CBasicVector2 &pos , u32 abgr );

	/// <summary>
	/// 指定の位置に点を打つ(colorNoはパレット番号だったりRGBAだったりするはず）
	/// </summary>
	void DrawPoint( const math::CBasicVector2 &pos , u32 abgr, u8 boldSize );

	/// <summary>
	/// 指定の位置に線を引く(colorNoはパレット番号だったりRGBAだったりするはず）
	/// </summary>
	void DrawLine( const math::CBasicVector2& start, math::CBasicVector2& Gole , u32 abgr, u8 boldSize );

	/// <summary>
	/// 指定の位置に四角形をタイル状に全体に描く
	/// </summary>
	void DrawTile( CBmp24BitColor &bmpFile , math::CBasicVector2 &point, u32 nocpyColor );

	/// <summary>
	/// 指定の位置に四角形を描く(colorNoはパレット番号だったりRGBAだったりするはず）
	/// </summary>
	void DrawRect( const math::CBasicRect& rect, u32 abgr );

	/// <summary>
	/// 指定の位置に三角形を描く(colorNoはパレット番号だったりRGBAだったりするはず）
	/// </summary>
	void DrawTriangle( const math::CBasicTriangle3& tri, u32 abgr );

	/// <summary>
	/// 指定の位置に円形を描く(colorNoはパレット番号だったりRGBAだったりするはず）
	/// </summary>
	void DrawCircle( const math::CBasicCircle& circle , u32 abgr );

	/// <summary>
	/// 指定の位置の点のカラー番号を取得する
	/// </summary>
	u32 GetPoint( const math::CBasicVector2& start );

	/// <summary>
	/// 純粋仮想関数の実装
	/// </summary>
	void vWriteFontInit ( const CFontBmpInfo &info );
	void vWriteFont     ( const math::CBasicVector2 &pos , const CFontBmpInfo &info );
	void vCreate        ( u32 width, u32 height , u32 abgr );
	void vSave          ( stream::IStreamPtr &dest );
	void vLoad          ( stream::IStreamPtr &src );

};

#endif

} } }
