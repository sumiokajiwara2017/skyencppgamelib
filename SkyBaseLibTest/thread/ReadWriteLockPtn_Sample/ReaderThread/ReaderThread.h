#pragma once

/// <summary>
/// �ǂݍ��ݍs�׃X���b�h
/// </summary>
class ReaderThread : public sky::lib::thread::ThreadWin32
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ReaderThread( sky::lib::thread::ReadWriteLock *pRWLock , CommonResource *pComRes , skyWString *pName );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~ReaderThread( void );

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
