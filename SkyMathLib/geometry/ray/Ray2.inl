namespace sky { namespace lib { namespace math {

template< typename T >
CRay2< T >::CRay2()
	: CGeometryBase()
{
}
template< typename T >
CRay2< T >::CRay2( const CBasicVector2 &pos , const CBasicVector2 &vec )
	: CGeometryBase()
	, m_Pos( pos )
	, m_Vec( vec )
{
	m_Vec.Normalize();
}

} } }