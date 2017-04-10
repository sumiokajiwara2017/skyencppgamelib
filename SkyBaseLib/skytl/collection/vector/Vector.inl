namespace sky { namespace lib {

template< typename T >
inline CVector< T >::CVector( s32 size , u32 upSize )
{
	SKY_ASSERT_MSG( size > 0 , _T( "0 nor a minus value are allowed." ) ); //�O����у}�C�i�X�l�͋��e���܂���B

	m_pData = NEW_ARRAY_() T[ size ]; //�X�^�b�N�̐[�����̈���m�ۂ��A���̐擪�|�C���^��ێ�
	m_Size = size;
	m_nSetNum = 0;
	m_nUpSize = upSize;
}

template< typename T >
inline CVector< T >::CVector( const CVector& src )
{
	*this = src;
}

template< typename T >
inline typename CVector< T >& CVector< T >::operator =( const CVector< T >& src )
{
	Reset( src.m_Size );

	m_Size    = src.m_Size;
	m_nSetNum = src.m_nSetNum;
	m_nUpSize = src.m_nUpSize;
	for ( u32 i = 0 ; i < m_Size ; i++ )
	{
		m_pData[ i ] = src.m_pData[ i ];
	}
	return *this;
}

template< typename T >
inline CVector< T >::~CVector()
{
	DEL_ARRAY m_pData;
}

template< typename T >
T &CVector< T >::Add( const T &value, u32 *idx )
{
	return Add( &value , idx );
}

template< typename T >
T &CVector< T >::Add( const T *value, u32* idx )
{
	//���łɂ����ς���������z�����蒼��
	if ( m_Size == m_nSetNum )
	{
		//�V�����̈���m��
		T* pNewData = NEW_ARRAY T[ m_Size + m_nUpSize ];

		//�V�����̈�ɌÂ��̈���R�s�[
		for ( u32 i = 0 ; i < m_Size ; i++ )
		{
			pNewData[ i ] = m_pData[ i ];
		}

		if ( value != skyNull )
		{
			//�V�����̈�̍Ō��Add���ꂽ�l������
			pNewData[ m_nSetNum ] = *value;
		}

		//�Â��̈������
		DEL_ARRAY m_pData;

		//�V�����̈�������o�ϐ��ɑ��
		m_pData = pNewData;

		//�T�C�Y��ς���
		m_Size += m_nUpSize;
	}
	else
	{
		if ( value != skyNull )
		{
			//���݂̗̈�̊i�[�T�C�Y�{�P�̕����ɒl�𓊓�
			m_pData[ m_nSetNum ] = *value;
		}
	}

	//�󂯎���������΃C���f�b�N�X��߂�
	if ( idx != skyNull )
	{
		*idx = m_nSetNum;
	}

	//�i�[���𑝂₷
	m_nSetNum++;

	return m_pData[ m_nSetNum - 1 ];
}

template< typename T >
skyBool CVector< T >::Delete( const T &value )
{
	if ( m_nSetNum == 0 )
	{
		return skyFalse;
	}

	//�l��T��
	s32 index = -1;
	for( s32 i = 0 ; i < ( s32 )m_nSetNum ; i++ )
	{
		if ( m_pData[ i ] == value )
		{
			index = i;
		}
	}

	if ( index == -1 )
	{
		return skyFalse;
	}

	if ( m_nSetNum - 1 > ( u32 )index )
	{
		for( s32 i = index ; i < ( s32 )m_nSetNum - 1 ; i++ )
		{
			m_pData[ i ] = m_pData[ i + 1 ];
		}
	}

	m_nSetNum--;

	return skyTrue;
}

template< typename T >
skyBool CVector< T >::Index( const T &value , s32 &dest )
{
	if ( m_nSetNum == 0 )
	{
		return skyFalse;
	}

	//�l��T��
	s32 index = -1;
	for( s32 i = 0 ; i < ( s32 )m_nSetNum ; i++ )
	{
		if ( m_pData[ i ] == value )
		{
			index = i;
			break;
		}
	}

	if ( index == -1 )
	{
		return skyFalse;
	}

	dest = index;

	return skyTrue;
}

template< typename T >
skyBool CVector< T >::Get( u32 idx , T &dest )
{
	skyBool result = skyTrue;

	if ( m_nSetNum == 0 || m_nSetNum <= idx )
	{
		result = skyFalse;
	}
	else
	{
		dest = m_pData[ idx ];
	}

	return result;
}

template< typename T >
void CVector< T >::Clear( s32 size )
{
	SKY_ASSERT_MSG( size > 0 , _T( "0 nor a minus value are allowed." ) ); //�O����у}�C�i�X�l�͋��e���܂���B

	//�Â��̈������
	DEL_ARRAY m_pData;

	m_pData = NEW_ARRAY_() T[ size ]; //�X�^�b�N�̐[�����̈���m�ۂ��A���̐擪�|�C���^��ێ�
	m_Size = size;
	m_nSetNum = 0;
}

template< typename T >
void CVector< T >::Reset( u32 size )
{
	//�Â��̈������
	DEL_ARRAY m_pData;

	//�����̃T�C�Y�Ŏ�蒼��
	m_pData = NEW_ARRAY T[ size ]; //�X�^�b�N�̐[�����̈���m�ۂ��A���̐擪�|�C���^��ێ�
	m_Size = size;
	m_nSetNum = size;
}

template< typename T >
void CVector< T >::Assign( u32 size , T value )
{
	//��U�R���e�i�����Z�b�g
	Reset( size );

	//�w��̒l�ŏ㏑��
	for( u32 i = 0 ; i < m_Size ; i++ )
	{
		m_pData[ i ] = value;
	}
}

template< typename T >
u32 CVector< T >::GetSize() const
{
	return m_Size;
}

template< typename T >
u32 CVector< T >::GetNum() const
{
	return m_nSetNum;
}

template< typename T >
T &CVector< T >::operator[]( s32 idx ) const
{
	T *result;
	SetItCursor( idx , result );
	return *result;
}

template< typename T >
CVector< T >::iterator::iterator( CVector< T > *pVector, s32 cursor ) :
Iterator<CVector< T >,T>( pVector , cursor )
{
}

template< typename T >
inline skyBool CVector< T >::IsBeginBefor( iterator &it ) const
{
	return ( it.GetIdx() < 0 );
}

template< typename T >
typename CVector< T >::iterator CVector< T >::Begin()
{
    return CVector< T >::iterator( this , 0 );
}

template< typename T >
typename CVector< T >::iterator CVector< T >::End()
{
    return CVector< T >::iterator( this , m_nSetNum );
}

template< typename T >
typename CVector< T >::iterator CVector< T >::Erases( typename CVector< T >::iterator &it )
{
	s32 cursor = it.Delete();
    return CVector< T >::iterator( this , cursor );
}

template< typename T >
skyBool CVector< T >::SetItCursor( u32 idx , T* &data ) const
{
    SKY_ASSERT_MSG( idx < m_nSetNum , _T( "index over. idx=[%d], pushNum=[%d]" ) , idx , m_nSetNum );

	//�v�f���Ȃ��ꍇ�͉��������Ԃ�
	if ( m_nSetNum == 0 ) return skyFalse;

	//�I��
	_SetCursor( idx , data );

	return skyTrue;
}

template< typename T >
skyBool CVector< T >::DeleteIdx( u32 idx )
{
	if ( m_nSetNum == 0 )
	{
		return skyFalse;
	}

    SKY_ASSERT_MSG( idx < m_nSetNum , _T( "index over. idx=[%d], pushNum=[%d]" ) , idx , m_nSetNum );

	if ( m_nSetNum - 1 > idx )
	{
		for( s32 i = idx ; i < ( s32 )m_nSetNum - 1 ; i++ )
		{
			m_pData[ i ] = m_pData[ i + 1 ];
		}
	}

	m_nSetNum--;

	return skyTrue;
}

template< typename T >
skyBool	CVector< T >::_SetCursor( u32 idx , T* &pCur ) const
{
	//�f�[�^�擪�A�h���X�𐔒l�ɂ��āA�P�̊i�[�T�C�Y�~�i�[�����|�������̂𐔒l�Ƃ��đ���
	pCur = ( T * )( ( u32 )( m_pData ) + ( idx * sizeof( T ) ) );

	return skyTrue;
}

template< typename T >
T* CVector< T >::GetTopAdr() const
{
	return m_pData;
}

template< typename T >
T* CVector< T >::GetBottomAdr() const
{
	SKY_ASSERT_MSG( m_nSetNum > 0 , _T( "item num is zero. \n" ) );

	return m_pData + ( m_nSetNum );
}

template< typename T >
inline void CVector< T >::Copy( CVector &dest ) const
{
	//�R�s�[�悪�������ꍇ�͑傫������
	if ( m_Size > dest.m_Size )
	{
		dest.Reset( m_Size );
	}

	for ( u32 i = 0 ; i < m_Size ; i++ )
	{
		dest.m_pData[ i ] = m_pData[ i ];
	}
}

template< typename T >
inline CVectorPrimitive< T >::CVectorPrimitive( s32 size , u32 upSize ) : CVector< T >( size , upSize )
{
}

template< typename T >
inline CVectorPrimitive< T >::CVectorPrimitive( const CVectorPrimitive& src )
{
	*this = src;
}

template< typename T >
inline CVectorPrimitive< T >::~CVectorPrimitive()
{
}


template< typename T >
inline typename CVectorPrimitive< T >& CVectorPrimitive< T >::operator =( const CVectorPrimitive< T >& src )
{
	Reset( src.m_Size );

	m_Size    = src.m_Size;
	m_nSetNum = src.m_nSetNum;
	m_nUpSize = src.m_nUpSize;
	CMemoryUtil_::Copy( m_pData , src.m_pData ,  sizeof( T ) * src.m_Size );
	return *this;
}

template< typename T >
skyBool CVectorPrimitive< T >::Add( const T &value, u32 *idx )
{
	return Add( &value , idx );
}

template< typename T >
skyBool CVectorPrimitive< T >::Add( const T *value, u32* idx )
{
	//���łɂ����ς���������z�����蒼��
	if ( m_Size == m_nSetNum )
	{
		//�V�����̈���m��
		T* pNewData = NEW_ARRAY T[ m_Size + m_nUpSize ];

		CMemoryUtil_::Copy( pNewData , m_pData ,  sizeof( T ) * m_Size ); //�������̂ق����������A���̗v�f���R�s�[����ۂɑ�����Z�q�̃I�[�o�[���C�h��ʂ�Ȃ�

		if ( value != skyNull )
		{
			//�V�����̈�̍Ō��Add���ꂽ�l������
			pNewData[ m_nSetNum ] = *value;
		}

		//�Â��̈������
		DEL_ARRAY m_pData;

		//�V�����̈�������o�ϐ��ɑ��
		m_pData = pNewData;

		//�T�C�Y��ς���
		m_Size += m_nUpSize;
	}
	else
	{
		if ( value != skyNull )
		{
			//���݂̗̈�̊i�[�T�C�Y�{�P�̕����ɒl�𓊓�
			m_pData[ m_nSetNum ] = *value;
		}
	}

	//�󂯎���������΃C���f�b�N�X��߂�
	if ( idx != skyNull )
	{
		*idx = m_nSetNum;
	}

	//�i�[���𑝂₷
	m_nSetNum++;

	return skyTrue;
}

template< typename T >
inline void CVectorPrimitive< T >::Copy( CVector &dest ) const
{
	//�R�s�[�悪�������ꍇ�͑傫������
	if ( m_Size > dest.m_Size )
	{
		dest.Reset( m_Size );
	}

	CMemoryUtil_::Copy( dest.m_pData , m_pData ,  sizeof( T ) * m_Size );
}

} }
