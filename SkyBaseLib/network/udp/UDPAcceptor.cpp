#include "StdAfx.h"
#include "SkyBaseLib/NetWork.h"

namespace sky { namespace lib { namespace net {

CUDPAcceptor::CUDPAcceptor( u16 portNo , u32 receiveTempBufSize ) :
m_ReplySendBuffer( _T( "sendBuffer" ) , 32 , skyTrue , skyFalse , skyTrue )
{
	Constractor( receiveTempBufSize );

	//UDP��M�p�\�P�b�g�I�[�v��
	Open( m_SoketHndl , AF_INET , SOCK_DGRAM , 0 );

	//UDP��M�p�ڑ����\�z
	m_spSoketAddress.SetPtr( NEW CSoketAddress( AF_INET , portNo ) );

	//�o�C���h
	Bind( m_SoketHndl , m_spSoketAddress );

	// �m���u���b�L���O�ɐݒ肷��Brecv�֐��Œ�~�͂��Ȃ��B
	// recv���ʂ��G���[��WSAWOULDBLOCK�̏ꍇ�͒P�Ƀf�[�^�����������Ɣ���ł���
	u_long val = 1;
	ioctlsocket( m_SoketHndl , FIONBIO , &val );
}

CUDPAcceptor::CUDPAcceptor( u16 portNo , const char *address , u32 receiveTempBufSize ) :
m_ReplySendBuffer( _T( "sendBuffer" ) , 32 , skyTrue , skyFalse , skyTrue )
{
	Constractor( receiveTempBufSize );

	//UDP��M�p�\�P�b�g�I�[�v��
	Open( m_SoketHndl , AF_INET , SOCK_DGRAM , 0 );

	//UDP��M�p�ڑ����\�z
	m_spSoketAddress.SetPtr( NEW CSoketAddress( AF_INET , portNo ) );

	//�o�C���h
	Bind( m_SoketHndl , m_spSoketAddress );

	//�}���`�L���X�g��M�p�Ƀp�P�b�g�I�v�V������ݒ肷��
	struct ip_mreq mreq;
	CMemoryUtil_::ClearZero( &mreq, sizeof( mreq ) );
    mreq.imr_interface.S_un.S_addr = EndianUtil::DataConvert_HostToNetWork( INADDR_ANY );
    mreq.imr_multiaddr.S_un.S_addr = inet_addr( address );
	int n = setsockopt( m_SoketHndl , IPPROTO_IP , IP_ADD_MEMBERSHIP , ( char * )&mreq, sizeof( mreq ) );

	if ( n != 0 )
	{
		PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "setsockopt failed." ) ) )
	}

	// �m���u���b�L���O�ɐݒ肷��Brecv�֐��Œ�~�͂��Ȃ��B
	// recv���ʂ��G���[��WSAWOULDBLOCK�̏ꍇ�͒P�Ƀf�[�^�����������Ɣ���ł���
	u_long val = 1;
	ioctlsocket( m_SoketHndl , FIONBIO , &val );
}

void CUDPAcceptor::Constractor( u32 receiveTempBufSize )
{
	//�ꎞ��M�o�b�t�@���쐬
	m_pTempReceiveBuffer = NEW char[ receiveTempBufSize ];
	m_TempReceiveBufferSize = receiveTempBufSize;
}

void CUDPAcceptor::Accept()
{
	//��M����Ԃɂ���
	m_eState = eState_Accept;

	//��M�҂�
	while( m_eState == eState_Accept || m_eState == eState_AcceptEndRequest )
	{
		//������M�I�����N�G�X�g�����Ă����瑗�M���Ւf����
		if ( m_eState == eState_AcceptEndRequest )
		{
			//�V�������M���Ւf
			DisConnect( m_SoketHndl , 1 );
		}

		//���M�����
		struct sockaddr_in fromAddr;
		int fromLen = sizeof( fromAddr );

		//�f�[�^��M��
		s32 receiveSize = 0;
		memset( m_pTempReceiveBuffer , 0 , m_TempReceiveBufferSize );

		//��M����
		receiveSize = recvfrom( m_SoketHndl , m_pTempReceiveBuffer , m_TempReceiveBufferSize , 0 , ( struct sockaddr * )&fromAddr, &fromLen ); //�m���u���b�L���O

		if ( receiveSize < 0 && WSAEWOULDBLOCK != WSAGetLastError() )
		{
			PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "recv failed. \n" ) ) )
		} 
		else if ( receiveSize > 0 )
		{
			//��M�C�x���g����
			m_ReceiveEventHndl( CommonSmartPointer() , delegate::CDelegateArgs( ( memSize )m_pTempReceiveBuffer , ( memSize )receiveSize ) );

			//�����ԐM�v��������Α��M����
			if ( m_ReplySendBuffer.vGetSeek() > 0 )
			{
				//���M�v��������ꍇ�͑��M
				int n = sendto( m_SoketHndl , ( const char * )m_ReplySendBuffer.vGetData() , m_ReplySendBuffer.vGetSize() , 0 , ( struct sockaddr * )&fromAddr , fromLen );

				//�G���[�`�F�b�N
				if ( n < 1 )
				{
					PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "sendto failed." ) ) )
				}

				//���M�o�b�t�@�N���A
				m_ReplySendBuffer.vCreate( 1 );
			}
		}

		//������M�I�����N�G�X�g�����Ă����瑗�M���Ւf����
		if ( m_eState == eState_AcceptEndRequest )
		{
			//�V������M���Ւf
			DisConnect( m_SoketHndl , 0 );

			//�I��
			m_eState = eState_AcceptEnd;
		}
	}

	//�ڑ��͉�������
	this->CSoket::Close( m_SoketHndl );
}

void CUDPAcceptor::ReplySendRequest( void *pData , u32 size )
{
	m_ReplySendBuffer.vWriteStream( ( u8 * )pData , size );
}

socket_hndl CUDPAcceptor::GetHndl()
{
	return m_SoketHndl;
}

CUDPAcceptor::~CUDPAcceptor()
{
	DEL_ARRAY m_pTempReceiveBuffer;
}

delegate::CDelegateHandler &CUDPAcceptor::ReceiveEventHndl()
{ 
	return m_ReceiveEventHndl;
}

void CTCPAcceptor::AcceptEndRequest()
{ 
	m_eState = eState_AcceptEndRequest;
}

skyBool CTCPAcceptor::IsAcceptEnd()
{ 
	return m_eState == eState_AcceptEnd;
}

} } }
