namespace sky { namespace lib {

inline SLinkListSize::SLinkListSize( u32 blockSize ) : m_pTop( skyNull ) , m_uiBlockSize( blockSize )
{
    SKY_ASSERT_MSG( m_uiBlockSize >= sizeof( Item ) , _T( "The size doesn't suffice." ) );
}

inline void SLinkListSize::Link( void *pItem )
{
	Item *pNewItem = ( Item * )pItem;

	//コンストラクタが発動しないので自分で呼ぶ；
	pNewItem->Constructor();

	if ( m_pTop == skyNull )
	{//もし初回の場合

		//先頭に指定
		m_pTop = pNewItem;
	}
	else
	{
		//先頭を新人の次にする
		pNewItem->m_pNext = m_pTop;

		//新人が先頭になる
		m_pTop = pNewItem;
	}
}

inline void *SLinkListSize::GetTop()
{
	//もしTopがskyNullだったら1件も登録がないのでskyNullを返す
	if ( m_pTop == skyNull )
	{
		return skyNull;
	}

	//先頭のアドレスを戻り値用ワークに
	Item *pResult = m_pTop;

	//もし次がなかったら
	if ( pResult->m_pNext == skyNull )
	{//TopをNullにする
		m_pTop = skyNull;
	}
	else
	{//Topの次をTopにする
		m_pTop = pResult->m_pNext;
	}

	//こうやったら指定のアドレスを使用してインスタンスを構築し、コンストラクタが発動する・・はず。
	return ( void * )pResult;
}

} }
