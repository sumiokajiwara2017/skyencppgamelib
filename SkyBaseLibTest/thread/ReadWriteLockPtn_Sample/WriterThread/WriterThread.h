#pragma once

/// <summary>
/// 
/// </summary>
class WriterThread : public sky::lib::thread::ThreadWin32
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	WriterThread( sky::lib::thread::ReadWriteLock *pRWLock , CommonResource *pComRes , skyWString *pName );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~WriterThread( void );

	/// <summary>
	/// 処理メソッド
	/// </summary>
	void ThreadMain( void );

private:

	/// <summary>
	/// 読み込み書き込みロック
	/// </summary>
	sky::lib::thread::ReadWriteLock *m_pRWLock;

	/// <summary>
	/// 共通リソース
	/// </summary>
	CommonResource					*m_pComRes;

	/// <summary>
	/// スレッド名
	/// </summary>
	sky::lib::WString				m_Name;

};
