#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// SkyGraphicBinary形式のデータからグラフィックデータをロードするクラス
/// </summary>
class CSkyGraphicBinaryLoader : public base::SkyObject
{

public:

	/// <summary>
	/// ファイルからロード
	/// パスのコントロールはCGraphicFilePathManagerで必ず行ってください。
	/// 読み込んだ結果はデータベースから取得できます。
	/// </summary>
	static void LoadFile( const hash::CHashStringPtr &path );

	/// <summary>
	/// SceneGraphNodeをファイルにセーブ
	/// </summary>
	static void SaveFile( const hash::CHashStringPtr &path , const CSceneGraphNodePtr &spSceneGraphNode );
};

} } }

#define CSkyGraphicBinaryLoader_	sky::lib::graphic::CSkyGraphicBinaryLoader
