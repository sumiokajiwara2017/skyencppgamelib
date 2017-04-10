#pragma once

namespace sky { namespace lib { namespace memory {

struct CArenaData;

//�e���v���[�gTypedef�S
typedef DLinkList< CArenaData >	ArenaDataPtrDList;	//�o�������X�g��`

/// <summary>
/// �A���[�i�f�[�^
/// </summary>
struct CArenaData : public ArenaDataPtrDList
{

public:

	/// <summary>
	/// �������j�󌟒m�p�h�c
	/// </summary>
	u32							m_CheckBreakId;

	/// <summary>
	/// �A���[�i��
	/// </summary>
	skyWString					m_Name[ ARENA_NAME_MAX ];

	/// <summary>
	/// �T�C�Y
	/// </summary>
	u32							m_Size;

	/// <summary>
	/// �J�n�A�h���X
	/// </summary>
	u32							m_StartAdr;

	/// <summary>
	/// �I���A�h���X
	/// </summary>
	u32							m_EndAdr;

	/// <summary>
	/// �A���[�i�o�b�t�@
	/// </summary>
	void						*m_pBuf;

	/// <summary>
	/// ��������O���E���h�n���h���i�폜���Ɏg�p�j
	/// </summary>
	u32							m_GroundHndl;

	/// <summary>
	/// �X�^�b�N�A���P�[�^�[
	/// </summary>
	CStackAllocator             m_StackAllocator;

	/// <summary>
	/// �A���P�[�^�[ID
	/// </summary>
	u32                         m_BlockSize;

	/// <summary>
	/// �A���b�N�^�C�v
	/// </summary>
	eAllocType                  m_eAllocType;

	/// <summary>
	/// �������m��
	/// </summary>
	inline void* Malloc( u32 size , s32 groundHndl , s32 arenaHndl , const skyMString *pFname , u32 line );

	/// <summary>
	/// �������Ċm��
	/// </summary>
	inline void* Realloc( void *ptr , u32 size , s32 groundHndl , s32 arenaHndl , const skyMString *pFname , u32 line );

	/// <summary>
	/// �폜
	/// </summary>
	inline void	 Delete( CAllocatorDataHeader *pData );

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

#include "ArenaData.inl"