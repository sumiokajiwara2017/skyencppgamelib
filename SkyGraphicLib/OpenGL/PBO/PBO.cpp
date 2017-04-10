#include "StdAfx.h"
#include "SkyGraphicLib/SkyGraphicLibTypes.h"
#include "SkyGraphicLib/OpenGL/SkyGraphicLibGL.h"

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

CPixelBufferObject::~CPixelBufferObject()
{
	Dispose();
}

void CPixelBufferObject::TransportFrameBufferToPBO()
{
	// フレームバッファの内容をPBOに転送
    glBindBuffer( GL_PIXEL_PACK_BUFFER, m_PBOHandle );                        // PBOをアクティブにする
    glReadPixels( 0 , 0 , ( GLsizei )m_Size.W , ( GLsizei )m_Size.H , GL_BGRA , GL_UNSIGNED_BYTE , 0 ); // 描画をPBOにロード
}

void CPixelBufferObject::TransportPBOToTexture()
{
    // PBOからテクスチャへ転送
    glBindBuffer( GL_PIXEL_UNPACK_BUFFER , m_PBOHandle );
    glBindTexture( GL_TEXTURE_2D, m_PBOTextureHandle );
	glTexSubImage2D( GL_TEXTURE_2D , 0 , 0 , 0 , ( GLsizei )m_Size.W , ( GLsizei )m_Size.H , GL_BGRA , GL_UNSIGNED_BYTE , 0 );
    glBindBuffer( GL_PIXEL_UNPACK_BUFFER , 0 );
}

void CPixelBufferObject::ConvertGrayScale()
{
    // PBOの内容を編集
    GLubyte *ptr = ( GLubyte * )glMapBuffer( GL_PIXEL_PACK_BUFFER , GL_READ_WRITE );
    if( ptr )
	{
        for(int i = 0; i < m_Size.W * m_Size.H; ++i )
		{
            GLubyte gray = ( GLubyte )( ( ptr[ m_ChannelNum * i ] + ptr[ m_ChannelNum * i + 1 ] + ptr[ m_ChannelNum * i + 2 ] ) / 3.0 );
            ptr[ m_ChannelNum * i ]     = gray;
            ptr[ m_ChannelNum * i + 1 ] = gray;
            ptr[ m_ChannelNum * i + 2 ] = gray;
        }
        glUnmapBuffer( GL_PIXEL_PACK_BUFFER );
    }
    glBindBuffer( GL_PIXEL_PACK_BUFFER , 0 );
}

void CPixelBufferObject::Initialize( const math::CBasicSize &size , u32 channelNum )
{
    SKY_ASSERT_MSG( channelNum == 4 , _T( "The number of channels supported only 4. \n" ) );

    // PBO作成とバインド
    glGenBuffers( 1 , &m_PBOHandle );
    glBindBuffer( GL_PIXEL_UNPACK_BUFFER , m_PBOHandle );
	glBufferData( GL_PIXEL_UNPACK_BUFFER , ( GLsizeiptr )( size.W * size.H * channelNum ) , 0 , GL_DYNAMIC_DRAW );    // バッファの作成と初期化
    glBindBuffer( GL_PIXEL_UNPACK_BUFFER , 0 );

	glPixelStorei( GL_UNPACK_ALIGNMENT , channelNum );

	glGenTextures( 1 , &m_PBOTextureHandle );
	glBindTexture( GL_TEXTURE_2D , m_PBOTextureHandle );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST ); //拡大・縮小フィルタ
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP );//繰り返しなし
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP );
	glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGBA , ( GLsizei )size.W , ( GLsizei )size.H , 0 , GL_BGRA , GL_UNSIGNED_BYTE , 0 );
	glBindTexture( GL_TEXTURE_2D , 0 );

	m_Size = size;
	m_ChannelNum = channelNum;
}

void CPixelBufferObject::Dispose()
{
    glBindBuffer( GL_ARRAY_BUFFER, m_PBOHandle );
    glDeleteBuffers( 1, &m_PBOHandle );
    glDeleteTextures( 1 , &m_PBOTextureHandle );

    m_PBOHandle = 0;
	m_PBOTextureHandle = 0;
}

CPixelBufferObjectPtr CPixelBufferObject::Create()
{
	return CPixelBufferObjectPtr( NEW__( CPixelBufferObject , CPixelBufferObject() ) );
}

CPixelBufferObject::CPixelBufferObject() : m_PBOHandle( 0 ) , m_PBOTextureHandle( 0 )
{

}

} } }

#endif
