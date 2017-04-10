#include "../../Util/MathUtil.h"

namespace sky { namespace lib { namespace math {

template< typename T >
inline CVector2< T >::CVector2() : CGeometryBase() , X( 0.0f ) , Y( 0.0f )							//�f�t�H���g�̃R���X�g���N�^
{
}

template< typename T >
inline CVector2< T >::CVector2( const CVector2& v1 ) :	CGeometryBase() ,		//���̃C���X�^���X����̗v�f�̒l�R�s�[�C���X�^���X����
X( v1.X ) , Y( v1.Y )
{
}

template< typename T >
inline CVector2< T >::CVector2(T nX , T nY ) : CGeometryBase() , X( nX ) , Y( nY )					//�v�f�ԍ����󂯎��C���X�^���X����
{
}

template< typename T >
inline CVector2< T > &CVector2< T >::Set( T x , T y )
{
	X = x; Y = y;

	return *this;
}

template< typename T >
inline void CVector2< T >::Zero()
{
	X = Y = 0.0f;
}

template< typename T >
inline T CVector2< T >::operator[]( s32 idx )
{
	T data = 0;

	switch ( idx )
	{
		case 0:	data = X; break;
		case 1:	data = Y; break;
		default:
			SKY_PANIC_MSG( _T("CVector2�̓Y�����ɂ͎g���Ȃ��l�����܂����B�l=[%d]" ) , idx );
			break;
	}

	return data;
}

template< typename T >
inline skyBool CVector2< T >::IsEmpty()
{ 
	return ( X == 0.0f && Y == 0.0f );
}

template< typename T >
inline CVector2< T >& CVector2< T >::operator =( const CVector2< T >& v1 )
{
	X = v1.X; Y = v1.Y;
	return *this;
}

template< typename T >
inline skyBool CVector2< T >::operator ==( const CVector2& v1 ) const
{
	return X==v1.X && Y==v1.Y;
}

template< typename T >
inline skyBool CVector2< T >::operator !=( const CVector2& v1 ) const
{
	return X!=v1.X || Y!=v1.Y;
}

template< typename T >
inline CVector2< T > CVector2< T >::operator +( const CVector2& v1 ) const
{
	return CVector2(X + v1.X , Y + v1.Y);
}

template< typename T >
inline CVector2< T > CVector2< T >::operator -( const CVector2& v1 ) const
{
	return CVector2(X - v1.X , Y - v1.Y);
}

template< typename T >
inline CVector2< T > CVector2< T >::operator -() const
{
	return CVector2( -X , -Y );
}

template< typename T >
inline CVector2< T > CVector2< T >::operator *( T val ) const
{
	return CVector2( X * val , Y * val );
}

template< typename T >
inline CVector2< T > CVector2< T >::operator *( CVector2< T > vec ) const
{
	return CVector2( X * vec.X , Y * vec.Y );
}

template< typename T >
inline CVector2< T > CVector2< T >::operator /( T val ) const
{

//�f�o�b�O���[�h�ł̂�0���Z���`�F�b�N����
	SKY_ASSERT_MSG( val != 0.0f , _T( "0 division was generated." ) ); //0���Z���������܂����B

	T	oneOverA = 1.0f / val;
	return CVector2(X*oneOverA, Y*oneOverA);
}

template< typename T >
inline CVector2< T > CVector2< T >::operator /( CVector2< T > vec ) const
{
	return CVector2( X / vec.X , Y / vec.Y );
}

template< typename T >
inline CVector2< T >& CVector2< T >::operator +=( const CVector2 &v1 )
{
	X += v1.X; Y += v1.Y;
	return *this;
}

template< typename T >
inline CVector2< T >& CVector2< T >::operator -=( const CVector2 &v1 )
{
	X -= v1.X; Y -= v1.Y;
	return *this;
}

template< typename T >
inline CVector2< T >& CVector2< T >::operator *=( T val )
{
	X *= val; Y *= val;
	return *this;
}

template< typename T >
inline CVector2< T >& CVector2< T >::operator /=( T val )
{
	T	oneOverA = 1.0f / val;
	X *= oneOverA; Y *= oneOverA;
	return *this;
}

template< typename T >
inline void CVector2< T >::Normalize()
{
	T magSq = X*X + Y*Y;
	if (magSq > 0.0f)
	{
		T oneOverMag = 1.0f / CMathUtil::Sqrt(magSq);
		X *= oneOverMag;
		Y *= oneOverMag;
	}
}

template< typename T >
inline T CVector2< T >::Dot( const CVector2& v1 ) const
{
	return X*v1.X + Y*v1.Y;
}

//x' = x * cos�� - y * sin��
//y' = x * sin�� + y * cos��
template< typename T >
inline CVector2< T > CVector2< T >::Rot( T rad )
{
	T lx = X * CMathUtil::Cos( rad ) - Y * CMathUtil::Sin( rad );
	T ly = X * CMathUtil::Sin( rad ) + Y * CMathUtil::Cos( rad );
	return  CVector2< T >( lx , ly );
}

template< typename T >
inline CVector2< T > CVector2< T >::Perp()
{
	return  Rot( util::KPIOVER2 );
}

template< typename T >
inline T CVector2< T >::IncludedAngle( const CVector2 &v1 )
{
    //�C���X�^���X�̎��̃R�s�[
    CVector2 my_N = *this;
    CVector2 v1_N = v1;

    //���K��
    my_N.Normalize();
    v1_N.Normalize();

    //���K�����ꂽ�ғ��m�̓��ς����߂�
    T dot = my_N.Dot(v1_N);

    //�Ȃ��p��Ԃ�
	return CMathUtil::Acos(dot);
}

template< typename T >
inline T CVector2< T >::SetLength( T length )
{
	Normalize();
	*this *= length;
	return length;
}

template< typename T >
inline T CVector2< T >::Truncate( T length )
{
	T nowLenght = Length();
	if ( nowLenght > length )
	{
		nowLenght = SetLength( length );
	}
	return nowLenght;
}

template< typename T >
inline T CVector2< T >::Length() const
{
	return CMathUtil::Sqrt(X * X + Y * Y);
}

template< typename T >
inline void CVector2< T >::Floor()
{
	X = CMathUtil::Floor( X );
	Y = CMathUtil::Floor( Y );
}

template< typename T >
inline void CVector2< T >::Round()
{
	X = CMathUtil::Round( X );
	Y = CMathUtil::Round( Y );
}

template< class T >
inline T VectorMag( const CVector2< T > &v1 )
{
	return CMathUtil::Sqrt( v1.X*v1.X + v1.Y*v1.Y );
}

template< class T >
inline T Cross( const CVector2< T >& v1, const CVector2< T >& v2 )
{
	return v1.X * v2.Y - v1.Y * v2.X;
}

template< class T >
inline CVector2< T > Mult(f32 k, const CVector2< T > &v1)
{
    return CVector2< T >(k*v1.X , k*v1.Y);
}

template< class T >
inline T Distance(const CVector2< T >& v1, const CVector2< T >& v2)
{
	T dx = v1.X - v2.X;
	T dy = v1.Y - v2.Y;
	return CMathUtil::Sqrt(dx*dx + dy*dy);
}

template< class T >
inline T DistanceSquared(const CVector2< T >& v1, const CVector2< T >& v2)
{
	T dx = v1.X - v2.X;
	T dy = v1.Y - v2.Y;
	return dx*dx + dy*dy;
}

} } }
