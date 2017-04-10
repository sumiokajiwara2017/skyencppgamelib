namespace sky { namespace lib { namespace memory {

inline void* CArenaData::Malloc( u32 size , s32 groundHndl , s32 arenaHndl , const skyMString *pFname , u32 line )
{
	CAllocatorDataHeader* pResult = skyNull;

	if ( m_BlockSize >= size )
	{
		//�m��
		pResult = ( CAllocatorDataHeader * )( ( u8 * )m_StackAllocator.Malloc( START_BLOCK_SIZE , pFname , line , eAllocSizeType_Block ) - sizeof( CAllocatorDataHeader ) );
		SKY_ASSERT_MSG( pResult != skyNull , _T( "malloc is failed." ) );
	}
	else
	{
		//�m��
		pResult = ( CAllocatorDataHeader * )( ( u8 * )m_StackAllocator.BackMalloc( size , pFname , line , eAllocSizeType_Size ) - sizeof( CAllocatorDataHeader ) );
		SKY_ASSERT_MSG( pResult != skyNull , _T( "backmalloc is failed." ) );
	}

	//�����i�[
	pResult->m_GroundHndl = groundHndl;
	pResult->m_ArenaHndl = arenaHndl;
	pResult->m_Line = line;

	//���^�[��
	return ( u8 * )pResult + sizeof( CAllocatorDataHeader );
}

inline void CArenaData::Delete( CAllocatorDataHeader *pData )
{
	m_StackAllocator.Delete( ( u8 * )pData + sizeof( CAllocatorDataHeader ) );
}

inline void* CArenaData::Realloc( void *pData , u32 size , s32 groundHndl , s32 arenaHndl , const skyMString *pFname , u32 line )
{
	CAllocatorDataHeader* pResult = ( CAllocatorDataHeader * )( ( u8 * )m_StackAllocator.Realloc( pData , size , pFname , line ) - sizeof( CAllocatorDataHeader ) );

	//�����i�[
	pResult->m_GroundHndl = groundHndl;
	pResult->m_ArenaHndl = arenaHndl;
	pResult->m_Line = line;

	//���^�[��
	return ( u8 * )pResult + sizeof( CAllocatorDataHeader );
}

inline void CArenaData::PrintMemory()
{
	m_StackAllocator.PrintMemory();
}

inline void CArenaData::CheckMemBreak()
{
	//�������g�̃w�b�_�[�ƃt�b�^�[�̔j�󌟍�
	CAllocatorDataFooter *pFooter = ( CAllocatorDataFooter * )( ( u8 * )this + sizeof( CArenaData ) + m_Size );
	SKY_UNUSED_ARG( pFooter );
	SKY_ASSERT_MSG( m_CheckBreakId == ALLOCATE_DATA_ID && pFooter->m_CheckBreakId == ALLOCATE_DATA_ID , _T( "The value of the memory is illegal. " ) ); //���������󂳂�Ă��邩�A �z��NEW�����̈��DEL�������iDEL_ARRAY�łȂ���΂Ȃ�Ȃ��jNEW_NATIVE�Ŋm�ۂ����̈��delete���܂����i���m�ɂ�DEL_NATIVE���g�p���Ă��������j

	//�A���b�N���̔j�󌟍�
	m_StackAllocator.CheckMemBreak();
}

} } }
