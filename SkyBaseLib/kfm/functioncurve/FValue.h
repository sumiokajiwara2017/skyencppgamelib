#pragma once

namespace sky { namespace lib { namespace kfm {

/// <summary>
/// �t�@���N�V�����J�[�u�L�[
/// CFKey��CFValue�͈�Έ�B
/// �l�Ƃ��Ďg�p����N���X�͉��L�̉��Z�q�̃I�[�o�[���C�h����K�v������B
/// ���̒�`�͕�ԃ��W�b�N�̒ǉ��ōX�ɑ�����\��������B�R�s�[�R���X�g���N�^��Y���Ɠ��삷�邪�������j�󂪔�������\��������
///
/// <summary>
/// �R�s�[�R���X�g���N�^
/// </summary>
///T( const T& src )
///
/// <summary>
/// =���Z�q�I�[�o�[���C�h
/// </summary>
///virtual T& operator =( const T& src )
///
/// <summary>
/// ==���Z�q�I�[�o�[���C�h
/// </summary>
///	skyBool operator ==( const T& src ) const
///
/// <summary>
/// *���Z�q�I�[�o�[���C�h
/// </summary>
///T operator *( colortype src ) const
///T operator *( const T &src ) const
///
/// <summary>
/// -���Z�q�I�[�o�[���C�h
/// </summary>
///T operator -( const T& src ) const
///
/// <summary>
/// +���Z�q�I�[�o�[���C�h
/// </summary>
///T operator +( const T& src ) const
///
/// </summary>
template < class T >
class CFValue : public base::SkyObject
{

public:

	/// <summary>
	/// �l
	/// </summary>
	T m_Value;

	/// <summary>
	/// *���Z�q�̃I�[�o�[���C�h
	/// </summary>
	T& operator *(); 
};

} } }

#include "FValue.inl"