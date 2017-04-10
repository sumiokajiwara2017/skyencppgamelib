#pragma once

#ifdef SW_SKYLIB_DEFAULT_NEW_OVERWRIDE

//Windows�̃f�o�b�O�ł������������[�N�̓��ʂȎd�g�݂��쓮����
#if defined( _DEBUG ) && defined( SW_SKYLIB_CRT_MEMORY_LEAK_CHK )
#define CrtLeakCheckInit()    int iDbgFlg;										\
							  iDbgFlg = _CrtSetDbgFlag(0);						\
							  _CrtSetDbgFlag( iDbgFlg | _CRTDBG_LEAK_CHECK_DF);

#define CrtLeakCheck()        _CrtDumpMemoryLeaks();
#else
#define CrtLeakCheckInit() (void)0
#define CrtLeakCheck()     (void)0
#endif

#endif

namespace sky { namespace lib { 

class CMemoryPoolSize;

namespace thread { class CriticalSection; }

namespace memory {

/// <summary>
/// �A���P�[�^�[
/// </summary>
/*
		�{�A���P�[�^�[�ɂ͉��L�̊T�O������
		�E�O���E���h
		�E�A���[�i
		�E�u���b�N
		�E�o�b�N

		���ꂼ��̊֌W�͉��L�̒ʂ�

		�� �O���E���h ��
		+---------------------------------------------------+-----------------------------------------------------+
		|  �u���b�N  |                                      |                                                     |
		+------------+                                      |                                                     |
		|                                                   |                                                     |
		|                                                   |                                                     |
		|                                                   |                                                     |
		|                                                   |                                                     |
		|                                                   |                                                     |
		|               �� �A���[�i�P ��                    |                 ���@�A���[�i�Q �� �@                |
		|                                                   |                                                     |
		|                                                   |                                                     |
		|                                                   |                                                     |
		|                                                   |                                                     |
		|                                                   |                                                     |
		|      +--------------------------------------------+                                                     |
		|      |                                            |                                                     |
		|      |               �o�b�N                       |                                                     |
		|      |                                            |                                                     |
		+---------------------------------------------------+-----------------------------------------------------+

		�����BLOCK_SIZE_MAX ����@BLOCK_SIZE_MIN �܂ł̊Ԃ̃T�C�Y���m�ۂ��悤�Ƃ����ꍇ�A��L�A���P�[�^�[�Ƃ͕ʂɃ~�j�T�C�Y�p�A���P�[�^�[������B

---------------------------------------------------------------------------------------------
    �O���E���h�A�A���[�i�w��̃������m�ە��@�BSkyAllocatorInfo�ɃA���P�[�g�������w�肵�ăA���P�[�g���ɂɂ����n���B

	u32 groundHndl = CreateMemoryGround( 128 * 1024 );
	u32 arenaHndl = CreateMemoryArena( groundHndl , 64 * 1024 , _T( "hoge_arena" ) );

	const memory::SkyAllocatorInfo &info;
	info.m_GroundId = groundHndl;
	info.m_ArenaId = arenaHndl;

	s32 *pHoge = NEW_() s32;
	*pHoge = 1;
	DEL pHoge;

	pHoge = ( s32 * )SkyMalloc_( sizeof( s32 ) , info );
	*pHoge = 2;

	d64 *pHoged = ( d64 * )SkyRealloc_( pHoge , sizeof( d64 ) , info );
	*pHoged = 0.1f;

	SkyFree( pHoged );

	DeleteMemoryGround( groundHndl );

---------------------------------------------------------------------------------------------


*/
class CAllocator : public IAllocator
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CAllocator( u32 blockSize = START_BLOCK_SIZE );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CAllocator();

	/// <summary>
	/// �O���E���h�i�S�g�p�̈�j�̊m��
	/// </summary>
	u32 CreateGround( u32 size );

	/// <summary>
	/// �A���P�[�^�[ID�擾
	/// </summary>
	u32 GetAllocatorId(){ return m_AllocatorId; }

	/// <summary>
	/// �O���E���h�i�S�g�p�̈�j�̊J��
	/// </summary>
	void DeleteGround( u32 groundHndl );

	/// <summary>
	/// �A���[�i�̍쐬�i�A���[�i���O���E���h�̂P���j
	/// </summary>
	u32 CreateArena( u32 groundHndl , u32 size , const skyWString *arenaName = _T( "default_name" ) );

	/// <summary>
	/// �A���[�i�̍쐬�i�A���[�i���O���E���h�̂P���j
	/// </summary>
	void DeleteArena( u32 groundHndl , u32 arenaHndl );

	/// <summary>
	/// �������m��
	/// </summary>
	void *Malloc( u32 size ,  s32 groundHndl , s32 arenaHndl , eAllocDirectionType eAreaType = eAllocDirectionType_Automatic , const skyMString *pFname = "none" , s32 line = -1 );

	/// <summary>
	/// �������Ċm��
	/// </summary>
	void *Realloc( void *pData , u32 size , s32 groundHndl , s32 arenaHndl , eAllocDirectionType eAreaType = eAllocDirectionType_Automatic , const skyMString *pFname = "none" , s32 line = -1 );

	/// <summary>
	/// �s�v�Ȓl������������폜
	/// </summary>
	void Delete( void *pData );

	/// <summary>
	/// �������󋵂��v�����g����
	/// </summary>
	void PrintMemory();

	/// <summary>
	/// �������j��̃`�F�b�N
	/// </summary>
	void CheckMemBreak();

	/// <summary>
	/// �N���e�B�J���Z�N�V�����̎擾
	/// </summary>
    thread::CriticalSection* GetCriticalSection();

	/// <summary>
	/// �A���b�N�^�C�v�̐ݒ�
	/// </summary>
	void SetAllocType( eAllocType eAllocType )
	{
		m_eAllocType = eAllocType;
	}

//---System---

	/// <summary>
	/// �R���X�g���N�^�i����͎g��Ȃ��ł��������j
	/// </summary>
	CAllocator( u32 blockSize , u32 id );

	/// <summary>
	/// �f�t�H���g�̃O���E���h�n���h��
	/// </summary>
	u32					m_DefaultGroundHndl;

	/// <summary>
	/// �f�t�H���g�̃A���[�i�n���h��
	/// </summary>
	u32					m_DefaultArenaHndl;

	/// <summary>
	/// �O�����h�f�[�^�o�������X�g�i�o�������X�g�Ȃ̂Œǉ����ɓ����ŃA���b�N���܂���j
	/// </summary>
	GroundDataPtrDList	m_pGroundDataDList;

private:

	/// <summary>
	/// �A���P�[�^�[ID
	/// </summary>
	u32        m_AllocatorId;

	/// <summary>
	/// �A���b�N�^�C�v
	/// </summary>
	eAllocType m_eAllocType;

	/// <summary>
	/// �A���P�[�^�[ID
	/// </summary>
	u32        m_BlockSize;

	/// <summary>
	/// �N���e�B�J���Z�N�V����
	/// </summary>
	thread::CriticalSection* m_pCs;

	/// <summary>
	/// �������ς݃t���O
	/// </summary>
	skyBool				m_bIsDeposed;

	/// <summary>
	/// �O���E���h�̎���ID
	/// </summary>
	s32                 GroundUniqueid;

};

} } }

#ifdef SW_SKYLIB_DEFAULT_NEW_OVERWRIDE

/// <summary>
/// �w��̃n���h���̃O���E���h�̏����擾
/// </summary>
const u32 GetDefaultMemoryGroundHandl();
const sky::lib::memory::CGroundData *GetMemoryGroundData( s32 groundHndl );

/// <summary>
/// �w��̃n���h���̃A���[�i�̏����擾
/// </summary>
const u32 GetDefaultMemoryArenaHandl();
const sky::lib::memory::CArenaData *GetMemoryArenaData( s32 groundHndl , u32 arenaHndl );

#else

/// <summary>
/// �w��̃n���h���̃O���E���h�̏����擾
/// </summary>
inline const u32 GetDefaultMemoryGroundHandl(){ return 0; }
inline const sky::lib::memory::CGroundData *GetMemoryGroundData( s32 groundHndl )
{ 
	SKY_UNUSED_ARG( groundHndl );
	return ( sky::lib::memory::CGroundData * )0;
}

/// <summary>
/// �w��̃n���h���̃A���[�i�̏����擾
/// </summary>
inline const u32 GetDefaultMemoryArenaHandl(){ return 0; }
inline const sky::lib::memory::CArenaData *GetMemoryArenaData( s32 groundHndl , u32 arenaHndl )
{ 
	SKY_UNUSED_ARG( groundHndl );
	SKY_UNUSED_ARG( arenaHndl );
	return ( sky::lib::memory::CArenaData * )0;
}

#endif