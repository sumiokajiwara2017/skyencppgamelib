#include "StdAfx.h"
#include "SkyBaseLib/NetWork.h"

namespace sky { namespace lib { namespace net {

namespace NetWorkInfoUtilt
{

static CNetWorkInterfaceInfo *GetInterfaceInfoListItem( CNetWorkInterfaceInfoList &dest , u32 index )
{
	CNetWorkInterfaceInfo *pResult = skyNull;

	FOREACH( CNetWorkInterfaceInfoList , it , dest )
	{
		if ( ( *it ).index == index )
		{
			pResult =  &( *it );
		}
	}
	return pResult;
}

void GetNetWorkInterfaceInfo( CNetWorkInterfaceInfoList &dest )
{

//-----------まずインデックスと名前を取得

	PIP_INTERFACE_INFO pInfo = NULL;
	ULONG ulOutBufLen = 0;
	DWORD dwRetVal = 0;

	//変数 ulOutBufLen に必要なサイズを取得
	if ( GetInterfaceInfo( NULL , &ulOutBufLen ) == ERROR_INSUFFICIENT_BUFFER )
	{
		pInfo = ( IP_INTERFACE_INFO * ) malloc( ulOutBufLen );
	}

	// 実際にデータを取得する
	dwRetVal = GetInterfaceInfo( pInfo , &ulOutBufLen );

	if ( dwRetVal == NO_ERROR )
	{
		for ( int i = 0 ; i < pInfo->NumAdapters ; i++ )
		{
			CNetWorkInterfaceInfo &item = dest.AddTail();

			item.index = pInfo->Adapter[ i ].Index;
			item.m_Name = pInfo->Adapter[ i ].Name;
		}
	}
	else
	{
		LPVOID lpMsgBuf;
		
		if ( FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				dwRetVal,
				MAKELANGID(LANG_NEUTRAL,
						SUBLANG_DEFAULT ) , /* Default language */
				( LPTSTR ) &lpMsgBuf ,
				0 ,
				NULL ) )
		{
			SKY_PANIC_MSG( "GetInterfaceInfo failed. Error: %s", lpMsgBuf );
		}
		LocalFree( lpMsgBuf );
	}

	if ( pInfo != NULL )
	{
		free( pInfo );
	}

//------次にIPを取得

	DWORD dwSize = 0;
	PMIB_IPADDRTABLE pIpAddrTable = NULL;

	// GetIpAddrTable()で必要になるサイズを取得
	if (GetIpAddrTable(NULL, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER)
	{
		pIpAddrTable = (MIB_IPADDRTABLE *) malloc (dwSize);
	}

	// 実際にGetIpAddrTable()を使う
	if ( ( dwRetVal = GetIpAddrTable( pIpAddrTable , &dwSize , 0 ) ) == NO_ERROR)
	{
		if ( pIpAddrTable->dwNumEntries > 0 )
		{
			for( DWORD i = 1; i < pIpAddrTable->dwNumEntries ; i++ )
			{
				CNetWorkInterfaceInfo *item = GetInterfaceInfoListItem( dest , pIpAddrTable->table[ i ].dwIndex );
				if ( item != skyNull )
				{
					item->m_IPAddressString = ( const skyMString * )inet_ntoa( * ( struct in_addr * )&pIpAddrTable->table[ i ].dwAddr );
					item->m_IPAddress = pIpAddrTable->table[ i ].dwAddr;

					item->m_NetMaskString = ( const skyMString * )inet_ntoa( * ( struct in_addr * )&pIpAddrTable->table[ i ].dwMask );
					item->m_NetMask        = pIpAddrTable->table[ i ].dwMask;

					item->m_BrodCastAddressString = ( const skyMString * )inet_ntoa( * ( struct in_addr * )&pIpAddrTable->table[ i ].dwBCastAddr );
					item->m_BrodCastAddress = pIpAddrTable->table[ i ].dwBCastAddr;
				}
				else
				{
					CNetWorkInterfaceInfo &item = dest.AddTail();

					item.index = pIpAddrTable->table[ i ].dwIndex;

					item.m_IPAddressString = ( const skyMString * )inet_ntoa( * ( struct in_addr * )&pIpAddrTable->table[ i ].dwAddr );
					item.m_IPAddress = pIpAddrTable->table[ i ].dwAddr;

					item.m_NetMaskString = ( const skyMString * )inet_ntoa( * ( struct in_addr * )&pIpAddrTable->table[ i ].dwMask );
					item.m_NetMask        = pIpAddrTable->table[ i ].dwMask;

					item.m_BrodCastAddressString = ( const skyMString * )inet_ntoa( * ( struct in_addr * )&pIpAddrTable->table[ i ].dwBCastAddr );
					item.m_BrodCastAddress = pIpAddrTable->table[ i ].dwBCastAddr;
				}
			}
		}
	}
	else
	{//エラー
		LPVOID lpMsgBuf;
		if ( FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |
							FORMAT_MESSAGE_FROM_SYSTEM | 
							FORMAT_MESSAGE_IGNORE_INSERTS,
							NULL,
							dwRetVal,
							MAKELANGID(LANG_NEUTRAL,
									SUBLANG_DEFAULT), //Default language
							( LPTSTR )&lpMsgBuf,
							0,
							NULL ) )
		{
			SKY_PANIC_MSG( "GetIpAddrTable failed. \tError: %s", lpMsgBuf );
		}
		LocalFree( lpMsgBuf );
	}

	if ( pIpAddrTable != NULL )
	{
		free( pIpAddrTable );
	}
}

void GetDNSInfo( CHostDNSInfo &dest )
{
	FIXED_INFO *pFixedInfo = NULL;
	ULONG    ulOutBufLen = 0;
	DWORD    dwRetVal = 0;
	IP_ADDR_STRING *pIPAddr;
   
	if( GetNetworkParams( NULL , &ulOutBufLen ) == ERROR_BUFFER_OVERFLOW )
	{
		pFixedInfo = ( FIXED_INFO * )GlobalAlloc( GPTR , ulOutBufLen );
	}
	else
	{
		SKY_PANIC_MSG( "GetDNSInfo failed." );
	}

	if ( dwRetVal == GetNetworkParams( pFixedInfo, &ulOutBufLen ) )
	{
		SKY_PANIC_MSG( "GetNetworkParams() failed: %08x\n" , dwRetVal );
	}
	else
	{
		dest.m_HostName   = ( const skyMString * )pFixedInfo->HostName;
		dest.m_DomainName = ( const skyMString * )pFixedInfo->DomainName;

		CDNSInfo &dnsInfo = dest.m_DNSServerList.AddTail();
		dnsInfo.m_IPAddressString = ( const skyMString * )pFixedInfo->DnsServerList.IpAddress.String;
		dnsInfo.m_IPAddress       = inet_addr( ( const char * )pFixedInfo->DnsServerList.IpAddress.String );
		dnsInfo.m_IPMaskString    = ( const skyMString * )pFixedInfo->DnsServerList.IpMask.String;
		dnsInfo.m_IPMask          = inet_addr( ( const char * )pFixedInfo->DnsServerList.IpMask.String );
		pIPAddr = pFixedInfo->DnsServerList.Next;
		while ( pIPAddr )
		{
			dnsInfo = dest.m_DNSServerList.AddTail();
			dnsInfo.m_IPAddressString = ( const skyMString * )pIPAddr->IpAddress.String;
			dnsInfo.m_IPAddress       = inet_addr( ( const char * )pIPAddr->IpAddress.String );
			dnsInfo.m_IPMaskString    = ( const skyMString * )pFixedInfo->DnsServerList.IpMask.String;
			dnsInfo.m_IPMask          = inet_addr( ( const char * )pFixedInfo->DnsServerList.IpMask.String );
			pIPAddr = pIPAddr->Next;
		}
	}

	if ( pFixedInfo != NULL )
	{
		GlobalFree( pFixedInfo );
	}
}

}

} } }