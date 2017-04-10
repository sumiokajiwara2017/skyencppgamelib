#pragma once

/// <summary>
/// 更新スレッド
/// </summary>
class DrawThread : public sky::lib::thread::ThreadWin32
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DrawThread( DrawExecEventPtr spEvent );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~DrawThread( void );

	/// <summary>
	/// スレッド実行関数
	/// </summary>
	virtual void ThreadMain( void );

private:

	/// <summary>
	/// イベント
	/// </summary>
	DrawExecEventPtr m_spEvent;

};
