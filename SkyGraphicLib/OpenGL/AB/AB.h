#pragma once

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// �A�L�������[�V�����o�b�t�@����N���X
/// �A�L�������[�V�����o�b�t�@���̂�OpenGL�����ɂ���A���̃N���X�̃����o�ɂ͂Ȃ��ł��B
/// �A�L�������[�V�����o�b�t�@���g�p����ꍇ��PIXELFORMATDESCRIPTOR�̍\���̂ŃA�L�������[�V�����o�b�t�@�̕�����0�ȊO�ɂ���
/// </summary>
class CAccumulationBuffer
{
	/// <summary>
	/// �A�L�������[�V�����o�b�t�@�̃N���A�J���[�ݒ�
	/// </summary>
	inline void SetClearColor( const CColor &color );

	/// <summary>
	/// �A�L�������[�V�����o�b�t�@���N���A����
	/// </summary>
	inline void Clear();

	/// <summary>
	/// ���݉�ʂɕ\������Ă�����̂��A�L���[���o�b�t�@�ɓ]������
	/// </summary>
	inline void TransmitFromColorBuffer( f32 rate );

	/// <summary>
	/// ���݉�ʂɕ\������Ă�����̂��A�L���[���o�b�t�@�ɉ��Z����
	/// </summary>
	inline void AddFromColorBuffer( f32 rate );

	/// <summary>
	/// �A�L���[���o�b�t�@�Ɏw��̒l�����Z����
	/// </summary>
	inline void Add( f32 rate );

	/// <summary>
	/// �A�L���[���o�b�t�@�Ɏw��̒l���|����
	/// </summary>
	inline void Mul( f32 rate );

	/// <summary>
	/// �A�L���[���o�b�t�@�̓��e����ʕ`��̈�ɓ]������i��ʂɕ\������j
	/// </summary>
	inline void TransmitToColorBuffer( f32 rate );
};

} } }

#endif

#include "AB.inl"