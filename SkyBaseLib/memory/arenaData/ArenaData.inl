namespace sky { namespace lib { namespace memory {

inline void* CArenaData::Malloc( u32 size , s32 groundHndl , s32 arenaHndl , const skyMString *pFname , u32 line )
{
	CAllocatorDataHeader* pResult = skyNull;

	if ( m_BlockSize >= size )
	{
		//確保
		pResult = ( CAllocatorDataHeader * )( ( u8 * )m_StackAllocator.Malloc( START_BLOCK_SIZE , pFname , line , eAllocSizeType_Block ) - sizeof( CAllocatorDataHeader ) );
		SKY_ASSERT_MSG( pResult != skyNull , _T( "malloc is failed." ) );
	}
	else
	{
		//確保
		pResult = ( CAllocatorDataHeader * )( ( u8 * )m_StackAllocator.BackMalloc( size , pFname , line , eAllocSizeType_Size ) - sizeof( CAllocatorDataHeader ) );
		SKY_ASSERT_MSG( pResult != skyNull , _T( "backmalloc is failed." ) );
	}

	//情報を格納
	pResult->m_GroundHndl = groundHndl;
	pResult->m_ArenaHndl = arenaHndl;
	pResult->m_Line = line;

	//リターン
	return ( u8 * )pResult + sizeof( CAllocatorDataHeader );
}

inline void CArenaData::Delete( CAllocatorDataHeader *pData )
{
	m_StackAllocator.Delete( ( u8 * )pData + sizeof( CAllocatorDataHeader ) );
}

inline void* CArenaData::Realloc( void *pData , u32 size , s32 groundHndl , s32 arenaHndl , const skyMString *pFname , u32 line )
{
	CAllocatorDataHeader* pResult = ( CAllocatorDataHeader * )( ( u8 * )m_StackAllocator.Realloc( pData , size , pFname , line ) - sizeof( CAllocatorDataHeader ) );

	//情報を格納
	pResult->m_GroundHndl = groundHndl;
	pResult->m_ArenaHndl = arenaHndl;
	pResult->m_Line = line;

	//リターン
	return ( u8 * )pResult + sizeof( CAllocatorDataHeader );
}

inline void CArenaData::PrintMemory()
{
	m_StackAllocator.PrintMemory();
}

inline void CArenaData::CheckMemBreak()
{
	//自分自身のヘッダーとフッターの破壊検査
	CAllocatorDataFooter *pFooter = ( CAllocatorDataFooter * )( ( u8 * )this + sizeof( CArenaData ) + m_Size );
	SKY_UNUSED_ARG( pFooter );
	SKY_ASSERT_MSG( m_CheckBreakId == ALLOCATE_DATA_ID && pFooter->m_CheckBreakId == ALLOCATE_DATA_ID , _T( "The value of the memory is illegal. " ) ); //メモリが壊されているか、 配列NEWした領域をDELしたか（DEL_ARRAYでなければならない）NEW_NATIVEで確保した領域をdeleteしました（正確にはDEL_NATIVEを使用してください）

	//アロック情報の破壊検査
	m_StackAllocator.CheckMemBreak();
}

} } }
