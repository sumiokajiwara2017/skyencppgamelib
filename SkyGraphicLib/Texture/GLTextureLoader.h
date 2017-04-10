/**************************************************************************
　　Texture.h

　　 Texture Loader Library

　　Date : Dec. 01, 2007
　　Version : 1.0
　　Author : Pocol
***************************************************************************/

#ifdef SKYLIB_RENDER_OPENGL4

#ifndef __TEXTURE_H_INCLUDED_
#define __TEXTURE_H_INCLUDED_

// Disable Warning C4996
#ifndef _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#endif
#ifndef _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES
#define _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES 1
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef USE_GLEW
#define USE_GLEW 1
#endif

#define USE_PNG_JPEG_LOAD

//
//　include
//
#include <iostream>
using namespace std;

//
// link
//
//
//　enum
//
typedef enum _IMAGE_FILEFORMAT
{
	_UNKOWN = -1,
	_BMP =1,
	_TGA,
#if USE_GLEW
	_DDS,
#endif
	_JPEG ,
	_PNG ,
};

class Rect
{
public:
	unsigned long left;
	unsigned long right;
	unsigned long top;
	unsigned long bottom;

	Rect(unsigned long l=0, unsigned long r=0, unsigned long t=0, unsigned long b=0)
		{ left = l; right = r; top = t; bottom = b; }
	Rect &operator= (const Rect &rect)
		{ left=rect.left; right=rect.right; top=rect.top; bottom=rect.bottom; return (*this); }
};

#ifdef SKYLIB_IDE_VC10

#pragma warning( disable : 4996 )	//メソッドにthrowを書いてもＯＫにする

#endif

namespace sky { namespace lib { namespace graphic {

//-------------------------------------------------------------------------------------------------------
//　　Texture class
//-------------------------------------------------------------------------------------------------------
class GLTextureLoader
{
	
public:
	GLTextureLoader(GLenum target=GL_TEXTURE_2D);
	~GLTextureLoader();
	skyBool IsLoaded();
	void Delete();
	void Parameter(GLenum pname, GLint param);
	void Enviorement(GLenum pname, GLint param);
	void Enable();
	void Disable();
	void SetHandle(GLuint handle) { m_handle=handle; }
	void SetTextureTarget(GLenum target) { m_target=target; }
	void SetNameA( const char name[]) { strcpy(m_nameA, name); }
	void SetNameW( const wchar_t name[]) { wcscpy(m_nameW, name); }
	GLuint GetHandle() { return m_handle; }
	GLenum GetTextureTarget() { return m_target; }
	char* GetNameA() { return m_nameA; }
	wchar_t* GetNameW() { return m_nameW; }
	GLTextureLoader& operator = (const GLTextureLoader &texture);
	bool operator == (const GLTextureLoader &texture);
	bool operator != (const GLTextureLoader &texture);

	//テクスチャの情報
	GLuint m_Width;
	GLuint m_Height;

protected:

	GLuint m_handle;
	GLenum m_target;
	char m_nameA[256];
	wchar_t m_nameW[256];
};

//
//　Function
//
void CreateTextureFromFileA( const char* filename, GLTextureLoader& texture);
void CreateTextureFromFileW( const wchar_t* filename, GLTextureLoader& texture);
void CreateTexture(int width, int height, GLuint format, GLTextureLoader& texture);
bool CreateImageFileA(char *filename, int width, int height, _IMAGE_FILEFORMAT format, unsigned char *pixel); 
bool CreateImageFileW(wchar_t * filename, int width, int height, _IMAGE_FILEFORMAT format, unsigned char *pixel);
bool SaveSurfaceToFileA(char *filename, Rect rect, _IMAGE_FILEFORMAT format);
bool SaveSurfaceToFileW(wchar_t *filename, Rect rect, _IMAGE_FILEFORMAT format);
#ifdef UNICODE
#define CreateTextureFromFile CreateTextureFromFileW
#define CrateImageFile CreateImageFileW
#define SaveSurfaceToFile SaveSurfaceToFileW
#else
#define CreateTexutreFromFile CrateTextureFromFileA
#define CrateImageFile CrateImageFileA
#define SaveSurfaceToFile SaveSurfaceToFileA
#endif
#endif	//　__TEXTURE_H_INCLUDED_

} } }

#endif
