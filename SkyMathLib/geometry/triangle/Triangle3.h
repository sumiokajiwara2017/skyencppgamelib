#pragma once

#define TRIANGLE3_POINT_NUM		( 3 )

namespace sky { namespace lib { namespace math {

/// <summary>
/// �O�p�`���N���X
/// </summary>
template< typename T >
class CTriangle3 : public CGeometryBase
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CTriangle3(){};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CTriangle3();
	CTriangle3( const CVector3< T >& top, const CVector3< T >& left, const CVector3< T >& right );

	/// <summary>
	/// �쐬
	/// </summary>
	void Create( const CVector3< T >& top, const CVector3< T >& left, const CVector3< T >& right );

	/// <summary>
	/// ���W
	/// </summary>
	CVector3< T >	m_Pos[ TRIANGLE3_POINT_NUM ];

	/// <summary>
	/// �@��
	/// </summary>
	CVector3< T >	m_Normal;
};

//��{�~�^
typedef CTriangle3< dectype >     CBasicTriangle3;

} } }

#include "Triangle3.inl"