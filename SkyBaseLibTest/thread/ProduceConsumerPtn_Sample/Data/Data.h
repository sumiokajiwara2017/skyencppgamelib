#pragma once

/// <summary>
/// �X���b�h�Ԃł��Ƃ肳���f�[�^�N���X
/// </summary>
class Data
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Data( void );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Data( void );

	/// <summary>
	/// ���N�G�X�g�f�[�^
	/// </summary>
	sky::lib::WString		m_Data;

};

typedef sky::lib::Fifo< Data* >			DataPtrFifo;	//Fifo��`
