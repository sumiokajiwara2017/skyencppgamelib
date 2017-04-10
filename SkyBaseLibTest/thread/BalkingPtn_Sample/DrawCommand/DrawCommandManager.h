#pragma once

/// <summary>
/// �X�V�X���b�h
/// </summary>
class DrawCommandManager
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	DrawCommandManager( void );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~DrawCommandManager( void );

	/// <summary>
	/// �V���O���g���@�\�t��
	/// <summary>
	friend class sky::lib::Singleton< DrawCommandManager >;
	static sky::lib::Singleton< DrawCommandManager > Singleton;

	/// <summary>
	/// �X�C�b�`���X�g
	/// </summary>
	void SwitchList( void );

	/// <summary>
	/// �f�[�^�X�V���X�g�̎擾
	/// </summary>
	DrawCommandPtrList	*GetUpdateList( void );

	/// <summary>
	/// �f�[�^�`�惊�X�g�̎擾
	/// </summary>
	DrawCommandPtrList	*GetDrawList( void );

private:

	/// <summary>
	/// �R�}���h���X�g
	/// </summary>
	DrawCommandPtrList		m_CommandList[ 2 ];

	/// <summary>
	/// �X�V���X�g�C���f�b�N�X
	/// </summary>
	u32						m_UpdateListIdx; // �l��0,1�̂ǂ��炩

};

//�V���O���g���C���X�^���X�A�N�Z�X������
#define DrawCommandManager_ DrawCommandManager::Singleton.GetInstance()