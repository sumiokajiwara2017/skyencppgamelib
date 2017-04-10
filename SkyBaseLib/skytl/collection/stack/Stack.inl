namespace sky { namespace lib {

template< typename T >
inline CStack< T >::CStack( s32 size )
{
	
	m_pData = NEW_ARRAY_() T[ size ]; //スタックの深さ分領域を確保し、その先頭ポインタを保持
	m_Size = size;
	m_PushNum = 0;
}

template< typename T >
inline CStack< T >::~CStack()
{
	DEL_ARRAY m_pData; //@@SKYTODO sizeの値が大きいとここがエラーする（要調査）
}

template< typename T >
inline u32 CStack< T >::Push( const T &data )
{
    SKY_ASSERT_MSG( m_PushNum < m_Size , _T( "Stack size over. Size=[%d], PushNum=[%d]" ) , m_Size , m_PushNum );

	T* pCursor;

	//Push数から現在のカーソルトップアドレスを取得
	_SetCursor( m_PushNum , pCursor );

	//トップアドレスに値をコピー
	*pCursor = data;

	//Push数Up
	m_PushNum++;

	return ( m_PushNum-1 );
}

template< typename T >
inline u32 CStack< T >::Push()
{
    SKY_ASSERT_MSG( m_PushNum < m_Size , _T( "stack size over. Size=[%d], PushNum=[%d]" ) , m_Size , m_PushNum );

	//Push数Up
	m_PushNum++;

	return ( m_PushNum-1 );
}

template< typename T >
inline skyBool CStack< T >::Pop( T &data )
{
	SKY_ASSERT_MSG( m_PushNum > 0, _T( "The stack is empty." ) ); //The stack is empty.

	//Push数Donw
	--m_PushNum;

	T *pCur;

	//カーソルを進める
	_SetCursor( m_PushNum , pCur );

	//値を返す。（コピーコンストラクタ発動）
	data = *pCur;

	return skyTrue;
}

template< typename T >
inline skyBool CStack< T >::Pop()
{
	SKY_ASSERT_MSG( m_PushNum > 0, _T( "The stack is empty." ) );

	//Push数Donw
	--m_PushNum;

	return skyTrue;
}

template< typename T >
inline skyBool CStack< T >::PopBottom( T &data )
{
	T *pCur;

	//カーソルを取得
	_SetCursor( 0 , pCur );

	//値を返す。（コピーコンストラクタ発動）
	data = *pCur;

	return PopBottom();
}

template< typename T >
inline skyBool CStack< T >::PopBottom()
{
	T *pCur;

	//カーソルを取得
	_SetCursor( 1 , pCur );

	//Push数Donw
	--m_PushNum;

	//メモリ全体を１レコード分スタックの下にずらす（一番したをつぶす）
	CMemoryUtil_::Copy( m_pData , pCur , sizeof( T ) * m_PushNum );

	return skyTrue;
}

template< typename T >
inline void CStack< T >::Set( const T &data )
{
	SKY_ASSERT_MSG( m_PushNum > 0, _T( "The stack is empty." ) );

	T *pCur;

	//カーソルを進める
	_SetCursor( m_PushNum-1 , pCur );

	*pCur = data;
}


template< typename T >
inline void CStack< T >::Clear()
{
	m_PushNum = 0;
}

template< typename T >
inline u32 CStack< T >::GetNum() const { return m_PushNum; }

template< typename T >
inline u32 CStack< T >::GetSize() const	{ return m_Size; }

template< typename T >
inline T &CStack< T >::GetTop()
{
	T *pCur;

	//一番上のポインタを取得
	_SetCursor( m_PushNum-1 , pCur );

	return *pCur;
}

template< typename T >
inline T &CStack< T >::GetBottom()
{
	T *pCur;

	//一番下のポインタを取得
	_SetCursor( 0 , pCur );

	return *pCur;
}

template< typename T >
inline T &CStack< T >::GetItem( s32 idx ) const
{
	T *pCur;

	//カーソルを進める
	_SetCursor( idx , pCur );

	return *pCur;
}
template< typename T >
inline skyBool CStack< T >::GetItem( s32 idx , T &data ) const
{
	T *pCur;

	//カーソルを進める
	_SetCursor( idx , pCur );

	data = *pCur;

	return skyTrue;
}
template< typename T >
inline T &CStack< T >::operator[]( s32 idx ) const
{
	T *result;
	SetItCursor( idx , result );
	return *result;
}

template< typename T >
inline CStack< T >::iterator::iterator( CStack< T > *pStack, s32 cursor ) :
Iterator< CStack , T >( pStack , cursor )
{
}

template< typename T >
inline skyBool CStack< T >::IsBeginBefor( iterator &it ) const
{
	return ( it.GetIdx() < 0 );
}

template< typename T >
inline typename CStack< T >::iterator CStack< T >::Begin()
{
    return CStack< T >::iterator( this , 0 );
}

template< typename T >
inline typename CStack< T >::iterator CStack< T >::End()
{
    return CStack< T >::iterator( this , m_PushNum );
}

template< typename T >
inline typename CStack< T >::iterator CStack< T >::Erases( typename CStack< T >::iterator &it )
{
	s32 cursor = it.Delete();
    return CStack< T >::iterator( this , cursor );
}

template< typename T >
inline skyBool CStack< T >::SetItCursor( u32 idx , T* &data ) const
{
	//要素がない場合は何もせず返す
	if ( m_PushNum == 0 ) return skyFalse;

    SKY_ASSERT_MSG( idx < m_PushNum , _T( "index over. idx=[%d], pushNum=[%d]" ) , idx , m_PushNum );

	//選択
	_SetCursor( idx , data );

	return skyTrue;
}

template< typename T >
inline skyBool CStack< T >::DeleteIdx( u32 idx )
{
	idx;
	SKY_PANIC_MSG( _T( "This function is invalid." ) ); //この関数は無効です。何故呼んだんですか？

	return skyFalse;
}

template< typename T >
inline skyBool	CStack< T >::_SetCursor( u32 idx , T* &pCur ) const
{
	//データ先頭アドレスを数値にして、１個の格納サイズ×格納数を掛けたものを数値として足す
	pCur = ( T * )( ( u32 )( m_pData ) + ( idx * sizeof( T ) ) );

	return skyTrue;
}

} }
