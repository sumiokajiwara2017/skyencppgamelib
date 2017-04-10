#include "StdAfx.h"
#include "SkyGraphicLib/Screen.h"

namespace sky { namespace lib { namespace graphic {

math::CBasicRect CScreenUtil::CreateViewPort( const math::CBasicSize &screenSize , dectype screenAspectRatio )
{
	dectype scopeHeight = GetCinemaScopeHeight( screenSize , screenAspectRatio );
	return math::CBasicRect( 0.0f, 0.0f , 1.0f, 1.0f - scopeHeight );
}

math::CBasicVector2 CScreenUtil::PixelToPoint( const math::CBasicVector2& pixelPosition , const math::CBasicSize &screenSize , dectype width , dectype height )
{
	const dectype scopeHeight = GetCinemaScopeHeight( screenSize , width / height );
	const dectype rate        = GetValidScreenHeightRateInVirtualScreen( screenSize , width , height );
		
	return math::CBasicVector2( pixelPosition.X / screenSize.W , scopeHeight + pixelPosition.Y / screenSize.H * rate );
}

math::CBasicSize CScreenUtil::GetScreenSize( const math::CBasicSize &windowSize )
{
	return windowSize;
}

dectype CScreenUtil::GetCinemaScopeHeight( const math::CBasicSize &screenSize , dectype screenAspectRatio )
{
	return ( 1.0f - ( screenSize.W / screenSize.H ) / screenAspectRatio ) * 0.5f;
}

dectype CScreenUtil::GetValidScreenHeightRateInVirtualScreen( const math::CBasicSize &screenSize , dectype width , dectype height )
{
	return height / width / ( screenSize.H / screenSize.W );
}

math::CBasicSize CScreenUtil::GetViewPortSpacedPixelSize( const math::CBasicSize &screenSize , dectype width , dectype height )
{
	return math::CBasicSize( 1.0f / screenSize.W , 1.0f / screenSize.H * GetValidScreenHeightRateInVirtualScreen( screenSize , width , height ) );
}


} } }