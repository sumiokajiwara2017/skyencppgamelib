#pragma once

/// <summary>
/// �X�V�X���b�h
/// </summary>
class DrawThread : public sky::lib::thread::ThreadWin32
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	DrawThread( DrawExecEventPtr spEvent );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~DrawThread( void );

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
