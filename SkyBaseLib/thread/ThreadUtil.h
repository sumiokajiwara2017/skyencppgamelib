#pragma once

namespace sky { namespace lib { namespace thread {

/// <summary>
/// スレッド便利関数群
/// </summary>
class CThreadUtil
{

public:

	/// <summary>
	/// 現在のスレッドのＩＤを取得する(この関数を呼んだスレッドのIDを返す）
	/// </summary>
	 static inline tread_id  GetCurrentThreadId();

	/// <summary>
	/// 現在のスレッドを眠らせる
	/// </summary>
	 static inline void Sleep( updateTime ms );

	/// <summary>
	/// 現在のスレッドを終了させる（これを呼んだスレッドが終了する）
	/// </summary>
	 static inline void End();
};

} } }

//インスタンスアクセス文字列
#define CThreadUtil_			sky::lib::thread::CThreadUtil

#include "ThreadUtil.inl"