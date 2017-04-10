#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// テクスチャ便利処理群
/// </summary>
class CTextureUtil : public base::SkyObject
{

public:

	/// <summary>
	/// テクスチャパスからテクスチャ情報を取得する
	/// </summary>
	static void GetTextureInfo( const hash::CHashStringPtr &path , texture::CTextureInfo &dest );

	/// <summary>
	/// テクスチャをddsフォーマットに圧縮する
	/// </summary>
	static void ddsConvert( const hash::CHashStringPtr &path , stream::IStreamPtr &dest );
};

} } }