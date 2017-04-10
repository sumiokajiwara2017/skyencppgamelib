#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �萔
/// <summary>
namespace util
{
	static const dectype KPI		    = ( dectype )3.14159265f;
	static const dectype K2PI		    = ( dectype )KPI * 2.0f;
	static const dectype KPIOVER2	    = ( dectype )KPI / 2.0f;
	static const dectype K1OVERPI	    = ( dectype )1.0f / KPI;
	static const dectype K1OVER2PI	    = ( dectype )1.0f / K2PI;
	static const dectype KPIOVER180	    = ( dectype )KPI / 180.0f;
	static const dectype K180OVERPI	    = ( dectype )180.0f / KPI;
	static const dectype EPSILON        = ( dectype )1e-08;
    static const dectype EPSILON_LENGTH = ( dectype )1 - EPSILON;
}

//-------------------------------------< ���� >---------------------------------------------//

/// <summary>
/// ���w�֘A���ʏ���
/// </summary>
class CMathUtil : public base::SkyObject
{

public:

	/// <summary>
	/// ������(�Q�悵����j�����߂�
	/// </summary>
	static inline dectype Sqrt( dectype val );

	/// <summary>
	/// ������Ԃ��܂��B�P�ʉ~�����Y���W�̒l
	/// ��������Ӂ^�Ε�
	/// ����(90�x) = 1.0
	/// ����(45�x) = 0.7071067811865476
	/// ����(0�x)  = 0.0
	/// </summary>
	static inline dectype Sin( dectype asin );

	/// <summary>
	/// Sin�̋t�֐����v�Z���A
	/// ���ʂ� 0 ���� �� �܂ł̃��W�A���ŕԂ��܂��B
	/// �t����(1.0)					= 90�x�i�̃��W�A���\�L�j
	/// �t����(0.7071067811865476)	= 45�x�i�̃��W�A���\�L�j
	/// �t����(0.0)					= 0�x�i�̃��W�A���\�L�j
	/// </summary>
	static inline dectype Asin( dectype sin );

	/// <summary>
	/// �]����Ԃ��܂��B�P�ʉ~�����X���W�̒l
	/// �]�����ΕӁ^���
	/// �]��(90�x) = 0.0
	/// �]��(45�x) = 0.7071067811865476
	/// �]��(0�x)  = 1.0
	/// </summary>
	static inline dectype Cos( dectype acos );

	/// <summary>
	/// Cos�̋t�֐����v�Z���A
	/// ���ʂ� 0 ���� �� �܂ł̃��W�A���ŕԂ��܂��B
	/// �t�]��(0.0)					= 90�x�i�̃��W�A���\�L�j
	/// �t�]��(0.7071067811865476)	= 45�x�i�̃��W�A���\�L�j
	/// �t�]��(1.0)					= 0�x�i�̃��W�A���\�L�j
	/// </summary>
	static inline dectype Acos( dectype cos );

	/// <summary>
	/// ���ڂ��v�Z�Ԃ��܂�
	/// ���ځ��וӁ^���
	/// ����(0�x)	= 0.0 
	/// ����(45�x)	= 1.0 
	/// ����(90�x)	= �� 
	/// </summary>
	static inline dectype Tan( dectype atan );

	/// <summary>
	/// Tan�̋t�֐�
	/// ���ʂ� 0 ���� �� �܂ł̃��W�A���ŕԂ��܂��B
	/// �t����(0.0) = 0�x�i�̃��W�A���\�L�j
	/// �t����(1.0) = 45�x�i�̃��W�A���\�L�j
	/// </summary>
	static inline dectype Atan( dectype tan );

	/// <summary>
	/// Tan�̋t��(cot)
	/// </summary>
	static inline dectype Ctan( dectype tan );

	/// <summary>
	/// Sin�̋t�֐�
	/// Atan�̌��ʂ��t���ڂŌv�Z���܂��B
	/// </summary>
	static inline dectype Atan2( dectype x , dectype y );

    /// <summary>
    /// �K�؂�2�΂̔{���������邱�ƂŊp�x��-��...�΂͈̔͂Ƀ��b�v����
    /// </summary>
	static inline dectype WrapPi( dectype theta );

    /// <summary>
    /// acos(x)�Ɠ���
    /// �������Ax���L���O�̏ꍇ�A�ł��߂��L���Ȓl�ɃN�����v���������
    /// �Ԃ����l�́A�͈�0...�΂ł���A����͕W����C��acos()�֐��Ɠ����ł���B
    /// </summary>
	static inline dectype SafeAcos( dectype cos );

    /// <summary>
    /// �����̒l�ȉ��ōł��߂������l��Ԃ��i�؎̂āj
    /// </summary>
	static inline dectype	Floor( dectype val );

    /// <summary>
	/// ��Βl��Ԃ��܂��B
    /// </summary>
	static inline dectype Abs( dectype val );

    /// <summary>
	/// ���������_�̐�Βl��Ԃ��܂��B
    /// </summary>
	static inline s32 FAbs( s32 val );
	static inline dectype FAbs( dectype val );

    /// <summary>
	///  �����̒l�ȏ�ōł��߂������l��Ԃ��i�؂�グ�j
    /// </summary>
	static inline dectype Ceil( dectype val );

    /// <summary>
	///  �l�̌ܓ�
    /// </summary>
	static inline dectype Round( dectype val );

	/// <summary>
	/// �x���烉�W�A����
	/// </summary>
	static inline dectype	DegToRad( dectype deg );

	/// <summary>
	/// ���W�A������x��
	/// </summary>
	static inline dectype	RadToDeg( dectype rad );

	/// <summary>
	///	Sin�l��Cos�l�𓯎��ɓ���
	/// </summary>
	static inline void		SinCos( dectype *returnSin, dectype *returnCos , dectype rad );

	/// <summary>
	///	����ƃY�[���Ԃ�ϊ����܂��B�R�c���w��14.2.4�Q�ƁB����p�͒ʏ탉�W�A���ŕ\�����܂��B
	///
	/// �y�R�c���w�������́z
	/// �Y�[����90�x�̎���ɑ΂���I�u�W�F�N�g�̌�������̃T�C�Y�̊����𑪂�܂��B
	/// �Ⴆ��2.0�̃Y�[���́A���ꂪ90�x�̎����p���Ă���ꍇ�ɔ�ׂĂ��̃I�u�W�F�N�g��
	/// �Q�{�̑傫���Ƃ��Č���邱�Ƃ��Ӗ����܂��B
	/// ����͐l�Ԃ��g�p����̂ɕ֗��Ȓl�ł����A�R���s���[�^�[���ʏ�K�v�Ƃ��鐔���̓Y�[���ł��B
	///
	/// ���Ȃ킿�A��ʏ�̂��̂����ׂĂQ�{�̑傫���ɂ������ꍇ�A�Y�[���l��2.0�Ƃ������ƂŁA
	/// ����2.0�͎���p�������ɂ����炻���Ȃ�̂��Ƃ������ƁB
	/// </summary>
	static inline dectype	FovToZoom( dectype fov );
	static inline dectype	ZoomToFov( dectype zoom );

    /// <summary>
	///  �}�C�i�X��360�ȏ�̐����̓x��K�؂�360�ȓ��̐��l�ɕϊ����܂�
    /// </summary>
	static inline dectype  ConvDegIn360( s32 deg );

    /// <summary>
	///  �P�ʉ~��̍��W��Ԃ�
    /// </summary>
	static inline void GetUnitCirclePos( dectype deg , dectype *pPosX , dectype *pPosY , dectype length = 1.0f );

    /// <summary>
	///  �T�C���J�[�u �R�T�C���J�[�u��Ԃ�
    /// </summary>
	static inline void GetUnitCirclePosWave( dectype deg , dectype *pPosX , dectype *pPosY , dectype length = 1.0f );

    /// <summary>
	///  �ݏ��Ԃ�
    /// </summary>
	static inline dectype Pow( dectype k , dectype num );

    /// <summary>
	///  �]���Ԃ�
    /// </summary>
	static inline dectype Mod( dectype param1 , dectype param2 );

    /// <summary>
	///  �w��̒l���w��̌덷�͈͓��ŃC�R�[����������True��Ԃ�
    /// </summary>
	static inline skyBool NearyEq( dectype src1 , dectype src2 , dectype gosa );

    /// <summary>
	///  �w��̒l���w��̍ŏ��l�ƍő�l�͈̔͊O�������炻�ꂼ��ŏ��l�ƍő�l�Ɉȓ��ɂ܂�߂�
    /// </summary>
	template <class Type>
	static inline void Mul( Type &value , Type min , Type max );

private:

	//�e�@�\����
	CMathUtil(){};													//�R���X�g���N�^
	CMathUtil( const CMathUtil& ){};							//�R�s�[�R���X�g���N�^
	virtual ~CMathUtil(){};											//�f�X�g���N�^
	CMathUtil &operator=(const CMathUtil& ){ return *this; };	//������Z�q�I�[�o�[���[�h
};

} } }

#define CMathUtil_	 sky::lib::math::CMathUtil

#include "MathUtil.inl"