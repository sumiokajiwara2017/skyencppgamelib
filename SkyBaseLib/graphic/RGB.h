#pragma once

namespace sky { namespace lib { namespace graphic {

template< typename T >
class CRGBA_4byte;

/// <summary>
/// RGB色空間クラス
/// RGB色空間は色の表現法の一種で、赤 (Red)、緑 (Green)、青 (Blue) の三つの原色を混ぜて幅広い色を再現する加法混色の一種である。
/// RGBは三原色の頭文字である。ブラウン管（CRT）や液晶ディスプレイ（LCD）、デジタルカメラなどで画像再現に使われている。
/// </summary>
template< typename T = colortype >
class CRGB : public base::SkyObject
{
public:

	/// <summary>
	/// メンバ変数
	/// </summary>
    T r, g, b;

	/// <summary>
	/// コンストラクタ
	/// </summary>
    CRGB ( T lr = 0.0f, T lg = 0.0f, T lb = 0.0f );

	/// <summary>
	/// rgb各要素値を1.0f以内に補正
	/// </summary>
    void Clamp		();

	/// <summary>
	/// rgb各要素のうち最大の値を基準に各要素値を1.0f以内に補正
	/// </summary>
    void Scale		();

	/// <summary>
	/// 4バイト表現のrgba値に変換
	/// </summary>
    void GetAs		( CRGBA_4byte< T >& rgba ) const;

	/// <summary>
	/// 代入演算子のオーバーライド
	/// </summary>
    CRGB< T >& operator=	( T value );

	/// <summary>
	/// 比較演算子のオーバーライド
	/// </summary>
    skyBool operator==	( const CRGB< T >& rgb )		const;
    skyBool operator!=	( const CRGB< T >& rgb )		const;

	/// <summary>
	/// 算術演算子のオーバーライド
	/// </summary>
    CRGB< T > operator+	( const CRGB< T >& rgb )		const;
    CRGB< T > operator-	( const CRGB< T >& rgb )		const;
    CRGB< T > operator*	( T value )						const;
    CRGB< T > operator*	( const CRGB< T >& rgb )		const;
    CRGB< T > operator/	( T value )						const;
    CRGB< T > operator/	( const CRGB< T >& rgb )		const;
    CRGB< T > operator-	()								const;
    CRGB< T >& operator+= ( const CRGB< T >& rgb );
    CRGB< T >& operator-= ( const CRGB< T >& rgb );
    CRGB< T >& operator*= ( T value );
    CRGB< T >& operator*= ( const CRGB< T >& rgb );
    CRGB< T >& operator/= ( T value );
    CRGB< T >& operator/= ( const CRGB< T >& rgb );

};

/// <summary>
/// 基本RGB型
/// </summary>
typedef CRGB< colortype >	CBasicRGB;

/// <summary>
/// 色定数
/// </summary>
static const CBasicRGB RGB_White        ( 1.0f , 1.0f , 1.0f );
static const CBasicRGB RGB_Black        ( 0.0f , 0.0f , 0.0f ); 
static const CBasicRGB RGB_Red          ( 1.0f , 0.0f , 0.0f ); 
static const CBasicRGB RGB_Green        ( 0.0f , 1.0f , 0.0f ); 
static const CBasicRGB RGB_Blue         ( 0.0f , 0.0f , 1.0f ); 
static const CBasicRGB RGB_Tomato       ( 1.0f , 0.38f , 0.27f ); 
static const CBasicRGB RGB_Orangered    ( 1.0f , 0.27f , 0.0f ); 
static const CBasicRGB RGB_Sandybrown   ( 0.95f , 0.64f , 0.37f ); 
static const CBasicRGB RGB_Yellow       ( 1.0f , 1.0f , 0.0f ); 
static const CBasicRGB RGB_Violetw      ( 0.93f , 0.50f , 0.93f ); 
static const CBasicRGB RGB_Silver       ( 0.75f , 0.75f , 0.75f ); 

} } }