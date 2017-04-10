#pragma once

namespace sky { namespace lib { namespace net {

/// <summary>
/// エンディアン変換処理
/// Windowsはリトルエンディアンだが、ネットワーク上は大体ビッグエンディアンらしい。
/// </summary>
namespace EndianUtil
{

//---送信時

	/// <summary>
	/// 送信元コンピューターのエンディアンから送信先ネットワークのエンディアンに変換する（u_long型）
	/// </summary>
	static inline u32 DataConvert_HostToNetWork( u32 data );

	/// <summary>
	/// 送信元コンピューターのエンディアンから送信先ネットワークのエンディアンに変換する（u_short型）
	/// </summary>
	static inline u16 DataConvert_HostToNetWork( u16 data );

//---受信時

	/// <summary>
	/// 送信元ネットワークのエンディアンから受信コンピューターのエンディアンに変換する（u_long型）
	/// </summary>
	static inline u32 DataConvert_NetWorkToHost( u32 data );

	/// <summary>
	/// 送信元ネットワークのエンディアンから受信コンピューターのエンディアンに変換する（u_short型）
	/// </summary>
	static inline u16 DataConvert_NetWorkToHost( u16 data );

};

} } }

#include "Endian.inl"