#pragma once

/// <summary>
/// memory�@�\���ʒ萔
/// </summary>
namespace sky { namespace lib { namespace memory {

/// <summary>
/// �^��`
/// </summary>
static const u32            DEFAULT_ALIGN               = 16;
static const u32			ALLOCATE_DATA_ID			= MK_4BYTE_ID( 'M' , 'A' , 'L' , 'C' );				//�������j�󌟒m�p�h�c
static const u32			ARENA_NAME_MAX				= 64;												//�A���[�i���ő吔
static const u32			MEMORY_INIT_VALUE			= MK_4BYTE_ID( 'I' , 'N' , 'I' , 'T' );				//�����������l
static const u32			START_GROUND_SIZE			= 1024 * 1024 * 72;									//�X�^�[�g�̃O���E���h�T�C�Y�Bmain�֐����Ă΂��O�ɂ����ɗ��Ă��܂��̂ŃX�^�[�g�T�C�Y�̓p�����[�^�[�ł͓n���Ȃ��B�Ȃ̂Œ萔�B���ۂ͂��̃T�C�Y���킸���ɑ������V�X�e���A���P�[�^�[����m�ۂ���B�w�b�_�[�isizeof ( CArenaData )�j���ƃt�b�^�[���isizeof( CAllocatorDataFooter )�j��������B
static const u32			START_ARENA_SIZE			= START_GROUND_SIZE;                                //�X�^�[�g�̃A���[�i�T�C�Y�Bmain�֐����Ă΂��O�ɂ����ɗ��Ă��܂��̂ŃX�^�[�g�T�C�Y�̓p�����[�^�[�ł͓n���Ȃ��B�Ȃ̂Œ萔�B���ۂɃG���A�Ƃ��Ďg�p�ł���l�͂��̒l��菭�����Ȃ��B�w�b�_�[�isizeof ( CArenaData )�j���ƃt�b�^�[���isizeof( CAllocatorDataFooter )�j�������B
static const u32            START_BLOCK_SIZE            = 32;
static const skyWString		*START_ARENA_NAME			= L"Start_Arena";									//�X�^�[�g�̃A���[�i���Bmain�֐����Ă΂��O�ɂ����ɗ��Ă��܂��̂ŃX�^�[�g�T�C�Y�̓p�����[�^�[�ł͓n���Ȃ��B�Ȃ̂Œ萔�B
static const skyMString		*DEFAULT_MALLOC_FILE_NAME	= "Default_Malloc_FileName";						//�������m�ۂ̃p�X�̃f�t�H���g�l

static const u32            FAST_GROUND_ID              = 0;
static const u32            FAST_ARENA_ID               = 0;
static const u32            GROUND_DATA_MIN_SIZE        = 128 * 32;
static const u32            ARENA_DATA_MIN_SIZE         = 128 * 16;
static const u32            SYSTEM_ALLOCATOR_ID         = UINT_MAX;
static const u32            DEBUG_FILE_NAME_LENGHT      = 32;

/// <summary>
/// �A���b�N�^�C�v�i���R�����Ȃ�ɂ����l�ɂ��Ă݂��j
/// </summary>
enum eAllocType
{
	eAllocType_SkyNew             = 51515151   , //�I�[�o�[���C�h����new���Z�q�is_CAllocator�ɂ��A���b�N�j
	eAllocType_Native_New         = 52525252  , //���ɂ���̊�{new
	eAllocType_AllocatorClass_New = 53535353 , //����A���P�[�^�[�ɂ��A���b�N
};

/// <summary>
/// �A���b�N�����^�C�v�i���R�����Ȃ�ɂ����l�ɂ��Ă݂��j
/// </summary>
enum eAllocDirectionType
{
	eAllocDirectionType_Flont     = 71717171   , //�O����������̃A���b�N
	eAllocDirectionType_Back      = 72727272 , //�����������̃A���b�N
	eAllocDirectionType_Automatic = 73737373 , //�����������̃A���b�N
};

/// <summary>
/// �A���b�N�T�C�Y�^�C�v�i���R�����Ȃ�ɂ����l�ɂ��Ă݂��j
/// </summary>
enum eAllocSizeType
{
	eAllocSizeType_Size  = 31313131 , //�T�C�Y�Ŏ擾�i�ʏ�j
	eAllocSizeType_Block = 32323232 , //�Œ�u���b�N�Ŏ擾
};

/// <summary>
/// �A���P�[�^�[�C���^�t�F�[�X
/// </summary>
class IAllocator
{

public:

	/// <summary>
	/// �������j�󌟒m�p�h�c
	/// </summary>
	u32  m_CheckBreakId;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	IAllocator() : m_CheckBreakId( ALLOCATE_DATA_ID ){}

	/// <summary>
	/// �A���P�[�^�[ID�擾
	/// </summary>
	virtual u32 GetAllocatorId() = 0;

	/// <summary>
	/// �������m��
	/// </summary>
	virtual void *Malloc( u32 size ,  s32 groundHndl , s32 arenaHndl , eAllocDirectionType eAreaType = eAllocDirectionType_Automatic , const skyMString *pFname = "none" , s32 line = -1 ) = 0;

	/// <summary>
	/// �������Ċm��
	/// </summary>
	virtual void *Realloc( void *pData , u32 size , s32 groundHndl , s32 arenaHndl , eAllocDirectionType eAreaType = eAllocDirectionType_Automatic , const skyMString *pFname = "none" , s32 line = -1 ) = 0;

	/// <summary>
	/// �s�v�Ȓl������������폜
	/// </summary>
	virtual void Delete( void *pData ) = 0;
};

/// <summary>
/// �������m�ۏ��(�x�[�X�j
/// </summary>
struct SkyAllocatorInfo_
{
	IAllocator*         Allocator; //������skyNull�ȊO���Ƃ��̃A���P�[�^�[���g�p����
	s32                 m_GroundId;
	s32                 m_ArenaId;
	eAllocDirectionType m_eDirType;

#ifdef SW_SKYLIB_DEFAULT_NEW_OVERWRIDE
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SkyAllocatorInfo_( s32 groundId = FAST_GROUND_ID , s32 arenaId = FAST_ARENA_ID , eAllocDirectionType eAreaType = eAllocDirectionType_Automatic , IAllocator *allocator = skyNull )
	{
		m_GroundId   = groundId;
		m_ArenaId    = arenaId;
		m_eDirType   = eAreaType;
		Allocator    = allocator;
	}
#endif
};

extern SkyAllocatorInfo_ g_BaseAllocatorInfo;

/// <summary>
/// ��{�A���P�[�^�[����ݒ肷��
/// </summary>
void SetBaseAllocatorInfo( const SkyAllocatorInfo_ &info );

/// <summary>
/// ��{�A���P�[�^�[�����擾����
/// </summary>
const SkyAllocatorInfo_ &GetBaseAllocatorInfo();

/// <summary>
/// ��{�A���P�[�^�[��������������
/// </summary>
void ClearBaseAllocatorInfo();

} } }

#ifdef SW_SKYLIB_DEFAULT_NEW_OVERWRIDE

void *SkyNewMalloc_( u32 size , const skyMString *pFname = sky::lib::memory::DEFAULT_MALLOC_FILE_NAME , s32 line = 0 , sky::lib::memory::eAllocType eType = sky::lib::memory::eAllocType_SkyNew , s32 groundHndl = sky::lib::memory::FAST_GROUND_ID , s32 arenaHndl = 0 , sky::lib::memory::eAllocDirectionType eAreaType = sky::lib::memory::eAllocDirectionType_Automatic );
void *SkyNewRealloc_( void* pData , u32 size , const skyMString *pFname = sky::lib::memory::DEFAULT_MALLOC_FILE_NAME , s32 line = 0 , sky::lib::memory::eAllocType eType = sky::lib::memory::eAllocType_SkyNew , s32 groundHndl = sky::lib::memory::FAST_GROUND_ID , s32 arenaHndl = 0 , sky::lib::memory::eAllocDirectionType eAreaType = sky::lib::memory::eAllocDirectionType_Automatic );
void SkyNewDelete_( void* pData , sky::lib::memory::eAllocType eAllocType );

/// <summary>
/// �O���E���h�̍쐬
/// </summary>
u32 CreateMemoryGround( u32 size );

/// <summary>
/// �O���E���h�̊J��
/// </summary>
void DeleteMemoryGround( u32 groundHndl );

/// <summary>
/// �A���[�i�̍쐬
/// </summary>
u32 CreateMemoryArena( u32 groundHndl , u32 size , const skyString *pArenaName );

/// <summary>
/// �A���[�i�̍쐬
/// </summary>
void DeleteMemoryArena( u32 groundHndl , u32 arenaHndl );

/// <summary>
/// new���Z�q�̏㏑��
/// </summary>
inline void* operator new( size_t size )
{
	return SkyNewMalloc_( size );
}

/// <summary>
/// new[]���Z�q�̏㏑��
/// </summary>
 inline void* operator new[]( size_t size )
{
	return SkyNewMalloc_( size );
}

/// <summary>
/// new���Z�q�̏㏑��
/// </summary>
inline void* operator new( size_t size , skyMString *pFname , s32 line , sky::lib::memory::eAllocType eType , s32 groundHndl , s32 arenaHndl, sky::lib::memory::eAllocDirectionType eAreaType )
{
	return SkyNewMalloc_( size , pFname , line , eType , groundHndl , arenaHndl, eAreaType );
}

/// <summary>
/// new[]���Z�q�̏㏑��
/// </summary>
inline void* operator new[]( size_t size , skyMString *pFname , s32 line , sky::lib::memory::eAllocType eType , s32 groundHndl , s32 arenaHndl, sky::lib::memory::eAllocDirectionType eAreaType )
{
	return SkyNewMalloc_( size , pFname , line , eType , groundHndl , arenaHndl, eAreaType );
}

/// <summary>
/// delete���Z�q�̏㏑��
/// </summary>
inline void	operator delete( void *ptr )
{
	SkyNewDelete_( ptr , sky::lib::memory::eAllocType_SkyNew );
}

/// <summary>
/// delete[]���Z�q�̏㏑��
/// </summary>
inline void	operator delete[]( void *ptr )
{
	SkyNewDelete_( ptr , sky::lib::memory::eAllocType_SkyNew );
}

/// <summary>
/// delete���Z�q�̏㏑��
/// </summary>
inline void operator delete( void * pData , skyMString *pFname , s32 line , sky::lib::memory::eAllocType eType , s32 groundHndl , s32 arenaHndl, sky::lib::memory::eAllocDirectionType eAreaType )
{
	pFname;
	line;
	groundHndl;
	arenaHndl;
	eAreaType;
	SkyNewDelete_( pData , eType );
}

/// <summary>
/// delete���Z�q�̏㏑��
/// </summary>
inline void operator delete[]( void* pData , skyMString *pFname , s32 line , sky::lib::memory::eAllocType eType , s32 groundHndl , s32 arenaHndl, sky::lib::memory::eAllocDirectionType eAreaType )
{
	pFname;
	line;
	groundHndl;
	arenaHndl;
	eAreaType;
	SkyNewDelete_( pData , eType );
}

/// <summary>
/// new���Z�q�̍Ē�`
/// </summary>
#define NEW						new(__FILE__ , __LINE__ , sky::lib::memory::eAllocType_SkyNew , sky::lib::memory::FAST_GROUND_ID , sky::lib::memory::FAST_ARENA_ID , sky::lib::memory::eAllocDirectionType_Automatic )
#define NEW_ARRAY				NEW //���܂�Ӗ��͖������E�E
#define NEW_NATIVE				new(__FILE__ , __LINE__ , sky::lib::memory::eAllocType_Native_New , sky::lib::memory::FAST_GROUND_ID , sky::lib::memory::FAST_ARENA_ID , sky::lib::memory::eAllocDirectionType_Flont )	//�ʏ��New���ĂԁB�ǂ����Ă�Allocator���O�ɐ錾���Ȃ��Ƃ����Ȃ��N���X�̒��ł̃������m�ۂɎg���B
#define NEW_()                  new(__FILE__ , __LINE__ , sky::lib::memory::eAllocType_SkyNew , sky::lib::memory::g_BaseAllocatorInfo.m_GroundId , sky::lib::memory::g_BaseAllocatorInfo.m_ArenaId , sky::lib::memory::g_BaseAllocatorInfo.m_eDirType )   
#define NEW_ARRAY_()            NEW_()

/// <summary>
/// delete���Z�q�̍Ē�`
/// </summary>
#define DEL						delete
#define DEL_ARRAY				delete[]
#define DEL_NATIVE( t , p )		p->~t(); SkyNewDelete_( p , sky::lib::memory::eAllocType_Native_New )   // �d�g�ݏ�f�X�g���N�^���Ă΂�Ȃ��̂Ŗ������Ă�ł���
#define SAFE_RELEASE( p )		{ if ( p )					{ ( p )->Release();	( p ) = skyNull; } }	// Release()�֐����Ă�
#define SAFE_DELETE( p )		{ if (skyNull != ( p ) )	{ DEL	( p );	( p ) = skyNull; } }		// �|�C���^���J������
#define SAFE_DELETE_ARR( p )	{ if (skyNull != ( p ) )	{ DEL[] ( p );	( p ) = skyNull; } }		// �z��|�C���^���폜����

/// <summary>
/// �������m�ۊ֐��Ē�`
/// </summary>
#define SkyMalloc( size )			SkyNewMalloc_( size , __FILE__ , __LINE__ , sky::lib::memory::eAllocType_SkyNew , sky::lib::memory::FAST_GROUND_ID , sky::lib::memory::FAST_ARENA_ID, sky::lib::memory::eAllocDirectionType_Automatic )
#define SkyRealloc( ptr , size )	SkyNewRealloc_( ptr , size , __FILE__ , __LINE__ , sky::lib::memory::eAllocType_SkyNew , sky::lib::memory::FAST_GROUND_ID , sky::lib::memory::FAST_ARENA_ID, sky::lib::memory::eAllocDirectionType_Automatic )
#define SkyMalloc_( size )          SkyNewMalloc_ ( size , __FILE__ , __LINE__ , sky::lib::memory::eAllocType_SkyNew , sky::lib::memory::g_BaseAllocatorInfo.m_GroundId , sky::lib::memory::g_BaseAllocatorInfo.m_ArenaId , sky::lib::memory::g_BaseAllocatorInfo.m_eDirType )
#define SkyRealloc_( ptr , size )   SkyNewRealloc_( ptr  , size , __FILE__ , __LINE__ , sky::lib::memory::eAllocType_SkyNew , sky::lib::memory::g_BaseAllocatorInfo.m_GroundId , sky::lib::memory::g_BaseAllocatorInfo.m_ArenaId , sky::lib::memory::g_BaseAllocatorInfo.m_eDirType )

/// <summary>
/// �������J���֐��Ē�`
/// </summary>
#define SkyFree( ptr )			SkyNewDelete_( ptr , sky::lib::memory::eAllocType_SkyNew )

/// <summary>
/// �m�ې����o�͂���
/// </summary>
void PrintMemory();

/// <summary>
/// �������j��`�F�b�N�i�j�󂳂�Ă����Ȃ������Assert���o�܂��j
/// </summary>
void CheckMemBreak();

#ifdef SW_SKYLIB_DEBUG_ON
	#define PRINT_MEMORY( isFileOut )	PrintMemory( isFileOut )
#else
	#define PRINT_MEMORY( isFileOut )	( ()0 )
#endif

#ifdef SW_SKYLIB_DEBUG_ON
	#define CHECK_MEM_BREAK()			CheckMemBreak()
#else
	#define CHECK_MEM_BREAK()				( ()0 )
#endif

//�A���P�[�^�[������΃A���P�[�^�[���g�p���A�Ȃ���΍őP��new���g�p����
#define NEW__( type , constractor ) NEW_() constractor
#define NEW_ARRAY__( type , num )   NEW_ARRAY_() type[ num ]
#define SkyMalloc__( size )         SkyMalloc_ ( size )
#define SkyRealloc__( ptr , size )  SkyRealloc_( ptr , size )
#else

/// <summary>
/// new���Z�q�̍Ē�`
/// </summary>
#define NEW						new
#define NEW_ARRAY				new
#define NEW_NATIVE				new
#define NEW_()                  new
#define NEW_ARRAY_()            new

/// <summary>
/// delete���Z�q�̍Ē�`
/// </summary>
#define DEL						delete
#define DEL_ARRAY				delete[]
#define DEL_NATIVE( t , p )		delete p
#define SAFE_RELEASE( p )		{ if ( p )					{ ( p )->Release();	( p ) = skyNull; } }	// Release()�֐����Ă�
#define SAFE_DELETE( p )		{ if (skyNull != ( p ) )	{ DEL	( p );	( p ) = skyNull; } }		// �|�C���^���J������
#define SAFE_DELETE_ARR( p )	{ if (skyNull != ( p ) )	{ DEL[] ( p );	( p ) = skyNull; } }		// �z��|�C���^���폜����

/// <summary>
/// �������m�ۊ֐��Ē�`
/// </summary>
#define SkyMalloc( size )			MALLOC__( size , __FILE__ , __LINE__ )
#define SkyRealloc( ptr , size )	REALLOC__( ptr , size , __FILE__ , __LINE__ )
#define SkyMalloc_( size )          MALLOC__( size , __FILE__ , __LINE__ )
#define SkyRealloc_( ptr , size )   REALLOC__( ptr , size , __FILE__ , __LINE__ )

/// <summary>
/// �������J���֐��Ē�`
/// </summary>
#define SkyFree( ptr )			    FREE__( ptr )

/// <summary>
/// �m�ې����o�͂���
/// </summary>
inline void PrintMemory(){}

/// <summary>
/// �������j��`�F�b�N�i�j�󂳂�Ă����Ȃ������Assert���o�܂��j
/// </summary>
inline void CheckMemBreak(){}

//#define��OFF�ɂȂ��Ă���^�̒�`
namespace sky { namespace lib { namespace memory {
struct CGroundData;
struct CArenaData;
} } }

#define PRINT_MEMORY( isFileOut )	( ()0 )
#define CHECK_MEM_BREAK()			( ()0 )

#define NEW__( type , constractor ) NEW constractor
#define NEW_ARRAY__( type , num )   NEW_ARRAY type[ num ]
#define SkyMalloc__( size )         SkyMalloc(size)
#define SkyRealloc__( ptr , size )  SkyRealloc(ptr,size)

#endif
