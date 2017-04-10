#include "StdAfx.h"
#include "SkyBaseLib/NetWork.h"

namespace sky { namespace lib { namespace net {

#ifdef SKYLIB_PLATFORM_WIN32

CTCPAcceptor::CTCPAcceptor( u16 portNo , u32 receiveTempBufSize , u32 connectCount ) :
m_ReplySendBuffer( _T( "sendBuffer" ) , 32 , skyTrue , skyFalse , skyTrue )
{
	//一時受信バッファを作成
	m_pTempReceiveBuffer = NEW char[ receiveTempBufSize ];
	m_TempReceiveBufferSize = receiveTempBufSize;

	//TCP待ち受け用ソケットオープン
	Open( m_SoketHndl , AF_INET , SOCK_STREAM , 0 );

	//TCP待ち受け用接続情報構築
	CSoketAddressPtr addressInfo( NEW CSoketAddress( AF_INET , portNo ) );

	//TCPサーバを終了して直後にもう一度起動したときに、 bindが「Address already in use」というようなエラーで失敗してしまうことへの対処。
	BOOL yes = 1;
	int n = setsockopt( m_SoketHndl , SOL_SOCKET, SO_REUSEADDR, ( const char * )&yes , sizeof( yes ) );

	if ( n != 0 )
	{
		PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "setsockopt failed. \n" ) ) )
	}

	//バインド
	Bind( m_SoketHndl , addressInfo );

	//コネクション数を指定して準備完了
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
	//受信中状態にする
	m_eState = eState_Accept;

	SOCKADDR_IN client;
	int len = 0;

	//受信待ち
	skyBool isReceiveExec = skyTrue;
	while( m_eState == eState_Accept || m_eState == eState_AcceptEndRequest )
	{
		//もし受信終了リクエストが着ていたら送信を遮断する
		if ( m_eState == eState_AcceptEndRequest )
		{
			//新しい送信を遮断
			DisConnect( m_SoketHndl , 1 );
		}

		len = sizeof( client );
		SOCKET sock = accept( m_SoketHndl , ( sockaddr * )&client, &len ); //接続がくるまでここで停止

		if ( sock == INVALID_SOCKET )
		{
			PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "accept is failed. \n" ) ) )
		}

		//データ受信中
		isReceiveExec = skyTrue;
		while( isReceiveExec )
		{
			//受信処理
			memset( m_pTempReceiveBuffer , 0 , m_TempReceiveBufferSize );
			s32 receiveSize = recv( sock , m_pTempReceiveBuffer , m_TempReceiveBufferSize , 0 ); //ブロッキング

			if ( receiveSize < 0 )
			{
				PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "recv failed. \n" ) ) )
			}
			//データがとれた
			else if ( receiveSize > 0 )
			{
				//受信イベント発生
				m_ReceiveEventHndl( CommonSmartPointer() , delegate::CDelegateArgs( ( memSize )m_pTempReceiveBuffer , ( memSize )receiveSize ) );

				//もし返信要求があれば送信する
				if ( m_ReplySendBuffer.vGetSeek() > 0 )
				{
					int n = send( sock , ( const char * )m_ReplySendBuffer.vGetData() , m_ReplySendBuffer.vGetSize() , 0 );

					//エラーチェック
					if ( n < 1 )
					{
						PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "send failed. \n" ) ) )
					}

					//送信バッファクリア
					m_ReplySendBuffer.vCreate( 1 );
				}
			}
			else
			{
				//データ取得終了( receiveSize == 0 は相手がソケットを閉じたことを意味する）
				isReceiveExec = skyFalse;
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

		//今回の接続は解除する
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
