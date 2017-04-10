/// <summary>
/// 汎用型の双方向リストの実装
/// </summary>
namespace sky { namespace lib {

template< typename T >
DLinkList< T >::DLinkList()
{
	if ( m_CreateInstanceNum == 0 )
	{
		for ( u32 i = 0; i < DLINK_LIST_CREATE_MAX_NUM ; i++ )
		{
			m_UsedFlg[ i ] = skyFalse;
		}
	}

	Init();
}

template< typename T >
DLinkList< T >::~DLinkList()
{
	Term();
}

template< typename T >
void DLinkList< T >::Init()
{
	u32 useId = 0;
	skyBool isFound = skyFalse;
	for ( u32 i = 0 ; i < DLINK_LIST_CREATE_MAX_NUM ; i++ )
	{
		if ( m_UsedFlg[ i ] == skyFalse )
		{
			useId = i;
			isFound = skyTrue;
			break;
		}
	}

	SKY_ASSERT_MSG( isFound , _T( "The number of instances exceeded the limit." ) ); //DLinkListのインスタンス数が限界を超えました。
	SKY_ASSERT_MSG( useId < DLINK_LIST_CREATE_MAX_NUM , _T( "The number of instances exceeded the limit." ) ); //DLinkListのインスタンス数が限界を超えました。

	m_InstanceNo = useId;
	m_UsedFlg[ m_InstanceNo ] = skyTrue;
	m_LinkNum[ m_InstanceNo ] = 0;
	m_pHead[ m_InstanceNo ] = skyNull;
	m_pTail[ m_InstanceNo ] = skyNull;
	m_CreateInstanceNum++;
}

template< typename T >
void DLinkList< T >::RecInit()
{
	m_pPrev = skyNull;
	m_pNext = skyNull;
}

template< typename T >
void DLinkList< T >::Term()
{
	m_UsedFlg[ m_InstanceNo ] = skyFalse;
	m_CreateInstanceNum--;
}

template< typename T >
u32 DLinkList< T >::Link( T *pItem )
{
	SKY_ASSERT_MSG( m_InstanceNo < DLINK_LIST_CREATE_MAX_NUM , _T( "The number of instances exceeded the limit." ) ); //DLinkListのインスタンス数が限界を超えました。

	pItem->RecInit();

	//最初が居ない場合
	if ( m_pHead[ m_InstanceNo ] == skyNull )
	{
		m_pHead[ m_InstanceNo ]				= pItem;
		m_pTail[ m_InstanceNo ]				= pItem;
	}
	else
	{
		pItem->m_pPrev		= m_pTail[ m_InstanceNo ];
		m_pTail[ m_InstanceNo ]->m_pNext	= pItem;
		m_pTail[ m_InstanceNo ]				= pItem;
	}

	m_LinkNum[ m_InstanceNo ]++;

	return ( m_LinkNum[ m_InstanceNo ] - 1 );
}

template< typename T >
u32 DLinkList< T >::LinkItemNext( T *pItem , T *pNewItem )
{
	pNewItem->RecInit();

	pNewItem->m_pNext = pItem->m_pNext;

	if ( pItem->m_pNext == skyNull )
	{
		m_pTail[ m_InstanceNo ] = pNewItem;
	}
	else
	{
		pItem->m_pNext->m_pPrev = pNewItem;
	}

	pItem->m_pNext = pNewItem;

	pNewItem->m_pPrev = pItem;

	m_LinkNum[ m_InstanceNo ]++;

	return ( m_LinkNum[ m_InstanceNo ] - 1 );
}

template< typename T >
u32 DLinkList< T >::LinkItemPrev( T *pItem , T *pNewItem )
{
	pNewItem->RecInit();

	pNewItem->m_pPrev = pItem->m_pPrev;

	if ( pItem->m_pPrev == skyNull )
	{
		m_pHead[ m_InstanceNo ] = pNewItem;
	}
	else
	{
		pItem->m_pPrev->m_pNext = pNewItem;
	}

	pItem->m_pPrev = pNewItem;

	pNewItem->m_pNext = pItem;

	m_LinkNum[ m_InstanceNo ]++;

	return ( m_LinkNum[ m_InstanceNo ] - 1 );
}

template< typename T >
void DLinkList< T >::UnLink( T *pItem )
{
	SKY_ASSERT_MSG( m_InstanceNo < DLINK_LIST_CREATE_MAX_NUM , _T( "The number of instances exceeded the limit." ) ); //DLinkListのインスタンス数が限界を超えました。

	if ( pItem->m_pPrev )
	{
		pItem->m_pPrev->m_pNext = pItem->m_pNext;
	}
	else
	{
		m_pHead[ m_InstanceNo ] = pItem->m_pNext;
	}
	if ( pItem->m_pNext )
	{
		pItem->m_pNext->m_pPrev = pItem->m_pPrev;
	}
	else
	{
		m_pTail[ m_InstanceNo ] = pItem->m_pPrev;
	}

	m_LinkNum[ m_InstanceNo ]--;
	SKY_ASSERT( m_LinkNum[ m_InstanceNo ] >= 0 ); //リンク数が0にも関わらずリンクをはずしに来ました。
}

template< typename T >
T* DLinkList< T >::IdxFromHead( u32 idx )
{
	T* pResult = m_pHead[ m_InstanceNo ];

	if ( pResult )
	{
		for( u32 i = 0 ; pResult != skyNull ; i++  )
		{
			if ( idx == i )
			{
				break;
			}
			pResult = pResult->m_pNext;
		}
	}

	return pResult;
}

template< typename T >
T* DLinkList< T >::IdxFromTail( u32 idx )
{
	T* pResult = m_pTail[ m_InstanceNo ];

	if ( pResult )
	{
		for( u32 i = 0 ; pResult != skyNull ; i++  )
		{
			if ( idx == i )
			{
				break;
			}
			pResult = pResult->m_pPrev;
		}
	}

	return pResult;
}

template< typename T >
T* DLinkList< T >::GetUniqueid( s32 uniqueid )
{
	T* pResult = m_pHead[ m_InstanceNo ];

	if ( pResult )
	{
		for( u32 i = 0 ; pResult != skyNull ; i++  )
		{
			if ( pResult->m_Uniqueid == uniqueid )
			{
				break;
			}
			pResult = pResult->m_pNext;
		}
	}

	return pResult;
}

template< typename T >
u32 DLinkList< T >::GetNum() const
{
	return m_LinkNum[ m_InstanceNo ];
}

template <class T>
u32 DLinkList< T >::m_CreateInstanceNum = 0;

template <class T>
skyBool DLinkList< T >::m_UsedFlg[ DLINK_LIST_CREATE_MAX_NUM ];

template <class T>
u32 DLinkList< T >::m_LinkNum[ DLINK_LIST_CREATE_MAX_NUM ];

template <class T>
T* DLinkList< T >::m_pHead[ DLINK_LIST_CREATE_MAX_NUM ];

template <class T>
T* DLinkList< T >::m_pTail[ DLINK_LIST_CREATE_MAX_NUM ];

} }