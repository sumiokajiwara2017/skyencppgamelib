#pragma once

namespace sky { namespace lib { namespace memory {

/// <summary>
/// �X�^�b�N�A���P�[�^�[
/// ��{�m�ۂ����̈��ςݏグ�Ă����`�������A�X�^�b�N�̓r���̗̈���폜�錾�ł���B
/// �폜�錾���ꂽ�̈�͍폜���X�g�ɓo�^����A��ʂ̗̈悪�|�b�v���ꂽ���Ɏ��ۂɎg�p�\�ȗ̈�Ƃ��ĊJ�������B
/// ����������������
/// �E�X�^�b�N���X�g���疢�g�p�̃�������{���Ə������ׂ���������̂ō폜���X�g���g�p�B
/// �E�폜���X�g����̎擾�͐��`�����Ȃ̂ŉ��ǂ̗]�n������B
/// </summary>
class CStackAllocator : public IAllocator
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CStackAllocator();

	/// <summary>
	/// �w��̗̈���Ǘ�������
	/// </summary>
	inline void AttachMemory( void *pBuffer , u32 bufferSize , u32 blockSize = 32 , u32 backBlockSize = 32 , eAllocType eType = eAllocType_AllocatorClass_New );

	/// <summary>
	/// �S�̃������m��
	/// </summary>
	inline void CreateMemory( u32 bufferSize , u32 blockSize = 32 , u32 backBlockSize = 32 , eAllocType eType = eAllocType_AllocatorClass_New );

	/// <summary>
	/// �S�̃������J��
	/// </summary>
	inline void DeleteMemory();

	/// <summary>
	/// �������m��
	/// </summary>
	inline void *Malloc( u32 size , const skyMString *pFname , s32 line , eAllocSizeType eSize = eAllocSizeType_Block );

	/// <summary>
	/// ����������m��
	/// </summary>
	inline void *BackMalloc( u32 size , const skyMString *pFname , s32 line , eAllocSizeType eSize = eAllocSizeType_Block );

	/// <summary>
	/// �������Ċm��
	/// </summary>
	inline void *Realloc( void *pData , u32 size , const skyMString *pFname , s32 line );

	/// <summary>
	/// �s�v�Ȓl������������폜
	/// �����폜���ꂽ�̈悪�X�^�b�N�̃g�b�v����Ȃ������ꍇ�A�폜���X�g�ɓo�^���A�폜�ς݂̃t���O�𗧂ĂĂ���
	/// �����폜���ꂽ�̈悪�X�^�b�N�̃g�b�v�������ꍇ�A�����Ǝ����̒����̍폜�ς݃t���O�̂������̈�������i���ۂ̓g�b�v�̈ʒu�ϐ����}�C�i�X���邾���j
	/// </summary>
	inline void Delete( void *pData );

	/// <summary>
	/// �X�^�b�N�̃N���A
	/// </summary>
	inline void Clear();

	/// <summary>
	/// �A���P�[�g��񂩂�A���P�[�g����؂蕪����
	/// </summary>
	inline void MargeMemoryArea( CAllocatorDataHeader *pMemory ); 

	/// <summary>
	/// ���g�p�̃A���P�[�g�A���̈���}�[�W����i�g���ǂ���͐T�d�Ɂj
	/// </summary>
	inline CAllocatorDataHeader *MallocFromMallocData( CAllocatorDataHeader *pMemory , u32 Size );

	/// <summary>
	/// �S�̃T�C�Y�̎擾
	/// </summary>
	inline u32 GetAllSize() const;

	/// <summary>
	/// �g�p�\�̈�̎擾
	/// </summary>
	inline u32 GetUsefulSize() const;

	/// <summary>
	/// �X�^�b�N�̎g�p���̈�̎擾(�w�b�_�[�A�t�b�^�[�̃T�C�Y�������̂Ŋm�ۂ����l���������Ȃ�j
	/// </summary>
	inline u32 GetUsedStackSize() const;
	inline u32 GetFrontUsedStackSize() const;
	inline u32 GetBackUsedStackSize() const;

	/// <summary>
	/// �g�p���̈�̎擾(�X�^�b�N�̎g�p���̈悩��폜���X�g�̒l���������l�j
	/// </summary>
	inline u32 GetUsedSize() const;
	inline u32 GetFrontUsedSize() const;
	inline u32 GetBackUsedSize() const;

	/// <summary>
	/// �������A���b�N���̎擾
	/// </summary>
	inline u32 GetMallocNum() const;

	/// <summary>
	/// �폜���X�g�̃A�C�e�����i���̒l�������ƒf�Љ����Ă���Ƃ�����j
	/// </summary>
	inline u32 GetDeleteListItemNum() const;

	/// <summary>
	/// �������`�F�b�N
	/// </summary>
	inline void AssertCheckMemory( void *pData );

	/// <summary>
	/// �w��̃A�h���X���A���P�[�^�[�Ǘ�����������true��Ԃ�
	/// </summary>
	inline skyBool CheckAdr( void *pAdr ) const;

//------------�C���^�[�t�F�[�X�̎����i�s�v�ȃp�����[�^�[�͎g��Ȃ��Œ��p����j--------------

	inline void *Malloc( u32 size ,  s32 groundHndl , s32 arenaHndl , eAllocDirectionType eAreaType , const skyMString *pFname , s32 line );
	inline void *Realloc( void *pData , u32 size , s32 groundHndl , s32 arenaHndl , eAllocDirectionType eAreaType , const skyMString *pFname , s32 line );
	inline u32 GetAllocatorId();

	/// <summary>
	/// ���������̏o��
	/// </summary>
	inline void PrintMemory();

	/// <summary>
	/// �������j��̃`�F�b�N
	/// </summary>
	inline void CheckMemBreak();

private:

	/// <summary>
	/// �X�^�b�N�擪�ʒu
	/// </summary>
	u32  m_BlockSize;

	/// <summary>
	/// �X�^�b�N�擪�ʒu
	/// </summary>
	u32  m_BackBlockSize;

	/// <summary>
	/// �X�^�b�N�擪�ʒu
	/// </summary>
	u32  m_StackTop;

	/// <summary>
	/// �X�^�b�N�擪�ԍ�
	/// </summary>
	u32  m_StackNo;

	/// <summary>
	/// �폜���X�g�o�^��������
	/// </summary>
	u32  m_DeleteListMemorySize;

	/// <summary>
	/// ����X�^�b�N�擪�ʒu
	/// </summary>
	u32  m_BackStackTop;

	/// <summary>
	/// ����X�^�b�N�擪�ԍ�
	/// </summary>
	u32  m_BackStackNo;

	/// <summary>
	/// ����폜���X�g�o�^��������
	/// </summary>
	u32  m_BackDeleteListMemorySize;

	/// <summary>
	/// �m�ۗ̈�T�C�Y
	/// </summary>
	u32 m_BufferSize;

	/// <summary>
	/// �̈�̃|�C���^
	/// </summary>
	void *m_pBuffer;

	/// <summary>
	/// �A���P�[�^�[ID
	/// </summary>
	u32 m_AllocatorId;

	/// <summary>
	/// �A���b�N�^�C�v
	/// </summary>
	eAllocType m_eType;

	/// <summary>
	/// �X�^�b�N���X�g(�o�������X�g�Ȃ̂ł��̌^���̂����[�g�ɂȂ�j
	/// </summary>
	CAllocatorDataHeaderPtrDList  m_StackList;

	/// <summary>
	/// �o�b�N�X�^�b�N���X�g(�o�������X�g�Ȃ̂ł��̌^���̂����[�g�ɂȂ�j
	/// </summary>
	CAllocatorDataHeaderPtrDList  m_BackStackList;

	/// <summary>
	/// �폜���X�g(�o�������X�g�Ȃ̂ł��̌^���̂����[�g�ɂȂ�j
	/// </summary>
	CAllocatorDeleteDataHeaderPtrDList m_DeleteList;

	/// <summary>
	/// ����폜���X�g(�o�������X�g�Ȃ̂ł��̌^���̂����[�g�ɂȂ�j
	/// </summary>
	CAllocatorDeleteDataHeaderPtrDList m_BackDeleteList;

	/// <summary>
	/// 32�o�C�g�u���b�N�폜���X�g(�o�������X�g�Ȃ̂ł��̌^���̂����[�g�ɂȂ�j
	/// </summary>
	CAllocatorDeleteDataHeaderPtrDList m_BlockDeleteList;

	/// <summary>
	/// 32�o�C�g����u���b�N�폜���X�g(�o�������X�g�Ȃ̂ł��̌^���̂����[�g�ɂȂ�j
	/// </summary>
	CAllocatorDeleteDataHeaderPtrDList m_BackBlockDeleteList;
};

} } }

#include "StackAllocator.inl"