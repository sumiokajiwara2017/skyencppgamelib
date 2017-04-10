#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// XFile形式のデータからグラフィックデータをロードするクラス
/// </summary>
class CXFileLoader : public base::SkyObject
{

public:

	/// <summary>
	/// ファイルからロード
	/// パスのコントロールはCGraphicFilePathManagerで必ず行ってください。
	/// 読み込んだ結果はデータベースから取得できます。
	/// </summary>
	static void LoadFile( const hash::CHashStringPtr &path );
};

} } }

#define CXFileLoaderLoader_	    sky::lib::graphic::CXFileLoader
