namespace sky { namespace lib { namespace kfm {

template < class T >
inline CFKey< T >::CFKey( T value )
{
	m_Value.m_Value = value;
}

template < class T >
inline typename CFValue< T > &CFKey< T >::GetValue()
{
	return m_Value;
}

template < class T >
inline T& CFKey< T >::operator *()
{
	return *m_Value;
}

template < class T >
inline T* CFKey< T >::operator &()
{
	return &( *m_Value );
}

} } }