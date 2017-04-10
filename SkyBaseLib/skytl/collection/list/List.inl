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
	//�S�폜
	DeleteAll();
}

template< typename KEY , typename DATA , class ThreadingModel >
inline DATA &CList< KEY , DATA , ThreadingModel >::AddFront()
{
	thread::CriticalLock Lock( this->m_pCs );

	//�V�����̈�̊m��
	ListItem *pNewItem = _CreateItem();

	if ( !m_pHead )
	{//�ŏ��̈��

		m_pHead = pNewItem;
		m_pTail = pNewItem;
	}
	else
	{//�Q�ڈȍ~

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

	//�V�����̈�̊m��
	ListItem *pNewItem = _CreateItem();

	if ( !m_pHead )
	{//�ŏ��̈��

		m_pHead = pNewItem;
		m_pTail = pNewItem;
	}
	else
	{//�Q�ڈȍ~

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

	//�V�����̈�̊m��
	ListItem *pNewItem = _CreateItem( id , data);

	if ( !m_pHead )
	{//�ŏ��̈�͂h�c�Ƃ����\���Ȃ��Œǉ�
		m_pHead = pNewItem;
		m_pTail = pNewItem;
	}
	else
	{
		//�擪������`�ŒT���i�Ȃ񂩍H�v�͂ł��񂩂ȁj
		ListItem *pCur = m_pHead;
		skyBool isFind = skyFalse;
		while( pCur )
		{
			if ( pCur->id == id )
			{
				//�������l�������Ă��̎��̒l�����݂��Ȃ������ꍇ���̓��l�̖����ɓo�^���ďI��
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

				//������ʂ�Ƃ������Ƃ͓��l��T�����E�E�E�E
			}
			else if ( pCur->id > id )
			{//id�������Ă��܂����炻�̑O�ɓo�^����
				pNewItem->pPrev = pCur->pPrev;
				pCur->pPrev = pNewItem;
				pNewItem->pNext	= pCur;

				//�����V�����f�[�^���擪���O�Ȃ�V�����擪�Ƃ���
				if ( m_pHead == pCur )
				{
					m_pHead = pNewItem;
				}

				break;
			}
			else
			{//id����������̂ł��̖����ɓo�^����

				//�������l�������Ă��̎��̒l�����݂��Ȃ������ꍇ���̓��l�̖����ɓo�^���ďI��
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

	//�V�����̈�̊m��
	ListItem *pNewItem = _CreateItem( data );

	if ( !m_pHead )
	{//�ŏ��̈�͂h�c�Ƃ����\���Ȃ��Œǉ�
		m_pHead = pNewItem;
		m_pTail = pNewItem;
	}
	else
	{
		//�擪������`�ŒT���i�Ȃ񂩍H�v�͂ł��񂩂ȁj
		ListItem *pCur = m_pHead;
		while( pCur )
		{
			if ( pCur->data == frontData )
			{
				//�����������l��������������V�����������𖖔��Ƃ���
				if ( pCur->pNext == skyNull )
				{
					m_pTail = pNewItem;
				}
				//�������l�̌��ɂ��o�^
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

	//�v�f���Ȃ��ꍇ�͉��������Ԃ�
	if ( m_nAddNum == 0 ) return skyFalse;

    SKY_ASSERT_MSG( idx < m_nAddNum , _T( "index over. idx=[%d], addNum=[%d]" ), idx , m_nAddNum );

	ListItem		*pCur;

	if ( m_nAddNum / 2 > idx )
	{//�O����idx���O������H�낤
		_SetCursorSearchStartHeadIdx( idx , pCur );
	}
	else
	{//�㔼��idx���㔼����H�낤
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

	//�v�f���Ȃ��ꍇ�͉��������Ԃ�
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

	//�v�f���Ȃ��ꍇ�͉��������Ԃ�
	if ( m_nAddNum == 0 ) return skyTrue;

	ListItem		*pCur;

	//�f�[�^�ŃJ�[�\���ݒ�
	if ( _SetCursor( data , pCur ) == skyFalse )
	{
		return skyTrue;
	}

	//�����̎����ꎞ�ۑ�
	ListItem *pSaveNext = pCur->pNext;

	if ( pCur->pPrev )
	{//�O�����݂���ꍇ�O�̎��������̎��ɂ���
		pCur->pPrev->pNext = pCur->pNext;
	}
	else
	{//�O�����݂��Ȃ��ꍇ�i�擪�j�����̎���擪�ɂ���
		m_pHead = pCur->pNext;
	}

	if ( pCur->pNext )
	{//�������݂���ꍇ�A���̑O�������̑O�Œu��������
		pCur->pNext->pPrev = pCur->pPrev;
	}
	else
	{//�������݂��Ȃ��ꍇ�i��Ԍ��̏ꍇ�j�����̑O����Ԍ��ɂ���
		m_pTail = pCur->pPrev;
	}

	//�v�f�̊J��
	_DelCursor( pCur );

	//�����̎����J�[�\���ɒu���i���[�v�����̉ߒ��ō폜���ꂽ�\�����������߁j
	pCur = pSaveNext;

	m_nAddNum--;

	return skyTrue;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline skyBool CList< KEY , DATA , ThreadingModel >::DeleteTail()
{
	//�v�f���Ȃ��ꍇ�͉��������Ԃ�
	if ( m_nAddNum == 0 ) return skyTrue;

	ListItem		*pCur;

	//�v�f����̏ꍇ�͏����ďI���
	if ( m_nAddNum == 1 )
	{
		//m_pHead�ł�m_pTail�ł��ǂ����ł��������E�E
		pCur = m_pTail;

		//�J�[�\�����폜
		_DelCursor( pCur );
	}
	else
	{
		//�Ō�̗v�f�̑O�̗v�f�̎�������
		m_pTail->pPrev->pNext = NULL;

		//�Ō�̗v�f�̈�O��V�v�f�Ƃ���B
		ListItem *pNewTail = m_pTail->pPrev;

		//���݂̖���������
		pCur = m_pTail;

		//�J�[�\�����폜
		_DelCursor( pCur );

		//�V�����������Z�b�g
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
			//����ۑ�
			pNext = pCur->pNext;

			//�v�f�̊J��
			_DelCursor( pCur );

			//�����T�[�`
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

	//�v�f���Ȃ��ꍇ�͉��������Ԃ�
	if ( m_nAddNum == 0 ) return skyNull;

	//�擪����T��
	DATA	*pCurData;
	_SetCursorSearchStartHead( id , pCurData );

	return *pCurData;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline DATA &CList< KEY , DATA , ThreadingModel >::SetCursorSearchStartTail( KEY id )
{
	thread::CriticalLock Lock( this->m_pCs );

	//�v�f���Ȃ��ꍇ�͉��������Ԃ�
	if ( m_nAddNum == 0 ) return skyNull;

	//�擪����T��
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

	//�v�f������ǂ��炩��H�����ق������������肷��

	ListItem *pCur;
	if ( (u32)m_nAddNum / 2 > idx ) //�V�t�g���Z�����������R���p�C�����œK�����邾�낤
	{//�O����idx���O������H�낤
		_SetCursorSearchStartHeadIdx( idx , pCur );
	}
	else
	{//�㔼��idx���㔼����H�낤
		_SetCursorSearchStartTailIdx( idx , pCur );
	}

	data = &pCur->data;

	return skyTrue;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline skyBool CList< KEY , DATA , ThreadingModel >::DeleteIdx( u32 idx )
{
	thread::CriticalLock Lock( this->m_pCs );

	//�v�f���Ȃ��ꍇ�͉��������Ԃ�
	if ( m_nAddNum == 0 ) return skyTrue;

    SKY_ASSERT_MSG( idx < ( u32 )m_nAddNum , _T( "index over. idx=[%d], addNum=[%d]" ), idx , m_nAddNum );

	ListItem		*pCur;

	if ( m_nAddNum / 2 > idx )//�V�t�g���Z�����������R���p�C�����œK�����邾�낤
	{//�O����idx���O������H�낤
		_SetCursorSearchStartHeadIdx( idx , pCur );
	}
	else
	{//�㔼��idx���㔼����H�낤
		_SetCursorSearchStartTailIdx( idx , pCur );
	}

	if ( pCur->pPrev )
	{//�O�����݂���ꍇ�O�̎��������̎��ɂ���
		pCur->pPrev->pNext = pCur->pNext;
	}
	else
	{//�O�����݂��Ȃ��ꍇ�i�擪�j�����̎���擪�ɂ���
		m_pHead = pCur->pNext;
	}

	if ( pCur->pNext )
	{//�������݂���ꍇ�A���̑O�������̑O�Œu��������
		pCur->pNext->pPrev = pCur->pPrev;
	}
	else
	{//�������݂��Ȃ��ꍇ�i��Ԍ��̏ꍇ�j�����̑O����Ԍ��ɂ���
		m_pTail = pCur->pPrev;
	}

	//�v�f�̊J��
	_DelCursor( pCur );

	m_nAddNum--;

	return skyTrue;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline typename CList< KEY , DATA , ThreadingModel >::ListItem* CList< KEY , DATA , ThreadingModel >::_CreateItem( KEY id , const DATA &data )
{
	//�V�����̈�̊m��
	ListItem *pNewItem	= m_kItemPool.Create();	//���X�g�̗v�f�̍쐬
	pNewItem->pPrev = skyNull;
	pNewItem->pNext = skyNull;

	//�f�[�^�̃R�s�[
	pNewItem->data = data;			//�N���X�Ȃ�R�s�[�R���X�g���N�^����

	//ID������
	pNewItem->id = id;

	return pNewItem;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline typename CList< KEY , DATA , ThreadingModel >::ListItem* CList< KEY , DATA , ThreadingModel >::_CreateItem( KEY id )
{
	//�V�����̈�̊m��
	ListItem *pNewItem	= m_kItemPool.Create();	//���X�g�̗v�f�̍쐬
	pNewItem->pPrev = skyNull;
	pNewItem->pNext = skyNull;

	//ID������
	pNewItem->id = id;

	return pNewItem;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline typename CList< KEY , DATA , ThreadingModel >::ListItem* CList< KEY , DATA , ThreadingModel >::_CreateItem( const DATA &data )
{
	//�V�����̈�̊m��
	ListItem *pNewItem	= m_kItemPool.Create();	//���X�g�̗v�f�̍쐬
	pNewItem->pPrev = skyNull;
	pNewItem->pNext = skyNull;

	//�f�[�^�̃R�s�[
	pNewItem->data = data;			//�N���X�Ȃ�R�s�[�R���X�g���N�^����

	return pNewItem;
}

template< typename KEY , typename DATA , class ThreadingModel >
inline typename CList< KEY , DATA , ThreadingModel >::ListItem* CList< KEY , DATA , ThreadingModel >::_CreateItem()
{
	//�V�����̈�̊m��
	ListItem *pNewItem	= m_kItemPool.Create();	//���X�g�̗v�f�̍쐬
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
	skyBool isFind = skyFalse; //������
	while( pCur )
	{
		if ( pCur->id == id )
		{
			//�������l�̂h�c���������̑O���������݂��Ȃ������炻���ŏI��
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
			pCur = skyNull; //���̂h�c�̃A�C�e���͖����B
			break;
		}
		else
		{
			if ( isFind == skyTrue )
			{//�h�c�S�̐擪�𑨂���
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
	//�l���폜
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
