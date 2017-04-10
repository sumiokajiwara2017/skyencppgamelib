#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// 一般平面クラス
/// 平面とは、平面上の1点とその法線であらわされる。下記方程式がメジャー
/// 一般平面方程式 ax + by + cz = d
/// 法線(a,b,c) と d は平面上の点(x,y,z)と法線の内積なので、平面上の点(x,y,z)はdがあれば法線から導き出せる。
/// 結果平面を表す値としては4つの数字abcdがあればいいということになるので、このクラスのメンバはabcdになっている。
/// これで面の表か裏か、面と点の距離、面と点の角度など重要な情報を計算できる。
/// 参考URL : http://marupeke296.com/COL_Basic_No3_WhatIsPlaneEquation.html
/// </summary>
template< typename T >
class CPlane : public CGeometryBase
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CPlane();
	CPlane( const CVector3< T > &normal , const CVector3< T > &pos );
	CPlane( const CVector3< T > &pos1 , const CVector3< T > &pos2 , const CVector3< T > &pos3 );
	CPlane( const CTriangle3< T > &triangle );

	/// <summary>
	/// ３点から平面を作成する
	/// </summary>
	void CreatePlane( const CVector3< T > &pos1 , const CVector3< T > &pos2 , const CVector3< T > &pos3 );

	/// <summary>
	/// 法線と点から平面を作成する
	/// </summary>
	void CreatePlane( const CVector3< T > &normal , const CVector3< T > &pos );

	/// <summary>
	/// 三角形から平面を作成する
	/// </summary>
	void CreatePlane( const CTriangle3< T > &triangle );

	/// <summary>
	/// 任意の数の点から最適な面を作成する
	/// </summary>
	void CreatePlane( CVector3< T > pPoints[] , u32 pointNum );

	/// <summary>
	/// 点が面の表と裏どちらにあるか判定する(1:表 -1:裏 0:平面上）
	/// </summary>
	s32 ColPointPlaneSide( CVector3< T > &pos ) const;

	/// <summary>
	/// 点が面の内側にあるか判定する
	/// </summary>
	skyBool Inside( const CVector3< T > &point ) const;

	/// <summary>
	/// 球が面の内側にあるか判定する
	/// </summary>
	skyBool Inside( const CVector3< T > &point, const T radius ) const;

	/// <summary>
	/// 点と平面の内積を計算する
	/// 平面 (a, b, c, d) と 3D ベクトル (x, y, z) を指定した場合、この関数の戻り値は a*x + b*y + c*z + d*1 
	/// </summary>
	T Dot( const CVector3< T > &point ) const;

	/// <summary>
	/// 正規化
	/// </summary>
	CPlane &Normalize();

	/// <summary>
	/// キャスト演算子オーバーライド
	/// </summary>
    operator const CVector4< T >() const;

	/// <summary>
	/// 平面の法線(一般平面方程式におけるa , b, c )
	/// </summary>
	CVector3< T >	m_Normal;

	/// <summary>
	/// 法線nと平面上の点との内積
	/// </summary>
	T				d;

	/// <summary>
	/// 平面上の点（作成時に取っておく）
	/// </summary>
	CVector3< T >	m_Pos;

};

//基本円型
typedef CPlane< dectype >     CBasicPlane;

} } }

#include "Plane.inl"
