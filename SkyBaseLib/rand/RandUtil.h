#pragma once

namespace sky { namespace lib {

/// <summary>
/// �����_���v�Z
/// </summary>
namespace RandUtil
{
	/// <summary>
	/// �w��̗������擾����
	/// </summary>
	inline void SetTimeSeed	();
	inline void SetSeed		( u32 seed );
	inline u32 Rand			( u32 maxValue );

	/// <summary>
	/// 0.0f �` 1.0f�܂ł̒l���������@
	/// </summary>
	inline d64 Rand         ();

	/// <summary>
	/// -1.0f �` 1.0f �܂ł̒l��Ԃ�
	/// </summary>
	inline d64 RandomClamped();

	/// <summary>
	/// x�`y�܂ł̊Ԃ̐��������_���ŕԂ�
	/// </summary>
	inline d64 RandInRange( d64 x, d64 y );

	/// <summary>
	/// skyTrue skyFalse�������_���ŕԂ�
	/// </summary>
	inline skyBool RandBool();
}

} }

#include "RandUtil.inl"