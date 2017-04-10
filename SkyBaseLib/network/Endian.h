#pragma once

namespace sky { namespace lib { namespace net {

/// <summary>
/// �G���f�B�A���ϊ�����
/// Windows�̓��g���G���f�B�A�������A�l�b�g���[�N��͑�̃r�b�O�G���f�B�A���炵���B
/// </summary>
namespace EndianUtil
{

//---���M��

	/// <summary>
	/// ���M���R���s���[�^�[�̃G���f�B�A�����瑗�M��l�b�g���[�N�̃G���f�B�A���ɕϊ�����iu_long�^�j
	/// </summary>
	static inline u32 DataConvert_HostToNetWork( u32 data );

	/// <summary>
	/// ���M���R���s���[�^�[�̃G���f�B�A�����瑗�M��l�b�g���[�N�̃G���f�B�A���ɕϊ�����iu_short�^�j
	/// </summary>
	static inline u16 DataConvert_HostToNetWork( u16 data );

//---��M��

	/// <summary>
	/// ���M���l�b�g���[�N�̃G���f�B�A�������M�R���s���[�^�[�̃G���f�B�A���ɕϊ�����iu_long�^�j
	/// </summary>
	static inline u32 DataConvert_NetWorkToHost( u32 data );

	/// <summary>
	/// ���M���l�b�g���[�N�̃G���f�B�A�������M�R���s���[�^�[�̃G���f�B�A���ɕϊ�����iu_short�^�j
	/// </summary>
	static inline u16 DataConvert_NetWorkToHost( u16 data );

};

} } }

#include "Endian.inl"