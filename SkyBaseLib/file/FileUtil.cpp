#include "StdAfx.h"
#include "SkyBaseLib/File.h"

#include "SkyBaseLib/Directory.h"
#include "SkyBaseLib/Thread.h"

SkyImplementRTTI( sky::lib::file::FileInfoWin32 , sky::lib::file::IFileInfo );

namespace sky { namespace lib { namespace file { 


void CFileUtil::Initialize()
{
}

void CFileUtil::Dispose()
{
}

stream::IStreamPtr CFileUtil::LoadFile( const hash::CHashStringPtr pPath )
{
	//�t�@�C�����݃`�F�b�N
	if ( CPathUtil_::IsFileOn( pPath ) == skyFalse )
	{
		SKY_PANIC_MSG( _T( "The file doesn't exist. " ) ); //�t�@�C�������݂��܂���B
		return stream::IStreamPtr();
	}

	u32 size = 0;
	u8 *pBuf;
	skyBool result = CFileUtil::LoadTextFileFromDisc( pPath , &pBuf , &size );

	if ( result == skyFalse )
	{
		SKY_PANIC_MSG( _T( "The file reading failed. " ) ); //�t�@�C���ǂݍ��݂����s���܂����B
		return stream::IStreamPtr();
	}

	//�t�@�C���I�u�W�F�N�g����(�����āA�������݉\�A�g���s�j
	stream::IStreamPtr filePtr;
	filePtr.SetPtr( NEW__( CFileStream , CFileStream( pPath , pBuf , size , skyTrue , skyFalse , skyFalse ) ) );

	return filePtr;
}

FileLoadAsyncRs CFileUtil::LoadFileAsync( const hash::CHashStringPtr &pPath )
{
	//�t�@�C���^�C�v�ɂ���ă��[�h�������Ăѕ�����
	FunctionCallBackAsyncPtr funcPtr( FunctionCallBackAsync_r< stream::IStreamPtr >::Create( &CFileUtil::LoadFile , pPath ) );

	//Functor�𑼂̃X���b�h�Ɏ��s�˗�
	CWorkerThreadManager_->RequestWork( funcPtr );

	return funcPtr;
}

void CFileUtil::LoadFileAsyncCancel( FileLoadAsyncRs &retObject )
{
	FunctionCallBackAsyncPtr funcPtr = retObject;
	CWorkerThreadManager_->CancelWork( funcPtr );
}

void CFileUtil::SaveFile( stream::IStreamPtr spFile , skyBool isResume  )
{
	skyBool result = skyFalse;

	result = CFileUtil::SaveBinFileForDisc( spFile->vGetName() , spFile->vGetData() , spFile->vGetSize() , isResume );
}

FileSaveAsyncRs CFileUtil::SaveFileAsync( stream::IStreamPtr &spFile , skyBool isResume )
{
	//�t�@�C���^�C�v�ɂ���ă��[�h�������Ăѕ�����
	FunctionCallBackAsyncPtr funcPtr( FunctionCallBackAsync_v::Create( &CFileUtil::SaveFile , spFile , isResume ) );

	//Functor�𑼂̃X���b�h�Ɏ��s�˗�
	CWorkerThreadManager_->RequestWork( funcPtr );

	return funcPtr;
}

void CFileUtil::FileSaveAsyncCancel( FileLoadAsyncRs &retObject )
{
	FunctionCallBackAsyncPtr funcPtr = retObject;
	CWorkerThreadManager_->CancelWork( funcPtr );
}

//-------------------------------------< ���� >---------------------------------------------//

skyBool CFileUtil::GetSize						( IFileInfoPtr &pFileInfo )
{
//-------�t�@�C���T�C�Y���擾
	FileInfoWin32 *pFileInfoWin32 = ( FileInfoWin32 * )pFileInfo.GetPtr();

	u32 saveSeek = ftell( pFileInfoWin32->pFp );				//���݈ʒu��ۑ�
	fseek( pFileInfoWin32->pFp, 0, SEEK_END );
	pFileInfoWin32->fileSize = ftell( pFileInfoWin32->pFp );	//�t�@�C���T�C�Y�̌v��
	fseek( pFileInfoWin32->pFp, saveSeek, SEEK_SET );			//���݈ʒu�̕���
	return skyTrue;
}


#ifdef SKYLIB_PLATFORM_WIN32

inline const skyString  *ConvOpenMode( eOpenMode eMode )
{
	switch ( eMode )
	{
	case eOpenMode_Text_R_NoCreate:
		return OpenMode::rp;
		break;
	case eOpenMode_Text_RW_Create:
		return OpenMode::wp;
		break;
	case eOpenMode_Binary_R_NoCreate:
		return OpenMode::rpb;
		break;
	case eOpenMode_Binary_RW_Create:
		return OpenMode::wpb;
		break;
	default:
		SKY_PANIC_MSG( _T( "It started opening the file in an unknown open mode." ) ); //���m�̃I�[�v�����[�h�Ńt�@�C�����I�[�v�����悤�Ƃ��܂����B
		break;
	}

	return skyNull;
}

//�X�V���Ԃ��擾���邽�߂����̃t�@�C���ǂݍ���
#define FuncLoadFile(s,a) CreateFile((LPCTSTR)(s),(GENERIC_READ|GENERIC_WRITE),(FILE_SHARE_READ|FILE_SHARE_WRITE),NULL,OPEN_EXISTING,a,NULL)

u32	CFileUtil::GetFileUpdateTime( const skyString *pPath )
{
	u32 time = 0;

	FILETIME ftDate;
	HANDLE hFile;

	if ( (hFile = FuncLoadFile( pPath,FILE_ATTRIBUTE_NORMAL))!=INVALID_HANDLE_VALUE )
	{
		GetFileTime( hFile, NULL, NULL, &ftDate );  //���X�V���Ԏ擾
		CloseHandle( hFile );
		time = ftDate.dwLowDateTime;
	}
	else
	{
        SKY_PANIC_MSG( _T( "The file was not found. filePath=%s" ) , pPath );
	}

	return time;
}
skyBool CFileUtil::IstFileUpdateTimeChange( u32 time , const skyString *pPath )
{
	u32 gettime = GetFileUpdateTime( pPath );

	return ( time != gettime );
}

skyBool	CFileUtil::Open( const skyString *pPath , IFileInfoPtr &pFileInfo , eOpenMode eMode )
{
//-------���G���A���쐬
	FileInfoWin32 *pFileInfoWin32 = NEW__( FileInfoWin32 , FileInfoWin32() );

//-------�t�@�C�����J��
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	if ( ( _tfopen_s( &pFileInfoWin32->pFp , pPath , ConvOpenMode( eMode ) ) ) != 0 )
#else
	if ( ( pFileInfoWin32->pFp = _tfopen( pPath , ConvOpenMode( eMode ) ) ) == 0 )
#endif
	{
        SKY_PANIC_MSG( _T( "The file doesn't open it. =[ %s ]" ) , pPath );
		return skyFalse;
	}

//-------�t�@�C���T�C�Y���擾

	u32 saveSeek = ftell( pFileInfoWin32->pFp );				//���݈ʒu��ۑ�
	fseek( pFileInfoWin32->pFp, 0, SEEK_END );
	pFileInfoWin32->fileSize = ftell( pFileInfoWin32->pFp );	//�t�@�C���T�C�Y�̌v��
	fseek( pFileInfoWin32->pFp, saveSeek, SEEK_SET );			//���݈ʒu�̕���

	pFileInfo.SetPtr( pFileInfoWin32 );

	GetSize( pFileInfo );

	return skyTrue;
}

u32 CFileUtil::Read( IFileInfoPtr &pFileInfo , u8 *pBuf , u32 bufSize , u32 readSize , u32 pos )
{
	SKY_ASSERT_MSG( bufSize >= readSize , _T( "The size of the buffer doesn't suffice. " ) ); //�o�b�t�@�T�C�Y�𒴂��ăt�@�C����ǂݍ������Ƃ��܂����B

	FileInfoWin32 *pFileInfoWin32 = ( FileInfoWin32 * )pFileInfo.GetPtr();

//----pos�փV�[�N

	fseek( pFileInfoWin32->pFp, pos, SEEK_SET );

//----�ǂݍ���

	u32 readSizeBuf;
	if ( ( readSizeBuf = fread( pBuf , 1 , readSize , pFileInfoWin32->pFp) ) == 0 )
	{
		SKY_PANIC_MSG( _T( "It failed in reading the file. " ) ); //�t�@�C���̓ǂݍ��݂Ɏ��s�������A�O�o�C�g�̃t�@�C����ǂݍ������Ƃ������A�t�@�C���T�C�Y�Ɠ����V�[�N�ʒu����ǂݍ������Ƃ��܂����B
		return skyFalse;
	}

	return readSizeBuf;
}

u32 CFileUtil::Write( IFileInfoPtr &pFileInfo , u8* pBuf , u32 bufSize , u32 writeSize , u32 pos )
{
	SKY_ASSERT_MSG( bufSize >= writeSize , _T( "It tried to write the file exceeding the size of the buffer. " ) ); //�o�b�t�@�T�C�Y�𒴂��ăt�@�C���������������Ƃ��܂����B

	FileInfoWin32 *pFileInfoWin32 = ( FileInfoWin32 * )pFileInfo.GetPtr();

//----pos�փV�[�N

	fseek( pFileInfoWin32->pFp, pos, SEEK_SET );

//----��������

	u32 writeSizeBuf;
	if ( ( writeSizeBuf = fwrite( pBuf , 1 , writeSize , pFileInfoWin32->pFp ) ) == 0 )
	{
		SKY_PANIC_MSG( _T( "It failed in writing the file. " ) ); //�t�@�C���̏������݂Ɏ��s���܂����B
		return skyFalse;
	}

//----�T�C�Y���擾�i�L��������������Ȃ��̂Łj

	GetSize( pFileInfo );

	return writeSizeBuf;
}

skyBool CFileUtil::Close( IFileInfoPtr &pFileInfo )
{
	FileInfoWin32 *pFileInfoWin32 = ( FileInfoWin32 * )pFileInfo.GetPtr();

	fclose( pFileInfoWin32->pFp );
//	_fcloseall(); //�Ȃ�Ƃ�����g���ƑS�Ẵt�@�C��������悤��

	pFileInfo.Delete();

	return skyTrue;
}

skyBool CFileUtil::LoadTextFileFromDisc( const skyString *pPath , u8 **ppBuf , u32 *pReadSize )
{

	//�t�@�C�����݃`�F�b�N
	if ( CPathUtil_::IsFileOn( pPath ) == skyFalse )
	{
        SKY_PANIC_MSG( _T( "The file was not found.filePath=[ %s ]" ) , pPath );
		return skyFalse;
	}

//-------�t�@�C�����J��

	FILE*	pFp;

#ifdef SW_SKYLIB_MEMORY_TEXT_UNICODE
	//���C�h�������s�K�؂ȕ�����u�����s���Ȃ��悤��UNICODE�̏ꍇ�̓o�C�i�����[�h�ŊJ��
	const skyString *pOpenMode = OpenMode::rpb;
#else
	const skyString *pOpenMode = OpenMode::rp;
#endif

#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	if ( ( _tfopen_s( &pFp , pPath , pOpenMode ) ) != 0 )
#else
	if ( ( pFp = _tfopen( pPath , pOpenMode ) ) == 0 )
#endif
	{
        SKY_PANIC_MSG( _T( "The file doesn't open it. filePath=[ %s ]" ) , pPath );
		return skyFalse;
	}

//-------�t�@�C���T�C�Y���擾

	u32 saveSeek = ftell( pFp );			//���݈ʒu��ۑ�
	u32 fileSize = 0;
	fseek( pFp, 0, SEEK_END );
	fileSize = ftell( pFp );				//�t�@�C���T�C�Y�̌v��
	fseek( pFp, saveSeek, SEEK_SET );		//���݈ʒu�̕���

	if ( fileSize == 0 )
	{
        SKY_PANIC_MSG( _T( "It is an empty file. filePath=[ %s ]" ) );
		return false;
	}

//-------�t�@�C���o�b�t�@�̊m��

	*ppBuf = ( u8 * )SkyMalloc( sizeof( u8 ) * fileSize + sizeof( skyString ) );

//-------�t�@�C����ǂ�

	if ( ( *pReadSize = fread( *ppBuf , 1 , fileSize , pFp) ) == skyNull )
	{
        SKY_PANIC_MSG( _T( "The file was not able to be read.filePath=[ %s ]" ) , pPath );
		return skyFalse;
	}

//-------�����ɕ����I�[��t����
	CMemoryUtil_::Copy( *ppBuf + *pReadSize , &EOT , sizeof ( skyString ) ); //�I�[�q������

//-------�t�@�C�������

	fclose( pFp );
//	_fcloseall(); //�Ȃ�Ƃ�����g���ƑS�Ẵt�@�C��������悤��

	return true;
}

skyBool CFileUtil::SaveLineTextFileForDisc( const skyString *pPath , u8 *pBuf , skyBool isResume  )
{

//-------�t�@�C�����J��

	FILE*	pFp;

#ifdef SW_SKYLIB_MEMORY_TEXT_UNICODE
//���C�h�������s�K�؂ȕ�����u�����s���Ȃ��悤��UNICODE�̏ꍇ�̓o�C�i�����[�h�ŊJ��
	const skyString *mark;
	if ( isResume == skyTrue )
	{
		mark = OpenMode::apb;
	}
	else
	{
		mark = OpenMode::wpb;
	}
#else
	const skyString *mark;
	if ( isResume == skyTrue )
	{
		mark = fileadptrwin32::wa;
	}
	else
	{
		mark = fileadptrwin32::w;
	}
#endif
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	if ( ( _tfopen_s( &pFp , pPath , mark ) ) != 0 )
#else
	if ( ( pFp = _tfopen( pPath , mark ) ) == 0 )
#endif
	{
        SKY_PANIC_MSG( _T( "The file doesn't open it. filePath=[ %s ]" ) , pPath );
		return skyFalse;
	}

//-------��������

	_fputts( ( skyString * )pBuf , pFp );

	return true;
}

skyBool CFileUtil::LoadFileFromDisc( const skyString *pPath , u8 **ppBuf , u32 *pReadSize )
{
	//�t�@�C�����݃`�F�b�N
	if ( CPathUtil_::IsFileOn( pPath ) == skyFalse )
	{
        SKY_PANIC_MSG( _T( "The file is not found. filePath=[ %s ]" ) , pPath );
		return skyFalse;
	}

//-------�t�@�C�����J��

	FILE*	pFp;

#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	if ( ( _tfopen_s( &pFp , pPath , OpenMode::rpb ) ) != 0 ) //�o�C�i�����[�h�ŊJ��
#else
	if ( ( pFp = _tfopen( pPath , OpenMode::rpb ) ) == 0 ) //�o�C�i�����[�h�ŊJ��
#endif
	{
        SKY_PANIC_MSG( _T( "The file doesn't open it. filePath=[ %s ]" ) , pPath );
		return skyFalse;
	}

//-------�t�@�C���T�C�Y���擾

	u32 saveSeek = ftell( pFp );			//���݈ʒu��ۑ�
	u32 fileSize = 0;
	fseek( pFp, 0, SEEK_END );
	fileSize = ftell( pFp );				//�t�@�C���T�C�Y�̌v��
	fseek( pFp, saveSeek, SEEK_SET );		//���݈ʒu�̕���

//-------�t�@�C���o�b�t�@�̊m��

	*ppBuf = ( u8 * )SkyMalloc( sizeof( u8 ) * fileSize );

//-------�t�@�C����ǂ�

	if ( ( *pReadSize = fread( *ppBuf , 1 , fileSize , pFp) ) == skyNull )
	{
        SKY_PANIC_MSG( _T( "The file cannot be read. filePath=[ %s ]" ) , pPath );
		return skyFalse;
	}

//-------�t�@�C�������

	fclose( pFp );
//	_fcloseall(); //�Ȃ�Ƃ�����g���ƑS�Ẵt�@�C��������悤��

	return true;
}
skyBool CFileUtil::SaveBinFileForDisc( const skyString *pPath , const u8 *pBuf , u32 bufSize , skyBool isResume  ) 
{
//-------�t�@�C�����J��

	FILE*	pFp;

	const skyString *mark;
	if ( isResume == skyTrue )
	{
		mark = OpenMode::apb;
	}
	else
	{
		mark = OpenMode::wpb;
	}

#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	if ( ( _tfopen_s( &pFp , pPath , mark ) ) != 0 ) //�o�C�i�����[�h�ŊJ��
#else
	if ( ( pFp = _tfopen( pPath , mark ) ) == 0 ) //�o�C�i�����[�h�ŊJ��
#endif
	{
        SKY_PANIC_MSG( _T( "The file is not found.filePath=[ %s ]" ) , pPath );
		return false;
	}

//-------��������

	u32 writeSize;
	if ( ( writeSize = fwrite( pBuf , 1 , bufSize , pFp ) ) == 0 )
	{
		SKY_PANIC_MSG( _T( "It failed in writing the file. " ) ); //�t�@�C���̏������݂Ɏ��s���܂����B
		return skyFalse;
	}

//-------�t�@�C�������

	fclose( pFp );
//	_fcloseall(); //�Ȃ�Ƃ�����g���ƑS�Ẵt�@�C��������悤��

	return true;
}

#endif

} } }
