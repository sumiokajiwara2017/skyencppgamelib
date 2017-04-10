#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// レイクラス（２次元）
/// 直線：始点(pos)・ベクトル(vec)
/// </summary>
template< typename T >
class CRay2 : public CGeometryBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CRay2();
	CRay2( const CBasicVector2 &pos , const CBasicVector2 &vec );

	/// <summary>
	/// 直線は２点であらわされる。または一つの点と方向ベクトル。
	/// </summary>
	CVector2< T >	m_Pos;
	CVector2< T >	m_Vec;
};

//基本２次元線
typedef CRay2< dectype >     CBasicRay2;

} } }

#include "Ray2.inl"

