namespace sky { namespace lib {

inline void CMemoryPoolSize::CreateBuffer( u32 poolNum )
{
	//ポインタ分余計に取る
	u8* pBuffer = ( u8 * )SkyMalloc( sizeof( void * ) + ( m_uiBlockSize * poolNum ) );

	//データの先頭ポインタを取り出す
	u8 *pBufferTop = pBuffer + sizeof( void * );

	//確保したメモリの要素をすべてインスタンスリストに登録する
	void *pBlock = pBufferTop;
	for ( u32 i = 0 ; i < poolNum ; i++ )
	{
		m_InstanceList.Link( pBlock );
		pBlock = ( u8 * )pBlock + m_uiBlockSize;
	}

	//バッファを単方向リストに登録する
	m_BufferList.Link( pBuffer );
}

inline CMemoryPoolSize::CMemoryPoolSize( u32 blockSize , u32 poolNum ) : 
m_uiBlockSize( blockSize ) ,
m_InstanceList( blockSize ) ,
m_PoolNum( poolNum ) ,
m_BufferList( sizeof( void * ) + ( blockSize * poolNum ) )
{
	SKY_ASSERT_MSG( poolNum > 0 , _T( "Please set the value of one or more to the number of pools." ) ); //プール数は1以上の値を設定してください。

	CreateBuffer( poolNum );
}

inline CMemoryPoolSize::~CMemoryPoolSize()
{
	void* pBuffer = NULL;
	while( ( pBuffer = m_BufferList.GetTop() ) != NULL )
	{
		SkyFree( pBuffer );
	}
}

inline void *CMemoryPoolSize::Create()
{
	//インスタンスリストから値を取得
	void *pResult = m_InstanceList.GetTop();

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

inline void CMemoryPoolSize::Delete( void *value )
{
	m_InstanceList.Link( value );
}

} }