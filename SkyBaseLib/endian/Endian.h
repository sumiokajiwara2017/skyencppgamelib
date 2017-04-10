#pragma once

namespace sky { namespace lib { namespace endian {

//-----------------------------------< �֐��Q >---------------------------------------------//

/// <summary>
/// �G���f�B�A���ϊ������Q
/// </summary>
namespace EndianUtil
{
	/// <summary>
	/// �r�b�O�G���f�B�A���̏ꍇ1��Ԃ��A���g���G���f�B�A���̏ꍇ��0��Ԃ�
	/// </summary>
	inline u8	GetEndian();

	/// <summary>
	/// �Q�o�C�g�̕������G���f�B�A���ϊ�����
	/// </summary>
    inline void Swap16( u8 *pSrcDest, u32 num = 1 );

	/// <summary>
	/// �S�o�C�g�̕������G���f�B�A���ϊ�����
	/// </summary>
    inline void Swap32( u8 *pSrcDest, u32 num = 1 );

	/// <summary>
	/// �W�o�C�g�̕������G���f�B�A���ϊ�����
	/// </summary>
    inline void Swap64( u8 *pSrcDest, u32 num = 1 );

}

#include "Endian.inl"

} } }

