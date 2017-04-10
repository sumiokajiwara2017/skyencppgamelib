#pragma once

namespace sky { namespace lib { namespace bit {

/*

ビット操作メモ

変数　unsigned int uiVariable = 0;
ビットフラグ const unsigned int BITFLAG16 = 16;

ビットフラグを立てる
uiVariable |= BITFLAG16

ビットフラグを倒す
uiVariable &= ~BITFLAG16

ビットフラグが立ってたら倒す、ビットフラグが倒れてたら立てる
uiVariable ^= BITFLAG16

ビットフラグが立っているかを調べる
if( uiVariable & BITFLAG16 )

ビットフラグが倒れているかを調べる
if( !(uiVariable & BITFLAG16 ) )

*/

namespace BinaryUtil
{
	/// <summary>
	/// 任意Bitを1にする。
	/// </summary>
	inline void SetBitDataOn( u8 *pData , u32 idx , u32 dataSize = sizeof ( u32 ) * 8 );

	/// <summary>
	/// 任意Bitを0にする。
	/// </summary>
	inline void SetBitDataOff( u8 *pData , u32 idx , u32 dataSize = sizeof ( u32 ) * 8 );

	/// <summary>
	/// 任意Bitが1かどうか判定する。
	/// </summary>
	inline skyBool IsBitDataOn( u8 *pData , u32 idx , u32 dataSize = sizeof ( u32 ) * 8 );

	/// <summary>
	/// u32のフラグのビットを数える
	/// </summary>
	inline u8 CountBit( u32 data );
}

} } }

#include "BitUtil.inl"