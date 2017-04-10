#include "StdAfx.h"
#include "SkyGraphicLib/SkyGraphicLibTypes.h"
#include "SkyGraphicLib/OpenGL/SkyGraphicLibGL.h"

#include "SkyBaseLib/File.h"

#include <fcntl.h>

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

void CGLSL::CreateVertexShader( GLuint &dest , const skyString *src )
{
   stream::IStreamPtr iStream = CFileUtil_::LoadFile( CHS( src ) );
   CreateVertexShader( dest , iStream );
}

void CGLSL::CreateVertexShader( GLuint &dest , stream::IStreamPtr &src )
{
    //シェーダーオブジェクトの作成
    GLuint shader = glCreateShader(GL_VERTEX_SHADER);

	//コンパイル
    Compile_( shader , src );

	// プログラムオブジェクトの作成 
	dest = glCreateProgram();

	// シェーダオブジェクトをシェーダプログラムに関連付ける
	glAttachShader(dest, shader);

	// シェーダオブジェクトの削除 
	glDeleteShader(shader);

	//リンク
    Link_( dest );
}

void CGLSL::CreateFragmentShader( GLuint &dest , const skyString *src )
{
   stream::IStreamPtr iStream = CFileUtil_::LoadFile( CHS( src ) );
   CreateVertexShader( dest , iStream );
}

void CGLSL::CreateFragmentShader( GLuint &dest , stream::IStreamPtr &src )
{
    //シェーダーオブジェクトの作成
    GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);

	//コンパイル
    Compile_( shader , src );

	// プログラムオブジェクトの作成 
	dest = glCreateProgram();

	// シェーダオブジェクトをシェーダプログラムに関連付ける
	glAttachShader(dest, shader);

	// シェーダオブジェクトの削除 
	glDeleteShader(shader);

	//リンク
    Link_( dest );
}

void CGLSL::CreateVertexShaderAndFragmentShader( GLuint &dest , const skyString *srcVertex , const skyString *srcFragment )
{
   stream::IStreamPtr viStream = CFileUtil_::LoadFile( CHS( srcVertex ) );
   stream::IStreamPtr fiStream = CFileUtil_::LoadFile( CHS( srcFragment ) );
   CreateVertexShaderAndFragmentShader( dest , viStream , fiStream );
}

void CGLSL::CreateVertexShaderAndFragmentShader( GLuint &dest , stream::IStreamPtr &srcVertex , stream::IStreamPtr &srcFragment )
{
    //シェーダーオブジェクトの作成
    GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);

	//コンパイル
    Compile_( vshader , srcVertex );
    Compile_( fshader , srcFragment );

	// プログラムオブジェクトの作成 
	dest = glCreateProgram();

	// シェーダオブジェクトをシェーダプログラムに関連付ける
	glAttachShader(dest, vshader);
	glAttachShader(dest, fshader);

	// シェーダオブジェクトの削除 
	glDeleteShader(vshader);
	glDeleteShader(fshader);

	//リンク
    Link_( dest );
}

void CGLSL::Compile_( GLuint shader , stream::IStreamPtr &src )
{
    GLint compiled;

    //シェーダオブジェクトにプログラムをセット 
    GLsizei size = src->vGetSize();
    glShaderSource(shader, 1, (const GLchar **)src->vGetDataPtr() , &size);

	// シェーダのコンパイル 
	glCompileShader(shader);
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );

	if ( compiled == GL_FALSE ) 
	{
		glGetProgramiv( shader, GL_INFO_LOG_LENGTH, &size );
		if ( size > 0 ) 
		{
			char *buf;
			GLsizei len;
			buf = (char *)malloc(size);
			glGetShaderInfoLog( shader, size, &len, buf);
			OutputDebugStringA( buf );
			free(buf);
			SKY_PANIC_MSG( _T("It failed in the compilation of GLSL.")); //ＧＬＳＬのコンパイルに失敗しました。
		}
	}
}

void CGLSL::Link_( GLuint prog )
{
	GLsizei size;
    GLint linked;

	//リンク
	glLinkProgram( prog );

	glGetProgramiv( prog, GL_LINK_STATUS, &linked );

	if ( linked == GL_FALSE ) 
	{
		printf("リンクできませんでした!! \n");
	  	
		glGetProgramiv( prog, GL_INFO_LOG_LENGTH, &size );
		if ( size > 0 ) 
		{
			char *buf;
			GLsizei len;
			buf = (char *)malloc(size);
			glGetProgramInfoLog( prog, size, &len, buf );
			OutputDebugStringA(buf);
			free(buf);
		}
	}
}

} } }

#endif
