#pragma once

namespace sky { namespace lib { namespace math {

class CMatrix4x3;

/// <summary>
/// �R�c���_�N���X
/// �x�[�X�̓Q�[���R�c���w�̃T���v�����璸���Ă��܂����B
/// </summary>
template< typename T >
class CVector3 : public CGeometryBase
{

public:

	/// <summary>
	/// �R�c���_
	/// </summary>
	union 
	{
		struct 
		{
			T		X;
			T		Y;
			T		Z;
		};
		T v[ 3 ];
	};


	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CVector3();				        //�f�t�H���g�̃R���X�g���N�^
	inline CVector3( const CVector3& v );	//���̃C���X�^���X����̗v�f�̒l�R�s�[�C���X�^���X����
	inline CVector3( T nX , T nY , T nZ );	//�v�f�ԍ����󂯎��C���X�^���X����
	inline CVector3( const dectype vec[ 3 ] );

	/// <summary>
	/// []���Z�q�I�[�o�[���C�h
	/// </summary>
	inline T operator[]( s32 idx ) const;

	/// <summary>
	/// �O������
	/// </summary>
	inline void Zero();

	/// <summary>
	/// �l�̃Z�b�g
	/// </summary>
	inline CVector3< T > &Set( T x , T y , T z );

	/// <summary>
	/// =���Z�q�I�[�o�[���C�h�i�v�f�̑���j
	/// </summary>
	inline CVector3& operator =( const CVector3& v );
	inline CVector3& operator =( const CVector2< T >& v );

	/// <summary>
	/// ==���Z�q�I�[�o�[���C�h�i�v�f�̔�r�j
	/// </summary>
	inline skyBool operator ==( const CVector3& v ) const;

	/// <summary>
	/// !=���Z�q�I�[�o�[���C�h
	/// </summary>
	inline skyBool operator !=( const CVector3& v ) const;

	/// <summary>
	/// +���Z�q�I�[�o�[���C�h
	/// </summary>
	inline CVector3 operator +( const CVector3& v ) const;

	/// <summary>
	/// -���Z�q�I�[�o�[���C�h
	/// </summary>
	inline CVector3 operator -( const CVector3& v ) const;

	/// <summary>
	/// -���Z�q�I�[�o�[���C�h
	/// </summary>
	inline CVector3 operator -() const;

	/// <summary>
	/// *���Z�q�I�[�o�[���C�h
	/// </summary>
	inline CVector3 operator *( T val ) const;
	inline CVector3 operator *( const CVector3 &vec ) const;

	/// <summary>
	/// /���Z�q�I�[�o�[���C�h
	/// </summary>
	inline CVector3 operator /( T val ) const;
	inline CVector3 operator /( const CVector3 &vec ) const;

	/// <summary>
	/// +=���Z�q�I�[�o�[���C�h
	/// </summary>
	inline CVector3& operator +=( const CVector3 &v );

	/// <summary>
	/// -=���Z�q�I�[�o�[���C�h
	/// </summary>
	inline CVector3& operator -=( const CVector3 &v );

	/// <summary>
	/// *=���Z�q�I�[�o�[���C�h
	/// </summary>
	inline CVector3& operator *=( T val );

	/// <summary>
	/// /=���Z�q�I�[�o�[���C�h
	/// </summary>
	inline CVector3& operator /=( T val );

	/// <summary>
	/// �x�N�g���̐��K��
	/// </summary>
	inline CVector3&	Normalize();

	/// <summary>
	/// �x�N�g���̓��ς��v�Z����
	///
	/// �y���ρz
	/// ��ʓI�ɂǂ�Ȏ����̃x�N�g���̓��ς���̃x�N�g�����ǂꂮ�炢�u���Ă��邩�v��\���܂��B
	/// ���ς��傫���قǁu���Ă���v���ƂɂȂ�܂��B
	/// +-----+------+---------------------------------------------------------------------------------------------------+
	/// | > 0 | �s�p | �����ނ˓�������������(���K�������x�N�g�����m�̏ꍇ��1.0f�Ŋ��S�ɓ��������Ƃ������ƂɂȂ�)        |
	/// |  0  | ���p | ���s���Ă���								                                                         |
	/// | < 0 | �݊p | �����ނ˔��Ε����������Ă���i���K�������x�N�g�����m�̏ꍇ��-1.0f�Ŋ��S�ɔ��Ε����Ƃ������ƂɂȂ� |
	/// +-----+------+---------------------------------------------------------------------------------------------------+
	/// </summary>
	inline T Dot( const CVector3& v ) const;

	/// <summary>
	/// �x�N�g���̊O�ς��v�Z����
	/// 
	/// �y�O�ρz 
	///  �O�ς͗^����ꂽ�Q�̃x�N�g���ɐ����ȃx�N�g����Ԃ��܂��B
	///  ���ςƊO�ς������Ɍv�Z���ꂽ�ꍇ�O�ς̂ق�����Ɍv�Z����܂��B
	///  
	///  �O�ς̑傫���̓x�N�g���̂Ȃ��p��sin�̑傫���ɔ�Ⴕ�܂��B
	///  �O�ς̑傫���͂��ƂƂȂ�Q�̃x�N�g���̑傫����sin�Ƃ��������l�ɓ������B
	///  ||a�~b|| == ||a||||b||sin��
	/// </summary>
	inline CVector3< T > Cross( const CVector3< T >& v ) const;

    /// <summary>
    /// �x�N�g���̂Ȃ��p(���W�A���j�����߂�
    /// </summary>
    inline T IncludedAngle( const CVector3 &v ) const;

    /// <summary>
    /// �x�N�g�����w��̒����ɂ���
    /// </summary>
    inline T SetLength( T length );

    /// <summary>
    /// �x�N�g�����w��̒������z���Ă�����w��̒����ɂ���
    /// </summary>
	inline T Truncate( T length );

    /// <summary>
    /// �x�N�g���̒��������߂�
    /// </summary>
    inline T Length() const;
	inline T QuaredLength() const; //2��
    inline T LengthXZ() const;

	/// <summary>
	/// �x�N�g���̒������R�s�[�����x�N�g����Ԃ�
	/// </summary>
	inline CVector3< T > Vector3Length() const;

	/// <summary>
	/// f32�^�̔z��ɓ��e���R�s�[����
	/// </summary>
	inline void Copyf32Array( f32 *pDestArray ) const;

	/// <summary>
	/// �S�Ă̗v�f��0���ǂ������ׂ�
	/// </summary>
	inline skyBool IsAllZero() const;

	/// <summary>
	/// �S�Ă̗v�f��1���ǂ������ׂ�
	/// </summary>
	inline skyBool IsAllOne() const;
};

/// <summary>
/// �x�N�g���̑傫�����v�Z����
/// </summary>
template< class T >
inline T VectorMag( const CVector3< T > &v );

/// <summary>
/// �x�N�g���̊O�ς��v�Z����
/// 
/// �y�O�ρz 
///  �O�ς͗^����ꂽ�Q�̃x�N�g���ɐ����ȃx�N�g����Ԃ��܂��B
///  ���ςƊO�ς������Ɍv�Z���ꂽ�ꍇ�O�ς̂ق�����Ɍv�Z����܂��B
///  
///  �O�ς̑傫���̓x�N�g���̂Ȃ��p��sin�̑傫���ɔ�Ⴕ�܂��B
///  �O�ς̑傫���͂��ƂƂȂ�Q�̃x�N�g���̑傫����sin�Ƃ��������l�ɓ������B
///  ||a�~b|| == ||a||||b||sin��
/// </summary>
template< class T >
inline CVector3< T > Cross( const CVector3< T >& v1 , const CVector3< T >& v2 );

/// <summary>
/// �x�N�g���̓��ς��v�Z����
///
/// �y���ρz
/// ��ʓI�ɂǂ�Ȏ����̃x�N�g���̓��ς���̃x�N�g�����ǂꂮ�炢�u���Ă��邩�v��\���܂��B
/// ���ς��傫���قǁu���Ă���v���ƂɂȂ�܂��B
///
/// +-----+------+------------------------------------------+
/// | > 0 | �s�p | �����ނ˓�������������					|
/// |  0  | ���p | ���s���Ă���								|
/// | < 0 | �݊p | �����ނ˔��Ε����������Ă���				|
/// +-----+------+------------------------------------------+
/// </summary>
template< class T >
inline T Dot( const CVector3< T >& v1 , const CVector3< T >& v2 );

/// <summary>
/// *���Z�q�I�[�o�[���C�h�i�l*CVector3�C���X�^���X�B�X�^�b�N��ɐV�����C���X�^���X�𐶐����܂��B�j
/// </summary>
template< class T >
inline CVector3< T > Mult( T k , const CVector3< T > &v );

/// <summary>
/// �x�N�g���Ԃ̋�����Ԃ��܂��B
/// </summary>
template< class T >
inline T Distance( const CVector3< T >& v1, const CVector3< T >& v2 );

/// <summary>
/// �x�N�g���Ԃ̋�����Ԃ��܂��B
/// �������̌v�Z���Ȃ��Ă��܂��B�P���Ȕ�r�̏ꍇ��������g�����ق�������
/// </summary>
template< class T >
inline T DistanceSquared( const CVector3< T >& v1, const CVector3< T >& v2 );

/// <summary>
/// v1��v2�����傫���ꍇskyTrue��Ԃ��B
/// </summary>
template< class T >
inline skyBool Vector3Greater( const CVector3< T >& v1 , const CVector3< T >& v2 );

/// <summary>
/// �e�v�f�̋t���x�N�g����Ԃ�
/// </summary>
template< class T >
inline CVector3< T > VectorReciprocal( const CVector3< T > &v );

/// <summary>
/// �s��ϊ�
/// </summary>
template< class T >
inline CVector3< T > Transform( const CVector3< T > &vec , const CMatrix4x3 &matrix );

//��{�x�N�g���R�^
typedef CVector3< dectype >                 CBasicVector3;
typedef CList< u32 , CBasicVector3 >        CBasicVector3List;
typedef CVectorPrimitive< CBasicVector3 >	CBasicVector3Vector;	//�z��
typedef CStack< CBasicVector3 >		        CBasicVector3Stack;
typedef kfm::CFCurve< CBasicVector3 >       CBasicVector3FCurve;
typedef kfm::CFCurvePlayer< CBasicVector3 > CBasicVector3FCurvePlayer;

/// <summary>
/// �R����ZERO�x�N�g��
/// </summary>
static const CBasicVector3 CBasicVector3_ZERO( 0.0f , 0.0f , 0.0f );
static const CBasicVector3 CBasicVector3_UPY( 0.0f , 1.0f , 0.0f );
static const CBasicVector3 CBasicVector3_UPZ( 0.0f , 0.0f , 1.0f );

/// <summary>
/// ���萔
/// </summary>
static const CBasicVector3 VECTOR3_AXIS_X   ( 1.0f , 0.0f , 0.0f );
static const CBasicVector3 VECTOR3_AXIS_Y   ( 0.0f , 1.0f , 0.0f ); 
static const CBasicVector3 VECTOR3_AXIS_Z   ( 1.0f , 0.0f , 1.0f ); 

} } }

#include "Vector3.inl"