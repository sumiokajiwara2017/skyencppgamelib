#pragma once

namespace sky { namespace lib { namespace thread {

class CWorkQueue;
typedef SmartPointer< CWorkQueue >	CWorkQueuePtr;

/// <summary>
/// ���[�N�L���[
/// </summary>
class CWorkQueue : public base::SkyRefObject
{

friend class CWorkerThreadManager;

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CWorkQueue();
	
	/// <summary>
	/// �X���b�h�J�E���g�A�b�v
	/// </summary>
	void IncThreadCount();

	/// <summary>
	/// �X���b�h�J�E���g�_�E��
	/// </summary>
	void DecThreadCount();

	/// <summary>
	/// �X���b�h�J�E���g�擾
	/// </summary>
	u32 GetThreadCount();

	/// <summary>
	/// �v����ݒ�i�r���j
	/// </summary>
	void PushRequest( const FunctionCallBackAsyncPtr &spFunc );

	/// <summary>
	/// �v�����擾�i�r���j
	/// </summary>
	FunctionCallBackAsyncPtr PopRequest();

	/// <summary>
	/// �v����j��
	/// </summary>
	void CanselRequest( const FunctionCallBackAsyncPtr &spFunc );
	void CanselAll();

	/// <summary>
	/// �v�������擾�i�r���j
	/// </summary>
	u32 GetRequestNum();

	/// <summary>
	/// ���s�����J�E���g�A�b�v
	/// </summary>
	void IncExecNum();

	/// <summary>
	/// ���s�����J�E���g�_�E��
	/// </summary>
	void DecExecNum();

	/// <summary>
	/// ���s�����擾
	/// </summary>
	u32 GetExecNum();

private:

	/// <summary>
	/// �N���e�B�J���Z�N�V����
	/// </summary>
	CriticalSection *m_pCs;

	/// <summary>
	/// ���s��
	/// </summary>
	u32 m_ExecNum;

	/// <summary>
	/// �X���b�h��
	/// </summary>
	u32 m_ThreadNum;

	/// <summary>
	/// ���[�N���X�g
	/// </summary>
	FunctionCallBackAsyncPtrList m_WorkList;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CWorkQueue();
};

} } }