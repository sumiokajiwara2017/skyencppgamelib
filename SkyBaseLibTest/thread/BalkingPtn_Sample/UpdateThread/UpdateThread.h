#pragma once

/// <summary>
/// 更新スレッド
/// </summary>
class UpdateThread : public sky::lib::thread::ThreadWin32
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	UpdateThread( DrawExecEventPtr spEvent );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~UpdateThread( void );

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
