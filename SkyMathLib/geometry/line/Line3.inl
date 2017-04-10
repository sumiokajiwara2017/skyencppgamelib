namespace sky { namespace lib { namespace math {

template< typename T >
CLine3< T >::CLine3() : CGeometryBase()
{
}

template< typename T >
CLine3< T >::CLine3( const CVector3< T > &pos1 , const CVector3< T > &pos2 )
	: CGeometryBase()
	, m_Pos1( pos1 )
	, m_Pos2( pos2 )
{
}

template< typename T >
CVector3< T > CLine3< T >::Interpolate( f32 t ) const
{
	CVector3< T > dest;
	return interp::Linear< CVector3< T > >( dest , m_Pos1 , m_Pos2 , t );
}

} } }