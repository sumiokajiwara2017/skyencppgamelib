#pragma once

namespace sky { namespace lib { namespace util { 

//-----------------------------------< 関数群 >---------------------------------------------//

/// <summary>
/// MUrmurHash関連処理群
/// </summary>
namespace MurmurHash {

	/// <summary>
	/// 作成
	/// </summary>
	inline u32 Create ( const void * key, s32 len, u32 seed );
}

} } }

#include "MurmurHash.inl"