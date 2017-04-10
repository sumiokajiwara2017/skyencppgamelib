#pragma once

/// <summary>
/// 更新スレッド
/// </summary>
class DrawExecEvent
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DrawExecEvent( void );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~DrawExecEvent( void );

	/// <summary>
	/// フラグＯＮ
	/// </summary>
	void On( void );

	/// <summary>
	/// フラグＯＦＦ
	/// </summary>
	void Off( void );

	/// <summary>
	/// フラグ状態確認
	/// </summary>
	skyBool Is( void );

	/// <summary>
	/// 終了
	/// </summary>
	void End( void);
	skyBool IsEnd( void);

private:

	/// <summary>
	/// 管理フラグ
	/// </summary>
	skyBool		m_IsDrawExecGo;

	/// <summary>
	/// 終了フラグ
	/// </summary>
	skyBool		m_IsEnd;

};

typedef sky::lib::SmartPointerMT< DrawExecEvent >			DrawExecEventPtr;	//SmartPointer型
