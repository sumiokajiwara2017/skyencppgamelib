namespace sky { namespace lib {

template< typename T , typename KEY , class ThreadingModel >
inline CHash< T , KEY , ThreadingModel >::CHash( s32 hashSize , u32 poolNum ) :
m_kItemPool( poolNum )
{
	m_nHashSize = hashSize;

	//�e�[�u���쐬
	u32 tblSize = sizeof ( _HashListTbl * ) * hashSize;
	m_ppListTbl = ( _HashListTbl ** )SkyMalloc( tblSize ); //�|�C���^�e�[�u��������Ă邾��������Malloc�ł���
	CMemoryUtil_::ClearZero( m_ppListTbl , tblSize );	

	m_nSetNum = 0;
}

template< typename T , typename KEY , class ThreadingModel >
inline CHash< T , KEY , ThreadingModel >::~CHash()
{
	//�S�폜
	DeleteAll();
	SkyFree( m_ppListTbl ); //�ق�ƂɊ�������̂̓f�X�g���N�^����
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
//	_Set( id , id % m_nHashSize , data , CHS( CStringUtil_::ConvNumToString( id , buf , 64 ) ) ); //����������Ă��Ӗ��������̂ŏ���
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
					//�l�����B�Ԃ�
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
	{//�n�b�V���l���d�����Ă��Ȃ�

		pNew = m_kItemPool.Create();													//�V�������R�[�h�쐬
		pNew->id 		= id;
		pNew->idst 		= idst;

		pNew->data					= data;												//�l���R�s�[(�N���X�Ȃ�R�s�[�R���X�g���N�^�����j
		pNew->pNext 				= skyNull;
		m_ppListTbl[ hashId ] 		= pNew;    											//���X�g�̐擪���R�[�h�Ȃ̂ł��̂܂܃Z�b�g
	}
	else
	{//�n�b�V���l���d�������̂Ő��`�T�����K�v
		
		//�����̍��ڂ�T��
		_HashListTbl *pNow = m_ppListTbl[ hashId ];  										//�擪���Z�b�g
		while( pNow )
		{
			if( EqId( id , pNow ) )
			{//id�܂ŏd�������i�㏑���j
				pNow->data = data;														//�l���R�s�[�i�N���X�Ȃ�R�s�[�R���X�g���N�^�����j
				return skyTrue; 														//�I��
			}
			pNow = pNow->pNext;  														//���̃`�F�[����H��
		}

	   //�V����id�Ȃ̂Ő擪�ɒǉ�
		pNew = m_kItemPool.Create();													//�V�������R�[�h�쐬
		pNew->id   = id;
		pNew->idst = idst;
		pNew->data = data;																//�l���R�s�[

		pNew->pNext	= m_ppListTbl[ hashId ];  											// �ȑO�̐擪�������̎��ɂ���
		m_ppListTbl[ hashId ]	= pNew;    												// �擪�ɒǉ�
	}
	m_nSetNum++;
	return skyTrue;
}

template< typename T , typename KEY , class ThreadingModel >
inline skyBool CHash< T , KEY , ThreadingModel >::_Get( u32 id , u32 hashId , T* &data ) const
{
	if( m_ppListTbl[ hashId ] == skyNull )
	{//�Y���̃n�b�V���l�ɑ΂���l������
	   return skyNull;
	}
	else
	{//�n�b�V���l�̒l�͂���A���Ƃ�id����v���邩��
		_HashListTbl *pNow = m_ppListTbl[ hashId ];  										//�擪���Z�b�g
		while( pNow )
		{
			if( EqId( id , pNow ) )
			{//����
				data = &pNow->data;
				return skyTrue;
			}
			pNow = pNow->pNext;															//���̃`�F�[����H��
		}
		return skyFalse;//�����n�b�V���l�̒��Ԃ͋�����id�͂Ȃ������B
	}
}

template< typename T , typename KEY , class ThreadingModel >
inline skyBool CHash< T , KEY , ThreadingModel >::_Delete(  u32 id , u32 hashId )
{
	//���`�T�����K�v
	_HashListTbl *pNow = m_ppListTbl[ hashId ];  											//�擪���Z�b�g
	_HashListTbl *pPrev = skyNull;															//�O�̗v�f
	_HashListTbl *pNext = skyNull;															//���̗v�f
	while( pNow )
	{
		pNext = pNow->pNext; 															//����ۑ����Ă���
		if( EqId( id , pNow ) )
		{//�폜�Ώۂ𔭌�

			//�v�f������
			_DelList( pNow );
			
			//�O�̗v�f�̎��Ɏ��̗v�f���i�[����
			if ( pPrev != skyNull )
			{
				pPrev->pNext = pNext;
			}
			else
			{
				//�O�̗v�f���Ȃ������A�܂�擪�������̂Ő擪�Ɏ��̗v�f���i�[�iskyNull�ł�OK)
				m_ppListTbl[ hashId ] = pNext;
			}
			m_nSetNum--;
			return skyTrue;																//���������܂����I
		}
		else
		{
			pPrev = pNow;																//�O��ۑ����Ă���
		}
		pNow = pNext;																	//���̃`�F�[����H��
	}

	return skyFalse;																	//�������̂�����܂���ł����B
}

template< typename T , typename KEY , class ThreadingModel >
inline void CHash< T , KEY , ThreadingModel >::_DelList( _HashListTbl *pList )
{
	//�l���폜
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
