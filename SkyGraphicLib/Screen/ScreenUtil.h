#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// �X�N���[���֘A�֗��֐��Q
/// �X�N���[���Ƃ́A�`���ʔ͈́BWindows�Ȃ�Window�T�C�Y�i���j�A�ƒ�p�Q�[���@�Ȃ�e���r�̏c���T�C�Y�i���j�A
/// ���ʏ큁�`��̈�ɂȂ邪�A�S�F�R��Window,�������̓e���r���P�U�F�X�ɕ␳����ꍇ�㉺�ɗ]�肪�ł��邽�߁A���`��̈�ł͂Ȃ��Ȃ�B
/// </summary>
class CScreenUtil : base::SkyObject
{

public:

	/// <summary>
	/// �r���[�|�[�g���쐬
    /// �r���[�|�[�g�͕��ƍ�����0.0f�`1.0f�̋�`
	/// </summary>
	static math::CBasicRect CreateViewPort( const math::CBasicSize &screenSize , dectype screenAspectRatio );

	/// <summary>
	/// �s�N�Z�����W��0.0f �` 1.0f�̊Ԃ̃s�N�Z�����W�ɕϊ����܂�
	/// </summary>
	static math::CBasicVector2 PixelToPoint( const math::CBasicVector2& pixelPosition , const math::CBasicSize &screenSize , dectype width = screen::VIRTUAL_SCREEN_W , dectype height = screen::VIRTUAL_SCREEN_H );

	/// <summary>
	/// �X�N���[���T�C�Y���擾
    /// ���̃V�X�e���ɂ�����X�N���[���̓E�B���h�E�ƂP�F�P�ł���A�X�N���[���T�C�Y�Ƃ̓E�B���h�E�T�C�Y�Ɠ����B�������E�B���h�E�͕������݂���\�������邽�߁A���X�N���[���ł͂Ȃ��B
	/// </summary>
	static math::CBasicSize GetScreenSize( const math::CBasicSize &windowSize );

	/// <summary>
	/// �V�l�}�X�R�[�v�̗]���̊�����Ԃ��i�V�l�}�X�R�[�v�͏㉺�ɋϓ��ȗ]�����ł���ꍇ�����邪�A���̊֐��͂��̕Е��̃X�N���[���̍����ɂ����銄����Ԃ��j
	/// </summary>
	/*
	*	�Ⴆ�΁A�f�t�H���g�����ŁA�X�N���[����1280*720�̉𑜓x�̏ꍇ��0.0�i�]���Ȃ��j�@800*600�̉𑜓x�̏ꍇ��0.125���Ԃ�܂��B
	*/
    static dectype GetCinemaScopeHeight( const math::CBasicSize &screenSize ,  dectype screenAspectRatio = screen::VIRTUAL_SCREEN_W / screen::VIRTUAL_SCREEN_H );

	/// <summary>
	/// �w��̉�ʔ䗦( width : height )�ɕ␳�������Ɏg�p�ł����ʂ̍����̊������擾
	/// </summary>
	/*
	*	�Ⴆ�΁A�f�t�H���g�����ŁA�X�N���[����1280*720�̉𑜓x�̏ꍇ��1.0�@800*600�̉𑜓x�̏ꍇ��0.75���Ԃ�܂��B
	*/
	static dectype GetValidScreenHeightRateInVirtualScreen( const math::CBasicSize &screenSize , dectype width = screen::VIRTUAL_SCREEN_W , dectype height = screen::VIRTUAL_SCREEN_H );

	/// <summary>
	/// �X�N���[���̃s�N�Z���̕��ƍ����̊�����Ԃ��i���̒l�Ɏ��ۂ̉�ʂ̕��ƍ��������ꂼ�ꂩ����ƁA1�s�N�Z���̕��ƍ����ɂȂ�j
	/// </summary>
	static math::CBasicSize GetViewPortSpacedPixelSize( const math::CBasicSize &screenSize , dectype width = screen::VIRTUAL_SCREEN_W , dectype height = screen::VIRTUAL_SCREEN_H );

};

} } }

#define CScreenUtil_	sky::lib::graphic::CScreenUtil
