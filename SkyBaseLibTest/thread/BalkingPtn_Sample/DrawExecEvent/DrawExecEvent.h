#pragma once

/// <summary>
/// �X�V�X���b�h
/// </summary>
class DrawExecEvent
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	DrawExecEvent( void );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~DrawExecEvent( void );

	/// <summary>
	/// �t���O�n�m
	/// </summary>
	void On( void );

	/// <summary>
	/// �t���O�n�e�e
	/// </summary>
	void Off( void );

	/// <summary>
	/// �t���O��Ԋm�F
	/// </summary>
	skyBool Is( void );

	/// <summary>
	/// �I��
	/// </summary>
	void End( void);
	skyBool IsEnd( void);

private:

	/// <summary>
	/// �Ǘ��t���O
	/// </summary>
	skyBool		m_IsDrawExecGo;

	/// <summary>
	/// �I���t���O
	/// </summary>
	skyBool		m_IsEnd;

};

typedef sky::lib::SmartPointerMT< DrawExecEvent >			DrawExecEventPtr;	//SmartPointer�^
