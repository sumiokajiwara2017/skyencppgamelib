namespace sky { namespace lib { namespace math {

template< typename T >
CRay3< T >::CRay3( const CVector3< T > &pos , const CVector3< T > &vec )
	: CGeometryBase()
	, m_Pos( pos )
	, m_Vec( vec )
{
	m_Vec.Normalize();
}

} } }