#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �Q�c�����p�R�w�Q�s��N���X
/// </summary>
class CMatrix3x2
{
public:

    /// <summary>
    /// �v���p�e�B
    /// </summary>
	union 
	{
		struct 
		{
			dectype	M11, M12;
			dectype	M21, M22;
			dectype	M31, M32;
			dectype	Tx ,  Ty;
		};
		dectype m[ 3 ][ 2 ];
	};

	/// <summary>
    /// �R���X�g���N�^
    /// </summary>
	CMatrix3x2();

	/// <summary>
    /// �f�X�g���N�^
    /// </summary>
	virtual ~CMatrix3x2();

    /// <summary>
    /// �P���s���ݒ肷��
    /// </summary>
	void	Identity();

    /// <summary>
    /// �s��̕��s�ړ������𒼐ڃA�N�Z�X����
    /// �Ō�̍s��[0,0]�ɐݒ肵���`�ϊ������i�Q�~�Q�̕����j�����̂܂܎c�����ƂŁA
    /// ���̍s��̕��s�ړ���������菜���܂��B
    /// </summary>
	void	ZeroTranslation();

    /// <summary>
    /// �s��̕��s�ړ������𒼐ڃA�N�Z�X����
	///
	/// �R�s�ڕ��������L�̂悤�ɂ���
	/// | posX ,posY |
    /// �s��̕��s�ړ��������w�肳�ꂽ�l�ɐݒ肵�A�Q�~�Q�̕����͕ύX���܂���B
	///
    /// </summary>
	void	SetTranslation( const CBasicVector2 &d );

    /// <summary>
    /// ��]�s���ݒ肷��
	/// 
	/// �Q�~�Q���������L�̂悤�ɂ���
	/// | cos��,-sin��|
	/// | sin��,cos�� |
	/// ���s�ړ������͕ύX���܂���B
	///
    /// </summary>
	void	SetupRotate( dectype theta );

    /// <summary>
    /// �g�k�s����쐬����
	/// 
	/// �Q�~�Q���������L�̂悤�ɂ���
	/// | sc, 0  |
	/// | 0,  sy |
	/// ���s�ړ������͕ύX���܂���B
	///
    /// </summary>
	void	SetupScale( const CBasicVector2 &sc );

	/// <summary>
	/// ==���Z�q�I�[�o�[���C�h�i�v�f�̔�r�j
	/// </summary>
	skyBool operator ==( const CMatrix3x2 &m ) const;

	/// <summary>
	/// !=���Z�q�I�[�o�[���C�h�i�v�f�̔�r�j
	/// </summary>
	skyBool operator !=( const CMatrix3x2 &m ) const;

};

} } }

/// <summary>
/// ���Z�q�̃I�[�o�[���C�h
/// ����́A���̏�̐��`�㐔�L�@�Ɏ����ăx�N�g���N���X���g����悤�ɂ���
/// </summary>
sky::lib::math::CBasicVector2	 operator*( const sky::lib::math::CBasicVector2 &p , const sky::lib::math::CMatrix3x2 &m );
sky::lib::math::CMatrix3x2		 operator*( const sky::lib::math::CMatrix3x2 &m1 , const sky::lib::math::CMatrix3x2 &m2 );
sky::lib::math::CBasicVector2	&operator*=( sky::lib::math::CBasicVector2 &p , const sky::lib::math::CMatrix3x2 &m );
sky::lib::math::CMatrix3x2		&operator*=( sky::lib::math::CMatrix3x2 &m1 , const sky::lib::math::CMatrix3x2 &m2 );
sky::lib::math::CBasicVector2	GetTranslationVector( const sky::lib::math::CMatrix3x2 &m );
sky::lib::math::CBasicVector2	operator*( const sky::lib::math::CBasicVector2 &p , const sky::lib::math::CMatrix3x2 &m );
