namespace sky { namespace lib { namespace net {

#ifdef SKYLIB_PLATFORM_WIN32

inline CResolver::CResolver()
{
	m_pMember = NEW CResolverMember;
}

inline CResolver::~CResolver()
{
	DEL m_pMember;
}

inline void CResolver::Lookup( const char *pStr )
{
	LPHOSTENT   host; //���ꂪ�~�����B
	SOCKADDR_IN server;

	//�@�܂�IP�A�h���X���Ɨ\�z����inet_addr()�ŕ������A�h���X�̕ϊ����s��
	server.sin_addr.S_un.S_addr = inet_addr( ( const char * )pStr );
	if ( server.sin_addr.S_un.S_addr != 0xffffffff )
	{
		//���̊֐���pv4�̂ݑΉ�
		host = gethostbyaddr( ( const char * )&server.sin_addr.S_un.S_addr , sizeof( server.sin_addr.S_un.S_addr ), AF_INET );
		if ( host == NULL )
		{
			PANIC_THROW_EXCEPTION( CSoketAppException( eSoketAppErrorIdThisAddressIsInvalid , _T( "This IP is no host." ) ) );
		}
	}
	else
	//�A�@�����s�����ꍇ�A�z�X�g�����Ƃ݂Ȃ���gethostbyname()�Ńz�X�g��������
	{
		host = gethostbyname( ( const char * )pStr );
		if ( host == NULL )
		{
			PANIC_THROW_EXCEPTION( CSoketAppException( eSoketAppErrorIdThisAddressIsInvalid , _T( "This char is not host." ) ) );
		}
	}

	//�z�X�g���i�[
	m_HostName.Clear();
	m_HostName = host->h_name;

	//�ʖ��i�[
	m_AliasesList.DeleteAll();
	for( u32 i = 0 ; host->h_aliases[ i ] ; i++)
	{
		m_AliasesList.AddTail( host->h_aliases[ i ] );
	}

	//�A�h���X�𕶎��ɂ��Ċi�[���Ȃ���
	m_AddressList.DeleteAll();
	for( u32 i = 0 ; host->h_addr_list[ i ] ; i++ )
	{
		//�A�h���X�𕶎��ϊ�����
		skyMString address[ 32 ];
		CStringUtil_::Sprintf( address , 32 , "%d.%d.%d.%d" ,
			( BYTE ) * ( ( host->h_addr_list[ i ] ) ) ,
			( BYTE ) * ( ( host->h_addr_list[ i ] ) + 1 ) ,
			( BYTE ) * ( ( host->h_addr_list[ i ] ) + 2 ) ,
			( BYTE ) * ( ( host->h_addr_list[ i ] ) + 3 ) );
			m_AddressList.AddTail( address );
	}

	//�A�h���X���o�C�i���ɂ��Ċi�[����(�{���͏�L�����Ŏ擾�ł���悤�ȋC������񂾂��ǁA�ϊ��e�X�g�����˂ĕ�������o�C�i��������j
	FOREACH( string::CStringList , it , m_AddressList )
	{
		char *connectAddress;
#ifdef  SW_SKYLIB_MEMORY_TEXT_UNICODE
		connectAddress = CStringUtil_::CreateMString( ( const skyString * )( *it ) );
#else
		connectAddress = ( const skyString * )resolver.GetAddressList()[ 0 ];
#endif

		ULONG addr = inet_addr( ( const char * )connectAddress );
		if ( addr == 0xffffffff )
		{
			PANIC_THROW_EXCEPTION( CSoketAppException( eSoketAppErrorIdThisAddressIsInvalid , _T( "This IP is no host." ) ) );
		}
		else
		{
			m_pMember->m_BinAddress.Add( addr );
		}

#ifdef  SW_SKYLIB_MEMORY_TEXT_UNICODE
		CStringUtil_::DeleteMString( connectAddress );
#endif
	}
}

inline const string::CString& CResolver::GetHostName()
{
	return m_HostName;
}

inline const string::CStringList& CResolver::GetAliasesList()
{
	return m_AliasesList;
}

inline const string::CStringList& CResolver::GetAddressList()
{
	return m_AddressList;
}

inline void CResolver::Print()
{
	SKY_PRINTF( _T( "HostName : %s \n" ) , ( const skyString* )m_HostName );

	SKY_PRINTF( _T( "AliaseName : " ) );
	FOREACH( string::CStringList , it , m_AliasesList )
	{
		SKY_PRINTF( _T( " %s " ) , ( const skyString* )( *it ) );
	}
	SKY_PRINTF( _T( "\n" ) );

	SKY_PRINTF( _T( "Address : " ) );
	FOREACH( string::CStringList , it , m_AddressList )
	{
		SKY_PRINTF( _T( " %s " ) , ( const skyString* )( *it ) );
	}
	SKY_PRINTF( _T( "\n" ) );

}

inline const CResolverMember *CResolver::GetMember() const
{
	return m_pMember;
}

inline skyBool CResolverMember::CheckBinAddress( ULONG address ) const
{
	skyBool isFound = skyFalse;

	ULONG* topAdr = m_BinAddress.GetTopAdr();
	for ( u32 i = 0 ; i < m_BinAddress.GetNum() ; i++ )
	{
		if ( *( topAdr + i ) == address )
		{//������
			isFound = skyTrue;
		}
	}

	return isFound;
}

#endif

} } }
