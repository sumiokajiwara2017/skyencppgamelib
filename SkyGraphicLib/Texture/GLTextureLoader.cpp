#include "StdAfx.h"
#include "SkyGraphicLib/Texture.h"

#ifdef SKYLIB_RENDER_OPENGL4

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

//
//　include
//
#include <fstream>
#ifdef USE_PNG_JPEG_LOAD
#include "SkyThirdPartyLib/pnglib/png.h"
extern "C" {
#define XMD_H
#include "SkyThirdPartyLib/jpeglib/jpeglib.h"
}
#endif
using namespace std;

namespace sky { namespace lib { namespace graphic {

//
//　define
//
#if  USE_GLEW
#define FOURCC_DXT1	'1TXD'
#define FOURCC_DXT2	'2TXD'
#define FOURCC_DXT3	'3TXD'
#define FOURCC_DXT4	'4TXD'
#define FOURCC_DXT5	'5TXD'
#define FOURCC_3DcATI	'2ITA'
#define FOURCC_A16B16G16R16		0x00000024
#define FOURCC_Q16W16V16U16	0x0000006E
#define FOURCC_R16F					0x0000006F
#define FOURCC_G16R16F				0x00000070
#define FOURCC_A16B16G16R16F	0x00000071
#define FOURCC_R32F1					0x00000072
#define FOURCC_G32R32F				0x00000073
#define FOURCC_A32B32G32R32F	0x00000074
#define FOURCC_CxV8U8				0x00000075
#define FOURCC_Q8W8V8U8			0x0000003F
#endif

//
//　typedef
//

//　BitmapFileHeader
typedef struct _BitmapFileHeader
{
	unsigned short bfType;
	unsigned long bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned long bfOffBits;
} BitmapFileHeader;

//　BitmapInfoHeader
typedef struct _BimapInfoHeader
{
	unsigned long biSize;
	long biWidth;
	long biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned long biCompression;
	unsigned long biSizeImage;
	long biXPelsPerMeter;
	long biYPelsPerMeter;
	unsigned long biClrUsed;
	unsigned long biClrImportant;
} BitmapInfoHeader;

//　BitmapCoreHeader
typedef struct _BitmapCoreHeader
{
	unsigned long bcSize;
	unsigned short bcWidth;
	unsigned short bcHeight;
	unsigned short bcPlanes;
	unsigned short bcBitCount;
} BitmapCoreHeader;

//　BitmapCoreInfo
typedef struct _BitmapCoreInfo
{
	BitmapCoreHeader bmciHeader;
	unsigned char bmciColors[3];
} BitmapCoreInfo;

//　TargaHeader
typedef struct _TargaHeader
{
	GLubyte ID_Length;
	GLubyte ColormapType;
	GLubyte ImageType;

	short cmFirst;
	short cmLength;
	GLubyte cmSize;
	
	short xOrigin;
	short yOrigin;
	
	short Width;
	short Height;
	
	GLubyte BitPerPixel;
	GLubyte ImageDescriptor;
} TargaHeader;

#ifdef USE_PNG_JPEG_LOAD
//　PCPaintbrushHeader
typedef struct _PCPaintburshHeader
{
	GLubyte manufactuer;
	GLubyte version;
	GLubyte encoding;
	GLubyte BitsPerPixel;

	GLushort xmin, ymin;
	GLushort xmax, ymax;
	GLushort horzRes, vertRes;

	GLubyte palette[48];
	GLubyte reserved;
	GLubyte numColorPlanes;

	GLushort bytesPerScanLine;
	GLushort paletteType;
	GLushort horzSize, vertSize;

	GLubyte padding[54];
} PCPaintbrushHeader;
#endif

//　DirectDrawSurfacePixelFormat
typedef struct _DDPixelFormat
{
	GLuint size;
	GLuint flags;
	GLuint fourCC;
	GLuint bpp;
	GLuint redMask;
	GLuint greenMask;
	GLuint blueMask;
	GLuint alphaMask;
} DDPixelFormat;

//　DirectDrawSurfaceCaps
typedef struct _DDSCaps
{
	GLuint caps;
	GLuint caps2;
	GLuint caps3;
	GLuint caps4;

} DDSCaps;

//　DirectDrawColorKey
typedef struct _DDColorKey
{
	GLuint lowVal;
	GLuint highval;

} DDColorKey;

//　DirectDrawSurfaceDesc
typedef struct _DDSurfaceDesc
{
	GLuint size;
	GLuint flags;
	GLuint height;
	GLuint width;
	GLuint pitch;
	GLuint depth;
	GLuint mipMapLevels;
	GLuint alphaBitDepth;
	GLuint reserved;
	GLuint surface;

	DDColorKey ckDestOverlay;
	DDColorKey ckDestBlt;
	DDColorKey ckSrcOverlay;
	DDColorKey ckSrcBlt;

	DDPixelFormat format;
	DDSCaps caps;

	GLuint textureStage;
} DDSurfaceDesc;

//　OStype
typedef enum
{
	BMP_WIN,
	BMP_OS2,
} OStype;

enum 
{
	//　BMP comression type constants
	Bi_RGB = 0,
	Bi_RLE8 = 1,
	Bi_RLE4 = 2,
	Bi_BITFILEDS = 3,
	//　RLE byte type constants
	RLE_COMMAND = 0,
	RLE_ENDOFLINE = 0,
	RLE_ENDOFBITMAP = 1,
	RLE_DELTA =2,
};

//　DDS_SupportFormat
typedef enum 
{
	NotSupport = 0,
	DXT1 = 1,
	DXT3 = 3,
	DXT5 = 5,
	A16B16G16R16 = 7,
	A8R8G8B8,
	A8B8G8R8,
	R8G8B8
} DDS_SupportFormat;

//--------------------------------------------------------------------------------------------------
//　　ImageObject class
//--------------------------------------------------------------------------------------------------
class ImageObject
{
protected:
	GLuint imageSize;
	GLubyte *imageData;
	GLenum format;
	GLuint internalFormat;
	GLuint width;
	GLuint height;
	GLuint bpp;
	GLuint handle;

public:
	ImageObject();
	virtual ~ImageObject();
	GLuint GetImageSize() { return imageSize; }
	GLubyte* GetImageData() { return imageData; }
	GLenum GetFormat() { return format; }
	GLuint GetInternalFormat() { return internalFormat; }
	GLuint GetWidth() { return width; }
	GLuint GetHeight() { return height; }
	GLuint GetBpp() { return bpp; }
	GLuint GetHandle() { return handle; }
	void SafeDelete() { if ( imageData ){ delete [] imageData; imageData=NULL; } }
};

//--------------------------------------------------------------------------------------------------
//　　BMPImage class
//--------------------------------------------------------------------------------------------------
class BMPImage : public ImageObject
{
protected:
	BitmapFileHeader bmfh;
	BitmapInfoHeader bmih;
	BitmapCoreHeader bmch;
public:
	void readBMP_1bits( FILE *fp, GLubyte *colormap, OStype ostype  );
	void readBMP_4bits( FILE *fp, GLubyte *colormap, OStype ostype );
	void readBMP_8bits( FILE *fp, GLubyte *colormap, OStype ostype );
	void readBMP_24bits( FILE *fp );
	void readBMP_32bits( FILE *fp );
	void readBMP_4bits_RLE( FILE *fp, GLubyte *colormap );
	void readBMP_8bits_RLE( FILE *fp, GLubyte *colormap );  
public:
	BMPImage() {}
	virtual ~BMPImage() {}
	bool ReadA( const char *filename );
	bool ReadW( const wchar_t *filename );
	GLuint LoadA( const char *filename );
	GLuint LoadW( const wchar_t *filename );
};

//--------------------------------------------------------------------------------------------------
//　　TGAImage class
//--------------------------------------------------------------------------------------------------
class TGAImage : public ImageObject
{
protected:
	TargaHeader header;
	void readTGA_8bits( FILE *fp, GLubyte *colormap );
	void readTGA_16bits( FILE *fp );
	void readTGA_24bits( FILE *fp );
	void readTGA_32bits( FILE *fp );
	void readTGA_8bits_Gray( FILE *fp );
	void readTGA_16bits_Gray( FILE *fp );
	void readTGA_8bits_RLE( FILE *fp, GLubyte *colormap );
	void readTGA_16bits_RLE( FILE *fp );
	void readTGA_24bits_RLE( FILE *fp );
	void readTGA_32bits_RLE( FILE *fp );
	void readTGA_8bits_GrayRLE( FILE *fp );
	void readTGA_16bits_GrayRLE( FILE *fp );

public:
	TGAImage() {}
	virtual ~TGAImage() {}
	bool ReadA( const char * filename );
	bool ReadW( const wchar_t * filename );
	GLuint LoadA( const char * filename );
	GLuint LoadW( const wchar_t * filename );
};

#if USE_GLEW
//-------------------------------------------------------------------------------------------------
//　　DDSImage class
//-------------------------------------------------------------------------------------------------
class DDSImage : public ImageObject
{
protected:
	DDSurfaceDesc ddsd;
	GLuint numMipmaps;
	DDS_SupportFormat SupportFormat;
public:
	DDSImage() {}
	virtual ~DDSImage() {}
	void Decompress();
	bool ReadA( const char *filename );
	bool ReadW( const wchar_t * filename );
	GLuint LoadA( const char *filename );
	GLuint LoadW( const wchar_t * filename );
};
#endif

#ifdef USE_PNG_JPEG_LOAD
//--------------------------------------------------------------------------------------------------
//　　PNGImage class
//--------------------------------------------------------------------------------------------------
class PNGImage : public ImageObject
{
public:
	PNGImage() {}
	virtual ~PNGImage() {}
	void GetPNG_TextureInfo( int color_type );
	bool ReadA( const char *filename );
	bool ReadW( const wchar_t * filename );
	GLuint LoadA( const char *filename );
	GLuint LoadW( const wchar_t * filename );
};

//-------------------------------------------------------------------------------------------------
//　　JPEGImage class
//-------------------------------------------------------------------------------------------------
class JPEGImage : public ImageObject
{
public:
	JPEGImage() {}
	virtual ~ JPEGImage() {}
	void Decompress( jpeg_decompress_struct *cInfo );
	bool ReadA( const char *filename );
	bool ReadW( const wchar_t * filename );
	GLuint LoadA( const char *filename );
	GLuint LoadW( const wchar_t * filename );
};
#endif

////////////////////////////////////////////////////////////////////////
//　　ImageObject class
////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------------
//　　ImageObject
//　　Desc : コンストラクタ
//--------------------------------------------------------------------------------------------------
ImageObject::ImageObject()
{
	imageSize = 0;
	imageData = NULL;
	format = GL_RGB;
	internalFormat = GL_RGB;
	width = 0;
	height = 0;
	bpp = 0;
	handle = 0;
}

//--------------------------------------------------------------------------------------------------
//　　~ImageObject
//　　Desc : デストラクタ
//--------------------------------------------------------------------------------------------------
ImageObject::~ImageObject()
{
}


////////////////////////////////////////////////////////////////////////
//　　BMPImage class
////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------------
//　　ReadA
//　　Desc : Bitmapファイルを読み込み
//--------------------------------------------------------------------------------------------------
bool BMPImage::ReadA(const char* filename)
{
	FILE *fp;
	fpos_t bmhPos;
	GLubyte *colormap = NULL;
	int colormapSize;
	unsigned int compression;
	OStype os_type;

	GLubyte header[14];		

	//　File Open
	if ( (fp = fopen(filename, "rb") ) ==NULL )
	{
		cout << "Error: couldn't open \"" << filename << "\"!\n";
		SKY_PANIC();
		return false;
	}

	//　ヘッダー情報を格納 
	fread( header, 1, sizeof( header ), fp );	//　エラー対策
	bmfh.bfType = header[0] + (header[1]<<8);
	bmfh.bfSize = header[2] + (header[3]<<8) + (header[4]<<16) + (header[5]<<24);
	bmfh.bfReserved1 = header[6] + (header[7]<<8);
	bmfh.bfReserved2 = header[8] + (header[9]<<8);
	bmfh.bfOffBits = header[10] + (header[11]<<8) + (header[12]<<16) + (header[13]<<24);
	fgetpos(fp, &bmhPos);

	//　Check Magic
	if ( bmfh.bfType != 0x4d42 )
	{
		cout << "Error: " << filename << "is not a valid BMP file!\n";
		fclose(fp);
		SKY_PANIC();
		return false;
	}

	fread( &bmih, sizeof(BitmapInfoHeader), 1, fp );
	format = GL_RGB;
	internalFormat = 3;

	///　Bitmap Style Check　///
	//　OS2 style
	if ( bmih.biCompression > 3 )
	{
		fsetpos( fp, &bmhPos );
		fread( &bmch, sizeof(BitmapCoreHeader), 1, fp );

		os_type = BMP_OS2;
		compression = Bi_RGB;
		bpp = bmch.bcBitCount;
		width = bmch.bcWidth;
		height = bmch.bcHeight;
	}
	//　Windows style
	else
	{
		compression = bmih.biCompression;
		os_type = BMP_WIN;
		bpp = bmih.biBitCount;
		width = bmih.biWidth;
		height = bmih.biHeight;
	}

	//　less than 8bit
	if ( bpp <= 8 )
	{
		colormapSize = ( 1 << bpp )*( ( os_type == BMP_OS2)? 3:4);
		colormap = new GLubyte [colormapSize];
		fread(colormap, sizeof(GLubyte), colormapSize, fp);
	}

	imageSize = width*height*internalFormat;
	imageData = new GLubyte [imageSize];
	fseek( fp, bmfh.bfOffBits, SEEK_SET );

	switch( compression )
	{
	case Bi_RGB:
		switch( bpp )
		{
		case 1:
			readBMP_1bits(fp, colormap, os_type);
			break;

		case 4:
			readBMP_4bits(fp, colormap, os_type);
			break;

		case 8:
			readBMP_8bits(fp, colormap, os_type);
			break;

		case 24:
			readBMP_24bits(fp);
			break;

		case 32:
			readBMP_32bits(fp);
			break;
		}
		break;

	case Bi_RLE8:
		readBMP_8bits_RLE(fp, colormap);
		break;

	case Bi_RLE4:
		readBMP_4bits_RLE(fp, colormap);
		break;

	case Bi_BITFILEDS:
	default:
		cout << "Error : Unsupported Bitmap Type or Bad file - ";
		cout << "compression type(" << compression << ")\n";
		SafeDelete();
		SKY_PANIC();
		break;
	}

	if ( colormap ) delete [] colormap;
	fclose(fp);
	return true;
}

//--------------------------------------------------------------------------------------------------
//　　ReadW
//　　Desc : Bitmapファイルを読み込み
//--------------------------------------------------------------------------------------------------
bool BMPImage::ReadW(const wchar_t* filename)
{
	FILE *fp;
	fpos_t bmhPos;
	GLubyte *colormap = NULL;
	int colormapSize;
	unsigned int compression;
	OStype os_type;

	GLubyte header[14];		

	//　File Open
	if ( (fp = _wfopen(filename, L"rb") ) ==NULL )
	{
		wcout << L"Error: couldn't open \"" << filename << L"\"!\n";
		SKY_PANIC();
		return false;
	}

	//　ヘッダー情報を格納 
	fread( header, 1, sizeof( header ), fp );	//　エラー対策
	bmfh.bfType = header[0] + (header[1]<<8);
	bmfh.bfSize = header[2] + (header[3]<<8) + (header[4]<<16) + (header[5]<<24);
	bmfh.bfReserved1 = header[6] + (header[7]<<8);
	bmfh.bfReserved2 = header[8] + (header[9]<<8);
	bmfh.bfOffBits = header[10] + (header[11]<<8) + (header[12]<<16) + (header[13]<<24);
	fgetpos(fp, &bmhPos);

	//　Check Magic
	if ( bmfh.bfType != 0x4d42 )
	{
		wcout << L"Error: " << filename << L"is not a valid BMP file!\n";
		fclose(fp);
		SKY_PANIC();
		return false;
	}

	fread( &bmih, sizeof(BitmapInfoHeader), 1, fp );
	format = GL_RGB;
	internalFormat = 3;

	///　Bitmap Style Check　///
	//　OS2 style
	if ( bmih.biCompression > 3 )
	{
		fsetpos( fp, &bmhPos );
		fread( &bmch, sizeof(BitmapCoreHeader), 1, fp );

		os_type = BMP_OS2;
		compression = Bi_RGB;
		bpp = bmch.bcBitCount;
		width = bmch.bcWidth;
		height = bmch.bcHeight;
	}
	//　Windows style
	else
	{
		compression = bmih.biCompression;
		os_type = BMP_WIN;
		bpp = bmih.biBitCount;
		width = bmih.biWidth;
		height = bmih.biHeight;
	}

	//　less than 8bit
	if ( bpp <= 8 )
	{
		colormapSize = ( 1 << bpp )*( ( os_type == BMP_OS2)? 3:4);
		colormap = new GLubyte [colormapSize];
		fread(colormap, sizeof(GLubyte), colormapSize, fp);
	}

	imageSize = width*height*internalFormat;
	imageData = new GLubyte [imageSize];
	fseek( fp, bmfh.bfOffBits, SEEK_SET );

	switch( compression )
	{
	case Bi_RGB:
		switch( bpp )
		{
		case 1:
			readBMP_1bits(fp, colormap, os_type);
			break;

		case 4:
			readBMP_4bits(fp, colormap, os_type);
			break;

		case 8:
			readBMP_8bits(fp, colormap, os_type);
			break;

		case 24:
			readBMP_24bits(fp);
			break;

		case 32:
			readBMP_32bits(fp);
			break;
		}
		break;

	case Bi_RLE8:
		readBMP_8bits_RLE(fp, colormap);
		break;

	case Bi_RLE4:
		readBMP_4bits_RLE(fp, colormap);
		break;

	case Bi_BITFILEDS:
	default:
		wcout << L"Error : Unsupported Bitmap Type or Bad file - ";
		wcout << L"compression type(" << compression << L")\n";
		SafeDelete();
		SKY_PANIC();
		break;
	}

	if ( colormap ) delete [] colormap;
	fclose(fp);
	return true;
}


//--------------------------------------------------------------------------------------------------
//　　LoadA
//　　Desc : Bitmapファイルを読み込みテクスチャを生成
//--------------------------------------------------------------------------------------------------
GLuint BMPImage::LoadA(const char *filename)
{
	if ( !ReadA(filename) )
	{
		cout << "Error: Bitmap File Load False\n";
		SKY_PANIC();
		return 0;
	}
	glGenTextures( 1, &handle );
	glBindTexture( GL_TEXTURE_2D, handle );
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	gluBuild2DMipmaps( GL_TEXTURE_2D, internalFormat, width, height, format, GL_UNSIGNED_BYTE, imageData );
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	SafeDelete();
	return handle;
}

//--------------------------------------------------------------------------------------------------
//　　LoadW
//　　Desc : Bitmapファイルを読み込みテクスチャを生成
//--------------------------------------------------------------------------------------------------
GLuint BMPImage::LoadW(const wchar_t *filename)
{
	if ( !ReadW(filename) )
	{
		wcout << L"Error: Bitmap File Load False\n";
		SKY_PANIC();
		return 0;
	}
	glGenTextures( 1, &handle );
	glBindTexture( GL_TEXTURE_2D, handle );
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	gluBuild2DMipmaps( GL_TEXTURE_2D, internalFormat, width, height, format, GL_UNSIGNED_BYTE, imageData );
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	SafeDelete();
	return handle;
}


//-----------------------------------------------------------------------------------------------
//　　readBMP_1bits
//　　Desc : モノクロ　読み込み
//-----------------------------------------------------------------------------------------------
void BMPImage::readBMP_1bits( FILE *fp, GLubyte *colormap, OStype ostype )
{
	int cmPixSize;
	GLubyte color, clrIndex;
	cmPixSize = (ostype == BMP_OS2) ? 3 : 4;

	for ( int i=0; i<(int)(width*height); )
	{
		color = (GLubyte)fgetc(fp);
		for ( int j=7; j>=0; j--, i++ )
		{
			clrIndex = ( (color&(i<<j)) > 0 );
			imageData[(i*3)+2] = colormap[(clrIndex*cmPixSize)+0];
			imageData[(i*3)+1] = colormap[(clrIndex*cmPixSize)+1];
			imageData[(i*3)+0] = colormap[(clrIndex*cmPixSize)+2];
		}
	}
}

//-----------------------------------------------------------------------------------------------
//　　readBMP_4bits
//　　Desc : 16色　読み込み
//-----------------------------------------------------------------------------------------------
void BMPImage::readBMP_4bits( FILE *fp, GLubyte *colormap, OStype ostype )
{
	int cmPixSize;
	GLubyte color, clrIndex;
	cmPixSize = (ostype == BMP_OS2) ? 3 : 4;

	for ( int i=0; i<(int)(width*height); i+=2 )
	{
		color = (GLubyte)fgetc(fp);
		clrIndex = (color>>4);
		imageData[(i*3)+2] = colormap[(clrIndex*cmPixSize)+0];
		imageData[(i*3)+1] = colormap[(clrIndex*cmPixSize)+1];
		imageData[(i*3)+0] = colormap[(clrIndex*cmPixSize)+2];
		clrIndex = (color&0x0F);
		imageData[(i*3)+5] = colormap[(clrIndex*cmPixSize)+0];
		imageData[(i*3)+4] = colormap[(clrIndex*cmPixSize)+1];
		imageData[(i*3)+3] = colormap[(clrIndex*cmPixSize)+2];
	}
}

//-----------------------------------------------------------------------------------------------
//　　readBMP_8bits
//　　Desc : 256色　読み込み
//-----------------------------------------------------------------------------------------------
void BMPImage::readBMP_8bits( FILE *fp, GLubyte *colormap, OStype ostype )
{
	int cmPixSize;
	GLubyte color;
	cmPixSize = (ostype == BMP_OS2) ? 3 : 4;

	for ( int i=0; i<(int)(width*height); i++ )
	{
		color = (GLubyte)fgetc(fp);
		imageData[(i*3)+2] = colormap[(color*cmPixSize)+0];
		imageData[(i*3)+1] = colormap[(color*cmPixSize)+1];
		imageData[(i*3)+0] = colormap[(color*cmPixSize)+2];
	}
}

//-----------------------------------------------------------------------------------------------
//　　readBMP_24bits
//　　Desc : 24bit　読み込み
//-----------------------------------------------------------------------------------------------
void BMPImage::readBMP_24bits( FILE *fp )
{
	for ( int i=0; i<(int)(width*height); i++ )
	{
		imageData[(i*3)+2] = (GLubyte)fgetc(fp);
		imageData[(i*3)+1] = (GLubyte)fgetc(fp);
		imageData[(i*3)+0] = (GLubyte)fgetc(fp);
	}
}

//-------------------------------------------------------------------------------------------------
//　　readBMP_32bits
//　　Desc : 32bit 読み込み
//------------------------------------------------------------------------------------------------
void BMPImage::readBMP_32bits( FILE *fp )
{
	GLubyte skip;
	
	for ( int i=0; i<(int)(width*height); i++ )
	{
		imageData[(i*3)+2] = (GLubyte)fgetc(fp);
		imageData[(i*3)+1] = (GLubyte)fgetc(fp);
		imageData[(i*3)+0] = (GLubyte)fgetc(fp);
		skip = (GLubyte)fgetc(fp);
	}
}

//-------------------------------------------------------------------------------------------------
//　　readBMP_8bits_RLE
//　　Desc : 8bit RLE　読み込み
//------------------------------------------------------------------------------------------------
void BMPImage::readBMP_8bits_RLE( FILE *fp, GLubyte *colormap )
{
	GLubyte color, skip;
	GLubyte byte1, byte2;
	GLubyte *ptr = imageData;

	while ( ptr < imageData + (width*height)*3 )
	{
		byte1 = (GLubyte)fgetc(fp);
		byte2 = (GLubyte)fgetc(fp);
		if ( byte1 == RLE_COMMAND )
		{
			for ( int i=0; i<byte2; i++, ptr+=3 )
			{
				color = (GLubyte)fgetc(fp);
				ptr[0] = colormap[(color*4)+2];
				ptr[1] = colormap[(color*4)+1];
				ptr[2] = colormap[(color*4)+0];
			}
			if ( byte2 %2 )
			{
				//　skip one byte if number of pixel is odd
				skip = (GLubyte)fgetc(fp);
			}
		}
		else
		{
			for ( int i=0; i<byte1; i++, ptr+=3 )
			{
				ptr[0] = colormap[(byte2*4)+2];
				ptr[1] = colormap[(byte2*4)+1];
				ptr[2] = colormap[(byte2*4)+0];
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
//　　readBMP_4bits_RLE
//　　Desc : 4bit RLE　読み込み
//-------------------------------------------------------------------------------------------------
void BMPImage::readBMP_4bits_RLE( FILE *fp, GLubyte *colormap )
{
	int bytesRead = 0;
	GLubyte color, databyte, skip;
	GLubyte byte1, byte2;
	GLubyte *ptr = imageData;

	while ( ptr < imageData + (width*height)*3 )
	{
		byte1 = (GLubyte)fgetc(fp);
		byte2 = (GLubyte)fgetc(fp);
		bytesRead += 2;

		if ( byte1 == RLE_COMMAND )
		{
			databyte = 0;
			for ( int i=0; i<byte2; i++, ptr+=3 )
			{
				if ( i % 2 )
				{
					//　four less significant bits
					color = (databyte&0x0F);
				}
				else
				{
					databyte = (GLubyte)fgetc(fp);
					bytesRead++;
					//　four most significant bits
					color = (databyte >> 4);
				}

				ptr[0] = colormap[(color*4) + 2];
				ptr[1] = colormap[(color*4) + 1];
				ptr[2] = colormap[(color*4) + 0];
			}

			if ( bytesRead % 2 )
			{
				//　skip one byte if number of read bytes is odd
				skip = (GLubyte)fgetc(fp);
				bytesRead++;
			}
		}
		else
		{
			for ( int i=0; i<byte1; i++, ptr+=3 )
			{
				if ( i % 2 )
					color = (byte2&0x0F);
				else
					color = (byte2 >> 4);
			
				//　convert from index color to RGB 24 bits
				ptr[0] = colormap[(color*4) + 2];
				ptr[1] = colormap[(color*4) + 1];
				ptr[2] = colormap[(color*4) + 0];
			}
		}
	}
}


////////////////////////////////////////////////////////////////////////
//　　TGAImage class
////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------------
//　　ReadA
//　　Desc : TGAファイルからデータを読み込む
//--------------------------------------------------------------------------------------------------
bool TGAImage::ReadA( const char *filename )
{

	FILE *fp;
	GLubyte *colormap = NULL;
	GLubyte tmp_header[18];		//　エラー回避用

	if ( ( fp = fopen(filename, "rb") ) == NULL )
	{
		cout << "Error: couldn't open \"" << filename << "\"!\n";
		SKY_PANIC();
		return false;
	}
	fread( &tmp_header, sizeof(tmp_header), 1, fp);
	header.ID_Length = tmp_header[0];
	header.ColormapType = tmp_header[1];
	header.ImageType = tmp_header[2];
	header.cmFirst = tmp_header[3] + (tmp_header[4]<<8);
	header.cmLength = tmp_header[5] + (tmp_header[6]<<8);
	header.cmSize = tmp_header[7];
	header.xOrigin = tmp_header[8] + (tmp_header[9]<<8);
	header.yOrigin = tmp_header[10] + (tmp_header[11]<<8);
	header.Width = tmp_header[12] + (tmp_header[13]<<8);
	header.Height = tmp_header[14] + (tmp_header[15]<<8);
	header.BitPerPixel = tmp_header[16];
	header.ImageDescriptor = tmp_header[17];

	width = header.Width;
	height = header.Height;

	switch( header.ImageType )
	{
	case 3:		//　grayscale 8 bits
	case 11:		//　grayscale 8 bit (compression RLE)
		if ( (int)header.BitPerPixel == 8 )
		{
			format = GL_LUMINANCE;
			internalFormat = 1;
		}
		else
		{
			format = GL_LUMINANCE;
			internalFormat = 2;
		}
		break;

	case 1:		//　index color 8 bits
	case 2:		//　BGR 16/24/32 bits
	case 9:		//　index color 8 bits (compression RLE)
	case 10:	//　BGR 16/24/32 bits (compression RLE)
		if ( (int)header.BitPerPixel <= 24 )
		{
			format = GL_RGB;
			internalFormat = 3;
		}
		else
		{
			format = GL_RGBA;
			internalFormat = 4;
		}
		break;
	}

	fseek( fp, header.ID_Length, SEEK_CUR );
	imageSize = width*height*internalFormat;
	imageData = new GLubyte[imageSize];

	if ( header.ColormapType )
	{
		colormap = new GLubyte [header.cmLength*(header.cmSize>>3)];
		fread( colormap, sizeof(GLubyte), header.cmLength*(header.cmSize>>3), fp );
	}

	switch ( header.ImageType )
	{
	case 0:
		cout << "header.ImageType value is Zero\n";
		break;

	case 1:
		readTGA_8bits(fp, colormap);
		break;

	case 2:
		switch( (int)header.BitPerPixel )
		{
		case 16:
			readTGA_16bits(fp);
			break;

		case 24:
			readTGA_24bits(fp);
			break;

		case 32:
			readTGA_32bits(fp);
			break;
		}
		break;

	case 3:
		if ( (int)header.BitPerPixel == 8 )
			readTGA_8bits_Gray(fp);
		else if ( (int)header.BitPerPixel == 16 )
			readTGA_16bits_Gray(fp);
		break;

	case 9:
		readTGA_8bits_RLE(fp, colormap);
		break;

	case 10:
		switch( (int)header.BitPerPixel )
		{
		case 16:
			readTGA_16bits_RLE(fp);
			break;

		case 24:
			readTGA_24bits_RLE(fp);
			break;

		case 32:
			readTGA_32bits_RLE(fp);
			break;
		}
		break;

	case 11:
		if ( (int)header.BitPerPixel == 8 )
			readTGA_8bits_GrayRLE(fp);
		else if ( (int)header.BitPerPixel == 16 )
			readTGA_16bits_GrayRLE(fp);
		break;

	default:
		cout << "Error : Unknown Targa image type\n";
		SafeDelete();
		SKY_PANIC();
		break;
	}

	if ( colormap ) 
	{
		delete [] colormap;
		colormap = NULL;
	}
	fclose(fp);
	return true;
}

//--------------------------------------------------------------------------------------------------
//　　ReadW
//　　Desc : TGAファイルからデータを読み込む
//--------------------------------------------------------------------------------------------------
bool TGAImage::ReadW( const wchar_t *filename )
{

	FILE *fp;
	GLubyte *colormap = NULL;
	GLubyte tmp_header[18];		//　エラー回避用

	if ( ( fp = _wfopen(filename, L"rb") ) == NULL )
	{
		wcout << L"Error: couldn't open \"" << filename << L"\"!\n";
		SKY_PANIC();
		return false;
	}
	fread( &tmp_header, sizeof(tmp_header), 1, fp);
	header.ID_Length = tmp_header[0];
	header.ColormapType = tmp_header[1];
	header.ImageType = tmp_header[2];
	header.cmFirst = tmp_header[3] + (tmp_header[4]<<8);
	header.cmLength = tmp_header[5] + (tmp_header[6]<<8);
	header.cmSize = tmp_header[7];
	header.xOrigin = tmp_header[8] + (tmp_header[9]<<8);
	header.yOrigin = tmp_header[10] + (tmp_header[11]<<8);
	header.Width = tmp_header[12] + (tmp_header[13]<<8);
	header.Height = tmp_header[14] + (tmp_header[15]<<8);
	header.BitPerPixel = tmp_header[16];
	header.ImageDescriptor = tmp_header[17];

	width = header.Width;
	height = header.Height;

	switch( header.ImageType )
	{
	case 3:		//　grayscale 8 bits
	case 11:		//　grayscale 8 bit (compression RLE)
		if ( (int)header.BitPerPixel == 8 )
		{
			format = GL_LUMINANCE;
			internalFormat = 1;
		}
		else
		{
			format = GL_LUMINANCE;
			internalFormat = 2;
		}
		break;

	case 1:		//　index color 8 bits
	case 2:		//　BGR 16/24/32 bits
	case 9:		//　index color 8 bits (compression RLE)
	case 10:	//　BGR 16/24/32 bits (compression RLE)
		if ( (int)header.BitPerPixel <= 24 )
		{
			format = GL_RGB;
			internalFormat = 3;
		}
		else
		{
			format = GL_RGBA;
			internalFormat = 4;
		}
		break;
	}

	fseek( fp, header.ID_Length, SEEK_CUR );
	imageSize = width*height*internalFormat;
	imageData = new GLubyte[imageSize];

	if ( header.ColormapType )
	{
		colormap = new GLubyte [header.cmLength*(header.cmSize>>3)];
		fread( colormap, sizeof(GLubyte), header.cmLength*(header.cmSize>>3), fp );
	}

	switch ( header.ImageType )
	{
	case 0:
		wcout << L"header.ImageType value is Zero\n";
		break;

	case 1:
		readTGA_8bits(fp, colormap);
		break;

	case 2:
		switch( (int)header.BitPerPixel )
		{
		case 16:
			readTGA_16bits(fp);
			break;

		case 24:
			readTGA_24bits(fp);
			break;

		case 32:
			readTGA_32bits(fp);
			break;
		}
		break;

	case 3:
		if ( (int)header.BitPerPixel == 8 )
			readTGA_8bits_Gray(fp);
		else if ( (int)header.BitPerPixel == 16 )
			readTGA_16bits_Gray(fp);
		break;

	case 9:
		readTGA_8bits_RLE(fp, colormap);
		break;

	case 10:
		switch( (int)header.BitPerPixel )
		{
		case 16:
			readTGA_16bits_RLE(fp);
			break;

		case 24:
			readTGA_24bits_RLE(fp);
			break;

		case 32:
			readTGA_32bits_RLE(fp);
			break;
		}
		break;

	case 11:
		if ( (int)header.BitPerPixel == 8 )
			readTGA_8bits_GrayRLE(fp);
		else if ( (int)header.BitPerPixel == 16 )
			readTGA_16bits_GrayRLE(fp);
		break;

	default:
		wcout << L"Error : Unknown Targa image type\n";
		SafeDelete();
		SKY_PANIC();
		break;
	}

	if ( colormap ) 
	{
		delete [] colormap;
		colormap = NULL;
	}
	fclose(fp);
	return true;
}


//-------------------------------------------------------------------------------------------------
//　　LoadA
//　　Desc : Targaファイルを読み込み，テクスチャを生成
//-------------------------------------------------------------------------------------------------
GLuint TGAImage::LoadA( const char *filename )
{
	if ( !ReadA(filename ) )
	{
		cout << "Error: Targa File Load False\n";
		SKY_PANIC();
		return 0;
	}
	glGenTextures( 1, &handle );
	glBindTexture( GL_TEXTURE_2D, handle );
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	gluBuild2DMipmaps( GL_TEXTURE_2D, internalFormat, width, height, format, GL_UNSIGNED_BYTE, imageData );
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	SafeDelete();
	return handle;
}

//-------------------------------------------------------------------------------------------------
//　　LoadW
//　　Desc : Targaファイルを読み込み，テクスチャを生成
//-------------------------------------------------------------------------------------------------
GLuint TGAImage::LoadW( const wchar_t *filename )
{
	if ( !ReadW(filename ) )
	{
		wcout << L"Error: Targa File Load False\n";
		SKY_PANIC();
		return 0;
	}
	glGenTextures( 1, &handle );
	glBindTexture( GL_TEXTURE_2D, handle );
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	gluBuild2DMipmaps( GL_TEXTURE_2D, internalFormat, width, height, format, GL_UNSIGNED_BYTE, imageData );
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	SafeDelete();
	return handle;
}


//--------------------------------------------------------------------------------------------------
//　　readTGA_8bits
//　　Desc : 8bit　読み込み
//--------------------------------------------------------------------------------------------------
void TGAImage::readTGA_8bits( FILE *fp, GLubyte *colormap )
{
	GLubyte color;
	for ( int i=0; i<(int)(width*height); i++ )
	{
		color = (GLubyte)fgetc(fp);
		imageData[(i*3)+2] = colormap[(color*3)+0];
		imageData[(i*3)+1] = colormap[(color*3)+1];
		imageData[(i*3)+0] = colormap[(color*3)+2];
	}
}

//---------------------------------------------------------------------------------------------------
//　　readTGA_16bits
//　　Desc : 16bit　読み込み
//--------------------------------------------------------------------------------------------------
void TGAImage::readTGA_16bits( FILE *fp )
{
	unsigned short color;
	for ( int i=0; i<(int)(width*height); i++ )
	{
		color = fgetc(fp) + (fgetc(fp) << 8);
		imageData[(i*3)+0] = (GLubyte)( ( (color&0x7C00) >> 10 ) << 3 );
		imageData[(i*3)+1] = (GLubyte)( ( (color&0x03E0) >>  5 ) << 3 );
		imageData[(i*3)+2] = (GLubyte)( ( (color&0x001F) >> 0 ) << 3 );
	}
}

//--------------------------------------------------------------------------------------------------
//　　readTGA_24bits
//　　Desc : 24bit　読み込み
//--------------------------------------------------------------------------------------------------
void TGAImage::readTGA_24bits( FILE *fp )
{
	for ( int i=0; i<(int)(width*height); i++ )
	{
		imageData[(i*3)+2] = (GLubyte)fgetc(fp);
		imageData[(i*3)+1] = (GLubyte)fgetc(fp);
		imageData[(i*3)+0] = (GLubyte)fgetc(fp);
	}
}

//--------------------------------------------------------------------------------------------------
//　　readTGA_32bits
//　　Desc : 32bit　読み込み
//--------------------------------------------------------------------------------------------------
void TGAImage::readTGA_32bits( FILE *fp )
{
	for ( int i=0; i<(int)(width*height); i++ )
	{
		imageData[(i*4)+2] = (GLubyte)fgetc(fp);
		imageData[(i*4)+1] = (GLubyte)fgetc(fp);
		imageData[(i*4)+0] = (GLubyte)fgetc(fp);
		imageData[(i*4)+3] = (GLubyte)fgetc(fp);
	}
}

//--------------------------------------------------------------------------------------------------
//　　readTGA_8bits_Gray
//　　Desc : 8bit グレ―スケール　読み込み
//--------------------------------------------------------------------------------------------------
void TGAImage::readTGA_8bits_Gray( FILE *fp )
{
	for ( int i=0; i<(int)(width*height); i++ )
	{
		imageData[i] = (GLubyte)fgetc(fp);
	}
}

//--------------------------------------------------------------------------------------------------
//　　readTGA_16bits_Gray
//　　Dsec : 16bit グレースケール　読み込み
//--------------------------------------------------------------------------------------------------
void TGAImage::readTGA_16bits_Gray( FILE *fp )
{
	for ( int i=0; i<(int)(width*height); i++ )
	{
		imageData[(i*2)+0] = (GLubyte)fgetc(fp);
		imageData[(i*2)+1] = (GLubyte)fgetc(fp);
	}
}

//--------------------------------------------------------------------------------------------------
//　　readTGA_8bits_RLE
//　　Desc : 8bit Targa(RLE)　読み込み
//--------------------------------------------------------------------------------------------------
void TGAImage::readTGA_8bits_RLE( FILE *fp, GLubyte *colormap )
{
	int size;
	GLubyte color, PacketHeader;
	GLubyte *ptr = imageData;

	while ( ptr < imageData + (width*height)*3 )
	{
		PacketHeader = (GLubyte)fgetc(fp);
		size = 1 + (PacketHeader&0x7F);
		if ( PacketHeader&0x80 )
		{
			color = (GLubyte)fgetc(fp);
			for ( int i=0; i<size; i++, ptr += 3 )
			{
				ptr[0] = colormap[(color*3)+2];
				ptr[1] = colormap[(color*3)+1];
				ptr[2] = colormap[(color*3)+0];
			}
		}
		else
		{
			for ( int i=0; i<size; i++, ptr += 3 )
			{
				color = (GLubyte)fgetc(fp);
				ptr[0] = colormap[(color*3)+2];
				ptr[1] = colormap[(color*3)+1];
				ptr[2] = colormap[(color*3)+0];
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------
//　　readTGA_16bits_RLE
//　　Desc : 16bit Targa(RLE)　読み込み
//--------------------------------------------------------------------------------------------------
void TGAImage::readTGA_16bits_RLE( FILE *fp )
{
	int size;
	unsigned short color;
	GLubyte PacketHeader;
	GLubyte *ptr = imageData;

	while ( ptr < imageData + (width*height)*3 )
	{
		PacketHeader = fgetc(fp);
		size = 1 + (PacketHeader&0x7F);
		if ( PacketHeader&0x80 )
		{
			
			color = fgetc(fp) + (fgetc(fp) <<8);
			for ( int i=0; i<size; i++, ptr += 3 )
			{
				ptr[0] = (GLubyte)( ( (color&0x7C00) >> 10 ) << 3 );
				ptr[1] = (GLubyte)( ( (color&0x03E0) >> 5 ) << 3 );
				ptr[2] = (GLubyte)( ( (color&0x001F) >> 0 ) << 3 );
			}
		}
		else
		{
			for ( int i=0; i<size; i++, ptr += 3 )
			{
				color = fgetc(fp) + (fgetc(fp)<<8);
				ptr[0] = (GLubyte)( ( (color&0x7C00) >> 10 ) << 3 );
				ptr[1] = (GLubyte)( ( (color&0x03E0) >> 5 ) << 3 );
				ptr[2] = (GLubyte)( ( (color&0x001F) >> 0 ) << 3 );
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------
//　　readTGA_24bits_RLE
//　　Desc : 24bit Targa(RLE)　読み込み
//--------------------------------------------------------------------------------------------------
void TGAImage::readTGA_24bits_RLE( FILE *fp )
{
	int size;
	GLubyte rgb[3];
	GLubyte PacketHeader;
	GLubyte *ptr = imageData;

	while ( ptr < imageData + (width*height)*3 )
	{
		PacketHeader = (GLubyte)fgetc(fp);
		size = 1 + (PacketHeader&0x7F);
		if ( PacketHeader&0x80 )
		{
			fread( rgb, sizeof(GLubyte), 3, fp );
			for ( int i=0; i<size; i++, ptr += 3 )
			{
				ptr[0] = rgb[2];
				ptr[1] = rgb[1];
				ptr[2] = rgb[0];
			}
		}
		else
		{
			for ( int i=0; i<size; i++, ptr += 3 )
			{
				ptr[2] = (GLubyte)fgetc(fp);
				ptr[1] = (GLubyte)fgetc(fp);
				ptr[0] = (GLubyte)fgetc(fp);
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------
//　　readTGA_32bits_RLE
//　　Desc : 32bit Targa(RLE)　読み込み
//-----------------------------------------------------------------------------------------------
void TGAImage::readTGA_32bits_RLE( FILE *fp )
{
	int size;
	GLubyte rgba[4];
	GLubyte PacketHeader;
	GLubyte *ptr = imageData;

	while ( ptr < imageData + (width*height)*4 )
	{
		PacketHeader = (GLubyte)fgetc(fp);
		size = 1 + (PacketHeader&0x7F);
		if ( PacketHeader&0x80 )
		{
			fread( rgba, sizeof(GLubyte), 4, fp );
			for ( int i=0; i<size; i++, ptr += 4 )
			{
				ptr[0] = rgba[2];
				ptr[1] = rgba[1];
				ptr[2] = rgba[0];
				ptr[3] = rgba[3];
			}
		}
		else
		{
			for ( int i=0; i<size; i++, ptr += 4 )
			{
				ptr[2] = (GLubyte)fgetc(fp);
				ptr[1] = (GLubyte)fgetc(fp);
				ptr[0] = (GLubyte)fgetc(fp);
				ptr[3] = (GLubyte)fgetc(fp);
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------
//　　readTGA_8bits_GrayRLE
//　　Desc : 8bit グレースケールTarga(RLE)　読み込み
//----------------------------------------------------------------------------------------------
void TGAImage::readTGA_8bits_GrayRLE( FILE *fp )
{
	int size;
	GLubyte color;
	GLubyte PacketHeader;
	GLubyte *ptr = imageData;

	while( ptr < imageData + (width*height) )
	{
		PacketHeader = (GLubyte)fgetc(fp);
		size = 1 + (PacketHeader&0x7F);
		if ( PacketHeader&0x80 )
		{
			color = (GLubyte)fgetc(fp);
			for ( int i=0; i<size; i++, ptr++ )
				*ptr = color;
		}
		else
		{
			for ( int i=0; i<size; i++, ptr++ )
				*ptr = (GLubyte)fgetc(fp);
		}
	}
}

//-----------------------------------------------------------------------------------------------
//　　readTGA_16bits_GrayRLE
//　　Desc : 16bit グレースケールTarga(RLE)　読み込み
//-----------------------------------------------------------------------------------------------
void TGAImage::readTGA_16bits_GrayRLE( FILE *fp )
{
	int size;
	GLubyte color, alpha;
	GLubyte PacketHeader;
	GLubyte *ptr = imageData;

	while ( ptr < imageData + (width*height)*2 )
	{
		PacketHeader = (GLubyte)fgetc(fp);
		size = 1 + (PacketHeader&0x7F);
		if ( PacketHeader&0x80 )
		{
			color = (GLubyte)fgetc(fp);
			alpha = (GLubyte)fgetc(fp);
			for ( int i=0; i<size; i++, ptr+= 2 )
			{
				ptr[0] = color;
				ptr[1] = alpha;
			}
		}
		else
		{
			for ( int i=0; i<size; i++, ptr+=2 )
			{
				ptr[0] = (GLubyte)fgetc(fp);
				ptr[1] = (GLubyte)fgetc(fp);
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////
//　　DDSImage class
///////////////////////////////////////////////////////////////////////
#if USE_GLEW

//---------------------------------------------------------------------------------------------
//　　ReadA
//　　Desc : Direct Draw Surfaceファイルの読み込み
//---------------------------------------------------------------------------------------------
bool DDSImage::ReadA( const char *filename )
{
	FILE *fp;
	char magic[4];
	int mipmapFactor;
	long curr, end;
	bool isSupportFormat = false;

	if ( glewInit() != GLEW_OK )
	{
		cout << "Error : glew initialize false\n";
		SKY_PANIC();
		return false;
	}

	fp = fopen( filename, "rb" );
	if ( !fp )
	{
		cout << "Error : couldn't open \"" << filename << "\"!\n";
		SKY_PANIC();
		return false;
	}

	fread( &magic, sizeof(char), 4, fp );
	
	if ( strncmp( magic, "DDS ", 4 ) != 0 )
	{
		cout << "Error : the file \"" << filename << "\" dosen't appear to be a valid .dds file!\n";
		fclose(fp);
		SKY_PANIC();
		return false;
	}

	fread( &ddsd, sizeof(ddsd), 1, fp );
	height = ddsd.height;
	width = ddsd.width;
	numMipmaps = ddsd.mipMapLevels;

	switch( ddsd.format.fourCC )
	{
	//　DXT1
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		internalFormat = 3;
		mipmapFactor = 2;
		SupportFormat = DXT1;
		break;

	//　DXT2
	case FOURCC_DXT2:
		cout << "Error: not support format \"DXT2\" !\n";
		SupportFormat = NotSupport;
		SKY_PANIC();
		break;

	//　DXT3
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		internalFormat = 4;
		mipmapFactor = 4;
		SupportFormat = DXT3;
		break;

	//　DXT4
	case FOURCC_DXT4:
		cout << "Error: not support format \"DXT4\" !\n";
		SupportFormat = NotSupport;
		SKY_PANIC();
		break;

	//　DXT5
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		internalFormat = 4;
		mipmapFactor = 4;
		SupportFormat = DXT5;
		break;

	//　3Dc ATI
	case FOURCC_3DcATI:
		cout << "Error : not support format \"3Dc ATI\" !\n";
		SupportFormat = NotSupport;
		SKY_PANIC();
		break;

	//　A16 B16 G16 R16
	case FOURCC_A16B16G16R16:
		internalFormat = GL_RGBA;
		format = GL_RGBA;
		SupportFormat = A16B16G16R16;
		break;

	//　Q16 W16 V16 U16
	case FOURCC_Q16W16V16U16:
		cout << "Error : not support format \"Q16 W16 V16 U16\" !\n";
		SupportFormat = NotSupport;
		SKY_PANIC();
		break;

	//　R16F
	case FOURCC_R16F:
		cout << "Error : not support format \"R16F\" !\n";
		SupportFormat = NotSupport;
		break;

	//　G16 R16F
	case FOURCC_G16R16F:
		cout << "Error : not support format \"G16 R16F\" !\n";
		SupportFormat = NotSupport;
		SKY_PANIC();
		break;

	//　A16 B16 G16 R16F
	case FOURCC_A16B16G16R16F:
		cout << "Error : not support format \"A16 B16 G16 R16F\" !\n";
		SupportFormat = NotSupport;
		SKY_PANIC();
		break;

	//　R32F1
	case FOURCC_R32F1:
		cout << "Error : not support format \"R32F1\" !\n";
		SupportFormat = NotSupport;
		break;

	//　G32 R32F
	case FOURCC_G32R32F:
		cout << "Error : not support format \"G32 R32F\" !\n";
		SupportFormat = NotSupport;
		SKY_PANIC();
		break;

	//　A32 B32 G32 R32F
	case FOURCC_A32B32G32R32F:
		cout << "Error : not support format \"A32 B32 G32 R32F\" !\n";
		SupportFormat = NotSupport;
		break;

	//　Cx V8 U8
	case FOURCC_CxV8U8:
		cout << "Error : not support format \"Cx V8 U9\" !\n";
		SupportFormat = NotSupport;
		SKY_PANIC();
		break;

	//　Q8 W8 V8 U8
	case FOURCC_Q8W8V8U8:
		cout << "Error : not support format \"Q8 W8 V8 U8\" !\n";
		isSupportFormat = false;
		SKY_PANIC();

	default:
		cout << "Error : not support format !\n";
		isSupportFormat = false;
		SKY_PANIC();
	}

	if ( SupportFormat == NotSupport )
	{
		fclose(fp);
		return false;
	}

	curr = ftell(fp);
	fseek( fp, 0, SEEK_END );
	end = ftell(fp);
	fseek( fp, curr, SEEK_SET );
	imageSize = end - curr;
	imageData = new GLubyte [imageSize];

	fread( imageData, sizeof(GLubyte), imageSize, fp );
	fclose(fp);

	//　ARGB
	if ( ddsd.format.redMask == 0x00FF0000
		&& ddsd.format.greenMask == 0x0000FF00
		&& ddsd.format.blueMask == 0x0000000FF
		&& ddsd.format.alphaMask == 0xFF000000 )
	{
		//　A8 R8 G8 B8
		if ( ddsd.format.flags == 65 )
		{
			//　convert BGRA to RGBA
			for ( int i=0; i<(int)imageSize; i+= 4 )
			{
				GLubyte temp;
				temp = imageData[i];
				imageData[i] = imageData[i+2];
				imageData[i+2] = temp;
				internalFormat = GL_RGBA;
				format = GL_RGBA;
				SupportFormat = A8R8G8B8;
			}
		}
	}
	//　ABGR
	else if ( ddsd.format.redMask == 0x000000FF
		&& ddsd.format.greenMask == 0x0000FF00
		&& ddsd.format.blueMask == 0x00FF0000
		&& ddsd.format.alphaMask == 0xFF000000 )
	{
		//　A8 B8 G8 R8
		if ( ddsd.format.flags == 65 )
		{
			internalFormat = GL_RGBA;
			format = GL_RGBA;
			SupportFormat = A8B8G8R8;
		}
	}

	//　BGR
	else if ( ddsd.format.redMask == 0x00FF0000
		&& ddsd.format.greenMask == 0x0000FF00
		&& ddsd.format.blueMask == 0x000000FF
		&& ddsd.format.alphaMask == 0x00000000 )
	{
		//　R8 G8 B8
		if ( ddsd.format.flags == 64 )
		{
			//　convert to BGR to RGB
			for ( int i=0; i<(int)imageSize; i+= 3 )
			{
				GLubyte temp;
				temp = imageData[i];
				imageData[i] = imageData[i+2];
				imageData[i+2] = temp;
				internalFormat = GL_RGB;
				format = GL_RGB;
				SupportFormat = R8G8B8;
			}
		}
	}

	return true;
}

//---------------------------------------------------------------------------------------------
//　　ReadW
//　　Desc : Direct Draw Surfaceファイルの読み込み
//---------------------------------------------------------------------------------------------
bool DDSImage::ReadW( const wchar_t *filename )
{
	FILE *fp;
	char magic[4];
	int mipmapFactor;
	long curr, end;
	bool isSupportFormat = false;

	if ( glewInit() != GLEW_OK )
	{
		cout << "Error : glew initialize false\n";
		SKY_PANIC();
		return false;
	}

	fp = _wfopen( filename, L"rb" );
	if ( !fp )
	{
		wcout << L"Error : couldn't open \"" << filename << L"\"!\n";
		SKY_PANIC();
		return false;
	}

	fread( &magic, sizeof(char), 4, fp );
	
	if ( strncmp( magic, "DDS ", 4 ) != 0 )
	{
		wcout << L"Error : the file \"" << filename << L"\" dosen't appear to be a valid .dds file!\n";
		fclose(fp);
		SKY_PANIC();
		return false;
	}

	fread( &ddsd, sizeof(ddsd), 1, fp );
	height = ddsd.height;
	width = ddsd.width;
	numMipmaps = ddsd.mipMapLevels;

	switch( ddsd.format.fourCC )
	{
	//　DXT1
	case FOURCC_DXT1:
		format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		internalFormat = 3;
		mipmapFactor = 2;
		SupportFormat = DXT1;
		break;

	//　DXT2
	case FOURCC_DXT2:
		wcout << L"Error: not support format \"DXT2\" !\n";
		SupportFormat = NotSupport;
		SKY_PANIC();
		break;

	//　DXT3
	case FOURCC_DXT3:
		format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		internalFormat = 4;
		mipmapFactor = 4;
		SupportFormat = DXT3;
		break;

	//　DXT4
	case FOURCC_DXT4:
		wcout << L"Error: not support format \"DXT4\" !\n";
		SupportFormat = NotSupport;
		SKY_PANIC();
		break;

	//　DXT5
	case FOURCC_DXT5:
		format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		internalFormat = 4;
		mipmapFactor = 4;
		SupportFormat = DXT5;
		break;

	//　3Dc ATI
	case FOURCC_3DcATI:
		wcout << L"Error : not support format \"3Dc ATI\" !\n";
		SupportFormat = NotSupport;
		SKY_PANIC();
		break;

	//　A16 B16 G16 R16
	case FOURCC_A16B16G16R16:
		internalFormat = GL_RGBA;
		format = GL_RGBA;
		SupportFormat = A16B16G16R16;
		break;

	//　Q16 W16 V16 U16
	case FOURCC_Q16W16V16U16:
		wcout << L"Error : not support format \"Q16 W16 V16 U16\" !\n";
		SupportFormat = NotSupport;
		SKY_PANIC();
		break;

	//　R16F
	case FOURCC_R16F:
		wcout << L"Error : not support format \"R16F\" !\n";
		SupportFormat = NotSupport;
		SKY_PANIC();
		break;

	//　G16 R16F
	case FOURCC_G16R16F:
		wcout << L"Error : not support format \"G16 R16F\" !\n";
		SupportFormat = NotSupport;
		SKY_PANIC();
		break;

	//　A16 B16 G16 R16F
	case FOURCC_A16B16G16R16F:
		wcout << L"Error : not support format \"A16 B16 G16 R16F\" !\n";
		SupportFormat = NotSupport;
		SKY_PANIC();
		break;

	//　R32F1
	case FOURCC_R32F1:
		wcout << L"Error : not support format \"R32F1\" !\n";
		SupportFormat = NotSupport;
		SKY_PANIC();
		break;

	//　G32 R32F
	case FOURCC_G32R32F:
		wcout << L"Error : not support format \"G32 R32F\" !\n";
		SupportFormat = NotSupport;
		SKY_PANIC();
		break;

	//　A32 B32 G32 R32F
	case FOURCC_A32B32G32R32F:
		wcout << L"Error : not support format \"A32 B32 G32 R32F\" !\n";
		SupportFormat = NotSupport;
		SKY_PANIC();
		break;

	//　Cx V8 U8
	case FOURCC_CxV8U8:
		wcout << L"Error : not support format \"Cx V8 U9\" !\n";
		SupportFormat = NotSupport;
		SKY_PANIC();
		break;

	//　Q8 W8 V8 U8
	case FOURCC_Q8W8V8U8:
		wcout << L"Error : not support format \"Q8 W8 V8 U8\" !\n";
		isSupportFormat = false;
		SKY_PANIC();

	default:
		wcout << L"Error : not support format !\n";
		isSupportFormat = false;
		SKY_PANIC();
	}

	if ( SupportFormat == NotSupport )
	{
		fclose(fp);
		return false;
	}

	curr = ftell(fp);
	fseek( fp, 0, SEEK_END );
	end = ftell(fp);
	fseek( fp, curr, SEEK_SET );
	imageSize = end - curr;
	imageData = new GLubyte [imageSize];

	fread( imageData, sizeof(GLubyte), imageSize, fp );
	fclose(fp);

	//　ARGB
	if ( ddsd.format.redMask == 0x00FF0000
		&& ddsd.format.greenMask == 0x0000FF00
		&& ddsd.format.blueMask == 0x0000000FF
		&& ddsd.format.alphaMask == 0xFF000000 )
	{
		//　A8 R8 G8 B8
		if ( ddsd.format.flags == 65 )
		{
			//　convert BGRA to RGBA
			for ( int i=0; i<(int)imageSize; i+= 4 )
			{
				GLubyte temp;
				temp = imageData[i];
				imageData[i] = imageData[i+2];
				imageData[i+2] = temp;
				internalFormat = GL_RGBA;
				format = GL_RGBA;
				SupportFormat = A8R8G8B8;
			}
		}
	}
	//　ABGR
	else if ( ddsd.format.redMask == 0x000000FF
		&& ddsd.format.greenMask == 0x0000FF00
		&& ddsd.format.blueMask == 0x00FF0000
		&& ddsd.format.alphaMask == 0xFF000000 )
	{
		//　A8 B8 G8 R8
		if ( ddsd.format.flags == 65 )
		{
			internalFormat = GL_RGBA;
			format = GL_RGBA;
			SupportFormat = A8B8G8R8;
		}
	}

	//　BGR
	else if ( ddsd.format.redMask == 0x00FF0000
		&& ddsd.format.greenMask == 0x0000FF00
		&& ddsd.format.blueMask == 0x000000FF
		&& ddsd.format.alphaMask == 0x00000000 )
	{
		//　R8 G8 B8
		if ( ddsd.format.flags == 64 )
		{
			//　convert to BGR to RGB
			for ( int i=0; i<(int)imageSize; i+= 3 )
			{
				GLubyte temp;
				temp = imageData[i];
				imageData[i] = imageData[i+2];
				imageData[i+2] = temp;
				internalFormat = GL_RGB;
				format = GL_RGB;
				SupportFormat = R8G8B8;
			}
		}
	}

	return true;
}
	
//---------------------------------------------------------------------------------------------
//　　LoadA
//　　Desc : DDSファイルを読み込み，テクスチャを生成
//---------------------------------------------------------------------------------------------
GLuint DDSImage::LoadA( const char *filename )
{
	if ( !ReadA( filename ) )
	{
		cout << "Error : Direct Draw Surface Load File False\n";
		SKY_PANIC();
		return 0;
	}
	glGenTextures( 1, &handle );
	glBindTexture( GL_TEXTURE_2D, handle );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	
	switch ( SupportFormat )
	{
	case DXT1:
	case DXT3:
	case DXT5:
		Decompress();
		break;

	case A8R8G8B8:
	case A8B8G8R8:
	case R8G8B8:
		gluBuild2DMipmaps( GL_TEXTURE_2D, internalFormat, width, height, format, GL_UNSIGNED_BYTE, imageData );
		break;

	case A16B16G16R16:
		gluBuild2DMipmaps( GL_TEXTURE_2D, internalFormat, width, height, format, GL_UNSIGNED_SHORT, imageData );
		break;

	default:
		break;
	}

	SafeDelete();
	return handle;
}

	
//---------------------------------------------------------------------------------------------
//　　LoadW
//　　Desc : DDSファイルを読み込み，テクスチャを生成
//---------------------------------------------------------------------------------------------
GLuint DDSImage::LoadW( const wchar_t *filename )
{
	if ( !ReadW( filename ) )
	{
		wcout << L"Error : Direct Draw Surface Load File False\n";
		SKY_PANIC();
		return 0;
	}
	glGenTextures( 1, &handle );
	glBindTexture( GL_TEXTURE_2D, handle );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	
	switch ( SupportFormat )
	{
	case DXT1:
	case DXT3:
	case DXT5:
		Decompress();
		break;

	case A8R8G8B8:
	case A8B8G8R8:
	case R8G8B8:
		gluBuild2DMipmaps( GL_TEXTURE_2D, internalFormat, width, height, format, GL_UNSIGNED_BYTE, imageData );
		break;

	case A16B16G16R16:
		gluBuild2DMipmaps( GL_TEXTURE_2D, internalFormat, width, height, format, GL_UNSIGNED_SHORT, imageData );
		break;

	default:
		break;
	}

	SafeDelete();
	return handle;
}


//---------------------------------------------------------------------------------------------
//　　Decompress
//　　Desc : S3TC圧縮ファイルの解凍
//---------------------------------------------------------------------------------------------
void DDSImage::Decompress()
{
	int BlockSize;
	int offset = 0;
	GLint i;
	GLsizei mWidth = width, mHeight = height, mSize = 0;

	if ( format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT )
		BlockSize = 8;
	else
		BlockSize = 16;

	for ( i=0; i<(int)numMipmaps; i++ )
	{
		mSize = ( (mWidth+3)/4 )*( (mHeight+3)/4 )*BlockSize;
		glCompressedTexImage2D( GL_TEXTURE_2D, i, format, mWidth, mHeight, 0, mSize, imageData + offset );
		if ( (mWidth >> 1) )
			mWidth = (mWidth>>1);
		else
			mWidth = 1;
		if ( (mHeight >> 1) )
			mHeight = (mHeight>>1);
		else
			mHeight = 1;
		offset += mSize;
	}
}
#endif

#ifdef USE_PNG_JPEG_LOAD
///////////////////////////////////////////////////////////////////////
//　　PNGImage class
///////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------
//　　GetPNG_TextureInfo
//　　Desc:
//---------------------------------------------------------------------------------------------
void PNGImage::GetPNG_TextureInfo( int color_type )
{
	switch( color_type )
	{
	case PNG_COLOR_TYPE_GRAY:
		format = GL_LUMINANCE;
		internalFormat = 1;
		break;

	case PNG_COLOR_TYPE_GRAY_ALPHA:
		format = GL_RGB;
		internalFormat = 3;
		break;

	case PNG_COLOR_TYPE_RGB:
		format = GL_RGB;
		internalFormat = 3;
		break;

	case PNG_COLOR_TYPE_RGB_ALPHA:
		format = GL_RGBA;
		internalFormat = 4;
		break;

	default:
		break;
	}
}

//----------------------------------------------------------------------------------------------
//　　ReadA
//　　Desc : Portable Network Graphicファイルからデータを読み込む
//-----------------------------------------------------------------------------------------------
bool PNGImage::ReadA( const char *filename )
{
	png_byte magic[8];
	png_structp png_ptr;
	png_infop info_ptr;
	int bit_depth, color_type;
	FILE *fp = NULL;
	 png_bytep *row_pointers = NULL;
	int i;

	fp = fopen( filename, "rb" );
	if ( !fp )
	{
		cout << "Error : couldn't open \"" << filename << "\"!\n";
		SKY_PANIC();
		return false;
	 }

	// read magic number
	fread (magic, 1, sizeof (magic), fp);

	// check for valid magic number 
	if (!png_check_sig (magic, sizeof (magic)))
	{
		cout << "Error: this file not PNG file \"" << filename << "\"!\n";
		fclose (fp);
		SKY_PANIC();
		return false;
    }

	 // create a png read struct 
	png_ptr = png_create_read_struct(
		  PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	 {
	   fclose (fp);
	   return false;
	 }

	// create a png info struct 
	info_ptr = png_create_info_struct (png_ptr);
	if (!info_ptr)
    {
		fclose (fp);
		png_destroy_read_struct (&png_ptr, NULL, NULL);
		return false;
    }

	// initialize the setjmp for returning properly after a libpng error occured 
	if (setjmp (png_jmpbuf (png_ptr)))
    {
	   fclose (fp);
	   png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

		if (row_pointers)
		delete [] row_pointers;

		return false;
    }

	// setup libpng for using standard C fread() function with our FILE pointer 
	png_init_io (png_ptr, fp);

	// tell libpng that we have already read the magic number 
	png_set_sig_bytes (png_ptr, sizeof (magic));

	// read png info 
	png_read_info (png_ptr, info_ptr);

	// get some usefull information from header 
	bit_depth = png_get_bit_depth (png_ptr, info_ptr);
	color_type = png_get_color_type (png_ptr, info_ptr);

	// convert index color images to RGB images 
	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb (png_ptr);

	// convert 1-2-4 bits grayscale images to 8 bits grayscale. 
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8 (png_ptr);

	if (png_get_valid (png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha (png_ptr);

	if (bit_depth == 16)
		png_set_strip_16 (png_ptr);
	else if (bit_depth < 8)
		png_set_packing (png_ptr);

	// update info structure to apply transformations 
	png_read_update_info (png_ptr, info_ptr);

	// retrieve updated information 
	png_get_IHDR (png_ptr, info_ptr,
		(png_uint_32*)(&width),
		(png_uint_32*)(&height),
		&bit_depth, &color_type,
		NULL, NULL, NULL);

	// get image format and components per pixel 
	GetPNG_TextureInfo (color_type);

	// we can now allocate memory for storing pixel data 
	imageSize = sizeof( GLubyte) * width * height * internalFormat;
	imageData = new GLubyte[imageSize];
	bpp = bit_depth;

	// setup a pointer array.  Each one points at the begening of a row. 
	row_pointers = new png_bytep [sizeof(png_bytep)*height];

	for (i = 0; i < (int)height; ++i)
    {
		  row_pointers[i] = (png_bytep)(imageData +
		((height - (i + 1)) * width * internalFormat));
	}

	// read pixel data using row pointers 
	png_read_image (png_ptr, row_pointers);

	// finish decompression and release memory 
	png_read_end (png_ptr, NULL);
	png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

	// we don't need row pointers anymore 
	delete [] row_pointers;

	fclose (fp);
	return true;
}


//----------------------------------------------------------------------------------------------
//　　ReadW
//　　Desc : Portable Network Graphicファイルからデータを読み込む
//-----------------------------------------------------------------------------------------------
bool PNGImage::ReadW( const wchar_t *filename )
{
	png_byte magic[8];
	png_structp png_ptr;
	png_infop info_ptr;
	int bit_depth, color_type;
	FILE *fp = NULL;
	 png_bytep *row_pointers = NULL;
	int i;

	fp = _wfopen( filename, L"rb" );
	if ( !fp )
	{
		wcout << L"Error : couldn't open \"" << filename << L"\"!\n";
		SKY_PANIC();
		return false;
	 }

	// read magic number
	fread (magic, 1, sizeof (magic), fp);

	// check for valid magic number 
	if (!png_check_sig (magic, sizeof (magic)))
	{
		wcout << L"Error: this file not PNG file \"" << filename << L"\"!\n";
		fclose (fp);
		SKY_PANIC();
		return false;
    }

	 // create a png read struct 
	png_ptr = png_create_read_struct(
		  PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	 {
	   fclose (fp);
	   return false;
	 }

	// create a png info struct 
	info_ptr = png_create_info_struct (png_ptr);
	if (!info_ptr)
    {
		fclose (fp);
		png_destroy_read_struct (&png_ptr, NULL, NULL);
		return false;
    }

	// initialize the setjmp for returning properly after a libpng error occured 
	if (setjmp (png_jmpbuf (png_ptr)))
    {
	   fclose (fp);
	   png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

		if (row_pointers)
		delete [] row_pointers;

		return false;
    }

	// setup libpng for using standard C fread() function with our FILE pointer 
	png_init_io (png_ptr, fp);

	// tell libpng that we have already read the magic number 
	png_set_sig_bytes (png_ptr, sizeof (magic));

	// read png info 
	png_read_info (png_ptr, info_ptr);

	// get some usefull information from header 
	bit_depth = png_get_bit_depth (png_ptr, info_ptr);
	color_type = png_get_color_type (png_ptr, info_ptr);

	// convert index color images to RGB images 
	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb (png_ptr);

	// convert 1-2-4 bits grayscale images to 8 bits grayscale. 
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8 (png_ptr);

	if (png_get_valid (png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha (png_ptr);

	if (bit_depth == 16)
		png_set_strip_16 (png_ptr);
	else if (bit_depth < 8)
		png_set_packing (png_ptr);

	// update info structure to apply transformations 
	png_read_update_info (png_ptr, info_ptr);

	// retrieve updated information 
	png_get_IHDR (png_ptr, info_ptr,
		(png_uint_32*)(&width),
		(png_uint_32*)(&height),
		&bit_depth, &color_type,
		NULL, NULL, NULL);

	// get image format and components per pixel 
	GetPNG_TextureInfo (color_type);

	// we can now allocate memory for storing pixel data 
	imageSize = sizeof( GLubyte) * width * height * internalFormat;
	imageData = new GLubyte[imageSize];
	bpp = bit_depth;

	// setup a pointer array.  Each one points at the begening of a row. 
	row_pointers = new png_bytep [sizeof(png_bytep)*height];

	for (i = 0; i < (int)height; ++i)
    {
		  row_pointers[i] = (png_bytep)(imageData +
		((height - (i + 1)) * width * internalFormat));
	}

	// read pixel data using row pointers 
	png_read_image (png_ptr, row_pointers);

	// finish decompression and release memory 
	png_read_end (png_ptr, NULL);
	png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

	// we don't need row pointers anymore 
	delete [] row_pointers;

	fclose (fp);
	return true;
}

//---------------------------------------------------------------------------------------------
//　　LoadA
//　　Desc : PNGファイルを読み込み，テクスチャを生成
//---------------------------------------------------------------------------------------------
GLuint PNGImage::LoadA( const char *filename )
{
	if ( !ReadA(filename) )
	{
		cout << "Error : Portable Network Graphic Load File False\n";
		SKY_PANIC();
		return 0;
	}
	glGenTextures( 1, &handle );
	glBindTexture( GL_TEXTURE_2D, handle );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	gluBuild2DMipmaps( GL_TEXTURE_2D, internalFormat, width, height, format, GL_UNSIGNED_BYTE, imageData );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	SafeDelete();
	return handle;
}

//---------------------------------------------------------------------------------------------
//　　LoadW
//　　Desc : PNGファイルを読み込み，テクスチャを生成
//---------------------------------------------------------------------------------------------
GLuint PNGImage::LoadW( const wchar_t *filename )
{
	if ( !ReadW(filename) )
	{
		wcout << L"Error : Portable Network Graphic Load File False\n";
		SKY_PANIC();
		return 0;
	}
	glGenTextures( 1, &handle );
	glBindTexture( GL_TEXTURE_2D, handle );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	gluBuild2DMipmaps( GL_TEXTURE_2D, internalFormat, width, height, format, GL_UNSIGNED_BYTE, imageData );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	SafeDelete();
	return handle;
}

///////////////////////////////////////////////////////////////////////
//　　JPEGImage class
///////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------------------------
//　　ReadA
//　　Desc : Jpegファイルを読み込む
//---------------------------------------------------------------------------------------------
bool JPEGImage::ReadA( const char *filename )
{
	struct jpeg_decompress_struct cInfo;
	FILE *fp;
	GLubyte temp;

	fp = fopen( filename, "rb" );
    if ( !fp )
	{
		cout << "Error : couldn't open \"" << filename << "\"!\n";
		SKY_PANIC();
		return false;
	}

	//　
	jpeg_error_mgr jerr;

	//
	jpeg_create_decompress( &cInfo );
	cInfo.err = jpeg_std_error( &jerr );
	jpeg_stdio_src( &cInfo, fp );

	//　
	Decompress( &cInfo );
	jpeg_destroy_decompress( &cInfo );
	fclose( fp );

	//　BGR　→　RGB
	for ( int i=0; i<(int)imageSize; i+=3 )
	{
		temp = imageData[i+0];
		imageData[i+0] = imageData[i+2];
		imageData[i+2] = temp;		
	}

	return true;
}

//---------------------------------------------------------------------------------------------
//　　ReadW
//　　Desc : Jpegファイルを読み込む
//---------------------------------------------------------------------------------------------
bool JPEGImage::ReadW( const wchar_t *filename )
{
	struct jpeg_decompress_struct cInfo;
	FILE *fp;
	GLubyte temp;

	fp = _wfopen( filename, L"rb" );
    if ( !fp )
	{
		wcout << L"Error : couldn't open \"" << filename << L"\"!\n";
		SKY_PANIC();
		return false;
	}

	//　
	jpeg_error_mgr jerr;

	//
	jpeg_create_decompress( &cInfo );
	cInfo.err = jpeg_std_error( &jerr );
	jpeg_stdio_src( &cInfo, fp );

	//　
	Decompress( &cInfo );
	jpeg_destroy_decompress( &cInfo );
	fclose( fp );

	//　BGR　→　RGB
	for ( int i=0; i<(int)imageSize; i+=3 )
	{
		temp = imageData[i+0];
		imageData[i+0] = imageData[i+2];
		imageData[i+2] = temp;		
	}

	return true;
}


//---------------------------------------------------------------------------------------------
//　　LoadA
//　　Desc : Jpegファイルを読み込み，テクスチャを生成
//----------------------------------------------------------------------------------------------
GLuint JPEGImage::LoadA( const char *filename )
{
	if ( !ReadA(filename) )
	{
		cout << "Error: Jpeg Load File False\n";
		SKY_PANIC();
		return 0;
	}
	glGenTextures( 1, &handle );
	glBindTexture( GL_TEXTURE_2D, handle );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	gluBuild2DMipmaps( GL_TEXTURE_2D, internalFormat, width, height, format, GL_UNSIGNED_BYTE, imageData );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	SafeDelete();
	return handle;
}

//---------------------------------------------------------------------------------------------
//　　LoadW
//　　Desc : Jpegファイルを読み込み，テクスチャを生成
//----------------------------------------------------------------------------------------------
GLuint JPEGImage::LoadW( const wchar_t *filename )
{
	if ( !ReadW(filename) )
	{
		wcout << L"Error: Jpeg Load File False\n";
		SKY_PANIC();
		return 0;
	}
	glGenTextures( 1, &handle );
	glBindTexture( GL_TEXTURE_2D, handle );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	gluBuild2DMipmaps( GL_TEXTURE_2D, internalFormat, width, height, format, GL_UNSIGNED_BYTE, imageData );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
	SafeDelete();
	return handle;
}

//----------------------------------------------------------------------------------------------
//　　Decompress
//　　Desc: Jpegファイルの圧縮データを解凍
//---------------------------------------------------------------------------------------------
void JPEGImage::Decompress( jpeg_decompress_struct *cInfo )
{
	//
	jpeg_read_header( cInfo, TRUE );
	jpeg_start_decompress( cInfo );

	//
	internalFormat = cInfo->num_components;
	format =  (cInfo->num_components == 1) ? GL_LUMINANCE : GL_RGB;
	width = cInfo->image_width;
	height = cInfo->image_height;
	imageSize = width * height * internalFormat;
	imageData = new unsigned char [imageSize];
	GLubyte ** rowPtr = new unsigned char * [height];

	//
	for ( unsigned int i=0; i<height; i++ )
	{
		rowPtr[i] = ( imageData + ((height -(i+1))*width*internalFormat));
		jpeg_read_scanlines( cInfo, &rowPtr[i], 1 );
	}	

	delete [] rowPtr;
	jpeg_finish_decompress( cInfo );
}
#endif

////////////////////////////////////////////////////////////////////////
//　　Texture class
////////////////////////////////////////////////////////////////////////

//--------------------------------------------------------------------------------------------------
//　　Texture
//　　Desc : コンストラクタ
//--------------------------------------------------------------------------------------------------
GLTextureLoader::GLTextureLoader(GLenum target)
{
	strcpy(m_nameA, "none");
	wcscpy(m_nameW, L"none");
	m_handle = 0;
	m_target = 0;
	m_Width = 0;
	m_Height = 0;

}

skyBool GLTextureLoader::IsLoaded()
{
	return ( m_Width == 0 && m_Height == 0 ) ? skyFalse : skyTrue;
}

//--------------------------------------------------------------------------------------------------
//　　Texture
//　　Desc : デストラクタ
//--------------------------------------------------------------------------------------------------
GLTextureLoader::~GLTextureLoader()
{
}

//-------------------------------------------------------------------------------------------------
//　　Delete
//　　Desc : テクスチャを削除
//-------------------------------------------------------------------------------------------------
void GLTextureLoader::Delete()
{
	if ( m_handle )
	{
		glDeleteTextures(1, &m_handle);
		m_handle = 0;
	}
}

//-------------------------------------------------------------------------------------------------
//　　Parameter
//　　Desc : パラメータの設定
//-------------------------------------------------------------------------------------------------
void GLTextureLoader::Parameter(GLenum pname, GLint param)
{
	glBindTexture(m_target, m_handle);
	glTexParameteri(m_target, pname, param);
	glBindTexture(m_target, 0);
}

//-------------------------------------------------------------------------------------------------
//　　Enviorement
//　　Desc : 環境の設定
//-------------------------------------------------------------------------------------------------
void GLTextureLoader::Enviorement(GLenum pname, GLint param)
{
	glBindTexture(m_target, m_handle);
	glTexEnvi(GL_TEXTURE_ENV, pname, param);
	glBindTexture(m_target, 0);
}

//-------------------------------------------------------------------------------------------------
//　　Enable
//　　Desc : テクスチャの有効化
//-------------------------------------------------------------------------------------------------
void GLTextureLoader::Enable()
{
	glEnable(m_target);
	glBindTexture(m_target, m_handle);
}

//-------------------------------------------------------------------------------------------------
//　　Disable
//　　Desc : テクスチャの無効化
//-------------------------------------------------------------------------------------------------
void GLTextureLoader::Disable()
{
	glDisable(m_target);
	glBindTexture(m_target, 0);
}

//-------------------------------------------------------------------------------------------------
//　　operator =
//-------------------------------------------------------------------------------------------------
GLTextureLoader &GLTextureLoader::operator =(const GLTextureLoader &texture)
{
	strcpy(m_nameA, texture.m_nameA);
	wcscpy(m_nameW, texture.m_nameW);
	m_handle = texture.m_handle;
	m_target = texture.m_target;
	return *this;
}

//-------------------------------------------------------------------------------------------------
//　　operator ==
//-------------------------------------------------------------------------------------------------
bool GLTextureLoader::operator== (const GLTextureLoader &texture)
{
	return (m_handle==texture.m_handle && m_target==texture.m_target);
}

//-------------------------------------------------------------------------------------------------
//　　operator !=
//-------------------------------------------------------------------------------------------------
bool GLTextureLoader::operator !=(const GLTextureLoader &texture)
{
	return ( m_handle != texture.m_handle || m_target != texture.m_target);
}

//-------------------------------------------------------------------------------------------------
//　　CreateTextureFromFileA
//　　Desc : ファイルからテクスチャを作成
//-------------------------------------------------------------------------------------------------
void CreateTextureFromFileA( const char *filename, GLTextureLoader& texture)
{
	//　Bitmap
	if ( strstr(filename, ".bmp") || strstr(filename, ".BMP") )
	{
		BMPImage tempBMP;
		texture.SetNameA(filename);
		texture.SetTextureTarget(GL_TEXTURE_2D);
		texture.SetHandle(tempBMP.LoadA(filename));
		texture.m_Width = tempBMP.GetWidth();
		texture.m_Height = tempBMP.GetHeight();
	}

	//　Targa
	else if ( strstr(filename, ".tga") || strstr(filename, ".TGA" ) )
	{
		TGAImage tempTGA;
		texture.SetNameA(filename);
		texture.SetTextureTarget(GL_TEXTURE_2D);
		texture.SetHandle(tempTGA.LoadA(filename));
		texture.m_Width = tempTGA.GetWidth();
		texture.m_Height = tempTGA.GetHeight();
	}

#if USE_GLEW
	//　Direct Draw Surface
	else if ( strstr(filename, ".dds") || strstr(filename, ".DDS") )
	{
		DDSImage tempDDS;
		texture.SetNameA(filename);
		texture.SetTextureTarget(GL_TEXTURE_2D);
		texture.SetHandle(tempDDS.LoadA(filename));
		texture.m_Width = tempDDS.GetWidth();
		texture.m_Height = tempDDS.GetHeight();
	}
#endif

#ifdef USE_PNG_JPEG_LOAD
	//　Portable Network Graphics
	else if ( strstr(filename, ".png") || strstr(filename, ".PNG") )
	{
		PNGImage tempPNG;
		texture.SetNameA(filename);
		texture.SetTextureTarget(GL_TEXTURE_2D);
		texture.SetHandle(tempPNG.LoadA(filename));
		texture.m_Width = tempPNG.GetWidth();
		texture.m_Height = tempPNG.GetHeight();
	}

	//　Jpeg
	else if ( strstr(filename, ".jpg") || strstr(filename, ".JPG") || strstr(filename, ".jpeg") || strstr(filename, ".JPEG") )
	{
		JPEGImage tempJPEG;
		texture.SetNameA(filename);
		texture.SetTextureTarget(GL_TEXTURE_2D);
		texture.SetHandle(tempJPEG.LoadA(filename));
		texture.m_Width = tempJPEG.GetWidth();
		texture.m_Height = tempJPEG.GetHeight();
	}
#endif

	//　Unknown
	else 
	{
		cout << "Error : Not Support File Format !!\n";
		cout << "File Name : " << filename << endl;
		texture.SetNameA("none");
		texture.SetTextureTarget(NULL);
		texture.SetHandle(0);
		SKY_PANIC();
	}

}

//-------------------------------------------------------------------------------------------------
//　　CreateTextureFromFileW
//　　Desc : ファイルからテクスチャを作成
//-------------------------------------------------------------------------------------------------
void CreateTextureFromFileW( const wchar_t *filename, GLTextureLoader& texture)
{
	//　Bitmap
	if ( wcsstr(filename, L".bmp") || wcsstr(filename, L".BMP") )
	{
		BMPImage tempBMP;
		texture.SetNameW(filename);
		texture.SetTextureTarget(GL_TEXTURE_2D);
		texture.SetHandle(tempBMP.LoadW(filename));
		texture.m_Width = tempBMP.GetWidth();
		texture.m_Height = tempBMP.GetHeight();
	}

	//　Targa
	else if ( wcsstr(filename, L".tga") || wcsstr(filename, L".TGA" ) )
	{
		TGAImage tempTGA;
		texture.SetNameW(filename);
		texture.SetTextureTarget(GL_TEXTURE_2D);
		texture.SetHandle(tempTGA.LoadW(filename));
		texture.m_Width = tempTGA.GetWidth();
		texture.m_Height = tempTGA.GetHeight();
	}

#if USE_GLEW
	//　Direct Draw Surface
	else if ( wcsstr(filename, L".dds") || wcsstr(filename, L".DDS") )
	{
		DDSImage tempDDS;
		texture.SetNameW(filename);
		texture.SetTextureTarget(GL_TEXTURE_2D);
		texture.SetHandle(tempDDS.LoadW(filename));
		texture.m_Width = tempDDS.GetWidth();
		texture.m_Height = tempDDS.GetHeight();
	}
#endif

#ifdef USE_PNG_JPEG_LOAD
	//　Portable Network Graphics
	else if ( wcsstr(filename, L".png") || wcsstr(filename, L".PNG") )
	{
		PNGImage tempPNG;
		texture.SetNameW(filename);
		texture.SetTextureTarget(GL_TEXTURE_2D);
		texture.SetHandle(tempPNG.LoadW(filename));
		texture.m_Width = tempPNG.GetWidth();
		texture.m_Height = tempPNG.GetHeight();
	}

	//　Jpeg
	else if ( wcsstr(filename, L".jpg") || wcsstr(filename, L".JPG") || wcsstr(filename, L".jpeg") || wcsstr(filename, L".JPEG") )
	{
		JPEGImage tempJPEG;
		texture.SetNameW(filename);
		texture.SetTextureTarget(GL_TEXTURE_2D);
		texture.SetHandle(tempJPEG.LoadW(filename));
		texture.m_Width = tempJPEG.GetWidth();
		texture.m_Height = tempJPEG.GetHeight();
	}
#endif

	//　Unknown
	else 
	{
		wcout << L"Error : Not Support File Format !!\n";
		wcout << L"File Name : " << filename << endl;
		texture.SetNameW(L"none");
		texture.SetTextureTarget(NULL);
		texture.SetHandle(0);
		SKY_PANIC();
	}

}

//-------------------------------------------------------------------------------------------------
//　　CreateTexture
//　　Desc : 空テクスチャを作成
//-------------------------------------------------------------------------------------------------
void CreateTexture(GLuint width, GLuint height, GLuint format, GLTextureLoader &texture)
{
	GLuint tempHandle=0;
	glGenTextures(1, &tempHandle);
	glBindTexture(texture.GetTextureTarget(), tempHandle);
	glTexImage2D(texture.GetTextureTarget(), 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, 0);
	texture.SetHandle(tempHandle);
	glBindTexture(texture.GetTextureTarget(), 0);
}

//--------------------------------------------------------------------------------------------------
//　　WriteFileBMP_A
//　　Desc : Bitmapファイルを作成
//--------------------------------------------------------------------------------------------------
bool WriteFileBMP_A(const char *filename, int width, int height, unsigned char *pixelData)
{
	FILE *fp;
	BitmapFileHeader header;
	BitmapInfoHeader info;
	int writeWidth = 0;
	unsigned char zero=0;

	if ( (fp = fopen(filename, "wb") )== NULL )
	{
        cout << "Error : It failed in the file open.\n";
		cout << "File Name : " << filename << endl;
		return false;
	}

	header.bfType =  0x4d42;
	header.bfSize = 0;
	header.bfReserved1 = 0;
	header.bfReserved2 = 0;
	header.bfOffBits = 54;

	info.biSize = 40;
	info.biWidth = width;
	info.biHeight = height;
	info.biPlanes = 1;
	info.biBitCount = 24;
	info.biCompression = 0;
	info.biSizeImage = 0;
	info.biXPelsPerMeter = 0;
	info.biYPelsPerMeter = 0;
	info.biClrUsed = 0;
	info.biClrImportant = 0;

	if ( (width*3)%4 == 0 )
		writeWidth = width*3;
	else
		writeWidth = width*3 + 4- (width*3)%4;

	header.bfSize = writeWidth*height + 14 + 40;

	fwrite(&(header.bfType), sizeof(unsigned short), 1, fp);
	fwrite(&(header.bfSize), sizeof(unsigned long), 1, fp);
	fwrite(&(header.bfReserved1), sizeof(unsigned short), 1, fp);
	fwrite(&(header.bfReserved2), sizeof(unsigned short), 1, fp);
	fwrite(&(header.bfOffBits), sizeof(unsigned long), 1, fp);

	fwrite(&(info.biSize), sizeof(unsigned long), 1, fp);
	fwrite(&(info.biWidth), sizeof(long), 1, fp);
	fwrite(&(info.biHeight), sizeof(long), 1, fp);
	fwrite(&(info.biPlanes), sizeof(unsigned short), 1, fp);
	fwrite(&(info.biBitCount), sizeof(unsigned short), 1, fp);
	fwrite(&(info.biCompression), sizeof(unsigned long), 1, fp);
	fwrite(&(info.biSizeImage), sizeof(unsigned long), 1, fp);
	fwrite(&(info.biXPelsPerMeter), sizeof(long), 1, fp);
	fwrite(&(info.biYPelsPerMeter), sizeof(long), 1, fp);
	fwrite(&(info.biClrUsed), sizeof(unsigned long), 1, fp);
	fwrite(&(info.biClrImportant), sizeof(unsigned long), 1, fp);

	for ( int y=0; y<height; y++ )
	{
		for ( int x=0; x<width; x++ )
		{
			fwrite((pixelData + x*3 + writeWidth*y + 2), sizeof(unsigned char), 1, fp);
			fwrite((pixelData + x*3 + writeWidth*y + 1), sizeof(unsigned char), 1, fp);
			fwrite((pixelData + x*3 + writeWidth*y + 0), sizeof(unsigned char), 1, fp);
		}
		if ( (width*3)%4 != 0 )
		{
			for ( int j=0; j<4 - (width*3)%4; j++ )
				fwrite(&zero, sizeof(unsigned char), 1, fp);
		}
	}

	fclose(fp);
	return true;
}

//--------------------------------------------------------------------------------------------------
//　　WriteFileBMP_W
//　　Desc : Bitmapファイルを作成
//--------------------------------------------------------------------------------------------------
bool WriteFileBMP_W(const wchar_t *filename, int width, int height, unsigned char *pixelData)
{
	FILE *fp;
	BitmapFileHeader header;
	BitmapInfoHeader info;
	int writeWidth = 0;
	unsigned char zero=0;

	if ( (fp = _wfopen(filename, L"wb") )== NULL )
	{
        wcout << _T( "It failed in the file open. \n" );
        wcout << _T( "File Name : " << filename << endl );
		SKY_PANIC();
		return false;
	}

	header.bfType =  0x4d42;
	header.bfSize = 0;
	header.bfReserved1 = 0;
	header.bfReserved2 = 0;
	header.bfOffBits = 54;

	info.biSize = 40;
	info.biWidth = width;
	info.biHeight = height;
	info.biPlanes = 1;
	info.biBitCount = 24;
	info.biCompression = 0;
	info.biSizeImage = 0;
	info.biXPelsPerMeter = 0;
	info.biYPelsPerMeter = 0;
	info.biClrUsed = 0;
	info.biClrImportant = 0;

	if ( (width*3)%4 == 0 )
		writeWidth = width*3;
	else
		writeWidth = width*3 + 4- (width*3)%4;

	header.bfSize = writeWidth*height + 14 + 40;

	fwrite(&(header.bfType), sizeof(unsigned short), 1, fp);
	fwrite(&(header.bfSize), sizeof(unsigned long), 1, fp);
	fwrite(&(header.bfReserved1), sizeof(unsigned short), 1, fp);
	fwrite(&(header.bfReserved2), sizeof(unsigned short), 1, fp);
	fwrite(&(header.bfOffBits), sizeof(unsigned long), 1, fp);

	fwrite(&(info.biSize), sizeof(unsigned long), 1, fp);
	fwrite(&(info.biWidth), sizeof(long), 1, fp);
	fwrite(&(info.biHeight), sizeof(long), 1, fp);
	fwrite(&(info.biPlanes), sizeof(unsigned short), 1, fp);
	fwrite(&(info.biBitCount), sizeof(unsigned short), 1, fp);
	fwrite(&(info.biCompression), sizeof(unsigned long), 1, fp);
	fwrite(&(info.biSizeImage), sizeof(unsigned long), 1, fp);
	fwrite(&(info.biXPelsPerMeter), sizeof(long), 1, fp);
	fwrite(&(info.biYPelsPerMeter), sizeof(long), 1, fp);
	fwrite(&(info.biClrUsed), sizeof(unsigned long), 1, fp);
	fwrite(&(info.biClrImportant), sizeof(unsigned long), 1, fp);

	for ( int y=0; y<height; y++ )
	{
		for ( int x=0; x<width; x++ )
		{
			fwrite((pixelData + x*3 + writeWidth*y + 2), sizeof(unsigned char), 1, fp);
			fwrite((pixelData + x*3 + writeWidth*y + 1), sizeof(unsigned char), 1, fp);
			fwrite((pixelData + x*3 + writeWidth*y + 0), sizeof(unsigned char), 1, fp);
		}
		if ( (width*3)%4 != 0 )
		{
			for ( int j=0; j<4 - (width*3)%4; j++ )
				fwrite(&zero, sizeof(unsigned char), 1, fp);
		}
	}

	fclose(fp);
	return true;
}

//--------------------------------------------------------------------------------------------------
//　　WriteFileTGA_A
//　　Desc : TGAファイルを生成
//--------------------------------------------------------------------------------------------------
bool WriteFileTGA_A(const char *filename, int width, int height, unsigned char* pixelData)
{
	FILE *fp;
	unsigned char tgaHeaders[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	unsigned char header[6];
	unsigned char bits = 0;
	int colorMode = 0;
	unsigned char temp = 0;
	
	if ( (fp = fopen(filename, "wb") ) == NULL )
	{
        cout << "Error : It failed in the file open.\n";
		cout << "File Name : "<< filename << endl;
		return false;
	}

	colorMode = 4;
	bits = 32;

	header[0] = width%256;
	header[1] = width/256;
	header[2] = height%256;
	header[3] = height/256;
	header[4] = bits;
	header[5] = 0;

	fwrite(tgaHeaders, sizeof(tgaHeaders), 1, fp);
	fwrite(header, sizeof(header), 1, fp);


	for ( int i=0; i<width*height*colorMode; i+=colorMode )
	{
		temp = pixelData[i];
		pixelData[i+0] = pixelData[i+2];
		pixelData[i+2] = temp;
	}

	fwrite(pixelData, width*height*colorMode, 1, fp);

	fclose(fp);
	return true;	
}


//--------------------------------------------------------------------------------------------------
//　　WriteFileTGA_W
//　　Desc : TGAファイルを生成
//--------------------------------------------------------------------------------------------------
bool WriteFileTGA_W(const wchar_t *filename, int width, int height, unsigned char* pixelData)
{
	FILE *fp;
	unsigned char tgaHeaders[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	unsigned char header[6];
	unsigned char bits = 0;
	int colorMode = 0;
	unsigned char temp = 0;
	
	if ( (fp = _wfopen(filename, L"wb") ) == NULL )
	{
        wcout << L"Error : It failed in the file open.\n";
		wcout << L"File Name : "<< filename << endl;
		SKY_PANIC();
		return false;
	}

	colorMode = 4;
	bits = 32;

	header[0] = width%256;
	header[1] = width/256;
	header[2] = height%256;
	header[3] = height/256;
	header[4] = bits;
	header[5] = 0;

	fwrite(tgaHeaders, sizeof(tgaHeaders), 1, fp);
	fwrite(header, sizeof(header), 1, fp);


	for ( int i=0; i<width*height*colorMode; i+=colorMode )
	{
		temp = pixelData[i];
		pixelData[i+0] = pixelData[i+2];
		pixelData[i+2] = temp;
	}

	fwrite(pixelData, width*height*colorMode, 1, fp);

	fclose(fp);
	return true;	
}

#ifdef USE_PNG_JPEG_LOAD
//-------------------------------------------------------------------------------------------------
//　　WriteJPEG_A
//　　Desc : JPEGファイルを生成
//-------------------------------------------------------------------------------------------------
bool WriteFileJPEG_A(char *filename, int width, int height, int quality, unsigned char* pixelData)
{
	FILE *fp;
	unsigned char *buff=NULL;
	unsigned char zero=0;
	unsigned char temp;
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPROW row_pointer[1];
	int row_stride=0;

	if ( (fp = fopen(filename, "wb") ) == NULL )
	{
        cout << "Error : It failed in the file open.\n";
		cout << "File Name : " << filename << endl;
		SKY_PANIC();
		return false;
	}

	//　RGBA　→　BGRA
	for ( int i=0; i<width*height*4; i+=4 )
	{
		temp = pixelData[i+0];
		pixelData[i+0] = pixelData[i+2];
		pixelData[i+2] = temp;
	}

	buff = new unsigned char [width*4];

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	jpeg_stdio_dest(&cinfo, fp);
	
	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
	
	row_stride = width*4;

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE);
	jpeg_start_compress(&cinfo, TRUE);

	while ( cinfo.next_scanline < cinfo.image_height )
	{
		register int x;
		unsigned char *p, *pbuff;

		p = buff;
		pbuff = pixelData + (height - cinfo.next_scanline - 1) * row_stride;
		for ( x = 0; x <width; x++ )
		{
			switch ( cinfo.input_components )
			{
			case 1:
				*(p++) = *(pbuff);
				pbuff += 4;
				break;

			default:
				*(p++) = *(pbuff++);
				*(p++) = *(pbuff++);
				*(p++) = *(pbuff++);
				pbuff++;
				break;
			}
		}
		row_pointer[0] = &buff[0];
		(void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	fclose(fp);

	return true;
}


//-------------------------------------------------------------------------------------------------
//　　WriteJPEG_W
//　　Desc : JPEGファイルを生成
//-------------------------------------------------------------------------------------------------
bool WriteFileJPEG_W(wchar_t *filename, int width, int height, int quality, unsigned char* pixelData)
{
	FILE *fp;
	unsigned char *buff=NULL;
	unsigned char temp;
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	JSAMPROW row_pointer[1];
	int row_stride=0;

	if ( (fp = _wfopen(filename, L"wb") ) == NULL )
	{
        wcout << L"Error : It failed in the file open.\n";
		wcout << L"File Name : " << filename << endl;
		SKY_PANIC();
		return false;
	}

	//　RGBA　→　BGRA
	for ( int i=0; i<width*height*4; i+=4 )
	{
		temp = pixelData[i+0];
		pixelData[i+0] = pixelData[i+2];
		pixelData[i+2] = temp;
	}

	buff = new unsigned char [width*4];

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	jpeg_stdio_dest(&cinfo, fp);
	
	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
	
	row_stride = width*4;

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE);
	jpeg_start_compress(&cinfo, TRUE);

	while ( cinfo.next_scanline < cinfo.image_height )
	{
		register int x;
		unsigned char *p, *pbuff;

		p = buff;
		pbuff = pixelData + (height - cinfo.next_scanline - 1) * row_stride;
		for ( x = 0; x <width; x++ )
		{
			switch ( cinfo.input_components )
			{
			case 1:
				*(p++) = *(pbuff);
				pbuff += 4;
				break;

			default:
				*(p++) = *(pbuff++);
				*(p++) = *(pbuff++);
				*(p++) = *(pbuff++);
				pbuff++;
				break;
			}
		}
		row_pointer[0] = &buff[0];
		(void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	fclose(fp);

	return true;
}
#endif

//--------------------------------------------------------------------------------------------------
//　　CreateImageFileA
//　　Desc : 画像ファイルを作成
//--------------------------------------------------------------------------------------------------
bool CreateImageFileA(char *filename, int width, int height, _IMAGE_FILEFORMAT format, unsigned char *pixel)
{
	bool result = false;
	switch ( format )
	{
	case _BMP:
		result =  WriteFileBMP_A(filename, width, height, pixel);
		break;

	case _TGA:
		result = WriteFileTGA_A(filename, width, height, pixel);
		break;
		
#ifdef USE_PNG_JPEG_LOAD
	case _JPEG:
		result = WriteFileJPEG_A(filename, width, height, 100, pixel);
		break;
		
	case _PNG:
		cout << "Error : PNGには対応していません\n";
		result = false;
		SKY_PANIC();
		break;
#endif

#if USE_GLEW
	case _DDS:
		cout << "Error : DDSには対応していません\n";
		result = false;
		SKY_PANIC();
		break;
#endif

	default :
		cout << "Error : フォーマットが不正です\n";
		result = false;
		SKY_PANIC();
		break;
	}
	return result;
}


//--------------------------------------------------------------------------------------------------
//　　CreateImageFileW
//　　Desc : 画像ファイルを作成
//--------------------------------------------------------------------------------------------------
bool CreateImageFileW(wchar_t *filename, int width, int height, _IMAGE_FILEFORMAT format, unsigned char *pixel)
{
	bool result = false;
	switch ( format )
	{
	case _BMP:
		result =  WriteFileBMP_W(filename, width, height, pixel);
		break;

	case _TGA:
		result = WriteFileTGA_W(filename, width, height, pixel);
		break;
		
#ifdef USE_PNG_JPEG_LOAD
	case _JPEG:
		result = WriteFileJPEG_W(filename, width, height, 100, pixel);
		break;
		
	case _PNG:
        wcout << L"Error : Png is not supported.\n";
		result = false;
		SKY_PANIC();
		break;
#endif

#if USE_GLEW
	case _DDS:
        wcout << L"Error : DDS is not supported. \n";
		result = false;
		SKY_PANIC();
		break;
#endif

	default :
        wcout << L"Error : This Format is invalid. \n";
		result = false;
		SKY_PANIC();
		break;
	}
	return result;
}

//--------------------------------------------------------------------------------------------------
//　　SaveSurfaceToFileA
//　　Desc : 画面をファイルに保存
//--------------------------------------------------------------------------------------------------
bool SaveSurfaceToFileA(char *filename, Rect rect, _IMAGE_FILEFORMAT format)
{
	GLubyte *texel;
	texel = new GLubyte [(rect.right-rect.left)*(rect.bottom-rect.top)*4];
	glReadPixels(rect.left, rect.top,  rect.right-rect.left, rect.bottom-rect.top, GL_RGBA, GL_UNSIGNED_BYTE, texel);
	bool result = CreateImageFileA(filename, rect.right-rect.left, rect.bottom-rect.top, format, texel);
	if ( texel )
	{
		delete [] texel;
		texel = NULL;
	}
	return result;
}

//--------------------------------------------------------------------------------------------------
//　　SaveSurfaceToFileW
//　　Desc : 画面をファイルに保存
//--------------------------------------------------------------------------------------------------
bool SaveSurfaceToFileW(wchar_t *filename, Rect rect, _IMAGE_FILEFORMAT format)
{
	GLubyte *texel;
	texel = new GLubyte [(rect.right-rect.left)*(rect.bottom-rect.top)*4];
	glReadPixels(rect.left, rect.top,  rect.right-rect.left, rect.bottom-rect.top, GL_RGBA, GL_UNSIGNED_BYTE, texel);
	bool result = CreateImageFileW(filename, rect.right-rect.left, rect.bottom-rect.top, format, texel);
	if ( texel )
	{
		delete [] texel;
		texel = NULL;
	}
	return result;
}

} } }

#endif
