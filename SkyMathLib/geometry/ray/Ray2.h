#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// ���C�N���X�i�Q�����j
/// �����F�n�_(pos)�E�x�N�g��(vec)
/// </summary>
template< typename T >
class CRay2 : public CGeometryBase
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CRay2();
	CRay2( const CBasicVector2 &pos , const CBasicVector2 &vec );

	/// <summary>
	/// �����͂Q�_�ł���킳���B�܂��͈�̓_�ƕ����x�N�g���B
	/// </summary>
	CVector2< T >	m_Pos;
	CVector2< T >	m_Vec;
};

//��{�Q������
typedef CRay2< dectype >     CBasicRay2;

} } }

#include "Ray2.inl"

