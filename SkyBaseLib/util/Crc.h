#pragma once

namespace sky { namespace lib { namespace util { 

//-----------------------------------< �֐��Q >---------------------------------------------//

/// <summary>
/// CRC�l�֘A�����Q
/// </summary>
namespace Crc {

	/// <summary>
	/// CRC�l�쐬
	/// </summary>
	inline u32 Create( const skyString *pKey , u32 size );

	/// <summary>
	/// CRC�l�쐬
	/// </summary>
	inline u32 Create( const skyString *pKey );

}

} } }

#include "Crc.inl"