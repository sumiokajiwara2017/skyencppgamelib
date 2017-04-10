#pragma once

namespace sky { namespace lib { namespace thread {

/// <summary>
/// �N���e�B�J���Z�N�V����
/// </summary>
class CriticalSection : public base::SkyObject
{

public:

	/// <summary>
	/// �쐬
	/// </summary>
	static CriticalSection *Create();

	/// <summary>
	/// �폜
	/// </summary>
	static void Delete( CriticalSection *pCs );

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CriticalSection();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CriticalSection();

	/// <summary>
	/// �r���J�n�i�N���e�B�J���Z�N�V�����ɓ���j
	/// </summary>
	inline void Enter();

	/// <summary>
	/// �r���J�n�i�N���e�B�J���Z�N�V��������o��j
	/// </summary>
	inline void Leave();

private:

	/// <summary>
	/// ���b�N�X���b�h
	/// </summary>
	tread_id m_LoackThread;

	/// <summary>
	/// ���b�N�J�E���g
	/// </summary>
	s32	m_LockCount;

#ifdef SKYLIB_PLATFORM_WIN32

	/// <summary>
	/// Windows�N���e�B�J���Z�N�V�������
	/// </summary>
	CRITICAL_SECTION m_Cs;

#endif

};

/// <summary>
/// �N���e�B�J�����b�N
/// </summary>
class CriticalLock : public base::SkyObject
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CriticalLock( CriticalSection *pCs );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CriticalLock();

	/// <summary>
	///	�����I��CriticalSection�ɓ���
	/// </summary>
	inline void	Enter();

	/// <summary>
	///	�����I��CriticalSection���甲����
	/// </summary>
	inline void	Leave();

	/// <summary>
	///	Lock�J�E���g�̎擾
	/// </summary>
	inline s32	GetCount();

private:

	/// <summary>
	/// �Ǘ����Ă���N���e�B�J���Z�N�V����
	/// </summary>
	CriticalSection *m_pCs;

	/// <summary>
	///	���b�N�J�E���g
	/// </summary>
	u32 m_LockCount;
};

} } }

#include "CriticalSection.inl"