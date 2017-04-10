#include "StdAfx.h"
#include "SkyBaseLib/Directory.h"

#include <direct.h>

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

namespace sky { namespace lib { namespace directory {

//-------------------------------------< 実装 >---------------------------------------------//

#ifdef SKYLIB_PLATFORM_WIN32

skyBool CPathUtil::CreateDir( const skyString *srcPath )
{
	return _tmkdir( srcPath ) == 0;
}
skyBool CPathUtil::DeleteDir( const skyString *srcPath )
{
	return _trmdir( srcPath ) == 0;
}
skyBool CPathUtil::GetCurrentDir( skyString *destPath , u32 destLen )
{
	return _tgetcwd( destPath , destLen ) == 0;
}
s32 CPathUtil::GetCurrentDrive()
{
	return _getdrive();
}

skyBool CPathUtil::ChangeCurrentDrive( s32 drive )
{
	return _chdrive( drive ) == 0;
}

void CPathUtil::CutFileExt( const skyString *srcPath , skyString *destBuf , u32 destBufSize )
{
	// パスからファイル名と拡張子部分のみを抽出
	skyString    drive[DRIVE_LEN_MAX+1], dir[DIR_LEN_MAX+1];
	skyString    fname[FILE_LEN_MAX+1 ];
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	_tsplitpath_s( srcPath, drive , DRIVE_LEN_MAX+1 , dir, DIR_LEN_MAX+1 , fname , FILE_LEN_MAX+1 , NULL , NULL );
#else
	_tsplitpath( srcPath, drive , dir , fname , NULL );
#endif
 
	// ファイル名と拡張子のみを合成
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	_tmakepath_s( destBuf , destBufSize , drive, dir, fname, NULL );
#else
	_tmakepath( destBuf , drive, dir, fname, NULL );
#endif
}

void CPathUtil::GetDrive( const skyString *srcPath , skyString *destBuf , u32 destBufSize )
{
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	_tsplitpath_s( srcPath, destBuf , destBufSize , NULL, NULL , NULL , NULL , NULL , NULL );
#else
	_tsplitpath( srcPath, destBuf , NULL , NULL , NULL );
#endif
}

void CPathUtil::GetFileExt( const skyString *srcPath , skyString *destBuf , u32 destBufSize )
{
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	_tsplitpath_s( srcPath, NULL , NULL , NULL, NULL , NULL , NULL , destBuf , destBufSize );
#else
	_tsplitpath( srcPath , NULL , NULL , NULL , destBuf );
#endif
}

void CPathUtil::ReplaceFileExt( const skyString *srcPath, skyString *destBuf , u32 destBufSize , const skyString *pExt )
{
	// 入力パスを４つの要素に分ける
	skyString    drive[ DRIVE_LEN_MAX + 1 ], dir[ DIR_LEN_MAX + 1 ];
	skyString    fname[ FILE_LEN_MAX + 1 ] , ext[ EXT_LEN_MAX + 1 ];
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	_tsplitpath_s( srcPath, drive , DRIVE_LEN_MAX+1 , dir, DIR_LEN_MAX+1 , fname , FILE_LEN_MAX+1 , ext , EXT_LEN_MAX+1 );
#else
	_tsplitpath( srcPath, drive , dir , fname , ext );
#endif
 
	//拡張子だけあたらしいものをつける
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	_tmakepath_s( destBuf , destBufSize , drive, dir, fname, pExt );
#else
	_tmakepath( destBuf , drive, dir, fname, pExt );
#endif
}

skyBool CPathUtil::IsExt( const skyString *srcPath , const skyString *srcExt )
{
	// 入力パスを４つの要素に分ける
	skyString    drive[ DRIVE_LEN_MAX + 1 ], dir[ DIR_LEN_MAX + 1 ];
	skyString    fname[ FILE_LEN_MAX + 1 ] , ext[ EXT_LEN_MAX + 1 ];
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	_tsplitpath_s( srcPath, drive , DRIVE_LEN_MAX+1 , dir, DIR_LEN_MAX+1 , fname , FILE_LEN_MAX+1 , ext , EXT_LEN_MAX+1 );
#else
	_tsplitpath( srcPath, drive , dir , fname  , ext );
#endif

	return CStringUtil_::Comp( ext , srcExt );
}

void CPathUtil::ReplaceFileName( const skyString *srcPath, skyString *destBuf , u32 destBufSize , const skyString *pName )
{
	// 入力パスを４つの要素に分ける
	skyString    drive[ DRIVE_LEN_MAX + 1 ], dir[ DIR_LEN_MAX + 1 ];
	skyString    fname[ FILE_LEN_MAX + 1 ] , ext[ EXT_LEN_MAX + 1 ];
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	_tsplitpath_s( srcPath, drive , DRIVE_LEN_MAX+1 , dir, DIR_LEN_MAX+1 , fname , FILE_LEN_MAX+1 , ext , EXT_LEN_MAX+1 );
#else
	_tsplitpath( srcPath, drive , dir , fname , ext );
#endif
 
	// ファイル名だけあたらしいものにかえる
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	_tmakepath_s( destBuf , destBufSize , drive, dir, pName, ext );
#else
	_tmakepath( destBuf , drive, dir, pName, ext );
#endif
}

void CPathUtil::AddTailFileName( const skyString *srcPath, skyString *destBuf , u32 destBufSize , const skyString *pNameSignature )
{
	// 入力パスを４つの要素に分ける
	skyString    drive[ DRIVE_LEN_MAX + 1 ], dir[ DIR_LEN_MAX + 1 ];
	skyString    fname[ FILE_LEN_MAX + 1 ] , ext[ EXT_LEN_MAX + 1 ];
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	_tsplitpath_s( srcPath, drive , DRIVE_LEN_MAX+1 , dir, DIR_LEN_MAX+1 , fname , FILE_LEN_MAX+1 , ext , EXT_LEN_MAX+1 );
#else
	_tsplitpath( srcPath, drive , dir , fname , ext );
#endif
 
	//ファイル名にサインを結合
	CStringUtil_::Cat( fname , pNameSignature );

	// ファイル名だけあたらしいものにかえる
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	_tmakepath_s( destBuf , destBufSize , drive, dir, fname, ext );
#else
	_tmakepath( destBuf , drive, dir, fname, ext );
#endif
}

void CPathUtil::GetFileName( const skyString *srcPath , skyString *destBuf , u32 destBufSize )
{
	// パスからファイル名と拡張子部分のみを抽出
	skyString    fname[FILE_LEN_MAX+1 ] , ext[EXT_LEN_MAX+1];
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	_tsplitpath_s( srcPath, NULL , NULL , NULL , NULL , fname , FILE_LEN_MAX+1 , ext , EXT_LEN_MAX+1 );
#else
	_tsplitpath( srcPath , NULL , NULL , fname , ext );
#endif
 
	// ファイル名と拡張子のみを合成
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	_tmakepath_s( destBuf , destBufSize , NULL, NULL, fname, ext );
#else
	_tmakepath( destBuf , NULL, NULL, fname, ext );
#endif
}

void CPathUtil::GetFileNameNoExt( const skyString *srcPath , skyString *destBuf , u32 destBufSize )
{
	// パスからファイル名と拡張子部分のみを抽出
	skyString    fname[FILE_LEN_MAX+1 ];
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	_tsplitpath_s( srcPath, NULL , NULL , NULL , NULL , fname , FILE_LEN_MAX+1 , NULL , NULL );
#else
	_tsplitpath( srcPath , NULL , NULL , fname , NULL );
#endif
 
	// ファイル名と拡張子のみを合成
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	_tmakepath_s( destBuf , destBufSize , NULL, NULL, fname, NULL );
#else
	_tmakepath( destBuf , NULL, NULL, fname, NULL );
#endif
}

void CPathUtil::GetDirPath( const skyString *srcPath, skyString *destBuf , u32 destBufSize )
{
	// パスからドライブ名とディレクトリ名のみを抽出
	skyString    drive[DRIVE_LEN_MAX+1], dir[DIR_LEN_MAX+1];
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	_tsplitpath_s( srcPath, drive , DRIVE_LEN_MAX+1 , dir, DIR_LEN_MAX+1 , NULL , NULL , NULL , NULL );
#else
	_tsplitpath( srcPath, drive , dir , NULL , NULL );
#endif
 
	// ドライブとディレクトリのみを合成
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	_tmakepath_s( destBuf, destBufSize , drive, dir, NULL, NULL );
#else
	_tmakepath( destBuf , drive, dir, NULL, NULL );
#endif
}

void CPathUtil::GetFullPath ( const skyString *srcPath, skyString *destBuf , u32 destBufSize )
{
	_tfullpath( destBuf , srcPath , destBufSize );
}

skyBool CPathUtil::ChangeCurrentDirectory( const skyString *srcPath )
{
	return _tchdir( srcPath ) == 0;
}

skyBool CPathUtil::IsFileOn( const skyString *pPath )
{
	FILE *fp;
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	if ( ( _tfopen_s( &fp , pPath , _T( "r" ) ) ) == 0 )
#else
	if ( ( fp = _tfopen( pPath , _T( "r" ) ) ) != 0 )
#endif
	{
		// ファイルが存在する
		fclose( fp );
		return skyTrue;
	}
	else
	{
		return skyFalse;
	}
}

skyBool CPathUtil::IsDirectoryOn( const skyString *pPath )
{
	return IsFileOn( pPath );
}

skyBool	CPathUtil::IsSlashEnd( const skyString *pPath )
{
	u32 len = CStringUtil_::Len( pPath );

	return ( _T('/') == pPath[ len - 1 ] || _T('\\') == pPath[ len - 1 ] );
}

skyBool CPathUtil::IsWildCardMatch ( const skyString *wildCard , const skyString *filePath )
{
    //この関数はWindowsAPI　必須Lib：ShLwApi.Lib　必須Dll：ShLwApi.dll
	return PathMatchSpec( wildCard , filePath );
}
void CPathUtil::GetPathFileNameAddPath( const skyString *srcPath , const skyString *srcDirPath , skyString *destBuf , u32 destBufSize )
{
	skyString    fname[FILE_LEN_MAX+1 ];
	GetFileName( srcPath , fname , FILE_LEN_MAX+1 );

	// ドライブとディレクトリのみを合成
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	_tmakepath_s( destBuf, destBufSize , NULL, srcDirPath, fname, NULL );
#else
	_tmakepath( destBuf , NULL , srcDirPath , fname , NULL );
#endif
}

void CPathUtil::GetEnv( memSize &destlen , const skyString *name , skyString *destBuf , u32 destBufSize )
{
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	_tgetenv_s( ( size_t * )&destlen , destBuf , destBufSize , name );
#else
	skyString *value = _tgetenv( name );
	CStringUtil_::Copy( destBuf , destBufSize , value );
#endif
}

void CPathUtil::PutEnv( const skyString *name , const skyString *val )
{
#ifdef SW_SKYLIB_USE_SECURITY_UP_FUNCTION
	_tputenv_s( name , val );
#else
	skyWString param[ PATH_LEN_MAX ];
	CStringUtil_::Sprintf( param , PATH_LEN_MAX , _T( "%s=%s" ) , name , val );
	_tputenv( param );
#endif
}

skyBool CPathUtil::ReplaceEnv( const skyString *src , skyString *destBuf , u32 destBufSize )
{
	//WindowsAPI
	return ExpandEnvironmentStrings( src , destBuf , destBufSize ) != 0;
}

#endif

} } }
