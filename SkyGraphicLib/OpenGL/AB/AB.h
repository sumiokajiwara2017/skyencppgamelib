#pragma once

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// アキュムレーションバッファ操作クラス
/// アキュムレーションバッファ自体はOpenGL内部にあり、このクラスのメンバにはないです。
/// アキュムレーションバッファを使用する場合はPIXELFORMATDESCRIPTORの構造体でアキュムレーションバッファの部分を0以外にする
/// </summary>
class CAccumulationBuffer
{
	/// <summary>
	/// アキュムレーションバッファのクリアカラー設定
	/// </summary>
	inline void SetClearColor( const CColor &color );

	/// <summary>
	/// アキュムレーションバッファをクリアする
	/// </summary>
	inline void Clear();

	/// <summary>
	/// 現在画面に表示されているものをアキュームバッファに転送する
	/// </summary>
	inline void TransmitFromColorBuffer( f32 rate );

	/// <summary>
	/// 現在画面に表示されているものをアキュームバッファに加算する
	/// </summary>
	inline void AddFromColorBuffer( f32 rate );

	/// <summary>
	/// アキュームバッファに指定の値を加算する
	/// </summary>
	inline void Add( f32 rate );

	/// <summary>
	/// アキュームバッファに指定の値を掛ける
	/// </summary>
	inline void Mul( f32 rate );

	/// <summary>
	/// アキュームバッファの内容を画面描画領域に転送する（画面に表示する）
	/// </summary>
	inline void TransmitToColorBuffer( f32 rate );
};

} } }

#endif

#include "AB.inl"