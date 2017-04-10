#include "StdAfx.h"
#include "SkyGraphicLib/SkyGraphicLibTypes.h"
#include "SkyGraphicLib/OpenGL/SkyGraphicLibGL.h"

#ifdef SKYLIB_RENDER_OPENGL4

static GLEWContext *s_Current_GlewContext = skyNull;

namespace sky { namespace lib { namespace graphic {

void CGlew::Initialize	()
{
    //　GLEWの初期化
    if ( glewInit() != GLEW_OK )
    {
        SKY_PANIC_MSG( _T("It failed in the initialization of GLEW.") );
    }

    //　サポートチェック
    if (! GLEW_EXT_framebuffer_object ) 
    {
        SKY_PANIC_MSG( _T("FOB is not supported.") );
    }
}

void CGlew::Initialize( GLEWContext *pContext )
{
	SetCurrentContext( pContext );
	Initialize();
}

GLEWContext *CGlew::CreateContext()
{
	return NEW GLEWContext;
}

void CGlew::DeleteContext( GLEWContext *pContext )
{
	DEL pContext;
}

void CGlew::SetCurrentContext( GLEWContext *pContext )
{
	s_Current_GlewContext = pContext;
}

} } }

GLEWContext *glewGetContext()
{
	return s_Current_GlewContext;
}

#endif
