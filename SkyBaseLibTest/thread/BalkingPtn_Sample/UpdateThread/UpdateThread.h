#pragma once

/// <summary>
/// �X�V�X���b�h
/// </summary>
class UpdateThread : public sky::lib::thread::ThreadWin32
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	UpdateThread( DrawExecEventPtr spEvent );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~UpdateThread( void );

	/// <summary>
	/// �X���b�h���s�֐�
	/// </summary>
	virtual void ThreadMain( void );

private:

	/// <summary>
	/// �C�x���g
	/// </summary>
	DrawExecEventPtr m_spEvent;

};
