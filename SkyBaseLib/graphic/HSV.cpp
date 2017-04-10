#include "StdAfx.h"
#include "SkyBaseLib/Graphic.h"

namespace sky { namespace lib { namespace graphic {

namespace { //ñ≥ñºÇÃnamespaceÇÕÇ±ÇÃÉtÉ@ÉCÉãì‡Ç≈ÇÃÇ›égópÇ∑ÇÈÇ±Ç∆Çà”ñ°Ç∑ÇÈ

	struct QuadColors
	{
		CBasicRGBA_4byte upperLeft;
		CBasicRGBA_4byte lowerLeft;
		CBasicRGBA_4byte upperRight;
		CBasicRGBA_4byte lowerRight;
	};

	const QuadColors	g_akHueColors[ 6 ] =
	{
		{ CBasicRGBA_4byte( 255, 0,   0,   255 ), CBasicRGBA_4byte( 255, 0,   0,   255 ), CBasicRGBA_4byte( 255, 255, 0,   255 ), CBasicRGBA_4byte( 255, 255, 0,   255 ) }, // RÅ`Y
		{ CBasicRGBA_4byte( 255, 255, 0,   255 ), CBasicRGBA_4byte( 255, 255, 0,   255 ), CBasicRGBA_4byte( 0,   255, 0,   255 ), CBasicRGBA_4byte( 0,   255, 0,   255 ) }, // YÅ`G
		{ CBasicRGBA_4byte( 0,   255, 0,   255 ), CBasicRGBA_4byte( 0,   255, 0,   255 ), CBasicRGBA_4byte( 0,   255, 255, 255 ), CBasicRGBA_4byte( 0,   255, 255, 255 ) }, // GÅ`C
		{ CBasicRGBA_4byte( 0,   255, 255, 255 ), CBasicRGBA_4byte( 0,   255, 255, 255 ), CBasicRGBA_4byte( 0,   0,   255, 255 ), CBasicRGBA_4byte( 0,   0,   255, 255 ) }, // CÅ`B
		{ CBasicRGBA_4byte( 0,   0,   255, 255 ), CBasicRGBA_4byte( 0,   0,   255, 255 ), CBasicRGBA_4byte( 255, 0,   255, 255 ), CBasicRGBA_4byte( 255, 0,   255, 255 ) }, // BÅ`M
		{ CBasicRGBA_4byte( 255, 0,   255, 255 ), CBasicRGBA_4byte( 255, 0,   255, 255 ), CBasicRGBA_4byte( 255, 0,   0,   255 ), CBasicRGBA_4byte( 255, 0,   0,   255 ) }, // MÅ`R
	};
	
	CBasicRGBA_4byte LerpRGBA( CBasicRGBA_4byte kPrev, CBasicRGBA_4byte kNext, f32 fRate )
	{
		CBasicRGBA	kfPrev;
		CBasicRGBA	kfNext;
		
		kPrev.GetAs( kfPrev );
		kNext.GetAs( kfNext );
		
		CBasicRGBA	kfResult = kfPrev + ( kfNext - kfPrev ) * fRate;
		kfResult.r = TemplateUtil::Min< f32 >( kfResult.r, 1.0f );
		kfResult.g = TemplateUtil::Min< f32 >( kfResult.g, 1.0f );
		kfResult.b = TemplateUtil::Min< f32 >( kfResult.b, 1.0f );
		kfResult.a = TemplateUtil::Min< f32 >( kfResult.a, 1.0f );
		
		CBasicRGBA_4byte		kResult;
		
		kfResult.GetAs( kResult );
		
		return kResult;
	}
	
	CBasicRGB LerpColor( const CBasicRGB& kPrev, const CBasicRGB& kNext, f32 fRate ) {
		return kPrev + ( kNext - kPrev ) * fRate;
	}
}

template< typename T >
CRGBA_4byte< T > CHSV< T >::GetHueColor( T hue )
{
	s32	iValue = std::min< s32 >( 5, static_cast< s32 >( floor( hue / 60.0f ) ) );
		
	CRGBA_4byte< T >	kPrev = g_akHueColors[ iValue ].upperLeft;
	CRGBA_4byte< T >	kNext = g_akHueColors[ iValue ].upperRight;
	T	fRate = ( hue - iValue * 60.0f ) / 60.0f;
		
	return LerpRGBA( kPrev, kNext, fRate );
}
	
template< typename T >
void CHSV< T >::FromCRGBA_4byte( const CRGBA_4byte< T >& rgba )
{
	s32 iMax = rgba.r();
	if ( iMax < rgba.g() )
	{
		iMax = rgba.g();
	}
	if ( iMax < rgba.b() )
	{
		iMax = rgba.b();
	}
		
	s32 iMin = rgba.r();
	if ( iMin > rgba.g() )
	{
		iMin = rgba.g();
	}
	if ( iMin > rgba.b() )
	{
		iMin = rgba.b();
	}
		
	v = static_cast< T >( iMax / 255.0f );
		
	if ( iMax == 0 )
	{
		s = 0.0f;
		h = 0.0f;
	}
	else
	{
		s = ( iMax - iMin) / static_cast< T >( iMax );
		T	fDiff = static_cast< T >( iMax - iMin );
			
		T fR = fDiff <= FLT_EPSILON ? 1.0f : ( iMax - rgba.r() ) / fDiff;
		T fG = fDiff <= FLT_EPSILON ? 1.0f : ( iMax - rgba.g() ) / fDiff;
		T fB = fDiff <= FLT_EPSILON ? 1.0f : ( iMax - rgba.b() ) / fDiff;
		T fH = 0.0f;
			
		if ( iMax == rgba.r() )
		{
			fH = fB - fG;
		}
		else if ( iMax == rgba.g() )
		{
			fH = 2.0f + fR - fB;
		}
		else
		{
			fH = 4.0f + fG - fR;
		}
			
		fH *= 60.0f;
		if ( fH < 0.0f )
		{
			fH += 360.0f;
		}
		h = fH;
	}
}

template< typename T >
CRGBA_4byte< T > CHSV< T >::ToCRGBA_4byte() const
{
	const CRGBA_4byte< T >	kHueColor( GetHueColor( h ) );
	return LerpRGBA( CRGBA_4byte< T >::BLACK, LerpRGBA( CRGBA_4byte< T >::WHITE, kHueColor, s ), v );
}
	
template< typename T >
void CHSV< T >::FromCRGB( const CRGB< T >& rgb )
{
	CRGBA_4byte< T >	kRGBA;
	rgb.GetAs( kRGBA );
	FromRGBA( kRGBA );
}
	
template< typename T >
CRGB< T > CHSV< T >::ToCRGB() const
{
	const CRGBA_4byte< T >	kHueRGBA( GetHueColor( h ) );
	CRGB< T >			kHueColor;
	kHueRGBA.GetAs( kHueColor );
	return LerpColor( CRGB< T >::BLACK, LerpColor( CRGB< T >::WHITE, kHueColor, s ), v );
}

template< typename T >
void CHSV< T >::FromCRGBA( const CRGBA< T >& rgba )
{
	CRGBA_4byte< T > kRGBA;
	rgba.GetAs( kRGBA );
	FromRGBA( kRGBA );
}

template< typename T >
CRGBA< T > CHSV< T >::ToCRGBA() const
{
	const CRGBA_4byte< T >	kHueRGBA( GetHueColor( h ) );
	CRGB< T >			kHueColor;
	kHueRGBA.GetAs( kHueColor );
	CRGB< T >			kResult = LerpColor( CRGB< T >::BLACK, LerpColor( CRGB< T >::WHITE, kHueColor, s ), v );
	return CRGBA< T >( kResult.r, kResult.g, kResult.b, 1.0f );
}

} } }