#pragma once

namespace sky { namespace lib {

/// <summary>
/// �������[�v�[���N���X�i�w��̃T�C�Y��p�̍����ȈՃA���P�[�^�[�̂���j
/// </summary>
class CMemoryPoolSize : public base::SkyRefObject
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CMemoryPoolSize( u32 blockSize , u32 poolNum );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CMemoryPoolSize();

	/// <summary>
	/// �쐬�i�����ō쐬�����C���X�^���X�͓��R������Delete�ŏ����Ȃ���_���j
	/// </summary>
	inline void *Create();

	/// <summary>
	/// �폜
	/// </summary>
	inline void Delete( void *pValue );

private:

	/// <summary>
	/// �o�b�t�@�T�C�Y
	/// </summary>
	u32	m_PoolNum;

	/// <summary>
	/// �u���b�N�T�C�Y
	/// </summary>
	u32 m_uiBlockSize;

	/// <summary>
	/// �o�b�t�@�[�P�������X�g
	/// </summary>
	SLinkListSize m_BufferList;
	
	/// <summary>
	/// �C���X�^���X�P�������X�g
	/// </summary>
	SLinkListSize m_InstanceList;

	/// <summary>
	/// �o�b�t�@�m��
	/// </summary>
	inline void CreateBuffer( u32 bufSize );
};

} }

#include "MemoryPoolSize.inl"