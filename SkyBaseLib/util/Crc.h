#pragma once

namespace sky { namespace lib { namespace util { 

//-----------------------------------< 関数群 >---------------------------------------------//

/// <summary>
/// CRC値関連処理群
/// </summary>
namespace Crc {

	/// <summary>
	/// CRC値作成
	/// </summary>
	inline u32 Create( const skyString *pKey , u32 size );

	/// <summary>
	/// CRC値作成
	/// </summary>
	inline u32 Create( const skyString *pKey );

}

} } }

#include "Crc.inl"