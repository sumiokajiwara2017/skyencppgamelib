#pragma once

/// <summary>
/// ���N�G�X�g�L���[
/// </summary>
/*
ClientThread�����PutRequest(�L���[�ɐςށj�����B
ServerThread�����GetRequest(�L���[����擾����j�����B
�����ACServerThread����GetRequest���ꂽ�Ƃ��ɃL���[�Ƀf�[�^��
���݂��Ȃ���΁AClientThread����PutRequest�����̂�҂B
*/
class RequestQueue
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	RequestQueue( void );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~RequestQueue( void );

	/// <summary>
	/// ���N�G�X�g�f�[�^�ݒ�i�q�[�v�Ŋm�ۂ���RequestData�̃|�C���^��n���Ă��������B�j
	/// </summary>
	void PutRequest( RequestData *pRequest );

	/// <summary>
	/// ���N�G�X�g�f�[�^�擾
	/// </summary>
	RequestData *GetRequest( void );

private:

	/// <summary>
	/// ���N�G�X�g�f�[�^Fifo
	/// </summary>
	RequestDataPtrFifo	m_spRequestFifo;

	/// <summary>
	/// ���b�N�I�u�W�F�N�g
	/// </summary>
	sky::lib::thread::ILockObject *m_pLock;

};


typedef sky::lib::SmartPointerMT< RequestQueue >			RequestQueuePtr;	//SmartPointer�^
