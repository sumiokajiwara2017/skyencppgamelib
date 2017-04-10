#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// ���N���X�i�R�����j
/// �����F�n�_(pos1)�E�I�_(pos2)
/// </summary>
template< typename T >
class CLine3 : public CGeometryBase
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CLine3();
	CLine3( const CVector3< T > &pos1 , const CVector3< T > &pos2 );

	/// <summary>
	/// �����̋����S�̂�1�Ƃ������́A�w���0�`1�̒l�ɂ�������W��Ԃ�
	/// </summary>
	CVector3< T > Interpolate( f32 t ) const;

	/// <summary>
	/// �����͂Q�_�ł���킳���B�܂��͈�̓_�ƕ����x�N�g���B
	/// </summary>
	CVector3< T >	m_Pos1;
	CVector3< T >	m_Pos2;
};

//��{�R������
typedef CLine3< dectype >          CBasicLine3;
typedef CList< u32 , CBasicLine3 > CBasicLine3List;

} } }

#include "Line3.inl"
