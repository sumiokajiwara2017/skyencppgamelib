#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_NETWORK

#if ON_

using namespace sky::lib;

#include "SkyBaseLib/NetWork.h"

#if ON_

const char *pSendData = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";

TEST( net , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();
	char receiveBuffer[ 256 ];

	try
	{
		net::CTCPSoket tcpSoket( 555 , "127.0.0.1" );

		tcpSoket.Connect();
		tcpSoket.Send( pSendData , CStringUtil_::LenByte( pSendData ) );
		CMemoryUtil_::ClearZero( receiveBuffer , 256 );
		tcpSoket.Receive( receiveBuffer , array_sizeof( receiveBuffer ) );
		SKY_PRINTF( "%s \n" , receiveBuffer );
		tcpSoket.DisConnect();

		tcpSoket.Connect();
		tcpSoket.Send( "SendClient2" , 11 );
		CMemoryUtil_::ClearZero( receiveBuffer , 256 );
		tcpSoket.Receive( receiveBuffer , array_sizeof( receiveBuffer ) );
		SKY_PRINTF( "%s \n" , receiveBuffer );
		tcpSoket.DisConnect();

		tcpSoket.Connect();
		tcpSoket.Send( "SendClient3" , 11 );
		CMemoryUtil_::ClearZero( receiveBuffer , 256 );
		tcpSoket.Receive( receiveBuffer , array_sizeof( receiveBuffer ) );
		SKY_PRINTF( "%s \n" , receiveBuffer );
		tcpSoket.DisConnect();

		tcpSoket.Connect();
		tcpSoket.Send( "SendClient4" , 11 );
		CMemoryUtil_::ClearZero( receiveBuffer , 256 );
		tcpSoket.Receive( receiveBuffer , array_sizeof( receiveBuffer ) );
		SKY_PRINTF( "%s \n" , receiveBuffer );
		tcpSoket.DisConnect();

		tcpSoket.Connect();
		tcpSoket.Send( "SendClient5" , 11 );
		CMemoryUtil_::ClearZero( receiveBuffer , 256 );
		tcpSoket.Receive( receiveBuffer , array_sizeof( receiveBuffer ) );
		SKY_PRINTF( "%s \n" , receiveBuffer );
		tcpSoket.DisConnect();

		tcpSoket.Connect();
		tcpSoket.Send( "SendClient6" , 11 );
		CMemoryUtil_::ClearZero( receiveBuffer , 256 );
		tcpSoket.Receive( receiveBuffer , array_sizeof( receiveBuffer ) );
		SKY_PRINTF( "%s \n" , receiveBuffer );
		tcpSoket.DisConnect();

		tcpSoket.Connect();
		tcpSoket.Send( "SendClient7" , 11 );
		CMemoryUtil_::ClearZero( receiveBuffer , 256 );
		tcpSoket.Receive( receiveBuffer , array_sizeof( receiveBuffer ) );
		SKY_PRINTF( "%s \n" , receiveBuffer );
		tcpSoket.DisConnect();

		tcpSoket.Connect();
		tcpSoket.Send( "SendClient8" , 11 );
		CMemoryUtil_::ClearZero( receiveBuffer , 256 );
		tcpSoket.Receive( receiveBuffer , array_sizeof( receiveBuffer ) );
		SKY_PRINTF( "%s \n" , receiveBuffer );
		tcpSoket.Send( "SendClient9" , 11 );
		CMemoryUtil_::ClearZero( receiveBuffer , 256 );
		tcpSoket.Receive( receiveBuffer , array_sizeof( receiveBuffer ) );
		SKY_PRINTF( "%s \n" , receiveBuffer );
		tcpSoket.Send( "SendClient10" , 12 );
		CMemoryUtil_::ClearZero( receiveBuffer , 256 );
		tcpSoket.Receive( receiveBuffer , array_sizeof( receiveBuffer ) );
		SKY_PRINTF( "%s \n" , receiveBuffer );
		tcpSoket.Send( "SendClient11" , 12 );
		CMemoryUtil_::ClearZero( receiveBuffer , 256 );
		tcpSoket.Receive( receiveBuffer , array_sizeof( receiveBuffer ) );
		SKY_PRINTF( "%s \n" , receiveBuffer );
		tcpSoket.DisConnect();
	}
	catch( net::CSoketException &e )
	{
		e.Print();
	}
	catch( net::CSoketAppException &e )
	{
		e.Print();
	}

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#if OFF_

TEST( net , 2 ) 
{
	SKY_LIB_GRAPHIC_INIT();
	char receiveBuffer[ 256 ];

	try
	{
		//相手が受信者として起動していないとこの処理はだめになる。テストなのでブレークでなんとかする
		net::CUDPSoket sender;
		sender.Open( 5555 , "127.0.0.1" );

		sender.Send( "SendClient1" , 11 );
		CMemoryUtil_::ClearZero( receiveBuffer , 256 );
		sender.Receive( receiveBuffer , array_sizeof( receiveBuffer ) );
		SKY_PRINTF( "%s \n" , receiveBuffer );

		sender.Send( "SendClient2" , 11 );
		CMemoryUtil_::ClearZero( receiveBuffer , 256 );
		sender.Receive( receiveBuffer , array_sizeof( receiveBuffer ) );
		SKY_PRINTF( "%s \n" , receiveBuffer );

		sender.Send( "SendClient3" , 11 );
		CMemoryUtil_::ClearZero( receiveBuffer , 256 );
		sender.Receive( receiveBuffer , array_sizeof( receiveBuffer ) );
		SKY_PRINTF( "%s \n" , receiveBuffer );

		sender.Close();
	}
	catch( net::CSoketException &e )
	{
		e.Print();
	}
	catch( net::CSoketAppException &e )
	{
		e.Print();
	}

	SKY_LIB_GRAPHIC_TERM();
}

#endif


#if OFF_

TEST( net , 3 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	net::CSoketAddressPtr from( NEW net::CSoketAddress() );
	char receiveBuffer[ 256 ];

	try
	{
		//相手が受信者として起動していないとこの処理はだめになる。テストなのでブレークでなんとかする
		net::CUDPSoket sender;
		sender.OpenBroadCast( 5555 , "255.255.255.255" );

		sender.SendBroadCast( "SendClient1" , 11 );
		CMemoryUtil_::ClearZero( receiveBuffer , 256 );
		sender.ReceiveBroadCast( receiveBuffer , array_sizeof( receiveBuffer ) , from );
		SKY_PRINTF( "%s \n" , receiveBuffer );

		sender.Open( from );
		sender.Send( "GOOD!!" , 6 );
		CMemoryUtil_::ClearZero( receiveBuffer , 256 );
		sender.Receive( receiveBuffer , array_sizeof( receiveBuffer ) );
		SKY_PRINTF( "%s \n" , receiveBuffer );
		sender.Close();

		sender.SendBroadCast( "SendClient2" , 11 );
		CMemoryUtil_::ClearZero( receiveBuffer , 256 );
		sender.ReceiveBroadCast( receiveBuffer , array_sizeof( receiveBuffer ) , from );
		SKY_PRINTF( "%s \n" , receiveBuffer );

		sender.SendBroadCast( "SendClient3" , 11 );
		CMemoryUtil_::ClearZero( receiveBuffer , 256 );
		sender.ReceiveBroadCast( receiveBuffer , array_sizeof( receiveBuffer ) , from );
		SKY_PRINTF( "%s \n" , receiveBuffer );

		sender.CloseBroadCast();
	}
	catch( net::CSoketException &e )
	{
		e.Print();
	}
	catch( net::CSoketAppException &e )
	{
		e.Print();
	}

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#if OFF_

TEST( net , 4 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	net::CSoketAddressPtr from( NEW net::CSoketAddress() );
	char receiveBuffer[ 256 ];

	try
	{
		//相手が受信者として起動していないとこの処理はだめになる。テストなのでブレークでなんとかする
		net::CUDPSoket sender;
		sender.OpenMultiCast( 5555 , "239.192.1.2" , "192.168.13.2" , 10 );

		sender.SendMultiCast( "SendClient1" , 11 );
		CMemoryUtil_::ClearZero( receiveBuffer , 256 );
		sender.ReceiveMultiCast( receiveBuffer , array_sizeof( receiveBuffer ) , from );
		SKY_PRINTF( "%s \n" , receiveBuffer );

		sender.Open( from );
		sender.Send( "GOOD!!" , 6 );
		CMemoryUtil_::ClearZero( receiveBuffer , 256 );
		sender.Receive( receiveBuffer , array_sizeof( receiveBuffer ) );
		SKY_PRINTF( "%s \n" , receiveBuffer );
		sender.Close();

		sender.SendMultiCast( "SendClient2" , 11 );
		CMemoryUtil_::ClearZero( receiveBuffer , 256 );
		sender.ReceiveMultiCast( receiveBuffer , array_sizeof( receiveBuffer ) , from );
		SKY_PRINTF( "%s \n" , receiveBuffer );

		sender.SendMultiCast( "SendClient3" , 11 );
		CMemoryUtil_::ClearZero( receiveBuffer , 256 );
		sender.ReceiveMultiCast( receiveBuffer , array_sizeof( receiveBuffer ) , from );
		SKY_PRINTF( "%s \n" , receiveBuffer );

		sender.CloseMultiCast();
	}
	catch( net::CSoketException &e )
	{
		e.Print();
	}
	catch( net::CSoketAppException &e )
	{
		e.Print();
	}

	SKY_LIB_GRAPHIC_TERM();
}

#endif


#endif

#endif