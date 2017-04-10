#pragma once

namespace sky { namespace lib { namespace time {

/// <summary>
/// ���ԊԊu�ێ�����N���X
/// </summary>
class CRegulator : public base::SkyObject
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CRegulator();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CRegulator( updateTime radyIntvlTime );

	/// <summary>
	/// ���Ԃ��o�߂������H
	/// </summary>
	inline skyBool IsReady( updateTime time );

	/// <summary>
	/// �����������ԊԊu�̐ݒ�
	/// </summary>
	inline void SetReadyIntvlTime( updateTime radyIntvlTime );

private:

	/// <summary>
	/// �����������ԊԊu
	/// </summary>
	updateTime m_ReadyIntvlTime;

	/// <summary>
	/// �ݐώ���
	/// </summary>
	updateTime m_AccumulatedTime;
};

} } }

#include "Regulator.inl"