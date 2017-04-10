#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// カプセル２情報クラス
/// カプセル２とは２つの円が形作る太さを持った線分である
/// </summary>
template< typename T >
class Capsule2 : public CGeometryBase
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Capsule2();
	Capsule2( T x1 , T y1 , T x2 , T y2 , T radius );

	/// <summary>
	/// 端点１
	/// </summary>
	CVector2< T >	m_Pos1;

	/// <summary>
	/// 端点２
	/// </summary>
	CVector2< T >	m_Pos2;

	/// <summary>
	/// 半径
	/// </summary>
	T			    m_Radius;
};

//基本円型
typedef Capsule2< dectype >     CBasicCapsule2;


} } }

#include "Capsule2.inl"