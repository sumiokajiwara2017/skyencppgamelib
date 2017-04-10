#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_COMPRESS

#include "SkyBaseLib/Compress.h"

#define COMPRESS_SRC_FILE_PATH		_T( "data/test/compress/CompressSrcFile.txt" )
#define COMPRESS_FILE_PATH			_T( "data/test/compress/CompressFile.gz" )
#define UNCOMPRESS_FILE_PATH		_T( "data/test/compress/UnCompressFile.txt" )

TEST( compress , 1 ) 
{
	SKY_BASE_LIB_INIT();

//----------------�f�[�^���t�@�C���Ɉ��k���ĕۑ�
	
	//�t�@�C�����J��
	u8 *bbuf;
	u32 readSize;
	if ( CFileUtil_::LoadFileFromDisc( COMPRESS_SRC_FILE_PATH, &bbuf , &readSize ) == skyFalse )
	{
		SKY_PANIC_MSG( _T( "can't open file \n" ) );
		return;
	}

	//���k
	CCompressUtil_::CompressFile( bbuf , readSize , CHS( COMPRESS_FILE_PATH ) );

	//�ǂݍ��݃f�[�^�j��
	DEL bbuf;

//----------------�t�@�C������𓀂��Ȃ���f�[�^�����[�h

	//��
#define UNCOMPRESS_DATA_BUF_SIZE 256
	u8 uncompressDataBuf[ UNCOMPRESS_DATA_BUF_SIZE ];
	u32 uncompressDataSize = 0;
	CCompressUtil_::UnCompressFile( &uncompressDataBuf[ 0 ] , UNCOMPRESS_DATA_BUF_SIZE , uncompressDataSize , CHS( COMPRESS_FILE_PATH ) );

//----------------�f�[�^�����k���ăf�[�^�ɓ����

	//���k���f�[�^
	u8 compressSrcData[ UNCOMPRESS_DATA_BUF_SIZE ] = "abcdefghijklmnbbbb";

	//���k�f�[�^�i�[�o�b�t�@
	u8 compressDataBuffer[ UNCOMPRESS_DATA_BUF_SIZE ];
	u32 compressSize = UNCOMPRESS_DATA_BUF_SIZE; //�o�b�t�@�̃T�C�Y���w�肵�Ă����̂��R�c�B�����I����͎��ۂ̈��k���̃T�C�Y�������ɓ����Ă���B

	//���k
	CCompressUtil_::CompressData( compressSrcData , UNCOMPRESS_DATA_BUF_SIZE , compressDataBuffer , compressSize );

//----------------�f�[�^���𓀂��ăf�[�^�ɓ����

	//�𓀃f�[�^�i�[�o�b�t�@
	u8 uncompressDataBuffer[ UNCOMPRESS_DATA_BUF_SIZE ];
	u32 uncompressSize = UNCOMPRESS_DATA_BUF_SIZE; //�o�b�t�@�̃T�C�Y���w�肵�Ă����̂��R�c�B�����I����͎��ۂ̈��k���̃T�C�Y�������ɓ����Ă���B

	//��
	CCompressUtil_::UnCompressData( compressDataBuffer , compressSize , uncompressDataBuffer , uncompressSize );


	SKY_BASE_LIB_TERM();
}

#endif