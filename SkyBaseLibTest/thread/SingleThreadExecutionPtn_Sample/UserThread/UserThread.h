#pragma once

/// <summary>
/// ユーザースレッドクラス
/// </summary>
class UserThread : public sky::lib::thread::ThreadWin32
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	UserThread( const sky::lib::SmartPointerMT< ThreadCommonRes >& res , s32 threadNo );

	/// <summary>
	/// 処理メソッド
	/// </summary>
	void ThreadMain( void );

protected:

	/// <summary>
	/// メンバ
	/// </summary>
	sky::lib::SmartPointerMT< ThreadCommonRes >	m_Res;
	s32											m_ThreadNo;
};
