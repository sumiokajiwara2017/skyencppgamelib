namespace sky { namespace lib { namespace math {

template< typename T >
CTriangle3< T >::CTriangle3()
 : CGeometryBase()
{
}

template< typename T >
CTriangle3< T >::CTriangle3( const CVector3< T >& top, const CVector3< T >& left, const CVector3< T >& right )
 : CGeometryBase()
{
	Create( top , left , right );
}

template< typename T >
void CTriangle3< T >::Create( const CVector3< T >& top, const CVector3< T >& left, const CVector3< T >& right )
{
	//���W���i�[
	m_Pos[ 0 ] = top;
	m_Pos[ 1 ] = left;
	m_Pos[ 2 ] = right;

	//�@���̌v�Z
    CVector3< T > vec1, vec2;

    // �@���x�N�g�������߂�
	vec1 = m_Pos[ 1 ] - m_Pos[ 0 ];
	vec2 = m_Pos[ 2 ] - m_Pos[ 0 ];
	m_Normal = Cross( vec1 , vec2 );
	m_Normal.Normalize();
}

} } }