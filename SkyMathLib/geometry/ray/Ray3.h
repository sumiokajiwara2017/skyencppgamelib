#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// レイクラス（３次元）
/// 直線：始点(pos)・ベクトル(vec)
/// </summary>
template< typename T >
class CRay3 : public CGeometryBase
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CRay3( const CVector3< T > &pos , const CVector3< T > &vec );

	/// <summary>
	/// 直線は２点であらわされる。または一つの点と方向ベクトル。
	/// </summary>
	CVector3< T >	m_Pos;
	CVector3< T >	m_Vec;
};

//基本３次元線
typedef CRay3< dectype >     CBasicRay3;

} } }

#include "Ray3.inl"
