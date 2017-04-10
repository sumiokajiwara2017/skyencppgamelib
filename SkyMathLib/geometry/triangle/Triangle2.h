#pragma once

#define TRIANGLE2_POINT_NUM		( 2 )

namespace sky { namespace lib { namespace math {

/// <summary>
/// �O�p�`���N���X
/// </summary>
template< typename T >
class CTriangle2 : public CGeometryBase
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CTriangle2(){};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CTriangle2();
	CTriangle2( const CVector2< T >& top, const CVector2< T >& left, const CVector2< T >& right );

	/// <summary>
	/// �쐬
	/// </summary>
	void Create( const CVector2< T >& top, const CVector2< T >& left, const CVector2< T >& right );

	/// <summary>
	/// ���W
	/// </summary>
	CVector2< T >	m_Pos[ TRIANGLE2_POINT_NUM ];
};

//��{�~�^
typedef CTriangle2< dectype >     CBasicTriangle2;

} } }

#include "Triangle2.inl"