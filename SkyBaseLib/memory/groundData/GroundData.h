#pragma once

namespace sky { namespace lib { namespace memory {

struct CGroundData;

//�e���v���[�gTypedef�S
typedef DLinkList< CGroundData >					GroundDataPtrDList;	//�o�������X�g��`

/// <summary>
/// �O���E���h�f�[�^
/// </summary>
struct CGroundData : public GroundDataPtrDList
{

public:

	/// <summary>
	/// �������j�󌟒m�p�h�c
	/// </summary>
	u32						m_CheckBreakId;

	/// <summary>
	/// �T�C�Y
	/// </summary>
	u32						m_Size;

	/// <summary>
	/// �J�n�A�h���X
	/// </summary>
	u32						m_StartAdr;

	/// <summary>
	/// �I���A�h���X
	/// </summary>
	u32						m_EndAdr;

	/// <summary>
	/// �t�����g�g�p�ς݃T�C�Y
	/// </summary>
	u32						m_UsedSize;

	/// <summary>
	/// �O���E���h�o�b�t�@
	/// </summary>
	void                   *m_pBuf;

	/// <summary>
	/// �폜���X�g�i�[�T�C�Y
	/// </summary>
	u32                     m_DeleteListAddSize;

	/// <summary>
	/// �A���[�i�f�[�^���X�g(�o�������X�g�Ȃ̂ł��̌^���̂����[�g�ɂȂ�j
	/// </summary>
	ArenaDataPtrDList      m_ArenaList;

	/// <summary>
	/// �X�^�b�N�A���P�[�^�[
	/// </summary>
	CStackAllocator         m_StackAllocator;

	/// <summary>
	/// �A���[�i�̎���ID
	/// </summary>
	s32                     m_ArenaUniqueid;

	/// <summary>
	/// �A���P�[�^�[ID
	/// </summary>
	u32                     m_AllocatorId;

	/// <summary>
	/// �A���P�[�^�[ID
	/// </summary>
	u32                     m_BlockSize;

	/// <summary>
	/// �A���b�N�^�C�v
	/// </summary>
	eAllocType             m_eAllocType;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CGroundData();

	/// <summary>
	/// �A���[�i�̍쐬
	/// </summary>
	inline u32	CreateArena( u32 size , const skyWString *pArenaName );

	/// <summary>
	/// �A���[�i�̍폜
	/// </summary>
	inline void DeleteArena( u32 arenaHndl );

	/// <summary>
	/// �������󋵂��v�����g����
	/// </summary>
	inline void PrintMemory();

	/// <summary>
	/// �������j��̃`�F�b�N
	/// </summary>
	inline void CheckMemBreak();
};

} } }

#include "GroundData.inl"