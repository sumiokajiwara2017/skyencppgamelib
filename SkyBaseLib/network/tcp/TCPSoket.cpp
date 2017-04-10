#include "StdAfx.h"
#include "SkyBaseLib/NetWork.h"

namespace sky { namespace lib { namespace net {

#ifdef SKYLIB_PLATFORM_WIN32

CTCPSoket::CTCPSoket( u16 portNo , const char *address , u32 receiveTempBufSize )
{
	Constractor( receiveTempBufSize );

	//TCP�T�[�o�[�p�ڑ����\�z
	m_spSoketAddress = CSoketAddressPtr( NEW CSoketAddress( AF_INET , portNo , address ) );
}

CTCPSoket::CTCPSoket( const CSoketAddressPtr &spAddress , u32 receiveTempBufSize )
{
	Constractor( receiveTempBufSize );

	//TCP�T�[�o�[�p�ڑ����\�z
	m_spSoketAddress = spAddress;
}

void CTCPSoket::Constractor( u32 receiveTempBufSize )
{
	m_IsConnect = skyFalse;

	//�ꎞ��M�o�b�t�@���쐬
	m_pTempReceiveBuffer = NEW char[ receiveTempBufSize ];
	m_TempReceiveBufferSize = receiveTempBufSize;
}

CTCPSoket::~CTCPSoket()
{
	DEL_ARRAY m_pTempReceiveBuffer;

	if ( m_IsConnect == skyTrue )
	{
		DisConnect();
	}
}

void CTCPSoket::Connect()
{
	ASSERT_THROW_EXCEPTION( m_IsConnect == skyFalse , CSoketAppException( eSoketAppErrorIdConnectAlreadyExec , _T( "Coneect already exec." ) ) );

	//TCP�T�[�o�[�ڑ��p�\�P�b�g�I�[�v��
	this->CSoket::Open( m_SoketHndl , AF_INET , SOCK_STREAM , 0 );

	//�ڑ�
	this->CSoket::Connect( m_SoketHndl , m_spSoketAddress );

	m_IsConnect = skyTrue;
}

void CTCPSoket::DisConnect()
{//�����ŃT�[�o�[�����G���[����ꍇ�A�T�[�o�[���������f�[�^���擾�����Ƀ\�P�b�g���J�����悤�Ƃ��Ă��܂����\��������B
 //�A�v���P�[�V�����̓T�[�o�[�̉���������Ƒz�肳���ꍇ�͕K�������f�[�^�����ׂĎ󂯎���Ă���J�����邱�ƁB

	ASSERT_THROW_EXCEPTION( m_IsConnect == skyTrue , CSoketAppException( eSoketAppErrorIdConnectNotExec , _T( "Connect not exec yet." ) ) );

	//�ڑ�����
	this->CSoket::DisConnect( m_SoketHndl , 2 );

	//�\�P�b�g�N���[�Y
	this->CSoket::Close( m_SoketHndl );

	m_IsConnect = skyFalse;
}

u32 CTCPSoket::Send( const void* pData , u32 size )
{
	ASSERT_THROW_EXCEPTION( m_IsConnect == skyTrue , CSoketAppException( eSoketAppErrorIdConnectNotExec , _T( "Coneect not exec." ) ) );

	//���M
	int n = send( m_SoketHndl , ( const char * )pData , size , 0 );

	//�G���[�`�F�b�N
	if ( n < 1 )
	{
		PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "send failed." ) ) )
	}

	return ( u32 )n;
}

u32 CTCPSoket::Receive( void* pBuf , u32 size )
{
	ASSERT_THROW_EXCEPTION( m_IsConnect == skyTrue , CSoketAppException( eSoketAppErrorIdConnectNotExec , _T( "Coneect not exec." ) ) );

	//�f�[�^��M��
	skyBool isReceiveExec = skyTrue;
	s32 receiveSize = 0;
	while( isReceiveExec )
	{
		//��M����
		CMemoryUtil_::ClearZero( m_pTempReceiveBuffer , m_TempReceiveBufferSize );
		receiveSize = recv( m_SoketHndl , m_pTempReceiveBuffer , m_TempReceiveBufferSize , 0 );

		if ( receiveSize < 0 )
		{
			PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "recv failed." ) ) )
		}
		//�f�[�^���Ƃꂽ
		else if ( receiveSize > 0 )
		{
			if ( ( u32 )receiveSize > size )
			{
				PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "Receive buffer size orver." ) ) )
			}
			else
			{
				CMemoryUtil_::Copy( pBuf , m_pTempReceiveBuffer , receiveSize );
			}

			//�f�[�^�擾�I��( TCP�̏ꍇ�P��ŕK���K�v�ȃf�[�^�����ׂē͂��H�j
			isReceiveExec = skyFalse;
		}
	}

	return receiveSize;
}

socket_hndl CTCPSoket::GetHndl()
{
	return m_SoketHndl;
}

#endif

} } }
