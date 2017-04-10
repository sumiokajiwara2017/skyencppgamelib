#pragma once

namespace sky { namespace lib { namespace thread {

class CWorkerThreadManager;
typedef SmartPointer< CWorkerThreadManager >	CWorkerThreadManagerPtr;	

/// <summary>
/// ���[�J�[�X���b�h�Ǘ�
/// FunctionCallBackAsyncPtr��n���Η]�T�̂��鎞���s���Ă����B
/// �I�������FunctionCallBackAsyncPtr��IsEnd()�֐��ōs���B
/// </summary>
class CWorkerThreadManager : public base::SkyRefObject
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CWorkerThreadManager();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CWorkerThreadManager();

	/// <summary>
	/// ���[�J�[�X���b�h�N��
	/// </summary>
	void StartWorker( u32 workerNum = DEFAULT_WORKER_NUM , u32 stackSize = DEFAULT_STACK_SIZE );

	/// <summary>
	/// ���[�J�[�X���b�h�S�̂��N����
	/// ���[�J�[�X���b�h�͎d���������Ǝ����ő҂���Ԃɓ��邽�߁A�d���������Ƃ��ɋN�����Ȃ��Ƃ����Ȃ��B
	/// </summary>
	void WakeUpWorker();

	/// <summary>
	/// ���[�J�[�X���b�h�I���v��(�d�����̏ꍇ�͎d�����I����Ă���I�����܂��j
	/// </summary>
	void EndWorkerRequest();

	/// <summary>
	/// ���[�J�[�X���b�h�I���҂�
	/// </summary>
	void WaitEndWorker();

	/// <summary>
	/// ���[�J�[�X���b�h�I������
	/// </summary>
	skyBool IsEndWoker();

	/// <summary>
	/// �˗���������i���s�҂��Ǝ��s���̈˗������݂��Ȃ��ꍇskyTrue )
	/// </summary>
	skyBool IsEndWork();

	/// <summary>
	/// �˗������҂�
	/// </summary>
	void WaitEndWork();

	/// <summary>
	/// �d���˗�
	/// </summary>
	void RequestWork( const FunctionCallBackAsyncPtr &spWork );

	/// <summary>
	/// �d���˗��L�����Z���i�L���[�̎��s�˗�����N���A���܂��B���Ɏ��s����Ă��܂����ꍇ�̓��[�U�[�Œ�~���Ă��������B���ʏ����ŋ�����~������̂͊댯�Ȃ̂ŁB�j
	/// </summary>
	void CancelWork( const FunctionCallBackAsyncPtr &spWork );
	void CancelWorkAll();

//---static

	//�V���O���g���@�\��t��
	friend class SingletonPtr< CWorkerThreadManager >;
	static SingletonPtr< CWorkerThreadManager > Singleton;

private:

	/// <summary>
	/// ���[�J�[�X���b�h���X�g
	/// </summary>
	CWorkerThreadPtrList m_WorkerList;

	/// <summary>
	/// �d���˗��L���[�i��������r���������s���j
	/// </summary>
	CWorkQueuePtr m_spQueue;
};

} } }


//�C���X�^���X�A�N�Z�X������
#define CWorkerThreadManagerCreate_()	sky::lib::thread::CWorkerThreadManager::Singleton.CreateInstance()
#define CWorkerThreadManager_			sky::lib::thread::CWorkerThreadManager::Singleton.GetInstance()
#define CWorkerThreadManagerDelete_()	sky::lib::thread::CWorkerThreadManager::Singleton.DeleteInstance()
