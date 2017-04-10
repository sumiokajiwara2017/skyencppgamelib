#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// 点の動きを計算する便利関数群
/// ベクトル、行列を使用して点の座標変換を行う。
/// </summary>
class CMoveUtil : public base::SkyObject
{

public:

	/// <summary>
	/// 指定の点を指定の行列で変換した値を返す
	/// </summary>
	template< typename VECTOR3 >
	static inline VECTOR3 VectorXMatrix( const VECTOR3 &v , const CMatrix4x3 &m );

	/// <summary>
	/// 指定の点を中心に指定の点を回転する。回転軸は点と点を結ぶベクトルの法線を使用する
	/// </summary>
	template< typename VECTOR3 >
	static inline VECTOR3 RotPoint( const VECTOR3 &point , const VECTOR3 &axis , dectype theta );

};

} } }

#include "MoveUtil.inl"