#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// 線クラス（２次元）
/// 線分：始点(pos1)・終点(pos2)
/// </summary>
template< typename T >
class CLine2 : public CGeometryBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CLine2();
	CLine2( const CVector2< T > &pos1 , const CVector2< T > &pos2 );

	/// <summary>
	/// 線分の距離全体を1とした時の、指定の0～1の値における座標を返す
	/// </summary>
	CVector2< T > Interpolate( f32 t ) const;

	/// <summary>
	/// 直線は２点であらわされる。または一つの点と方向ベクトル。
	/// </summary>
	CVector2< T >	m_Pos1;
	CVector2< T >	m_Pos2;
};

//基本２次元線
typedef CLine2< dectype >     CBasicLine2;

} } }

#include "Line2.inl"
