#pragma once

#ifdef SW_SKYLIB_DEFAULT_NEW_OVERWRIDE

//Windowsのデバッグ版だけメモリリークの特別な仕組みを作動する
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
/// アロケーター
/// </summary>
/*
		本アロケーターには下記の概念がある
		・グラウンド
		・アリーナ
		・ブロック
		・バック

		それぞれの関係は下記の通り

		＜ グラウンド ＞
		+---------------------------------------------------+-----------------------------------------------------+
		|  ブロック  |                                      |                                                     |
		+------------+                                      |                                                     |
		|                                                   |                                                     |
		|                                                   |                                                     |
		|                                                   |                                                     |
		|                                                   |                                                     |
		|                                                   |                                                     |
		|               ＜ アリーナ１ ＞                    |                 ＜　アリーナ２ ＞ 　                |
		|                                                   |                                                     |
		|                                                   |                                                     |
		|                                                   |                                                     |
		|                                                   |                                                     |
		|                                                   |                                                     |
		|      +--------------------------------------------+                                                     |
		|      |                                            |                                                     |
		|      |               バック                       |                                                     |
		|      |                                            |                                                     |
		+---------------------------------------------------+-----------------------------------------------------+

		さらにBLOCK_SIZE_MAX から　BLOCK_SIZE_MIN までの間のサイズを確保しようとした場合、上記アロケーターとは別にミニサイズ用アロケーターが走る。

---------------------------------------------------------------------------------------------
    グラウンド、アリーナ指定のメモリ確保方法。SkyAllocatorInfoにアロケート元情報を指定してアロケート時ににそれを渡す。

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
	/// コンストラクタ
	/// </summary>
	CAllocator( u32 blockSize = START_BLOCK_SIZE );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CAllocator();

	/// <summary>
	/// グラウンド（全使用領域）の確保
	/// </summary>
	u32 CreateGround( u32 size );

	/// <summary>
	/// アロケーターID取得
	/// </summary>
	u32 GetAllocatorId(){ return m_AllocatorId; }

	/// <summary>
	/// グラウンド（全使用領域）の開放
	/// </summary>
	void DeleteGround( u32 groundHndl );

	/// <summary>
	/// アリーナの作成（アリーナ＝グラウンドの１区画）
	/// </summary>
	u32 CreateArena( u32 groundHndl , u32 size , const skyWString *arenaName = _T( "default_name" ) );

	/// <summary>
	/// アリーナの作成（アリーナ＝グラウンドの１区画）
	/// </summary>
	void DeleteArena( u32 groundHndl , u32 arenaHndl );

	/// <summary>
	/// メモリ確保
	/// </summary>
	void *Malloc( u32 size ,  s32 groundHndl , s32 arenaHndl , eAllocDirectionType eAreaType = eAllocDirectionType_Automatic , const skyMString *pFname = "none" , s32 line = -1 );

	/// <summary>
	/// メモリ再確保
	/// </summary>
	void *Realloc( void *pData , u32 size , s32 groundHndl , s32 arenaHndl , eAllocDirectionType eAreaType = eAllocDirectionType_Automatic , const skyMString *pFname = "none" , s32 line = -1 );

	/// <summary>
	/// 不要な値をメモリから削除
	/// </summary>
	void Delete( void *pData );

	/// <summary>
	/// メモリ状況をプリントする
	/// </summary>
	void PrintMemory();

	/// <summary>
	/// メモリ破壊のチェック
	/// </summary>
	void CheckMemBreak();

	/// <summary>
	/// クリティカルセクションの取得
	/// </summary>
    thread::CriticalSection* GetCriticalSection();

	/// <summary>
	/// アロックタイプの設定
	/// </summary>
	void SetAllocType( eAllocType eAllocType )
	{
		m_eAllocType = eAllocType;
	}

//---System---

	/// <summary>
	/// コンストラクタ（これは使わないでください）
	/// </summary>
	CAllocator( u32 blockSize , u32 id );

	/// <summary>
	/// デフォルトのグラウンドハンドル
	/// </summary>
	u32					m_DefaultGroundHndl;

	/// <summary>
	/// デフォルトのアリーナハンドル
	/// </summary>
	u32					m_DefaultArenaHndl;

	/// <summary>
	/// グランドデータ双方向リスト（双方向リストなので追加時に内部でアロックしません）
	/// </summary>
	GroundDataPtrDList	m_pGroundDataDList;

private:

	/// <summary>
	/// アロケーターID
	/// </summary>
	u32        m_AllocatorId;

	/// <summary>
	/// アロックタイプ
	/// </summary>
	eAllocType m_eAllocType;

	/// <summary>
	/// アロケーターID
	/// </summary>
	u32        m_BlockSize;

	/// <summary>
	/// クリティカルセクション
	/// </summary>
	thread::CriticalSection* m_pCs;

	/// <summary>
	/// 末期化済みフラグ
	/// </summary>
	skyBool				m_bIsDeposed;

	/// <summary>
	/// グラウンドの識別ID
	/// </summary>
	s32                 GroundUniqueid;

};

} } }

#ifdef SW_SKYLIB_DEFAULT_NEW_OVERWRIDE

/// <summary>
/// 指定のハンドルのグラウンドの情報を取得
/// </summary>
const u32 GetDefaultMemoryGroundHandl();
const sky::lib::memory::CGroundData *GetMemoryGroundData( s32 groundHndl );

/// <summary>
/// 指定のハンドルのアリーナの情報を取得
/// </summary>
const u32 GetDefaultMemoryArenaHandl();
const sky::lib::memory::CArenaData *GetMemoryArenaData( s32 groundHndl , u32 arenaHndl );

#else

/// <summary>
/// 指定のハンドルのグラウンドの情報を取得
/// </summary>
inline const u32 GetDefaultMemoryGroundHandl(){ return 0; }
inline const sky::lib::memory::CGroundData *GetMemoryGroundData( s32 groundHndl )
{ 
	SKY_UNUSED_ARG( groundHndl );
	return ( sky::lib::memory::CGroundData * )0;
}

/// <summary>
/// 指定のハンドルのアリーナの情報を取得
/// </summary>
inline const u32 GetDefaultMemoryArenaHandl(){ return 0; }
inline const sky::lib::memory::CArenaData *GetMemoryArenaData( s32 groundHndl , u32 arenaHndl )
{ 
	SKY_UNUSED_ARG( groundHndl );
	SKY_UNUSED_ARG( arenaHndl );
	return ( sky::lib::memory::CArenaData * )0;
}

#endif