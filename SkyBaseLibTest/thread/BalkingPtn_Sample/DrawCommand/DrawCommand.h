#pragma once

/// <summary>
/// �X�V�X���b�h
/// </summary>
class DrawCommand
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	DrawCommand( void );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~DrawCommand( void );

	/// <summary>
	/// ���N�G�X�g�f�[�^
	/// </summary>
	sky::lib::WString		m_Data;

};

typedef sky::lib::List< s32 , DrawCommand * >			DrawCommandPtrList;	//List��`
