#pragma once

/// <summary>
/// 
/// </summary>
class CommonResource
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CommonResource( void );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CommonResource( void );

	/// <summary>
	/// �f�[�^�ǂݍ���
	/// </summary>
	const skyWString *Read( void );

	/// <summary>
	/// �f�[�^��������
	/// </summary>
	void Write( skyWString *pData );

private:

	/// <summary>
	/// �f�[�^
	/// </summary>
	sky::lib::WString	m_Data;

};
