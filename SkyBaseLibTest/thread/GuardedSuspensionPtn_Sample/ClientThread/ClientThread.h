#pragma once

/// <summary>
/// �N���C�A���g�X���b�h
/// </summary>
class ClientThread : public sky::lib::thread::ThreadWin32
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ClientThread( RequestQueuePtr& queue );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~ClientThread( void );

	/// <summary>
	/// �X���b�h���s�֐�
	/// </summary>
	virtual void ThreadMain( void );

private:

	/// <summary>
	/// �L���[�|�C���^
	/// </summary>
	RequestQueuePtr		m_spRq;

};
