#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// ���C�N���X�i�R�����j
/// �����F�n�_(pos)�E�x�N�g��(vec)
/// </summary>
template< typename T >
class CRay3 : public CGeometryBase
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CRay3( const CVector3< T > &pos , const CVector3< T > &vec );

	/// <summary>
	/// �����͂Q�_�ł���킳���B�܂��͈�̓_�ƕ����x�N�g���B
	/// </summary>
	CVector3< T >	m_Pos;
	CVector3< T >	m_Vec;
};

//��{�R������
typedef CRay3< dectype >     CBasicRay3;

} } }

#include "Ray3.inl"
