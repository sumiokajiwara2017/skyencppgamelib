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
	//ファイル存在チェック
	if ( CPathUtil_::IsFileOn( pPath ) == skyFalse )
	{
		SKY_PANIC_MSG( _T( "The file doesn't exist. " ) ); //ファイルが存在しません。
		return stream::IStreamPtr();
	}

	u32 size = 0;
	u8 *pBuf;
	skyBool result = CFileUtil::LoadTextFileFromDisc( pPath , &pBuf , &size );

	if ( result == skyFalse )
	{
		SKY_PANIC_MSG( _T( "The file reading failed. " ) ); //ファイル読み込みが失敗しました。
		return stream::IStreamPtr();
	}

	//ファイルオブジェクト生成(消せて、書き込み可能、拡張不可）
	stream::IStreamPtr filePtr;
	filePtr.SetPtr( NEW__( CFileStream , CFileStream( pPath , pBuf , size , skyTrue , skyFalse , skyFalse ) ) );

	return filePtr;
}

FileLoadAsyncRs CFileUtil::LoadFileAsync( const hash::CHashStringPtr &pPath )
{
	//ファイルタイプによってロード処理を呼び分ける
	FunctionCallBackAsyncPtr funcPtr( FunctionCallBackAsync_r< stream::IStreamPtr >::Create( &CFileUtil::LoadFile , pPath ) );

	//Functorを他のスレッドに実行依頼
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
	//ファイルタイプによってロード処理を呼び分ける
	FunctionCallBackAsyncPtr funcPtr( FunctionCallBackAsync_v::Create( &CFileUtil::SaveFile , spFile , isResume ) );

	//Functorを他のスレッドに実行依頼
	CWorkerThreadManager_->RequestWork( funcPtr );

	return funcPtr;
}

void CFileUtil::FileSaveAsyncCancel( FileLoadAsyncRs &retObject )
{
	FunctionCallBackAsyncPtr funcPtr = retObject;
	CWorkerThreadManager_->CancelWork( funcPtr );
}

//-------------------------------------< 実装 >---------------------------------------------//

skyBool CFileUtil::GetSize						( IFileInfoPtr &pFileInfo )
{
//-------ファイルサイズを取得
	FileInfoWin32 *pFileInfoWin32 = ( FileInfoWin32 * )pFileInfo.GetPtr();

	u32 saveSeek = ftell( pFileInfoWin32->pFp );				//現在位置を保存
	fseek( pFileInfoWin32->pFp, 0, SEEK_END );
	pFileInfoWin32->fileSize = ftell( pFileInfoWin32->pFp );	//ファイルサイズの計測
	fseek( pFileInfoWin32->pFp, saveSeek, SEEK_SET );			//現在位置の復元
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
		SKY_PANIC_MSG( _T( "It started opening the file in an unknown open mode." ) ); //未知のオープンモードでファイルをオープンしようとしました。
		break;
	}

	return skyNull;
}

//更新時間を取得するためだけのファイル読み込み
#define FuncLoadFile(s,a) CreateFile((LPCTSTR)(s),(GENERIC_READ|GENERIC_WRITE),(FILE_SHARE_READ|FILE_SHARE_WRITE),NULL,OPEN_EXISTING,a,NULL)

u32	CFileUtil::GetFileUpdateTime( const skyString *pPath )
{
	u32 time = 0;

	FILETIME ftDate;
	HANDLE hFile;

	if ( (hFile = FuncLoadFile( pPath,FILE_ATTRIBUTE_NORMAL))!=INVALID_HANDLE_VALUE )
	{
		GetFileTime( hFile, NULL, NULL, &ftDate );  //←更新時間取得
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
//-------情報エリアを作成
	FileInfoWin32 *pFileInfoWin32 = NEW__( FileInfoWin32 , FileInfoWin32() );

//-------ファイルを開く
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	if ( ( _tfopen_s( &pFileInfoWin32->pFp , pPath , ConvOpenMode( eMode ) ) ) != 0 )
#else
	if ( ( pFileInfoWin32->pFp = _tfopen( pPath , ConvOpenMode( eMode ) ) ) == 0 )
#endif
	{
        SKY_PANIC_MSG( _T( "The file doesn't open it. =[ %s ]" ) , pPath );
		return skyFalse;
	}

//-------ファイルサイズを取得

	u32 saveSeek = ftell( pFileInfoWin32->pFp );				//現在位置を保存
	fseek( pFileInfoWin32->pFp, 0, SEEK_END );
	pFileInfoWin32->fileSize = ftell( pFileInfoWin32->pFp );	//ファイルサイズの計測
	fseek( pFileInfoWin32->pFp, saveSeek, SEEK_SET );			//現在位置の復元

	pFileInfo.SetPtr( pFileInfoWin32 );

	GetSize( pFileInfo );

	return skyTrue;
}

u32 CFileUtil::Read( IFileInfoPtr &pFileInfo , u8 *pBuf , u32 bufSize , u32 readSize , u32 pos )
{
	SKY_ASSERT_MSG( bufSize >= readSize , _T( "The size of the buffer doesn't suffice. " ) ); //バッファサイズを超えてファイルを読み込もうとしました。

	FileInfoWin32 *pFileInfoWin32 = ( FileInfoWin32 * )pFileInfo.GetPtr();

//----posへシーク

	fseek( pFileInfoWin32->pFp, pos, SEEK_SET );

//----読み込む

	u32 readSizeBuf;
	if ( ( readSizeBuf = fread( pBuf , 1 , readSize , pFileInfoWin32->pFp) ) == 0 )
	{
		SKY_PANIC_MSG( _T( "It failed in reading the file. " ) ); //ファイルの読み込みに失敗したか、０バイトのファイルを読み込もうとしたか、ファイルサイズと同じシーク位置から読み込もうとしました。
		return skyFalse;
	}

	return readSizeBuf;
}

u32 CFileUtil::Write( IFileInfoPtr &pFileInfo , u8* pBuf , u32 bufSize , u32 writeSize , u32 pos )
{
	SKY_ASSERT_MSG( bufSize >= writeSize , _T( "It tried to write the file exceeding the size of the buffer. " ) ); //バッファサイズを超えてファイルを書き込もうとしました。

	FileInfoWin32 *pFileInfoWin32 = ( FileInfoWin32 * )pFileInfo.GetPtr();

//----posへシーク

	fseek( pFileInfoWin32->pFp, pos, SEEK_SET );

//----書き込む

	u32 writeSizeBuf;
	if ( ( writeSizeBuf = fwrite( pBuf , 1 , writeSize , pFileInfoWin32->pFp ) ) == 0 )
	{
		SKY_PANIC_MSG( _T( "It failed in writing the file. " ) ); //ファイルの書き込みに失敗しました。
		return skyFalse;
	}

//----サイズを取得（広がったかもしれないので）

	GetSize( pFileInfo );

	return writeSizeBuf;
}

skyBool CFileUtil::Close( IFileInfoPtr &pFileInfo )
{
	FileInfoWin32 *pFileInfoWin32 = ( FileInfoWin32 * )pFileInfo.GetPtr();

	fclose( pFileInfoWin32->pFp );
//	_fcloseall(); //なんとこれを使うと全てのファイルが閉じるようだ

	pFileInfo.Delete();

	return skyTrue;
}

skyBool CFileUtil::LoadTextFileFromDisc( const skyString *pPath , u8 **ppBuf , u32 *pReadSize )
{

	//ファイル存在チェック
	if ( CPathUtil_::IsFileOn( pPath ) == skyFalse )
	{
        SKY_PANIC_MSG( _T( "The file was not found.filePath=[ %s ]" ) , pPath );
		return skyFalse;
	}

//-------ファイルを開く

	FILE*	pFp;

#ifdef SW_SKYLIB_MEMORY_TEXT_UNICODE
	//ワイド文字が不適切な文字列置換を行われないようにUNICODEの場合はバイナリモードで開く
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

//-------ファイルサイズを取得

	u32 saveSeek = ftell( pFp );			//現在位置を保存
	u32 fileSize = 0;
	fseek( pFp, 0, SEEK_END );
	fileSize = ftell( pFp );				//ファイルサイズの計測
	fseek( pFp, saveSeek, SEEK_SET );		//現在位置の復元

	if ( fileSize == 0 )
	{
        SKY_PANIC_MSG( _T( "It is an empty file. filePath=[ %s ]" ) );
		return false;
	}

//-------ファイルバッファの確保

	*ppBuf = ( u8 * )SkyMalloc( sizeof( u8 ) * fileSize + sizeof( skyString ) );

//-------ファイルを読む

	if ( ( *pReadSize = fread( *ppBuf , 1 , fileSize , pFp) ) == skyNull )
	{
        SKY_PANIC_MSG( _T( "The file was not able to be read.filePath=[ %s ]" ) , pPath );
		return skyFalse;
	}

//-------末尾に文字終端を付ける
	CMemoryUtil_::Copy( *ppBuf + *pReadSize , &EOT , sizeof ( skyString ) ); //終端子もつける

//-------ファイルを閉じる

	fclose( pFp );
//	_fcloseall(); //なんとこれを使うと全てのファイルが閉じるようだ

	return true;
}

skyBool CFileUtil::SaveLineTextFileForDisc( const skyString *pPath , u8 *pBuf , skyBool isResume  )
{

//-------ファイルを開く

	FILE*	pFp;

#ifdef SW_SKYLIB_MEMORY_TEXT_UNICODE
//ワイド文字が不適切な文字列置換を行われないようにUNICODEの場合はバイナリモードで開く
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

//-------書きこむ

	_fputts( ( skyString * )pBuf , pFp );

	return true;
}

skyBool CFileUtil::LoadFileFromDisc( const skyString *pPath , u8 **ppBuf , u32 *pReadSize )
{
	//ファイル存在チェック
	if ( CPathUtil_::IsFileOn( pPath ) == skyFalse )
	{
        SKY_PANIC_MSG( _T( "The file is not found. filePath=[ %s ]" ) , pPath );
		return skyFalse;
	}

//-------ファイルを開く

	FILE*	pFp;

#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	if ( ( _tfopen_s( &pFp , pPath , OpenMode::rpb ) ) != 0 ) //バイナリモードで開く
#else
	if ( ( pFp = _tfopen( pPath , OpenMode::rpb ) ) == 0 ) //バイナリモードで開く
#endif
	{
        SKY_PANIC_MSG( _T( "The file doesn't open it. filePath=[ %s ]" ) , pPath );
		return skyFalse;
	}

//-------ファイルサイズを取得

	u32 saveSeek = ftell( pFp );			//現在位置を保存
	u32 fileSize = 0;
	fseek( pFp, 0, SEEK_END );
	fileSize = ftell( pFp );				//ファイルサイズの計測
	fseek( pFp, saveSeek, SEEK_SET );		//現在位置の復元

//-------ファイルバッファの確保

	*ppBuf = ( u8 * )SkyMalloc( sizeof( u8 ) * fileSize );

//-------ファイルを読む

	if ( ( *pReadSize = fread( *ppBuf , 1 , fileSize , pFp) ) == skyNull )
	{
        SKY_PANIC_MSG( _T( "The file cannot be read. filePath=[ %s ]" ) , pPath );
		return skyFalse;
	}

//-------ファイルを閉じる

	fclose( pFp );
//	_fcloseall(); //なんとこれを使うと全てのファイルが閉じるようだ

	return true;
}
skyBool CFileUtil::SaveBinFileForDisc( const skyString *pPath , const u8 *pBuf , u32 bufSize , skyBool isResume  ) 
{
//-------ファイルを開く

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
	if ( ( _tfopen_s( &pFp , pPath , mark ) ) != 0 ) //バイナリモードで開く
#else
	if ( ( pFp = _tfopen( pPath , mark ) ) == 0 ) //バイナリモードで開く
#endif
	{
        SKY_PANIC_MSG( _T( "The file is not found.filePath=[ %s ]" ) , pPath );
		return false;
	}

//-------書きこむ

	u32 writeSize;
	if ( ( writeSize = fwrite( pBuf , 1 , bufSize , pFp ) ) == 0 )
	{
		SKY_PANIC_MSG( _T( "It failed in writing the file. " ) ); //ファイルの書き込みに失敗しました。
		return skyFalse;
	}

//-------ファイルを閉じる

	fclose( pFp );
//	_fcloseall(); //なんとこれを使うと全てのファイルが閉じるようだ

	return true;
}

#endif

} } }
