#include "StdAfx.h"
#include "SkyMathLib/Euler.h"

const sky::lib::math::CEulerAngles skyCEulerAnglesIdentity(0.0f , 0.0f , 0.0f);

namespace sky { namespace lib { namespace math {

void	CEulerAngles::Canonize()
{
    //�s�b�`�͈͂�-180�x�`180�x�Ƀ��b�v(-180�x�`180�x�͈͕\���ɕϊ��j����

	Pitch = CMathUtil::WrapPi( Pitch );

    //�����͈� -90�x...90�x�̊O���ōs��pitch�̗������`�F�b�N����
    //�s�b�` < 90�x

	if (Pitch < -util::KPIOVER2)
	{
		Pitch = -util::KPI - Pitch;		//-180�x - �s�b�`
		Heading += util::KPI;				//�w�f�B���O += 180�x
		Bank += util::KPI;				//�o���N += 180�x
	}
	else if (Pitch > util::KPIOVER2)
	{
		Pitch = util::KPI - Pitch;		//180�x - �s�b�`
		Heading += util::KPI;				//�w�f�B���O += 180�x
		Bank += util::KPI;				//�o���N
	}

     //�s�b�` > 90�x�ȏ�

	// �����ŃW���o�����b�N�̃P�[�X���`�F�b�N����i���e�͈͂�����(1e-4)���j
	if ( CMathUtil::FAbs( Pitch ) > util::KPIOVER2 - 1e-4 )
	{

        //�W���o�����b�N���Ă���
        //�������Ɋւ���S�Ẳ�]���w�f�B���O�Ɋ��蓖�Ă�iZ����]��X����]�Ɋ��蓖�Ă�j
		Heading += Bank;
		Bank = 0.0f;

	} else {

        //�W���o�����b�N�ɂȂ�
        //�o���N�p�𐳏��͈͂Ƀ��b�v(-180�x�`180�x�͈͕\���ɕϊ��j����
		Bank = CMathUtil::WrapPi(Bank);
	}

	//�w�f�B���O�𐳏��͈͂Ƀ��b�v(-180�x�`180�x�͈͕\���ɕϊ��j����
	Heading = CMathUtil::WrapPi(Heading);
}

void	CEulerAngles::FromObjectToInertialQuaternion(const CQuaternion &q) {

    //sin(pitch)�����o���B

	dectype sp = -2.0f * (q.Y*q.Z - q.W*q.X);

    //�W���o�����b�N�`�F�b�N������
    //�s���m���̂��ߋ��e�͈͂������^����
	if (CMathUtil::FAbs(sp) > 0.9999f) {

        //�^�ォ�^���������Ă���
		Pitch = util::KPIOVER2 * sp;

	    //�w�f�B���O���v�Z���A�o���N���O�ɐݒ肷��
		Heading = CMathUtil::Atan2(-q.X*q.Z + q.W*q.Y, 0.5f - q.Y*q.Y - q.Z*q.Z);
		Bank = 0.0f;

	} else {

        //�p�x���v�Z����
        //���S��asin�֐���p����K�v�͂Ȃ�
        //�W���o�����b�N�`�F�b�N���ɔ͈͂����łɃ`�F�b�N���Ă��邩��ł���B

		Pitch	= CMathUtil::Asin(sp);
		Heading	= CMathUtil::Atan2(q.X*q.Z + q.W*q.Y, 0.5f - q.X*q.X - q.Y*q.Y);
		Bank	= CMathUtil::Atan2(q.X*q.Y + q.W*q.Z, 0.5f - q.X*q.X - q.Z*q.Z);
	}
}

void	CEulerAngles::FromInertialToObjectQuaternion(const CQuaternion &q) {

    //sin(pitch)�����o���B
	dectype sp = -2.0f * (q.Y*q.Z + q.W*q.X);

    //�W���o�����b�N���`�F�b�N����
    //�����̕s���m���̂��߂ɂ킸���ȋ��e�͈͂�^����
	if (CMathUtil::FAbs(sp) > 0.9999f) {

        //�^���������Ă���
		Pitch = util::KPIOVER2 * sp;

        //�w�f�B���O���v�Z���A�o���N��0�ɐݒ肷��
		Heading = CMathUtil::Atan2(-q.X*q.Z - q.W*q.Y, 0.5f - q.Y*q.Y - q.Z*q.Z);
		Bank = 0.0f;

	} else {

        //�p�x���v�Z����
        //���S��asin�֐���p����K�v�͂Ȃ�
        //�W���o�����b�N�`�F�b�N���ɗt�ɂ����łɃ`�F�b�N���Ă��邩��ł���B

		Pitch	= CMathUtil::Asin(sp);
		Heading	= CMathUtil::Atan2(q.X*q.Z - q.W*q.Y, 0.5f - q.X*q.X - q.Y*q.Y);
		Bank	= CMathUtil::Atan2(q.X*q.Y - q.W*q.Z, 0.5f - q.X*q.X - q.Z*q.Z);
	}
}

void	CEulerAngles::FromObjectToWorldMatrix(const CMatrix4x3 &m) {

    //m32 ���� sin(pitch)�����o���B

	dectype	sp = -m.M32;

    //�W���o�����b�N���`�F�b�N����
	
	if (CMathUtil::FAbs(sp) > 9.99999f) {

        //�W���o�����b�N���`�F�b�N����

		Pitch = util::KPIOVER2 * sp;

        // �^�ォ�^���������Ă���

		Heading = CMathUtil::Atan2(-m.M23, m.M11);
		Bank = 0.0f;

	}
	else
	{
        //�p�x���v�Z����
        //���S��asin�֐���p����K�v�͂Ȃ�
        //�W���o�����b�N�`�F�b�N���ɔ͈͂����łɃ`�F�b�N���Ă��邩��ł���
 
		Heading = CMathUtil::Atan2(m.M31, m.M33);
		Pitch = CMathUtil::Asin(sp);
		Bank = CMathUtil::Atan2(m.M12, m.M22);
	}
}

void	CEulerAngles::FromWorldToObjectMatrix(const CMatrix4x3 &m) {

    //m23����sin(pitch)�����o���B
	dectype	sp = -m.M23;

    //�W���o�����b�N���`�F�b�N����
	if (CMathUtil::FAbs(sp) > 9.99999f) {

        //�^�ォ�^���������Ă���

		Pitch = util::KPIOVER2 * sp;

        //�w�f�B���O���v�Z���A�o���N��0�ɐݒ肷��

		Heading = CMathUtil::Atan2(-m.M31, m.M11);
		Bank = 0.0f;

	} else {

        //�p�x���v�Z����
        //���S��asin�֐���p����K�v�͂Ȃ�
        //�W���o�����b�N�`�F�b�N���ɔ͈͂����łɃ`�F�b�N���Ă��邩��ł���

		Heading = CMathUtil::Atan2(m.M13, m.M33);
		Pitch = CMathUtil::Asin(sp);
		Bank = CMathUtil::Atan2(m.M21, m.M22);
	}
}

void	CEulerAngles::FromRotationMatrix(const CRotationMatrix &m) {

    //m23����sin(pitch)�����o��
	dectype	sp = -m.M23;

    //�W���o�����b�N���`�F�b�N����
	if (CMathUtil::FAbs(sp) > 9.99999f) {

        //�^�ォ�^���������Ă���
		Pitch = util::KPIOVER2 * sp;

        //�w�f�B���O���v�Z���A�o���N��0�ɐݒ肷��
		Heading = CMathUtil::Atan2(-m.M31, m.M11);
		Bank = 0.0f;

	} else {

        //�p�x���v�Z����
        //���S��asin�֐���p����K�v�͂Ȃ�
        //�W���o�����b�N�`�F�b�N���ɔ͈͂����łɃ`�F�b�N���Ă��邩��ł���

		Heading = CMathUtil::Atan2(m.M13, m.M33);
		Pitch = CMathUtil::Asin(sp);
		Bank = CMathUtil::Atan2(m.M21, m.M22);
	}
}

} } }