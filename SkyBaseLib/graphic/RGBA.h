#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// RGBA色空間クラス
/// RGBの色空間に加えて、アルファチャンネルも色決定に考慮させる。これは、透過（透明度）を表現するものである（厳密にはこれは色空間ではないとも考えられる）。
/// </summary>
template< typename T = colortype >
class CRGBA : public base::SkyObject
{
public:

	/// <summary>
	/// メンバ変数
	/// </summary>
    T r, g, b, a;

	/// <summary>
	/// コンストラクタ
	/// </summary>
    CRGBA (T lr = 0.0f, T lg = 0.0f, T lb = 0.0f, T la = 0.0f);

	/// <summary>
	/// rgb各要素値を1.0f以内に補正
	/// </summary>
    void Clamp();

	/// <summary>
	/// rgb各要素のうち最大の値を基準に各要素値を1.0f以内に補正
	/// </summary>
    void Scale();

	/// <summary>
	/// 4バイト表現のrgba値に変換
	/// </summary>
    void GetAs( CRGBA_4byte< T >& rgba ) const;
    u32  GetAs() const;
    void GetAsARGB( CRGBA_4byte< T >& argb ) const;
    u32  GetAsARGB() const;
    void GetAsABGR( CRGBA_4byte< T >& argb ) const;
    u32  GetAsABGR() const;

	/// <summary>
	/// 配列にデータをコピーする
	/// </summary>
	void Copyf32Array3RGB( f32 *dest ) const;
	void Copyf32Array4RGBA( f32 *dest ) const;

	/// <summary>
	/// 代入演算子のオーバーライド
	/// </summary>
    CRGBA< T >& operator= ( T value );
    CRGBA< T >& operator= ( const CRGBA< T >& rgba );

	/// <summary>
	/// 比較演算子のオーバーライド
	/// </summary>
    skyBool operator== ( const CRGBA< T >& rgba ) const;
    skyBool operator!= ( const CRGBA< T >& rgba ) const;

	/// <summary>
	/// 算術演算子のオーバーライド
	/// </summary>
    CRGBA< T > operator+ ( const CRGBA< T >& rgba ) const;
    CRGBA< T > operator- ( const CRGBA< T >& rgba ) const;
    CRGBA< T > operator* ( T value )			const;
    CRGBA< T > operator* ( const CRGBA< T >& rgba ) const;
    CRGBA< T > operator/ ( T value )			const;
    CRGBA< T > operator/ ( const CRGBA< T >& rgba ) const;
    CRGBA< T > operator- ()					const;

    CRGBA< T >& operator+= ( const CRGBA< T >& rgba );
    CRGBA< T >& operator-= ( const CRGBA< T >& rgba );
    CRGBA< T >& operator*= ( T value );
    CRGBA< T >& operator*= ( const CRGBA< T >& rgba );
    CRGBA< T >& operator/= ( T value );
    CRGBA< T >& operator/= ( const CRGBA< T >& rgba );
};

/// <summary>
/// 基本RGBA型
/// </summary>
typedef CRGBA< colortype >	CBasicRGBA;
typedef CBasicRGBA			CColor;
typedef CVectorPrimitive< CColor > CVector_CColor;

/// <summary>
/// 色定数
/// </summary>
static const CBasicRGBA CColor_White            ( 1.0f , 1.0f , 1.0f , 1.0f );
static const CBasicRGBA CColor_Black            ( 0.0f , 0.0f , 0.0f , 1.0f ); 
static const CBasicRGBA CColor_Red              ( 1.0f , 0.0f , 0.0f , 1.0f ); 
static const CBasicRGBA CColor_Green            ( 0.0f , 1.0f , 0.0f , 1.0f ); 
static const CBasicRGBA CColor_Blue             ( 0.0f , 0.0f , 1.0f , 1.0f ); 
static const CBasicRGBA CColor_DeepBlue         ( 0.0f , 0.125f, 0.3f, 1.0f ); 
static const CBasicRGBA CColor_Tomato           ( 1.0f , 0.38f , 0.27f , 1.0f ); 
static const CBasicRGBA CColor_Orangered        ( 1.0f , 0.27f , 0.0f , 1.0f ); 
static const CBasicRGBA CColor_Sandybrown       ( 0.95f, 0.64f , 0.37f, 1.0f ); 
static const CBasicRGBA CColor_Yellow           ( 1.0f , 1.0f  , 0.0f , 1.0f ); 
static const CBasicRGBA CColor_Violetw          ( 0.93f, 0.50f , 0.93f, 1.0f ); 
static const CBasicRGBA CColor_Silver           ( 0.75f , 0.75f , 0.75f , 1.0f ); 
static const CBasicRGBA CColor_Dimgray          ( 0.41f , 0.41f , 0.41f , 1.0f ); 
static const CBasicRGBA CColor_Lightseagreen    ( 0.12f , 0.69f , 0.66f , 1.0f ); 
static const CBasicRGBA CColor_Powderblue       ( 0.69f , 0.87f , 0.90f , 1.0f ); 
static const CBasicRGBA CColor_Mistyrose        ( 0.88f , 0.89f , 0.88f , 1.0f ); 
static const CBasicRGBA CColor_Lightsteelblue   ( 0.69f , 0.76f , 0.87f , 1.0f ); 
static const CBasicRGBA CColor_Wheat            ( 0.96f , 0.87f , 0.70f , 1.0f ); 
static const CBasicRGBA CColor_Gold             ( 1.00f , 0.84f , 0.00f , 1.0f ); 

} } }