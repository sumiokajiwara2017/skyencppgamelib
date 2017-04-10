namespace sky { namespace lib { namespace math {

template< typename T >
CTriangle2< T >::CTriangle2()
 : CGeometryBase()
{
}

template< typename T >
CTriangle2< T >::CTriangle2( const CVector2< T >& top, const CVector2< T >& left, const CVector2< T >& right )
 : CGeometryBase()
{
	Create( top , left , right );
}

template< typename T >
void CTriangle2< T >::Create( const CVector2< T >& top, const CVector2< T >& left, const CVector2< T >& right )
{
	//ç¿ïWÇäiî[
	m_Pos[ 0 ] = top;
	m_Pos[ 1 ] = left;
	m_Pos[ 2 ] = right;
}

} } }