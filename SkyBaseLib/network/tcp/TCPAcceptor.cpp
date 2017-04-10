#include "StdAfx.h"
#include "SkyBaseLib/NetWork.h"

namespace sky { namespace lib { namespace net {

#ifdef SKYLIB_PLATFORM_WIN32

CTCPAcceptor::CTCPAcceptor( u16 portNo , u32 receiveTempBufSize , u32 connectCount ) :
m_ReplySendBuffer( _T( "sendBuffer" ) , 32 , skyTrue , skyFalse , skyTrue )
{
	//�ꎞ��M�o�b�t�@���쐬
	m_pTempReceiveBuffer = NEW char[ receiveTempBufSize ];
	m_TempReceiveBufferSize = receiveTempBufSize;

	//TCP�҂��󂯗p�\�P�b�g�I�[�v��
	Open( m_SoketHndl , AF_INET , SOCK_STREAM , 0 );

	//TCP�҂��󂯗p�ڑ����\�z
	CSoketAddressPtr addressInfo( NEW CSoketAddress( AF_INET , portNo ) );

	//TCP�T�[�o���I�����Ē���ɂ�����x�N�������Ƃ��ɁA bind���uAddress already in use�v�Ƃ����悤�ȃG���[�Ŏ��s���Ă��܂����Ƃւ̑Ώ��B
	BOOL yes = 1;
	int n = setsockopt( m_SoketHndl , SOL_SOCKET, SO_REUSEADDR, ( const char * )&yes , sizeof( yes ) );

	if ( n != 0 )
	{
		PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "setsockopt failed. \n" ) ) )
	}

	//�o�C���h
	Bind( m_SoketHndl , addressInfo );

	//�R�l�N�V���������w�肵�ď�������
	n = listen( m_SoketHndl , connectCount );

	if ( n != 0 )
	{
		PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "listen failed." ) ) )
	}
}

CTCPAcceptor::~CTCPAcceptor()
{
	DEL_ARRAY m_pTempReceiveBuffer;
}

void CTCPAcceptor::Accept()
{
	//��M����Ԃɂ���
	m_eState = eState_Accept;

	SOCKADDR_IN client;
	int len = 0;

	//��M�҂�
	skyBool isReceiveExec = skyTrue;
	while( m_eState == eState_Accept || m_eState == eState_AcceptEndRequest )
	{
		//������M�I�����N�G�X�g�����Ă����瑗�M���Ւf����
		if ( m_eState == eState_AcceptEndRequest )
		{
			//�V�������M���Ւf
			DisConnect( m_SoketHndl , 1 );
		}

		len = sizeof( client );
		SOCKET sock = accept( m_SoketHndl , ( sockaddr * )&client, &len ); //�ڑ�������܂ł����Œ�~

		if ( sock == INVALID_SOCKET )
		{
			PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "accept is failed. \n" ) ) )
		}

		//�f�[�^��M��
		isReceiveExec = skyTrue;
		while( isReceiveExec )
		{
			//��M����
			memset( m_pTempReceiveBuffer , 0 , m_TempReceiveBufferSize );
			s32 receiveSize = recv( sock , m_pTempReceiveBuffer , m_TempReceiveBufferSize , 0 ); //�u���b�L���O

			if ( receiveSize < 0 )
			{
				PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "recv failed. \n" ) ) )
			}
			//�f�[�^���Ƃꂽ
			else if ( receiveSize > 0 )
			{
				//��M�C�x���g����
				m_ReceiveEventHndl( CommonSmartPointer() , delegate::CDelegateArgs( ( memSize )m_pTempReceiveBuffer , ( memSize )receiveSize ) );

				//�����ԐM�v��������Α��M����
				if ( m_ReplySendBuffer.vGetSeek() > 0 )
				{
					int n = send( sock , ( const char * )m_ReplySendBuffer.vGetData() , m_ReplySendBuffer.vGetSize() , 0 );

					//�G���[�`�F�b�N
					if ( n < 1 )
					{
						PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "send failed. \n" ) ) )
					}

					//���M�o�b�t�@�N���A
					m_ReplySendBuffer.vCreate( 1 );
				}
			}
			else
			{
				//�f�[�^�擾�I��( receiveSize == 0 �͑��肪�\�P�b�g��������Ƃ��Ӗ�����j
				isReceiveExec = skyFalse;
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

		//����̐ڑ��͉�������
		closesocket( sock );
	}
}

socket_hndl CTCPAcceptor::GetHndl()
{
	return m_SoketHndl;
}

void CTCPAcceptor::ReplySendRequest( void *pData , u32 size )
{
	m_ReplySendBuffer.vWriteStream( ( u8 * )pData , size );
}

delegate::CDelegateHandler &CTCPAcceptor::ReceiveEventHndl()
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

#endif

} } }
