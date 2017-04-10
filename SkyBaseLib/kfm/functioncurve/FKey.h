#pragma once

namespace sky { namespace lib { namespace kfm {

/// <summary>
/// �t�@���N�V�����J�[�u�L�[�x�[�X
/// </summary>
class CFKeyBase : public base::SkyObject
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CFKeyBase(){};

	/// <summary>
	/// ���Ԃ̎擾�^�ݒ�
	/// </summary>
	updateTime  GetTime() const { return m_Time; }
	void SetTime( updateTime time ){ m_Time = time; }

	/// <summary>
	/// ��ԃ^�C�v�̎擾�^�ݒ�
	/// </summary>
	eInterpolateType GetInterpType() const { return m_eInterpType; }
	void SetInterpType( eInterpolateType eInterpType ){ m_eInterpType = eInterpType; }

protected:

	/// <summary>
	/// ����
	/// </summary>
	updateTime m_Time;

	/// <summary>
	/// ��ԃ^�C�v
	/// </summary>
	eInterpolateType m_eInterpType;

};

/// <summary>
/// �t�@���N�V�����J�[�u�L�[
/// CFKey��FValue�͈�Έ�B
/// </summary>
template < class T >
class CFKey : public CFKeyBase
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CFKey( T value );

	/// <summary>
	/// �l�̎擾
	/// </summary>
	inline CFValue< T > &GetValue();

	/// <summary>
	/// *���Z�q�̃I�[�o�[���C�h
	/// </summary>
	T& operator *(); 

	/// <summary>
	/// *���Z�q�̃I�[�o�[���C�h
	/// </summary>
	T* operator &(); 

private:

	/// <summary>
	/// �l
	/// </summary>
	CFValue< T > m_Value;

};

} } }

#include "FKey.inl"