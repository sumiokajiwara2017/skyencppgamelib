#include "StdAfx.h"
#include "SkyBaseLib/NetWork.h"

namespace sky { namespace lib { namespace net {

CUDPSoket::CUDPSoket()
{
	m_IsOpen = skyFalse;
	m_IsOpenBroadCast = skyFalse;
	m_IsOpenMultiCast = skyFalse;
}

CUDPSoket::~CUDPSoket()
{
	if ( m_IsOpen == skyTrue )
	{
		Close();
	}
}

void CUDPSoket::Open( u16 portNo , const char *address )
{
	ASSERT_THROW_EXCEPTION( m_IsOpen == skyFalse , CSoketAppException( eSoketAppErrorIdSoketAlreadyOpen , _T( "Open already exec." ) ) );

	//UDP���M�p�ڑ����\�z
	m_spSendSoketAddress = CSoketAddressPtr( NEW CSoketAddress( AF_INET , portNo , address ) );

	//UDP�N���C�A���g�p�\�P�b�g�I�[�v��
	this->CSoket::Open( m_SendSoketHndl , AF_INET , SOCK_DGRAM , 0 );

	m_IsOpen = skyTrue;
}

void CUDPSoket::Open( const CSoketAddressPtr &spAddress )
{
	ASSERT_THROW_EXCEPTION( m_IsOpen == skyFalse , CSoketAppException( eSoketAppErrorIdSoketAlreadyOpen , _T( "Open already exec." ) ) );

	//UDP���M�p�ڑ����\�z
	m_spSendSoketAddress = spAddress;

	//UDP�N���C�A���g�p�\�P�b�g�I�[�v��
	this->CSoket::Open( m_SendSoketHndl , AF_INET , SOCK_DGRAM , 0 );

	m_IsOpen = skyTrue;
}

void CUDPSoket::Close()
{
	ASSERT_THROW_EXCEPTION( m_IsOpen == skyTrue , CSoketAppException( eSoketAppErrorIdSoketNotOpen , _T( "Open not exec yet." ) ) );

	//�\�P�b�g�N���[�Y
	this->CSoket::Close( m_SendSoketHndl );

	m_IsOpen = skyFalse;
}

u32 CUDPSoket::Send( const void* pData , u32 size )
{
	ASSERT_THROW_EXCEPTION( m_IsOpen == skyTrue , CSoketAppException( eSoketAppErrorIdSoketNotOpen , _T( "Open not exec yet." ) ) );

	int n = sendto( m_SendSoketHndl , ( const char * )pData , size , 0 , (struct sockaddr *)&m_spSendSoketAddress->GetMember()->m_ConnectInfo, sizeof( m_spSendSoketAddress->GetMember()->m_ConnectInfo ) );

	//�G���[�`�F�b�N
	if ( n < 1 )
	{
		PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "sendto failed." ) ) )
	}

	return ( u32 )n;
}

u32 CUDPSoket::Receive( void* pData , u32 size , u32 tryNum )
{
	ASSERT_THROW_EXCEPTION( m_IsOpen == skyTrue , CSoketAppException( eSoketAppErrorIdSoketNotOpen , _T( "Open not exec yet." ) ) );

	//���M�����
	struct sockaddr_in fromAddr;
	int fromLen = sizeof( fromAddr );

	//�f�[�^��M��
	s32 receiveSize = 0;
	for ( u32 i = 0 ; i < tryNum ; i++ )
	{
		//��M����
		receiveSize = recvfrom( m_SendSoketHndl , ( char * )pData , size , 0 , ( struct sockaddr * )&fromAddr, &fromLen );

		if ( receiveSize < 0 && WSAEWOULDBLOCK != WSAGetLastError() )
		{
			PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "recv failed. \n" ) ) )
		} 
		else if ( ( u32 )receiveSize > size )
		{
			PANIC_THROW_EXCEPTION( CSoketAppException( eSoketAppErrorIdReceiveBufferOver , _T( "ReceiveBuffer Over." ) ) )
		}
		else
		{
			//���ł���M������������Ԃ��Ă����i�p�P�b�g���B���������Ă�\��������̂ŋ��ʋ@�\�Ńf�[�^���}�[�W���邱�Ƃ͂ł��Ȃ��j
			break; 
		}
	}

	return receiveSize;
}


void CUDPSoket::OpenBroadCast( u16 portNo , const char *address )
{
	ASSERT_THROW_EXCEPTION( m_IsOpenBroadCast == skyFalse , CSoketAppException( eSoketAppErrorIdSoketAlreadyOpen , _T( "Open already exec." ) ) );

	//UDP���M�p�ڑ����\�z
	m_spBroadCastSoketAddress = CSoketAddressPtr( NEW CSoketAddress( AF_INET , portNo , address , skyFalse ) );

	//UDP�N���C�A���g�p�\�P�b�g�I�[�v��
	this->CSoket::Open( m_BroadCastSoketHndl , AF_INET , SOCK_DGRAM , 0 );

	//�u���[�h�L���X�g�p�̃\�P�b�g�I�v�V������ݒ肷��i�������ʏ��send�ƈႤ�j
	BOOL yes = 1;
	int n = setsockopt( m_BroadCastSoketHndl , SOL_SOCKET, SO_BROADCAST, ( char * )&yes, sizeof( yes ) );

	if ( n != 0 )
	{
		PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "setsockopt failed." ) ) )
	}

	m_IsOpenBroadCast = skyTrue;
}

void CUDPSoket::CloseBroadCast()
{
	ASSERT_THROW_EXCEPTION( m_IsOpenBroadCast == skyTrue , CSoketAppException( eSoketAppErrorIdSoketNotOpen , _T( "Open not exec yet." ) ) );

	//�\�P�b�g�N���[�Y
	this->CSoket::Close( m_BroadCastSoketHndl );

	m_IsOpenBroadCast = skyFalse;
}

u32  CUDPSoket::SendBroadCast( const void* pData , u32 size )
{
	ASSERT_THROW_EXCEPTION( m_IsOpenBroadCast == skyTrue , CSoketAppException( eSoketAppErrorIdSoketNotOpen , _T( "Open not exec yet." ) ) );

	int n = sendto( m_BroadCastSoketHndl , ( const char * )pData , size , 0 , ( struct sockaddr * )&m_spBroadCastSoketAddress->GetMember()->m_ConnectInfo, sizeof( m_spBroadCastSoketAddress->GetMember()->m_ConnectInfo ) );

	//�G���[�`�F�b�N
	if ( n < 1 )
	{
		PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "sendto failed." ) ) )
	}

	return ( u32 )n;
}

u32  CUDPSoket::ReceiveBroadCast( void* pData , u32 size , const CSoketAddressPtr &spFromAddress , u32 tryNum )
{
	ASSERT_THROW_EXCEPTION( m_IsOpenBroadCast == skyTrue , CSoketAppException( eSoketAppErrorIdSoketNotOpen , _T( "Open not exec yet." ) ) );

	//���M�����
	int fromLen = sizeof( spFromAddress->GetMember()->m_ConnectInfo );

	//�f�[�^��M��
	s32 receiveSize = 0;
	for ( u32 i = 0; i < tryNum ; i++ )
	{
		//��M����
		receiveSize = recvfrom( m_BroadCastSoketHndl , ( char * )pData , size , 0 , ( struct sockaddr * )&spFromAddress->GetMember()->m_ConnectInfo, &fromLen );

		if ( receiveSize < 0 && WSAEWOULDBLOCK != WSAGetLastError() )
		{
			PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "recv failed. \n" ) ) )
		} 
		else if ( ( u32 )receiveSize > size )
		{
			PANIC_THROW_EXCEPTION( CSoketAppException( eSoketAppErrorIdReceiveBufferOver , _T( "ReceiveBuffer Over." ) ) )
		}
		else
		{
			//���ł���M������������Ԃ��Ă����i�p�P�b�g���B���������Ă�\��������̂ŋ��ʋ@�\�Ńf�[�^���}�[�W���邱�Ƃ͂ł��Ȃ��j
			break; 
		}
	}

	return receiveSize;
}

void CUDPSoket::OpenMultiCast( u16 portNo , const char *address , char *netInterfaceAdr , s32 ttlNum )
{
	ASSERT_THROW_EXCEPTION( m_IsOpenMultiCast == skyFalse , CSoketAppException( eSoketAppErrorIdSoketAlreadyOpen , _T( "Open already exec." ) ) );

	//UDP���M�p�ڑ����\�z
	m_spMultiCastSoketAddress = CSoketAddressPtr( NEW CSoketAddress( AF_INET , portNo , address , skyFalse ) );

	//UDP�N���C�A���g�p�\�P�b�g�I�[�v��
	this->CSoket::Open( m_MultiCastSoketHndl , AF_INET , SOCK_DGRAM , 0 );

	//�}���`�L���X�g�p�̃\�P�b�g�I�v�V������ݒ肷��i�������ʏ��send�ƈႤ�j
	struct ip_mreq stMreq;

	//�}���`�L���X�g�����o�o�^
	stMreq.imr_multiaddr.s_addr = m_spMultiCastSoketAddress->GetMember()->m_ConnectInfo.sin_addr.s_addr;
	stMreq.imr_interface.s_addr = EndianUtil::DataConvert_HostToNetWork( INADDR_ANY );
	int n = setsockopt( m_MultiCastSoketHndl , IPPROTO_IP , IP_ADD_MEMBERSHIP , ( char * )&stMreq, sizeof( stMreq ) ); // �����o�V�b�v

	if ( n != 0 )
	{
		PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "setsockopt failed." ) ) )
	}

	DWORD ipaddr = inet_addr( netInterfaceAdr );
	n = setsockopt( m_MultiCastSoketHndl , IPPROTO_IP , IP_MULTICAST_IF , ( char * )&ipaddr, sizeof( ipaddr ) );

	if ( n != 0 )
	{
		PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "setsockopt failed." ) ) )
	}

	//TTL�̒l��ݒ肷��
	int ttl = ttlNum;
	n = setsockopt( m_MultiCastSoketHndl , IPPROTO_IP , IP_MULTICAST_TTL , ( char * )&ttl, sizeof( ttl ) );

	if ( n != 0 )
	{
		PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "setsockopt failed." ) ) )
	}

	m_IsOpenMultiCast = skyTrue;
}

void CUDPSoket::CloseMultiCast()
{
	ASSERT_THROW_EXCEPTION( m_IsOpenMultiCast == skyTrue , CSoketAppException( eSoketAppErrorIdSoketNotOpen , _T( "Open not exec yet." ) ) );

	//�\�P�b�g�N���[�Y
	this->CSoket::Close( m_MultiCastSoketHndl );

	m_IsOpenMultiCast = skyFalse;
}

u32  CUDPSoket::SendMultiCast( const void* pData , u32 size )
{
	ASSERT_THROW_EXCEPTION( m_IsOpenMultiCast == skyTrue , CSoketAppException( eSoketAppErrorIdSoketNotOpen , _T( "Open not exec yet." ) ) );

	int n = sendto( m_MultiCastSoketHndl , ( const char * )pData , size , 0 , ( struct sockaddr * )&m_spMultiCastSoketAddress->GetMember()->m_ConnectInfo, sizeof( m_spMultiCastSoketAddress->GetMember()->m_ConnectInfo ) );

	//�G���[�`�F�b�N
	if ( n < 1 )
	{
		PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "sendto failed." ) ) )
	}

	return ( u32 )n;
}

u32  CUDPSoket::ReceiveMultiCast( void* pData , u32 size , const CSoketAddressPtr &spFromAddress , u32 tryNum )
{
	ASSERT_THROW_EXCEPTION( m_IsOpenMultiCast == skyTrue , CSoketAppException( eSoketAppErrorIdSoketNotOpen , _T( "Open not exec yet." ) ) );

	//���M�����
	int fromLen = sizeof( spFromAddress->GetMember()->m_ConnectInfo );

	//�f�[�^��M��
	s32 receiveSize = 0;
	for ( u32 i = 0; i < tryNum ; i++ )
	{
		//��M����
		receiveSize = recvfrom( m_MultiCastSoketHndl , ( char * )pData , size , 0 , ( struct sockaddr * )&spFromAddress->GetMember()->m_ConnectInfo, &fromLen );

		if ( receiveSize < 0 && WSAEWOULDBLOCK != WSAGetLastError() )
		{
			PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "recv failed. \n" ) ) )
		} 
		else if ( ( u32 )receiveSize > size )
		{
			PANIC_THROW_EXCEPTION( CSoketAppException( eSoketAppErrorIdReceiveBufferOver , _T( "ReceiveBuffer Over." ) ) )
		}
		else
		{
			//���ł���M������������Ԃ��Ă����i�p�P�b�g���B���������Ă�\��������̂ŋ��ʋ@�\�Ńf�[�^���}�[�W���邱�Ƃ͂ł��Ȃ��j
			break; 
		}
	}

	return receiveSize;
}

} } }
