#pragma once

/// <summary>
/// データを送るスレッドクラス
/// </summary>
class ProducerThread : public sky::lib::thread::ThreadWin32
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ProducerThread( Channel *pChannel , skyWString *pName );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ProducerThread( void );

	/// <summary>
	/// 処理メソッド
	/// </summary>
	void ThreadMain( void );

private:

	/// <summary>
	/// 名前
	/// </summary>
	sky::lib::WString	m_Name;

	/// <summary>
	/// チャンネル
	/// </summary>
	Channel				*m_pChannel;

};