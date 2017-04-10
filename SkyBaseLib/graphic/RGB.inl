namespace sky { namespace lib { namespace graphic {

template< typename T >
CRGB< T >::CRGB(T fR, T fG, T fB)
{
    r = fR;
    g = fG;
    b = fB;
}

template< typename T >
CRGB< T >& CRGB< T >::operator= (T value)
{
    r = value;
    g = value;
    b = value;
    return *this;
}

template< typename T >
skyBool CRGB< T >::operator== (const CRGB< T >& rgb) const
{
    return (r == rgb.r && g == rgb.g && b == rgb.b);
}

template< typename T >
skyBool CRGB< T >::operator!= (const CRGB< T >& rgb) const
{
    return !(*this == rgb);
}

template< typename T >
CRGB< T > CRGB< T >::operator+ (const CRGB< T >& rgb) const
{
    CRGB< T > result = *this;
    result.r += rgb.r;
    result.g += rgb.g;
    result.b += rgb.b;
    return result;
}

template< typename T >
CRGB< T > CRGB< T >::operator- (const CRGB< T >& rgb) const
{
    CRGB< T > result = *this;
    result.r -= rgb.r;
    result.g -= rgb.g;
    result.b -= rgb.b;
    return result;
}

template< typename T >
CRGB< T > CRGB< T >::operator* (T value) const
{
    CRGB< T > result = *this;
    result.r *= value;
    result.g *= value;
    result.b *= value;
    return result;
}

template< typename T >
CRGB< T > CRGB< T >::operator* (const CRGB< T >& rgb) const
{
    CRGB< T > result = *this;
    result.r *= rgb.r;
    result.g *= rgb.g;
    result.b *= rgb.b;
    return result;
}

template< typename T >
CRGB< T > CRGB< T >::operator/ (T value) const
{
    CRGB< T > result = *this;
    result.r /= value;
    result.g /= value;
    result.b /= value;
    return result;
}

template< typename T >
CRGB< T > CRGB< T >::operator/ (const CRGB< T > &rgb) const
{
    CRGB result = *this;
    result.r /= rgb.r;
    result.g /= rgb.g;
    result.b /= rgb.b;
    return result;
}

template< typename T >
CRGB< T > CRGB< T >::operator- () const
{
    return CRGB< T >(-r,-g,-b);
}

template< typename T >
CRGB< T >& CRGB< T >::operator+= (const CRGB< T >& rgb)
{
    r += rgb.r;
    g += rgb.g;
    b += rgb.b;
    return *this;
}

template< typename T >
CRGB< T >& CRGB< T >::operator-= (const CRGB< T >& rgb)
{
    r -= rgb.r;
    g -= rgb.g;
    b -= rgb.b;
    return *this;
}

template< typename T >
CRGB< T >& CRGB< T >::operator*= (T value)
{
    r *= value;
    g *= value;
    b *= value;
    return *this;
}

template< typename T >
CRGB< T >& CRGB< T >::operator*= (const CRGB< T > &rgb)
{
    r *= rgb.r;
    g *= rgb.g;
    b *= rgb.b;
    return *this;
}

template< typename T >
CRGB< T >& CRGB< T >::operator/= (T value)
{
    r /= value;
    g /= value;
    b /= value;
    return *this;
}

template< typename T >
CRGB< T >& CRGB< T >::operator/= (const CRGB< T > &rgb)
{
    r /= rgb.r;
    g /= rgb.g;
    b /= rgb.b;
    return *this;
}

template< typename T >
void CRGB< T >::Clamp()
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
}

template< typename T >
void CRGB< T >::Scale()
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
        T imax = 1.0f / maxValue;
        r *= imax;
        g *= imax;
        b *= imax;
    }
}

template< typename T >
void CRGB< T >::GetAs( CRGBA_4byte< T >& rgba ) const
{
    rgba.r() = ( u8 )( r * 255.0f );
    rgba.g() = ( u8 )( g * 255.0f );
    rgba.b() = ( u8 )( b * 255.0f );
    rgba.a() = 255;
}

} } }