#pragma once

/// <summary>
/// サーバースレッド
/// </summary>
class ServerThread : public sky::lib::thread::ThreadWin32
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ServerThread( RequestQueuePtr& queue );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ServerThread( void );

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
