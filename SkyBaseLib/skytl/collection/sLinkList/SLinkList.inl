namespace sky { namespace lib {

template< class T >
SLinkList< T >::SLinkList() : m_pTop( skyNull )
{
}

template< class T >
void SLinkList< T >::Link( T *pItem )
{
	SKY_ASSERT_MSG( sizeof( T ) >= sizeof( Item ) , _T( "The size of a specified type is too small." ) ); //指定の型はサイズが小さすぎて単方向リストを構成できません。型のメンバにvoid* dummy等を定義して、最低限１アドレスを格納できる大きさを持つ型にして使用してください。

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

template< class T >
T *SLinkList< T >::GetTop()
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
	return new( pResult ) T;
}

} }