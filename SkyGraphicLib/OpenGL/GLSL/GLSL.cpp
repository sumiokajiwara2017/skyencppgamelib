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
    //�V�F�[�_�[�I�u�W�F�N�g�̍쐬
    GLuint shader = glCreateShader(GL_VERTEX_SHADER);

	//�R���p�C��
    Compile_( shader , src );

	// �v���O�����I�u�W�F�N�g�̍쐬 
	dest = glCreateProgram();

	// �V�F�[�_�I�u�W�F�N�g���V�F�[�_�v���O�����Ɋ֘A�t����
	glAttachShader(dest, shader);

	// �V�F�[�_�I�u�W�F�N�g�̍폜 
	glDeleteShader(shader);

	//�����N
    Link_( dest );
}

void CGLSL::CreateFragmentShader( GLuint &dest , const skyString *src )
{
   stream::IStreamPtr iStream = CFileUtil_::LoadFile( CHS( src ) );
   CreateVertexShader( dest , iStream );
}

void CGLSL::CreateFragmentShader( GLuint &dest , stream::IStreamPtr &src )
{
    //�V�F�[�_�[�I�u�W�F�N�g�̍쐬
    GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);

	//�R���p�C��
    Compile_( shader , src );

	// �v���O�����I�u�W�F�N�g�̍쐬 
	dest = glCreateProgram();

	// �V�F�[�_�I�u�W�F�N�g���V�F�[�_�v���O�����Ɋ֘A�t����
	glAttachShader(dest, shader);

	// �V�F�[�_�I�u�W�F�N�g�̍폜 
	glDeleteShader(shader);

	//�����N
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
    //�V�F�[�_�[�I�u�W�F�N�g�̍쐬
    GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);

	//�R���p�C��
    Compile_( vshader , srcVertex );
    Compile_( fshader , srcFragment );

	// �v���O�����I�u�W�F�N�g�̍쐬 
	dest = glCreateProgram();

	// �V�F�[�_�I�u�W�F�N�g���V�F�[�_�v���O�����Ɋ֘A�t����
	glAttachShader(dest, vshader);
	glAttachShader(dest, fshader);

	// �V�F�[�_�I�u�W�F�N�g�̍폜 
	glDeleteShader(vshader);
	glDeleteShader(fshader);

	//�����N
    Link_( dest );
}

void CGLSL::Compile_( GLuint shader , stream::IStreamPtr &src )
{
    GLint compiled;

    //�V�F�[�_�I�u�W�F�N�g�Ƀv���O�������Z�b�g 
    GLsizei size = src->vGetSize();
    glShaderSource(shader, 1, (const GLchar **)src->vGetDataPtr() , &size);

	// �V�F�[�_�̃R���p�C�� 
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
			SKY_PANIC_MSG( _T("It failed in the compilation of GLSL.")); //�f�k�r�k�̃R���p�C���Ɏ��s���܂����B
		}
	}
}

void CGLSL::Link_( GLuint prog )
{
	GLsizei size;
    GLint linked;

	//�����N
	glLinkProgram( prog );

	glGetProgramiv( prog, GL_LINK_STATUS, &linked );

	if ( linked == GL_FALSE ) 
	{
		printf("�����N�ł��܂���ł���!! \n");
	  	
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
