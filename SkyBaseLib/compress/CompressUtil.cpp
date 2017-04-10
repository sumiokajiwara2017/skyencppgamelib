#include "StdAfx.h"
#include "SkyBaseLib/Compress.h"
#include "SkyBaseLib/Debug.h"

namespace sky { namespace lib { namespace comp {

//-------------------------------< ���� >----------------------------------------------

skyBool CCompressUtil::CompressFile( const u8 *pSrcData , u32 srcSize , const hash::CHashStringPtr &pDestPath , const skyMString *pOpenMode )
{
	skyMString *pPath = CStringUtil_::CreateMString( pDestPath->GetString() );

	// �o�͐戳�k�t�@�C�����I�[�v������.
	gzFile outFile = gzopen( pPath , pOpenMode );

	if ( outFile == NULL )
	{
		SKY_PANIC_MSG( _T( "The compression file did not open it at the output destination." ) ); //�o�͐戳�k�t�@�C�����J���܂���ł����B
		return skyFalse;
	}

	// �f�[�^�����k���ďo�̓t�@�C���ɏ�������
	u32 writeLen = gzwrite( outFile, ( void * )pSrcData , srcSize );

	if ( writeLen != srcSize )
	{
		SKY_PANIC_MSG( _T( "Writing failed in the data compressed file. " ) ); //���k�t�@�C���֏������݂����s���܂����B
		return skyFalse; 
	}

	// �o�͐戳�k�t�@�C�������
	s32 ret = gzclose(outFile);

	if ( ret != Z_OK )
	{
		if ( ret == Z_MEM_ERROR )
		{
			SKY_PANIC_MSG( _T( "The data compressed file was not able to be shut." ) ); //���k�t�@�C�������܂���ł����B�������s���� malloc() �ł��Ȃ�����
		}
		else if ( ret == Z_MEM_ERROR )
		{
			SKY_PANIC_MSG( _T( "The data compressed file was not able to be shut." ) ); //���k�t�@�C�������܂���ł����B�o�̓o�b�t�@�̃T�C�Y�s��
		}
		return skyFalse;
	}

	CStringUtil_::DeleteMString( pPath );

	return skyTrue;
}
skyBool CCompressUtil::CompressData( const u8 *pSrcData , u32 srcSize , u8* pDestData , u32 &destSize )
{
	//���k����
	s32 ret = compress( pDestData , &destSize , pSrcData , srcSize );

	if ( ret != Z_OK )
	{
		if ( ret == Z_MEM_ERROR )
		{
			SKY_PANIC_MSG( _T( "The data compressed file was not able to be shut." ) ); //���k�Ɏ��s���܂����B�������s���� malloc() �ł��Ȃ�����
		}
		else if ( ret == Z_MEM_ERROR )
		{
			SKY_PANIC_MSG( _T( "The data compressed file was not able to be shut." ) ); //���k�Ɏ��s���܂����B�o�̓o�b�t�@�̃T�C�Y�s��
		}
		return skyFalse;
	}

	return skyTrue;
}
skyBool CCompressUtil::UnCompressFile( const u8 *pDestData , u32 destBufSize , u32 &destReadSize , const hash::CHashStringPtr &pSrcPath , const skyMString *pOpenMode )
{
	skyMString *pPath = CStringUtil_::CreateMString( pSrcPath->GetString() );

	// �o�͐戳�k�t�@�C�����I�[�v������.
	gzFile inFile = gzopen( pPath , pOpenMode );

	if ( inFile == NULL )
	{
		SKY_PANIC_MSG( _T( "The compression file did not open it at the output destination." ) );
		return skyFalse;
	}

	s32 ret;
	while( (ret = gzread( inFile, ( void *)pDestData , destBufSize ) ) != 0 && ret != -1)
	{
		//�ǂݍ��݃T�C�Y���v�Z
		destReadSize += ret;
	}

	if(ret == -1)
	{
		// gzerror�ŃG���[���b�Z�[�W���擾����
		const skyMString *gzerr = gzerror( inFile, ( int * )&ret );
		skyString *errMsg;

		skyString *msg = _T( "���k�t�@�C���̉𓀓Ǎ��Ɏ��s���܂����B errorMsg=%s" );
		SKY_UNUSED_ARG( msg );
		//�G���[���b�Z�[�W�Ƃꂽ�H
		if ( ret == Z_ERRNO )
		{
			//�W���G���[���o��
			#define ERR_MSG_SIZE	256
			skyString errMsgBuf[ ERR_MSG_SIZE ];
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
			_tcserror_s( errMsgBuf , ERR_MSG_SIZE );
			SKY_PANIC_MSG( msg , errMsgBuf );
#else
			SKY_PANIC_MSG( msg );
#endif
		}
		else
		{
			errMsg = CStringUtil_::CreateString( gzerr );
			SKY_PANIC_MSG( msg , errMsg );
			CStringUtil_::DeleteString( errMsg );
		}
		return skyFalse;
	}

	if ( ( ret = gzclose( inFile ) ) != Z_OK )
	{
		if ( ret == Z_MEM_ERROR )
		{
			SKY_PANIC_MSG( _T( "The data compressed file was not able to be shut." ) ); //���k�t�@�C�������܂���ł����B�������s���� malloc() �ł��Ȃ�����
		}
		else if ( ret == Z_MEM_ERROR )
		{
			SKY_PANIC_MSG( _T( "The data compressed file was not able to be shut." ) ); //���k�t�@�C�������܂���ł����B�o�̓o�b�t�@�̃T�C�Y�s��
		}
		return skyFalse;
	}

	return skyTrue;
}
skyBool CCompressUtil::UnCompressData( const u8 *pSrcData , u32 srcSize , u8* pDestData , u32 &destSize )
{
	s32 ret = uncompress( pDestData , &destSize , pSrcData , srcSize );

	if ( ret != Z_OK )
	{
		if ( ret == Z_MEM_ERROR )
		{
			SKY_PANIC_MSG( _T( "It failed in the decompression." ) ); //�𓀂Ɏ��s���܂����B�������s���� malloc() �ł��Ȃ�����
		}
		else if ( ret == Z_MEM_ERROR )
		{
			SKY_PANIC_MSG( _T( "It failed in the decompression." ) ); //�𓀂Ɏ��s���܂����B�o�̓o�b�t�@�̃T�C�Y�s��
		}
		return skyFalse;
	}

	return skyTrue;
}

} } }