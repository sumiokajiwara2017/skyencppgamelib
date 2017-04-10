#include "StdAfx.h"
#include "SkyGraphicLib/SkyGraphicLibTypes.h"
#include "SkyGraphicLib/OpenGL/SkyGraphicLibGL.h"

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

CFramebufferObject::CFramebufferObject()
{
	m_Size.W = 0;
	m_Size.H = 0;
    m_FBOHandl = 0;
    m_DepthStencilBufferHandle = 0;
    m_ColorBufferTextureHandl = 0;
}

CFramebufferObject::~CFramebufferObject()
{
}

void CFramebufferObject::InitializeWithGlew( const math::CBasicSize &size )
{
    //　GLEWの初期化
    if ( glewInit() != GLEW_OK )
    {
        SKY_PANIC_MSG( _T("Glew initialize invalid.") );
    }

    //　サポートチェック
    if (! GLEW_EXT_framebuffer_object ) 
    {
        SKY_PANIC_MSG( _T("FOB is not supported. ") );
    }

	Initialize( size );
}

CFramebufferObjectPtr CFramebufferObject::Create()
{
	return CFramebufferObjectPtr( NEW__( CFramebufferObject , CFramebufferObject() ) );
}

void CFramebufferObject::Initialize( const math::CBasicSize &size )
{
	m_Size = size;

    //　テクスチャの設定
    glGenTextures( 1 , &m_ColorBufferTextureHandl );
    glBindTexture( GL_TEXTURE_2D , m_ColorBufferTextureHandl );
    glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGBA , ( GLsizei )m_Size.W , ( GLsizei )m_Size.H , 0 , GL_RGBA , GL_UNSIGNED_BYTE , 0 );

    //　レンダーバッファの設定
    glGenRenderbuffersEXT( 1 , &m_DepthStencilBufferHandle );
    glBindRenderbufferEXT( GL_RENDERBUFFER_EXT , m_DepthStencilBufferHandle );
	glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT , GL_DEPTH_COMPONENT32 , ( GLsizei )m_Size.W, ( GLsizei )m_Size.H );

    //　フレームバッファの設定
    glGenFramebuffersEXT( 1, &m_FBOHandl );
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT , m_FBOHandl );
    glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT , GL_COLOR_ATTACHMENT0_EXT , GL_TEXTURE_2D , m_ColorBufferTextureHandl , 0 );
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT , GL_DEPTH_ATTACHMENT_EXT , GL_RENDERBUFFER_EXT , m_DepthStencilBufferHandle );
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT , 0 );
}

void CFramebufferObject::PreRender()
{
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, m_FBOHandl );
}

void CFramebufferObject::Render( void ( * render_func )( void ) )
{
    PreRender();
    render_func();
    PostRender();
}

void CFramebufferObject::PostRender()
{
    glBindFramebufferEXT( GL_FRAMEBUFFER_EXT , 0 );
}

void CFramebufferObject::Dispose()
{
    glDeleteFramebuffersEXT( 1 , &m_FBOHandl );
    glDeleteRenderbuffersEXT( 1 , &m_DepthStencilBufferHandle );
    glDeleteTextures( 1 , &m_ColorBufferTextureHandl );

    m_FBOHandl = 0;
    m_DepthStencilBufferHandle = 0;
    m_ColorBufferTextureHandl = 0;
}

} } }

#endif
