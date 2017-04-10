#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// 線クラス（３次元）
/// 線分：始点(pos1)・終点(pos2)
/// </summary>
template< typename T >
class CLine3 : public CGeometryBase
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CLine3();
	CLine3( const CVector3< T > &pos1 , const CVector3< T > &pos2 );

	/// <summary>
	/// 線分の距離全体を1とした時の、指定の0～1の値における座標を返す
	/// </summary>
	CVector3< T > Interpolate( f32 t ) const;

	/// <summary>
	/// 直線は２点であらわされる。または一つの点と方向ベクトル。
	/// </summary>
	CVector3< T >	m_Pos1;
	CVector3< T >	m_Pos2;
};

//基本３次元線
typedef CLine3< dectype >          CBasicLine3;
typedef CList< u32 , CBasicLine3 > CBasicLine3List;

} } }

#include "Line3.inl"
