#pragma once

namespace sky { namespace lib { namespace thread {

/// <summary>
/// クリティカルセクション
/// </summary>
class CriticalSection : public base::SkyObject
{

public:

	/// <summary>
	/// 作成
	/// </summary>
	static CriticalSection *Create();

	/// <summary>
	/// 削除
	/// </summary>
	static void Delete( CriticalSection *pCs );

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CriticalSection();

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CriticalSection();

	/// <summary>
	/// 排他開始（クリティカルセクションに入る）
	/// </summary>
	inline void Enter();

	/// <summary>
	/// 排他開始（クリティカルセクションから出る）
	/// </summary>
	inline void Leave();

private:

	/// <summary>
	/// ロックスレッド
	/// </summary>
	tread_id m_LoackThread;

	/// <summary>
	/// ロックカウント
	/// </summary>
	s32	m_LockCount;

#ifdef SKYLIB_PLATFORM_WIN32

	/// <summary>
	/// Windowsクリティカルセクション情報
	/// </summary>
	CRITICAL_SECTION m_Cs;

#endif

};

/// <summary>
/// クリティカルロック
/// </summary>
class CriticalLock : public base::SkyObject
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CriticalLock( CriticalSection *pCs );

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CriticalLock();

	/// <summary>
	///	明示的にCriticalSectionに入る
	/// </summary>
	inline void	Enter();

	/// <summary>
	///	明示的にCriticalSectionから抜ける
	/// </summary>
	inline void	Leave();

	/// <summary>
	///	Lockカウントの取得
	/// </summary>
	inline s32	GetCount();

private:

	/// <summary>
	/// 管理しているクリティカルセクション
	/// </summary>
	CriticalSection *m_pCs;

	/// <summary>
	///	ロックカウント
	/// </summary>
	u32 m_LockCount;
};

} } }

#include "CriticalSection.inl"