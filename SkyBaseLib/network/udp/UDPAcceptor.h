#pragma once

namespace sky { namespace lib { namespace net {

/// <summary>
/// UDPサーバーソケットクラス
/// 相互通信を行う場合、通信を行うプログラム両方がUDPAcceptor（受信用）とUDPSoket（送信用）を起動して行う。
/// </summary>
class CUDPAcceptor : public CSoket
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CUDPAcceptor( u16 portNo , u32 receiveTempBufSize = DEFAULT_RECEIVE_TEMP_BUF_SIZE  );
	CUDPAcceptor( u16 portNo , const char *address , u32 receiveTempBufSize = DEFAULT_RECEIVE_TEMP_BUF_SIZE  ); //マルチキャスト受信用（addressはマルチキャストグループアドレスを想定）
	void Constractor( u32 receiveTempBufSize );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CUDPAcceptor();

	/// <summary>
	/// 受信待ち受け実行
	/// ※このメソッドは内側でWhileループをするため制御が帰ってきません。別途アプリケーションを実行する場合は、この処理を別スレッドで実行してください。
	/// </summary>
	void Accept();

	/// <summary>
	/// クライアント要求発生イベントハンドラ
	/// 0 : 受信データバッファアドレス( u8 * )
	/// 1 : 受信データバッファサイズ
	/// ※ もし応答データを送信したい場合はデータ送信要求( ReplySendRequest() )にデータを設定すればイベント終了後直ちに返信される。
	/// ※ UDP はベストエフォート型の転送を行う。つまり、パケットの到達も、その到達順序も保証しない。データを取得順番に並べて使ってもダメなことがあるので注意。
	/// </summary>
	delegate::CDelegateHandler &ReceiveEventHndl();

	/// <summary>
	/// データ返信要求（コネクションが確立している際の応答を想定しているため、送り先アドレスが必要ない）
	/// クライアント要求発生イベントハンドラ内でこの関数を呼ぶことを想定しています。
	/// Ethernetのフレームサイズの上限が1500バイトなので、送信経路上にEthernetが存在する場合これをこえるとIP層でパケット分割が行われてしまう。
    /// 分割されたパケットはIP層で再び結合されるはずなので、基本問題無いが、分割に対応していない機器等が存在する場合問題なので1500バイトを超
    /// えないようにアプリケーションで調整するのが望ましい。
	/// </summary>
	void ReplySendRequest( void *pData , u32 size );

	/// <summary>
	/// 終了リクエスト
	/// </summary>
	void AcceptEndRequest();

	/// <summary>
	/// 終了判定
	/// </summary>
	skyBool IsAcceptEnd();

	/// <summary>
	/// ソケットハンドル取得
	/// </summary>
	socket_hndl GetHndl();

private:

	/// <summary>
	/// ステート
	/// </summary>
	enum eState
	{
		eState_Accept = 0 ,        //受信待ち
		eState_AcceptEndRequest ,  //受信待ち終了リクエスト中
		eState_AcceptEnd ,         //受信終了

	} m_eState;

	/// <summary>
	/// 接続先情報
	/// </summary>
	CSoketAddressPtr m_spSoketAddress;

	/// <summary>
	/// ソケットハンドル
	/// </summary>
	socket_hndl           m_SoketHndl;

	/// <summary>
	/// データ受信一時バッファ
	/// </summary>
	char*                 m_pTempReceiveBuffer;

	/// <summary>
	/// データ受信一時バッファサイズ
	/// </summary>
	u32                   m_TempReceiveBufferSize;

	/// <summary>
	/// データ返信要求バッファ
	/// </summary>
	memory::CMemoryStream m_ReplySendBuffer;

	/// <summary>
	/// クライアント接続イベントハンドラ
	/// </summary>
	delegate::CDelegateHandler m_ReceiveEventHndl;
};

} } }
