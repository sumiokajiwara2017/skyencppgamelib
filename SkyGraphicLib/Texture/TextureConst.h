#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// テクスチャ機能の定数
///
/// ○対応フォーマット（必要に応じて増やすが、むやみに色々サポートする必要はない）
/// ・tag
/// ・bmp256
/// ・dds
/// ※対応フォーマットであれば、パスからテクスチャの幅、高さ、フォーマット等の情報を取得する処理を実装する
/// 
/// ○ddsへのコンバート対応タイプ（コンバートはNVIDIAのコンバーターを使用する。どの形式にコンバートしたいかはテクスチャ名で指定する）
/// ・DXT1
/// ・DXT1a
/// ・DXT3
/// ・DXT5
/// ・RGBA
/// ※対応圧縮タイプであればパスを渡せば任意のフォルダにdds出力してくれるツールを用意する
/// 
/// ☆彡ポイントは、パスからあらゆる情報が取得できるような実装。テクスチャの情報を手打ちしないといけないなんて実装はＮＧ
/// </summary>
namespace texture
{
	/// <summary>
	/// 対応圧縮タイプキーワード
	/// ファイル名に圧縮形式を示す文字が含まれているので、それにしたがってddsにする。
	/// 例）hoge_dxt1_mip03.tga : dxt1圧縮で、mipmapは３枚
	/// </summary>
	static const skyString *s_eComp_dds_type_keyword[] =
	{ 
		_T( "dxt1" ) , 
		_T( "dxt1a" ) , 
		_T( "dxt3" ) ,
		_T( "dxt5" ) ,
		_T( "rgba" ) ,
	};
	/// <summary>
	/// ミップマップ数キーワード
	/// </summary>
	static const skyString *mip_keyword = _T( "mip" );

	/// <summary>
	/// 対応ファイルフォーマット
	/// </summary>
	enum eTextureFileFormat
	{
		eTextureFileFormat_bmp = 0 ,
		eTextureFileFormat_jpg ,
		eTextureFileFormat_png ,
		eTextureFileFormat_dds ,
		eTextureFileFormat_tiff ,
		eTextureFileFormat_gip ,
		eTextureFileFormat_wmp
	};

	/// <summary>
	/// 対応フォーマット
	/// </summary>
	enum eTextureFormat
	{
		eTextureFormat_R8G8B8 = 0 ,
		eTextureFormat_A8R8G8B8 ,    //一番有名どころです。1ピクセル1byte(8bit)でメモリ上に色情報が| ABGR | ABGR |...と並んでいると解釈します(リトルエンディアンのため)。
		eTextureFormat_X8R8G8B8 ,
		eTextureFormat_A32R32G32B32 ,
		eTextureFormat_P8 ,              //8bitカラーインデックス付き
	};

	/// <summary>
	/// 対応テクスチャ横サイズ
	/// </summary>
	enum eTexture_with
	{
		eTexture_with_1    = 1 ,
		eTexture_with_2    = 2 ,
		eTexture_with_4    = 4 ,
		eTexture_with_8    = 8 ,
		eTexture_with_16   = 16 ,
		eTexture_with_32   = 32 ,
		eTexture_with_64   = 64 ,
		eTexture_with_128  = 128 ,
		eTexture_with_256  = 256 ,
		eTexture_with_512  = 512 ,
		eTexture_with_768  = 768 ,
		eTexture_with_1024 = 1024 ,
	};

	/// <summary>
	/// 対応テクスチャ縦サイズ
	/// </summary>
	enum eTexture_height
	{
		eTexture_height_1    = 1 ,
		eTexture_height_2    = 2 ,
		eTexture_height_4    = 4 ,
		eTexture_height_8    = 8 ,
		eTexture_height_16   = 16 ,
		eTexture_height_32   = 32 ,
		eTexture_height_64   = 64 ,
		eTexture_height_128  = 128 ,
		eTexture_height_256  = 256 ,
		eTexture_height_512  = 512 ,
		eTexture_height_768  = 768 ,
		eTexture_height_1024 = 1024 ,
	};

	/// <summary>
	/// SkyTexture情報
	/// </summary>
	struct CTextureInfo
	{
		eTexture_with           Width;         //幅
		eTexture_height         Height;        //高さ
		u32                     MipLevels;     //ミップマップの枚数
		eTextureFormat          Format;        //厳密フォーマット
	};
}

} } }