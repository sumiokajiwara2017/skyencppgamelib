#pragma once

/// <summary>
///�f�[�^���󂯎��X���b�h�N���X
/// </summary>
class ConsumerThread : public sky::lib::thread::ThreadWin32
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ConsumerThread( Channel *pChannel );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~ConsumerThread( void );

	/// <summary>
	/// �������\�b�h
	/// </summary>
	void ThreadMain( void );

private:

	/// <summary>
	/// �`�����l��
	/// </summary>
	Channel				*m_pChannel;

};