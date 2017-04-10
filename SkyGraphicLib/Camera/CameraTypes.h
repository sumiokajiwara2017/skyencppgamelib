#pragma once

namespace sky { namespace lib { namespace graphic {

	/// <summary>
	/// �J�����̃f�t�H���g�l
	/// </summary>
	static const math::CBasicVector3 CAMERA_DEFAULT_UP_VECTOR           = math::CBasicVector3( 0 , 1 , 0 );
	static const math::CBasicVector3 CAMERA_DEFAULT_RIGHT_VECTOR        = math::CBasicVector3( 1 , 0 , 0 );
	static const dectype			 CAMERA_DEFAULT_NEAR                = 1.0f;//2D�J�����̏ꍇRENDERING_2D_PRIO_FAST�ɒu����������B
	static const dectype			 CAMERA_DEFAULT_FAR	                = 1000; //2D�J�����̏ꍇRENDERING_2D_PRIO_MAX�ɒu����������B
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
	/// �J�����̎��
	/// </summary>
	enum eCameraKind
	{
		eCameraKind_Fix = 0 ,    //�Œ�J����  �i���_�A�����_�Œ�j
		eCameraKind_Satellite ,  //�q���J����  �i���_�ړ��A�����_�Œ�j
		eCameraKind_Fps ,        //��l�̃J�����i���_�Œ�A�����_�ړ��j
	};

} } }
