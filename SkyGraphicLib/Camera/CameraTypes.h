#pragma once

namespace sky { namespace lib { namespace graphic {

	/// <summary>
	/// カメラのデフォルト値
	/// </summary>
	static const math::CBasicVector3 CAMERA_DEFAULT_UP_VECTOR           = math::CBasicVector3( 0 , 1 , 0 );
	static const math::CBasicVector3 CAMERA_DEFAULT_RIGHT_VECTOR        = math::CBasicVector3( 1 , 0 , 0 );
	static const dectype			 CAMERA_DEFAULT_NEAR                = 1.0f;//2Dカメラの場合RENDERING_2D_PRIO_FASTに置き換えられる。
	static const dectype			 CAMERA_DEFAULT_FAR	                = 1000; //2Dカメラの場合RENDERING_2D_PRIO_MAXに置き換えられる。
	static const dectype			 CAMERA_DEFAULT_FOV	                = math::util::KPI / 4;
	static const dectype			 CAMERA_DEFAULT_ZOOM                = 1.0f;
	static const dectype			 CAMERA_DEFAULT_ASPECT              = 4.0f / 3.0f;
	static const math::CBasicVector3 CAMERA_DEFAULT_EYE	                = math::CBasicVector3( 0.0f, 0.0f, -15.0f );
	static const math::CBasicVector3 CAMERA_DEFAULT_TAG	                = math::CBasicVector3( 0.0f, 0.0f, 0.0f );
	static const dectype			 CAMERA_DEFAULT_DISTANCE            = ( CAMERA_DEFAULT_EYE - CAMERA_DEFAULT_TAG ).Length();
	static const dectype			 CAMERA_DEFAULT_SCREEN_WIDTH        = 640.0f*2;
	static const dectype			 CAMERA_DEFAULT_SCREEN_HEIGHT       = 480.0f*2;
	static const updateTime          CAMERAMANAGER_DEFAULT_INTERP_TIME  = 0.5f;
	static const s32                 CAMERAMANAGER_QUEUE_SIZE           = 5;
	static const s32                 CAMERAMANAGER_DEBUFVEC_LEN         = 5;

	/// <summary>
	/// カメラの種類
	/// </summary>
	enum eCameraKind
	{
		eCameraKind_Fix = 0 ,    //固定カメラ  （視点、注視点固定）
		eCameraKind_Satellite ,  //衛星カメラ  （視点移動、注視点固定）
		eCameraKind_Fps ,        //一人称カメラ（視点固定、注視点移動）
	};

} } }
