#pragma once

namespace sky { namespace lib { namespace base {

/// <summary>
/// メモリ管理用クラス
/// </summary>
class SkyRefObject : public SkyObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline SkyRefObject();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SkyRefObject();

	/// <summary>
	/// 参照カウンタ取得／設定
	/// </summary>
	inline u32  GetRefCnt() const;
	inline void SetRefCnt( u32 cnt );

	/// <summary>
	/// 参照カウンタＵＰ
	/// </summary>
	inline u32 RefCntUp();

	/// <summary>
	/// 参照カウンタＤＯＷＮ
	/// </summary>
	inline skyBool RefCntDown();

private:

	/// <summary>
	/// 参照カウンタ
	/// </summary>
	u32 m_nRefCnt;

};

} } }

#include "SkyRefObject.inl"