#pragma once

namespace sky { namespace lib { namespace util { 

//-----------------------------------< �֐��Q >---------------------------------------------//

/// <summary>
/// MUrmurHash�֘A�����Q
/// </summary>
namespace MurmurHash {

	/// <summary>
	/// �쐬
	/// </summary>
	inline u32 Create ( const void * key, s32 len, u32 seed );
}

} } }

#include "MurmurHash.inl"