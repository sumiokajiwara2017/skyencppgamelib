namespace sky { namespace lib { namespace math {

template< typename T >
CCircle< T >::CCircle() : CGeometryBase()
{
}

template< typename T >
CCircle< T >::CCircle( T x , T y, T radius ) : CGeometryBase()
{
	m_Pos.X = x;
	m_Pos.Y = y;
	m_Radius = radius;
}

template< typename T >
T& CCircle< T >::X()
{
	return m_Pos.X;
}

template< typename T >
const T& CCircle< T >::X() const
{
	return m_Pos.X;
}

template< typename T >
T& CCircle< T >::Y()
{
	return m_Pos.Y;
}

template< typename T >
const T& CCircle< T >::Y() const
{
	return m_Pos.Y;
}

template< typename T >
T& CCircle< T >::R()
{
	return m_Radius;
}

template< typename T >
const T& CCircle< T >::R() const
{
	return m_Radius;
}


template< typename T >
CCircle< T >::~CCircle()
{
}

template< typename T >
void CCircle< T >::SetPos( CVector2< T >& vec )
{ 
	m_Pos = vec;
}

template< typename T >
const CVector2< T >&	CCircle< T >::GetPos() const
{ 
	return m_Pos;	
}

template< typename T >
void CCircle< T >::SetRadius( dectype radius )
{ 
	m_Radius = radius;
}

template< typename T >
T CCircle< T >::GetRadius() const
{ 
	return m_Radius;
}

} } }