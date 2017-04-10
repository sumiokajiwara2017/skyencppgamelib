#include "StdAfx.h"
#include "SkyGraphicLib/Window.h"

#include "SkyGraphicLib/Controller.h"

//===================================【Common】===================================================>

SkyImplementRTTI( sky::lib::graphic::CWindow , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&m_SetUpKind , sizeof( s32 * ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_WindowRect.X() , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_WindowRect.Y() , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_WindowRect.W() , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_WindowRect.H() , sizeof( d64 ) , isWrite ); \

skyBool CWindow::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "window bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN( skyTrue );

	return result;
}

skyBool CWindow::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "window bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN( skyFalse );

	return result;
}

const serialize::CSerializeInterfacePtr &CWindow::Copy( const serialize::CSerializeInterfacePtr &spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CWindowPtr )spContainer )->m_WindowRect = m_WindowRect;
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CWindow::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CWindow::Clone()
{
	return Copy( CWindowPtr( NEW__( CWindow , CWindow() ) ) );
}

serialize::CSerializeInterfacePtr CWindow::CloneSharing()
{
	return Clone();
}

void CWindow::Activate()
{
	ACTIVATE_COMMON();

	SetUp( m_WindowRect );
}

void CWindow::Deactivate()
{
	DEACTIVATE_COMMON();
}

CWindow::CWindow()
{
	m_pMember = NEW__( CWindowMember , CWindowMember );
	m_pMember->m_nWindowHndl = ( HWND )INT_MAX;
}

CWindow::~CWindow()
{
	if ( m_pMember->m_nWindowHndl != ( HWND )INT_MAX && m_SetUpKind == eSetUpKind_Create )
	{
		DestroyWindow( m_pMember->m_nWindowHndl );
		UnregisterClass( m_pMember->m_kWc.lpszClassName , m_pMember->m_kWc.hInstance );
	}

	DEL m_pMember;
}

void CWindow::SetUp( const math::CBasicRect &rect )
{
	m_SetUpKind = eSetUpKind_Create;
	m_WindowRect = rect;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	DWORD rscode = GetLastError();		//拡張エラー情報の取得

	ZeroMemory( &m_pMember->m_kWc, sizeof( WNDCLASSEX ) );//構造体を全部0で初期化
	m_pMember->m_kWc.cbSize	       = sizeof( WNDCLASSEX ); 
	m_pMember->m_kWc.lpfnWndProc   = ( WNDPROC )SkyWndProc;//静的プロシージャを設定
	m_pMember->m_kWc.hInstance     = hInstance;
	m_pMember->m_kWc.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
	m_pMember->m_kWc.lpszClassName = Name.GetName()->GetString();
	WORD rs = RegisterClassEx( &m_pMember->m_kWc );

	if( rs == 0 )
	{
		rscode = GetLastError();		//拡張エラー情報の取得
		LPVOID lpMessageBuffer;
  
		FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		rscode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // デフォルト ユーザー言語 
		(LPTSTR) &lpMessageBuffer,
		0,
		NULL );

        SKY_PRINTF( _T( "windows error. %s\n" ) , lpMessageBuffer );
  
		// システムによって確保されたバッファを開放します。
		LocalFree( lpMessageBuffer );

        SKY_PANIC_MSG( _T( "RegisterClassEx failed.\n" ) );
	}

	// ウィンドウ生成
	if(!SkyGraphicLibController_::IsFullScreen())
	{
		//Windowモード
	    m_pMember->m_nWindowHndl = CreateWindow( Name.GetName()->GetString(), Name.GetName()->GetString(), 
			WS_OVERLAPPEDWINDOW, ( s32 )rect.X() , ( s32 )rect.Y() , ( s32 )rect.W() , ( s32 )rect.H() ,
				GetDesktopWindow(), NULL, m_pMember->m_kWc.hInstance, NULL );
	}
	else
	{
		//フルスクリーン
		m_pMember->m_nWindowHndl = CreateWindow( Name.GetName()->GetString(), Name.GetName()->GetString(), 
			WS_VISIBLE, ( s32 )rect.X() , ( s32 )rect.Y() , ( s32 )rect.W() , ( s32 )rect.H() ,
		    	HWND_DESKTOP, NULL, m_pMember->m_kWc.hInstance, NULL );
	}

	if( m_pMember->m_nWindowHndl == NULL )
	{
		LPVOID lpMessageBuffer;
  
		FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // デフォルト ユーザー言語 
		(LPTSTR) &lpMessageBuffer,
		0,
		NULL );

        SKY_PRINTF( _T( "window error.%s\n" ) , lpMessageBuffer );
  
		// システムによって確保されたバッファを開放します。
		LocalFree( lpMessageBuffer );

        SKY_PANIC_MSG( _T( "window create failed.\n" ) );
		/*
			この関数の失敗で考えられる一般的な原因は次のとおりです。
			&#8226;パラメータに無効な値が指定された。
			&#8226;システムクラスを登録したモジュールが別のモジュールである。
			&#8226;WH_CBT フックがインストールされており、失敗コードを返した。
			&#8226;WM_CREATE または WM_NCCREATE のウィンドウプロシージャが失敗した。
		*/
	}
	
    ShowWindow( m_pMember->m_nWindowHndl, SW_SHOWDEFAULT );
    UpdateWindow( m_pMember->m_nWindowHndl );

	RECT windowPosSize = { 0, 0, ( LONG )rect.W(), ( LONG )rect.H() };
	AdjustWindowRectEx( &windowPosSize, WS_OVERLAPPEDWINDOW, FALSE, 0 );
	SetWindowPos( m_pMember->m_nWindowHndl, NULL, 0, 0, windowPosSize.right-windowPosSize.left, windowPosSize.bottom-windowPosSize.top, SWP_NOMOVE|SWP_NOZORDER );
}
void  CWindow::SetUp( s32 hndl )
{
	m_SetUpKind = eSetUpKind_Hndl;
	m_pMember->m_nWindowHndl = ( HWND )hndl;

    UpdateWindow( m_pMember->m_nWindowHndl );
}

wndType CWindow::GetWindowId()
{
	return ( wndType )m_pMember->m_nWindowHndl;
}

} } }
