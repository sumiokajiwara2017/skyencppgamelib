namespace sky { namespace lib {

inline SLinkListSize::SLinkListSize( u32 blockSize ) : m_pTop( skyNull ) , m_uiBlockSize( blockSize )
{
    SKY_ASSERT_MSG( m_uiBlockSize >= sizeof( Item ) , _T( "The size doesn't suffice." ) );
}

inline void SLinkListSize::Link( void *pItem )
{
	Item *pNewItem = ( Item * )pItem;

	//�R���X�g���N�^���������Ȃ��̂Ŏ����ŌĂԁG
	pNewItem->Constructor();

	if ( m_pTop == skyNull )
	{//��������̏ꍇ

		//�擪�Ɏw��
		m_pTop = pNewItem;
	}
	else
	{
		//�擪��V�l�̎��ɂ���
		pNewItem->m_pNext = m_pTop;

		//�V�l���擪�ɂȂ�
		m_pTop = pNewItem;
	}
}

inline void *SLinkListSize::GetTop()
{
	//����Top��skyNull��������1�����o�^���Ȃ��̂�skyNull��Ԃ�
	if ( m_pTop == skyNull )
	{
		return skyNull;
	}

	//�擪�̃A�h���X��߂�l�p���[�N��
	Item *pResult = m_pTop;

	//���������Ȃ�������
	if ( pResult->m_pNext == skyNull )
	{//Top��Null�ɂ���
		m_pTop = skyNull;
	}
	else
	{//Top�̎���Top�ɂ���
		m_pTop = pResult->m_pNext;
	}

	//�����������w��̃A�h���X���g�p���ăC���X�^���X���\�z���A�R���X�g���N�^����������E�E�͂��B
	return ( void * )pResult;
}

} }
