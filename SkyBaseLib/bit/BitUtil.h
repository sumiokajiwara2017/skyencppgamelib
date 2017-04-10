#pragma once

namespace sky { namespace lib { namespace bit {

/*

�r�b�g���상��

�ϐ��@unsigned int uiVariable = 0;
�r�b�g�t���O const unsigned int BITFLAG16 = 16;

�r�b�g�t���O�𗧂Ă�
uiVariable |= BITFLAG16

�r�b�g�t���O��|��
uiVariable &= ~BITFLAG16

�r�b�g�t���O�������Ă���|���A�r�b�g�t���O���|��Ă��痧�Ă�
uiVariable ^= BITFLAG16

�r�b�g�t���O�������Ă��邩�𒲂ׂ�
if( uiVariable & BITFLAG16 )

�r�b�g�t���O���|��Ă��邩�𒲂ׂ�
if( !(uiVariable & BITFLAG16 ) )

*/

namespace BinaryUtil
{
	/// <summary>
	/// �C��Bit��1�ɂ���B
	/// </summary>
	inline void SetBitDataOn( u8 *pData , u32 idx , u32 dataSize = sizeof ( u32 ) * 8 );

	/// <summary>
	/// �C��Bit��0�ɂ���B
	/// </summary>
	inline void SetBitDataOff( u8 *pData , u32 idx , u32 dataSize = sizeof ( u32 ) * 8 );

	/// <summary>
	/// �C��Bit��1���ǂ������肷��B
	/// </summary>
	inline skyBool IsBitDataOn( u8 *pData , u32 idx , u32 dataSize = sizeof ( u32 ) * 8 );

	/// <summary>
	/// u32�̃t���O�̃r�b�g�𐔂���
	/// </summary>
	inline u8 CountBit( u32 data );
}

} } }

#include "BitUtil.inl"