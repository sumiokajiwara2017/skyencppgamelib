#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �􉽊w�^
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
/// �􉽊w�\���x�[�X
/// </summary>
class CGeometryBase
{
	//���̃N���X�Ƀ����o�ϐ����`���Ắw�����܂���x
	//�f�[�^�T�C�Y���ς��ƕs�s��������܂��B
};

/// <summary>
/// �􉽊w�\���x�[�X
/// </summary>
class CGeometryData
{

public:

	/// <summary>
	/// �􉽊w�f�[�^
	/// </summary>
	const CGeometryBase* m_Geometry;

	/// <summary>
	/// �􉽊w�^
	/// </summary>
	eGeometoryType m_eType;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CGeometryData( eGeometoryType eType , const CGeometryBase *pGeometry ) : m_eType( eType ) , m_Geometry( pGeometry ) {} 
};


} } }