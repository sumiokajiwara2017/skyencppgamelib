#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// スクリーン関連便利関数群
/// スクリーンとは、描画画面範囲。WindowsならWindowサイズ（※）、家庭用ゲーム機ならテレビの縦横サイズ（※）、
/// ※通常＝描画領域になるが、４：３のWindow,もしくはテレビを１６：９に補正する場合上下に余りができるため、＝描画領域ではなくなる。
/// </summary>
class CScreenUtil : base::SkyObject
{

public:

	/// <summary>
	/// ビューポートを作成
    /// ビューポートは幅と高さが0.0f～1.0fの矩形
	/// </summary>
	static math::CBasicRect CreateViewPort( const math::CBasicSize &screenSize , dectype screenAspectRatio );

	/// <summary>
	/// ピクセル座標を0.0f ～ 1.0fの間のピクセル座標に変換します
	/// </summary>
	static math::CBasicVector2 PixelToPoint( const math::CBasicVector2& pixelPosition , const math::CBasicSize &screenSize , dectype width = screen::VIRTUAL_SCREEN_W , dectype height = screen::VIRTUAL_SCREEN_H );

	/// <summary>
	/// スクリーンサイズを取得
    /// このシステムにおけるスクリーンはウィンドウと１：１であり、スクリーンサイズとはウィンドウサイズと同等。しかしウィンドウは複数存在する可能性があるため、＝スクリーンではない。
	/// </summary>
	static math::CBasicSize GetScreenSize( const math::CBasicSize &windowSize );

	/// <summary>
	/// シネマスコープの余白の割合を返す（シネマスコープは上下に均等な余白ができる場合があるが、この関数はその片方のスクリーンの高さにおける割合を返す）
	/// </summary>
	/*
	*	例えば、デフォルト引数で、スクリーンが1280*720の解像度の場合は0.0（余白なし）　800*600の解像度の場合は0.125が返ります。
	*/
    static dectype GetCinemaScopeHeight( const math::CBasicSize &screenSize ,  dectype screenAspectRatio = screen::VIRTUAL_SCREEN_W / screen::VIRTUAL_SCREEN_H );

	/// <summary>
	/// 指定の画面比率( width : height )に補正した時に使用できる画面の高さの割合を取得
	/// </summary>
	/*
	*	例えば、デフォルト引数で、スクリーンが1280*720の解像度の場合は1.0　800*600の解像度の場合は0.75が返ります。
	*/
	static dectype GetValidScreenHeightRateInVirtualScreen( const math::CBasicSize &screenSize , dectype width = screen::VIRTUAL_SCREEN_W , dectype height = screen::VIRTUAL_SCREEN_H );

	/// <summary>
	/// スクリーンのピクセルの幅と高さの割合を返す（この値に実際の画面の幅と高さをそれぞれかけると、1ピクセルの幅と高さになる）
	/// </summary>
	static math::CBasicSize GetViewPortSpacedPixelSize( const math::CBasicSize &screenSize , dectype width = screen::VIRTUAL_SCREEN_W , dectype height = screen::VIRTUAL_SCREEN_H );

};

} } }

#define CScreenUtil_	sky::lib::graphic::CScreenUtil
