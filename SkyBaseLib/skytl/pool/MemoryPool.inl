namespace sky { namespace lib {

template< class T >
inline void CMemoryPool< T >::CreateBuffer( u32 poolNum )
{
	//ポインタ分余計に取る
	u8* pBuffer = ( u8 * )SkyMalloc_( sizeof( void * ) + ( sizeof( T ) * poolNum ) );

	//データの先頭ポインタを取り出す
	T *pBufferTop = ( T * )( pBuffer + sizeof( void * ) );

	//確保したメモリの要素をすべてインスタンスリストに登録する
	for ( u32 i = 0 ; i < poolNum ; i++ )
	{
		m_InstanceList.Link( &pBufferTop[ i ] );
	}

	//バッファを単方向リストに登録する
	m_BufferList.Link( pBuffer );
}

template< class T >
inline CMemoryPool< T >::CMemoryPool( u32 poolNum ) : 
m_PoolNum( poolNum ) , 
m_BufferList( sizeof( void * ) + ( sizeof( T ) * poolNum ) )
{
	SKY_ASSERT_MSG( poolNum > 0 , _T( "Please set the value of one or more to the number of pools." ) ); //プール数は1以上の値を設定してください。

	CreateBuffer( poolNum );
}

template< class T >
inline CMemoryPool< T >::~CMemoryPool()
{
	void* pBuffer = NULL;
	while( ( pBuffer = m_BufferList.GetTop() ) != NULL )
	{
		SkyFree( pBuffer );
	}
}

template< class T >
inline T *CMemoryPool< T >::Create()
{
	//インスタンスリストから値を取得
	T *pResult = m_InstanceList.GetTop();

	//帰ってこなかったらバッファを拡張
	if ( pResult == skyNull )
	{
		//バッファを拡張
		CreateBuffer( m_PoolNum );

		//あらためてインスタンスリストから取得（絶対取れる）
		pResult = m_InstanceList.GetTop();
	}

	return pResult;
}

template< class T >
inline void CMemoryPool< T >::Delete( T *value )
{
	//自分でデストラクタを呼ぶ
	value->~T();

	m_InstanceList.Link( value );
}

template< class T >
inline u32 CMemoryPool< T >::GetPoolNum() const
{
	return m_PoolNum;
}

} }