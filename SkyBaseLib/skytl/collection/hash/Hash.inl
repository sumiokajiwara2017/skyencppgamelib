namespace sky { namespace lib {

template< typename T , typename KEY , class ThreadingModel >
inline CHash< T , KEY , ThreadingModel >::CHash( s32 hashSize , u32 poolNum ) :
m_kItemPool( poolNum )
{
	m_nHashSize = hashSize;

	//テーブル作成
	u32 tblSize = sizeof ( _HashListTbl * ) * hashSize;
	m_ppListTbl = ( _HashListTbl ** )SkyMalloc( tblSize ); //ポインタテーブルを作ってるだけだからMallocでいい
	CMemoryUtil_::ClearZero( m_ppListTbl , tblSize );	

	m_nSetNum = 0;
}

template< typename T , typename KEY , class ThreadingModel >
inline CHash< T , KEY , ThreadingModel >::~CHash()
{
	//全削除
	DeleteAll();
	SkyFree( m_ppListTbl ); //ほんとに器も消すのはデストラクタだけ
}

template< typename T , typename KEY , class ThreadingModel >
inline T &CHash< T , KEY , ThreadingModel >::Set( const hash::CHashStringPtr &hash )
{
	u32 hashId = hash->Get();
	T data;
	_Set( hashId , hashId % m_nHashSize  , data , hash );
	return Get( hashId );
}

template< typename T , typename KEY , class ThreadingModel >
inline T &CHash< T , KEY , ThreadingModel >::Set( const skyString *pStrId )
{
	hash::CHashStringPtr idst = CHS( pStrId );
	T data;
	_Set( idst->Get() , idst->Get() % m_nHashSize  , data , idst );
	return Get( idst->Get() );
}

template< typename T , typename KEY , class ThreadingModel >
inline T &CHash< T , KEY , ThreadingModel >::Set( KEY id )
{
//	skyString buf[ 64 ];
	T data;
//	_Set( id , id % m_nHashSize , data , CHS( CStringUtil_::ConvNumToString( id , buf , 64 ) ) ); //文字を作っても意味が無いので消去
	_Set( id , id % m_nHashSize , data , hash::CHashStringPtr() );
	return Get( id );
}

template< typename T , typename KEY , class ThreadingModel >
inline void CHash< T , KEY , ThreadingModel >::Set( const hash::CHashStringPtr &hash , const T &data )
{
	u32 hashId = hash->Get();
	_Set( hashId , hashId % m_nHashSize  , data , hash );
}

template< typename T , typename KEY , class ThreadingModel >
inline void CHash< T , KEY , ThreadingModel >::Set( const skyString *pStrId , const T &data )
{
	hash::CHashStringPtr idst = CHS( pStrId );
	_Set( idst->Get() , idst->Get() % m_nHashSize  , data , idst );
}

template< typename T , typename KEY , class ThreadingModel >
inline void CHash< T , KEY , ThreadingModel >::Set( KEY id , const T &data)
{
//	skyString buf[ 64 ];
//	_Set( id , id % m_nHashSize , data , CHS( CStringUtil_::ConvNumToString( id , buf , 64 ) ) );
	_Set( id , id % m_nHashSize , data , hash::CHashStringPtr() );
}

template< typename T , typename KEY , class ThreadingModel >
inline skyBool CHash< T , KEY , ThreadingModel >::IsGet( KEY id )
{
	T *pData;
	return _Get( id , id % m_nHashSize , pData );
}

template< typename T , typename KEY , class ThreadingModel >
inline skyBool CHash< T , KEY , ThreadingModel >::IsGet( const skyString *pStrId )
{
	T *pData;
	u32 hashId = GetHashId( pStrId );
	return _Get( hashId , hashId % m_nHashSize , pData );
}

template< typename T , typename KEY , class ThreadingModel >
inline skyBool CHash< T , KEY , ThreadingModel >::IsGet( const hash::CHashStringPtr &hash )
{
	T *pData;
	u32 hashId = hash->Get();
	return _Get( hashId , hashId % m_nHashSize , pData );
}

template< typename T , typename KEY , class ThreadingModel >
inline skyBool CHash< T , KEY , ThreadingModel >::Get( const skyString *pStrId , T &data ) const
{
	if ( m_nSetNum == 0 ) return skyFalse;

	u32 hashId = GetHashId( pStrId );
	T *pData;
	if ( _Get( hashId , hashId % m_nHashSize , pData ) == skyTrue )
	{
		data = *pData;
		return skyTrue;
	}
	else
	{
		return skyFalse;
	}
}

template< typename T , typename KEY , class ThreadingModel >
inline T &CHash< T , KEY , ThreadingModel >::Get( const skyString *pStrId ) const
{
	u32 hashId = GetHashId( pStrId );
	T *pData;
	if ( _Get( hashId , hashId % m_nHashSize , pData ) == skyFalse )
	{
		SKY_PANIC_MSG( _T( "There was no value of a specified key value." ) ); //There was no value of a specified key value.
	}
	return *pData;
}


template< typename T , typename KEY , class ThreadingModel >
inline T &CHash< T , KEY , ThreadingModel >::operator[]( const skyString *pStrId ) const
{
	u32 hashId = GetHashId( pStrId );
	T *pData;
	if ( _Get( hashId , hashId % m_nHashSize , pData ) == skyFalse )
	{
		SKY_PANIC_MSG( _T( "There was no value of a specified key value." ) );
	}
	return *pData;
}

template< typename T , typename KEY , class ThreadingModel >
inline skyBool CHash< T , KEY , ThreadingModel >::Get( const hash::CHashStringPtr &hash , T &data ) const
{
	if ( m_nSetNum == 0 ) return skyFalse;

	u32 hashId = hash->Get();
	T *pData;
	if ( _Get( hashId , hashId % m_nHashSize , pData ) == skyTrue )
	{
		data = *pData;
		return skyTrue;
	}
	else
	{
		return skyFalse;
	}
}

template< typename T , typename KEY , class ThreadingModel >
inline T &CHash< T , KEY , ThreadingModel >::Get( const hash::CHashStringPtr &hash ) const
{
	u32 hashId = hash->Get();
	T *pData;
	if ( _Get( hashId , hashId % m_nHashSize , pData ) == skyFalse )
	{
		SKY_PANIC_MSG( _T( "There was no value of a specified key value." ) );
	}
	return *pData;
}

template< typename T , typename KEY , class ThreadingModel >
inline T &CHash< T , KEY , ThreadingModel >::operator[]( const hash::CHashStringPtr &hash ) const
{
	u32 hashId = hash->Get();
	T *pData;
	if ( _Get( hashId , hashId % m_nHashSize , pData ) == skyFalse )
	{
		SKY_PANIC_MSG( _T( "There was no value of a specified key value." ) );
	}
	return *pData;
}

template< typename T , typename KEY , class ThreadingModel >
inline skyBool CHash< T , KEY , ThreadingModel >::Get( KEY id , T &data ) const
{
	if ( m_nSetNum == 0 ) return skyFalse;

	T *pData;
	if ( _Get( id , id % m_nHashSize , pData ) == skyTrue )
	{
		data = *pData;
		return skyTrue;
	}
	else
	{
		return skyFalse;
	}
}

template< typename T , typename KEY , class ThreadingModel >
inline T &CHash< T , KEY , ThreadingModel >::Get( KEY id ) const
{
	T *pData;
	if ( _Get( id , id % m_nHashSize , pData ) == skyFalse )
	{
		SKY_PANIC_MSG( _T( "There was no value of a specified key value." ) );
	}
	return *pData;
}


template< typename T , typename KEY , class ThreadingModel >
inline T &CHash< T , KEY , ThreadingModel >::operator[]( KEY id ) const
{
	T *pData;
	if ( _Get( id , id % m_nHashSize , pData ) == skyFalse )
	{
		SKY_PANIC_MSG( _T( "There was no value of a specified key value." ) );
	}
	return *pData;
}

template< typename T , typename KEY , class ThreadingModel >
inline skyBool CHash< T , KEY , ThreadingModel >::Delete( const hash::CHashStringPtr &hash )
{
	u32 hashId = hash->Get();
	return _Delete( hashId , hashId % m_nHashSize );
}

template< typename T , typename KEY , class ThreadingModel >
inline skyBool CHash< T , KEY , ThreadingModel >::Delete( const skyString *pStrId )
{
	u32 hashId = GetHashId( pStrId );
	return _Delete( hashId , hashId % m_nHashSize );
}

template< typename T , typename KEY , class ThreadingModel >
inline skyBool CHash< T , KEY , ThreadingModel >::Delete( KEY id )
{
	return _Delete( id , id % m_nHashSize );
}

template< typename T , typename KEY , class ThreadingModel >
inline void CHash< T , KEY , ThreadingModel >::DeleteAll()
{
	while( m_nSetNum > 0 )
	{
		DeleteIdx( 0 );
	}
}

template< typename T , typename KEY , class ThreadingModel >
inline u32 CHash< T , KEY , ThreadingModel >::GetNum() const
{ 
	return m_nSetNum;
}

template< typename T , typename KEY , class ThreadingModel >
inline u32 CHash< T , KEY , ThreadingModel >::GetHashSize() const
{ 
	return m_nHashSize;
}

template< typename T , typename KEY , class ThreadingModel >
inline void CHash< T , KEY , ThreadingModel >::GetkeyList( hash::CHashStringPtrList &keyList ) const 
{
	T* data;
	hash::CHashStringPtr idst;
	for ( u32 i = 0 ; i < m_nSetNum ; i++ )
	{
		SetItCursor( i , data , idst );
		keyList.AddTail( idst );
	}
}

template< typename T , typename KEY , class ThreadingModel >
inline skyBool CHash< T , KEY , ThreadingModel >::IsEmpty() const
{
	return ( m_nSetNum == 0 );
}


template< typename T , typename KEY , class ThreadingModel >
inline CHash< T , KEY , ThreadingModel >::iterator::iterator( CHash< T , KEY , ThreadingModel > *pHash , s32 cursor ) :
Iterator< CHash< T , KEY , ThreadingModel > , T >( pHash , cursor )
{
}

template< typename T , typename KEY , class ThreadingModel >
inline skyBool CHash< T , KEY , ThreadingModel >::IsBeginBefor( iterator &it ) const
{
	return ( it.GetIdx() < 0 );
}

template< typename T , typename KEY , class ThreadingModel >
inline typename CHash< T , KEY , ThreadingModel >::iterator CHash< T , KEY , ThreadingModel >::Begin()
{
    return CHash< T , KEY , ThreadingModel >::iterator( this , 0 );
}

template< typename T , typename KEY , class ThreadingModel >
inline typename CHash< T , KEY , ThreadingModel >::iterator CHash< T , KEY , ThreadingModel >::End()
{
    return CHash< T , KEY , ThreadingModel >::iterator( this , m_nSetNum );
}

template< typename T , typename KEY , class ThreadingModel >
inline typename CHash< T , KEY , ThreadingModel >::iterator CHash< T , KEY , ThreadingModel >::Erases( typename CHash< T , KEY , ThreadingModel >::iterator &it )
{
	s32 cursor = it.Delete();
    return CHash< T , KEY , ThreadingModel >::iterator( this , cursor );
}

template< typename T , typename KEY , class ThreadingModel >
inline skyBool CHash< T , KEY , ThreadingModel >::SetItCursor( u32 idx , T* &data ) const
{
	hash::CHashStringPtr idst;
	return SetItCursor( idx , data , idst );
}

template< typename T , typename KEY , class ThreadingModel >
inline skyBool CHash< T , KEY , ThreadingModel >::SetItCursor( u32 idx , T* &data , hash::CHashStringPtr &idst ) const
{
	u32 count = 0;
	_HashListTbl *pCur = skyNull;
	for ( u32 i = 0 ; i < m_nHashSize ; i++ )
	{
		if ( m_ppListTbl[ i ] != skyNull )
		{
			pCur = m_ppListTbl[ i ];
			while( pCur != skyNull )
			{
				if ( idx == count )
				{
					//値発見。返す
					data = &pCur->data;
					idst = pCur->idst;
					return skyTrue;
				}
				count++;
				pCur = pCur->pNext;
			}
		}
	}
	return skyFalse;
}


template< typename T , typename KEY , class ThreadingModel >
inline skyBool CHash< T , KEY , ThreadingModel >::DeleteIdx( u32 idx )
{
	u32 count = 0;
	_HashListTbl*pCur = skyNull;
	skyBool ret = skyFalse;
	for ( u32 i = 0 ; i<m_nHashSize; i++ )
	{
		if ( m_ppListTbl[ i ] != skyNull )
		{
			pCur = m_ppListTbl[ i ];
			while( pCur != skyNull )
			{
				if ( idx == count )
				{
					ret = Delete( pCur->id );
					goto END;
				}
				count++;
				pCur = pCur->pNext;
			}
		}
	}
	END:
	return ret;
}

template< typename T , typename KEY , class ThreadingModel >
inline u32 CHash< T , KEY , ThreadingModel >::GetHashId( const skyString *pKeyStr ) const
{
	return GET_HASH_VALUE( ( skyString * )pKeyStr );
}

template< typename T , typename KEY , class ThreadingModel >
inline skyBool CHash< T , KEY , ThreadingModel >::EqId( KEY id , _HashListTbl *pList ) const
{
	return ( id == pList->id );
}

template< typename T , typename KEY , class ThreadingModel >
inline skyBool CHash< T , KEY , ThreadingModel >::_Set( u32 id , u32 hashId , const T &data , const hash::CHashStringPtr &idst  )
{
	idst;
	_HashListTbl*pNew;

	if( m_ppListTbl[ hashId ] == skyNull )
	{//ハッシュ値が重複していない

		pNew = m_kItemPool.Create();													//新しいレコード作成
		pNew->id 		= id;
		pNew->idst 		= idst;

		pNew->data					= data;												//値をコピー(クラスならコピーコンストラクタ発動）
		pNew->pNext 				= skyNull;
		m_ppListTbl[ hashId ] 		= pNew;    											//リストの先頭レコードなのでそのままセット
	}
	else
	{//ハッシュ値が重複したので線形探査が必要
		
		//既存の項目を探す
		_HashListTbl *pNow = m_ppListTbl[ hashId ];  										//先頭をセット
		while( pNow )
		{
			if( EqId( id , pNow ) )
			{//idまで重複した（上書き）
				pNow->data = data;														//値をコピー（クラスならコピーコンストラクタ発動）
				return skyTrue; 														//終了
			}
			pNow = pNow->pNext;  														//次のチェーンを辿る
		}

	   //新しいidなので先頭に追加
		pNew = m_kItemPool.Create();													//新しいレコード作成
		pNew->id   = id;
		pNew->idst = idst;
		pNew->data = data;																//値をコピー

		pNew->pNext	= m_ppListTbl[ hashId ];  											// 以前の先頭を自分の次にする
		m_ppListTbl[ hashId ]	= pNew;    												// 先頭に追加
	}
	m_nSetNum++;
	return skyTrue;
}

template< typename T , typename KEY , class ThreadingModel >
inline skyBool CHash< T , KEY , ThreadingModel >::_Get( u32 id , u32 hashId , T* &data ) const
{
	if( m_ppListTbl[ hashId ] == skyNull )
	{//該当のハッシュ値に対する値が無い
	   return skyNull;
	}
	else
	{//ハッシュ値の値はある、あとはidが一致するかだ
		_HashListTbl *pNow = m_ppListTbl[ hashId ];  										//先頭をセット
		while( pNow )
		{
			if( EqId( id , pNow ) )
			{//発見
				data = &pNow->data;
				return skyTrue;
			}
			pNow = pNow->pNext;															//次のチェーンを辿る
		}
		return skyFalse;//同じハッシュ値の仲間は居たがidはなかった。
	}
}

template< typename T , typename KEY , class ThreadingModel >
inline skyBool CHash< T , KEY , ThreadingModel >::_Delete(  u32 id , u32 hashId )
{
	//線形探査が必要
	_HashListTbl *pNow = m_ppListTbl[ hashId ];  											//先頭をセット
	_HashListTbl *pPrev = skyNull;															//前の要素
	_HashListTbl *pNext = skyNull;															//次の要素
	while( pNow )
	{
		pNext = pNow->pNext; 															//次を保存しておく
		if( EqId( id , pNow ) )
		{//削除対象を発見

			//要素を消す
			_DelList( pNow );
			
			//前の要素の次に次の要素を格納する
			if ( pPrev != skyNull )
			{
				pPrev->pNext = pNext;
			}
			else
			{
				//前の要素がなかった、つまり先頭だったので先頭に次の要素を格納（skyNullでもOK)
				m_ppListTbl[ hashId ] = pNext;
			}
			m_nSetNum--;
			return skyTrue;																//無事消しました！
		}
		else
		{
			pPrev = pNow;																//前を保存しておく
		}
		pNow = pNext;																	//次のチェーンを辿る
	}

	return skyFalse;																	//消すものがありませんでした。
}

template< typename T , typename KEY , class ThreadingModel >
inline void CHash< T , KEY , ThreadingModel >::_DelList( _HashListTbl *pList )
{
	//値を削除
	m_kItemPool.Delete( pList );
}

template< typename T , typename KEY >
inline CHashMT< T , KEY >::CHashMT( s32 hashSize ) :
CHash< T , KEY , thread::ThreadingModels::CMultiThread >( hashSize )
{
}

template< typename T , typename KEY >
inline CHashMT< T , KEY >::~CHashMT()
{
}

} }
