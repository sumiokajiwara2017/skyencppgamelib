#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// WavefrontOBJ形式のデータからグラフィックデータをロードするクラス
/// </summary>
class CWavefrontOBJLoader : public base::SkyObject
{

public:

	/// <summary>
	/// objファイルをロードする
	/// パスのコントロールはCGraphicFilePathManagerで必ず行ってください。
    /// objファイルに関連するマテリアル、テクスチャも読み込む
	/// ロードされたメッシュはSetMeshでデータベースに格納され、CreateMeshでクローンされた値をこの関数で返します。
	/// すでにロードされていたら、クローンした値を即返します。
	/// </summary>
	static CMeshContenaPtr Load( const hash::CHashStringPtr &path , const CVertexBufferPtr &spVertex , const CIndexBufferPtr &spIndex );
};

} } }

#define CWavefrontOBJLoader_	         sky::lib::graphic::CWavefrontOBJLoader

