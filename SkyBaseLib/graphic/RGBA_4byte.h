#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// RGBA色空間クラス(4byte版)
/// RGBの色空間に加えて、アルファチャンネルも色決定に考慮させる。これは、透過（透明度）を表現するものである（厳密にはこれは色空間ではないとも考えられる）。
/// 各要素に1byteずつ、系4byteで色を表現する。
/// </summary>
template< typename T = colortype >
class CRGBA_4byte : public base::SkyObject
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
    CRGBA_4byte (u8 lr = 0, u8 lg = 0, u8 lb = 0, u8 la = 0);

	/// <summary>
	/// rgba各要素への参照アクセス群（通常アクセスとconstアクセス版あり）
	/// </summary>
    u8& r();
    const u8& r() const;
    u8& g();
    const u8& g() const;
    u8& b();
    const u8& b() const;
    u8& a();
    const u8& a() const;

	/// <summary>
	/// rgbaの順で32bitにパック
	/// </summary>
	u32 rgba() const;

	/// <summary>
	/// u32値で値を取得
	/// </summary>
    void GetAs( u32& dest ) const;
    u32  GetAs() const;
    void GetAsBGRA( u32& dest ) const;
    u32  GetAsBGRA() const;
    void GetAsARGB( u32& dest ) const;
    u32  GetAsARGB() const;

	/// <summary>
	/// 色クラスで取得
	/// </summary>
    void GetAs( CRGB< T >& rgb ) const;
    void GetAs( CRGBA< T >& rgba ) const;

	/// <summary>
	/// 代入演算子のオーバーライド
	/// </summary>
    CRGBA_4byte& operator= (u8 value);

	/// <summary>
	/// 比較演算子のオーバーライド
	/// </summary>
    bool operator== (const CRGBA_4byte< T >& rgba) const;
    bool operator!= (const CRGBA_4byte< T >& rgba) const;

	/// <summary>
	/// 算術演算子のオーバーライド
	/// </summary>
    CRGBA_4byte operator+ (const CRGBA_4byte< T >& rgba) const;
    CRGBA_4byte operator- (const CRGBA_4byte< T >& rgba) const;
    CRGBA_4byte operator* (T value) const;
    CRGBA_4byte operator* (const CRGBA_4byte< T >& rgba) const;
    CRGBA_4byte operator/ (T value) const;
    CRGBA_4byte operator/ (const CRGBA_4byte< T >& rgba) const;
    CRGBA_4byte operator- () const;

    CRGBA_4byte& operator+= (const CRGBA_4byte< T >& rgba);
    CRGBA_4byte& operator-= (const CRGBA_4byte< T >& rgba);
    CRGBA_4byte& operator*= (T value);
    CRGBA_4byte& operator*= (const CRGBA_4byte< T >& rgba);
    CRGBA_4byte& operator/= (T value);
    CRGBA_4byte& operator/= (const CRGBA_4byte< T >& rgba);

private:

	/// <summary>
	/// 値型
	/// </summary>
    struct rgba_
    {
        u8 r, g, b, a;
    };

	/// <summary>
	/// 値
	/// </summary>
    union 
    {
        rgba_	m_rgba;
        u32		m_rgba_u32;
    }; 
};

/// <summary>
/// 基本RGBA型
/// </summary>
typedef CRGBA_4byte< colortype >	CBasicRGBA_4byte;

/// <summary>
/// 色定数
/// </summary>
static const CBasicRGBA_4byte CBasicRGBA_4byte_White      ( 255 , 255 , 255 , 255 );
static const CBasicRGBA_4byte CBasicRGBA_4byte_Black      ( 0   , 0   , 0   , 255 ); 
static const CBasicRGBA_4byte CBasicRGBA_4byte_Red        ( 255 , 0   , 0   , 255 ); 
static const CBasicRGBA_4byte CBasicRGBA_4byte_Green      ( 0   , 255 , 0   , 255 ); 
static const CBasicRGBA_4byte CBasicRGBA_4byte_Blue       ( 0   , 0   , 255 , 255 ); 
static const CBasicRGBA_4byte CBasicRGBA_4byte_Tomato     ( 255 , 99  , 71  , 255 ); 
static const CBasicRGBA_4byte CBasicRGBA_4byte_Orangered  ( 255 , 69  , 0   , 255 ); 
static const CBasicRGBA_4byte CBasicRGBA_4byte_Sandybrown ( 244 , 164 , 96  , 255 ); 
static const CBasicRGBA_4byte CBasicRGBA_4byte_Yellow     ( 255 , 255 , 0   , 255 ); 
static const CBasicRGBA_4byte CBasicRGBA_4byte_Violetw    ( 238 , 130 , 238 , 255 ); 
static const CBasicRGBA_4byte CBasicRGBA_4byte_Silver     ( 192 , 192 , 192 , 255 ); 
} } }