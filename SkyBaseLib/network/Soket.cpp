#include "StdAfx.h"
#include "SkyBaseLib/NetWork.h"

namespace sky { namespace lib { namespace net {

#ifdef SKYLIB_PLATFORM_WIN32

void CSoket::Initialize()
{
	WSADATA wsaData;
	int err;

	err = WSAStartup( MAKEWORD( 2 , 0 ) , &wsaData );
	if ( err != 0 )
	{
		switch ( err )
		{
			case WSASYSNOTREADY:
				SKY_PANIC_MSG( _T( "ネットワークサブシステムがネットワークへの接続を準備できていない。" ) );
				break;
			case WSAVERNOTSUPPORTED:
				SKY_PANIC_MSG( _T( "要求されたwinsockのバージョンがサポートされていない。" ) );
				break;
			case WSAEINPROGRESS:
				SKY_PANIC_MSG( _T( "ブロッキング操作の実行中であるか、 またはサービスプロバイダがコールバック関数を処理している。" ) );
				break;
			case WSAEPROCLIM:
				SKY_PANIC_MSG( _T( "winsockが処理できる最大プロセス数に達した。" ) );
				break;
			case WSAEFAULT:
				SKY_PANIC_MSG( _T( "第二引数であるlpWSAData は有効なポインタではない。" ) );
				break;
		}
	}
}

void CSoket::Dispose()
{
	 WSACleanup();
}

#endif


} } }
