namespace sky { namespace lib {

template< typename T >
inline CDictionary< T >::~CDictionary()
{
	SAFE_DELETE( m_pData );
}

template< typename T >
inline CDictionary< T >::CDictionary( u32 row , u32 col ) :
m_pData( skyNull ) ,
m_Col( 0 ) ,
m_Row( 0 )
{
	if ( row * col > 0 )
	{
		SizeChange( row , col );
	}
}

template< typename T >
inline void CDictionary< T >::Reset( u32 row , u32 col )
{
	SAFE_DELETE( m_pData );
	if ( row * col > 0 )
	{
		SizeChange( row , col );
	}
}

template< typename T >
inline void CDictionary< T >::SizeChange( u32 row , u32 col )
{
	if ( m_pData == skyNull || row > m_Row || col > m_Col )
	{
		u32 saveRow = m_Row;
		u32 saveCol = m_Col;

		T *pNewData = skyNull;

		if ( row > m_Row && col > m_Col )
		{
			pNewData = NEW T[ row * col ];
			m_Row = row;
			m_Col = col;
		}
		else if ( row > m_Row )
		{
			pNewData = NEW T[ row * m_Col ];
			m_Row = row;
		}
		else if ( col > m_Col )
		{
			pNewData = NEW T[ m_Row * col ];
			m_Col = col;
		}

		SKY_ASSERT( pNewData != skyNull );

		if ( m_pData != skyNull )
		{
			//値をコピー
			for ( u32 i = 0 ; i < saveRow ; i++ )
			{
				for ( u32 j = 0 ; j < saveCol ; j++ )
				{
					pNewData[ ( i * j ) + j ] = m_pData[ ( i * j ) + j ];
				}
			}

			SAFE_DELETE( m_pData );
		}

		m_pData = pNewData; 
	}
}

template< typename T >
inline u32 CDictionary< T >::GetColNum() const
{
	return m_Col;
}

template< typename T >
inline u32 CDictionary< T >::GetRowNum() const
{
	return m_Row;
}

template< typename T >
inline T &CDictionary< T >::Get( u32 row , u32 col ) const
{
	SKY_ASSERT( row < m_Row );
	SKY_ASSERT( col < m_Col );

	return m_pData[ ( row * col ) + col ];
}

template< typename T >
inline void CDictionary< T >::Set( const T &val , u32 row , u32 col )
{
	SizeChange( row + 1 , col + 1 );

	m_pData[ ( row * col ) + col ] = val;
}

template< typename T >
inline CDictionary< T >& CDictionary< T >::operator =( const CDictionary< T >& src )
{
	if ( src.m_pData != skyNull )
	{
		SizeChange( src.m_Row , src.m_Col );

		//値をコピー
		for ( u32 i = 0 ; i < m_Row ; i++ )
		{
			for ( u32 j = 0 ; j < m_Col ; j++ )
			{
				m_pData[ ( i * j ) + j ] = src.m_pData[ ( i * j ) + j ];
			}
		}
	}
	else
	{
		m_Row = 0;
		m_Col = 0;
		SAFE_DELETE( m_pData );
	}
	return *this;
}

template< typename T >
inline CDictionary< T >::CDictionary( const CDictionary< T >& obj )
{
	*this = obj;
}

template<>
inline void CDictionaryPrimitive< dectype >::DebugPrint()
{
	for ( u32 i = 0 ; i < m_Row ; i++ )
	{
		for ( u32 j = 0 ; j < m_Col ; j++ )
		{
			SKY_PRINTF( "[ %d , %d ] = %f \n" , i , j , Get( i , j )  );
		}
	}
}

template< typename T >
inline void CDictionaryPrimitive< T >::SizeChange( u32 row , u32 col )
{
	if ( m_pData == skyNull || row > m_Row || col > m_Col )
	{
		u32 saveRow = m_Row;
		u32 saveCol = m_Col;

		T *pNewData = skyNull;

		if ( row > m_Row && col > m_Col )
		{
			pNewData = NEW T[ row * col ];
			m_Row = row;
			m_Col = col;
		}
		else if ( row > m_Row )
		{
			pNewData = NEW T[ row * m_Col ];
			m_Row = row;
		}
		else if ( col > m_Col )
		{
			pNewData = NEW T[ m_Row * col ];
			m_Col = col;
		}

		SKY_ASSERT( pNewData != skyNull );

		if ( m_pData != skyNull )
		{
			SKY_ASSERT( sizeof( T ) * row * col > 0 );

			//値をコピー
			CMemoryUtil_::Copy( pNewData , m_pData , sizeof( T ) * saveRow * saveCol );

			SAFE_DELETE( m_pData );
		}

		m_pData = pNewData; 
	}
}

template< typename T >
inline CDictionaryPrimitive< T >::CDictionaryPrimitive( u32 row , u32 col )
{
	m_pData = skyNull;
	m_Col   = 0;
	m_Row   = 0;

	if ( row * col > 0 )
	{
		SizeChange( row , col );
	}
}

template< typename T >
inline CDictionaryPrimitive< T >& CDictionaryPrimitive< T >::operator =( const CDictionaryPrimitive< T >& src )
{
	if ( src.m_pData != skyNull )
	{
		SizeChange( src.m_Row , src.m_Col );

		SKY_ASSERT( sizeof( T ) * src.m_Row * src.m_Col > 0 );

		//値をコピー
		CMemoryUtil_::Copy( m_pData , src.m_pData , sizeof( T ) * src.m_Row * src.m_Col );
	}
	else
	{
		m_Row = 0;
		m_Col = 0;
		SAFE_DELETE( m_pData );
	}

	return *this;
}

template< typename T >
inline CDictionaryPrimitive< T >::CDictionaryPrimitive( const CDictionaryPrimitive< T >& obj )
{
	*this = obj;
}

template< typename T >
inline void CDictionaryPrimitive< T >::Set( const T &val , u32 row , u32 col )
{
	SizeChange( row + 1 , col + 1 );

	m_pData[ ( row * col ) + col ] = val;
}

} }