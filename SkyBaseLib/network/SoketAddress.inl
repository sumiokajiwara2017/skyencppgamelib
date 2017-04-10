namespace sky { namespace lib { namespace net {

#ifdef SKYLIB_PLATFORM_WIN32

inline CSoketAddress::CSoketAddress( s32 AddressFamily , u16 portNo , const char *address , skyBool isGetHostInfo )
{
	m_pMember = NEW CSoketAddressMember;

	//�ڑ������ۑ�
	m_pMember->m_ConnectInfo.sin_family = ( ADDRESS_FAMILY )AddressFamily;
	m_pMember->m_ConnectInfo.sin_port   = EndianUtil::DataConvert_HostToNetWork( portNo );

	//������̃R�s�[
	CStringUtil_::Sprintf( ( skyMString * )m_pMember->m_Address , ADDRESS_STRING_LENGTH , ( const skyMString * )address );

	char *connectAddress;
	if ( isGetHostInfo == skyTrue )
	{
		//address�𗘗p�\�Ȍ`�ɕϊ�����
		m_Resolver.Lookup( address );

#ifdef  SW_SKYLIB_MEMORY_TEXT_UNICODE
		connectAddress = CStringUtil_::CreateMString( ( const skyString * )m_Resolver.GetAddressList()[ 0 ] );
#else
		connectAddress = ( const skyString * )resolver.GetAddressList()[ 0 ];
#endif

		//����IP���o�C�i��IP�ɕϊ�
		m_pMember->m_ConnectInfo.sin_addr.S_un.S_addr = inet_addr( ( const char * )connectAddress );

		if ( m_pMember->m_ConnectInfo.sin_addr.S_un.S_addr == 0xffffffff )
		{//�����ŃG���[����悤�Ȓl�Ȃ�Lookup�̎��_�ŃG���[����͂��Ȃ̂ł����ŃG���[���邱�Ƃ͍l���ɂ����B
			PANIC_THROW_EXCEPTION( CSoketAppException( eSoketAppErrorIdThisAddressIsInvalid , _T( "adress is invalid." ) ) );
		}

#ifdef  SW_SKYLIB_MEMORY_TEXT_UNICODE
		CStringUtil_::DeleteMString( connectAddress );
#endif
	}
	else
	{
		//����IP���o�C�i��IP�ɕϊ�
		m_pMember->m_ConnectInfo.sin_addr.S_un.S_addr = inet_addr( address );
	}
}

inline CSoketAddress::CSoketAddress( s32 AddressFamily , u16 portNo )
{
	m_pMember = NEW CSoketAddressMember;

	m_pMember->m_ConnectInfo.sin_family           = ( ADDRESS_FAMILY )AddressFamily;
	m_pMember->m_ConnectInfo.sin_port             = EndianUtil::DataConvert_HostToNetWork( portNo );
	m_pMember->m_ConnectInfo.sin_addr.S_un.S_addr = EndianUtil::DataConvert_HostToNetWork( INADDR_ANY );
}

inline CSoketAddress::CSoketAddress()
{
	m_pMember = NEW CSoketAddressMember;
}

inline CSoketAddress::~CSoketAddress()
{
	DEL m_pMember;
}

inline CSoketAddressMember* CSoketAddress::GetMember()
{
	return m_pMember;
}

inline const CResolver &CSoketAddress::GetResolver() const
{
	return m_Resolver;
}

#endif

} } }
