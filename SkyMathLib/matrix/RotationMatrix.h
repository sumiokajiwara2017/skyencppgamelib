#pragma once

namespace sky { namespace lib { namespace math {

class CEulerAngles;
class CQuaternion;

typedef CList< u32 , CRotationMatrix > CRotationMatrixList;
typedef CVector< CRotationMatrix >     CRotationMatrixVector;

/// <summary>
/// ��]�s��
/// 
/// ------------------------------------------------------------------------------------------------------
/// ���̃N���X�̃��[�U�[�́A���̍s�񂪂ǂ̂悤�ɍ\������Ă��邩���C�ɂ��Ȃ��ł悢
/// �������Ȃ���A�����I�ɂ��ׂĂ̂��Ƃ��������肳���Ă������Ƃ͏d�v�ł���B
/// 
/// ���̍s��́A��]�s�񂾂��ł��邱�Ƃ�O��Ƃ��A���������āA���s���Ă���
/// ���W�ϊ��̐������i���ꂪ���ۂɂ��̏ꍇ�ɓ��Ă͂܂�̂ł���΁j�́A
/// ������Ԃ���I�u�W�F�N�g��Ԃւł���
/// �I�u�W�F�N�g��ԁ�������Ԃ̉�]�����s����ꍇ�́A���̓]�u����Z����B
/// 
/// ����������ƁA
/// 
/// �y������Ԃ���I�u�W�F�N�g��ԁz
/// 
///  ������ԍ��W �� �s�� �� �I�u�W�F�N�g��ԍ��W
/// 
///               | m11 m12 m13 |
///  { ix iy iz } | m21 m22 m23 | = { ox oy oz }
///               | m31 m32 m33 |
/// �y�I�u�W�F�N�g��Ԃ��犵����ԁz
/// 
///  �I�u�W�F�N�g��ԍ��W �� �s�s�� �� ������ԍ��W
/// 
///               | m11 m21 m31 |
///  { ox oy oz } | m12 m22 m32 | = { ix iy iz }
///               | m13 m23 m33 |
/// 
/// </summary>
class CRotationMatrix
{
public:

    /// <summary>
    /// �v���p�e�B
    /// </summary>
	union 
	{
		struct 
		{
			dectype	M11, M12, M13;
			dectype	M21, M22, M23;
			dectype	M31, M32, M33;
		};
		dectype m[ 3 ][ 3 ];
	};

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
	inline CRotationMatrix();

    /// <summary>
    /// �s����P���s���ݒ肷��
    /// </summary>
	inline CRotationMatrix	&Identity();

    /// <summary>
    /// ��{���̎���̉�]�����s����s����Z�b�g�A�b�v����
    /// 
    /// ��]�̎��́A�ȉ��̂悤�ɂP���x�[�X�ɂ����C���f�b�N�X��p���Ďw�肳���
    /// 
    /// �P���w���Ɋւ����]
    /// �Q���x���Ɋւ����]
    /// �R���y���Ɋւ����]
    /// 
    /// theta�͉�]�̗ʁi���W�A���j�A����̃��[���Ő��̉�]���`����
    /// 
    /// </summary>
	inline CRotationMatrix	&Setup( eAxis axis , dectype theta );

    /// <summary>
    /// �w��̎��ɂ����ĉ�]����s����Z�b�g�A�b�v����
    /// </summary>
	inline CRotationMatrix	&Setup( const CBasicVector3 &axis , dectype theta );

    /// <summary>
    /// �w�肳�ꂽ�����i�I�C���[�p�j�ōs����Z�b�g�A�b�v����
    /// </summary>
	CRotationMatrix	&Setup( const CEulerAngles &orientation );

    /// <summary>
    /// �w��̃x�N�g�������]�}�g���b�N�X�𐶐����܂��B���( 0 , 0 , 1 )�����ł��B( 0 , 0 , 1 )��ݒ肷��Ɖ�]���Ȃ��s�񂪐�������܂��B
    /// </summary>
	CRotationMatrix	&Setup( const CBasicVector3 &vec );

	/// <summary>
    /// �s����Z�b�g�A�b�v����
    /// ������ԁ��I�u�W�F�N�g��Ԃ̉�]�����s����l������O��Ƃ���
    /// </summary>
	CRotationMatrix	&FromInertialToObjectQuaternion(const CQuaternion& q);

    /// <summary>
    /// �s����Z�b�g�A�b�v����
    /// �I�u�W�F�N�g��ԁ�������Ԃ̉�]�����s����l������O��Ƃ���
    /// </summary>
	CRotationMatrix	&FromObjectToInertialQuaternion(const CQuaternion& q);

    /// <summary>
    /// �R���̌������擾����(�P���s��̏ꍇ�w����( 1.0f , 0.0f , 0.0f ) �x����( 0.0f , 1.0f , 0.0f ) �y����( 0.0f , 0.0f , 1.0f )
	/// isNormalizeExec:���K�����邩���Ȃ���
    /// </summary>
	inline CRotationMatrix	&GetDirection( eAxis axis , CBasicVector3 &dest );

	/// <summary>
	/// ==���Z�q�I�[�o�[���C�h�i�v�f�̔�r�j
	/// </summary>
	inline skyBool operator ==( const CRotationMatrix &m ) const;

	/// <summary>
	/// !=���Z�q�I�[�o�[���C�h�i�v�f�̔�r�j
	/// </summary>
	inline skyBool operator !=( const CRotationMatrix &m ) const;

    /// <summary>
    /// �x�N�g����������Ԃ���I�u�W�F�N�g��Ԃ։�]����
    /// </summary>
	inline CBasicVector3	InertialToObject( const CBasicVector3& v ) const;

    /// <summary>
    /// �x�N�g�����I�u�W�F�N�g��Ԃ��犵����Ԃ։�]����
    /// </summary>
	inline CBasicVector3	ObjectToInertial( const CBasicVector3& v ) const;

    /// <summary>
    /// �R���̊p�x���󂯎��A�s����쐬����
    /// </summary>
	inline CRotationMatrix	&SetUpXYZ( const CBasicVector3 &vec );
	inline CRotationMatrix	&SetUpXZY( const CBasicVector3 &vec );
	inline CRotationMatrix	&SetUpYXZ( const CBasicVector3 &vec );
	inline CRotationMatrix	&SetUpYZX( const CBasicVector3 &vec );
	inline CRotationMatrix	&SetUpZXY( const CBasicVector3 &vec );
	inline CRotationMatrix	&SetUpZYX( const CBasicVector3 &vec );
};

} } }

/// <summary>
/// ���Z�q�̃I�[�o�[���C�h
/// </summary>
inline sky::lib::math::CRotationMatrix operator+(const sky::lib::math::CRotationMatrix &a , const sky::lib::math::CRotationMatrix &b);
inline sky::lib::math::CRotationMatrix operator*(const sky::lib::math::CRotationMatrix &a , const sky::lib::math::CRotationMatrix &b);
inline sky::lib::math::CRotationMatrix operator*(const sky::lib::math::CRotationMatrix &a , const sky::lib::math::CQuaternion &q);
inline sky::lib::math::CRotationMatrix &operator*=(const sky::lib::math::CRotationMatrix &m1 , const sky::lib::math::CRotationMatrix &m2 );
inline sky::lib::math::CRotationMatrix &operator*=(const sky::lib::math::CRotationMatrix &m , const sky::lib::math::CQuaternion &q);
inline sky::lib::math::CRotationMatrix &operator+=(const sky::lib::math::CRotationMatrix &m1 , const sky::lib::math::CRotationMatrix &m2 );
inline sky::lib::math::CRotationMatrix Invert3x3(const sky::lib::math::CRotationMatrix &m);
inline sky::lib::math::CRotationMatrix PseudoInverse( const sky::lib::math::CRotationMatrix &m ); //�[���t�s����쐬
inline sky::lib::math::CRotationMatrix Transpose( sky::lib::math::CRotationMatrix m );

#include "RotationMatrix.inl"