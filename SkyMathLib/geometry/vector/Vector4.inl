#include "../../Util/MathUtil.h"

namespace sky { namespace lib { namespace math {

template< typename T >
inline CVector4< T >::CVector4() : CGeometryBase()
{
}

template< typename T >
inline CVector4< T >::CVector4( const CVector3< T >& v , T w ) :  CGeometryBase() ,
X( v.X ) , Y( v.Y ) , Z( v.Z ) , W( w )
{
}

template< typename T >
inline CVector4< T >::CVector4( const CVector4& v ) :  CGeometryBase() ,
X( v.X ) , Y( v.Y ) , Z( v.Z ) , W( v.W )
{
}

template< typename T >
inline CVector4< T >::CVector4( T nX , T nY, T nZ , T nW ) :  CGeometryBase() ,
X( nX ) , Y( nY ) , Z( nZ ) , W( nW )
{
}

template< typename T >
inline T CVector4< T >::operator[]( s32 idx )
{
	switch ( idx )
	{
		case 0:	return X; break;
		case 1:	return Y; break;
		case 2:	return Z; break;
		case 3:	return W; break;
		default:
			SKY_PANIC_MSG( T("CVector4の添え字には使えない値が来ました。値=[%d]" , idx ) );
			break;
	}
}

template< typename T >
inline void CVector4< T >::Zero()
{
	X = Y = Z = W = 0.0f;
}

template< typename T >
inline void CVector4< T >::Set( T x , T y , T z , T w )
{
	X = x;
	Y = y;
	Z = z;
	W = w;
}

template< typename T >
inline CVector3<T> CVector4< T >::GetVector3() const
{
	return CVector3<T>( X , Y , Z );
}

template< typename T >
inline CVector4< T >& CVector4< T >::operator =( const CVector4< T >& v )
{
	X = v.X; Y = v.Y; Z = v.Z; W = v.W; 
	return *this;
}

template< typename T >
inline CVector4< T >& CVector4< T >::operator *=( T val )
{
	X *= val; Y *= val; Z *= val; W *= val;
	return *this;
}

template< typename T >
inline T CVector4< T >::Length() const
{
	return ( T )CMathUtil::Sqrt( ( T )( X * X + Y * Y + Z * Z + W * W ) );
}

template< typename T >
inline CVector4< T >& CVector4< T >::Normalize()
{
	T magSq = X*X + Y*Y + Z*Z + W*W;
	if (magSq > 0.0f)
	{
		T oneOverMag = ( T )( 1.0f / CMathUtil::Sqrt( magSq ) );
		X *= oneOverMag;
		Y *= oneOverMag;
		Z *= oneOverMag;
		W *= oneOverMag;
	}
	return *this;
}

template< typename T >
inline T CVector4< T >::Dot( const CVector4< T >& v ) const
{
	return X * v.X + Y * v.Y + Z * v.Z + W * v.W;
}

} } }