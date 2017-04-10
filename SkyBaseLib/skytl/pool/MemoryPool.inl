namespace sky { namespace lib {

template< class T >
inline void CMemoryPool< T >::CreateBuffer( u32 poolNum )
{
	//�|�C���^���]�v�Ɏ��
	u8* pBuffer = ( u8 * )SkyMalloc_( sizeof( void * ) + ( sizeof( T ) * poolNum ) );

	//�f�[�^�̐擪�|�C���^�����o��
	T *pBufferTop = ( T * )( pBuffer + sizeof( void * ) );

	//�m�ۂ����������̗v�f�����ׂăC���X�^���X���X�g�ɓo�^����
	for ( u32 i = 0 ; i < poolNum ; i++ )
	{
		m_InstanceList.Link( &pBufferTop[ i ] );
	}

	//�o�b�t�@��P�������X�g�ɓo�^����
	m_BufferList.Link( pBuffer );
}

template< class T >
inline CMemoryPool< T >::CMemoryPool( u32 poolNum ) : 
m_PoolNum( poolNum ) , 
m_BufferList( sizeof( void * ) + ( sizeof( T ) * poolNum ) )
{
	SKY_ASSERT_MSG( poolNum > 0 , _T( "Please set the value of one or more to the number of pools." ) ); //�v�[������1�ȏ�̒l��ݒ肵�Ă��������B

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
	//�C���X�^���X���X�g����l���擾
	T *pResult = m_InstanceList.GetTop();

	//�A���Ă��Ȃ�������o�b�t�@���g��
	if ( pResult == skyNull )
	{
		//�o�b�t�@���g��
		CreateBuffer( m_PoolNum );

		//���炽�߂ăC���X�^���X���X�g����擾�i��Ύ���j
		pResult = m_InstanceList.GetTop();
	}

	return pResult;
}

template< class T >
inline void CMemoryPool< T >::Delete( T *value )
{
	//�����Ńf�X�g���N�^���Ă�
	value->~T();

	m_InstanceList.Link( value );
}

template< class T >
inline u32 CMemoryPool< T >::GetPoolNum() const
{
	return m_PoolNum;
}

} }