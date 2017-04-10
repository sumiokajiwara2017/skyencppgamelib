#pragma once

#define TRIANGLE3_POINT_NUM		( 3 )

namespace sky { namespace lib { namespace math {

/// <summary>
/// 三角形情報クラス
/// </summary>
template< typename T >
class CTriangle3 : public CGeometryBase
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CTriangle3(){};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CTriangle3();
	CTriangle3( const CVector3< T >& top, const CVector3< T >& left, const CVector3< T >& right );

	/// <summary>
	/// 作成
	/// </summary>
	void Create( const CVector3< T >& top, const CVector3< T >& left, const CVector3< T >& right );

	/// <summary>
	/// 座標
	/// </summary>
	CVector3< T >	m_Pos[ TRIANGLE3_POINT_NUM ];

	/// <summary>
	/// 法線
	/// </summary>
	CVector3< T >	m_Normal;
};

//基本円型
typedef CTriangle3< dectype >     CBasicTriangle3;

} } }

#include "Triangle3.inl"