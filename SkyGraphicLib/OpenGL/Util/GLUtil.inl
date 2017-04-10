#ifdef SKYLIB_RENDER_OPENGL4

#include <iostream>
#include <fstream>
#include <vector>

namespace sky { namespace lib { namespace graphic {

namespace GLUtil
{

/// <summary>
/// �r�b�g�}�b�v�̃w�b�_
/// </summary>
struct BITMAPFILEHEADER_
{
    //char bfType1;
    //char bfType2;
    unsigned long  bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned long  bfOffBits;
};

/// <summary>
/// �r�b�g�}�b�v���w�b�_�[
/// </summary>
struct BITMAPINFOHEADER_
{
    unsigned long  biSize;
    long           biWidth;
    long           biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned long  biCompression;
    unsigned long  biSizeImage;
    long           biXPixPerMeter;
    long           biYPixPerMeter;
    unsigned long  biClrUsed;
    unsigned long  biClrImporant;
};	

inline void ScreenShot( const skyMString* filename , u16 width , u16 height , u16 bpp )
{
	// �s�N�Z���f�[�^�S�̂̃T�C�Y
	const int datasize = height * ( ( ( ( width * bpp / 8 ) + 3 ) >> 2 ) << 2 );
	// �t�@�C���T�C�Y
	const int filesize = 2 + sizeof( BITMAPFILEHEADER_ ) + sizeof( BITMAPINFOHEADER_ ) + datasize;

	// �r�b�g�}�b�v�̃w�b�_
	BITMAPFILEHEADER_ bmfh = { filesize , 0 , 0 , 54 , };
	BITMAPINFOHEADER_ bmih = { 40 , width , height , 1 , bpp , 0 , 0 , 0 , 0 , 0 , 0 , };

	// �f�[�^�̃t�H�[�}�b�g
	int format;
	if ( bpp == 24 )
    {
		format = GL_RGB;
	}
	else if ( bpp == 32 )
    {
		format = GL_RGBA;
	}
	else
	{
        SKY_PANIC_MSG( _T("Screen shot output failed.\n") );
	}

	// �f�[�^�����炤
	std::vector< GLubyte > buf( datasize );
	//glPixelStorei(GL_PACK_ALIGNMENT, 1);
	//glReadBuffer(GL_FRONT);
	glReadPixels( 0 , 0 , width , height , format , GL_UNSIGNED_BYTE , &buf[ 0 ] );

	// RGB �� BGR
	for ( int i = 0; i < datasize ; i += bpp / 8 )
	{
		std::swap( buf[ i ] , buf[ i + 2 ] );
	}

	// �o��
	std::ofstream fs( filename , std::ios::out | std::ios::trunc | std::ios::binary );
	if (!fs)
	{
        SKY_PANIC_MSG( _T("Screen shot output failed.\n") );
	}
	
	fs.write( "BM" , 2 );
	fs.write( reinterpret_cast<const char*>( &bmfh ) , sizeof(BITMAPFILEHEADER_ ) );
	fs.write( reinterpret_cast<const char*>( &bmih ) , sizeof(BITMAPINFOHEADER_ ) );
	fs.write( reinterpret_cast<const char*>( &buf[0] ) , datasize);
	fs.close();	
}

}

} } }

#endif
