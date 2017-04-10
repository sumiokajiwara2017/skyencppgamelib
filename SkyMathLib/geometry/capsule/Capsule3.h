#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �J�v�Z���R���N���X
/// �J�v�Z���R�Ƃ͂Q�̋����`��鑾���������������ł���
/// </summary>
template< typename T >
class Capsule3 : public CGeometryBase
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Capsule3();
	Capsule3( CSphere< T > sphere , CVector3< T > vec );
	Capsule3( T x1 , T y1 , T z1 , T x2 , T y2 , T z2 , T radius );

	/// <summary>
	/// �[�_�P
	/// </summary>
	CVector3< T >	m_Pos1;

	/// <summary>
	/// �[�_�Q
	/// </summary>
	CVector3< T >	m_Pos2;

	/// <summary>
	/// ���a
	/// </summary>
	T			    m_Radius;
};

//��{�~�^
typedef Capsule3< dectype >     CBasicCapsule3;


} } }

#include "Capsule3.inl"