#include "StdAfx.h"
#include "SkyBaseLib/NetWork.h"

#ifdef SKYLIB_PLATFORM_WIN32
#include <Natupnp.h>
#endif

namespace sky { namespace lib { namespace net {

#ifdef SKYLIB_PLATFORM_WIN32

void CUPnPClient::OpnePort( const skyString *name , const char *ip , u16 portNo )
{
	// まずＣＯＭの初期化
	HRESULT ret = ::CoInitialize( NULL );

	if ( S_OK != ret )
	{
		SKY_PANIC_MSG( "CoInitialize failed." ); 
	}

	// インスタンス取得
	IUPnPNAT *nat;
	ret = ::CoCreateInstance( CLSID_UPnPNAT , NULL, CLSCTX_ALL , IID_IUPnPNAT , ( void ** )&nat );

	if ( S_OK != ret )
	{
		SKY_PANIC_MSG( "CoCreateInstance failed." ); 
	}

	// ポートマッピングのリスト取得
	IStaticPortMappingCollection *maps = NULL;
	nat->get_StaticPortMappingCollection( &maps );

	if ( maps == skyNull )
	{
		//セキュリティソフトやWindowsファイヤーウォールの設定ではじかれているか、ルーターがUPnPに対応していない。LANケーブルでつながっていないとか。
		//固定IPにするとか。http://www.akakagemaru.info/port/windows7upnp.html とか
		SKY_PANIC_MSG( "get_StaticPortMappingCollection failed." ); 
	}
	else
	{
		// アドレスと名前を登録できる形式に変更
		WCHAR localIPWideString[ 16 ];
		MultiByteToWideChar( CP_ACP , 0 , ip , -1 , localIPWideString , 16 );
		BSTR localIPBstr = SysAllocString( localIPWideString );
		BSTR proto       = SysAllocString( L"TCP" );     // TCP/IPで登録する
		BSTR description = SysAllocString( name ); //ここはポートマッピングに登録する名前

		//ポートマッピングを登録
		IStaticPortMapping *map;
		ret = maps->Add( portNo , proto , portNo , localIPBstr , VARIANT_TRUE , description , &map );

		if ( S_OK != ret )
		{
			SKY_PANIC_MSG( "Add failed." ); 
		}

		// BSTR の解放
		SysFreeString( description );
		SysFreeString( proto );
		SysFreeString( localIPBstr );

		// オブジェクト解放
		map->Release();
		maps->Release();
	}

	nat->Release();

	// ＣＯＭの終了
	::CoUninitialize();
}

void CUPnPClient::ClosePort( const char *ip , u16 portNo )
{
	// まずＣＯＭの初期化
	HRESULT ret = ::CoInitialize( NULL );

	if ( S_OK != ret )
	{
		SKY_PANIC_MSG( "CoInitialize failed." ); 
	}

	// インスタンス取得
	IUPnPNAT *nat;
	ret = ::CoCreateInstance( CLSID_UPnPNAT , NULL, CLSCTX_ALL , IID_IUPnPNAT , ( void ** )&nat );

	if ( S_OK != ret )
	{
		SKY_PANIC_MSG( "CoCreateInstance failed." ); 
	}

	// ポートマッピングのリスト取得
	IStaticPortMappingCollection *maps = NULL;
	nat->get_StaticPortMappingCollection( &maps );

	if ( maps == skyNull )
	{
		//セキュリティソフトやWindowsファイヤーウォールの設定ではじかれているか、ルーターがUPnPに対応していない。LANケーブルでつながっていないとか。
		//固定IPにするとか。http://www.akakagemaru.info/port/windows7upnp.html とか
		SKY_PANIC_MSG( "get_StaticPortMappingCollection failed." ); 
	}
	else
	{
		// アドレスと名前を登録できる形式に変更
		WCHAR localIPWideString[ 16 ];
		MultiByteToWideChar( CP_ACP , 0 , ip , -1 , localIPWideString , 16 );
		BSTR localIPBstr = SysAllocString( localIPWideString );
		BSTR proto = SysAllocString( L"TCP" );     // TCP/IPで登録する

		//ポートマッピングを削除
		ret = maps->Remove( portNo , proto );

		if ( S_OK != ret )
		{
			SKY_PANIC_MSG( "Remove failed." ); 
		}

		// BSTR の解放
		SysFreeString( proto );
		SysFreeString( localIPBstr );

		// オブジェクト解放
		maps->Release();
	}

	nat->Release();

	// ＣＯＭの終了
	::CoUninitialize();
}

#endif

} } }
