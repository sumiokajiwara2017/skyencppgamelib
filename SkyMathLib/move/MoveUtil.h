#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �_�̓������v�Z����֗��֐��Q
/// �x�N�g���A�s����g�p���ē_�̍��W�ϊ����s���B
/// </summary>
class CMoveUtil : public base::SkyObject
{

public:

	/// <summary>
	/// �w��̓_���w��̍s��ŕϊ������l��Ԃ�
	/// </summary>
	template< typename VECTOR3 >
	static inline VECTOR3 VectorXMatrix( const VECTOR3 &v , const CMatrix4x3 &m );

	/// <summary>
	/// �w��̓_�𒆐S�Ɏw��̓_����]����B��]���͓_�Ɠ_�����ԃx�N�g���̖@�����g�p����
	/// </summary>
	template< typename VECTOR3 >
	static inline VECTOR3 RotPoint( const VECTOR3 &point , const VECTOR3 &axis , dectype theta );

};

} } }

#include "MoveUtil.inl"