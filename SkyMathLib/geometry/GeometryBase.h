#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// 幾何学型
/// </summary>
enum eGeometoryType
{
	eGeometoryType_point2 = 0 ,
	eGeometoryType_point3 ,
	eGeometoryType_point4 ,
	eGeometoryType_circle ,
	eGeometoryType_sphere ,
	eGeometoryType_ellipse ,
	eGeometoryType_line2 ,
	eGeometoryType_line3 ,
	eGeometoryType_ray2 ,
	eGeometoryType_ray3 ,
	eGeometoryType_plane ,
	eGeometoryType_triangle2 ,
	eGeometoryType_triangle3 ,
	eGeometoryType_capsule2 ,
	eGeometoryType_capsule3 ,
	eGeometoryType_rect ,
	eGeometoryType_aabb2 ,
	eGeometoryType_aabb3 ,
	eGeometoryType_obb2 ,
	eGeometoryType_obb3 ,
};

/// <summary>
/// 幾何学構造ベース
/// </summary>
class CGeometryBase
{
	//このクラスにメンバ変数を定義しては『いけません』
	//データサイズが変わると不都合があります。
};

/// <summary>
/// 幾何学構造ベース
/// </summary>
class CGeometryData
{

public:

	/// <summary>
	/// 幾何学データ
	/// </summary>
	const CGeometryBase* m_Geometry;

	/// <summary>
	/// 幾何学型
	/// </summary>
	eGeometoryType m_eType;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CGeometryData( eGeometoryType eType , const CGeometryBase *pGeometry ) : m_eType( eType ) , m_Geometry( pGeometry ) {} 
};


} } }