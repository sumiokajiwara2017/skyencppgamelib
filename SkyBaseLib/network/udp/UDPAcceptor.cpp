#include "StdAfx.h"
#include "SkyBaseLib/NetWork.h"

namespace sky { namespace lib { namespace net {

CUDPAcceptor::CUDPAcceptor( u16 portNo , u32 receiveTempBufSize ) :
m_ReplySendBuffer( _T( "sendBuffer" ) , 32 , skyTrue , skyFalse , skyTrue )
{
	Constractor( receiveTempBufSize );

	//UDP受信用ソケットオープン
	Open( m_SoketHndl , AF_INET , SOCK_DGRAM , 0 );

	//UDP受信用接続情報構築
	m_spSoketAddress.SetPtr( NEW CSoketAddress( AF_INET , portNo ) );

	//バインド
	Bind( m_SoketHndl , m_spSoketAddress );

	// ノンブロッキングに設定する。recv関数で停止はしない。
	// recv結果がエラーでWSAWOULDBLOCKの場合は単にデータが無かったと判定できる
	u_long val = 1;
	ioctlsocket( m_SoketHndl , FIONBIO , &val );
}

CUDPAcceptor::CUDPAcceptor( u16 portNo , const char *address , u32 receiveTempBufSize ) :
m_ReplySendBuffer( _T( "sendBuffer" ) , 32 , skyTrue , skyFalse , skyTrue )
{
	Constractor( receiveTempBufSize );

	//UDP受信用ソケットオープン
	Open( m_SoketHndl , AF_INET , SOCK_DGRAM , 0 );

	//UDP受信用接続情報構築
	m_spSoketAddress.SetPtr( NEW CSoketAddress( AF_INET , portNo ) );

	//バインド
	Bind( m_SoketHndl , m_spSoketAddress );

	//マルチキャスト受信用にパケットオプションを設定する
	struct ip_mreq mreq;
	CMemoryUtil_::ClearZero( &mreq, sizeof( mreq ) );
    mreq.imr_interface.S_un.S_addr = EndianUtil::DataConvert_HostToNetWork( INADDR_ANY );
    mreq.imr_multiaddr.S_un.S_addr = inet_addr( address );
	int n = setsockopt( m_SoketHndl , IPPROTO_IP , IP_ADD_MEMBERSHIP , ( char * )&mreq, sizeof( mreq ) );

	if ( n != 0 )
	{
		PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "setsockopt failed." ) ) )
	}

	// ノンブロッキングに設定する。recv関数で停止はしない。
	// recv結果がエラーでWSAWOULDBLOCKの場合は単にデータが無かったと判定できる
	u_long val = 1;
	ioctlsocket( m_SoketHndl , FIONBIO , &val );
}

void CUDPAcceptor::Constractor( u32 receiveTempBufSize )
{
	//一時受信バッファを作成
	m_pTempReceiveBuffer = NEW char[ receiveTempBufSize ];
	m_TempReceiveBufferSize = receiveTempBufSize;
}

void CUDPAcceptor::Accept()
{
	//受信中状態にする
	m_eState = eState_Accept;

	//受信待ち
	while( m_eState == eState_Accept || m_eState == eState_AcceptEndRequest )
	{
		//もし受信終了リクエストが着ていたら送信を遮断する
		if ( m_eState == eState_AcceptEndRequest )
		{
			//新しい送信を遮断
			DisConnect( m_SoketHndl , 1 );
		}

		//送信元情報
		struct sockaddr_in fromAddr;
		int fromLen = sizeof( fromAddr );

		//データ受信中
		s32 receiveSize = 0;
		memset( m_pTempReceiveBuffer , 0 , m_TempReceiveBufferSize );

		//受信処理
		receiveSize = recvfrom( m_SoketHndl , m_pTempReceiveBuffer , m_TempReceiveBufferSize , 0 , ( struct sockaddr * )&fromAddr, &fromLen ); //ノンブロッキング

		if ( receiveSize < 0 && WSAEWOULDBLOCK != WSAGetLastError() )
		{
			PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "recv failed. \n" ) ) )
		} 
		else if ( receiveSize > 0 )
		{
			//受信イベント発生
			m_ReceiveEventHndl( CommonSmartPointer() , delegate::CDelegateArgs( ( memSize )m_pTempReceiveBuffer , ( memSize )receiveSize ) );

			//もし返信要求があれば送信する
			if ( m_ReplySendBuffer.vGetSeek() > 0 )
			{
				//送信要求がある場合は送信
				int n = sendto( m_SoketHndl , ( const char * )m_ReplySendBuffer.vGetData() , m_ReplySendBuffer.vGetSize() , 0 , ( struct sockaddr * )&fromAddr , fromLen );

				//エラーチェック
				if ( n < 1 )
				{
					PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "sendto failed." ) ) )
				}

				//送信バッファクリア
				m_ReplySendBuffer.vCreate( 1 );
			}
		}

		//もし受信終了リクエストが着ていたら送信を遮断する
		if ( m_eState == eState_AcceptEndRequest )
		{
			//新しい受信を遮断
			DisConnect( m_SoketHndl , 0 );

			//終了
			m_eState = eState_AcceptEnd;
		}
	}

	//接続は解除する
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
