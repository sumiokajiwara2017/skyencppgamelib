#pragma once

namespace sky { namespace lib { namespace endian {

//-----------------------------------< 関数群 >---------------------------------------------//

/// <summary>
/// エンディアン変換処理群
/// </summary>
namespace EndianUtil
{
	/// <summary>
	/// ビッグエンディアンの場合1を返し、リトルエンディアンの場合は0を返す
	/// </summary>
	inline u8	GetEndian();

	/// <summary>
	/// ２バイトの文字をエンディアン変換する
	/// </summary>
    inline void Swap16( u8 *pSrcDest, u32 num = 1 );

	/// <summary>
	/// ４バイトの文字をエンディアン変換する
	/// </summary>
    inline void Swap32( u8 *pSrcDest, u32 num = 1 );

	/// <summary>
	/// ８バイトの文字をエンディアン変換する
	/// </summary>
    inline void Swap64( u8 *pSrcDest, u32 num = 1 );

}

#include "Endian.inl"

} } }

