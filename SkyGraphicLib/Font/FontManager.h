#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// １文字データ
/// </summary>
struct COneFontData
{
	//文字座標
	math::CVector2< d64 > pos[ 4 ];

	//UV座標
	math::CVector2< d64 > uv[ 4 ];

	//文字を囲む矩形のサイズ。文字一つ一つに持つ必要がないと考えていたが、同じタイプの文字でも、全角と半角で違うなど、様々なパターンがあると考えたので。（実際は2種類かも）
	math::CSize< d64 > size;
};

/// <summary>
/// 文字管理クラス
/// </summary>
class CFontManager : public base::SkyObject
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 末期化
	/// </summary>
	static void Dispose();

	/// <summary>
	/// 文字データをロードする（ビットマップ、ＵＶバイナリ）
	/// </summary>
	static void Load( font::eFontStyleType eStyle , const skyString *bmpPath , const skyString *uvPath );

	/// <summary>
	/// 文字コードとスタイルに対応した頂点情報を取得する
	/// </summary>
	static COneFontData &GetFont( u32 srcCode , font::eFontStyleType eStyle );

	/// <summary>
	/// スタイルに対応したテクスチャ情報を取得する
	/// </summary>
	static CTexturePtr &GetFontStyleTex( font::eFontStyleType eStyle );
};

//インスタンスアクセス文字列
#define CFontManagerInitialize_()	sky::lib::graphic::CFontManager::Initialize()
#define CFontManager_				sky::lib::graphic::CFontManager
#define CFontManagerDispose_()		sky::lib::graphic::CFontManager::Dispose()

} } }