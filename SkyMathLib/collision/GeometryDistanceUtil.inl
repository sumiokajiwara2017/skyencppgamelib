namespace sky { namespace lib { namespace math {

namespace GeometryDistanceUtil
{

/*
ãÛä‘è„ÇÃì_(x0, y0, z0) Ç©ÇÁÅAïΩñ  ax+by+cz+d=0 Ç‹Ç≈ÇÃãóó£ÇÕ
Å@|ax0+by0+cz0+d|/Å„(a2+b2+c2)
 */
template< typename T >
inline T Pos3ToPlane( const CVector3< T > &pos , const CPlane< T > &plane )
{
	return plane.Dot( pos );
}

template< typename T >
inline T SqDistPointSegment( const CVector3< T > &a , const CVector3< T > &b , const CVector3< T > &c )
{

	CVector3< T > ab = b - a , ac = c - a;

	T d = ac.Dot( ac ) , e = ac.Dot( ab );

	if ( e < 0.0f )
	{
		return d;
	}

	T f = ab.Dot( ab );

	if ( e > f )
	{
		return d - e;
	}
	else
	{
		return d - e * e / f;
	}
}


template< typename T >
inline T Clamp( T n , T min , T max )
{
    if ( n < min ) return min;
    if ( n > max ) return max;
    return n;
}

template< typename T >
inline T ClosestPtSegmentSegment( const CVector3< T > &p1 , const CVector3< T > &q1 , const CVector3< T > &p2 , const CVector3< T > &q2 , T &s , T &t , CVector3< T > &c1 , CVector3< T > &c2 )
{
    CVector3< T > d1 = q1 - p1; // Direction vector of segment S1
    CVector3< T > d2 = q2 - p2; // Direction vector of segment S2
    CVector3< T > r = p1 - p2;
    T a = d1.QuaredLength(); // Squared length of segment S1, always nonnegative
    T e = d2.QuaredLength(); // Squared length of segment S2, always nonnegative
	T f = d2.Dot( r );

    // Check if either or both segments degenerate into points
    if ( a <= util::EPSILON && e <= util::EPSILON )
	{
        // Both segments degenerate into points
        s = t = 0.0f;
        c1 = p1;
        c2 = p2;
        return ( c1 - c2 ).Dot( c1 - c2 );
    }
    if ( a <= util::EPSILON )
	{
        // First segment degenerates into a point
        s = 0.0f;
        t = f / e; // s = 0 => t = (b*s + f) / e = f / e
        t = Clamp< T >( t , 0.0f , 1.0f );
    }
	else
	{
        T c = d1.Dot( r );
        if ( e <= util::EPSILON )
		{
            // Second segment degenerates into a point
            t = 0.0f;
            s = Clamp< T >( -c / a , 0.0f , 1.0f ); // t = 0 => s = (b*t - c) / a = -c / a
        }
		else
		{
            // The general nondegenerate case starts here
            T b = d1.Dot( d2 );
            T denom = a * e - b * b; // Always nonnegative

            // If segments not parallel, compute closest point on L1 to L2, and
            // clamp to segment S1. Else pick arbitrary s (here 0)
            if ( denom != 0.0f )
			{
                s = Clamp< T >( ( b * f - c * e ) / denom , 0.0f , 1.0f );
            } 
			else
			{
				s = 0.0f;
			}

            // Compute point on L2 closest to S1(s) using
            // t = Dot((P1+D1*s)-P2,D2) / Dot(D2,D2) = (b*s + f) / e
            t = ( b * s + f ) / e;

            // If t in [0,1] done. Else clamp t, recompute s for the new value
            // of t using s = Dot((P2+D2*t)-P1,D1) / Dot(D1,D1)= (t*b - c) / a
            // and clamp s to [0, 1]
            if (t < 0.0f)
			{
                t = 0.0f;
                s = Clamp< T >( -c / a , 0.0f , 1.0f );
            }
			else if ( t > 1.0f )
			{
                t = 1.0f;
                s = Clamp< T >( ( b - c ) / a , 0.0f , 1.0f );
            }
        }
    }

    c1 = p1 + d1 * s;
    c2 = p2 + d2 * t;
    return ( c1 - c2 ).Dot( c1 - c2 );
}

}

} } }
