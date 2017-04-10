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

	//�m�ۃT�C�Y���P�u���b�N�̃T�C�Y������邩�A�A���[�i�ɕK�v�ʂ̃����������݂��Ȃ���Ίm�ۂł��Ȃ��B
	if ( m_BlockSize > size || m_Size < m_UsedSize + size )
	{
		SKY_PANIC_MSG( _T( "The memory doesn't suffice. \n" ) );
	}
	else
	{
		//�w���size�ɉ�����sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter )�̒l���O���E���h�ɗ]�肪�Ȃ��Ɗm�ۂł��Ȃ��B
		CArenaData *pArena = new(  m_StackAllocator.Malloc( realSize , __FILE__ , __LINE__ ) ) CArenaData();

		SKY_ASSERT_MSG( pArena != skyNull , _T( "The memory of the ground did not suffice." ) ); //������������܂���B

		//�A���[�i�Ɏ���ID�𔭍s
		pArena->m_Uniqueid = m_ArenaUniqueid;
		m_ArenaUniqueid++;//�J�E���gUP�i����ID���g���؂邱�Ƃ͂Ȃ��Ƃ������j

		//�m�ۍς݃T�C�Y�����Z
		m_UsedSize += size;

		//�V�����A���[�i���m�ۊm�ۍς݃A���[�i���X�g�ɓ���
		m_ArenaList.Link( pArena );

		//�A���[�i����ݒ肷��
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

		//�t�b�^�[�̒l��ݒ�
		CAllocatorDataFooter *pFooter = ( CAllocatorDataFooter * )( ( u8 * )pArena + sizeof( CArenaData ) + pArena->m_Size );
		pFooter->m_CheckBreakId = ALLOCATE_DATA_ID;

		//����ID��Ԃ�
		result = pArena->m_Uniqueid;
	}
	return result;
}

inline void CGroundData::DeleteArena( u32 arenaHndl )
{
	CArenaData *pArena = m_ArenaList.GetUniqueid( arenaHndl );

	//�������j��`�F�b�N
	pArena->CheckMemBreak();

	//�A���[�i���X�g����O��
	m_ArenaList.UnLink( pArena );

	//�f�X�g���N�^
	pArena->~CArenaData();

	//�m�ۍς݃T�C�Y�����Z
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
	//�t�b�^�[�����o��
	CAllocatorDataFooter *pFooter = ( CAllocatorDataFooter * )( ( u8 * )this + sizeof( CGroundData ) + m_Size );
	SKY_UNUSED_ARG( pFooter );
	SKY_ASSERT_MSG( m_CheckBreakId == ALLOCATE_DATA_ID && pFooter->m_CheckBreakId == ALLOCATE_DATA_ID , _T( "The value of the memory is illegal. " ) ); //���������󂳂�Ă��邩�A �z��NEW�����̈��DEL�������iDEL_ARRAY�łȂ���΂Ȃ�Ȃ��jNEW_NATIVE�Ŋm�ۂ����̈��delete���܂����i���m�ɂ�DEL_NATIVE���g�p���Ă��������j

	CArenaData *pData = skyNull;
	for ( u32 i = 0 ; i < m_ArenaList.GetNum() ; i++ )
	{
		pData = m_ArenaList.IdxFromHead( i );
		pData->CheckMemBreak();
	}
}

} } }
