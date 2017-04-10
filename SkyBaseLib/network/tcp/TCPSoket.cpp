#include "StdAfx.h"
#include "SkyBaseLib/NetWork.h"

namespace sky { namespace lib { namespace net {

#ifdef SKYLIB_PLATFORM_WIN32

CTCPSoket::CTCPSoket( u16 portNo , const char *address , u32 receiveTempBufSize )
{
	Constractor( receiveTempBufSize );

	//TCPサーバー用接続情報構築
	m_spSoketAddress = CSoketAddressPtr( NEW CSoketAddress( AF_INET , portNo , address ) );
}

CTCPSoket::CTCPSoket( const CSoketAddressPtr &spAddress , u32 receiveTempBufSize )
{
	Constractor( receiveTempBufSize );

	//TCPサーバー用接続情報構築
	m_spSoketAddress = spAddress;
}

void CTCPSoket::Constractor( u32 receiveTempBufSize )
{
	m_IsConnect = skyFalse;

	//一時受信バッファを作成
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

	//TCPサーバー接続用ソケットオープン
	this->CSoket::Open( m_SoketHndl , AF_INET , SOCK_STREAM , 0 );

	//接続
	this->CSoket::Connect( m_SoketHndl , m_spSoketAddress );

	m_IsConnect = skyTrue;
}

void CTCPSoket::DisConnect()
{//ここでサーバー側がエラーする場合、サーバーが送ったデータを取得せずにソケットを開放しようとしてしまった可能性がある。
 //アプリケーションはサーバーの応答があると想定される場合は必ず応答データをすべて受け取ってから開放すること。

	ASSERT_THROW_EXCEPTION( m_IsConnect == skyTrue , CSoketAppException( eSoketAppErrorIdConnectNotExec , _T( "Connect not exec yet." ) ) );

	//接続解除
	this->CSoket::DisConnect( m_SoketHndl , 2 );

	//ソケットクローズ
	this->CSoket::Close( m_SoketHndl );

	m_IsConnect = skyFalse;
}

u32 CTCPSoket::Send( const void* pData , u32 size )
{
	ASSERT_THROW_EXCEPTION( m_IsConnect == skyTrue , CSoketAppException( eSoketAppErrorIdConnectNotExec , _T( "Coneect not exec." ) ) );

	//送信
	int n = send( m_SoketHndl , ( const char * )pData , size , 0 );

	//エラーチェック
	if ( n < 1 )
	{
		PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "send failed." ) ) )
	}

	return ( u32 )n;
}

u32 CTCPSoket::Receive( void* pBuf , u32 size )
{
	ASSERT_THROW_EXCEPTION( m_IsConnect == skyTrue , CSoketAppException( eSoketAppErrorIdConnectNotExec , _T( "Coneect not exec." ) ) );

	//データ受信中
	skyBool isReceiveExec = skyTrue;
	s32 receiveSize = 0;
	while( isReceiveExec )
	{
		//受信処理
		CMemoryUtil_::ClearZero( m_pTempReceiveBuffer , m_TempReceiveBufferSize );
		receiveSize = recv( m_SoketHndl , m_pTempReceiveBuffer , m_TempReceiveBufferSize , 0 );

		if ( receiveSize < 0 )
		{
			PANIC_THROW_EXCEPTION( CSoketException( WSAGetLastError() , _T( "recv failed." ) ) )
		}
		//データがとれた
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

			//データ取得終了( TCPの場合１回で必ず必要なデータがすべて届く？）
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
