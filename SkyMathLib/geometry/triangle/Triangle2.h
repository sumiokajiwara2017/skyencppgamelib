#pragma once

#define TRIANGLE2_POINT_NUM		( 2 )

namespace sky { namespace lib { namespace math {

/// <summary>
/// 三角形情報クラス
/// </summary>
template< typename T >
class CTriangle2 : public CGeometryBase
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CTriangle2(){};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CTriangle2();
	CTriangle2( const CVector2< T >& top, const CVector2< T >& left, const CVector2< T >& right );

	/// <summary>
	/// 作成
	/// </summary>
	void Create( const CVector2< T >& top, const CVector2< T >& left, const CVector2< T >& right );

	/// <summary>
	/// 座標
	/// </summary>
	CVector2< T >	m_Pos[ TRIANGLE2_POINT_NUM ];
};

//基本円型
typedef CTriangle2< dectype >     CBasicTriangle2;

} } }

#include "Triangle2.inl"