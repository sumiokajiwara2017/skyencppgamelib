#pragma once

namespace sky { namespace lib { namespace net {

/// <summary>
/// アドレス文字の長さ
/// </summary>
static const u32 ADDRESS_STRING_LENGTH = 32;

//プラットフォーム毎のメンバ変数(pimpl)
struct  CSoketAddressMember;

class CSoketAddress;
typedef SmartPointer< CSoketAddress > CSoketAddressPtr;

/// <summary>
/// ソケット接続先情報クラス
/// </summary>
class CSoketAddress : public base::SkyRefObject
{

public:

	/// <summary>
	/// コンストラクタ
	/// addressはIPかホスト名を使用する( 例)"127.0.0.1" または "localhost" )
	/// </summary>
	inline CSoketAddress( s32 AddressFamily , u16 portNo , const char *address , skyBool isGetHostInfo = skyTrue );
	inline CSoketAddress( s32 AddressFamily , u16 portNo ); //サーバー用（アドレスは要らない）
	inline CSoketAddress();                                 //空で生成

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CSoketAddress();

	/// <summary>
	/// アドレス情報取得
	/// </summary>
	inline const CResolver &GetResolver() const;

	/// <summary>
	/// メンバ変数取得
	/// </summary>
	inline CSoketAddressMember* GetMember();

private:

	/// <summary>
	/// メンバ変数
	/// </summary>
	CSoketAddressMember* m_pMember;

	/// <summary>
	/// アドレス情報変換器
	/// </summary>
	CResolver m_Resolver;
};

#ifdef SKYLIB_PLATFORM_WIN32

/// <summary>
/// WinSoket2ソケット接続先情報
/// </summary>
struct  CSoketAddressMember
{
	/// <summary>
	/// 接続情報
	/// </summary>
	SOCKADDR_IN m_ConnectInfo;

	/// <summary>
	/// 接続先文字情報
	/// </summary>
	u8 m_Address[ ADDRESS_STRING_LENGTH ];
};

#endif

} } }

#include "SoketAddress.inl"