#include "../../Util/MathUtil.h"

namespace sky { namespace lib { namespace math {

template< typename T >
inline CVector3< T >::CVector3() : CGeometryBase() , X( 0.0f ) , Y( 0.0f ) , Z( 0.0f )
{
}

template< typename T >
inline CVector3< T >::CVector3( const CVector3& v ) : CGeometryBase() ,
X( v.X ) , Y( v.Y ) , Z( v.Z )
{
}

template< typename T >
inline CVector3< T >::CVector3( T nX , T nY, T nZ ) : CGeometryBase() ,
X( nX ) , Y( nY ) , Z( nZ )
{
}

template< typename T >
inline CVector3< T >::CVector3( const dectype vec[ 3 ] )
{
	X = vec[ 0 ];
	Y = vec[ 1 ];
	Z = vec[ 2 ];
}

template< typename T >
inline T CVector3< T >::operator[]( s32 idx ) const
{
	switch ( idx )
	{
		case 0:	return X; break;
		case 1:	return Y; break;
		case 2:	return Z; break;
		default:
			SKY_PANIC_MSG( _T("CVector3の添え字には使えない値が来ました。値=[%d]" ) , idx );
			break;
	}
	return ( T )-FLT_MAX;
}

template< typename T >
inline void CVector3< T >::Zero()
{
	X = Y = Z = 0.0f;
}

template< typename T >
inline CVector3< T > &CVector3< T >::Set( T x , T y , T z )
{
	X = x;
	Y = y;
	Z = z;

	return *this;
}

template< typename T >
inline CVector3< T >& CVector3< T >::operator =( const CVector3< T >& v )
{
	X = v.X; Y = v.Y; Z = v.Z;
	return *this;
}

template< typename T >
inline CVector3< T >& CVector3< T >::operator =( const CVector2< T >& v )
{
	X = v.X; Y = v.Y; Z = 0;
	return *this;
}

template< typename T >
inline skyBool CVector3< T >::operator ==( const CVector3< T >& v ) const
{
	return X == v.X && Y == v.Y && Z == v.Z;
}

template< typename T >
inline skyBool CVector3< T >::operator !=( const CVector3< T >& v ) const
{
	return X != v.X || Y != v.Y || Z != v.Z;
}

template< typename T >
inline CVector3< T > CVector3< T >::operator +( const CVector3< T >& v ) const
{
	return CVector3( X + v.X , Y + v.Y, Z + v.Z );
}

template< typename T >
inline CVector3< T > CVector3< T >::operator -( const CVector3< T >& v ) const
{
	return CVector3( X - v.X , Y - v.Y, Z - v.Z );
}

template< typename T >
inline CVector3< T > CVector3< T >::operator -() const
{
	return CVector3( -X , -Y, -Z );
}

template< typename T >
inline CVector3< T > CVector3< T >::operator *( T val ) const
{
	return CVector3( X * val , Y * val , Z * val );
}

template< typename T >
inline CVector3< T > CVector3< T >::operator *( const CVector3< T >& v ) const
{
	return CVector3( X * v.X , Y * v.Y , Z * v.Z );
}

template< typename T >
inline CVector3< T > CVector3< T >::operator /( T val ) const
{

	SKY_ASSERT_MSG( val != 0.0f , _T( "0 division was generated." ) ); //0除算が発生しました。
	T	oneOverA = 1.0f / val;
	return CVector3(X*oneOverA, Y*oneOverA, Z*oneOverA);
}

template< typename T >
inline CVector3< T > CVector3< T >::operator /( const CVector3< T >& v ) const
{
	return CVector3( X / v.X , Y / v.Y , Z / v.Z );
}

template< typename T >
inline CVector3< T >& CVector3< T >::operator +=( const CVector3< T > &v )
{
	X += v.X; Y += v.Y; Z += v.Z;
	return *this;
}

template< typename T >
inline CVector3< T >& CVector3< T >::operator -=( const CVector3< T > &v )
{
	X -= v.X; Y -= v.Y; Z -= v.Z;
	return *this;
}

template< typename T >
inline CVector3< T >& CVector3< T >::operator *=( T val )
{
	X *= val; Y *= val; Z *= val;
	return *this;
}

template< typename T >
inline CVector3< T >& CVector3< T >::operator /=( T val )
{
	T	oneOverA = 1.0f / val;
	X *= oneOverA; Y *= oneOverA; Z *= oneOverA;
	return *this;
}

template< typename T >
inline CVector3< T >& CVector3< T >::Normalize()
{
	T magSq = X*X + Y*Y + Z*Z;
	if (magSq > 0.0f)
	{
		T oneOverMag = ( T )( 1.0f / CMathUtil::Sqrt( magSq ) );
		X *= oneOverMag;
		Y *= oneOverMag;
		Z *= oneOverMag;
	}
	return *this;
}

template< typename T >
inline T CVector3< T >::Dot( const CVector3< T >& v ) const
{
	return X*v.X + Y*v.Y + Z*v.Z;
}

template< typename T >
inline CVector3< T > CVector3< T >::Cross( const CVector3< T >& v ) const
{
	return math::Cross( *this , v );
}


template< typename T >
inline T CVector3< T >::IncludedAngle( const CVector3< T > &v ) const
{
    //インスタンスの実体コピー
    CVector3 my_N = *this;
    CVector3 v1_N = v;

    //正規化
    my_N.Normalize();
    v1_N.Normalize();

    //正規化された者同士の内積を求める
    T dot = my_N.Dot( v1_N );

    //なす角を返す
	return CMathUtil::Acos( dot );
}

template< typename T >
inline T CVector3< T >::SetLength( T length )
{
	Normalize();
	*this *= length;
	return length;
}

template< typename T >
inline T CVector3< T >::Truncate( T length )
{
	T nowLenght = Length();
	if ( nowLenght > length )
	{
		nowLenght = SetLength( length );
	}
	return nowLenght;
}

template< typename T >
inline T CVector3< T >::Length() const
{
	return ( T )CMathUtil::Sqrt( ( T )( X * X + Y * Y + Z * Z ) );
}

template< typename T >
inline T CVector3< T >::QuaredLength() const
{
	return ( T )( X * X + Y * Y + Z * Z );
}

template< typename T >
inline T CVector3< T >::LengthXZ() const
{
	CVector3< T > result = *this;
	result.Y = 0;
	return result.Length();
}

template< typename T >
inline CVector3< T > CVector3< T >::Vector3Length() const
{
	return CVector3< T >( Length() , Length() , Length() );
}

template< typename T >
inline skyBool CVector3< T >::IsAllZero() const
{
	return ( X == 0 && Y == 0 && Z == 0 )? skyTrue : skyFalse;
}

template< typename T >
inline skyBool CVector3< T >::IsAllOne() const
{
	return ( X == 1 && Y == 1 && Z == 1 )? skyTrue : skyFalse;
}

template< class T >
inline T VectorMag( const CVector3< T > &v )
{
	return ( T )CMathUtil::Sqrt( ( T )( v.X * v.X + v.Y * v.Y + v.Z * v.Z ) );
}

template< class T >
inline CVector3< T > Cross( const CVector3< T >& v1, const CVector3< T >& v2 )
{
	return CVector3< T >(
		v1.Y * v2.Z - v1.Z * v2.Y,
		v1.Z * v2.X - v1.X * v2.Z,
		v1.X * v2.Y - v1.Y * v2.X
	);
}

template< class T >
inline T Dot( const CVector3< T >& v1, const CVector3< T >& v2 )
{
	return v1.X*v2.X + v1.Y*v2.Y + v1.Z*v2.Z;
}

template< class T >
inline CVector3< T > Mult( T k, const CVector3< T > &v )
{
	return CVector3< T >(k*v.X , k*v.Y, k*v.Z);
}

template< class T >
inline T Distance( const CVector3< T >& v1, const CVector3< T >& v2 )
{
	T dx = v1.X - v2.X;
	T dy = v1.Y - v2.Y;
	T dz = v1.Z - v2.Z;
	return CMathUtil::Sqrt(dx*dx + dy*dy + dz*dz);
}

template< class T >
inline T DistanceSquared( const CVector3< T >& v1, const CVector3< T >& v2 )
{
	T dx = v1.X - v2.X;
	T dy = v1.Y - v2.Y;
	T dz = v1.Z - v2.Z;
	return dx*dx + dy*dy + dz*dz;
}

template< class T >
inline skyBool Vector3Greater( const CVector3< T >& v1 , const CVector3< T >& v2 )
{
	return ( v1.X > v2.X && v1.Y > v2.Y && v1.Z > v2.Z );
}

template< class T >
inline CVector3< T > VectorReciprocal( const CVector3< T > &v )
{
	return CVector3< T >( 1 / v.X , 1 / v.Y , 1 / v.Z );
}

template< class T >
inline CVector3< T > Transform( const CVector3< T > &vec , const CMatrix4x3 &matrix )
{
	return  MulMatrix4x4( vec , matrix );
}

template< typename T >
void CVector3< T >::Copyf32Array( f32 *pDestArray ) const
{
	pDestArray[ 0 ] = ( f32 )X;
	pDestArray[ 1 ] = ( f32 )Y;
	pDestArray[ 2 ] = ( f32 )Z;
}

} } }