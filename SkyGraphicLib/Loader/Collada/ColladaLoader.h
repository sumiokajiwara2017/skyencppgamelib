#pragma once

/// <summary>
/// COLLADAの実装LIBへのリンク
/// </summary>
#ifdef SW_SKYLIB_USE_COLLADA_DOM

#ifdef _DEBUG

#ifdef SW_SKYLIB_USE_COLLADA_14
#pragma comment( lib, "libcollada14dom22-sd.lib" )
#else
#pragma comment( lib, "libcollada15dom22-sd.lib" )
#endif

#pragma comment( lib, "pcrecppd.lib" )
#pragma comment( lib, "pcred.lib" )
#pragma comment( lib, "libxml2-vc100-mt.lib" )
#else

#ifdef SW_SKYLIB_USE_COLLADA_14
#pragma comment( lib, "libcollada14dom22-s.lib" )
#else
#pragma comment( lib, "libcollada15dom22-s.lib" )
#endif

#pragma comment( lib, "pcrecpp.lib" )
#pragma comment( lib, "pcre.lib" )
#pragma comment( lib, "libxml2-vc100-mt.lib" )
#endif

#endif

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// Collada形式のデータからグラフィックデータをロードするクラス
/// 注意！読み込むモデルは以下の条件を満たしてください
/// ・三角形ポリゴンであること
/// ・マテリアルが付属していること
/// 
/// Blender→SkyLib(DirectX11)データのために以下のコンパートをする
/// 静的Mesh
/// ・頂点のyとzを入れ替える。
/// アニメーションデータ
/// ・Y軸拡縮をZ軸拡縮に
/// ・Z軸拡縮をY軸拡縮に
/// ・Y軸回転をZ軸回転に
/// ・Z軸回転をY軸回転に
/// ・回転値はすべてマイナスに
/// ・Y軸移動をZ軸移動に
/// ・Z軸移動をY軸移動に
/// ・行列を以下のようにコンバートする
///  +-----------+    +-----------+
///  |00,01,02,03|    |00,08,04,12|
///  |04,05,06,07| ⇒ |02,10,06,14|
///  |08,09,10,11| ⇒ |01,09,05,13|
///  |12,13,14,15|    |03,11,07,15|
///  ------------+    +-----------+
/// ■コードにすると
/// skylib[ 00 ] = blender[ 00 ];
/// skylib[ 08 ] = blender[ 01 ];
/// skylib[ 04 ] = blender[ 02 ];
/// skylib[ 12 ] = blender[ 03 ] * scaling;
/// skylib[ 02 ] = blender[ 04 ];
/// skylib[ 10 ] = blender[ 05 ];
/// skylib[ 06 ] = blender[ 06 ];
/// skylib[ 14 ] = blender[ 07 ] * scaling;
/// skylib[ 01 ] = blender[ 08 ];
/// skylib[ 09 ] = blender[ 09 ];
/// skylib[ 05 ] = blender[ 10 ];
/// skylib[ 13 ] = blender[ 11 ] * scaling;
///	skylib[ 03 ] = blender[ 12 ];
///	skylib[ 11 ] = blender[ 13 ];
///	skylib[ 07 ] = blender[ 14 ];
///	skylib[ 15 ] = blender[ 15 ];
/// </summary>
class ColladaLoader : public base::SkyObject
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

#define ColladaLoaderLoader_	    sky::lib::graphic::ColladaLoader
