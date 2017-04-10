#pragma once

/// <summary>
/// memory機能共通定数
/// </summary>
namespace sky { namespace lib { namespace memory {

/// <summary>
/// 型定義
/// </summary>
static const u32            DEFAULT_ALIGN               = 16;
static const u32			ALLOCATE_DATA_ID			= MK_4BYTE_ID( 'M' , 'A' , 'L' , 'C' );				//メモリ破壊検知用ＩＤ
static const u32			ARENA_NAME_MAX				= 64;												//アリーナ名最大数
static const u32			MEMORY_INIT_VALUE			= MK_4BYTE_ID( 'I' , 'N' , 'I' , 'T' );				//メモリ初期値
static const u32			START_GROUND_SIZE			= 1024 * 1024 * 72;									//スタートのグラウンドサイズ。main関数が呼ばれる前にここに来てしまうのでスタートサイズはパラメーターでは渡せない。なので定数。実際はこのサイズよりわずかに多くをシステムアロケーターから確保する。ヘッダー（sizeof ( CArenaData )）分とフッター分（sizeof( CAllocatorDataFooter )）分増える。
static const u32			START_ARENA_SIZE			= START_GROUND_SIZE;                                //スタートのアリーナサイズ。main関数が呼ばれる前にここに来てしまうのでスタートサイズはパラメーターでは渡せない。なので定数。実際にエリアとして使用できる値はこの値より少し少ない。ヘッダー（sizeof ( CArenaData )）分とフッター分（sizeof( CAllocatorDataFooter )）引かれる。
static const u32            START_BLOCK_SIZE            = 32;
static const skyWString		*START_ARENA_NAME			= L"Start_Arena";									//スタートのアリーナ名。main関数が呼ばれる前にここに来てしまうのでスタートサイズはパラメーターでは渡せない。なので定数。
static const skyMString		*DEFAULT_MALLOC_FILE_NAME	= "Default_Malloc_FileName";						//メモリ確保のパスのデフォルト値

static const u32            FAST_GROUND_ID              = 0;
static const u32            FAST_ARENA_ID               = 0;
static const u32            GROUND_DATA_MIN_SIZE        = 128 * 32;
static const u32            ARENA_DATA_MIN_SIZE         = 128 * 16;
static const u32            SYSTEM_ALLOCATOR_ID         = UINT_MAX;
static const u32            DEBUG_FILE_NAME_LENGHT      = 32;

/// <summary>
/// アロックタイプ（偶然そうなりにくい値にしてみた）
/// </summary>
enum eAllocType
{
	eAllocType_SkyNew             = 51515151   , //オーバーライドしたnew演算子（s_CAllocatorによるアロック）
	eAllocType_Native_New         = 52525252  , //環境にあるの基本new
	eAllocType_AllocatorClass_New = 53535353 , //自作アロケーターによるアロック
};

/// <summary>
/// アロック方向タイプ（偶然そうなりにくい値にしてみた）
/// </summary>
enum eAllocDirectionType
{
	eAllocDirectionType_Flont     = 71717171   , //前方方向からのアロック
	eAllocDirectionType_Back      = 72727272 , //後方方向からのアロック
	eAllocDirectionType_Automatic = 73737373 , //後方方向からのアロック
};

/// <summary>
/// アロックサイズタイプ（偶然そうなりにくい値にしてみた）
/// </summary>
enum eAllocSizeType
{
	eAllocSizeType_Size  = 31313131 , //サイズで取得（通常）
	eAllocSizeType_Block = 32323232 , //固定ブロックで取得
};

/// <summary>
/// アロケーターインタフェース
/// </summary>
class IAllocator
{

public:

	/// <summary>
	/// メモリ破壊検知用ＩＤ
	/// </summary>
	u32  m_CheckBreakId;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IAllocator() : m_CheckBreakId( ALLOCATE_DATA_ID ){}

	/// <summary>
	/// アロケーターID取得
	/// </summary>
	virtual u32 GetAllocatorId() = 0;

	/// <summary>
	/// メモリ確保
	/// </summary>
	virtual void *Malloc( u32 size ,  s32 groundHndl , s32 arenaHndl , eAllocDirectionType eAreaType = eAllocDirectionType_Automatic , const skyMString *pFname = "none" , s32 line = -1 ) = 0;

	/// <summary>
	/// メモリ再確保
	/// </summary>
	virtual void *Realloc( void *pData , u32 size , s32 groundHndl , s32 arenaHndl , eAllocDirectionType eAreaType = eAllocDirectionType_Automatic , const skyMString *pFname = "none" , s32 line = -1 ) = 0;

	/// <summary>
	/// 不要な値をメモリから削除
	/// </summary>
	virtual void Delete( void *pData ) = 0;
};

/// <summary>
/// メモリ確保情報(ベース）
/// </summary>
struct SkyAllocatorInfo_
{
	IAllocator*         Allocator; //ここがskyNull以外だとこのアロケーターを使用する
	s32                 m_GroundId;
	s32                 m_ArenaId;
	eAllocDirectionType m_eDirType;

#ifdef SW_SKYLIB_DEFAULT_NEW_OVERWRIDE
	/// <summary>
	/// コンストラクタ
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
/// 基本アロケーター情報を設定する
/// </summary>
void SetBaseAllocatorInfo( const SkyAllocatorInfo_ &info );

/// <summary>
/// 基本アロケーター情報を取得する
/// </summary>
const SkyAllocatorInfo_ &GetBaseAllocatorInfo();

/// <summary>
/// 基本アロケーター情報を初期化する
/// </summary>
void ClearBaseAllocatorInfo();

} } }

#ifdef SW_SKYLIB_DEFAULT_NEW_OVERWRIDE

void *SkyNewMalloc_( u32 size , const skyMString *pFname = sky::lib::memory::DEFAULT_MALLOC_FILE_NAME , s32 line = 0 , sky::lib::memory::eAllocType eType = sky::lib::memory::eAllocType_SkyNew , s32 groundHndl = sky::lib::memory::FAST_GROUND_ID , s32 arenaHndl = 0 , sky::lib::memory::eAllocDirectionType eAreaType = sky::lib::memory::eAllocDirectionType_Automatic );
void *SkyNewRealloc_( void* pData , u32 size , const skyMString *pFname = sky::lib::memory::DEFAULT_MALLOC_FILE_NAME , s32 line = 0 , sky::lib::memory::eAllocType eType = sky::lib::memory::eAllocType_SkyNew , s32 groundHndl = sky::lib::memory::FAST_GROUND_ID , s32 arenaHndl = 0 , sky::lib::memory::eAllocDirectionType eAreaType = sky::lib::memory::eAllocDirectionType_Automatic );
void SkyNewDelete_( void* pData , sky::lib::memory::eAllocType eAllocType );

/// <summary>
/// グラウンドの作成
/// </summary>
u32 CreateMemoryGround( u32 size );

/// <summary>
/// グラウンドの開放
/// </summary>
void DeleteMemoryGround( u32 groundHndl );

/// <summary>
/// アリーナの作成
/// </summary>
u32 CreateMemoryArena( u32 groundHndl , u32 size , const skyString *pArenaName );

/// <summary>
/// アリーナの作成
/// </summary>
void DeleteMemoryArena( u32 groundHndl , u32 arenaHndl );

/// <summary>
/// new演算子の上書き
/// </summary>
inline void* operator new( size_t size )
{
	return SkyNewMalloc_( size );
}

/// <summary>
/// new[]演算子の上書き
/// </summary>
 inline void* operator new[]( size_t size )
{
	return SkyNewMalloc_( size );
}

/// <summary>
/// new演算子の上書き
/// </summary>
inline void* operator new( size_t size , skyMString *pFname , s32 line , sky::lib::memory::eAllocType eType , s32 groundHndl , s32 arenaHndl, sky::lib::memory::eAllocDirectionType eAreaType )
{
	return SkyNewMalloc_( size , pFname , line , eType , groundHndl , arenaHndl, eAreaType );
}

/// <summary>
/// new[]演算子の上書き
/// </summary>
inline void* operator new[]( size_t size , skyMString *pFname , s32 line , sky::lib::memory::eAllocType eType , s32 groundHndl , s32 arenaHndl, sky::lib::memory::eAllocDirectionType eAreaType )
{
	return SkyNewMalloc_( size , pFname , line , eType , groundHndl , arenaHndl, eAreaType );
}

/// <summary>
/// delete演算子の上書き
/// </summary>
inline void	operator delete( void *ptr )
{
	SkyNewDelete_( ptr , sky::lib::memory::eAllocType_SkyNew );
}

/// <summary>
/// delete[]演算子の上書き
/// </summary>
inline void	operator delete[]( void *ptr )
{
	SkyNewDelete_( ptr , sky::lib::memory::eAllocType_SkyNew );
}

/// <summary>
/// delete演算子の上書き
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
/// delete演算子の上書き
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
/// new演算子の再定義
/// </summary>
#define NEW						new(__FILE__ , __LINE__ , sky::lib::memory::eAllocType_SkyNew , sky::lib::memory::FAST_GROUND_ID , sky::lib::memory::FAST_ARENA_ID , sky::lib::memory::eAllocDirectionType_Automatic )
#define NEW_ARRAY				NEW //あまり意味は無いが・・
#define NEW_NATIVE				new(__FILE__ , __LINE__ , sky::lib::memory::eAllocType_Native_New , sky::lib::memory::FAST_GROUND_ID , sky::lib::memory::FAST_ARENA_ID , sky::lib::memory::eAllocDirectionType_Flont )	//通常のNewを呼ぶ。どうしてもAllocatorより前に宣言しないといけないクラスの中でのメモリ確保に使う。
#define NEW_()                  new(__FILE__ , __LINE__ , sky::lib::memory::eAllocType_SkyNew , sky::lib::memory::g_BaseAllocatorInfo.m_GroundId , sky::lib::memory::g_BaseAllocatorInfo.m_ArenaId , sky::lib::memory::g_BaseAllocatorInfo.m_eDirType )   
#define NEW_ARRAY_()            NEW_()

/// <summary>
/// delete演算子の再定義
/// </summary>
#define DEL						delete
#define DEL_ARRAY				delete[]
#define DEL_NATIVE( t , p )		p->~t(); SkyNewDelete_( p , sky::lib::memory::eAllocType_Native_New )   // 仕組み上デストラクタが呼ばれないので無理やり呼んでいる
#define SAFE_RELEASE( p )		{ if ( p )					{ ( p )->Release();	( p ) = skyNull; } }	// Release()関数を呼ぶ
#define SAFE_DELETE( p )		{ if (skyNull != ( p ) )	{ DEL	( p );	( p ) = skyNull; } }		// ポインタを開放する
#define SAFE_DELETE_ARR( p )	{ if (skyNull != ( p ) )	{ DEL[] ( p );	( p ) = skyNull; } }		// 配列ポインタを削除する

/// <summary>
/// メモリ確保関数再定義
/// </summary>
#define SkyMalloc( size )			SkyNewMalloc_( size , __FILE__ , __LINE__ , sky::lib::memory::eAllocType_SkyNew , sky::lib::memory::FAST_GROUND_ID , sky::lib::memory::FAST_ARENA_ID, sky::lib::memory::eAllocDirectionType_Automatic )
#define SkyRealloc( ptr , size )	SkyNewRealloc_( ptr , size , __FILE__ , __LINE__ , sky::lib::memory::eAllocType_SkyNew , sky::lib::memory::FAST_GROUND_ID , sky::lib::memory::FAST_ARENA_ID, sky::lib::memory::eAllocDirectionType_Automatic )
#define SkyMalloc_( size )          SkyNewMalloc_ ( size , __FILE__ , __LINE__ , sky::lib::memory::eAllocType_SkyNew , sky::lib::memory::g_BaseAllocatorInfo.m_GroundId , sky::lib::memory::g_BaseAllocatorInfo.m_ArenaId , sky::lib::memory::g_BaseAllocatorInfo.m_eDirType )
#define SkyRealloc_( ptr , size )   SkyNewRealloc_( ptr  , size , __FILE__ , __LINE__ , sky::lib::memory::eAllocType_SkyNew , sky::lib::memory::g_BaseAllocatorInfo.m_GroundId , sky::lib::memory::g_BaseAllocatorInfo.m_ArenaId , sky::lib::memory::g_BaseAllocatorInfo.m_eDirType )

/// <summary>
/// メモリ開放関数再定義
/// </summary>
#define SkyFree( ptr )			SkyNewDelete_( ptr , sky::lib::memory::eAllocType_SkyNew )

/// <summary>
/// 確保数を出力する
/// </summary>
void PrintMemory();

/// <summary>
/// メモリ破壊チェック（破壊されていたなら内部でAssertが出ます）
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

//アロケーターがあればアロケーターを使用し、なければ最善のnewを使用する
#define NEW__( type , constractor ) NEW_() constractor
#define NEW_ARRAY__( type , num )   NEW_ARRAY_() type[ num ]
#define SkyMalloc__( size )         SkyMalloc_ ( size )
#define SkyRealloc__( ptr , size )  SkyRealloc_( ptr , size )
#else

/// <summary>
/// new演算子の再定義
/// </summary>
#define NEW						new
#define NEW_ARRAY				new
#define NEW_NATIVE				new
#define NEW_()                  new
#define NEW_ARRAY_()            new

/// <summary>
/// delete演算子の再定義
/// </summary>
#define DEL						delete
#define DEL_ARRAY				delete[]
#define DEL_NATIVE( t , p )		delete p
#define SAFE_RELEASE( p )		{ if ( p )					{ ( p )->Release();	( p ) = skyNull; } }	// Release()関数を呼ぶ
#define SAFE_DELETE( p )		{ if (skyNull != ( p ) )	{ DEL	( p );	( p ) = skyNull; } }		// ポインタを開放する
#define SAFE_DELETE_ARR( p )	{ if (skyNull != ( p ) )	{ DEL[] ( p );	( p ) = skyNull; } }		// 配列ポインタを削除する

/// <summary>
/// メモリ確保関数再定義
/// </summary>
#define SkyMalloc( size )			MALLOC__( size , __FILE__ , __LINE__ )
#define SkyRealloc( ptr , size )	REALLOC__( ptr , size , __FILE__ , __LINE__ )
#define SkyMalloc_( size )          MALLOC__( size , __FILE__ , __LINE__ )
#define SkyRealloc_( ptr , size )   REALLOC__( ptr , size , __FILE__ , __LINE__ )

/// <summary>
/// メモリ開放関数再定義
/// </summary>
#define SkyFree( ptr )			    FREE__( ptr )

/// <summary>
/// 確保数を出力する
/// </summary>
inline void PrintMemory(){}

/// <summary>
/// メモリ破壊チェック（破壊されていたなら内部でAssertが出ます）
/// </summary>
inline void CheckMemBreak(){}

//#defineでOFFになっている型の定義
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
