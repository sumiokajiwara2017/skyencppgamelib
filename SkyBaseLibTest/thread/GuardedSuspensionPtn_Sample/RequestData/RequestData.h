#pragma once

/// <summary>
/// �L���[�f�[�^
/// </summary>
class RequestData
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	RequestData( void );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~RequestData( void );

	/// <summary>
	/// ���N�G�X�g�f�[�^
	/// </summary>
	sky::lib::WString		m_Data;

};

typedef sky::lib::Fifo< RequestData* >			RequestDataPtrFifo;	//Fifo��`
