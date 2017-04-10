#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// FBX形式のデータからグラフィックデータをロードするクラス
/// </summary>
class CFBXLoader : public base::SkyObject
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	static void    Initialize();

	/// <summary>
	/// 末期化
	/// </summary>
	static void    Dispose();

	/// <summary>
	/// ファイルからロード
	/// パスのコントロールはCGraphicFilePathManagerで必ず行ってください。
	/// 読み込んだ結果はデータベースから取得できます。
	/// </summary>
	static void LoadFile( const hash::CHashStringPtr &path );

};

} } }

#define CFBXLoaderInitialize_() sky::lib::graphic::CFBXLoader::Initialize()
#define CFBXLoaderLoader_	    sky::lib::graphic::CFBXLoader
#define CFBXLoaderDispose_()    sky::lib::graphic::CFBXLoader::Dispose()
