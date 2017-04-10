#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// 様々なグラフのY値をX値を指定することで取得する
/// Xの範囲と精度を指定することでグラフをプロットした点集合を返す
/// </summary>
namespace CGraphUtil
{
	/// <summary>
	/// 直線 y = ax + bにおいて、xの範囲を指定し , a , bの値を指定して、0.0f～1.0f範囲内のnowを指定することでその位置でのxとyを返す。
	/// </summary>
	inline CBasicVector3 &GetStraightLinePoint( CBasicVector3 &dest , dectype beginX , dectype endX , dectype now , dectype a = 1 , dectype b = 0 , dectype z = 0 );

	/// <summary>
	/// 直線のプロット値を返す。accuracy値が低ければ低いほどグラフの精度が増す。
	/// </summary>
	inline CBasicVector3Vector &DotsStraightLine( CBasicVector3Vector &dest , dectype beginX , dectype endX , dectype accuracy = 0.1f , dectype a = 1 , dectype b = 0 , dectype z = 0 );

	/// <summary>
	/// 2次曲線 y = ax~2 + bにおいて、xの範囲を指定し , a , bの値を指定して、0.0f～1.0f範囲内のnowを指定することでその位置でのxとyを返す。
	/// </summary>
	inline CBasicVector3 &GetQuadraticCurvePoint( CBasicVector3 &dest , dectype beginX , dectype endX , dectype now , dectype a = 1 , dectype b = 0 , dectype z = 0 );

	/// <summary>
	/// 2次曲線のプロット値を返す。accuracy値が低ければ低いほどグラフの精度が増す。
	/// </summary>
	inline CBasicVector3Vector &DotsQuadraticCurve( CBasicVector3Vector &dest , dectype beginX , dectype endX , dectype accuracy = 0.001f , dectype a = -0.1f , dectype b = 0 , dectype z = 0 );

	/// <summary>
	/// 3次曲線 y = ax~3 + bにおいて、xの範囲を指定し , a , bの値を指定して、0.0f～1.0f範囲内のnowを指定することでその位置でのxとyを返す。
	/// </summary>
	inline CBasicVector3 &GetCubicCurvePoint( CBasicVector3 &dest , dectype beginX , dectype endX , dectype now , dectype a = 1 , dectype b = 0 , dectype z = 0 );

	/// <summary>
	/// 3次曲線のプロット値を返す。accuracy値が低ければ低いほどグラフの精度が増す。
	/// </summary>
	inline CBasicVector3Vector &DotsCubicCurveCurve( CBasicVector3Vector &dest , dectype beginX , dectype endX , dectype accuracy = 0.001f , dectype a = -0.001f , dectype b = 0 , dectype z = 0 );

	/// <summary>
	/// 円 x = r * cosθ , y = r * sinθ r=円の半径において半径r、角度θ（ラジアン）の範囲を指定して0.0f～1.0f範囲内のnowを指定することでその位置でのxとyを返す。
	/// </summary>
	inline CBasicVector3 &GetCirclePoint( CBasicVector3 &dest , dectype r , dectype now , dectype beginRad = math::CMathUtil::DegToRad( 0 ) , dectype endRad = math::CMathUtil::DegToRad( 360 ) , dectype z = 0 );

	/// <summary>
	/// 円のプロット値を返す。accuracy値が低ければ低いほどグラフの精度が増す。
	/// </summary>
	inline CBasicVector3Vector &DotsCircle( CBasicVector3Vector &dest , dectype r , dectype beginRad = math::CMathUtil::DegToRad( 0 ) , dectype endRad = math::CMathUtil::DegToRad( 360 ) , dectype accuracy = 0.01 , dectype z = 0 );

	/// <summary>
	/// 楕円 x = ar * cosθ , y = br sinθ ar=x軸半径 br=y軸半径 角度θ（ラジアン）の範囲を指定して0.0f～1.0f範囲内のnowを指定することでその位置でのxとyを返す。
	/// </summary>
	inline CBasicVector3 &GetEllipticalPoint( CBasicVector3 &dest , dectype ar , dectype br , dectype now , dectype beginRad = math::CMathUtil::DegToRad( 0 ) , dectype endRad = math::CMathUtil::DegToRad( 360 ) , dectype z = 0 );

	/// <summary>
	/// 楕円のプロット値を返す。accuracy値が低ければ低いほどグラフの精度が増す。
	/// </summary>
	inline CBasicVector3Vector &DotsElliptical( CBasicVector3Vector &dest , dectype ar , dectype br , dectype beginRad = math::CMathUtil::DegToRad( 0 ) , dectype endRad = math::CMathUtil::DegToRad( 360 ) , dectype accuracy = 0.01 , dectype z = 0 );

	/// <summary>
	/// 双曲線 x = a / cosθ y = b * tanθ 角度θ（ラジアン）の範囲を指定して0.0f～1.0f範囲内のnowを指定することでその位置でのxとyを返す。
	/// </summary>
	inline CBasicVector3 &GetHyperbolaPoint( CBasicVector3 &dest , dectype a , dectype b , dectype now , dectype beginRad = math::CMathUtil::DegToRad( 0 ) , dectype endRad = math::CMathUtil::DegToRad( 360 ) , dectype z = 0 );

	/// <summary>
	/// 双曲線のプロット値を返す。accuracy値が低ければ低いほどグラフの精度が増す。
	/// </summary>
	inline CBasicVector3Vector &DotsHyperbola( CBasicVector3Vector &dest , dectype a , dectype b , dectype beginRad = math::CMathUtil::DegToRad( 0 ) , dectype endRad = math::CMathUtil::DegToRad( 360 ) , dectype accuracy = 0.01 , dectype z = 0 );

	/// <summary>
	/// トロコイド曲線 x = rm * θ - rd * sin(θ) , y = rm - rd * cos(θ) 動円半径rmを指定し、描画点半径rdを指定し、角度θ（ラジアン）の範囲を指定して0.0f～1.0f範囲内のnowを指定することでその位置でのxとyを返す。
	/// rm < rdのとき、1回の回転でx軸と2回交わる。
	/// rm = rdのとき、1回の回転でx軸と1回接し、曲線はサイクロイドとなる。
	/// rm > rdのとき、x軸と交わらない。
	/// </summary>
	inline CBasicVector3 &GetTrochoidPoint( CBasicVector3 &dest , dectype rm , dectype rd , dectype now , dectype beginRad = math::CMathUtil::DegToRad( -360 ) , dectype endRad = math::CMathUtil::DegToRad( 360 ) , dectype z = 0 );

	/// <summary>
	/// トロコイド曲線のプロット値を返す。accuracy値が低ければ低いほどグラフの精度が増す。
	/// </summary>
	inline CBasicVector3Vector &DotsTrochoid( CBasicVector3Vector &dest , dectype rm , dectype rd , dectype beginRad = math::CMathUtil::DegToRad( -360 ) , dectype endRad = math::CMathUtil::DegToRad( 360 ) , dectype accuracy = 0.01 , dectype z = 0 );

	/// <summary>
	/// クロソイド曲線 x = r * cos( θ~2 / 2 ) * θ , y = r * sin( θ~2 / 2 ) * θ 半径rを指定し、角度θ（ラジアン）の範囲を指定して0.0f～1.0f範囲内のnowを指定することでその位置でのxとyを返す。
	/// </summary>
	inline CBasicVector3 &GetClothoidPoint( CBasicVector3 &dest , dectype r , dectype now , dectype beginRad = math::CMathUtil::DegToRad( -360 ) , dectype endRad = math::CMathUtil::DegToRad( 360 ) , dectype z = 0 );

	/// <summary>
	/// クロソイド曲線のプロット値を返す。accuracy値が低ければ低いほどグラフの精度が増す。
	/// </summary>
	inline CBasicVector3Vector &DotsClothoid( CBasicVector3Vector &dest , dectype r , dectype beginRad = math::CMathUtil::DegToRad( -360 ) , dectype endRad = math::CMathUtil::DegToRad( 360 ) , dectype accuracy = 0.01 , dectype z = 0 );

//-----グラフとは関係ないですが便利関数

	/// <summary>
	/// 極座標(r,θ)を直交座標(x,y)へ変換する。半径r 角度θを渡し、座標x,yを得る。
	/// </summary>
	inline void ConvertFromPolarCoordinatesToRectangularCoordinate( CBasicVector2 &dest , const CBasicVector2 &src );

	/// <summary>
	/// 直交座標(x,y)を極座標(r,θ)へ変換する。座標x,yを渡し、半径r 角度θを得る。（反時計回りを正とする）
	/// 直線の角度などを求めるのに便利
	/// </summary>
	inline void ConvertFromRectangularCoordinateToPolarCoordinates( CBasicVector2 &dest , const CBasicVector2 &src );

	/// <summary>
	/// 極座標 (r,θ,φ)を直交座標 (x,y,z) へ変換する。半径r、XY平面回転角度θ、仰角φを渡し、座標x,y,zを得る(φの扱いが一般的な極座標とは異なる）
	/// </summary>
	inline void ConvertFromPolarCoordinatesToRectangularCoordinate( CBasicVector3 &dest , const CBasicVector3 &src );

	/// <summary>
	/// 直交座標 (x,y,z)を極座標 (r,θ,φ)を へ変換する。(φの扱いが一般的な極座標とは異なる）
	/// </summary>
	inline void ConvertFromRectangularCoordinateToPolarCoordinates( CBasicVector3 &dest , const CBasicVector3 &src );

};

} } }

#include "GraphUtil.inl"