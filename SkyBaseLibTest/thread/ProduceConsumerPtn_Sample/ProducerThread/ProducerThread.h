#pragma once

/// <summary>
/// �f�[�^�𑗂�X���b�h�N���X
/// </summary>
class ProducerThread : public sky::lib::thread::ThreadWin32
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ProducerThread( Channel *pChannel , skyWString *pName );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~ProducerThread( void );

	/// <summary>
	/// �������\�b�h
	/// </summary>
	void ThreadMain( void );

private:

	/// <summary>
	/// ���O
	/// </summary>
	sky::lib::WString	m_Name;

	/// <summary>
	/// �`�����l��
	/// </summary>
	Channel				*m_pChannel;

};