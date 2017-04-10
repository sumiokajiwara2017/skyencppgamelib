#pragma once

/// <summary>
///データを受け取るスレッドクラス
/// </summary>
class ConsumerThread : public sky::lib::thread::ThreadWin32
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ConsumerThread( Channel *pChannel );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ConsumerThread( void );

	/// <summary>
	/// 処理メソッド
	/// </summary>
	void ThreadMain( void );

private:

	/// <summary>
	/// チャンネル
	/// </summary>
	Channel				*m_pChannel;

};