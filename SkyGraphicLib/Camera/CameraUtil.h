#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// �J�����֗��֐��Q
/// </summary>
namespace CameraUtil
{
	/// <summary>
	/// �X�N���[�����W���R�c��Ԃɂ�������W�ɕϊ�����
	/// �X�N���[�����W�ɂ������ʂy�̏��͂ǂ����Ă�������̂ŁA�C�ӂ̋���(distance)�̐�ɂ���_�Ƃ��ĉ��߂���
	/// </summary>
	math::CBasicVector3 ConvertScreenPosTo3DPos( const math::CBasicVector2 &screenPos , const CWindowPtr &spWindow , const CameraPtr &spCamera , dectype distance = 1000 );

	/// <summary>
	/// �J�����̈ʒu����������̎��_�ɂ���
	/// </summary>
	void SetDefaultCameraPos_Left( const CameraPtr &dest , dectype distance = CAMERA_DEFAULT_DISTANCE );

	/// <summary>
	/// �J�����̈ʒu���E������̎��_�ɂ���
	/// </summary>
	void SetDefaultCameraPos_Right( const CameraPtr &dest , dectype distance = CAMERA_DEFAULT_DISTANCE );

	/// <summary>
	/// �J�����̈ʒu���ォ��̎��_�ɂ���
	/// </summary>
	void SetDefaultCameraPos_Top( const CameraPtr &dest , dectype distance = CAMERA_DEFAULT_DISTANCE );
};

} } }