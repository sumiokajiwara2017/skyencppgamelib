namespace sky { namespace lib {

inline void CMemoryPoolSize::CreateBuffer( u32 poolNum )
{
	//�|�C���^���]�v�Ɏ��
	u8* pBuffer = ( u8 * )SkyMalloc( sizeof( void * ) + ( m_uiBlockSize * poolNum ) );

	//�f�[�^�̐擪�|�C���^�����o��
	u8 *pBufferTop = pBuffer + sizeof( void * );

	//�m�ۂ����������̗v�f�����ׂăC���X�^���X���X�g�ɓo�^����
	void *pBlock = pBufferTop;
	for ( u32 i = 0 ; i < poolNum ; i++ )
	{
		m_InstanceList.Link( pBlock );
		pBlock = ( u8 * )pBlock + m_uiBlockSize;
	}

	//�o�b�t�@��P�������X�g�ɓo�^����
	m_BufferList.Link( pBuffer );
}

inline CMemoryPoolSize::CMemoryPoolSize( u32 blockSize , u32 poolNum ) : 
m_uiBlockSize( blockSize ) ,
m_InstanceList( blockSize ) ,
m_PoolNum( poolNum ) ,
m_BufferList( sizeof( void * ) + ( blockSize * poolNum ) )
{
	SKY_ASSERT_MSG( poolNum > 0 , _T( "Please set the value of one or more to the number of pools." ) ); //�v�[������1�ȏ�̒l��ݒ肵�Ă��������B

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
	//�C���X�^���X���X�g����l���擾
	void *pResult = m_InstanceList.GetTop();

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

inline void CMemoryPoolSize::Delete( void *value )
{
	m_InstanceList.Link( value );
}

} }