#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// ���N���X�i�Q�����j
/// �����F�n�_(pos1)�E�I�_(pos2)
/// </summary>
template< typename T >
class CLine2 : public CGeometryBase
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CLine2();
	CLine2( const CVector2< T > &pos1 , const CVector2< T > &pos2 );

	/// <summary>
	/// �����̋����S�̂�1�Ƃ������́A�w���0�`1�̒l�ɂ�������W��Ԃ�
	/// </summary>
	CVector2< T > Interpolate( f32 t ) const;

	/// <summary>
	/// �����͂Q�_�ł���킳���B�܂��͈�̓_�ƕ����x�N�g���B
	/// </summary>
	CVector2< T >	m_Pos1;
	CVector2< T >	m_Pos2;
};

//��{�Q������
typedef CLine2< dectype >     CBasicLine2;

} } }

#include "Line2.inl"
