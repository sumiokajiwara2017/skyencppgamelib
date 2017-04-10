namespace sky { namespace lib { namespace math {

template< typename T >
inline CMatrix3x2 &CAngleVector2< T >::ToMatrix( CMatrix3x2 &dest ) const
{
	dest.SetupRotate( m_Rot.ToRad() );
	return dest;
}

template< typename T >
inline CMatrix4x3 &CAngleVector2< T >::ToMatrix( CMatrix4x3 &dest ) const
{
	dest.SetupRotateYXZ( CBasicVector3( 0 , 0 , m_Rot.ToRad() ) );
	return dest;
}

template< typename T >
inline void CAngleVector2< T >::ToVector( CVector2< T > &dest , T len ) const
{
	dest = CVector2< T >( m.M11 + m.M21 , m.M12 + m.M22 ) * len;
}

template< typename T >
inline typename CVector2< T > CAngleVector2< T >::ToVector( T len ) const
{
	return CVector2< T >( m.M11 + m.M21 , m.M12 + m.M22 ) * len;
}

template< typename T >
inline void CAngleVector2< T >::RotVector( CVector2< T > &dest ) const
{
	CMatrix3x2 m;
	ToMatrix( m );
	dest = m * dest;
}

template< typename T >
inline typename CVector2< T > CAngleVector2< T >::RotVector( const CVector2< T > &src ) const
{
	CMatrix3x2 m;
	ToMatrix( m );
	return m * dest;
}

} } }