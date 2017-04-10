#pragma once

namespace sky { namespace lib { namespace time {

/// <summary>
/// 時間間隔保持動作クラス
/// </summary>
class CRegulator : public base::SkyObject
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CRegulator();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CRegulator( updateTime radyIntvlTime );

	/// <summary>
	/// 時間が経過したか？
	/// </summary>
	inline skyBool IsReady( updateTime time );

	/// <summary>
	/// 準備完了時間間隔の設定
	/// </summary>
	inline void SetReadyIntvlTime( updateTime radyIntvlTime );

private:

	/// <summary>
	/// 準備完了時間間隔
	/// </summary>
	updateTime m_ReadyIntvlTime;

	/// <summary>
	/// 累積時間
	/// </summary>
	updateTime m_AccumulatedTime;
};

} } }

#include "Regulator.inl"