#pragma once

/// <summary>
/// �T�[�o�[�X���b�h
/// </summary>
class ServerThread : public sky::lib::thread::ThreadWin32
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ServerThread( RequestQueuePtr& queue );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~ServerThread( void );

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
