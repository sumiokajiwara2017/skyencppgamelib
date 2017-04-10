#pragma once

namespace sky { namespace lib { namespace net {

/// <summary>
/// ＵＰｎＰを送信してルーターのポート開け閉めする。
/// get_StaticPortMappingCollectionで失敗する場合は http://www.akakagemaru.info/port/windows7upnp.html とかやって見る
/// </summary>
class CUPnPClient : public base::SkyObject
{

public:

	/// <summary>
	/// ポートを解放する
	/// </summary>
	static void OpnePort( const skyString *name , const char *ip , u16 portNo );

	/// <summary>
	/// ポートを閉じる
	/// </summary>
	static void ClosePort( const char *ip , u16 portNo );
};

} } }
