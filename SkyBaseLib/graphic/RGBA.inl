namespace sky { namespace lib { namespace graphic {

template< typename T >
CRGBA< T >::CRGBA( T lr , T lg , T lb , T la )
{
    r = lr;
    g = lg;
    b = lb;
    a = la;
}

template< typename T >
CRGBA< T >& CRGBA< T >::operator= ( T value )
{
    r = value;
    g = value;
    b = value;
    a = value;
    return *this;
}

template< typename T >
CRGBA< T >& CRGBA< T >::operator= ( const CRGBA< T >& rgba )
{
    r = rgba.r;
    g = rgba.g;
    b = rgba.b;
    a = rgba.a;
    return *this;
}

template< typename T >
skyBool CRGBA< T >::operator== ( const CRGBA< T >& rgba ) const
{
    return ( r == rgba.r && g == rgba.g && b == rgba.b && a == rgba.a );
}

template< typename T >
skyBool CRGBA< T >::operator!= ( const CRGBA< T >& rgba ) const
{
    return !( *this == rgba );
}

template< typename T >
CRGBA< T > CRGBA< T >::operator+ ( const CRGBA< T >& rgba ) const
{
    CRGBA< T > result = *this;
    result.r += rgba.r;
    result.g += rgba.g;
    result.b += rgba.b;
    result.a += rgba.a;
    return result;
}

template< typename T >
CRGBA< T > CRGBA< T >::operator- ( const CRGBA< T >& rgba ) const
{
    CRGBA< T > result = *this;
    result.r -= rgba.r;
    result.g -= rgba.g;
    result.b -= rgba.b;
    result.a -= rgba.a;
    return result;
}

template< typename T >
CRGBA< T > CRGBA< T >::operator* ( T value ) const
{
    CRGBA< T > result = *this;
    result.r *= value;
    result.g *= value;
    result.b *= value;
    result.a *= value;
    return result;
}

template< typename T >
CRGBA< T > CRGBA< T >::operator* (const CRGBA< T > &rgba) const
{
    CRGBA< T > result = *this;
    result.r *= rgba.r;
    result.g *= rgba.g;
    result.b *= rgba.b;
    result.a *= rgba.a;
    return result;
}

template< typename T >
CRGBA< T > CRGBA< T >::operator/ (T value) const
{
    CRGBA< T > result = *this;
    result.r /= value;
    result.g /= value;
    result.b /= value;
    result.a /= value;
    return result;
}

template< typename T >
CRGBA< T > CRGBA< T >::operator/ (const CRGBA< T > &rgba) const
{
    CRGBA< T > result = *this;
    result.r /= rgba.r;
    result.g /= rgba.g;
    result.b /= rgba.b;
    result.a /= rgba.a;
    return result;
}

template< typename T >
CRGBA< T > CRGBA< T >::operator- () const
{
    return CRGBA(-r,-g,-b,-a);
}

template< typename T >
CRGBA< T >& CRGBA< T >::operator+= ( const CRGBA< T >& rgba )
{
    r += rgba.r;
    g += rgba.g;
    b += rgba.b;
    a += rgba.a;
    return *this;
}

template< typename T >
CRGBA< T >& CRGBA< T >::operator-= (const CRGBA< T >& rgba)
{
    r -= rgba.r;
    g -= rgba.g;
    b -= rgba.b;
    a -= rgba.a;
    return *this;
}

template< typename T >
CRGBA< T >& CRGBA< T >::operator*= (T value)
{
    r *= value;
    g *= value;
    b *= value;
    a *= value;
    return *this;
}

template< typename T >
CRGBA< T >& CRGBA< T >::operator*= (const CRGBA< T > &rgba)
{
    r *= rgba.r;
    g *= rgba.g;
    b *= rgba.b;
    a *= rgba.a;
    return *this;
}

template< typename T >
CRGBA< T >& CRGBA< T >::operator/= (T value)
{
    r /= value;
    g /= value;
    b /= value;
    a /= value;
    return *this;
}

template< typename T >
CRGBA< T >& CRGBA< T >::operator/= (const CRGBA< T > &rgba)
{
    r /= rgba.r;
    g /= rgba.g;
    b /= rgba.b;
    a /= rgba.a;
    return *this;
}

template< typename T >
void CRGBA< T >::Clamp()
{
    if ( r > 1.0f )
	{
        r = 1.0f;
	}
    if ( g > 1.0f )
	{
        g = 1.0f;
	}
    if ( b > 1.0f )
	{
        b = 1.0f;
	}
    if ( a > 1.0f )
	{
        a = 1.0f;
	}
}

template< typename T >
void CRGBA< T >::Scale()
{
    T maxValue = r;
    if ( g > maxValue )
	{
        maxValue = g;
	}
    if ( b > maxValue )
	{
        maxValue = b;
	}

    if ( maxValue > 1.0f )
    {
        T imax = 1.0f/maxValue;
        r *= imax;
        g *= imax;
        b *= imax;
    }

    if ( a > 1.0f )
	{
        a = 1.0f;
	}
}

template< typename T >
void CRGBA< T >::GetAs( CRGBA_4byte< T >& rgba) const
{
    rgba.r() = ( u8 )( r * 255.0f );
    rgba.g() = ( u8 )( g * 255.0f );
    rgba.b() = ( u8 )( b * 255.0f );
    rgba.a() = ( u8 )( a * 255.0f );
}

template< typename T >
u32 CRGBA< T >::GetAs() const
{
	 CRGBA_4byte< T > rgba;
	 GetAs(rgba);
	 return rgba.GetAs();
}

template< typename T >
void CRGBA< T >::GetAsARGB( CRGBA_4byte< T >& argb) const
{
    argb.r() = ( u8 )( a * 255.0f );
    argb.g() = ( u8 )( r * 255.0f );
    argb.b() = ( u8 )( g * 255.0f );
    argb.a() = ( u8 )( b * 255.0f );
}

template< typename T >
u32 CRGBA< T >::GetAsARGB() const
{
	 CRGBA_4byte< T > argb;
	 GetAsARGB(argb);
	 return argb.GetAs();
}

template< typename T >
void CRGBA< T >::GetAsABGR( CRGBA_4byte< T >& argb) const
{
    argb.r() = ( u8 )( a * 255.0f );
    argb.g() = ( u8 )( b * 255.0f );
    argb.b() = ( u8 )( g * 255.0f );
    argb.a() = ( u8 )( r * 255.0f );
}

template< typename T >
u32 CRGBA< T >::GetAsABGR() const
{
	 CRGBA_4byte< T > argb;
	 GetAsARGB(argb);
	 return argb.GetAs();
}

template< typename T >
void CRGBA< T >::Copyf32Array3RGB( f32 *pDestArray ) const
{
	pDestArray[ 0 ] = r;
	pDestArray[ 1 ] = g;
	pDestArray[ 2 ] = b;
}

template< typename T >
void CRGBA< T >::Copyf32Array4RGBA( f32 *pDestArray ) const
{
	pDestArray[ 0 ] = r;
	pDestArray[ 1 ] = g;
	pDestArray[ 2 ] = b;
	pDestArray[ 3 ] = a;
}

} } }