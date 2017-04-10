namespace sky { namespace lib { namespace math {

template< typename T >
CLine2< T >::CLine2()
	: CGeometryBase()
{
}

template< typename T >
CLine2< T >::CLine2( const CVector2< T > &pos1 , const CVector2< T > &pos2 )
	: CGeometryBase()
	, m_Pos1( pos1 )
	, m_Pos2( pos2 )
{
}

template< typename T >
CVector2< T > CLine2< T >::Interpolate( f32 t ) const
{
	CVector2< T > dest;
	return interp::Linear< CVector2< T > >( dest , m_Pos1 , m_Pos2 , t );
}

} } }