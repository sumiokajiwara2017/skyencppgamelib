#pragma once

namespace sky { namespace lib { namespace graphic {

namespace DisplayUtil
{

inline math::CBasicSize	DisplayPixel()
{

#ifdef SKYLIB_PLATFORM_WIN32
	math::CBasicSize result( ( dectype )::GetSystemMetrics( SM_CXSCREEN ) , ( dectype )::GetSystemMetrics( SM_CYSCREEN ) );
#endif
	return result;
}

}

} } }
