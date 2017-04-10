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
	//�\�P�b�g���J��
	soketHndl = socket( AddressFamily, soketType , protcol );
	if ( soketHndl == INVALID_SOCKET )
	{
		PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "Sock create failed." ) ) )
	}
}

inline void CSoket::Close( socket_hndl &soketHndl )
{
	// �\�P�b�g�����
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
	// �܂����M�𖳌��ɂ�(1)�f�[�^�����ׂĎ󂯎�������M�������ɂ���(0�j�Ɗ��ɑ���ꂽ�f�[�^�͂��ׂĎ󂯎�肩�ؒf�ł���
	int n = shutdown( soketHndl , id );

	if ( n != 0 )
	{
		PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "shutdown failed." ) ) )
	}
}

#endif


} } }
