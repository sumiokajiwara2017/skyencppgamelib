#include "StdAfx.h"
#include "SkyBaseLib/Debug.h"
#include "SkyBaseLib/Memory.h"
#include "SkyBaseLib/Const.h"
#include "SkyBaseLib/File.h"
#include "SkyBaseLib/Locale.h"

#include <locale.h>
#include <stdio.h>

#ifdef SW_SKYLIB_DEBUG_ON

#include "DebugWin.inl"

namespace sky { namespace lib { namespace debuglogic {

eLogLevel    				 CDebugUtil::m_eLogLevel = eLogLevel_Error;
thread::CriticalSection	*CDebugUtil::m_pCs = skyNull;
u32							 CDebugUtil::m_nLogCategory = LOG_CATEGORY_ALL;

static file::CDiscFileStream* m_pFileStream = skyNull;
static memory::CMemoryStream* m_pMemStream  = skyNull;
static thread::tread_id m_nMainThreadId    = 0;

//-------------------------------------< 実装 >---------------------------------------------//

void CDebugUtil::Initialize()
{
	m_pCs = thread::CriticalSection::Create();
	m_nMainThreadId = thread::CThreadUtil::GetCurrentThreadId(); //初期化がメインスレッドで行われること前提
	m_pFileStream = skyNull;
	m_pMemStream  = skyNull;
}

void CDebugUtil::Dispose()
{
	thread::CriticalSection::Delete( m_pCs );
	m_pCs = skyNull;

	CloseFileStream();
	CloseMemoryStream();
	m_nMainThreadId = 0;
}

skyBool CDebugUtil::IsMainThread()
{
	return ( m_nMainThreadId == thread::CThreadUtil::GetCurrentThreadId() );
}

void CDebugUtil::SetLogLevel( eLogLevel eLevel )
{
	m_eLogLevel = eLevel;
}

void CDebugUtil::SetLogCategory( u32 category )
{
	m_nLogCategory |= category;
}

void CDebugUtil::DeleteLogCategory( u32 category )
{
	m_nLogCategory &= ~category;
}

void CDebugUtil::OpenFileStream( const skyString *path )
{
	CloseFileStream();
	m_pFileStream = NEW__( file::CDiscFileStream , file::CDiscFileStream( path , file::eOpenMode_Binary_RW_Create ) );
	m_pFileStream->vOpen();
#if defined( SKYLIB_COMPILER_VC10 ) && defined( _UNICODE )
	m_pFileStream->vWriteStream( ( u16 )BOM ); //WindowsでUnicode文字を扱ってる場合はBOMを付ける
#endif
}
void CDebugUtil::CloseFileStream()
{
	if ( m_pFileStream != skyNull )
	{
		SAFE_DELETE( m_pFileStream );
		m_pFileStream = skyNull;
	}
}
void CDebugUtil::WriteFileStream( const skyString *format , ... )
{
	if ( m_pFileStream != skyNull )
	{
		skyString	buf[ DEBUG_MSG_LENGHT_MAX ];
		VA_LIST( buf , DEBUG_MSG_LENGHT_MAX , format );
		m_pFileStream->vWriteStream( buf );
	}
}

void CDebugUtil::OpenMemoryStream( const void **symbolPointAdr )
{
	

	CloseMemoryStream();
	m_pMemStream = NEW__( memory::CMemoryStream , memory::CMemoryStream( _T( "DebugMemoryDump" ) , 32 , skyTrue , skyFalse , skyTrue ) );
	symbolPointAdr = ( const void ** )m_pMemStream->vGetDataPtr();
	m_pMemStream->vOpen();
}
void CDebugUtil::CloseMemoryStream()
{
	if ( m_pMemStream != skyNull )
	{
		SAFE_DELETE( m_pMemStream );
		m_pMemStream = skyNull;
	}
}
void CDebugUtil::WriteMemoryStream( const skyString *format , ... )
{
	if ( m_pMemStream != skyNull )
	{
		skyString	buf[ DEBUG_MSG_LENGHT_MAX ];
		VA_LIST( buf , DEBUG_MSG_LENGHT_MAX , format );
		m_pMemStream->vWriteStream( buf );
	}
}

void CDebugUtil::Printf( eLogLevel eLevel , u32 category , const skyWString *format , ... )
{
	if ( !( category & m_nLogCategory || LOG_CATEGORY_ALL & m_nLogCategory ) )
	{
		return;
	}

	if ( eLevel < CDebugUtil::m_eLogLevel )
	{
		return;
	}

	thread::CriticalLock cs( m_pCs );

	skyString	buf[ DEBUG_MSG_LENGHT_MAX ];
	VA_LIST_W( buf , DEBUG_MSG_LENGHT_MAX , format );

	WriteFileStream( buf );
	WriteMemoryStream( buf );
	::_tprintf( buf );
	OutputDebugString( buf );
}

void CDebugUtil::Printf( eLogLevel eLevel , u32 category , const skyMString *format , ... )
{
	if ( !( category & m_nLogCategory || LOG_CATEGORY_ALL & m_nLogCategory ) )
	{
		return;
	}

	if ( eLevel < CDebugUtil::m_eLogLevel )
	{
		return;
	}

	thread::CriticalLock cs( m_pCs );

	skyMString	buf[ DEBUG_MSG_LENGHT_MAX ];
	VA_LIST_S( buf , DEBUG_MSG_LENGHT_MAX , format );

#ifndef UNICODE
	WriteFileStream( buf );
	WriteMemoryStream( buf );
#endif
	::printf( buf );
	OutputDebugStringA( buf );
}

void CDebugUtil::Panic( const skyString *pFileName , const skyString *pFuncName , s32 lineNo, const skyWString *format , ... )
{
	skyString	buf[ DEBUG_MSG_LENGHT_MAX ];
	skyString	buf_Out[ DEBUG_MSG_LENGHT_MAX*2 ];
	VA_LIST( buf , DEBUG_MSG_LENGHT_MAX , format );

	CStringUtil_::Sprintf( buf_Out, DEBUG_MSG_LENGHT_MAX * 2 , _T( "Panic!! fileName=%s\n funcName=%s\n lineNo=%d\n message=%s\n" ) , pFileName , pFuncName , lineNo , buf );

	PrintfStackTrace( _T( "Panic generation. \n" ) );
	OutputDebugString( buf );
	WriteFileStream( buf );
	WriteMemoryStream( buf );
	MessageBox( skyNull , buf_Out ,_T( "Panic generation." ),MB_OK);

    DEBUG_BREAK();

	Exit( 1 );
}

void CDebugUtil::Panic( const skyString *pFileName , const skyString *pFuncName , s32 lineNo, const skyMString *format , ... )
{
	skyMString	buf[ DEBUG_MSG_LENGHT_MAX ];
	skyMString	buf_Out[ DEBUG_MSG_LENGHT_MAX*2 ];
	VA_LIST_S( buf , DEBUG_MSG_LENGHT_MAX , format );

	CStringUtil_::Sprintf( buf_Out, DEBUG_MSG_LENGHT_MAX * 2 , "Panic!! fileName=%s\n funcName=%s\n lineNo=%d\n message=%s\n" , pFileName , pFuncName , lineNo , buf );

	PrintfStackTrace( _T( "Panic generation. \n" ) );
	OutputDebugStringA( buf );
#ifndef UNICODE
	WriteFileStream( buf );
	WriteMemoryStream( buf );
#endif
	MessageBoxA( skyNull ,buf_Out, "Panic generation." , MB_OK );

    DEBUG_BREAK();

	Exit( 1 );
}

void CDebugUtil::Assert( skyBool isSuccess , const skyString *pFileName , const skyString *pFuncName , s32 lineNo, const skyWString *format , ... )
{
	skyString	buf[ DEBUG_MSG_LENGHT_MAX ];
	skyString	buf_Out[ DEBUG_MSG_LENGHT_MAX*2 ];
	VA_LIST( buf , DEBUG_MSG_LENGHT_MAX , format );

	if ( !isSuccess )
	{
        DEBUG_BREAK();

		VA_LIST_W( buf , DEBUG_MSG_LENGHT_MAX , format );

		CStringUtil_::Sprintf( buf_Out, DEBUG_MSG_LENGHT_MAX * 2 , _T( "Assert!! fileName=%s\n funcName=%s\n lineNo=%d\n message=%s\n" ) , pFileName , pFuncName , lineNo , buf );

		PrintfStackTrace( _T( "Assert generation. \n" ) );
		OutputDebugString( buf );
		WriteFileStream( buf );
		WriteMemoryStream( buf );
		MessageBox( skyNull , buf_Out , _T( "Assert generation." ) , MB_OK );

        DEBUG_BREAK();

		Exit( 1 );
	}
}

void CDebugUtil::Assert( skyBool isSuccess , const skyString *pFileName , const skyString *pFuncName , s32 lineNo, const skyMString *format , ... )
{
	skyMString	buf[ DEBUG_MSG_LENGHT_MAX ];
	skyMString	buf_Out[ DEBUG_MSG_LENGHT_MAX*2 ];
	VA_LIST_S( buf , DEBUG_MSG_LENGHT_MAX , format );

	if ( !isSuccess )
	{
        DEBUG_BREAK();

		VA_LIST_S( buf , DEBUG_MSG_LENGHT_MAX , format );

		CStringUtil_::Sprintf( buf_Out, DEBUG_MSG_LENGHT_MAX * 2 , "Assert!! fileName=%s\n funcName=%s\n lineNo=%d\n message=%s\n" , pFileName , pFuncName , lineNo , buf );

		PrintfStackTrace( _T( "Assert generation. \n" ) );
		OutputDebugStringA( buf );
#ifndef UNICODE
		WriteFileStream( buf );
		WriteMemoryStream( buf );
#endif
		MessageBoxA( skyNull , buf_Out , "Assert generation." , MB_OK );

        DEBUG_BREAK();

		Exit( 1 );
	}
}

void CDebugUtil::Exit( s32 status )
{
    SKY_PRINTF( _T( "It canceled.\n" ) );

	//強制終了
	::exit( status );
}

void CDebugUtil::PrintParam( s32 argc, skyString *argv[] )
{
	thread::CriticalLock cs( m_pCs );

    SKY_PRINTF( _T( "Number of arguments = %d\n" ), argc );

	for ( s32 i = 0 ; i < argc; i++ )
	{
        SKY_PRINTF( _T( "arguments%d = %s\n" ), i + 1, argv[ i ] );
	}
}

void CDebugUtil::PrintfStackTrace( const skyString *format , ... )
{
	thread::CriticalLock cs( m_pCs );

	skyString	buf[ DEBUG_MSG_LENGHT_MAX ];
	VA_LIST( buf , DEBUG_MSG_LENGHT_MAX , format );

	SKY_PRINTF( buf );
#ifdef SKYLIB_COMPILER_VC10
	OutputCurrentStackFrameToDebugStream();
#endif
}

void CDebugUtil::MemDump( u8 *adr , u32 before , u32 after , u32 lineCharNum )
{
	u32 lineNum = 1;
	u32 outNum = 1;
	u8 *point = adr - before;
	for ( u32 i = 1 ; i <= before + 1 ; i++ )
	{
		SKY_PRINTF( _T( "%02X" )  , *point );

		if ( i + 1 <= before + 1 )
		{
			SKY_PRINTF( _T( " " )  , *point );
		}
		else
		{
			SKY_PRINTF( _T( ">" )  , *point );
		}

		if ( lineCharNum * lineNum == outNum  )
		{
			u8 *p = point - lineCharNum;
			for ( u32 j = 0 ; j < lineCharNum ; j++ )
			{
				u32 csize = sizeof( skyMString ) * 2;
				skyMString *c = ( skyMString * )SkyMalloc( csize );
				CMemoryUtil_::ClearZero( c , csize );
				if ( p[ j ] == 0 )
				{
					CMemoryUtil_::Copy( c , " " , sizeof( skyMString ) );
				}
				else
				{
					CMemoryUtil_::Copy( c , &p[ j ] , sizeof( skyMString ) );
				}
				skyString *out = CStringUtil_::CreateString( c );
				SKY_PRINTF( out );
				CStringUtil_::DeleteString( out );
				DEL c;
				p++;
			}

			SKY_PRINTF( _T( "\n" ) );
			lineNum++;
		}

		outNum++;
		point++;
	}

	for ( u32 i = 1 ; i < after + 1 ; i++ )
	{
		SKY_PRINTF( _T( "%02X " )  , *point );

		if ( lineCharNum * lineNum == outNum  )
		{
			u8 *p = point - lineCharNum;
			for ( u32 j = 0 ; j < lineCharNum ; j++ )
			{
				u32 csize = sizeof( skyMString ) * 2;
				skyMString *c = ( skyMString * )SkyMalloc( csize );
				CMemoryUtil_::ClearZero( c , csize );
				if ( p[ j ] == 0 || p[ j ] == '%' )
				{
					CMemoryUtil_::Copy( c , " " , sizeof( skyMString ) );
				}
				else
				{
					CMemoryUtil_::Copy( c , &p[ j ] , sizeof( skyMString ) );
				}
				skyString *out = CStringUtil_::CreateString( c );
				SKY_PRINTF( out );
				CStringUtil_::DeleteString( out );
				DEL c;
				p++;
			}

			SKY_PRINTF( _T( "\n" ) );
			lineNum++;
		}

		outNum++;
		point++;
	}

	SKY_PRINTF( _T( "\n" ) );
}

void CDebugUtil::PrintIndent( u32 indentNum )
{
	for( u32 i = 0 ; i < indentNum ; i++ )
	{
		SKY_PRINTF( _T( " " ) );
	}
}

} } }

#endif
