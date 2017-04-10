#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// カメラ便利関数群
/// </summary>
namespace CameraUtil
{
	/// <summary>
	/// スクリーン座標を３Ｄ空間における座標に変換する
	/// スクリーン座標にした結果Ｚの情報はどうしても失われるので、任意の距離(distance)の先にある点として解釈する
	/// </summary>
	math::CBasicVector3 ConvertScreenPosTo3DPos( const math::CBasicVector2 &screenPos , const CWindowPtr &spWindow , const CameraPtr &spCamera , dectype distance = 1000 );

	/// <summary>
	/// カメラの位置を左側からの視点にする
	/// </summary>
	void SetDefaultCameraPos_Left( const CameraPtr &dest , dectype distance = CAMERA_DEFAULT_DISTANCE );

	/// <summary>
	/// カメラの位置を右側からの視点にする
	/// </summary>
	void SetDefaultCameraPos_Right( const CameraPtr &dest , dectype distance = CAMERA_DEFAULT_DISTANCE );

	/// <summary>
	/// カメラの位置を上からの視点にする
	/// </summary>
	void SetDefaultCameraPos_Top( const CameraPtr &dest , dectype distance = CAMERA_DEFAULT_DISTANCE );
};

} } }