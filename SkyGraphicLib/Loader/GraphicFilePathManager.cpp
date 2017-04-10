#include "StdAfx.h"
#include "SkyGraphicLib/Loader.h"

namespace sky { namespace lib { namespace graphic {

static hash::CHashStringPtr s_ColladaRootPath;
static hash::CHashStringPtr s_FbxRootPath;
static hash::CHashStringPtr s_XRootPath;
static hash::CHashStringPtr s_ObjRootPath;
static hash::CHashStringPtr s_MtlRootPath;
static hash::CHashStringPtr s_TextureRootPath;

void CGraphicFilePathManager::Initialize()
{
	s_ColladaRootPath = hash::CHashStringPtr();
	s_FbxRootPath = hash::CHashStringPtr();
	s_XRootPath = hash::CHashStringPtr();
	s_ObjRootPath = hash::CHashStringPtr();
	s_MtlRootPath = hash::CHashStringPtr();
	s_TextureRootPath = hash::CHashStringPtr();

	//デフォルトパスの指定
	SetColladaFilePathRoot( CHS( COLLADA_FILE_DIR_ROOT ) );
	SetFbxFilePathRoot( CHS( FBX_FILE_DIR_ROOT ) );
	SetXFilePathRoot( CHS( X_FILE_DIR_ROOT ) );
	SetObjFilePathRoot( CHS( OBJ_FILE_DIR_ROOT ) );
	SetMtlFilePathRoot( CHS( MTL_FILE_DIR_ROOT ) );
	SetTextureFilePathRoot( CHS( TEX_FILE_DIR_ROOT ) );
}

void CGraphicFilePathManager::Dispose()
{
	s_ColladaRootPath.Delete();
	s_FbxRootPath.Delete();
	s_XRootPath.Delete();
	s_ObjRootPath.Delete();
	s_MtlRootPath.Delete();
	s_TextureRootPath.Delete();
}

void CGraphicFilePathManager::SetColladaFilePathRoot( const hash::CHashStringPtr &path )
{
	s_ColladaRootPath = path;
}
const hash::CHashStringPtr &CGraphicFilePathManager::GetColladaFilePathRoot()
{
	return s_ColladaRootPath;
}
skyWString* CGraphicFilePathManager::ConcatColladaFilePathRoot( skyWString* destBuf , u32 bufSize , const skyWString *pPath )
{
	if ( s_ColladaRootPath.IsEmpty() == skyFalse && s_ColladaRootPath->GetSize() > 0 )
	{
		CStringUtil_::Copy( destBuf , bufSize , s_ColladaRootPath->GetString() );
		CStringUtil_::Cat( destBuf , pPath );
	}
	else
	{
		CStringUtil_::Copy( destBuf , bufSize , pPath );
	}
	return destBuf;
}
skyMString* CGraphicFilePathManager::ConcatColladaFilePathRoot( skyMString* destBuf , u32 bufSize , const skyMString *pPath )
{
	//wcharをcharに変換
	skyMString *cRootPath = CStringUtil_::CreateMString( s_ColladaRootPath->GetString() );

	if ( s_ColladaRootPath.IsEmpty() == skyFalse && s_ColladaRootPath->GetSize() > 0 )
	{
		CStringUtil_::Copy( destBuf , bufSize , cRootPath );
		CStringUtil_::Cat( destBuf , bufSize , pPath );
	}
	else
	{
		CStringUtil_::Copy( destBuf , bufSize , pPath );
	}

	//charのパスを消す
	CStringUtil_::DeleteMString( cRootPath );

	return destBuf;
}

void CGraphicFilePathManager::ClearColladaFilePathRoot()
{
	s_ColladaRootPath = hash::CHashStringPtr();
}

void CGraphicFilePathManager::SetFbxFilePathRoot( const hash::CHashStringPtr &path )
{
	s_FbxRootPath = path;
}
const hash::CHashStringPtr &CGraphicFilePathManager::GetFbxFilePathRoot()
{
	return s_FbxRootPath;
}
skyWString* CGraphicFilePathManager::ConcatFbxFilePathRoot( skyWString* destBuf , u32 bufSize , const skyWString *pPath )
{
	if ( s_FbxRootPath.IsEmpty() == skyFalse && s_FbxRootPath->GetSize() > 0 )
	{
		CStringUtil_::Copy( destBuf , bufSize , s_FbxRootPath->GetString() );
		CStringUtil_::Cat( destBuf , pPath );
	}
	else
	{
		CStringUtil_::Copy( destBuf , bufSize , pPath );
	}
	return destBuf;
}
skyMString* CGraphicFilePathManager::ConcatFbxFilePathRoot( skyMString* destBuf , u32 bufSize , const skyMString *pPath )
{
	//wcharをcharに変換
	skyMString *cRootPath = CStringUtil_::CreateMString( s_FbxRootPath->GetString() );

	if ( s_FbxRootPath.IsEmpty() == skyFalse && s_FbxRootPath->GetSize() > 0 )
	{
		CStringUtil_::Copy( destBuf , bufSize , cRootPath );
		CStringUtil_::Cat( destBuf , bufSize , pPath );
	}
	else
	{
		CStringUtil_::Copy( destBuf , bufSize , pPath );
	}

	//charのパスを消す
	CStringUtil_::DeleteMString( cRootPath );

	return destBuf;
}

void CGraphicFilePathManager::ClearFbxFilePathRoot()
{
	s_FbxRootPath = hash::CHashStringPtr();
}

void CGraphicFilePathManager::SetXFilePathRoot( const hash::CHashStringPtr &path )
{
	s_XRootPath = path;
}
const hash::CHashStringPtr &CGraphicFilePathManager::GetXFilePathRoot()
{
	return s_XRootPath;
}
skyWString* CGraphicFilePathManager::ConcatXFilePathRoot( skyWString* destBuf , u32 bufSize , const skyWString *pPath )
{
	if ( s_XRootPath.IsEmpty() == skyFalse && s_XRootPath->GetSize() > 0 )
	{
		CStringUtil_::Copy( destBuf , bufSize , s_XRootPath->GetString() );
		CStringUtil_::Cat( destBuf , pPath );
	}
	else
	{
		CStringUtil_::Copy( destBuf , bufSize , pPath );
	}
	return destBuf;
}
skyMString* CGraphicFilePathManager::ConcatXFilePathRoot( skyMString* destBuf , u32 bufSize , const skyMString *pPath )
{
	//wcharをcharに変換
	skyMString *cRootPath = CStringUtil_::CreateMString( s_XRootPath->GetString() );

	if ( s_XRootPath.IsEmpty() == skyFalse && s_XRootPath->GetSize() > 0 )
	{
		CStringUtil_::Copy( destBuf , bufSize , cRootPath );
		CStringUtil_::Cat( destBuf , bufSize , pPath );
	}
	else
	{
		CStringUtil_::Copy( destBuf , bufSize , pPath );
	}

	//charのパスを消す
	CStringUtil_::DeleteMString( cRootPath );

	return destBuf;
}

void CGraphicFilePathManager::ClearXFilePathRoot()
{
	s_XRootPath = hash::CHashStringPtr();
}

void CGraphicFilePathManager::SetObjFilePathRoot( const hash::CHashStringPtr &path )
{
	s_ObjRootPath = path;
}
const hash::CHashStringPtr &CGraphicFilePathManager::GetObjFilePathRoot()
{
	return s_ObjRootPath;
}
skyWString* CGraphicFilePathManager::ConcatObjFilePathRoot( skyWString* destBuf , u32 bufSize , const skyWString *pPath )
{
	if ( s_ObjRootPath.IsEmpty() == skyFalse && s_ObjRootPath->GetSize() > 0 )
	{
		CStringUtil_::Copy( destBuf , bufSize , s_ObjRootPath->GetString() );
		CStringUtil_::Cat( destBuf , pPath );
	}
	else
	{
		CStringUtil_::Copy( destBuf , bufSize , pPath );
	}
	return destBuf;
}
skyMString* CGraphicFilePathManager::ConcatObjFilePathRoot( skyMString* destBuf , u32 bufSize , const skyMString *pPath )
{
	//wcharをcharに変換
	skyMString *cRootPath = CStringUtil_::CreateMString( s_ObjRootPath->GetString() );

	if ( s_ObjRootPath.IsEmpty() == skyFalse && s_ObjRootPath->GetSize() > 0 )
	{
		CStringUtil_::Copy( destBuf , bufSize , cRootPath );
		CStringUtil_::Cat( destBuf , bufSize , pPath );
	}
	else
	{
		CStringUtil_::Copy( destBuf , bufSize , pPath );
	}

	//charのパスを消す
	CStringUtil_::DeleteMString( cRootPath );

	return destBuf;
}

void CGraphicFilePathManager::ClearObjFilePathRoot()
{
	s_ObjRootPath = hash::CHashStringPtr();
}

void CGraphicFilePathManager::SetMtlFilePathRoot( const hash::CHashStringPtr &path )
{
	s_MtlRootPath = path;
}
const hash::CHashStringPtr &CGraphicFilePathManager::GetMtlFilePathRoot()
{
	return s_MtlRootPath;
}
skyWString* CGraphicFilePathManager::ConcatMtlFilePathRoot( skyWString* destBuf , u32 bufSize , const skyWString *pPath )
{
	if ( s_MtlRootPath.IsEmpty() == skyFalse && s_MtlRootPath->GetSize() > 0 )
	{
		CStringUtil_::Copy( destBuf , bufSize , s_MtlRootPath->GetString() );
		CStringUtil_::Cat( destBuf , pPath );
	}
	else
	{
		CStringUtil_::Copy( destBuf , bufSize , s_MtlRootPath->GetString() );
	}
	return destBuf;
}
skyMString* CGraphicFilePathManager::ConcatMtlFilePathRoot( skyMString* destBuf , u32 bufSize , const skyMString *pPath )
{
	//wcharをcharに変換
	skyMString *cRootPath = CStringUtil_::CreateMString( s_MtlRootPath->GetString() );

	if ( s_MtlRootPath.IsEmpty() == skyFalse && s_MtlRootPath->GetSize() > 0 )
	{
		CStringUtil_::Copy( destBuf , bufSize , cRootPath );
		CStringUtil_::Cat( destBuf , bufSize , pPath );
	}
	else
	{
		CStringUtil_::Copy( destBuf , bufSize , cRootPath );
	}

	//charのパスを消す
	CStringUtil_::DeleteMString( cRootPath );

	return destBuf;
}

void CGraphicFilePathManager::ClearMtlFilePathRoot()
{
	s_MtlRootPath = hash::CHashStringPtr();
}

void CGraphicFilePathManager::SetTextureFilePathRoot( const hash::CHashStringPtr &path )
{
	s_TextureRootPath = path;
}

const hash::CHashStringPtr &CGraphicFilePathManager::GetTextureFilePathRoot()
{
	return s_TextureRootPath;
}
skyWString* CGraphicFilePathManager::ConcatTextureFilePathRoot( skyWString* destBuf , u32 bufSize , const skyWString *pPath )
{
	if ( s_TextureRootPath.IsEmpty() == skyFalse && s_TextureRootPath->GetSize() > 0 )
	{
		CStringUtil_::Copy( destBuf , bufSize , s_TextureRootPath->GetString() );
		CStringUtil_::Cat( destBuf , pPath );
	}
	else
	{
		CStringUtil_::Copy( destBuf , bufSize , s_TextureRootPath->GetString() );
	}
	return destBuf;
}
skyMString* CGraphicFilePathManager::ConcatTextureFilePathRoot( skyMString* destBuf , u32 bufSize , const skyMString *pPath )
{
	//wcharをcharに変換
	skyMString *cRootPath = CStringUtil_::CreateMString( s_TextureRootPath->GetString() );

	if ( s_TextureRootPath.IsEmpty() == skyFalse && s_XRootPath->GetSize() > 0 )
	{
		CStringUtil_::Copy( destBuf , bufSize , cRootPath );
		CStringUtil_::Cat( destBuf , bufSize , pPath );
	}
	else
	{
		CStringUtil_::Copy( destBuf , bufSize , cRootPath );
	}

	//charのパスを消す
	CStringUtil_::DeleteMString( cRootPath );

	return destBuf;
}

void CGraphicFilePathManager::ClearTextureFilePathRoot()
{
	s_TextureRootPath = hash::CHashStringPtr();
}

} } } 