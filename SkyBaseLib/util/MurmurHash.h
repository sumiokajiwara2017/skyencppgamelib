#pragma once

namespace sky { namespace lib { namespace util { 

//-----------------------------------< ŠÖ”ŒQ >---------------------------------------------//

/// <summary>
/// MUrmurHashŠÖ˜Aˆ—ŒQ
/// </summary>
namespace MurmurHash {

	/// <summary>
	/// ì¬
	/// </summary>
	inline u32 Create ( const void * key, s32 len, u32 seed );
}

} } }

#include "MurmurHash.inl"