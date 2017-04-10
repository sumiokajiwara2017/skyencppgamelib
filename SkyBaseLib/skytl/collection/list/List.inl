namespace sky { namespace lib {

template< typename KEY, typename DATA , class ThreadingModel >
inline CList< KEY , DATA , ThreadingModel >::CList( u32 poolNum ) :
m_kItemPool( poolNum )
{
	m_pHead		= skyNull;
	m_pTail		= skyNull;
	m_nAddNum	= 0;
}

template< typename KEY, typename DATA , class ThreadingModel >
inline CList< KEY , DATA , ThreadingModel >::~CList()
{
	//全削除
	DeleteAll();
}

template< typename KEY , typename DATA , class ThreadingModel >
inline DATA &CList< KEY , DATA , ThreadingModel >::AddFront()
{
	thread::CriticalLock Lock( this->m_pCs );

	//新しい領域の確保
	ListItem *pNewItem = _CreateItem();

	if ( !m_pHead )
	{//最初の一個

		m_pHead = pNewItem;
		m_pTail = pNewItem;
	}
	else
	{//２個目以降

		pNewItem->pNext = m_pHead;
		m_pHead->pPrev	= pNewItem;
		m_pHead			= pNewItem;
	}

	m_nAddNum++;

	return pNewItem->data;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline void CList< KEY , DATA , ThreadingModel >::AddFront( const DATA &data )
{
	thread::CriticalLock Lock( this->m_pCs );
	
	DATA &newData = AddFront();

	newData = data;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline DATA &CList< KEY , DATA , ThreadingModel >::AddTail()
{
	thread::CriticalLock Lock( this->m_pCs );

	//新しい領域の確保
	ListItem *pNewItem = _CreateItem();

	if ( !m_pHead )
	{//最初の一個

		m_pHead = pNewItem;
		m_pTail = pNewItem;
	}
	else
	{//２個目以降

		pNewItem->pPrev = m_pTail;
		m_pTail->pNext	= pNewItem;
		m_pTail			= pNewItem;
	}

	m_nAddNum++;

	return pNewItem->data;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline void CList< KEY , DATA , ThreadingModel >::AddTail( const DATA &data )
{
	thread::CriticalLock Lock( this->m_pCs );

	DATA &newData = AddTail();

	newData = data;
}


template< typename KEY , typename DATA , class ThreadingModel >
inline void CList< KEY , DATA , ThreadingModel >::AddTail( KEY id , const DATA &data )
{
	thread::CriticalLock Lock( this->m_pCs );

	//新しい領域の確保
	ListItem *pNewItem = _CreateItem( id , data);

	if ( !m_pHead )
	{//最初の一個はＩＤとかお構いなしで追加
		m_pHead = pNewItem;
		m_pTail = pNewItem;
	}
	else
	{
		//先頭から線形で探す（なんか工夫はできんかな）
		ListItem *pCur = m_pHead;
		skyBool isFind = skyFalse;
		while( pCur )
		{
			if ( pCur->id == id )
			{
				//もし同値を見つけてその次の値が存在しなかった場合その同値の末尾に登録して終了
				if ( pCur->pNext == skyNull )
				{
					pNewItem->pPrev = pCur;
					pCur->pNext	= pNewItem;
					m_pTail			= pNewItem;
					break;
				}
				else if ( isFind == skyFalse )
				{
					isFind = skyTrue;
				}

				//ここを通るということは同値を探索中・・・・
			}
			else if ( pCur->id > id )
			{//idが上回ってしまったらその前に登録する
				pNewItem->pPrev = pCur->pPrev;
				pCur->pPrev = pNewItem;
				pNewItem->pNext	= pCur;

				//もし新しいデータが先頭より前なら新しい先頭とする
				if ( m_pHead == pCur )
				{
					m_pHead = pNewItem;
				}

				break;
			}
			else
			{//idが下回ったのでその末尾に登録する

				//もし同値を見つけてその次の値が存在しなかった場合その同値の末尾に登録して終了
				if ( pCur->pNext == skyNull )
				{
					pNewItem->pPrev = pCur;
					pCur->pNext	= pNewItem;
					m_pTail			= pNewItem;
					break;
				}
				else if ( isFind == skyTrue )
				{
					pNewItem->pPrev			= pCur->pNext;
					pCur->pNext->pNext	= pNewItem;
					break;
				}
			}
			pCur = pCur->pNext;
		}
	}
	m_nAddNum++;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline void CList< KEY , DATA , ThreadingModel >::AddNext( const DATA &frontData , const DATA &data )
{
	thread::CriticalLock Lock( this->m_pCs );

	//新しい領域の確保
	ListItem *pNewItem = _CreateItem( data );

	if ( !m_pHead )
	{//最初の一個はＩＤとかお構いなしで追加
		m_pHead = pNewItem;
		m_pTail = pNewItem;
	}
	else
	{
		//先頭から線形で探す（なんか工夫はできんかな）
		ListItem *pCur = m_pHead;
		while( pCur )
		{
			if ( pCur->data == frontData )
			{
				//もし見つけた値が末尾だったら新しいあたいを末尾とする
				if ( pCur->pNext == skyNull )
				{
					m_pTail = pNewItem;
				}
				//見つけた値の後ろにも登録
				pNewItem->pPrev = pCur;
				pCur->pNext	    = pNewItem;
				break;
			}
			pCur = pCur->pNext;
		}
	}
	m_nAddNum++;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline skyBool CList< KEY , DATA , ThreadingModel >::GetIdx( u32 idx , DATA &data ) const
{
	thread::CriticalLock Lock( this->m_pCs );

	//要素がない場合は何もせず返す
	if ( m_nAddNum == 0 ) return skyFalse;

    SKY_ASSERT_MSG( idx < m_nAddNum , _T( "index over. idx=[%d], addNum=[%d]" ), idx , m_nAddNum );

	ListItem		*pCur;

	if ( m_nAddNum / 2 > idx )
	{//前半のidxだ前半から辿ろう
		_SetCursorSearchStartHeadIdx( idx , pCur );
	}
	else
	{//後半のidxだ後半から辿ろう
		_SetCursorSearchStartTailIdx( idx , pCur );
	}

	data = pCur->data;

	return skyTrue;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline DATA &CList< KEY , DATA , ThreadingModel >::GetIdx( u32 idx ) const
{
	DATA *result;
	SetItCursor( idx , result );
	return *result;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline DATA &CList< KEY , DATA , ThreadingModel >::operator[]( s32 idx ) const
{
	DATA *result;
	SetItCursor( idx , result );
	return *result;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline skyBool CList< KEY , DATA , ThreadingModel >::GetKey( KEY id , DATA &data ) const
{
	thread::CriticalLock Lock( this->m_pCs );

	//要素がない場合は何もせず返す
	if ( m_nAddNum == 0 ) return skyFalse;

	ListItem *pCur;
	_SetCursorSearchStartHead( id , pCur );

	data = pCur->data;
	return skyTrue;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline skyBool CList< KEY , DATA , ThreadingModel >::Delete( const DATA &data )
{
	thread::CriticalLock Lock( this->m_pCs );

	//要素がない場合は何もせず返す
	if ( m_nAddNum == 0 ) return skyTrue;

	ListItem		*pCur;

	//データでカーソル設定
	if ( _SetCursor( data , pCur ) == skyFalse )
	{
		return skyTrue;
	}

	//自分の次を一時保存
	ListItem *pSaveNext = pCur->pNext;

	if ( pCur->pPrev )
	{//前が存在する場合前の次を自分の次にする
		pCur->pPrev->pNext = pCur->pNext;
	}
	else
	{//前が存在しない場合（先頭）自分の次を先頭にする
		m_pHead = pCur->pNext;
	}

	if ( pCur->pNext )
	{//次が存在する場合、次の前を自分の前で置き換える
		pCur->pNext->pPrev = pCur->pPrev;
	}
	else
	{//次が存在しない場合（一番後ろの場合）自分の前を一番後ろにする
		m_pTail = pCur->pPrev;
	}

	//要素の開放
	_DelCursor( pCur );

	//自分の次をカーソルに置く（ループ処理の過程で削除された可能性が高いため）
	pCur = pSaveNext;

	m_nAddNum--;

	return skyTrue;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline skyBool CList< KEY , DATA , ThreadingModel >::DeleteTail()
{
	//要素がない場合は何もせず返す
	if ( m_nAddNum == 0 ) return skyTrue;

	ListItem		*pCur;

	//要素が一つの場合は消して終わり
	if ( m_nAddNum == 1 )
	{
		//m_pHeadでもm_pTailでもどっちでもいいが・・
		pCur = m_pTail;

		//カーソルを削除
		_DelCursor( pCur );
	}
	else
	{
		//最後の要素の前の要素の次を消す
		m_pTail->pPrev->pNext = NULL;

		//最後の要素の一つ前を新要素とする。
		ListItem *pNewTail = m_pTail->pPrev;

		//現在の末尾を消す
		pCur = m_pTail;

		//カーソルを削除
		_DelCursor( pCur );

		//新しい末尾をセット
		m_pTail = pNewTail;

	}
	return skyTrue;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline void CList< KEY , DATA , ThreadingModel >::DeleteAll()
{
	thread::CriticalLock Lock( this->m_pCs );

	if ( m_pHead != skyNull )
	{
		ListItem *pCur;
		ListItem *pNext;
		pCur = m_pHead;
		while( pCur )
		{
			//次を保存
			pNext = pCur->pNext;

			//要素の開放
			_DelCursor( pCur );

			//次をサーチ
			pCur = pNext;
		}
		m_pHead = skyNull;
		m_pTail = skyNull;
	}
	m_nAddNum = 0;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline DATA &CList< KEY , DATA , ThreadingModel >::SetCursorSearchStartHead( KEY id )
{
	thread::CriticalLock Lock( this->m_pCs );

	//要素がない場合は何もせず返す
	if ( m_nAddNum == 0 ) return skyNull;

	//先頭から探査
	DATA	*pCurData;
	_SetCursorSearchStartHead( id , pCurData );

	return *pCurData;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline DATA &CList< KEY , DATA , ThreadingModel >::SetCursorSearchStartTail( KEY id )
{
	thread::CriticalLock Lock( this->m_pCs );

	//要素がない場合は何もせず返す
	if ( m_nAddNum == 0 ) return skyNull;

	//先頭から探査
	DATA	*pCurData;
	_SetCursorSearchStartTail( id , pCurData );

	return *pCurData;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline u32 CList< KEY , DATA , ThreadingModel >::GetNum() const
{ 
	thread::CriticalLock Lock( this->m_pCs );

	return m_nAddNum;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline skyBool CList< KEY , DATA , ThreadingModel >::IsEmpty() const
{
	thread::CriticalLock Lock( this->m_pCs );

	return ( m_nAddNum == 0 );
}

template< typename KEY, typename DATA , class ThreadingModel >
inline CList< KEY , DATA , ThreadingModel >::iterator::iterator( CList< KEY , DATA , ThreadingModel > *pList, s32 cursor ) :
Iterator< CList< KEY , DATA , ThreadingModel > , DATA >(pList, cursor )
{
}

template< typename KEY , typename DATA , class ThreadingModel >
inline skyBool CList< KEY , DATA , ThreadingModel >::IsBeginBefor( iterator &it ) const
{
	return ( it.GetIdx() < 0 );
}

template< typename KEY , typename DATA , class ThreadingModel >
inline typename CList< KEY , DATA , ThreadingModel >::iterator CList< KEY , DATA , ThreadingModel >::Begin() const
{
	thread::CriticalLock Lock( this->m_pCs );

	return iterator( const_cast< CList< KEY , DATA , ThreadingModel > * >( this ) , 0 );
}

template< typename KEY , typename DATA , class ThreadingModel >
inline typename CList< KEY , DATA , ThreadingModel >::iterator CList< KEY , DATA , ThreadingModel >::End() const
{
	thread::CriticalLock Lock( this->m_pCs );

	return iterator( const_cast< CList< KEY , DATA , ThreadingModel > * >( this ) , m_nAddNum );
}

template< typename KEY , typename DATA , class ThreadingModel >
inline typename CList< KEY , DATA , ThreadingModel >::iterator CList< KEY , DATA , ThreadingModel >::Erases( typename CList< KEY , DATA , ThreadingModel >::iterator &it )
{
	thread::CriticalLock Lock( this->m_pCs );

	s32 cursor = it.Delete();
	return iterator( this , cursor );
}

template< typename KEY , typename DATA , class ThreadingModel >
inline typename CList< KEY , DATA , ThreadingModel >::iterator CList< KEY , DATA , ThreadingModel >::GetNextPos( typename CList< KEY , DATA , ThreadingModel >::iterator &it ) const
{
	thread::CriticalLock Lock( this->m_pCs );

	return iterator( const_cast< CList< KEY , DATA , ThreadingModel > * >( this ) , it.Next() );
}

template< typename KEY , typename DATA , class ThreadingModel >
inline skyBool CList< KEY , DATA , ThreadingModel >::SetItCursor( u32 idx , DATA* &data ) const
{
	thread::CriticalLock Lock( this->m_pCs );

    SKY_ASSERT_MSG( idx < ( u32 )m_nAddNum , _T( "index over. idx=[ %d ], addNum=[ %d ]" ), idx , m_nAddNum );

	//要素数からどちらから辿ったほうが早いか決定する

	ListItem *pCur;
	if ( (u32)m_nAddNum / 2 > idx ) //シフト演算が高速だがコンパイラが最適化するだろう
	{//前半のidxだ前半から辿ろう
		_SetCursorSearchStartHeadIdx( idx , pCur );
	}
	else
	{//後半のidxだ後半から辿ろう
		_SetCursorSearchStartTailIdx( idx , pCur );
	}

	data = &pCur->data;

	return skyTrue;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline skyBool CList< KEY , DATA , ThreadingModel >::DeleteIdx( u32 idx )
{
	thread::CriticalLock Lock( this->m_pCs );

	//要素がない場合は何もせず返す
	if ( m_nAddNum == 0 ) return skyTrue;

    SKY_ASSERT_MSG( idx < ( u32 )m_nAddNum , _T( "index over. idx=[%d], addNum=[%d]" ), idx , m_nAddNum );

	ListItem		*pCur;

	if ( m_nAddNum / 2 > idx )//シフト演算が高速だがコンパイラが最適化するだろう
	{//前半のidxだ前半から辿ろう
		_SetCursorSearchStartHeadIdx( idx , pCur );
	}
	else
	{//後半のidxだ後半から辿ろう
		_SetCursorSearchStartTailIdx( idx , pCur );
	}

	if ( pCur->pPrev )
	{//前が存在する場合前の次を自分の次にする
		pCur->pPrev->pNext = pCur->pNext;
	}
	else
	{//前が存在しない場合（先頭）自分の次を先頭にする
		m_pHead = pCur->pNext;
	}

	if ( pCur->pNext )
	{//次が存在する場合、次の前を自分の前で置き換える
		pCur->pNext->pPrev = pCur->pPrev;
	}
	else
	{//次が存在しない場合（一番後ろの場合）自分の前を一番後ろにする
		m_pTail = pCur->pPrev;
	}

	//要素の開放
	_DelCursor( pCur );

	m_nAddNum--;

	return skyTrue;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline typename CList< KEY , DATA , ThreadingModel >::ListItem* CList< KEY , DATA , ThreadingModel >::_CreateItem( KEY id , const DATA &data )
{
	//新しい領域の確保
	ListItem *pNewItem	= m_kItemPool.Create();	//リストの要素の作成
	pNewItem->pPrev = skyNull;
	pNewItem->pNext = skyNull;

	//データのコピー
	pNewItem->data = data;			//クラスならコピーコンストラクタ発動

	//IDあげる
	pNewItem->id = id;

	return pNewItem;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline typename CList< KEY , DATA , ThreadingModel >::ListItem* CList< KEY , DATA , ThreadingModel >::_CreateItem( KEY id )
{
	//新しい領域の確保
	ListItem *pNewItem	= m_kItemPool.Create();	//リストの要素の作成
	pNewItem->pPrev = skyNull;
	pNewItem->pNext = skyNull;

	//IDあげる
	pNewItem->id = id;

	return pNewItem;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline typename CList< KEY , DATA , ThreadingModel >::ListItem* CList< KEY , DATA , ThreadingModel >::_CreateItem( const DATA &data )
{
	//新しい領域の確保
	ListItem *pNewItem	= m_kItemPool.Create();	//リストの要素の作成
	pNewItem->pPrev = skyNull;
	pNewItem->pNext = skyNull;

	//データのコピー
	pNewItem->data = data;			//クラスならコピーコンストラクタ発動

	return pNewItem;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline typename CList< KEY , DATA , ThreadingModel >::ListItem* CList< KEY , DATA , ThreadingModel >::_CreateItem()
{
	//新しい領域の確保
	ListItem *pNewItem	= m_kItemPool.Create();	//リストの要素の作成
	pNewItem->pPrev = skyNull;
	pNewItem->pNext = skyNull;

	return pNewItem;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline void CList< KEY , DATA , ThreadingModel >::_SetCursorSearchStartHeadIdx( u32 idx, ListItem* &pCur ) const
{
	pCur = m_pHead;
	for ( u32 i = 0 ; i<m_nAddNum; i++ )
	{
		if ( i == idx ) break;
		pCur = pCur->pNext;
	}
}

template< typename KEY , typename DATA , class ThreadingModel >
inline void CList< KEY , DATA , ThreadingModel >::_SetCursorSearchStartTailIdx( u32 idx, ListItem* &pCur ) const
{
	pCur = m_pTail;
	for ( u32 i = m_nAddNum - 1 ; i >= 0 ; i-- )
	{
		if ( i == idx ) break;
		pCur = pCur->pPrev;
	}
}

template< typename KEY , typename DATA , class ThreadingModel >
inline void CList< KEY , DATA , ThreadingModel >::_SetCursorSearchStartHead( KEY id , ListItem* &pCur ) const
{
	pCur = m_pHead;
	while( pCur )
	{
		if ( pCur->id == id ) break;
		pCur = pCur->pNext;
	}
}

template< typename KEY , typename DATA , class ThreadingModel >
inline void CList< KEY , DATA , ThreadingModel >::_SetCursorSearchStartTail( KEY id, ListItem* &pCur ) const
{
	pCur = m_pTail;
	skyBool isFind = skyFalse; //見つけた
	while( pCur )
	{
		if ( pCur->id == id )
		{
			//もし同値のＩＤを見つけその前がもう存在しなかったらそこで終了
			if ( pCur->pPrev == skyNull )
			{
				break;
			}
			else if ( isFind == skyFalse )
			{
				isFind = skyTrue;
			}
		}
		else if ( pCur->id > id )
		{
			pCur = skyNull; //そのＩＤのアイテムは無い。
			break;
		}
		else
		{
			if ( isFind == skyTrue )
			{//ＩＤ郡の先頭を捉えた
				pCur = pCur->pNext;
				break;
			}
		}
		pCur = pCur->pPrev;
	}
}

template< typename KEY , typename DATA , class ThreadingModel >
inline skyBool CList< KEY , DATA , ThreadingModel >::_SetCursor( DATA p, ListItem* &pCur )
{
	pCur = m_pHead;
	while( pCur )
	{
		if ( pCur->data == p ) return skyTrue;
		pCur = pCur->pNext;
	}
	return skyFalse;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline void CList< KEY , DATA , ThreadingModel >::_DelCursor( ListItem* &pCur )
{
	//値を削除
	m_kItemPool.Delete( pCur );
}

template< typename KEY , typename DATA , class ThreadingModel >
inline KEY	CList< KEY , DATA , ThreadingModel >::Find( const DATA &data ) const
{
	KEY result = 0;
	ListItem* pCur = m_pHead;
	while( pCur )
	{
		if ( pCur->data == data )
		{
			return result;
		}
		pCur = pCur->pNext;
		result++;
	}
	return ( u32 )~0;
}

template< typename KEY , typename DATA >
inline CListMT< KEY , DATA >::CListMT() :
CList< KEY , DATA , thread::ThreadingModels::CMultiThread >()
{
}

template< typename KEY , typename DATA >
inline CListMT< KEY , DATA >::~CListMT()
{
}

} }
