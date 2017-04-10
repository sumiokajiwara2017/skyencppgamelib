#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// カプセル３情報クラス
/// カプセル３とは２つの球が形作る太さを持った線分である
/// </summary>
template< typename T >
class Capsule3 : public CGeometryBase
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Capsule3();
	Capsule3( CSphere< T > sphere , CVector3< T > vec );
	Capsule3( T x1 , T y1 , T z1 , T x2 , T y2 , T z2 , T radius );

	/// <summary>
	/// 端点１
	/// </summary>
	CVector3< T >	m_Pos1;

	/// <summary>
	/// 端点２
	/// </summary>
	CVector3< T >	m_Pos2;

	/// <summary>
	/// 半径
	/// </summary>
	T			    m_Radius;
};

//基本円型
typedef Capsule3< dectype >     CBasicCapsule3;


} } }

#include "Capsule3.inl"