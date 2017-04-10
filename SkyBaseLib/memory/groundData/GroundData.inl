namespace sky { namespace lib { namespace memory {

inline CGroundData::CGroundData()
{
	m_ArenaUniqueid = 0;
}

inline u32	CGroundData::CreateArena( u32 size , const skyWString *pArenaName )
{
	SKY_UNUSED_ARG( pArenaName );
	u32 result = UINT_MAX;
	u32 realSize = size 
		+ sizeof( CAllocatorDataHeader ) + sizeof ( CArenaData )          
		+ sizeof( CAllocatorDataFooter ) + sizeof( CAllocatorDataFooter );

	//確保サイズが１ブロックのサイズを下回るか、アリーナに必要量のメモリが存在しなければ確保できない。
	if ( m_BlockSize > size || m_Size < m_UsedSize + size )
	{
		SKY_PANIC_MSG( _T( "The memory doesn't suffice. \n" ) );
	}
	else
	{
		//指定のsizeに加えてsizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter )の値分グラウンドに余りがないと確保できない。
		CArenaData *pArena = new(  m_StackAllocator.Malloc( realSize , __FILE__ , __LINE__ ) ) CArenaData();

		SKY_ASSERT_MSG( pArena != skyNull , _T( "The memory of the ground did not suffice." ) ); //メモリが足りません。

		//アリーナに識別IDを発行
		pArena->m_Uniqueid = m_ArenaUniqueid;
		m_ArenaUniqueid++;//カウントUP（このIDを使い切ることはないとおもう）

		//確保済みサイズを加算
		m_UsedSize += size;

		//新しいアリーナを確保確保済みアリーナリストに投入
		m_ArenaList.Link( pArena );

		//アリーナ情報を設定する
#ifdef SW_SKYLIB_DEBUG_ON
		CStringUtil_::Copy( pArena->m_Name , ARENA_NAME_MAX , pArenaName );
#endif
		pArena->m_CheckBreakId	        = ALLOCATE_DATA_ID;
		pArena->m_Size	        = size + sizeof( CAllocatorDataHeader )	+ sizeof( CAllocatorDataFooter );
		pArena->m_StartAdr		= ( u32 )pArena;
		pArena->m_EndAdr		= pArena->m_StartAdr + pArena->m_Size;
		pArena->m_pBuf			= ( u8 * )pArena + sizeof( CArenaData );
		pArena->m_BlockSize     = m_BlockSize;
		pArena->m_eAllocType    = m_eAllocType;
		pArena->m_StackAllocator.AttachMemory( pArena->m_pBuf , pArena->m_Size , pArena->m_BlockSize , pArena->m_BlockSize , pArena->m_eAllocType );

		//フッターの値を設定
		CAllocatorDataFooter *pFooter = ( CAllocatorDataFooter * )( ( u8 * )pArena + sizeof( CArenaData ) + pArena->m_Size );
		pFooter->m_CheckBreakId = ALLOCATE_DATA_ID;

		//識別IDを返す
		result = pArena->m_Uniqueid;
	}
	return result;
}

inline void CGroundData::DeleteArena( u32 arenaHndl )
{
	CArenaData *pArena = m_ArenaList.GetUniqueid( arenaHndl );

	//メモリ破壊チェック
	pArena->CheckMemBreak();

	//アリーナリストから外す
	m_ArenaList.UnLink( pArena );

	//デストラクタ
	pArena->~CArenaData();

	//確保済みサイズを加算
	m_UsedSize -= pArena->m_Size;
}

inline void CGroundData::PrintMemory()
{
	CArenaData* pData = skyNull;
	for ( u32 i = 0 ; i < m_ArenaList.GetNum() ; i++ )
	{
		pData = m_ArenaList.IdxFromHead( i );
		pData->PrintMemory();
	}
}

inline void CGroundData::CheckMemBreak()
{
	//フッターを取り出す
	CAllocatorDataFooter *pFooter = ( CAllocatorDataFooter * )( ( u8 * )this + sizeof( CGroundData ) + m_Size );
	SKY_UNUSED_ARG( pFooter );
	SKY_ASSERT_MSG( m_CheckBreakId == ALLOCATE_DATA_ID && pFooter->m_CheckBreakId == ALLOCATE_DATA_ID , _T( "The value of the memory is illegal. " ) ); //メモリが壊されているか、 配列NEWした領域をDELしたか（DEL_ARRAYでなければならない）NEW_NATIVEで確保した領域をdeleteしました（正確にはDEL_NATIVEを使用してください）

	CArenaData *pData = skyNull;
	for ( u32 i = 0 ; i < m_ArenaList.GetNum() ; i++ )
	{
		pData = m_ArenaList.IdxFromHead( i );
		pData->CheckMemBreak();
	}
}

} } }
