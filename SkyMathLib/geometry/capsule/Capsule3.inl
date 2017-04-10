namespace sky { namespace lib { namespace math {

template< typename T >
Capsule3< T >::Capsule3() : CGeometryBase()
{
}

template< typename T >
Capsule3< T >::Capsule3( CSphere< T > sphere , CVector3< T > vec )
{
	m_Pos1   =  sphere.m_Pos;
	m_Pos2   += sphere.m_Pos + vec;
	m_Radius =  sphere.m_Radius;
}

template< typename T >
Capsule3< T >::Capsule3( T x1 , T y1 , T z1 , T x2 , T y2 , T z2 , T radius ) : m_Pos1( x1 , y1 , z1 ) , m_Pos2( x2 , y2 , z2 ) , m_Radius( radius ) , CGeometryBase()
{
}

} } }