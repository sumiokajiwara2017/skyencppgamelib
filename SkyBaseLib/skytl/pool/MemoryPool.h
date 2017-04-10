#pragma once

namespace sky { namespace lib {

/// <summary>
/// �������[�v�[���N���X�i�w��̌^(T)��p�̍����ȈՃA���P�[�^�[�̂���j
/// </summary>
template< class T >
class CMemoryPool : public base::SkyRefObject
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CMemoryPool( u32 poolNum );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CMemoryPool();

	/// <summary>
	/// �쐬�i�����ō쐬�����C���X�^���X�͓��R������Delete�ŏ����Ȃ���_���j
	/// </summary>
	inline T *Create();

	/// <summary>
	/// �폜
	/// </summary>
	inline void Delete( T *pValue );

	/// <summary>
	/// �v�[�����擾
	/// </summary>
	u32 GetPoolNum() const;

private:

	/// <summary>
	/// �v�[����
	/// </summary>
	u32	m_PoolNum;

	/// <summary>
	/// �o�b�t�@�[�P�������X�g
	/// </summary>
	SLinkListSize m_BufferList;
	
	/// <summary>
	/// �C���X�^���X�P�������X�g
	/// </summary>
	SLinkList< T > m_InstanceList;

	/// <summary>
	/// �o�b�t�@�m��
	/// </summary>
	void CreateBuffer( u32 bufSize );
};

} }

#include "MemoryPool.inl"