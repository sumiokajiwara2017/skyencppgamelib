#pragma once

/// <summary>
/// 
/// </summary>
class WriterThread : public sky::lib::thread::ThreadWin32
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	WriterThread( sky::lib::thread::ReadWriteLock *pRWLock , CommonResource *pComRes , skyWString *pName );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~WriterThread( void );

	/// <summary>
	/// �������\�b�h
	/// </summary>
	void ThreadMain( void );

private:

	/// <summary>
	/// �ǂݍ��ݏ������݃��b�N
	/// </summary>
	sky::lib::thread::ReadWriteLock *m_pRWLock;

	/// <summary>
	/// ���ʃ��\�[�X
	/// </summary>
	CommonResource					*m_pComRes;

	/// <summary>
	/// �X���b�h��
	/// </summary>
	sky::lib::WString				m_Name;

};
