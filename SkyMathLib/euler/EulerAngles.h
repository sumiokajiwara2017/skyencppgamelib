#pragma once

namespace sky { namespace lib { namespace math {

class CQuaternion;
class CMatrix4x3;
class CRotationMatrix;

/// <summary>
/// �I�C���[�p�N���X
/// 
/// �I�C���[�p�̓w�f�B���O�A�s�b�`�A�o���N�Ƃ����R�̊p�x���l��p����
/// ��]��\�����܂��B�iz-y-z���n�j
/// 
/// </summary>
class CEulerAngles
{
public:

    /// <summary>
    /// �����o�ϐ�
    /// </summary>
	dectype	Heading;
	dectype	Pitch;
	dectype	Bank;

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
	CEulerAngles() {}
	CEulerAngles( dectype h , dectype p , dectype b ) :
		Heading( h ) , Pitch( p ) , Bank( b ) {}

    /// <summary>
    /// �P�����i�S���O�j����
    /// </summary>
	void	Identity() { Pitch = Bank = Heading = 0.0f; }


    /// <summary>
    /// �����I�C���[�p�̂R�g�����肷��
    /// �w�f�B���O�ƃo���N��+-180�x�Ƀs�b�`��+-90�x�ɐ�������
    /// �܂��L�͈͂Ƀ����o�ϐ���ϊ�����
    /// </summary>
	void	Canonize();

    /// <summary>
    /// �l��������͂Ɏ󂯎��I�C���[�p�ɕϊ�����(�I�u�W�F�N�g��ԁ�������ԁj
    /// ������ԂƂ̓I�u�W�F�N�g��Ԃƃ��[���h��Ԃ̒��ԁA�ʒu���I�u�W�F�N�g��ԂŁA��]�����[���h��Ԃ̏�Ԃ̂��ƁB
    /// ����`���̖{���Ȃ�ł���ȋ�Ԓ�`�����Ă���̂����܂������ł��Ă��Ȃ��悤�ȁB�v����ɕ��s�ړ��Ɖ�]�𕪂��ĉ�������������̂ł��傤���B
    /// </summary>
	void	FromObjectToInertialQuaternion( const CQuaternion &q );

    /// <summary>
    /// �l��������͂Ɏ󂯎��I�C���[�p�ɕϊ�����(������ԁ��I�u�W�F�N�g��ԁj
    /// </summary>
	void	FromInertialToObjectQuaternion( const CQuaternion &q );

    /// <summary>
    /// 4x3�s�����͂ɃI�C���[�p�ɕϊ�����i�I�u�W�F�N�g��ԁ����[���h��ԁj
    /// </summary>
	void	FromObjectToWorldMatrix( const CMatrix4x3 &m );

    /// <summary>
    /// 4x3�s�����͂ɃI�C���[�p�ɕϊ�����i���[���h��ԁ��I�u�W�F�N�g��ԁj
    /// </summary>
	void	FromWorldToObjectMatrix( const CMatrix4x3 &m );

    /// <summary>
    /// 3x3�s�����͂ɃI�C���[�p�ɕϊ�����
    /// </summary>
	void	FromRotationMatrix( const CRotationMatrix &m );
};

} } }

/// <summary>
/// �P���s��
/// </summary>
extern const sky::lib::math::CEulerAngles skyCEulerAnglesIdentity;