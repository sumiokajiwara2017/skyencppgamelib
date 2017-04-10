#pragma once

namespace sky { namespace lib { namespace net {

/// UDPクライアントソケットクラス
/// UDP層のは送信確認をしてくれないので、もし送信確認をする場合は自前でプロトコルをつくってやる；
/// </summary>
class CUDPSoket : public CSoket
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CUDPSoket();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CUDPSoket();

//---Send

	/// <summary>
	/// 送信用通信経路を開く( TCPとは違い、ハンドシェイクが無いためコネクションは必要ない。オープンのみ）
	/// </summary>
	void Open( u16 portNo , const char *address );
	void Open( const CSoketAddressPtr &spAddress );

	/// <summary>
	/// 送信用通信経路を閉じる 
	/// </summary>
	void Close();

	/// <summary>
	/// データ送信
	/// 送ったデータは必ず届くわけではない。UDPの相互通信で必ずデータ送信を成功させたい場合、受信側からどこまでのデータが届いたかを送ってもらう必要がある。
	/// 相手の受信応答ですら消失する可能性があるため、受信応答の再送制御も行わないといけない。
	/// Ethernetのフレームサイズの上限が1500バイトなので、送信経路上にEthernetが存在する場合これをこえるとIP層でパケット分割が行われてしまう。
    /// 分割されたパケットはIP層で再び結合されるはずなので、基本問題無いが、分割に対応していない機器等が存在する場合問題なので1500バイトを超
    /// えないようにアプリケーションで調整するのが望ましい。
	/// </summary>
	u32  Send( const void* pData , u32 size );

	/// <summary>
	/// データの受信をする
	/// </summary>
	u32  Receive( void* pData , u32 size , u32 tryNum = 8  );

//---SendBroadCast

	/// <summary>
	/// ブロードキャスト送信用通信経路を開く
	/// </summary>
	void OpenBroadCast( u16 portNo , const char *address );

	/// <summary>
	/// ブロードキャスト送信用通信経路を閉じる 
	/// </summary>
	void CloseBroadCast();

	/// <summary>
	/// ブロードキャストデータ送信（最近のルーターはブロードキャスト送信を通さない設定がデフォルトである場合が多いらしいので届かないことが多いらしい）
	/// 例えばネットワーク上にとあるアプリケーションが立ち上がっていないかをブロードキャストした後のReceiveBroadCastの結果で探ることができる。
	/// </summary>
	u32  SendBroadCast( const void* pData , u32 size );

	/// <summary>
	/// データの受信をする(何処から返信が来たかが重要になるため、fromAddressでその情報を受取る）
	/// </summary>
	u32  ReceiveBroadCast( void* pData , u32 size , const CSoketAddressPtr &spFromAddress , u32 tryNum = 8 );

//---SendMultiCast

	/// <summary>
	/// マルチキャスト送信用通信経路を開く
	/// netInterfaceAdrは発信元機器が複数のネットワークカードを付けていた場合、どのネットワークカードからマルチキャストを出力するかを指定するためのアドレス。
	/// コマンドプロンプトで[ ipconfig ]と打ってでてくるインターネット機器のアドレスをどれでもいいので入れればよい。
	/// ネットワーク機器のIPアドレスをプログラムで取得する場合、WindowsならIP Helper APIを使用する。
	/// TTLとはIPパケットのルーティングにおける生存期間。パケットが1つのルーターを通過する毎に1が減算され、0になった場合パケットは廃棄される。
	/// </summary>
	void OpenMultiCast( u16 portNo , const char *address , char *netInterfaceAdr , s32 ttlNum );

	/// <summary>
	/// マルチキャスト送信用通信経路を閉じる 
	/// </summary>
	void CloseMultiCast();

	/// <summary>
	/// マルチキャストデータ送信
	/// </summary>
	u32  SendMultiCast( const void* pData , u32 size );

	/// <summary>
	/// データの受信をする(何処から返信が来たかが重要になるため、fromAddressでその情報を受取る）
	/// </summary>
	u32  ReceiveMultiCast( void* pData , u32 size , const CSoketAddressPtr &spFromAddress , u32 tryNum = 8 );

private:

//---Send

	/// <summary>
	/// 送信接続先情報
	/// </summary>
	CSoketAddressPtr m_spSendSoketAddress;

	/// <summary>
	/// 送信ソケットOpen済みフラグ
	/// </summary>
	skyBool m_IsOpen;

	/// <summary>
	/// ソケットハンドル
	/// </summary>
	socket_hndl m_SendSoketHndl;

//---SendBroadCast

	/// <summary>
	/// ブロードキャスト接続先情報
	/// </summary>
	CSoketAddressPtr m_spBroadCastSoketAddress;

	/// <summary>
	/// ブロードキャストソケットOpen済みフラグ
	/// </summary>
	skyBool m_IsOpenBroadCast;

	/// <summary>
	/// ブロードキャストソケットハンドル
	/// </summary>
	socket_hndl m_BroadCastSoketHndl;

//---SendMultiCast

	/// <summary>
	/// 送信接続先情報
	/// </summary>
	CSoketAddressPtr m_spMultiCastSoketAddress;

	/// <summary>
	/// マルチキャストソケットOpen済みフラグ
	/// </summary>
	skyBool m_IsOpenMultiCast;

	/// <summary>
	/// マルチキャストソケットハンドル
	/// </summary>
	socket_hndl m_MultiCastSoketHndl;
};

} } }
