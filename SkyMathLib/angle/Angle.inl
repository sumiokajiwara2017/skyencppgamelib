namespace sky { namespace lib { namespace math {

template< typename T >
inline typename CAngle< T > CAngle< T >::TurnN( const CAngle from , const CAngle to , T n )
{
	s32 diff;
	diff = 65535 & ( to.m_Angle - from.m_Angle );
	diff = diff <= 32768 ? diff : diff - 65536;
	return ( from + ( diff / n ) );
}

template< typename T >
inline typename CAngle< T > CAngle< T >::FaceN( const CAngle from , const CAngle to , T n )
{
	s32 diff = ( s32 )( ( to.m_Angle - from.m_Angle ) / n );
	return diff ? from.m_Angle + i_diff : to.m_Angle;
}

} } }