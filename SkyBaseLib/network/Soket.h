#pragma once

namespace sky { namespace lib { namespace net {

/// <summary>
/// ソケットクラス
/// エラー発生時には例外を発生させます。
/// アプリケーション側はエラーを判定し、処理を行ってください。
/// ■ 例外処理実装例
///	try
///	{
///    処理
/// }
///	catch( net::CSoketException &e )
/// {
///    例外処理
///	   e.Print();
/// }
/// catch( net::CSoketAppException &e )
/// {
///    例外処理
///	   e.Print();
/// }
///★ もしつながらなかったり、データが変になっていたらエンディアンを疑いましょう。
///★ ネットワークの各種仕様はRFCを調べるとでてくる。ICMPならRFC792等。（例：http://www5d.biglobe.ne.jp/~stssk/rfcjlist.html ）
/// </summary>
class CSoket : public base::SkyRefObject
{

public:

	/// <summary>
	/// 初期化（全体で一度だけ）
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 末期化（全体で一度だけ）
	/// </summary>
	static void Dispose();

protected:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CSoket();

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CSoket();

	/// <summary>
	/// ソケットを開く
	/// </summary>
	inline void Open(  socket_hndl &soketHndl , s32 AddressFamily , s32 soketType , s32 protcol );

	/// <summary>
	/// ソケットを閉じる
	/// </summary>
	inline void Close( socket_hndl &soketHndl );

	/// <summary>
	/// サーバーのアドレス情報(IPアドレス及びポート番号)をソケットに結び付ける。
	/// </summary>
	inline void Bind( socket_hndl &soketHndl , const CSoketAddressPtr &spAddr ); 

	/// <summary>
	/// 接続
	/// </summary>
	inline void Connect( socket_hndl &soketHndl , const CSoketAddressPtr &spAddr );

	/// <summary>
	/// 接続解除
	// 0 受信を無効にする
	// 1 送信を無効にする
	// 2 受信、送信を無効にする
	/// </summary>
	inline void DisConnect( socket_hndl &soketHndl , s32 id );

	/// <summary>
	/// 接続情報の取得
	/// </summary>
	inline CSoketAddressPtr GetAddress();
	inline void             SetAddress( const CSoketAddressPtr &spSoketAddress );
};

} } }

#define CSoketInitialize_()	sky::lib::net::CSoket::Initialize()
#define CSoketDispose_()	sky::lib::net::CSoket::Dispose()

#include "Soket.inl"