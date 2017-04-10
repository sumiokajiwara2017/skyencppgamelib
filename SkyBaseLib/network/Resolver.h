#pragma once

namespace sky { namespace lib { namespace net {

//プラットフォーム毎のメンバ変数(pimpl)
struct  CResolverMember;

/// <summary>
/// IPアドレス、ホスト名から関連付けられるホスト名、エイリアス名、IPアドレスのすべてを取得する
/// </summary>
class CResolver : public base::SkyObject
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CResolver();

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CResolver();
	
	/// <summary>
	/// ホスト情報を検索する（指定値は"127.0.0.1"などのアドレスか"localhost"等のホスト名
	/// </summary>
	inline void Lookup( const char *pStr );

	/// <summary>
	/// ホスト名を取得する
	/// </summary>
	inline const string::CString& GetHostName();

	/// <summary>
	/// ホスト情報に関連する別名を取得する（複数ある可能性あり）
	/// </summary>
	inline const string::CStringList& GetAliasesList();

	/// <summary>
	/// ホスト情報に関連するIPアドレスを取得する（複数ある可能性あり）
	/// </summary>
	inline const string::CStringList& GetAddressList();

	/// <summary>
	/// 情報を出力する
	/// </summary>
	inline void Print();

	/// <summary>
	/// メンバ変数の取得
	/// </summary>
	inline const CResolverMember *GetMember() const;

private:

	/// <summary>
	/// 環境毎のメンバ変数
	/// </summary>
	CResolverMember *m_pMember;

	/// <summary>
	/// ホスト名
	/// </summary>
	string::CString m_HostName;

	/// <summary>
	/// エイリアス名
	/// </summary>
	string::CStringList m_AliasesList;

	/// <summary>
	/// アドレスリスト（文字）
	/// </summary>
	string::CStringList m_AddressList;
};

#ifdef SKYLIB_PLATFORM_WIN32

/// <summary>
/// WinSoket2ソケット接続先情報
/// </summary>
struct  CResolverMember
{
	/// <summary>
	/// バイナリアドレス配列
	/// </summary>
	typedef CVectorPrimitive< ULONG > CBinAddressVector;
	CBinAddressVector m_BinAddress;

	/// <summary>
	/// 指定のバイナリアドレスが配列に存在するかチェック
	/// </summary>
	inline skyBool CheckBinAddress( ULONG address ) const;
};

#endif

} } }

#include "Resolver.inl"