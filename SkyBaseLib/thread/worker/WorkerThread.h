#pragma once

namespace sky { namespace lib { namespace thread {

class CWorkerThread;
typedef SmartPointer< CWorkerThread >	CWorkerThreadPtr;
typedef CList< u32 , CWorkerThreadPtr >	CWorkerThreadPtrList;

/// <summary>
/// ���[�J�[�X���b�h
/// </summary>
class CWorkerThread : public thread::CThread
{

friend class CWorkerThreadManager;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CWorkerThread();

private:

	/// <summary>
	/// �҂��I�u�W�F�N�g
	/// </summary>
	CWaitObjectPtr m_spWaitObject;

	/// <summary>
	/// �X���b�h�~�σL���[�̎Q��
	/// </summary>
	CWorkQueuePtr m_spQueue;

	/// <summary>
	/// �L���t���O
	/// </summary>
	skyBool m_IsValid;

	/// <summary>
	/// ���s
	/// </summary>
	virtual void ThreadMain();

	/// <summary>
	/// �L�����ݒ�
	/// </summary>
	void SetValid( skyBool isValid );

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CWorkerThread( const CWorkQueuePtr& spQueue , u32 stackSize );
};

} } }