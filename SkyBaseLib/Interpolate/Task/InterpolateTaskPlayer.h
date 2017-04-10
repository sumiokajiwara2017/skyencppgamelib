#pragma once

namespace sky { namespace lib { namespace interp {

/// <summary>
/// ��ԃ^�X�N�Đ��@
/// </summary>
class CInterpolateTaskPlayer : public base::SkyRefObject
{

friend class CInterpolateTaskBase;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CInterpolateTaskPlayer(){};

	/// <summary>
	/// �X�V
	/// </summary>
	void Update( updateTime time );

	/// <summary>
	/// ��~�ݒ�iUpdate��time��0�ɂ���Γ��l�̌��ʂ�������j
	/// </summary>
	void SetStop( skyBool isStop );

	/// <summary>
	/// �^�X�N�o�^
	/// </summary>
	void RegisterTask( const CInterpolateTaskBasePtr &pTask );

	/// <summary>
	/// �^�X�N�o�^����
	/// </summary>
	void UnRegisterTask( const CInterpolateTaskBasePtr &pTask );

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CInterpolateTaskPlayer();

private:

	/// <summary>
	/// ��~�t���O
	/// </summary>
	skyBool m_IsStop;

	/// <summary>
	/// �^�X�N���X�g
	/// </summary>
	CInterpolateTaskBasePtrList m_TaskList;
};

} } }
