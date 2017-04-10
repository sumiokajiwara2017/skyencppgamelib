namespace sky { namespace lib { namespace net {

#ifdef SKYLIB_PLATFORM_WIN32

inline CSoket::CSoket()
{
}

inline CSoket::~CSoket()
{
}

inline void CSoket::Open( socket_hndl &soketHndl , s32 AddressFamily , s32 soketType , s32 protcol )
{
	//ソケットを開く
	soketHndl = socket( AddressFamily, soketType , protcol );
	if ( soketHndl == INVALID_SOCKET )
	{
		PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "Sock create failed." ) ) )
	}
}

inline void CSoket::Close( socket_hndl &soketHndl )
{
	// ソケットを閉じる
	closesocket( soketHndl );
}

inline void CSoket::Bind( socket_hndl &soketHndl , const CSoketAddressPtr &spAddr )
{
	int n = bind( soketHndl , ( struct sockaddr * )&spAddr->GetMember()->m_ConnectInfo , sizeof( spAddr->GetMember()->m_ConnectInfo ) );

	if ( n != 0 )
	{
		PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "bind failed." ) ) )
	}
}

inline void CSoket::Connect( socket_hndl &soketHndl , const CSoketAddressPtr &spAddr )
{
	int n = connect( soketHndl , ( struct sockaddr * )&spAddr->GetMember()->m_ConnectInfo , sizeof( spAddr->GetMember()->m_ConnectInfo ) );

	if ( n != 0 )
	{
		PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "connect failed." ) ) )
	}
}

inline void CSoket::DisConnect( socket_hndl &soketHndl , s32 id )
{
	// まず送信を無効にし(1)データをすべて受け取ったら受信も無効にする(0）と既に送られたデータはすべて受け取りかつ切断できる
	int n = shutdown( soketHndl , id );

	if ( n != 0 )
	{
		PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "shutdown failed." ) ) )
	}
}

#endif


} } }
