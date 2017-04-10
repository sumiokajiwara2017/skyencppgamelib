#pragma once

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

namespace GLUtil
{

	enum eBmpFormat
	{
		eBmpFormat_24bit = 24,
		eBmpFormat_32bit = 32,
	};

	/// <summary>
	/// スクリーンショット
	/// </summary>
	inline void ScreenShot( const skyMString* filename , u16 width , u16 height , u16 bpp = ( u16 )eBmpFormat_32bit );
}

} } }


#endif

#include "GLUtil.inl"