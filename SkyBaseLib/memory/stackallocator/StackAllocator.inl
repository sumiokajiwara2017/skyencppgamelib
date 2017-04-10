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

	//メモリから確保する
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
	//メモリから消す
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

		//削除リストから消す
		pHeader = ( CAllocatorDataHeader * )( ( u8 * )m_BlockDeleteList.IdxFromTail( 0 ) - sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDeleteDataHeader ) );
		m_BlockDeleteList.UnLink( ( CAllocatorDeleteDataHeader * )&pHeader->m_DeleteList );
		m_DeleteListMemorySize -= pHeader->m_Size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );
		pResult = ( u8 * )pHeader + sizeof( CAllocatorDataHeader );
		pHeader->m_IsUsed = skyTrue;
		pHeader->m_pAllocator = this;
	}
	else if ( m_DeleteList.GetNum() > 0 )
	{//削除リストから確保

		//線形探査で削除リストを探査し、確保可能なサイズが存在するかどうか判定する
		pHeader = ( CAllocatorDataHeader * )( ( u8 * )m_DeleteList.IdxFromTail( 0 ) - sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDeleteDataHeader ) );
		while( pHeader != skyNull )
		{
			if ( pHeader->m_Size >= size )
			{
				isFound = skyTrue;

				//必要な分だけ取り出す
				pHeader = MallocFromMallocData( pHeader , size );

				//削除リストから消す
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
		//ブロックサイズに調整
		if ( eSize == eAllocSizeType_Block && size <= m_BlockSize )
		{
			//大きめの大きさに補正
			size = m_BlockSize;
		}
		else
		{
			//ブロック確保ができないのでサイズタイプを補正
			eSize = eAllocSizeType_Size;
		}

		//実際の確保領域計算
		u32 realSize = size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );

		//必要領域チェック
		if ( realSize + m_StackTop <= m_BackStackTop )
		{
			//領域確保
			u8 *pAllocMem = ( u8 * )CMemoryUtil::MallocPointer( m_pBuffer , m_BufferSize , m_StackTop , realSize );

			//使用領域をすすめる
			m_StackTop += realSize;

			//ヘッダーの編集
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
			pHeader->m_MargeCount = 1; //最初は必ず１

			m_StackNo++;

			//フッターの編集
			CAllocatorDataFooter *pFooter = ( CAllocatorDataFooter * )( pAllocMem + sizeof( CAllocatorDataHeader ) + size  );
			pFooter->m_CheckBreakId = ALLOCATE_DATA_ID;

			//スタックリスト
			m_StackList.Link( pHeader );

			//使用領域だけ返す
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

		//削除リストから消す
		pHeader = ( CAllocatorDataHeader * )( ( u8 * )m_BackBlockDeleteList.IdxFromTail( 0 ) - sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDeleteDataHeader ) );
		m_BackBlockDeleteList.UnLink( ( CAllocatorDeleteDataHeader * )&pHeader->m_DeleteList );
		m_BackDeleteListMemorySize -= pHeader->m_Size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );
		pResult = ( u8 * )pHeader + sizeof( CAllocatorDataHeader );
		pHeader->m_IsUsed = skyTrue;
		pHeader->m_pAllocator = this;
	}
	else if ( m_BackDeleteList.GetNum() > 0 )
	{//削除リストから確保

		//線形探査で削除リストを探査し、確保可能なサイズが存在するかどうか判定する
		pHeader = ( CAllocatorDataHeader * )( ( u8 * )m_BackDeleteList.IdxFromTail( 0 ) - sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDeleteDataHeader ) );
		while( pHeader != skyNull )
		{
			if ( pHeader->m_Size >= size )
			{
				isFound = skyTrue;

				//必要な分だけ取り出す
				pHeader = MallocFromMallocData( pHeader , size );

				//削除リストから消す
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
		//ブロックサイズに調整
		if ( eSize == eAllocSizeType_Block && size <= m_BackBlockSize )
		{
			//大きめの大きさに補正
			size = m_BlockSize;
		}
		else
		{
			//ブロック確保ができないのでサイズタイプを補正
			eSize = eAllocSizeType_Size;
		}

		//実際の確保領域計算
		u32 realSize = size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );

		//必要領域チェック
		if ( isFound == skyFalse && m_StackTop <= m_BackStackTop - realSize )
		{

			//領域確保
			u8 *pAllocMem = ( u8 * )CMemoryUtil::BackMallocPointer( m_pBuffer , m_BufferSize , m_BufferSize - m_BackStackTop , realSize );

			//使用領域をすすめる
			m_BackStackTop -= realSize;

			//ヘッダーの編集
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
			pHeader->m_MargeCount = 1; //最初は必ず１

			m_BackStackNo++;

			//フッターの編集
			CAllocatorDataFooter *pFooter = ( CAllocatorDataFooter * )( pAllocMem + sizeof( CAllocatorDataHeader ) + size  );
			pFooter->m_CheckBreakId = ALLOCATE_DATA_ID;

			//スタックリスト
			m_BackStackList.Link( pHeader );

			//使用領域だけ返す
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

	//未使用領域が渡されたら領域を確保して返す
	if ( pResult == skyNull )
	{
		return Malloc( size , pFname , line );
	}

	CAllocatorDataHeader *pHeader = ( CAllocatorDataHeader * )( ( u8 * )pData - sizeof( CAllocatorDataHeader ) );
	if ( pHeader->m_Size != size )
	{
		//古い領域を削除する（このアロケーターの構造上削除されても領域のデータはすぐには消えない。先に削除することで断片化を防ぐ）
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

		//新しい領域に古い領域の値をコピー
		CMemoryUtil_::Copy( pNewData , pData , copySize );

		//新しい領域を返す
		pResult = pNewData;

	}
	return pResult;
}

inline void CStackAllocator::Delete( void *pData )
{
//	SKY_PRINTF( "AllocatorNo = %d \n" , m_AllocatorId );

	//破壊チェック
	CAllocatorDataHeader *pHeader = ( CAllocatorDataHeader * )( ( u8 * )pData - sizeof( CAllocatorDataHeader ) );
	CAllocatorDataFooter *pFooter = ( CAllocatorDataFooter * )( ( u8 * )pData + pHeader->m_Size );
    SKY_UNUSED_ARG( pFooter );
	SKY_ASSERT_MSG( pHeader->m_CheckBreakId == ALLOCATE_DATA_ID && pFooter->m_CheckBreakId == ALLOCATE_DATA_ID , _T( "The value of the memory is illegal. " ) ); //メモリが壊されているか、 配列NEWした領域をDELしたか（DEL_ARRAYでなければならない）NEW_NATIVEで確保した領域をdeleteしました（正確にはDEL_NATIVEを使用してください）
	SKY_ASSERT_MSG( pHeader->m_AllocatorId == m_AllocatorId , _T( "AllocatorId is invalid. \n" ) ); //アロケーターが違います。
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
		//スタックの先頭ならばスタックの先頭値を下げる
		else if ( pHeader == m_StackList.IdxFromTail( 0 ) )
		{
			pHeader->m_pAllocator = skyNull;
			m_StackList.UnLink( pHeader );
			m_StackTop -= pHeader->m_Size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );
			m_StackNo -= pHeader->m_MargeCount;

			//下げた後削除リストに自分の下がないか調べる
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

					//必ず削除リストに居るので、削除する
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
		//スタックの先頭じゃなければ削除リストに登録
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

			//下げた後削除リストに自分の下がないか調べる
			pPrev = pHeader->m_pPrev;
			while( pPrev != skyNull )
			{
				if ( pHeader->m_IsUsed == skyFalse )
				{
					m_BackStackTop += pHeader->m_Size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );
					m_BackStackNo -= pHeader->m_MargeCount;
					pHeader->m_pAllocator = skyNull;
					m_BackStackList.UnLink( pHeader );

					//必ず削除リストに居るので、削除する
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
	SKY_ASSERT_MSG( pHeader->m_CheckBreakId == ALLOCATE_DATA_ID && pFooter->m_CheckBreakId == ALLOCATE_DATA_ID , _T( "The value of the memory is illegal. " ) ); //メモリが壊されているか、 配列NEWした領域をDELしたか（DEL_ARRAYでなければならない）NEW_NATIVEで確保した領域をdeleteしました（正確にはDEL_NATIVEを使用してください）
	SKY_ASSERT_MSG( pHeader->m_AllocatorId == m_AllocatorId , _T( "AllocatorId is invalid. \n" ) ); //アロケーターが違います。
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
	//使用中だったら処理おわり
	if ( pHeader->m_IsUsed == skyTrue ) return;

	//ブロックアロックだったら終わり
	if ( pHeader->m_eSizeType == eAllocSizeType_Block  == skyTrue ) return;

	skyBool isMargeExec = skyTrue;
	CAllocatorDataHeader *pNext = skyNull; //前方スタックの場合は次のメモリをマージする
	CAllocatorDataHeader *pPrev = skyNull; //後方スタックの場合は前のメモリをマージする
	while( isMargeExec )
	{
		if ( pHeader->m_eDirType == eAllocDirectionType_Flont )
		{
			//次の領域を取得
			pNext = pHeader->m_pNext;

			//次がなければ終わり
			if ( pNext == skyNull )
			{
				break;
			}

			//自分の次の領域が使用中だったら返す（マージできないので）
			if ( pNext->m_IsUsed == skyTrue )
			{
				break;
			}

			//自分自身だったら終わり
			if ( pHeader == pNext )
			{
				break;
			}

			//自分の使用領域に次のアロケート情報のヘッダーサイズ＋管理領域＋フッターサイズを足す。
			pHeader->m_Size += sizeof( CAllocatorDataHeader ) + pNext->m_Size + sizeof( CAllocatorDataFooter );

			//自分の次の領域をスタックリストから消す
	        m_StackList.UnLink( pNext );

			//自分の次の領域を削除リストから消す
	        m_DeleteList.UnLink( ( CAllocatorDeleteDataHeader * )&pNext->m_DeleteList );
		}
		else
		{
			//前の領域を取得
			pPrev = pHeader->m_pPrev;

			//前がなければ終わり
			if ( pPrev == skyNull )
			{
				break;
			}

			//自分の前の領域が使用中だったら返す（マージできないので）
			if ( pPrev->m_IsUsed == skyTrue )
			{
				break;
			}

			//自分自身だったら終わり
			if ( pHeader == pPrev )
			{
				break;
			}

			//自分の使用領域に次のアロケート情報のヘッダーサイズ＋管理領域＋フッターサイズを足す。
			pHeader->m_Size += sizeof( CAllocatorDataHeader ) + pPrev->m_Size + sizeof( CAllocatorDataFooter );

			//自分の前の領域をスタックリストから消す
	        m_BackStackList.UnLink( pPrev );

			//自分の前の領域を削除リストから消す
	        m_BackDeleteList.UnLink( ( CAllocatorDeleteDataHeader * )&pPrev->m_DeleteList );
		}

		//マージカウントをカウントアップ
		pHeader->m_MargeCount++;
	}
}

inline CAllocatorDataHeader *CStackAllocator::MallocFromMallocData( CAllocatorDataHeader *pMemory , u32 size )
{
	SKY_ASSERT_MSG( pMemory->m_IsUsed == skyFalse , _T( "It is using it. \n" ) );
	SKY_ASSERT_MSG( pMemory->m_Size >= size , _T( "The memory doesn't suffice. \n" ) );
	SKY_ASSERT_MSG( pMemory->m_eSizeType != eAllocSizeType_Block , _T( "Block alloc is invalid. \n" ) );

	CAllocatorDataHeader *pResult = pMemory;

	//もし同じサイズならそのまま帰す
	if ( pMemory->m_Size > size )
	{
		u32 reqSize = size + 1 + ( sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter ) ) * 2;   //2分割に最低限必要なサイズ(残った方は最低限1バイトあればいい）
		u32 nowSize = pMemory->m_Size + sizeof( CAllocatorDataHeader ) + sizeof( CAllocatorDataFooter );    //現在のサイズ

		//ヘッダーもフッターも含めて分割するサイズがあるか（なければちょっと余分でもそのまま返す
		if ( reqSize <= nowSize )
		{//分割可能

			//オリジナルのサイズをとっておく
			u32 originalSize = pMemory->m_Size;

			//メモリのサイズを代入する
			pMemory->m_Size = size;

			//フッターをつくり直す
			CAllocatorDataFooter *pFooter = ( CAllocatorDataFooter * )( ( u8 * )pMemory + ( sizeof( CAllocatorDataHeader ) + pMemory->m_Size ) );
			pFooter->m_CheckBreakId = ALLOCATE_DATA_ID;

			//新しい領域のヘッダーとフッターを作る
			//ヘッダーの編集
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
			pHeader->m_MargeCount   = 1; //最初は必ず１

			if ( pMemory->m_eDirType == eAllocDirectionType_Flont )
			{
				//スタックリストに追加
				m_StackList.LinkItemNext( pMemory , pHeader );

				//削除リストに追加
				m_DeleteList.Link( ( CAllocatorDeleteDataHeader * )&pHeader->m_DeleteList );
			}
			else
			{
				//スタックリストに追加
				m_BackStackList.LinkItemPrev( pMemory , pHeader );

				//削除リストに追加
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

	//前方スタックの破壊検査
	pHeader = m_StackList.IdxFromTail( 0 );
	while( pHeader != skyNull )
	{
		SKY_PRINTF( "%d F%d %d %s %d %d \n" , pHeader->m_AllocatorId , pHeader->m_StackNo , pHeader->m_IsUsed , pHeader->m_Path , pHeader->m_Line , pHeader->m_Size );
		pHeader = pHeader->m_pPrev;
	}

	//後方スタックの破壊検査
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

	//前方スタックの破壊検査
	pHeader = m_StackList.IdxFromTail( 0 );
	while( pHeader != skyNull )
	{
        pFooter = ( CAllocatorDataFooter * )( ( u8 * )pHeader + sizeof( CAllocatorDataHeader ) + pHeader->m_Size );
		SKY_ASSERT_MSG( pHeader->m_CheckBreakId == ALLOCATE_DATA_ID && pFooter->m_CheckBreakId == ALLOCATE_DATA_ID , _T( "The value of the memory is illegal. " ) ); //メモリが壊されているか、 配列NEWした領域をDELしたか（DEL_ARRAYでなければならない）NEW_NATIVEで確保した領域をdeleteしました（正確にはDEL_NATIVEを使用してください）
		pHeader = pHeader->m_pPrev;
	}

	//後方スタックの破壊検査
	pHeader = m_BackStackList.IdxFromTail( 0 );
	while( pHeader != skyNull )
	{
        pFooter = ( CAllocatorDataFooter * )( ( u8 * )pHeader + sizeof( CAllocatorDataHeader ) + pHeader->m_Size );
		SKY_ASSERT_MSG( pHeader->m_CheckBreakId == ALLOCATE_DATA_ID && pFooter->m_CheckBreakId == ALLOCATE_DATA_ID , _T( "The value of the memory is illegal. " ) ); //メモリが壊されているか、 配列NEWした領域をDELしたか（DEL_ARRAYでなければならない）NEW_NATIVEで確保した領域をdeleteしました（正確にはDEL_NATIVEを使用してください）
		pHeader = pHeader->m_pPrev;
	}
#endif
}

} } }