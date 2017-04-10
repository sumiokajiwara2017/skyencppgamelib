#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// 定数
/// <summary>
namespace util
{
	static const dectype KPI		    = ( dectype )3.14159265f;
	static const dectype K2PI		    = ( dectype )KPI * 2.0f;
	static const dectype KPIOVER2	    = ( dectype )KPI / 2.0f;
	static const dectype K1OVERPI	    = ( dectype )1.0f / KPI;
	static const dectype K1OVER2PI	    = ( dectype )1.0f / K2PI;
	static const dectype KPIOVER180	    = ( dectype )KPI / 180.0f;
	static const dectype K180OVERPI	    = ( dectype )180.0f / KPI;
	static const dectype EPSILON        = ( dectype )1e-08;
    static const dectype EPSILON_LENGTH = ( dectype )1 - EPSILON;
}

//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
/// 数学関連共通処理
/// </summary>
class CMathUtil : public base::SkyObject
{

public:

	/// <summary>
	/// 平方根(２乗したら）を求める
	/// </summary>
	static inline dectype Sqrt( dectype val );

	/// <summary>
	/// 正弦を返します。単位円周上のY座標の値
	/// 正弦＝底辺／斜辺
	/// 正弦(90度) = 1.0
	/// 正弦(45度) = 0.7071067811865476
	/// 正弦(0度)  = 0.0
	/// </summary>
	static inline dectype Sin( dectype asin );

	/// <summary>
	/// Sinの逆関数を計算し、
	/// 結果を 0 から π までのラジアンで返します。
	/// 逆正弦(1.0)					= 90度（のラジアン表記）
	/// 逆正弦(0.7071067811865476)	= 45度（のラジアン表記）
	/// 逆正弦(0.0)					= 0度（のラジアン表記）
	/// </summary>
	static inline dectype Asin( dectype sin );

	/// <summary>
	/// 余弦を返します。単位円周上のX座標の値
	/// 余弦＝斜辺／底辺
	/// 余弦(90度) = 0.0
	/// 余弦(45度) = 0.7071067811865476
	/// 余弦(0度)  = 1.0
	/// </summary>
	static inline dectype Cos( dectype acos );

	/// <summary>
	/// Cosの逆関数を計算し、
	/// 結果を 0 から π までのラジアンで返します。
	/// 逆余弦(0.0)					= 90度（のラジアン表記）
	/// 逆余弦(0.7071067811865476)	= 45度（のラジアン表記）
	/// 逆余弦(1.0)					= 0度（のラジアン表記）
	/// </summary>
	static inline dectype Acos( dectype cos );

	/// <summary>
	/// 正接を計算返します
	/// 正接＝隣辺／底辺
	/// 正接(0度)	= 0.0 
	/// 正接(45度)	= 1.0 
	/// 正接(90度)	= ∞ 
	/// </summary>
	static inline dectype Tan( dectype atan );

	/// <summary>
	/// Tanの逆関数
	/// 結果を 0 から π までのラジアンで返します。
	/// 逆正接(0.0) = 0度（のラジアン表記）
	/// 逆正接(1.0) = 45度（のラジアン表記）
	/// </summary>
	static inline dectype Atan( dectype tan );

	/// <summary>
	/// Tanの逆数(cot)
	/// </summary>
	static inline dectype Ctan( dectype tan );

	/// <summary>
	/// Sinの逆関数
	/// Atanの結果を逆正接で計算します。
	/// </summary>
	static inline dectype Atan2( dectype x , dectype y );

    /// <summary>
    /// 適切に2πの倍数を加えることで角度を-π...πの範囲にラップする
    /// </summary>
	static inline dectype WrapPi( dectype theta );

    /// <summary>
    /// acos(x)と同じ
    /// ただし、xが有効外の場合、最も近い有効な値にクランプ処理される
    /// 返される値は、範囲0...πであり、これは標準のCのacos()関数と同じである。
    /// </summary>
	static inline dectype SafeAcos( dectype cos );

    /// <summary>
    /// 引数の値以下で最も近い整数値を返す（切捨て）
    /// </summary>
	static inline dectype	Floor( dectype val );

    /// <summary>
	/// 絶対値を返します。
    /// </summary>
	static inline dectype Abs( dectype val );

    /// <summary>
	/// 浮動小数点の絶対値を返します。
    /// </summary>
	static inline s32 FAbs( s32 val );
	static inline dectype FAbs( dectype val );

    /// <summary>
	///  引数の値以上で最も近い整数値を返す（切り上げ）
    /// </summary>
	static inline dectype Ceil( dectype val );

    /// <summary>
	///  四捨五入
    /// </summary>
	static inline dectype Round( dectype val );

	/// <summary>
	/// 度からラジアンへ
	/// </summary>
	static inline dectype	DegToRad( dectype deg );

	/// <summary>
	/// ラジアンから度へ
	/// </summary>
	static inline dectype	RadToDeg( dectype rad );

	/// <summary>
	///	Sin値とCos値を同時に得る
	/// </summary>
	static inline void		SinCos( dectype *returnSin, dectype *returnCos , dectype rad );

	/// <summary>
	///	視野とズーム間を変換します。３Ｄ数学の14.2.4参照。視野角は通常ラジアンで表現します。
	///
	/// 【３Ｄ数学抜粋文章】
	/// ズームは90度の視野に対するオブジェクトの見かけ上のサイズの割合を測ります。
	/// 例えば2.0のズームは、それが90度の視野を用いている場合に比べてそのオブジェクトが
	/// ２倍の大きさとして現れることを意味します。
	/// 視野は人間が使用するのに便利な値ですが、コンピューターが通常必要とする数字はズームです。
	///
	/// すなわち、画面上のものをすべて２倍の大きさにしたい場合、ズーム値が2.0ということで、
	/// その2.0は視野角をいくつにしたらそうなるのかということ。
	/// </summary>
	static inline dectype	FovToZoom( dectype fov );
	static inline dectype	ZoomToFov( dectype zoom );

    /// <summary>
	///  マイナスや360以上の数字の度を適切な360以内の数値に変換します
    /// </summary>
	static inline dectype  ConvDegIn360( s32 deg );

    /// <summary>
	///  単位円上の座標を返す
    /// </summary>
	static inline void GetUnitCirclePos( dectype deg , dectype *pPosX , dectype *pPosY , dectype length = 1.0f );

    /// <summary>
	///  サインカーブ コサインカーブを返す
    /// </summary>
	static inline void GetUnitCirclePosWave( dectype deg , dectype *pPosX , dectype *pPosY , dectype length = 1.0f );

    /// <summary>
	///  累乗を返す
    /// </summary>
	static inline dectype Pow( dectype k , dectype num );

    /// <summary>
	///  余りを返す
    /// </summary>
	static inline dectype Mod( dectype param1 , dectype param2 );

    /// <summary>
	///  指定の値が指定の誤差範囲内でイコールだったらTrueを返す
    /// </summary>
	static inline skyBool NearyEq( dectype src1 , dectype src2 , dectype gosa );

    /// <summary>
	///  指定の値が指定の最小値と最大値の範囲外だったらそれぞれ最小値と最大値に以内にまるめる
    /// </summary>
	template <class Type>
	static inline void Mul( Type &value , Type min , Type max );

private:

	//各機能封印
	CMathUtil(){};													//コンストラクタ
	CMathUtil( const CMathUtil& ){};							//コピーコンストラクタ
	virtual ~CMathUtil(){};											//デストラクタ
	CMathUtil &operator=(const CMathUtil& ){ return *this; };	//代入演算子オーバーロード
};

} } }

#define CMathUtil_	 sky::lib::math::CMathUtil

#include "MathUtil.inl"