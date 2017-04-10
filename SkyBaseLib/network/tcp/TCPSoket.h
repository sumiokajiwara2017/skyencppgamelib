#pragma once

namespace sky { namespace lib { namespace net {

/// <summary>
/// TCPクライアントソケットクラス
/// TCP層のおかげで送ったデータはサーバーに必ず届く。（断線とか物理破壊がなければ）
/// </summary>
class CTCPSoket : public CSoket
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CTCPSoket( u16 portNo , const char *address , u32 receiveTempBufSize = DEFAULT_RECEIVE_TEMP_BUF_SIZE );
	CTCPSoket( const CSoketAddressPtr &spAddress , u32 receiveTempBufSize = DEFAULT_RECEIVE_TEMP_BUF_SIZE );
	void Constractor( u32 receiveTempBufSize = DEFAULT_RECEIVE_TEMP_BUF_SIZE );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CTCPSoket();

	/// <summary>
	/// 接続
	/// </summary>
	void Connect();

	/// <summary>
	/// 接続解除
	/// </summary>
	void DisConnect();

	/// <summary>
	/// 送信（基本必ず届く。届かなければ例外が発生する。その例外をcatchして自分で再送制御をかけることもできる）
	/// </summary>
	u32  Send( const void* pData , u32 size );

	/// <summary>
	/// 受信
	/// </summary>
	u32  Receive( void* pBuf , u32 size );

	/// <summary>
	/// ソケットハンドル取得
	/// </summary>
	socket_hndl GetHndl();

private:

	/// <summary>
	/// 接続先情報
	/// </summary>
	CSoketAddressPtr m_spSoketAddress;

	/// <summary>
	/// Connect済みフラグ
	/// </summary>
	skyBool m_IsConnect;

	/// <summary>
	/// ソケットハンドル
	/// </summary>
	socket_hndl m_SoketHndl;

	/// <summary>
	/// データ受信一時バッファ
	/// </summary>
	char* m_pTempReceiveBuffer;

	/// <summary>
	/// データ受信一時バッファサイズ
	/// </summary>
	u32 m_TempReceiveBufferSize;
};

} } }
