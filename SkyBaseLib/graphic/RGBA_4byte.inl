namespace sky { namespace lib { namespace graphic {

template< typename T >
CRGBA_4byte< T >::CRGBA_4byte(u8 lr, u8 lg, u8 lb, u8 la)
{
    r() = lr;
    g() = lg;
    b() = lb;
    a() = la;
}

template< typename T >
u8& CRGBA_4byte< T >::r()
{
    return m_rgba.r;
}

template< typename T >
const u8& CRGBA_4byte< T >::r() const
{
    return m_rgba.r;
}

template< typename T >
u8& CRGBA_4byte< T >::g()
{
    return m_rgba.g;
}

template< typename T >
const u8& CRGBA_4byte< T >::g() const
{
    return m_rgba.g;
}

template< typename T >
u8& CRGBA_4byte< T >::b()
{
    return m_rgba.b;
}

template< typename T >
const u8& CRGBA_4byte< T >::b() const
{
    return m_rgba.b;
}

template< typename T >
u8& CRGBA_4byte< T >::a()
{
    return m_rgba.a;
}

template< typename T >
const u8& CRGBA_4byte< T >::a() const
{
    return m_rgba.a;
}

template< typename T >
void CRGBA_4byte< T >::GetAs(u32& value) const
{
    value = m_rgba_u32;
#ifndef SKY_BIG_ENDIAN
	endian::EndianUtil::Swap32( ( u8* )&value , 1 );
#endif
}

template< typename T >
u32 CRGBA_4byte< T >::GetAs() const
{
	u32 value;
	GetAs( value );
	return value;
}

template< typename T >
void CRGBA_4byte< T >::GetAsBGRA(u32& value) const
{
    CRGBA_4byte result(b(), g(), r(), a() );
    result.GetAs(value);
}

template< typename T >
u32 CRGBA_4byte< T >::GetAsBGRA() const
{
    u32 value;
    GetAsBGRA(value);
    return value;
}

template< typename T >
void CRGBA_4byte< T >::GetAsARGB(u32& value) const
{
    CRGBA_4byte result(a(), r(), g(), b() );
    result.GetAs(value);
}

template< typename T >
u32 CRGBA_4byte< T >::GetAsARGB() const
{
    u32 value;
    GetAsARGB(value);
    return value;
}

template< typename T >
void CRGBA_4byte< T >::GetAs(CRGB< T >& rgb) const
{
    rgb.r = ( ( T )r() / 255.0f);
    rgb.g = ( ( T )g() / 255.0f);
    rgb.b = ( ( T )b() / 255.0f);
}

template< typename T >
void CRGBA_4byte< T >::GetAs(CRGBA< T >& rgba) const
{
    rgba.r = ( ( T )r() / 255.0f);
    rgba.g = ( ( T )g() / 255.0f);
    rgba.b = ( ( T )b() / 255.0f);
    rgba.a = ( ( T )a() / 255.0f);
}

template< typename T >
CRGBA_4byte< T >& CRGBA_4byte< T >::operator= (u8 value)
{
    r() = value;
    g() = value;
    b() = value;
    a() = value;
    return *this;
}

template< typename T >
bool CRGBA_4byte< T >::operator== (const CRGBA_4byte< T >& rgba) const
{
    return (r() == rgba.r() && g() == rgba.g() && b() == rgba.b() && a() == rgba.a() );
}

template< typename T >
bool CRGBA_4byte< T >::operator!= (const CRGBA_4byte< T >& rgba) const
{
    return !(*this == rgba);
}

template< typename T >
CRGBA_4byte< T > CRGBA_4byte< T >::operator+ (const CRGBA_4byte< T >& rgba) const
{
    CRGBA_4byte< T > result;
    result.r() = ( u8 )TemplateUtil::Min< T >(255, r() + rgba.r() );
    result.g() = ( u8 )TemplateUtil::Min< T >(255, g() + rgba.g() );
    result.b() = ( u8 )TemplateUtil::Min< T >(255, b() + rgba.b() );
    result.a() = ( u8 )TemplateUtil::Min< T >(255, a() + rgba.a() );
    return result;
}

template< typename T >
CRGBA_4byte< T > CRGBA_4byte< T >::operator- (const CRGBA_4byte< T >& rgba) const
{
    CRGBA_4byte< T > result;
    result.r() = ( u8 )TemplateUtil::Max< T >(0, r() - rgba.r() );
    result.g() = ( u8 )TemplateUtil::Max< T >(0, g() - rgba.g() );
    result.b() = ( u8 )TemplateUtil::Max< T >(0, b() - rgba.b() );
    result.a() = ( u8 )TemplateUtil::Max< T >(0, a() - rgba.a() );
    return result;
}

template< typename T >
CRGBA_4byte< T > CRGBA_4byte< T >::operator* (T value) const
{
    CRGBA_4byte< T > result;
    result.r() = ( u8 )TemplateUtil::Min< T >(255.0f, ( T )r() * value);
    result.g() = ( u8 )TemplateUtil::Min< T >(255.0f, ( T )g() * value);
    result.b() = ( u8 )TemplateUtil::Min< T >(255.0f, ( T )b() * value);
    result.a() = ( u8 )TemplateUtil::Min< T >(255.0f, ( T )a() * value);
    return result;
}

template< typename T >
CRGBA_4byte< T > CRGBA_4byte< T >::operator* (const CRGBA_4byte< T > &rgba) const
{
    CRGBA_4byte< T > result;
    result.r() = ( u8 )TemplateUtil::Min< T >(255, r() * rgba.r() );
    result.g() = ( u8 )TemplateUtil::Min< T >(255, g() * rgba.g() );
    result.b() = ( u8 )TemplateUtil::Min< T >(255, b() * rgba.b() );
    result.a() = ( u8 )TemplateUtil::Min< T >(255, a() * rgba.a() );
    return result;
}

template< typename T >
CRGBA_4byte< T > CRGBA_4byte< T >::operator/ (T value) const
{
    CRGBA_4byte< T > result;
    result.r() = ( u8 )( ( T )r() / value);
    result.g() = ( u8 )( ( T )g() / value);
    result.b() = ( u8 )( ( T )b() / value);
    result.a() = ( u8 )( ( T )a() / value);
    return result;
}

template< typename T >
CRGBA_4byte< T > CRGBA_4byte< T >::operator/ (const CRGBA_4byte< T > &rgba) const
{
    CRGBA_4byte< T > result;
    result.r() = r() / rgba.r();
    result.g() = g() / rgba.g();
    result.b() = b() / rgba.b();
    result.a() = a() / rgba.a();
    return result;
}

template< typename T >
CRGBA_4byte< T > CRGBA_4byte< T >::operator- () const
{
    return CRGBA_4byte< T >(-r(),-g(),-b(),-a() );
}

template< typename T >
CRGBA_4byte< T >& CRGBA_4byte< T >::operator+= (const CRGBA_4byte< T >& rgba)
{
    r() = ( u8 )TemplateUtil::Min< T >(255, r() + rgba.r() );
    g() = ( u8 )TemplateUtil::Min< T >(255, g() + rgba.g() );
    b() = ( u8 )TemplateUtil::Min< T >(255, b() + rgba.b() );
    a() = ( u8 )TemplateUtil::Min< T >(255, a() + rgba.a() );
    return *this;
}

template< typename T >
CRGBA_4byte< T >& CRGBA_4byte< T >::operator-= (const CRGBA_4byte< T >& rgba)
{
    r() = ( u8 )TemplateUtil::Max< T >(0, r() - rgba.r() );
    g() = ( u8 )TemplateUtil::Max< T >(0, g() - rgba.g() );
    b() = ( u8 )TemplateUtil::Max< T >(0, b() - rgba.b() );
    a() = ( u8 )TemplateUtil::Max< T >(0, a() - rgba.a() );
    return *this;
}

template< typename T >
CRGBA_4byte< T >& CRGBA_4byte< T >::operator*= (T value)
{
    r() = ( u8 )TemplateUtil::Min< T >(255.0f, ( T )r() * value);
    g() = ( u8 )TemplateUtil::Min< T >(255.0f, ( T )g() * value);
    b() = ( u8 )TemplateUtil::Min< T >(255.0f, ( T )b() * value);
    a() = ( u8 )TemplateUtil::Min< T >(255.0f, ( T )a() * value);
    return *this;
}

template< typename T >
CRGBA_4byte< T >& CRGBA_4byte< T >::operator*= (const CRGBA_4byte< T > &rgba)
{
    r() = ( u8 )TemplateUtil::Min< T >(255, r() * rgba.r() );
    g() = ( u8 )TemplateUtil::Min< T >(255, g() * rgba.g() );
    b() = ( u8 )TemplateUtil::Min< T >(255, b() * rgba.b() );
    a() = ( u8 )TemplateUtil::Min< T >(255, a() * rgba.a() );
    return *this;
}

template< typename T >
CRGBA_4byte< T >& CRGBA_4byte< T >::operator/= (T value)
{
    r() = ( u8 )( ( T )r() / value);
    g() = ( u8 )( ( T )g() / value);
    b() = ( u8 )( ( T )b() / value);
    a() = ( u8 )( ( T )a() / value);
    return *this;
}

template< typename T >
CRGBA_4byte< T >& CRGBA_4byte< T >::operator/= (const CRGBA_4byte< T > &rgba)
{
    r() = ( u8 )(r() / rgba.r() );
    g() = ( u8 )(g() / rgba.g() );
    b() = ( u8 )(b() / rgba.b() );
    a() = ( u8 )(a() / rgba.a() );
    return *this;
}

} } }
