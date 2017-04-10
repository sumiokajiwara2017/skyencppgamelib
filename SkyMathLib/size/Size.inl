#pragma once

namespace sky { namespace lib { namespace math {

template< typename T >
CSize< T >::CSize()
{
	W = 0;
	H = 0;
}

template< typename T >
CSize< T >::CSize( T w, T h )
{
	W = w;
	H = h;
}

template< typename T >
CSize< T >::~CSize()
{
}

template< typename T >
skyBool CSize< T >::IsEmpty()
{
	return ( W == 0 && H == 0 );
}

template< typename T >
void CSize< T >::Set( T w, T h )
{
	W = w;
	H = h;
}

template< typename T >
CSize< T >& CSize< T >::operator =( const CSize< T >& size )
{
	W = size.W; H = size.H;
	return *this;
}

template< typename T >
CVector2< T > CSize< T >::operator +( const CVector2< T >& v1 ) const
{
	return CVector2< T >( W + v1.X , H + v1.Y );
}

} } }