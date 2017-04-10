namespace sky { namespace lib {

//-------------------------- 戻り値クラスベース--------------------------------▽

template < u64 size >
COptionalBase< size >::COptionalBase() : m_bValid( skyFalse )
{
}

template < u64 size >
COptionalBase< size >::COptionalBase( COptionalBase const & other ) : m_bValid( other.m_bValid )
{
}

template < u64 size >
COptionalBase< size > &COptionalBase< size >::operator = ( COptionalBase const & t )
{
	m_bValid = t.m_bValid;
	return * this;
}

template < u64 size >
skyBool const COptionalBase< size >::Valid() const
{ 
	return m_bValid;
}

template < u64 size >
skyBool const COptionalBase< size >::Invalid() const
{ 
	return !m_bValid;
}

//--------------------------戻り値クラス--------------------------------------▽

template < class T >
COptional< T >::COptional ()
{
}

template < class T >
COptional< T >::COptional ( T const & t )
{ 
        Construct( t );
        this->m_bValid = (true);
}

template < class T >
COptional< T >::COptional ( COptional_empty const &)
{
}

template < class T >
COptional< T >::COptional ( COptional const & other )
{
	if ( other.m_bValid )
	{
		Construct( * other );
        this->m_bValid = true;	// order important for exception safety.
	}
}

template < class T >
COptional< T > &COptional< T >::operator = ( T const & t )
{
    if ( this->m_bValid )
    {
        * GetT() = t;
    }
    else
    {
        Construct( t );
                this->m_bValid = true;	// order important for exception safety.
    }

    return * this;
}

template < class T >
COptional< T > &COptional< T >::operator = ( COptional const & other )
{
	SKY_ASSERT_MSG( !( this == &other ) , _T( "don't copy over self!" ) );
        if ( this->m_bValid )
	{							// first, have to destroy our original.
        this->m_bValid = skyFalse;	// for exception safety if destroy() throws.
								// (big trouble if destroy() throws, though)
		Destroy();
	}

	if ( other.m_bValid )
	{
		Construct( * other );
                this->m_bValid = skyTrue;	// order vital.

	}
	return * this;
}

template < class T >
skyBool const COptional< T >::operator == ( COptional const & other ) const
{
        if ( ( !this->valid() ) && ( !other.valid() ) ) { return skyTrue; }

        if ( this->valid() ^ other.valid() ) { return skyFalse; }

	return ( ( **this ) == ( * other ) );
}

template < class T >
skyBool const COptional< T >::operator < ( COptional const & other) const
{
	// equally invalid - not smaller.
        if ( ( !this->valid() ) && ( !other.valid() ) )
	{ 
		return skyFalse;
	}

	// I'm not valid, other must be, smaller.
        if ( !this->valid() )
	{
		return skyTrue;
	}

	// I'm valid, other is not valid, I'm larger
	if ( !other.valid() )
	{ 
		return skyFalse;
	}

	return ( ( ** this ) < ( * other ) );
}

template < class T >
COptional< T >::~COptional() 
{ 
        if ( this->m_bValid ) Destroy();
}

template < class T >
T const &COptional< T >::operator * () const
{ 
        SKY_ASSERT( this->m_bValid );
	return * GetT();
}

template < class T >
T &COptional< T >::operator * ()
{ 
        SKY_ASSERT( this->m_bValid );
	return * GetT();
}

template < class T >
T const * const COptional< T >::operator -> () const
{ 
        SKY_ASSERT( this->m_bValid );
	return GetT();
}

template < class T >
T* const COptional< T >::operator -> ()
{ 
        SKY_ASSERT( this->m_bValid );
	return GetT();
}

template < class T >
void COptional< T >::Clear()
{
        if ( this->m_bValid )
	{
                this->m_bValid = skyFalse;
		Destroy();
	}
}

template < class T >
skyBool const COptional< T >::Valid() const
{ 
        return this->m_bValid;
}

template < class T >
skyBool const COptional< T >::Invalid() const
{ 
        return !this->m_bValid;
}

template < class T >
T const * const COptional< T >::GetT() const
{ 
        return reinterpret_cast< T const * const >( this->m_data );
}

template < class T >
T * const COptional< T >::GetT()
{ 
        return reinterpret_cast< T * const >( this->m_data );
}

template < class T >
void COptional< T >::Construct( T const & t )
{ 
	new ( GetT() ) T( t ); //領域を指定してのnewなのでここだけNEWではないです
}

template < class T >
void COptional< T >::Destroy()
{ 
	GetT()->~T();
}

} }
