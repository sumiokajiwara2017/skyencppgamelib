#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_NETWORK

using namespace sky::lib;

#include "SkyBaseLib/NetWork.h"

#if OFF_

TEST( network , 1 ) 
{
	SKY_BASE_LIB_INIT();

	net::CResolver resolver;

	resolver.Lookup( "127.0.0.1" );
	resolver.Print();

	resolver.Lookup( "yahoo.co.jp" );
	resolver.Print();

	SKY_PRINTF( _T( "[%s]" ) , ( const skyString * )resolver.GetAddressList()[ 0 ] );

	SKY_BASE_LIB_TERM();
}

#endif

#if OFF_

TEST( network , 2 ) 
{
	SKY_BASE_LIB_INIT();
	
	net::CSoketAddress soketAddress( AF_INET , 999 , ( u8 * )"127.0.0.1" );

	SKY_BASE_LIB_TERM();
}

#endif

#if OFF_

static net::CTCPAcceptor *server = skyNull;

static void ReceiveEventDelegate( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args )
{
#define PRINTF_BUF ( 1024 )
	skyMString printBuf[ PRINTF_BUF ];

	if( args.GetMemSize( 1 ) > 0 )
	{
		//�o�͗p�o�b�t�@���쐬
		CStringUtil_::CopyAdde0( printBuf , PRINTF_BUF , ( const skyMString * )args.GetMemSize( 0 ) , args.GetMemSize( 1 ) );

		SKY_PRINTF( "server get!! [ %s ] \n" , printBuf );


		server->ReplySendRequest( ( u8 * )"OK!" , 3 );
	}
}

TEST( network , 3 ) 
{
	SKY_BASE_LIB_INIT();

	try
	{
		//�T�[�o�[�쐬
		server = NEW net::CTCPAcceptor( 555 );

		//�C�x���g�o�^
		server->ReceiveEventHndl() += delegate::IDelegateObjectPtr( NEW delegate::CEventDelegateAdaptorFunc( ReceiveEventDelegate ) );

		//��M�҂�
		server->Accept();
	}
	catch( net::CSoketException &e )
	{
		e.Print();
	}
	catch( net::CSoketAppException &e )
	{
		e.Print();
	}
	
	SKY_BASE_LIB_TERM();
}

#endif

#if OFF_

net::CUDPAcceptor *pAcceptor;

static void ReceiveEventDelegate( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args )
{
#define PRINTF_BUF ( 1024 )
	skyMString printBuf[ PRINTF_BUF ];

	if( args.GetMemSize( 1 ) > 0 )
	{
		//�o�͗p�o�b�t�@���쐬
		CStringUtil_::CopyAdde0( printBuf , PRINTF_BUF , ( const skyMString * )args.GetMemSize( 0 ) , args.GetMemSize( 1 ) );

		SKY_PRINTF( "receiver get!! [ %s ] \n" , printBuf );

		pAcceptor->ReplySendRequest( "OK!!" , 4 );
	}
}

TEST( network , 4 ) 
{
	SKY_BASE_LIB_INIT();

	try
	{
		//��M�҂Ƃ��ċN��
		pAcceptor = NEW net::CUDPAcceptor( 5555 );

		//��M�C�x���g�o�^
		pAcceptor->ReceiveEventHndl() += delegate::IDelegateObjectPtr( NEW delegate::CEventDelegateAdaptorFunc( ReceiveEventDelegate ) );

		//��M�҂�
		pAcceptor->Accept();
	}
	catch( net::CSoketException &e )
	{
		e.Print();
	}
	catch( net::CSoketAppException &e )
	{
		e.Print();
	}
	
	SKY_BASE_LIB_TERM();
}

#endif

#if OFF_

net::CUDPAcceptor *pAcceptor;

static void ReceiveEventDelegate( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args )
{
#define PRINTF_BUF ( 1024 )
	skyMString printBuf[ PRINTF_BUF ];

	if( args.GetMemSize( 1 ) > 0 )
	{
		//�o�͗p�o�b�t�@���쐬
		CStringUtil_::CopyAdde0( printBuf , PRINTF_BUF , ( const skyMString * )args.GetMemSize( 0 ) , args.GetMemSize( 1 ) );

		SKY_PRINTF( "receiver get!! [ %s ] \n" , printBuf );

		pAcceptor->ReplySendRequest( "OK!!" , 4 );
	}
}

TEST( network , 5 ) 
{
	SKY_BASE_LIB_INIT();

	try
	{
		//��M�҂Ƃ��ċN��
		pAcceptor = NEW net::CUDPAcceptor( 5555 , "239.192.1.2" );

		//��M�C�x���g�o�^
		pAcceptor->ReceiveEventHndl() += delegate::IDelegateObjectPtr( NEW delegate::CEventDelegateAdaptorFunc( ReceiveEventDelegate ) );

		//��M�҂�
		pAcceptor->Accept();
	}
	catch( net::CSoketException &e )
	{
		e.Print();
	}
	catch( net::CSoketAppException &e )
	{
		e.Print();
	}
	
	SKY_BASE_LIB_TERM();
}

#endif

#if OFF_

TEST( network , 6 ) 
{
	SKY_BASE_LIB_INIT();

	net::CNetWorkInterfaceInfoList interfaceInfoList;

	net::NetWorkInfoUtilt::GetNetWorkInterfaceInfo( interfaceInfoList );

	FOREACH( net::CNetWorkInterfaceInfoList , it , interfaceInfoList )
	{
		( *it ).Print();
	}

	net::CHostDNSInfo hostDnsInfo;
	net::NetWorkInfoUtilt::GetDNSInfo( hostDnsInfo );

	hostDnsInfo.Print();

	SKY_BASE_LIB_TERM();
}

#endif

#if ON_

TEST( network , 7 ) 
{
	SKY_BASE_LIB_INIT();

	//�|�[�g���J��
	net::CUPnPSender::OpnePort( _T( "testmapping" ) , "192.168.13.2" , 10000 );
	net::CUPnPSender::ClosePort( "192.168.13.2" , 10000 );

	SKY_BASE_LIB_TERM();
}

#endif


#endif