#pragma once

namespace sky { namespace lib { namespace net {

/// <summary>
/// ネットワークインターフェース情報
/// </summary>
struct CNetWorkInterfaceInfo
{
	u32             index;                    //ネットワーク機器に割り振られているインデックス番号
	string::CString m_Name;                   //識別名

	//アドレス情報（IPテーブル情報）
	string::CString m_IPAddressString;        //IPアドレス
	u32             m_IPAddress;              //IPアドレス（バイナリ）処理ではこっちが使われるはず
	string::CString m_NetMaskString;          //ネットマスク
	u32             m_NetMask;                //ネットマスク（バイナリ）処理ではこちらが使われるはず
	string::CString m_BrodCastAddressString;  //ブロードキャストアドレス
	u32             m_BrodCastAddress;        //ブロードキャストアドレス（バイナリ）処理ではこちらが使われるはず

	void Print()
	{
		SKY_PRINTF( _T ( " CNetWorkInterfaceInfo : index = [ %d ] name = [ %s ] IPAddress= [ %s ] NetMask= [ %s ] BrodCastAddress= [ %s ] PhysAddr= [ %s ] \n" ) , 
			index , 
			( const skyString * )m_Name , 
			( const skyString * )m_IPAddressString , 
			( const skyString * )m_NetMaskString , 
			( const skyString * )m_BrodCastAddressString );
	}
};
typedef CList< u32 , CNetWorkInterfaceInfo > CNetWorkInterfaceInfoList;

/// <summary>
/// ＤＮＳ情報
/// </summary>
struct CDNSInfo
{
	string::CString m_IPAddressString;             //IPアドレス
	u32             m_IPAddress;                   //IPアドレス（バイナリ）
	string::CString m_IPMaskString;                //IPマスク
	u32             m_IPMask;                      //IPマスク（バイナリ）

	void Print()
	{
		SKY_PRINTF( _T( "CDNSInfo : IPAddress = [ %s ] IPMask = [ %s ] \n" ) , ( const skyString * )m_IPAddressString , ( const skyString * )m_IPMaskString );
	}
};
typedef CList< u32 , CDNSInfo > CDNSInfoList;

/// <summary>
/// ホストに関係するＤＮＳ情報
/// </summary>
struct CHostDNSInfo
{
	string::CString m_HostName;                  //ホスト名
	string::CString m_DomainName;                //ホストが所属するドメイン名
	CDNSInfoList    m_DNSServerList;             //DNSサーバーリスト

	void Print()
	{
		SKY_PRINTF( _T( "CHostDNSInfo : HostName = [ %s ] DomainName = [ %s ] \n" ) , ( const skyString * )m_HostName , ( const skyString * )m_DomainName );

		FOREACH( CDNSInfoList , it , m_DNSServerList )
		{
			( *it ).Print();
		}
	}
};

/// <summary>
/// ネットワーク情報オブジェクト
/// </summary>
namespace NetWorkInfoUtilt
{
	/// <summary>
	/// ネットワークインターフェース情報を取得する
	/// </summary>
	void GetNetWorkInterfaceInfo( CNetWorkInterfaceInfoList &dest );

	/// <summary>
	/// DNS情報を取得する
	/// </summary>
	void GetDNSInfo( CHostDNSInfo &dest );
}

} } }