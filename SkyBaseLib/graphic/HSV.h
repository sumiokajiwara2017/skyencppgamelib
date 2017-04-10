#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// HSV色空間クラス
/// H:色相 - 色の種類（赤、青、黄色のような）。0～360の範囲（アプリケーションによっては0～100%に正規化されることもある）。
/// S:彩度 - 色の鮮やかさ。0～100%の範囲。刺激純度とcolorimeric purityの色彩的な量と比較して「純度」などともいう。色の彩度の低下につれて、灰色さが顕著になり、くすんだ色が現れ、また彩度の逆として「desaturation」を定義すると有益である。
/// V:明度 - 色の明るさ。0～100%の範囲。
/// </summary>
template< typename T = colortype >
struct CHSV : public base::SkyObject
{

	/// <summary>
	/// メンバ変数
	/// </summary>
	T	h;		//色相（0.0～360.0）
	T	s;		//彩度（0.0～1.0）
	T	v;		//明度（0.0～1.0）
		
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CHSV() {}
	CHSV( T fH, T fS, T fV ) :
		h	( fH ),
		s	( fS ),
		v	( fV ){}
	
	/// <summary>
	/// CRGB値を設定
	/// </summary>
	void FromCRGB( const CRGB< T >& rgb );
		
	/// <summary>
	/// CRGB値を取得
	/// </summary>
	CRGB< T > ToCRGB() const;

	/// <summary>
	/// 色相を取得
	/// </summary>
	static CRGBA_4byte< T > GetHueColor( T hue );

	/// <summary>
	/// CRGBA_4byte値を設定
	/// </summary>
	void FromCRGBA_4byte( const CRGBA_4byte< T > &rgba );
		
	/// <summary>
	/// CRGBA_4byte値を取得
	/// </summary>
	CRGBA_4byte< T > ToCRGBA_4byte() const;
		
	/// <summary>
	/// CRGBA値を設定
	/// </summary>
	void FromCRGBA( const CRGBA< T > &rgba );
		
	/// <summary>
	/// CRGBA値を取得
	/// </summary>
	CRGBA< T > ToCRGBA() const;
		
	/// <summary>
	/// 演算子オーバーライド
	/// </summary>
	CHSV operator* ( T value ) const
	{
		return HSV( h * value, s * value, v * value );
	}
	CHSV operator+ ( const CHSV& kHSV ) const
	{
		return HSV( h + kHSV.h, s + kHSV.s, v + kHSV.v );
	}
	CHSV operator- ( const CHSV& kHSV ) const
	{
		return HSV( h - kHSV.h, s - kHSV.s, v - kHSV.v );
	}
	skyBool operator== ( const CHSV& kHSV ) const
	{
		//0.00001以下の差ならば同じ色と判定する
		return ( fabs( h - kHSV.h ) + fabs( s - kHSV.s ) + fabs( v - kHSV.v ) ) <= 0.00001f;
	}

};

/// <summary>
/// 基本HSV型
/// </summary>
typedef CHSV< colortype >	CBasicHSV;

/// <summary>
/// 色定数
/// </summary>
static const CBasicHSV CHSV_DeepskyBlue          ( 195.0f , 1.0f , 0.5f );
static const CBasicHSV CHSV_Red                  ( 29.0f  , 1.0f , 0.5f );
static const CBasicHSV CHSV_Lawngreen            ( 90.5f  , 1.0f , 0.5f );

} } }