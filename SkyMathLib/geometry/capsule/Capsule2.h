#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �J�v�Z���Q���N���X
/// �J�v�Z���Q�Ƃ͂Q�̉~���`��鑾���������������ł���
/// </summary>
template< typename T >
class Capsule2 : public CGeometryBase
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Capsule2();
	Capsule2( T x1 , T y1 , T x2 , T y2 , T radius );

	/// <summary>
	/// �[�_�P
	/// </summary>
	CVector2< T >	m_Pos1;

	/// <summary>
	/// �[�_�Q
	/// </summary>
	CVector2< T >	m_Pos2;

	/// <summary>
	/// ���a
	/// </summary>
	T			    m_Radius;
};

//��{�~�^
typedef Capsule2< dectype >     CBasicCapsule2;


} } }

#include "Capsule2.inl"