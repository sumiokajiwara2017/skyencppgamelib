#pragma once

namespace sky { namespace lib {

/// <summary>
/// ランダム計算
/// </summary>
namespace RandUtil
{
	/// <summary>
	/// 指定の乱数を取得する
	/// </summary>
	inline void SetTimeSeed	();
	inline void SetSeed		( u32 seed );
	inline u32 Rand			( u32 maxValue );

	/// <summary>
	/// 0.0f ～ 1.0fまでの値をかえす　
	/// </summary>
	inline d64 Rand         ();

	/// <summary>
	/// -1.0f ～ 1.0f までの値を返す
	/// </summary>
	inline d64 RandomClamped();

	/// <summary>
	/// x～yまでの間の数をランダムで返す
	/// </summary>
	inline d64 RandInRange( d64 x, d64 y );

	/// <summary>
	/// skyTrue skyFalseをランダムで返す
	/// </summary>
	inline skyBool RandBool();
}

} }

#include "RandUtil.inl"