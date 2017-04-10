#include "StdAfx.h"
#include "SkyBaseLib/Memory.h"
#include "SkyBaseLib/Thread.h"
#include "SkyBaseLib/Time.h"

#include "SkyBaseLib/SKYTL.h"

namespace sky { namespace lib { namespace memory {

CAllocator::CAllocator( u32 blockSize ) 
{
	m_bIsDeposed = skyFalse;
	GroundUniqueid = 0;
	m_pCs = skyNull;
	m_BlockSize = blockSize;
	m_AllocatorId = CMemoryUtil_::CreateAllocatorId();
	m_eAllocType = eAllocType_AllocatorClass_New;
}

CAllocator::CAllocator( u32 blockSize , u32 id ) 
{
	m_bIsDeposed = skyFalse;
	GroundUniqueid = 0;
	m_pCs = skyNull;
	m_BlockSize = blockSize;
	m_AllocatorId = id;
	m_eAllocType = eAllocType_AllocatorClass_New;
}

CAllocator::~CAllocator()
{
	if ( m_pCs != skyNull )
	{
		thread::CriticalSection::Delete( m_pCs );
		m_pCs = skyNull;
		m_bIsDeposed = skyTrue;
	}
}

thread::CriticalSection* CAllocator::GetCriticalSection()
{
	if ( m_pCs == skyNull && m_bIsDeposed == skyFalse )
	{
		m_pCs = thread::CriticalSection::Create();
	}
	return m_pCs;
}

u32 CAllocator::CreateGround( u32 size )
{
	u32 result = UINT_MAX;

	if ( m_BlockSize > size )
	{
		SKY_PANIC_MSG( _T( "The memory doesn't suffice. \n" ) );
	}
	else
	{
		//最低一個のアリーナがグラウンドと同等のサイズ指定で確保できるようにsizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter )を加算しておく
		u32 realSize = size 
			+ sizeof( CGroundData )          + sizeof( CAllocatorDataHeader ) + sizeof ( CArenaData )          + sizeof( CAllocatorDataHeader )
			+ sizeof( CAllocatorDataFooter ) + sizeof( CAllocatorDataFooter ) + sizeof( CAllocatorDataFooter ) + sizeof( CAllocatorDataFooter );

		//確保したメモリ領域の先頭部分をGroundDataクラスの型とみなす。
		CGroundData *pGroundData = new( MALLOC__( realSize , __FILE__ , __LINE__ ) ) CGroundData();

		//アロケーターIDを登録
		pGroundData->m_AllocatorId = m_AllocatorId;

		//グラウンドに情報を設定する
		pGroundData->m_CheckBreakId		 = ALLOCATE_DATA_ID;
		pGroundData->m_Size		 = size	+ sizeof( CAllocatorDataHeader ) + sizeof( CArenaData )           + sizeof( CAllocatorDataHeader )
									    + sizeof( CAllocatorDataFooter ) + sizeof( CAllocatorDataFooter ) + sizeof( CAllocatorDataFooter );
		pGroundData->m_StartAdr	 = ( u32 )pGroundData;
		pGroundData->m_EndAdr	 = pGroundData->m_StartAdr + pGroundData->m_Size;
		pGroundData->m_pBuf      = ( u8 * )pGroundData + sizeof ( CGroundData );
		pGroundData->m_UsedSize  = 0;
		pGroundData->m_BlockSize = m_BlockSize;
		pGroundData->m_eAllocType= m_eAllocType;
		pGroundData->m_StackAllocator.AttachMemory( pGroundData->m_pBuf , pGroundData->m_Size , pGroundData->m_BlockSize , pGroundData->m_BlockSize , m_eAllocType );

#ifdef SW_SKYLIB_DEBUG_ON
		//管理領域を指定の値で塗りつぶす（メモリマップを見たときに、グラウンド無いか、グランド外か、使用領域か未使用領域かわかる）
		CMemoryUtil_::ClearValue( pGroundData->m_pBuf , MEMORY_INIT_VALUE , pGroundData->m_Size );
#endif
		//フッターの値を設定
		CAllocatorDataFooter *pFooter = ( CAllocatorDataFooter * )( ( u8 * )pGroundData + sizeof( CGroundData ) + pGroundData->m_Size );
		pFooter->m_CheckBreakId = ALLOCATE_DATA_ID;

		//識別IDを付加
		pGroundData->m_Uniqueid = GroundUniqueid;
		GroundUniqueid++;

		//グラウンドリストに登録
		m_pGroundDataDList.Link( pGroundData );

		//識別IDを返す
		result = pGroundData->m_Uniqueid;
	}
	return result;
}

void CAllocator::DeleteGround( u32 groundHndl )
{
	//対象のグラウンドの先頭ポインタ取得
	CGroundData *pGround = m_pGroundDataDList.GetUniqueid( groundHndl );

	SKY_ASSERT_MSG( pGround->m_ArenaList.GetNum() == 0 , _T( "The arena still remains." ) );

	//破壊チェック
	pGround->CheckMemBreak();

	//アロケーターIDチェック
	SKY_ASSERT_MSG( pGround->m_AllocatorId == m_AllocatorId , _T( "AllocatorId id invalid. \n" ) );

	//グラウンドリストから破棄
	m_pGroundDataDList.UnLink( pGround );

	//デストラクタ
	pGround->~CGroundData();

	//メモリから消す
	FREE__( pGround );
}

u32 CAllocator::CreateArena( u32 groundHndl , u32 size , const skyWString *arenaName )
{
	u32 hndl = 0;

	//グラウンドハンドルが不正の場合停止
	SKY_ASSERT_MSG( m_pGroundDataDList.GetNum() >= ( u32 )groundHndl , _T( "It went to secure the memory for the ground that did not exist. " ) ); //存在しないグラウンドに対してメモリを確保しに行きました。

	//グラウンドの取得
	CGroundData *pGroundData = m_pGroundDataDList.GetUniqueid( groundHndl );

	//グラウンドからアリーナを生成する
	hndl = pGroundData->CreateArena( size , arenaName );

	return hndl;
}

void CAllocator::DeleteArena( u32 groundHndl , u32 arenaHndl )
{
	//グラウンドの取得
	CGroundData *pGroundData = m_pGroundDataDList.GetUniqueid( groundHndl );

	//アリーナの削除
	pGroundData->DeleteArena( arenaHndl );
}

void *CAllocator::Malloc( u32 size , s32 groundHndl , s32 arenaHndl , eAllocDirectionType eAreaType , const skyMString *pFname , s32 line )
{
	thread::CriticalLock lock( GetCriticalSection() );

	//グラウンドができていなかったらデフォルト値で確保する
	if ( m_pGroundDataDList.GetNum() == 0 )
	{
		//アロケーターIDの初期化
		CMemoryUtil_::ResetAllocatorId();

		//デフォルトのグラウンド確保
		m_DefaultGroundHndl = CreateGround( START_GROUND_SIZE );

		//デフォルトのアリーナ確保
		m_DefaultArenaHndl = CreateArena( m_DefaultGroundHndl , START_ARENA_SIZE , START_ARENA_NAME );
	}

	//グラウンドハンドルが不正の場合停止
	SKY_ASSERT_MSG( m_pGroundDataDList.GetNum() >= ( u32 )groundHndl , _T( "It went to secure the memory for the ground that did not exist. " ) ); //存在しないグラウンドに対してメモリを確保しに行きました。

	//グラウンドの取得
	CGroundData *pGroundData = m_pGroundDataDList.GetUniqueid( groundHndl );

	//サイズチェック
	SKY_ASSERT_MSG( pGroundData->m_Size >= size , _T( "The size that tries to be secured exceeds the size of the ground. " ) ); //確保しようとしているサイズがグラウンドサイズを超えています。

	//アリーナハンドルが不正の場合停止
	SKY_ASSERT_MSG( pGroundData->m_ArenaList.GetNum() >= ( u32 )arenaHndl , _T( "It went to secure the memory for the arena that did not exist. " ) ); //存在しないアリーナに対してメモリを確保しに行きました。

	//アリーナの取得
	CArenaData *pArenaData = pGroundData->m_ArenaList.GetUniqueid( arenaHndl );

	//サイズチェック
	SKY_ASSERT_MSG( pArenaData != skyNull , _T( "The memory doesn't suffice. " ) ); //メモリが足りません！
	SKY_ASSERT_MSG( pArenaData->m_Size >= size , _T( "The size that tries to be secured exceeds the size of the arena. " ) ); //確保しようとしているサイズがアリーナサイズを超えています。

	//アリーナから確保
	void *pResultAdr = pArenaData->Malloc( size , groundHndl , arenaHndl , pFname , line );

	return pResultAdr;
}

void *CAllocator::Realloc( void *pData , u32 size , s32 groundHndl , s32 arenaHndl , eAllocDirectionType eAreaType , const skyMString *pFname , s32 line )
{
	void *pResultAdr = skyNull;
	if ( pData == skyNull )
	{
		pResultAdr = Malloc( size , groundHndl , arenaHndl , eAreaType , pFname , line );
	}
	else
	{
		thread::CriticalLock lock( GetCriticalSection() );

		//グラウンドハンドルが不正の場合停止
		SKY_ASSERT_MSG( m_pGroundDataDList.GetNum() >= ( u32 )groundHndl , _T( "It went to secure the memory for the ground that did not exist. " ) ); //存在しないグラウンドに対してメモリを確保しに行きました。

		//グラウンドの取得
		CGroundData *pGroundData = m_pGroundDataDList.GetUniqueid( groundHndl );

		//サイズチェック
		SKY_ASSERT_MSG( pGroundData->m_Size >= size , _T( "The size that tries to be secured exceeds the size of the ground. " ) ); //確保しようとしているサイズがグラウンドサイズを超えています。

		//アリーナハンドルが不正の場合停止
		SKY_ASSERT_MSG( pGroundData->m_ArenaList.GetNum() >= ( u32 )arenaHndl , _T( "It went to secure the memory for the arena that did not exist. " ) ); //存在しないアリーナに対してメモリを確保しに行きました。

		//アリーナの取得
		CArenaData *pArenaData = pGroundData->m_ArenaList.GetUniqueid( arenaHndl );

		//サイズチェック
		SKY_ASSERT_MSG( pArenaData != skyNull , _T( "The memory doesn't suffice. " ) ); //メモリが足りません！
		SKY_ASSERT_MSG( pArenaData->m_Size >= size , _T( "The size that tries to be secured exceeds the size of the arena. " ) ); //確保しようとしているサイズがアリーナサイズを超えています。

		//アリーナから確保
		pResultAdr = pArenaData->Realloc( pData , size , groundHndl , arenaHndl , pFname , line );

	}
	return pResultAdr;
}

void CAllocator::Delete( void *pData )
{
//	TIMER_OBJCET( _T( "CAllocator::Delete" ) );

	thread::CriticalLock lock( GetCriticalSection() );

	//ヘッダーを取り出す
	CAllocatorDataHeader *pAllocateData = ( CAllocatorDataHeader * )( ( u8 * )pData - sizeof( CAllocatorDataHeader ) );

	//フッターを取り出す
	CAllocatorDataFooter *pFooter = ( CAllocatorDataFooter * )( ( u8 * )pData + pAllocateData->m_Size );

	//破壊チェック
	SKY_UNUSED_ARG( pFooter );
	SKY_ASSERT_MSG( pAllocateData->m_CheckBreakId == ALLOCATE_DATA_ID && pFooter->m_CheckBreakId == ALLOCATE_DATA_ID , _T( "The value of the memory is illegal. " ) ); //メモリが壊されているか、 配列NEWした領域をDELしたか（DEL_ARRAYでなければならない）NEW_NATIVEで確保した領域をdeleteしました（正確にはDEL_NATIVEを使用してください）

	//確保元グラウンドを判別
	CGroundData *pGroundData = m_pGroundDataDList.GetUniqueid( pAllocateData->m_GroundHndl );

	//グラウンドハンドルが不正の場合停止
	SKY_ASSERT_MSG( m_pGroundDataDList.GetNum() >= ( u32 )pAllocateData->m_GroundHndl , _T( "It went to secure the memory for the ground that did not exist. " ) ); //存在しないグラウンドに対してメモリを確保しに行きました。

	//確保元アリーナを判別
	CArenaData *pArenaData = pGroundData->m_ArenaList.GetUniqueid( pAllocateData->m_ArenaHndl );

	//アリーナから削除
	pArenaData->Delete( pAllocateData );
}

void CAllocator::PrintMemory()
{
	CGroundData* pData = skyNull;
	for ( u32 i = 0 ; i < m_pGroundDataDList.GetNum() ; i++ )
	{
		pData = m_pGroundDataDList.IdxFromHead( i );
		pData->PrintMemory();
	}
}

void CAllocator::CheckMemBreak()
{
	CGroundData* pData = skyNull;
	for ( u32 i = 0 ; i < m_pGroundDataDList.GetNum() ; i++ )
	{
		pData = m_pGroundDataDList.IdxFromHead( i );
		pData->CheckMemBreak();
	}
}

} } }

#ifdef SW_SKYLIB_DEFAULT_NEW_OVERWRIDE

/// <summary>
/// アロケーター
/// </summary>
static sky::lib::memory::CAllocator	s_CAllocator( sky::lib::memory::START_BLOCK_SIZE , sky::lib::memory::SYSTEM_ALLOCATOR_ID );

u32 CreateMemoryGround( u32 size )
{
	SKY_ASSERT_MSG( size >= sky::lib::memory::GROUND_DATA_MIN_SIZE , _T( "The size doesn't suffice." ) ); //サイズが足りない

	return s_CAllocator.CreateGround( size );
}

void DeleteMemoryGround( u32 groundHndl )
{
	s_CAllocator.DeleteGround( groundHndl );
}

u32 CreateMemoryArena( u32 groundHndl , u32 size , const skyString *pArenaName )
{
	SKY_ASSERT_MSG( size >= sky::lib::memory::ARENA_DATA_MIN_SIZE , _T( "The size doesn't suffice." ) ); //サイズが足りない

	return s_CAllocator.CreateArena( groundHndl , size , pArenaName );
}

void DeleteMemoryArena( u32 groundHndl , u32 arenaHndl )
{
	s_CAllocator.DeleteArena( groundHndl , arenaHndl );
}

void *SkyNewMalloc_( u32 size , const skyMString *pFname , s32 line , sky::lib::memory::eAllocType eType , s32 groundHndl , s32 arenaHndl, sky::lib::memory::eAllocDirectionType eAreaType )
{
	if ( eType == sky::lib::memory::eAllocType_Native_New )
	{
		return MALLOC__( size , pFname , line );
	}
	else
	{
#if defined( _DEBUG ) && defined( SW_SKYLIB_CRT_MEMORY_LEAK_CHK )
		eAreaType;
		arenaHndl;
		groundHndl;
		return MALLOC__( size , pFname , line );
#else
		s_CAllocator.SetAllocType( sky::lib::memory::eAllocType_SkyNew );
		return s_CAllocator.Malloc( size , groundHndl , arenaHndl , eAreaType , pFname , line );
#endif
	}
}

void *SkyNewRealloc_( void* pData , u32 size , const skyMString *pFname , s32 line , sky::lib::memory::eAllocType eType , s32 groundHndl , s32 arenaHndl, sky::lib::memory::eAllocDirectionType eAreaType )
{
	if ( eType == sky::lib::memory::eAllocType_Native_New )
	{//NativeなreallocはリアロックするデータもNativeであるとみなす。

		//ヘッダーを取り出す
		sky::lib::memory::CAllocatorDataHeader *pOldAllocateData = ( sky::lib::memory::CAllocatorDataHeader * )( ( u8 * )pData - sizeof( sky::lib::memory::CAllocatorDataHeader ) );

		SKY_ASSERT_MSG( pOldAllocateData->m_CheckBreakId == sky::lib::memory::ALLOCATE_DATA_ID , _T( "This memory cannot lock rear. " ) ); //CAllocatorでmallocしたメモリをネイティブなリアロックにかけた。

		return REALLOC__( pData , size , pFname , line );
	}
	else
	{
#if defined( _DEBUG ) && defined( SW_SKYLIB_CRT_MEMORY_LEAK_CHK )
		eAreaType;
		arenaHndl;
		groundHndl;
		return REALLOC__( pData , size , pFname , line );
#else
		return s_CAllocator.Realloc( pData , size , groundHndl , arenaHndl , eAreaType , pFname , line );
#endif
	}
}

void SkyNewDelete_( void* pData , sky::lib::memory::eAllocType eType )
{
	if ( pData == skyNull ) return;

	if ( eType == sky::lib::memory::eAllocType_Native_New )
	{
		FREE__( pData );
	}
	else
	{
		//ヘッダーを取り出す
		sky::lib::memory::CAllocatorDataHeader *pAllocateDataHeader = ( sky::lib::memory::CAllocatorDataHeader * )( ( u8 * )pData - sizeof( sky::lib::memory::CAllocatorDataHeader ) );

		//ユーザーが生成したアロケーターで確保されたか？
		if( pAllocateDataHeader->m_CheckBreakId == sky::lib::memory::ALLOCATE_DATA_ID && 
			pAllocateDataHeader->m_eAllocType == sky::lib::memory::eAllocType_AllocatorClass_New )
		{
			if ( pAllocateDataHeader->m_pAllocator == skyNull )
			{
				SKY_PANIC_MSG( _T( "There is a possibility that this memory has already been annulled. " ) ); //このメモリはすでに破棄されたか壊れている可能性があります。
			}
			else
			{
				SKY_ASSERT_MSG( pAllocateDataHeader->m_pAllocator->m_CheckBreakId == sky::lib::memory::ALLOCATE_DATA_ID  , _T( "This memory cannot lock rear. " ) );//このメモリはすでに破棄されているか、アロケーターポインタが壊れてる可能性がある。
				pAllocateDataHeader->m_pAllocator->Delete( pData );
			}
		}
		//システムがスタティックで生成したアロケーターで生成されれたデータ
		else
		{
#if defined( _DEBUG ) && defined( SW_SKYLIB_CRT_MEMORY_LEAK_CHK )
			FREE__( pData );
#else
			s_CAllocator.Delete( pData );
#endif
		}
	}
}

void PrintMemory()
{
	s_CAllocator.PrintMemory();
}

void CheckMemBreak()
{
	s_CAllocator.CheckMemBreak();
}

const u32 GetDefaultMemoryGroundHandl()
{
	return s_CAllocator.m_DefaultGroundHndl;
}

const sky::lib::memory::CGroundData *GetMemoryGroundData( s32 groundHndl )
{
	return s_CAllocator.m_pGroundDataDList.GetUniqueid( groundHndl );
}

const u32 GetDefaultMemoryArenaHandl()
{
	return s_CAllocator.m_DefaultArenaHndl;
}

const sky::lib::memory::CArenaData *GetMemoryArenaData( s32 groundHndl , u32 arenaHndl )
{
	sky::lib::memory::CGroundData *pGroundData = s_CAllocator.m_pGroundDataDList.GetUniqueid( groundHndl );
	return pGroundData->m_ArenaList.GetUniqueid( arenaHndl );
}

#else
void *SkyNewMalloc_( u32 size , const skyMString *pFname , s32 line , sky::lib::memory::eAllocType eType , s32 groundHndl , s32 arenaHndl, sky::lib::memory::eAllocDirectionType eAreaType )
{
    SKY_UNUSED_ARG( eType );
    SKY_UNUSED_ARG( groundHndl );
    SKY_UNUSED_ARG( eAreaType );
    return MALLOC__( size , pFname , line );
}

void *SkyNewRealloc_( void* pData , u32 size , const skyMString *pFname , s32 line , sky::lib::memory::eAllocType eType , s32 groundHndl , s32 arenaHndl, sky::lib::memory::eAllocDirectionType eAreaType )
{
    SKY_UNUSED_ARG( eType );
    SKY_UNUSED_ARG( groundHndl );
    SKY_UNUSED_ARG( eAreaType );
    return REALLOC__( pData , size , pFname , line );
}

void SkyNewDelete_( void* pData , sky::lib::memory::eAllocType eType )
{
    SKY_UNUSED_ARG( eType );
    FREE__( pData );
}
#endif

