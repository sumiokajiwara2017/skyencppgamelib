namespace sky { namespace lib { namespace math {

template< typename T >
Capsule2< T >::Capsule2() : CGeometryBase()
{
}

template< typename T >
Capsule2< T >::Capsule2( T x1 , T y1 , T x2 , T y2 , T radius ) : m_Pos1( x1 , y1 ) , m_Pos2( x2 , y2 ) , m_Radius( radius ) , CGeometryBase()
{
}

} } }