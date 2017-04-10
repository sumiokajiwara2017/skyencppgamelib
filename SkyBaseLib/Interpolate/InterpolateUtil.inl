namespace sky { namespace lib { namespace interp {

template< typename T >
inline T& None( T& dest, const T& start, const T& end, f32 t )
{
	if ( t == 1.0f )
	{
		dest = end;
	}
	else
	{
		dest = start;
	}
	return dest;
}

template< typename T >
inline T& Linear( T& dest, const T& start, const T& end, f32 t )
{
	if ( start == end )
	{
		dest = end;
	}
	else
	{
		dest = ( T )( start + ( end - start ) * t );
	}
	return dest;
}

template< typename T >
inline T& Acceleration( T& dest, const T& start, const T& end, f32 t )
{//‰~‰^“®‚Ì270“x‚©‚ç360“x‚Ü‚Å‚Ì“®‚«‚ÌY’l‚ð—˜—p

	if ( start == end )
	{
		dest = end;
	}
	else
	{
		f32 y = sinf( ( f32 )( 4.71238898038469 + ( ( 6.28318530717959 - 4.71238898038469 ) * t ) ) );

		T distance = end - start;

		dest = ( T )( start + ( distance * ( y + 1.0f ) ) );
	}
	return dest;
}

template< typename T >
inline T& Deceleration( T& dest, const T& start, const T& end, f32 t )
{//‰~‰^“®‚Ì0“x‚©‚ç90“x‚Ü‚Å‚Ì“®‚«‚ÌY’l‚ð—˜—p
	
	if ( start == end )
	{
		dest = end;
	}
	else
	{
		f32 y = sinf( ( f32 )( 1.5707963267949 * t ) );

		T distance = end - start;

		dest = ( T )( start + ( distance * y ) );
	}
	return dest;
}

template< typename T >
inline T& Div( T& dest, const T& end, const T& now, float divNum )
{
	dest = now + ( end - now ) / divNum;

	return dest;
}

template< typename T >
inline T& InterpolateCommon( T& dest, const T& start, const T& end, f32 t , eInterpolateType eType )
{
	switch( eType )
	{
	case eInterpolateType_Linear:
		Linear< T >( dest , start , end , t );
		break;
	case eInterpolateType_None:
		None< T >( dest , start , end , t );
		break;
	case eInterpolateType_Acceleration:
		Acceleration< T >( dest , start , end , t );
		break;
	case eInterpolateType_Deceleration:
		Deceleration< T >( dest , start , end , t );
		break;
	}
	return dest;
}
		
inline f32& LinearDegree( f32& dest, f32 start, f32 end, f32 t )
{
	f32 fDist = fabsf( start - end );
			
	if ( fDist > 180.0f  ) {
		f32 fReverseDist = 360.0f - fDist;
				
		if ( start > end )
		{
			end = start + fReverseDist;
		}
		else
		{
			end = start - fReverseDist;
		}
	}
			
	Linear( dest, start, end, t );
			
	if ( dest > 360.0f )
	{
		dest -= 360.0f;
	}

	if ( dest < 0.0f )
	{
		dest += 360.0f;
	}
	return dest;
}
		
template< typename T >
inline T& Hermite( T& dest, const T& start, const T& end, const T& tan0, const T& tan1, f32 t )
{
	f32 t2 = t * t;
	f32 t3 = t2 * t;
	f32 h1 = (  2.0f * t3 ) - ( 3.0f * t2 ) + 1.0f;
	f32 h2 = ( -2.0f * t3 ) + ( 3.0f * t2 );
	f32 h3 = t3 - ( 2.0f * t2 ) + t;
	f32 h4 = t3 - t2;
			
	dest = ( T )( h1 * start + h2 * end + h3 * tan0 + h4 * tan1 );

	return dest;
}
		
inline f32& HermiteDegree( f32& dest, f32 start, f32 end, f32 tan0, f32 tan1, f32 t )
{
	f32 fDist = fabsf( start - end );
			
	if ( fDist > 180.0f  )
	{
		f32 fReverseDist = 360.0f - fDist;
				
		if ( start > end )
		{
			end = start + fReverseDist;
		}
		else
		{
			end = start - fReverseDist;
		}
	}
			
	Hermite( dest, start, end, tan0, tan1, t );
			
	if ( dest > 360.0f )
	{
		dest -= 360.0f;
	}
	if ( dest < 0.0f )
	{
		dest += 360.0f;
	}
	return dest;
}

} } }