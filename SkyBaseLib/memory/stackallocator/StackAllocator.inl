namespace sky { namespace lib { namespace memory {

inline CStackAllocator::CStackAllocator() : m_pBuffer( skyNull )
{
	m_AllocatorId = CMemoryUtil_::CreateAllocatorId();
}

inline void CStackAllocator::AttachMemory( void *pBuffer , u32 bufferSize , u32 blockSize , u32 backBlockSize, eAllocType eType )
{
//	if ( m_pBuffer != skyNull ) return;

	SKY_ASSERT_MSG( bufferSize > sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter ) , _T( "The size doesn't suffice." ) );

	m_pBuffer = pBuffer;
	m_BufferSize = bufferSize;
	m_StackTop = 0;
	m_StackNo = 0;
	m_DeleteListMemorySize = 0;
	m_BackStackTop = bufferSize;
	m_BackStackNo = 0;
	m_BackDeleteListMemorySize = 0;
	m_BlockSize = blockSize;
	m_BackBlockSize = backBlockSize;
	m_eType = eType;
}

inline void CStackAllocator::CreateMemory( u32 bufferSize , u32 blockSize , u32 backBlockSize, eAllocType eType )
{
//	if ( m_pBuffer != skyNull ) return;

	SKY_ASSERT_MSG( bufferSize > sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter ) , _T( "The size doesn't suffice." ) );

	//����������m�ۂ���
//	m_pBuffer =  ( void * )MALLOC__( bufferSize , DEFAULT_ALIGN , __FILE__ , __LINE__ );
	m_pBuffer =  ( void * )MALLOC__( bufferSize , __FILE__ , __LINE__ );
	m_BufferSize = bufferSize;
	m_StackTop = 0;
	m_StackNo = 0;
	m_DeleteListMemorySize = 0;
	m_BackStackTop = bufferSize;
	m_BackStackNo = 0;
	m_BackDeleteListMemorySize = 0;
	m_BlockSize = blockSize;
	m_BackBlockSize = backBlockSize;
	m_eType = eType;
}

inline void CStackAllocator::DeleteMemory()
{
	//�������������
	FREE__( m_pBuffer );
}

inline u32 CStackAllocator::GetAllSize() const
{
	return m_BufferSize;
}

inline u32 CStackAllocator::GetUsefulSize() const
{
	return m_BufferSize - GetUsedSize();
}

inline u32 CStackAllocator::GetUsedStackSize() const
{
	return m_StackTop + ( m_BufferSize - m_BackStackTop );
}

inline u32 CStackAllocator::GetFrontUsedStackSize() const
{
	return m_StackTop;
}

inline u32 CStackAllocator::GetBackUsedStackSize() const
{
	return ( m_BufferSize - m_BackStackTop );
}

inline u32 CStackAllocator::GetUsedSize() const
{
	return m_StackTop + ( m_BufferSize - m_BackStackTop )
           - m_DeleteListMemorySize - m_BackDeleteListMemorySize;
}

inline u32 CStackAllocator::GetFrontUsedSize() const
{
	return m_StackTop - m_DeleteListMemorySize;
}

inline u32 CStackAllocator::GetBackUsedSize() const
{
	return m_BufferSize - m_BackStackTop - m_BackDeleteListMemorySize;
}

inline u32 CStackAllocator::GetMallocNum() const
{
	return m_StackNo + m_BackStackNo;
}

inline u32 CStackAllocator::GetDeleteListItemNum() const
{
	return m_DeleteList.GetNum() + m_BackDeleteList.GetNum();
}

inline void *CStackAllocator::Malloc( u32 size , const skyMString *pFname , s32 line , eAllocSizeType eSize )
{
//	SKY_PRINTF( "AllocatorNo = %d \n" , m_AllocatorId );

	SKY_UNUSED_ARG( pFname );

	u8 *pResult = skyNull;
	CAllocatorDataHeader *pHeader = skyNull;

	skyBool isFound = skyFalse;
	if ( eSize == eAllocSizeType_Block && m_BlockDeleteList.GetNum() > 0 && size <= m_BackBlockSize )
	{
		isFound = skyTrue;

		//�폜���X�g�������
		pHeader = ( CAllocatorDataHeader * )( ( u8 * )m_BlockDeleteList.IdxFromTail( 0 ) - sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDeleteDataHeader ) );
		m_BlockDeleteList.UnLink( ( CAllocatorDeleteDataHeader * )&pHeader->m_DeleteList );
		m_DeleteListMemorySize -= pHeader->m_Size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );
		pResult = ( u8 * )pHeader + sizeof( CAllocatorDataHeader );
		pHeader->m_IsUsed = skyTrue;
		pHeader->m_pAllocator = this;
	}
	else if ( m_DeleteList.GetNum() > 0 )
	{//�폜���X�g����m��

		//���`�T���ō폜���X�g��T�����A�m�ۉ\�ȃT�C�Y�����݂��邩�ǂ������肷��
		pHeader = ( CAllocatorDataHeader * )( ( u8 * )m_DeleteList.IdxFromTail( 0 ) - sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDeleteDataHeader ) );
		while( pHeader != skyNull )
		{
			if ( pHeader->m_Size >= size )
			{
				isFound = skyTrue;

				//�K�v�ȕ��������o��
				pHeader = MallocFromMallocData( pHeader , size );

				//�폜���X�g�������
				m_DeleteList.UnLink( ( CAllocatorDeleteDataHeader * )&pHeader->m_DeleteList );
				m_DeleteListMemorySize -= pHeader->m_Size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );
				pResult = ( u8 * )pHeader + sizeof( CAllocatorDataHeader );
				pHeader->m_IsUsed = skyTrue;
				pHeader->m_pAllocator = this;

				break;
			}

			if ( pHeader->m_DeleteList.m_pPrev == skyNull )
			{
				pHeader = skyNull;
			}
			else
			{
				pHeader = ( CAllocatorDataHeader * )( ( u8 * )pHeader->m_DeleteList.m_pPrev - sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDeleteDataHeader ) );
			}
		}
	}

	if( isFound == skyFalse )
	{
		//�u���b�N�T�C�Y�ɒ���
		if ( eSize == eAllocSizeType_Block && size <= m_BlockSize )
		{
			//�傫�߂̑傫���ɕ␳
			size = m_BlockSize;
		}
		else
		{
			//�u���b�N�m�ۂ��ł��Ȃ��̂ŃT�C�Y�^�C�v��␳
			eSize = eAllocSizeType_Size;
		}

		//���ۂ̊m�ۗ̈�v�Z
		u32 realSize = size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );

		//�K�v�̈�`�F�b�N
		if ( realSize + m_StackTop <= m_BackStackTop )
		{
			//�̈�m��
			u8 *pAllocMem = ( u8 * )CMemoryUtil::MallocPointer( m_pBuffer , m_BufferSize , m_StackTop , realSize );

			//�g�p�̈�������߂�
			m_StackTop += realSize;

			//�w�b�_�[�̕ҏW
			pHeader = ( CAllocatorDataHeader * )pAllocMem;
			pHeader->m_CheckBreakId = ALLOCATE_DATA_ID;
			pHeader->m_eDirType = eAllocDirectionType_Flont;
			pHeader->m_eSizeType = eSize;
			pHeader->m_Size = size;
			pHeader->m_StackNo = m_StackNo;
			pHeader->m_Line = line;
			pHeader->m_AllocatorId = m_AllocatorId;
			pHeader->m_IsUsed = skyTrue;
			pHeader->m_pAllocator = this;
			pHeader->SetFileName( pFname );
			pHeader->m_eAllocType = m_eType;
			pHeader->m_MargeCount = 1; //�ŏ��͕K���P

			m_StackNo++;

			//�t�b�^�[�̕ҏW
			CAllocatorDataFooter *pFooter = ( CAllocatorDataFooter * )( pAllocMem + sizeof( CAllocatorDataHeader ) + size  );
			pFooter->m_CheckBreakId = ALLOCATE_DATA_ID;

			//�X�^�b�N���X�g
			m_StackList.Link( pHeader );

			//�g�p�̈悾���Ԃ�
			pResult = pAllocMem + sizeof( CAllocatorDataHeader );
		}
		else
		{
			SKY_PANIC_MSG( _T( "The memory doesn't suffice. \n" ) );
		}
	}
	return pResult;
}

inline void *CStackAllocator::BackMalloc( u32 size , const skyMString *pFname , s32 line , eAllocSizeType eSize )
{
//	SKY_PRINTF( "AllocatorNo = %d \n" , m_AllocatorId );

	SKY_UNUSED_ARG( pFname );

	u8 *pResult = skyNull;
	CAllocatorDataHeader *pHeader = skyNull;

	skyBool isFound = skyFalse;
	if ( eSize == eAllocSizeType_Block && m_BackBlockDeleteList.GetNum() > 0 && size <= m_BackBlockSize )
	{
		isFound = skyTrue;

		//�폜���X�g�������
		pHeader = ( CAllocatorDataHeader * )( ( u8 * )m_BackBlockDeleteList.IdxFromTail( 0 ) - sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDeleteDataHeader ) );
		m_BackBlockDeleteList.UnLink( ( CAllocatorDeleteDataHeader * )&pHeader->m_DeleteList );
		m_BackDeleteListMemorySize -= pHeader->m_Size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );
		pResult = ( u8 * )pHeader + sizeof( CAllocatorDataHeader );
		pHeader->m_IsUsed = skyTrue;
		pHeader->m_pAllocator = this;
	}
	else if ( m_BackDeleteList.GetNum() > 0 )
	{//�폜���X�g����m��

		//���`�T���ō폜���X�g��T�����A�m�ۉ\�ȃT�C�Y�����݂��邩�ǂ������肷��
		pHeader = ( CAllocatorDataHeader * )( ( u8 * )m_BackDeleteList.IdxFromTail( 0 ) - sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDeleteDataHeader ) );
		while( pHeader != skyNull )
		{
			if ( pHeader->m_Size >= size )
			{
				isFound = skyTrue;

				//�K�v�ȕ��������o��
				pHeader = MallocFromMallocData( pHeader , size );

				//�폜���X�g�������
				m_BackDeleteList.UnLink( ( CAllocatorDeleteDataHeader * )&pHeader->m_DeleteList );
				m_BackDeleteListMemorySize -= pHeader->m_Size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );
				pResult = ( u8 * )pHeader + sizeof( CAllocatorDataHeader );
				pHeader->m_IsUsed = skyTrue;
				pHeader->m_pAllocator = this;

				break;
			}

			if ( pHeader->m_DeleteList.m_pPrev == skyNull )
			{
				pHeader = skyNull;
			}
			else
			{
				pHeader = ( CAllocatorDataHeader * )( ( u8 * )pHeader->m_DeleteList.m_pPrev - sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDeleteDataHeader ) );
			}
		}
	}

	if ( isFound == skyFalse )
	{
		//�u���b�N�T�C�Y�ɒ���
		if ( eSize == eAllocSizeType_Block && size <= m_BackBlockSize )
		{
			//�傫�߂̑傫���ɕ␳
			size = m_BlockSize;
		}
		else
		{
			//�u���b�N�m�ۂ��ł��Ȃ��̂ŃT�C�Y�^�C�v��␳
			eSize = eAllocSizeType_Size;
		}

		//���ۂ̊m�ۗ̈�v�Z
		u32 realSize = size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );

		//�K�v�̈�`�F�b�N
		if ( isFound == skyFalse && m_StackTop <= m_BackStackTop - realSize )
		{

			//�̈�m��
			u8 *pAllocMem = ( u8 * )CMemoryUtil::BackMallocPointer( m_pBuffer , m_BufferSize , m_BufferSize - m_BackStackTop , realSize );

			//�g�p�̈�������߂�
			m_BackStackTop -= realSize;

			//�w�b�_�[�̕ҏW
			pHeader = ( CAllocatorDataHeader * )pAllocMem;
			pHeader->m_CheckBreakId = ALLOCATE_DATA_ID;
			pHeader->m_eDirType = eAllocDirectionType_Back;
			pHeader->m_eSizeType = eSize;
			pHeader->m_Size = size;
			pHeader->m_StackNo = m_BackStackNo;
			pHeader->m_Line = line;
			pHeader->m_AllocatorId = m_AllocatorId;
			pHeader->m_IsUsed = skyTrue;
			pHeader->m_pAllocator = this;
			pHeader->SetFileName( pFname );
			pHeader->m_eAllocType = m_eType;
			pHeader->m_MargeCount = 1; //�ŏ��͕K���P

			m_BackStackNo++;

			//�t�b�^�[�̕ҏW
			CAllocatorDataFooter *pFooter = ( CAllocatorDataFooter * )( pAllocMem + sizeof( CAllocatorDataHeader ) + size  );
			pFooter->m_CheckBreakId = ALLOCATE_DATA_ID;

			//�X�^�b�N���X�g
			m_BackStackList.Link( pHeader );

			//�g�p�̈悾���Ԃ�
			pResult = pAllocMem + sizeof( CAllocatorDataHeader );
		}
		else
		{
			SKY_PANIC_MSG( _T( "The memory doesn't suffice. \n" ) );
		}
	}
	return pResult;
}

inline void *CStackAllocator::Realloc( void *pData , u32 size , const skyMString *pFname , s32 line )
{
	u8 *pResult = ( u8 * )pData;

	//���g�p�̈悪�n���ꂽ��̈���m�ۂ��ĕԂ�
	if ( pResult == skyNull )
	{
		return Malloc( size , pFname , line );
	}

	CAllocatorDataHeader *pHeader = ( CAllocatorDataHeader * )( ( u8 * )pData - sizeof( CAllocatorDataHeader ) );
	if ( pHeader->m_Size != size )
	{
		//�Â��̈���폜����i���̃A���P�[�^�[�̍\����폜����Ă��̈�̃f�[�^�͂����ɂ͏����Ȃ��B��ɍ폜���邱�ƂŒf�Љ���h���j
		Delete( pData );

		u32 copySize = 0;
		if ( pHeader->m_Size > size )
		{
			copySize = size;
		}
		else
		{
			copySize = pHeader->m_Size;
		}

		u8 *pNewData = skyNull;
		if ( pHeader->m_eDirType == eAllocDirectionType_Flont )
		{
			pNewData = ( u8 * )Malloc( size , pFname , line );
		}
		else
		{
			pNewData = ( u8 * )BackMalloc( size , pFname , line );
		}

		//�V�����̈�ɌÂ��̈�̒l���R�s�[
		CMemoryUtil_::Copy( pNewData , pData , copySize );

		//�V�����̈��Ԃ�
		pResult = pNewData;

	}
	return pResult;
}

inline void CStackAllocator::Delete( void *pData )
{
//	SKY_PRINTF( "AllocatorNo = %d \n" , m_AllocatorId );

	//�j��`�F�b�N
	CAllocatorDataHeader *pHeader = ( CAllocatorDataHeader * )( ( u8 * )pData - sizeof( CAllocatorDataHeader ) );
	CAllocatorDataFooter *pFooter = ( CAllocatorDataFooter * )( ( u8 * )pData + pHeader->m_Size );
    SKY_UNUSED_ARG( pFooter );
	SKY_ASSERT_MSG( pHeader->m_CheckBreakId == ALLOCATE_DATA_ID && pFooter->m_CheckBreakId == ALLOCATE_DATA_ID , _T( "The value of the memory is illegal. " ) ); //���������󂳂�Ă��邩�A �z��NEW�����̈��DEL�������iDEL_ARRAY�łȂ���΂Ȃ�Ȃ��jNEW_NATIVE�Ŋm�ۂ����̈��delete���܂����i���m�ɂ�DEL_NATIVE���g�p���Ă��������j
	SKY_ASSERT_MSG( pHeader->m_AllocatorId == m_AllocatorId , _T( "AllocatorId is invalid. \n" ) ); //�A���P�[�^�[���Ⴂ�܂��B
	CAllocatorDataHeader *pPrev = skyNull;
	if ( pHeader->m_eDirType == eAllocDirectionType_Flont )
	{
		if ( pHeader->m_eSizeType == eAllocSizeType_Block )
		{
			pHeader->m_pAllocator = skyNull;
			pHeader->m_IsUsed = skyFalse;
			m_BlockDeleteList.Link( ( CAllocatorDeleteDataHeader * )&pHeader->m_DeleteList );
			m_DeleteListMemorySize += pHeader->m_Size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );
		}
		//�X�^�b�N�̐擪�Ȃ�΃X�^�b�N�̐擪�l��������
		else if ( pHeader == m_StackList.IdxFromTail( 0 ) )
		{
			pHeader->m_pAllocator = skyNull;
			m_StackList.UnLink( pHeader );
			m_StackTop -= pHeader->m_Size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );
			m_StackNo -= pHeader->m_MargeCount;

			//��������폜���X�g�Ɏ����̉����Ȃ������ׂ�
			pPrev = pHeader->m_pPrev;
			while( pPrev != skyNull )
			{
				pHeader = pPrev;
				if ( pHeader->m_IsUsed == skyFalse )
				{
					m_StackTop -= pHeader->m_Size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );
					m_StackNo -= pHeader->m_MargeCount;;
					pHeader->m_pAllocator = skyNull;
					m_StackList.UnLink( pHeader );

					//�K���폜���X�g�ɋ���̂ŁA�폜����
					if ( pHeader->m_eSizeType == eAllocSizeType_Size )
					{
						m_DeleteList.UnLink( ( CAllocatorDeleteDataHeader * )&pHeader->m_DeleteList );
					}
					else if ( pHeader->m_eSizeType == eAllocSizeType_Block )
					{
						m_BlockDeleteList.UnLink( ( CAllocatorDeleteDataHeader * )&pHeader->m_DeleteList );
					}

					m_DeleteListMemorySize -= pHeader->m_Size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );
					if ( m_StackList.GetNum() > 0 )
					{
						pPrev = m_StackList.IdxFromTail( 0 );
					}
					else
					{
						pPrev = pHeader->m_pPrev;
					}
				}
				else
				{
					break;
				}
			}
		}
		//�X�^�b�N�̐擪����Ȃ���΍폜���X�g�ɓo�^
		else
		{
			pHeader->m_pAllocator = skyNull;
			pHeader->m_IsUsed = skyFalse;
			m_DeleteList.Link( ( CAllocatorDeleteDataHeader * )&pHeader->m_DeleteList );
			m_DeleteListMemorySize += pHeader->m_Size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );

			MargeMemoryArea( pHeader );
		}
	}
	else
	{
		if ( pHeader->m_eSizeType == eAllocSizeType_Block )
		{
			pHeader->m_pAllocator = skyNull;
			pHeader->m_IsUsed = skyFalse;
			m_BackBlockDeleteList.Link( ( CAllocatorDeleteDataHeader * )&pHeader->m_DeleteList );
			m_BackDeleteListMemorySize += pHeader->m_Size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );
		}
		else if ( pHeader == m_BackStackList.IdxFromTail( 0 ) )
		{
			pHeader->m_pAllocator = skyNull;
			m_BackStackList.UnLink( pHeader );
			m_BackStackTop += pHeader->m_Size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );
			m_BackStackNo -= pHeader->m_MargeCount;

			//��������폜���X�g�Ɏ����̉����Ȃ������ׂ�
			pPrev = pHeader->m_pPrev;
			while( pPrev != skyNull )
			{
				if ( pHeader->m_IsUsed == skyFalse )
				{
					m_BackStackTop += pHeader->m_Size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );
					m_BackStackNo -= pHeader->m_MargeCount;
					pHeader->m_pAllocator = skyNull;
					m_BackStackList.UnLink( pHeader );

					//�K���폜���X�g�ɋ���̂ŁA�폜����
					if ( pHeader->m_eSizeType == eAllocSizeType_Size )
					{
						m_BackDeleteList.UnLink( ( CAllocatorDeleteDataHeader * )&pHeader->m_DeleteList );
					}
					else if ( pHeader->m_eSizeType == eAllocSizeType_Block )
					{
						m_BackBlockDeleteList.UnLink( ( CAllocatorDeleteDataHeader * )&pHeader->m_DeleteList );
					}

					m_BackDeleteListMemorySize -= pHeader->m_Size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );
					if ( m_BackStackList.GetNum() > 0 )
					{
						pPrev = m_BackStackList.IdxFromTail( 0 );
					}
					else
					{
						pPrev = pHeader->m_pPrev;
					}
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			pHeader->m_pAllocator = skyNull;
			pHeader->m_IsUsed = skyFalse;
			m_BackDeleteList.Link( ( CAllocatorDeleteDataHeader * )&pHeader->m_DeleteList );
			m_BackDeleteListMemorySize += pHeader->m_Size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );

			MargeMemoryArea( pHeader );
		}
	}
}

inline void CStackAllocator::Clear()
{
	m_StackTop = 0;
	m_StackNo = 0;
	m_BackStackTop = m_BufferSize;
	m_BackStackNo = 0;
}

inline void CStackAllocator::AssertCheckMemory( void *pData )
{
	CAllocatorDataHeader *pHeader = ( CAllocatorDataHeader * )( ( u8 * )pData - sizeof( CAllocatorDataHeader ) );
	CAllocatorDataFooter *pFooter = ( CAllocatorDataFooter * )( ( u8 * )pData + pHeader->m_Size );
    SKY_UNUSED_ARG( pFooter );
	SKY_ASSERT_MSG( pHeader->m_CheckBreakId == ALLOCATE_DATA_ID && pFooter->m_CheckBreakId == ALLOCATE_DATA_ID , _T( "The value of the memory is illegal. " ) ); //���������󂳂�Ă��邩�A �z��NEW�����̈��DEL�������iDEL_ARRAY�łȂ���΂Ȃ�Ȃ��jNEW_NATIVE�Ŋm�ۂ����̈��delete���܂����i���m�ɂ�DEL_NATIVE���g�p���Ă��������j
	SKY_ASSERT_MSG( pHeader->m_AllocatorId == m_AllocatorId , _T( "AllocatorId is invalid. \n" ) ); //�A���P�[�^�[���Ⴂ�܂��B
}

inline skyBool CStackAllocator::CheckAdr( void *pAdr ) const
{
	u32 adr = ( u32 )pAdr;
	u32 top = ( u32 )m_pBuffer;
	u32 bottom = ( u32 )m_pBuffer + m_BufferSize;

	return ( top <= adr && bottom >= adr )? skyTrue : skyFalse;
}

inline void *CStackAllocator::Malloc( u32 size ,  s32 groundHndl , s32 arenaHndl , eAllocDirectionType eAreaType , const skyMString *pFname , s32 line )
{
	SKY_UNUSED_ARG( groundHndl );
	SKY_UNUSED_ARG( arenaHndl );

	switch ( eAreaType )
	{
	case eAllocDirectionType_Back :
		return BackMalloc( size , pFname , line );

	default :
		return Malloc( size , pFname , line );
	}
}

inline void *CStackAllocator::Realloc( void *pData , u32 size , s32 groundHndl , s32 arenaHndl , eAllocDirectionType eAreaType , const skyMString *pFname , s32 line )
{
	SKY_UNUSED_ARG( groundHndl );
	SKY_UNUSED_ARG( arenaHndl );
	SKY_UNUSED_ARG( eAreaType );

	return Realloc( pData , size , pFname , line );
}

inline u32 CStackAllocator::GetAllocatorId()
{ 
	return m_AllocatorId;
}

inline void CStackAllocator::MargeMemoryArea( CAllocatorDataHeader *pHeader )
{
	//�g�p���������珈�������
	if ( pHeader->m_IsUsed == skyTrue ) return;

	//�u���b�N�A���b�N��������I���
	if ( pHeader->m_eSizeType == eAllocSizeType_Block  == skyTrue ) return;

	skyBool isMargeExec = skyTrue;
	CAllocatorDataHeader *pNext = skyNull; //�O���X�^�b�N�̏ꍇ�͎��̃��������}�[�W����
	CAllocatorDataHeader *pPrev = skyNull; //����X�^�b�N�̏ꍇ�͑O�̃��������}�[�W����
	while( isMargeExec )
	{
		if ( pHeader->m_eDirType == eAllocDirectionType_Flont )
		{
			//���̗̈���擾
			pNext = pHeader->m_pNext;

			//�����Ȃ���ΏI���
			if ( pNext == skyNull )
			{
				break;
			}

			//�����̎��̗̈悪�g�p����������Ԃ��i�}�[�W�ł��Ȃ��̂Łj
			if ( pNext->m_IsUsed == skyTrue )
			{
				break;
			}

			//�������g��������I���
			if ( pHeader == pNext )
			{
				break;
			}

			//�����̎g�p�̈�Ɏ��̃A���P�[�g���̃w�b�_�[�T�C�Y�{�Ǘ��̈�{�t�b�^�[�T�C�Y�𑫂��B
			pHeader->m_Size += sizeof( CAllocatorDataHeader ) + pNext->m_Size + sizeof( CAllocatorDataFooter );

			//�����̎��̗̈���X�^�b�N���X�g�������
	        m_StackList.UnLink( pNext );

			//�����̎��̗̈���폜���X�g�������
	        m_DeleteList.UnLink( ( CAllocatorDeleteDataHeader * )&pNext->m_DeleteList );
		}
		else
		{
			//�O�̗̈���擾
			pPrev = pHeader->m_pPrev;

			//�O���Ȃ���ΏI���
			if ( pPrev == skyNull )
			{
				break;
			}

			//�����̑O�̗̈悪�g�p����������Ԃ��i�}�[�W�ł��Ȃ��̂Łj
			if ( pPrev->m_IsUsed == skyTrue )
			{
				break;
			}

			//�������g��������I���
			if ( pHeader == pPrev )
			{
				break;
			}

			//�����̎g�p�̈�Ɏ��̃A���P�[�g���̃w�b�_�[�T�C�Y�{�Ǘ��̈�{�t�b�^�[�T�C�Y�𑫂��B
			pHeader->m_Size += sizeof( CAllocatorDataHeader ) + pPrev->m_Size + sizeof( CAllocatorDataFooter );

			//�����̑O�̗̈���X�^�b�N���X�g�������
	        m_BackStackList.UnLink( pPrev );

			//�����̑O�̗̈���폜���X�g�������
	        m_BackDeleteList.UnLink( ( CAllocatorDeleteDataHeader * )&pPrev->m_DeleteList );
		}

		//�}�[�W�J�E���g���J�E���g�A�b�v
		pHeader->m_MargeCount++;
	}
}

inline CAllocatorDataHeader *CStackAllocator::MallocFromMallocData( CAllocatorDataHeader *pMemory , u32 size )
{
	SKY_ASSERT_MSG( pMemory->m_IsUsed == skyFalse , _T( "It is using it. \n" ) );
	SKY_ASSERT_MSG( pMemory->m_Size >= size , _T( "The memory doesn't suffice. \n" ) );
	SKY_ASSERT_MSG( pMemory->m_eSizeType != eAllocSizeType_Block , _T( "Block alloc is invalid. \n" ) );

	CAllocatorDataHeader *pResult = pMemory;

	//���������T�C�Y�Ȃ炻�̂܂܋A��
	if ( pMemory->m_Size > size )
	{
		u32 reqSize = size + 1 + ( sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter ) ) * 2;   //2�����ɍŒ���K�v�ȃT�C�Y(�c�������͍Œ��1�o�C�g����΂����j
		u32 nowSize = pMemory->m_Size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );    //���݂̃T�C�Y

		//�w�b�_�[���t�b�^�[���܂߂ĕ�������T�C�Y�����邩�i�Ȃ���΂�����Ɨ]���ł����̂܂ܕԂ�
		if ( reqSize <= nowSize )
		{//�����\

			//�I���W�i���̃T�C�Y���Ƃ��Ă���
			u32 originalSize = pMemory->m_Size;

			//�������̃T�C�Y��������
			pMemory->m_Size = size;

			//�t�b�^�[�����蒼��
			CAllocatorDataFooter *pFooter = ( CAllocatorDataFooter * )( ( u8 * )pMemory + ( sizeof( CAllocatorDataHeader ) + pMemory->m_Size ) );
			pFooter->m_CheckBreakId = ALLOCATE_DATA_ID;

			//�V�����̈�̃w�b�_�[�ƃt�b�^�[�����
			//�w�b�_�[�̕ҏW
			CAllocatorDataHeader *pHeader = ( CAllocatorDataHeader * )( ( u8 * )pFooter + sizeof( CAllocatorDataFooter ) );
			pHeader->m_CheckBreakId = ALLOCATE_DATA_ID;
			pHeader->m_eDirType     = pMemory->m_eDirType;
			pHeader->m_eSizeType    = pMemory->m_eSizeType;
			pHeader->m_Size         = originalSize - size - sizeof( CAllocatorDataHeader ) - sizeof( CAllocatorDataFooter );
			pHeader->m_StackNo      = pMemory->m_StackNo;
			pHeader->m_Line         = pMemory->m_Line;
			pHeader->m_AllocatorId  = pMemory->m_AllocatorId;
			pHeader->m_IsUsed       = skyFalse;
			pHeader->m_pAllocator   = skyNull;
			pHeader->SetFileName( "deleteList" );
			pHeader->m_eAllocType   = pMemory->m_eAllocType;
			pHeader->m_MargeCount   = 1; //�ŏ��͕K���P

			if ( pMemory->m_eDirType == eAllocDirectionType_Flont )
			{
				//�X�^�b�N���X�g�ɒǉ�
				m_StackList.LinkItemNext( pMemory , pHeader );

				//�폜���X�g�ɒǉ�
				m_DeleteList.Link( ( CAllocatorDeleteDataHeader * )&pHeader->m_DeleteList );
			}
			else
			{
				//�X�^�b�N���X�g�ɒǉ�
				m_BackStackList.LinkItemPrev( pMemory , pHeader );

				//�폜���X�g�ɒǉ�
				m_BackDeleteList.Link( ( CAllocatorDeleteDataHeader * )&pHeader->m_DeleteList );
			}
		}
	}
	return pResult;
}

inline void CStackAllocator::PrintMemory()
{
#ifdef SW_SKYLIB_DEBUG_ON
	CAllocatorDataHeader *pHeader = skyNull;

	//�O���X�^�b�N�̔j�󌟍�
	pHeader = m_StackList.IdxFromTail( 0 );
	while( pHeader != skyNull )
	{
		SKY_PRINTF( "%d F%d %d %s %d %d \n" , pHeader->m_AllocatorId , pHeader->m_StackNo , pHeader->m_IsUsed , pHeader->m_Path , pHeader->m_Line , pHeader->m_Size );
		pHeader = pHeader->m_pPrev;
	}

	//����X�^�b�N�̔j�󌟍�
	pHeader = m_BackStackList.IdxFromTail( 0 );
	while( pHeader != skyNull )
	{
		SKY_PRINTF( "%d B%d %d %s %d %d \n" , pHeader->m_AllocatorId , pHeader->m_StackNo , pHeader->m_IsUsed , pHeader->m_Path , pHeader->m_Line , pHeader->m_Size );
		pHeader = pHeader->m_pPrev;
	}
#endif
}

inline void CStackAllocator::CheckMemBreak()
{
#ifdef SW_SKYLIB_DEBUG_ON
	CAllocatorDataHeader *pHeader = skyNull;
	CAllocatorDataFooter *pFooter = skyNull;

	//�O���X�^�b�N�̔j�󌟍�
	pHeader = m_StackList.IdxFromTail( 0 );
	while( pHeader != skyNull )
	{
        pFooter = ( CAllocatorDataFooter * )( ( u8 * )pHeader + sizeof( CAllocatorDataHeader ) + pHeader->m_Size );
		SKY_ASSERT_MSG( pHeader->m_CheckBreakId == ALLOCATE_DATA_ID && pFooter->m_CheckBreakId == ALLOCATE_DATA_ID , _T( "The value of the memory is illegal. " ) ); //���������󂳂�Ă��邩�A �z��NEW�����̈��DEL�������iDEL_ARRAY�łȂ���΂Ȃ�Ȃ��jNEW_NATIVE�Ŋm�ۂ����̈��delete���܂����i���m�ɂ�DEL_NATIVE���g�p���Ă��������j
		pHeader = pHeader->m_pPrev;
	}

	//����X�^�b�N�̔j�󌟍�
	pHeader = m_BackStackList.IdxFromTail( 0 );
	while( pHeader != skyNull )
	{
        pFooter = ( CAllocatorDataFooter * )( ( u8 * )pHeader + sizeof( CAllocatorDataHeader ) + pHeader->m_Size );
		SKY_ASSERT_MSG( pHeader->m_CheckBreakId == ALLOCATE_DATA_ID && pFooter->m_CheckBreakId == ALLOCATE_DATA_ID , _T( "The value of the memory is illegal. " ) ); //���������󂳂�Ă��邩�A �z��NEW�����̈��DEL�������iDEL_ARRAY�łȂ���΂Ȃ�Ȃ��jNEW_NATIVE�Ŋm�ۂ����̈��delete���܂����i���m�ɂ�DEL_NATIVE���g�p���Ă��������j
		pHeader = pHeader->m_pPrev;
	}
#endif
}

} } }