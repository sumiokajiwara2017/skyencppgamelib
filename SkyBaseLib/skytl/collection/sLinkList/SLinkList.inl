namespace sky { namespace lib {

template< class T >
SLinkList< T >::SLinkList() : m_pTop( skyNull )
{
}

template< class T >
void SLinkList< T >::Link( T *pItem )
{
	SKY_ASSERT_MSG( sizeof( T ) >= sizeof( Item ) , _T( "The size of a specified type is too small." ) ); //�w��̌^�̓T�C�Y�����������ĒP�������X�g���\���ł��܂���B�^�̃����o��void* dummy�����`���āA�Œ���P�A�h���X���i�[�ł���傫�������^�ɂ��Ďg�p���Ă��������B

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

template< class T >
T *SLinkList< T >::GetTop()
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
	return new( pResult ) T;
}

} }