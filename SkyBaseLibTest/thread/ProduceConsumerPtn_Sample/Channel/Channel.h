#pragma once

/// <summary>
/// �f�[�^�̎󂯓n����S������N���X
/// </summary>
/*
ProducerThread�����PutData(�f�[�^��u���j�����B
�u���ꂽ�f�[�^���Aqueue�\���ŊǗ����Ă��邩�ǂ����͎����ɂ��B
ConsumerThread�����GetData(�f�[�^�����o���j�����B
�����AConsumerThread����GetData���ꂽ���Ƀf�[�^�����݂��Ȃ���΁A
ProducerThread����PutData�����̂�҂�
*/

/// <summary>
/// �萔
/// <summary>
namespace channel_const
{
	static const u32 BUFFER_LENGTH = 3;
}
using namespace channel_const;

class Channel
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Channel( void );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Channel( void );

	/// <summary>
	/// �f�[�^�擾
	/// </summary>
	Data *GetData( void );

	/// <summary>
	/// �f�[�^�i�[
	/// </summary>
	void PutData( Data* pData );

private:

	/// <summary>
	/// ���N�G�X�g�f�[�^Fifo
	/// </summary>
	DataPtrFifo	m_DataFifo;

	/// <summary>
	/// ���b�N�I�u�W�F�N�g
	/// </summary>
	sky::lib::thread::ILockObject *m_pLock;
};