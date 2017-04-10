#pragma once

/// <summary>
/// クライアントスレッド
/// </summary>
class ClientThread : public sky::lib::thread::ThreadWin32
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ClientThread( RequestQueuePtr& queue );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ClientThread( void );

	/// <summary>
	/// スレッド実行関数
	/// </summary>
	virtual void ThreadMain( void );

private:

	/// <summary>
	/// キューポインタ
	/// </summary>
	RequestQueuePtr		m_spRq;

};
