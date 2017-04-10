#pragma once

#ifdef SW_SKYLIB_DEBUG_ON

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// もんしょさんのＨＰのサンプルモデルをロードする
/// </summary>
class CMNSSampleModelFileLoader : public base::SkyObject
{

public:

	/// <summary>
	/// ファイルからロード
	/// </summary>
	static void LoadFile( const hash::CHashStringPtr &path );
};

} } }

#define CMNSSampleModelFileLoader_	sky::lib::graphic::CMNSSampleModelFileLoader

#endif