#pragma once

namespace sky { namespace lib { namespace math {

class CEulerAngles;

class CQuaternion;
typedef CVector< CQuaternion >  CQuaternionVector;

/// <summary>
/// �l�����N���X
/// 
/// �l������W,X ,Y,Z�̂S�̐��l��p���ĉ�]��\�����܂��B
/// ���͂�X�l,Y�l,Z�l���A���x�N�g���Ɖ�]�p�x�ł��B
/// ���Ӂj�]�����ƃx�N�g���̊|���Z�̌��ʂ̃x�N�g���̒l�Ɂu2.2204460492503131e-016�v�Ƃ����l�������Ă��邱�Ƃ����邪���̒l�́u�O�Ɍ���Ȃ��߂������v�ł���A
///       SKY_PRINTF�Ȃǂŏo�͂����0�Əo��Bfloat.h�ŁuDBL_EPSILON�v�Ƃ����}�N���Œ�`����Ă���B�u-4.3711390063094768e-008�v�����l��0�ł���B
/// </summary>
class CQuaternion
{

public:

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
	CQuaternion();
	CQuaternion( dectype x , dectype y , dectype z , dectype w );
	CQuaternion( const dectype q[ 4 ] );

    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
	virtual ~CQuaternion();

    /// <summary>
    /// �v���p�e�B
    /// </summary>
	union 
	{
		struct 
		{
			dectype	W, X , Y, Z;
		};
		dectype q[ 4 ];
	};

    /// <summary>
    /// �x�N�g����ݒ肷��
    /// </summary>
	CQuaternion &SetVector( const CBasicVector3 &vec );

    /// <summary>
    /// �x�N�g�����w�肵�č쐬����
    /// </summary>
	CQuaternion	&Setup( const CBasicVector3 &vec );

    /// <summary>
    /// �P���l����
    /// </summary>
	CQuaternion &Identity() { W = 1.0f; X = Y = Z = 0.0f; return *this; }

    /// <summary>
    /// X�l����l���������̊p�x�ɃZ�b�g�A�b�v����
    /// </summary>
	CQuaternion &SetToRotateAboutX( dectype radian );

    /// <summary>
    /// Y�l����l���������̊p�x�ɃZ�b�g�A�b�v����
    /// </summary>
	CQuaternion &SetToRotateAboutY( dectype radian );

    /// <summary>
    /// Z�l����l���������̊p�x�ɃZ�b�g�A�b�v����
    /// </summary>
	CQuaternion &SetToRotateAboutZ( dectype radian );

    /// <summary>
    /// ���x�N�g���Ɖ�]�p�x����l���������̊p�x�ɃZ�b�g�A�b�v����
    /// ���x�N�g��axis�͐��K������Ă��邱�ƁI
    /// </summary>
	CQuaternion &SetToRotateAboutAxis( const CBasicVector3 &axis , dectype radian );

    /// <summary>
    /// �Q�̃x�N�g������N�I�[�^�j�I���𐶐�����
    /// </summary>
	CQuaternion &SetupVector( const CBasicVector3 &vec1 , const CBasicVector3 &vec2 );

    /// <summary>
    /// �I�u�W�F�N�g��ԁ�������Ԃւ̉�]�����s����l�������Z�b�g�A�b�v����
    /// �����̓I�C���[�p�`���ŗ^������
    /// </summary>
	CQuaternion &SetToRotateObjectToInertial( const CEulerAngles &orientation );

    /// <summary>
    /// ������ԁ��I�u�W�F�N�g��Ԃւ̉�]�����s����l�������Z�b�g�A�b�v����
    /// �����̓I�C���[�p�`���ŗ^������
    /// </summary>
	CQuaternion &SetToRotateInertialToObject( const CEulerAngles &orientation );

    /// <summary>
    /// *���Z�q�̃I�[�o�[���C�h
    /// </summary>
	CQuaternion operator *( const CQuaternion &a ) const;
	CQuaternion &operator *( const dectype &v );

    /// <summary>
    /// *=���Z�q�̃I�[�o�[���C�h
    /// </summary>
	CQuaternion &operator *=( const CQuaternion &a );

    /// <summary>
    /// +=���Z�q�̃I�[�o�[���C�h
    /// </summary>
	CQuaternion &operator +=( const CQuaternion &a );

	/// <summary>
	/// �l�����𐳋K������
	/// ���ӂ��ׂ��_�́A�ʏ�A�l�����͏�Ɂi���l�̐��x�͈͓̔��Łj���K������Ă��邱�Ƃł���
	/// �ڍׂ�9.4.6�ߎQ��
	///
	/// ���̊֐��͎�ɁA���������_�̌덷�̐i����h�����߂Ɏg����
	/// �덷�̐N���́A��������̘A�������l�������Z���K�p���ꂽ�ꍇ�ɔ�������ꍇ������B
	/// </summary>
	CQuaternion &Normalize();

    /// <summary>
    /// ��]�p�Ƃ�Ԃ�
    /// </summary>
	dectype	GetRotationAngle() const;

    /// <summary>
    /// ��]����Ԃ�
    /// </summary>
	CBasicVector3	GetRotationAxis() const;
};

} } }

/// <summary>
/// �P���l����
/// </summary>
extern const sky::lib::math::CQuaternion skyQuaternionIdentity;

/// <summary>
/// �l�����̓���
/// �񃁃��o�֐���p����̂ŁA�t�@���L�[�ȍ\����p������y�����\����
/// ���Z���Ƃ��ēn�����Ƃ��ł���
/// </summary>
extern dectype DotProduct(const sky::lib::math::CQuaternion &a , const sky::lib::math::CQuaternion &b);

/// <summary>
/// ���ʐ��`���
/// q1��q2�̊Ԃ̐��`�⊮����0.0d��1.0d�̊Ԃ̐��l�ŕ\������B
/// t��0.1d 0.2d��1.0d�ɋ߂Â��ɂ�q2�ɋ߂Â��Ƃ������Ƃ��낤�B
/// </summary>
extern sky::lib::math::CQuaternion Slerp(const sky::lib::math::CQuaternion &p, const sky::lib::math::CQuaternion &q, dectype t);

/// <summary>
/// �l�����̋������v�Z����
/// ����́A���̎l�����̔��΂̉�]�����l�����ƂȂ�
/// </summary>
extern sky::lib::math::CQuaternion Conjugate(const sky::lib::math::CQuaternion &q);

/// <summary>
/// �l�����̗ݏ�
/// </summary>
extern sky::lib::math::CQuaternion Pow(const sky::lib::math::CQuaternion &q, dectype exponent);

/// <summary>
/// �x�N�g���Ǝl�����̊|���Z
/// </summary>
sky::lib::math::CBasicVector3	operator*( const sky::lib::math::CBasicVector3 &vec , const sky::lib::math::CQuaternion &q );
sky::lib::math::CBasicVector3	&operator*=( sky::lib::math::CBasicVector3 &vec , const sky::lib::math::CQuaternion &q );
sky::lib::math::CQuaternion   	operator*( const sky::lib::math::CQuaternion &q , dectype val );

