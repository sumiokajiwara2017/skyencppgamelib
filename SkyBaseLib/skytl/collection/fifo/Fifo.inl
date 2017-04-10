/// <summary>
/// 汎用型のＦＩＦＯクラスの実装
/// </summary>
namespace sky { namespace lib {

template< typename T , class ThreadingModel >
inline CFifo< T , ThreadingModel >::CFifo( s32 size )
{
	m_pDataBuffer	= NEW_ARRAY_() Item[ size + 1 ]; //ＦＩＦＯの深さ分領域を確保し、その先頭ポインタを保持
	m_BufferSize	= size + 1;
	m_PushNum		= 0;
	m_Rp			= 0;
	m_Wp			= 0;

	for ( u32 i = 0 ; i < m_BufferSize ; i++ )
	{
		m_pDataBuffer[ i ].isCancel = skyFalse;
	}
}

template< typename T , class ThreadingModel >
inline CFifo< T , ThreadingModel >::~CFifo()
{
	DEL_ARRAY m_pDataBuffer;
}

template< typename T , class ThreadingModel >
inline skyBool CFifo< T , ThreadingModel >::Push( const T &data )
{
	thread::CriticalLock Lock( this->m_pCs );

	//格納数溢れチェック
	u32 next = ( m_Wp + 1 ) % m_BufferSize;
	if( next == m_Rp )
	{
		SKY_PANIC_MSG( _T( "FIFO overflowed." ) ); //FIFO overflowed.
		return skyFalse;
	}

	//値を格納
	m_pDataBuffer[ m_Wp ].data = data;

	//数を増やす
	m_PushNum++;

	m_Wp = next;

	return skyTrue;
}

template< typename T , class ThreadingModel >
inline skyBool CFifo< T , ThreadingModel >::Push()
{
	thread::CriticalLock Lock( this->m_pCs );

	//格納数溢れチェック
	u32 next = ( m_Wp + 1 ) % m_BufferSize;
	if( next == m_Rp )
	{
		SKY_PANIC_MSG( _T( "FIFO overflowed." ) );
		return skyFalse;
	}

	//数を増やす
	m_PushNum++;

	m_Wp = next;

	return skyTrue;
}

template< typename T , class ThreadingModel >
inline skyBool CFifo< T , ThreadingModel >::IsFull()
{
	thread::CriticalLock Lock( this->m_pCs );

	//値がいっぱいならば値を消す
	u32 next = ( m_Wp + 1 ) % m_BufferSize;
	if( next == m_Rp )
	{
		return skyTrue;
	}
	else
	{
		return skyFalse;
	}
}

template< typename T , class ThreadingModel >
inline T &CFifo< T , ThreadingModel >::GetBack()
{
	SKY_ASSERT_MSG( m_Rp != m_Wp , _T( "FIFO is empty." ) ); //FIFO is empty.

	if ( m_Wp == 0 )
	{
		return m_pDataBuffer[ m_BufferSize - 1 ].data;
	}
	else
	{
		return m_pDataBuffer[ m_Wp - 1 ].data;
	}
}

template< typename T , class ThreadingModel >
inline skyBool CFifo< T , ThreadingModel >::Pop( T &dest )
{
	thread::CriticalLock Lock( this->m_pCs );

	if( m_Rp != m_Wp )  // リングバッファが空でない
	{   
		dest = m_pDataBuffer[ m_Rp ].data;
		m_pDataBuffer[ m_Rp ].isCancel = skyFalse;

		//FIFOを進める
		m_Rp = ( m_Rp + 1 ) % m_BufferSize;

		//数を減らす
		m_PushNum--;

		return skyTrue;
	}
	else
	{   // リングバッファが空
		return skyFalse;
	}
}

template< typename T , class ThreadingModel >
inline skyBool CFifo< T , ThreadingModel >::Pop()
{
	thread::CriticalLock Lock( this->m_pCs );

	if( m_Rp != m_Wp )  // リングバッファが空でない
	{   
		//FIFOを進める
		m_Rp = ( m_Rp + 1 ) % m_BufferSize;

		//数を減らす
		m_PushNum--;

		return skyTrue;
	}
	else
	{   // リングバッファが空
		return skyFalse;
	}
}

template< typename T , class ThreadingModel >
inline void CFifo< T , ThreadingModel >::SetCancelFlag( const T &data , skyBool isCancel )
{
	for ( u32 i = 0; i < m_PushNum; i++ )
	{
		if ( m_pDataBuffer[ m_Rp + i ].data == data )
		{
			m_pDataBuffer[ m_Rp + i ].isCancel = isCancel;
		}
	}
}

template< typename T , class ThreadingModel >
inline skyBool CFifo< T , ThreadingModel >::IsFrontCancel()
{
	SKY_ASSERT_MSG( m_Rp != m_Wp , _T( "FIFO is empty." ) );

	return m_pDataBuffer[ m_Rp ].isCancel;
}

template< typename T , class ThreadingModel >
inline T &CFifo< T , ThreadingModel >::GetFront()
{
	SKY_ASSERT_MSG( m_Rp != m_Wp , _T( "FIFO is empty." ) );

	return m_pDataBuffer[ m_Rp ].data;
}

template< typename T , class ThreadingModel >
inline void CFifo< T , ThreadingModel >::AllPop()
{
	thread::CriticalLock Lock( this->m_pCs );

	T dest;
	while( Pop( dest ) ){}
}

template< typename T , class ThreadingModel >
inline skyBool CFifo< T , ThreadingModel >::IsCancel( u32 idx ) const
{
	SKY_ASSERT_MSG( idx < m_PushNum , _T( "The value of the index is too large." ) ); //The value of the index is too large.

	return m_pDataBuffer[ m_Rp + idx ].isCancel;
}

template< typename T , class ThreadingModel >
inline skyBool CFifo< T , ThreadingModel >::GetItem( u32 idx , T* &data ) const
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( idx < m_PushNum , _T( "The value of the index is too large." ) );

	data = &m_pDataBuffer[ m_Rp + idx ].data;

	return skyTrue;
}

template< typename T , class ThreadingModel >
inline T &CFifo< T , ThreadingModel >::operator[]( s32 idx ) const
{
	thread::CriticalLock Lock( this->m_pCs );

	SKY_ASSERT_MSG( idx < m_PushNum , _T( "The value of the index is too large." ) );

	return m_pDataBuffer[ m_Rp + idx ].data;
}

template< typename T , class ThreadingModel >
inline skyBool CFifo< T , ThreadingModel >::IsNoData() const
{
	thread::CriticalLock Lock( this->m_pCs );

   return  ( m_Rp == m_Wp );
}

template< typename T , class ThreadingModel >
inline u32 CFifo< T , ThreadingModel >::GetNum() const
{ 
	thread::CriticalLock Lock( this->m_pCs );

	return m_PushNum;
}

template< typename T , class ThreadingModel >
inline u32 CFifo< T , ThreadingModel >::GetSize() const
{ 
	thread::CriticalLock Lock( this->m_pCs );

	return m_BufferSize;
}

template< typename T , class ThreadingModel >
inline CFifo< T , ThreadingModel >::iterator::iterator( CFifo< T , ThreadingModel > *pFifo , s32 cursor ) :
Iterator< CFifo< T , ThreadingModel > , T >( pFifo , cursor )
{
}

template< typename T , class ThreadingModel >
inline skyBool CFifo< T , ThreadingModel >::IsBeginBefor( iterator &it ) const
{
	return ( it.GetIdx() < 0 );
}

template< typename T , class ThreadingModel >
inline typename CFifo< T , ThreadingModel >::iterator CFifo< T , ThreadingModel >::Begin()
{
	thread::CriticalLock Lock( this->m_pCs );

	return iterator( this , 0 );
}

template< typename T , class ThreadingModel >
inline typename CFifo< T , ThreadingModel >::iterator CFifo< T , ThreadingModel >::End()
{
	thread::CriticalLock Lock( this->m_pCs );

	return iterator( this , m_PushNum );
}

template< typename T , class ThreadingModel >
inline typename CFifo< T , ThreadingModel >::iterator CFifo< T , ThreadingModel >::Erases( typename CFifo< T , ThreadingModel >::iterator &it )
{
	thread::CriticalLock Lock( this->m_pCs );

	s32 cursor = it.Delete();
	return iterator( this , cursor );
}

template< typename T , class ThreadingModel >
inline skyBool CFifo< T , ThreadingModel >::SetItCursor( u32 idx , T* &data ) const
{
	thread::CriticalLock Lock( this->m_pCs );

	//要素がない場合は何もせず返す
	if ( m_PushNum == 0 ) return skyFalse;

    SKY_ASSERT_MSG( idx < m_PushNum , _T( "index over. idx=[%d], pushNum=[%d]" ) , idx , m_PushNum );

	GetItem( idx , data );

	return skyTrue;
}

template< typename T , class ThreadingModel >
inline skyBool CFifo< T , ThreadingModel >::DeleteIdx( u32 idx )
{
	idx;
	SKY_PANIC_MSG( _T( "This function is invalid." ) ); //この関数は無効です。

	return skyFalse;
}

template< typename T >
inline CFifoMT< T >::CFifoMT( s32 size ) :
CFifo< T , thread::ThreadingModels::CMultiThread >( size )
{

}

template< typename T >
inline CFifoMT< T >::~CFifoMT()
{
}

} }
