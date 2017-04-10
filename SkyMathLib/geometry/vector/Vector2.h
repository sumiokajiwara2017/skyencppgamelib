#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �Q�c���_�N���X
/// �x�[�X�̓Q�[���R�c���w�̃T���v�����璸���Ă��܂����B
/// </summary>
template< typename T >
class CVector2 : public CGeometryBase
{

public:

	/// <summary>
	/// �Q�c���_
	/// </summary>
	union 
	{
		struct 
		{
			T		X;
			T		Y;
		};
		T v[ 2 ];
	};


	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CVector2();				//�f�t�H���g�̃R���X�g���N�^
	inline CVector2( const CVector2& v1 );		//���̃C���X�^���X����̗v�f�̒l�R�s�[�C���X�^���X����
	inline CVector2(T nX , T nY );			//�v�f�ԍ����󂯎��C���X�^���X����

	/// <summary>
	/// []���Z�q�I�[�o�[���C�h
	/// </summary>
	inline T operator[]( s32 idx );

	/// <summary>
	/// �l�ݒ�
	/// </summary>
	inline CVector2< T > &Set( T x , T y );

	/// <summary>
	/// �O������
	/// </summary>
	inline void Zero();

	/// <summary>
	/// �󔻒�
	/// </summary>
	inline skyBool IsEmpty();

	/// <summary>
	/// =���Z�q�I�[�o�[���C�h
	/// </summary>
	inline CVector2& operator =( const CVector2& v1 );

	/// <summary>
	/// ==���Z�q�I�[�o�[���C�h
	/// </summary>
	inline skyBool operator ==( const CVector2& v1 ) const;

	/// <summary>
	/// !=���Z�q�I�[�o�[���C�h
	/// </summary>
	inline skyBool operator !=( const CVector2& v1 ) const;

	/// <summary>
	/// +���Z�q�I�[�o�[���C�h
	/// </summary>
	inline CVector2 operator +( const CVector2& v1 ) const;

	/// <summary>
	/// -���Z�q�I�[�o�[���C�h
	/// </summary>
	inline CVector2 operator -( const CVector2& v1 ) const;

	/// <summary>
	/// -���Z�q�I�[�o�[���C�h
	/// </summary>
	inline CVector2 operator -() const;

	/// <summary>
	/// *���Z�q�I�[�o�[���C�h
	/// </summary>
	inline CVector2 operator *( T val ) const;
	inline CVector2 operator *( CVector2 vec ) const;

	/// <summary>
	/// /���Z�q�I�[�o�[���C�h
	/// </summary>
	inline CVector2 operator /( T val ) const;
	inline CVector2 operator /( CVector2 vec ) const;

	/// <summary>
	/// +=���Z�q�I�[�o�[���C�h
	/// </summary>
	inline CVector2& operator +=( const CVector2 &v1 );

	/// <summary>
	/// -=���Z�q�I�[�o�[���C�h
	/// </summary>
	inline CVector2& operator -=( const CVector2 &v1 );

	/// <summary>
	/// -=���Z�q�I�[�o�[���C�h
	/// </summary>
	inline CVector2& operator *=( T val );

	/// <summary>
	/// /=���Z�q�I�[�o�[���C�h
	/// </summary>
	inline CVector2& operator /=( T val );

	/// <summary>
	/// �x�N�g���̐��K��
	/// </summary>
	inline void	Normalize();

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
	/// ��jCVector2( 2 , 0 ) �� CVector( 3 , 0 )�̓��ς�6�B����2�̃x�N�g���͕��s�ł���
	/// ��jCVector2( 2 , 0 ) �� CVector( -3 , 0 )�̓��ς�-6�B����2�̃x�N�g���͕��s�i�t�����j�ł���
	/// ��jCVector2( 2 , 0 ) �� CVector( 0 , 3 )�̓��ς�0�B����2�̃x�N�g���͒��s�ł���
	/// </summary>
	T Dot( const CVector2& v1 ) const;

    /// <summary>
    /// �x�N�g������]��������̃x�N�g�����擾����
    /// </summary>
	inline CVector2< T > Rot( T rad );

    /// <summary>
    /// ���̃x�N�g���ɒ��p�ȃx�N�g����Ԃ�
    /// </summary>
    inline CVector2 Perp();

    /// <summary>
    /// �x�N�g���̂Ȃ��p(���W�A���j�����߂�
    /// </summary>
    inline T IncludedAngle( const CVector2 &v1 );

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

    /// <summary>
    /// �l��؂�̂Ă܂�
    /// </summary>
	inline void Floor();

    /// <summary>
    /// �l���l�̌ܓ����܂��B
    /// </summary>
	inline void Round();
};

/// <summary>
/// �x�N�g���̑傫�����v�Z����
/// </summary>
template< class T >
inline T VectorMag( const CVector2< T > &v1 );

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
inline T Cross( const CVector2< T >& v1, const CVector2< T >& v2 );

/// <summary>
/// *���Z�q�I�[�o�[���C�h�i�l*Vector2�C���X�^���X�B�X�^�b�N��ɐV�����C���X�^���X�𐶐����܂��B�j
/// </summary>
template< class T >
inline CVector2< T > Mult(f32 k, const CVector2< T > &v1);

/// <summary>
/// �x�N�g���Ԃ̋�����Ԃ��܂��B
/// </summary>
template< class T >
inline T Distance(const CVector2< T >& v1, const CVector2< T >& v2);

/// <summary>
/// �x�N�g���Ԃ̋�����Ԃ��܂��B
/// �������̌v�Z���Ȃ��Ă��܂��B�P���Ȕ�r�̏ꍇ��������g�����ق�������
/// </summary>
template< class T >
inline T DistanceSquared( const CVector2< T >& v1, const CVector2< T >& v2 );

//��{�x�N�g���Q�^
typedef CVector2< dectype >                 CBasicVector2;
typedef CVectorPrimitive< CBasicVector2 >	CBasicVector2Vector;			//�z��
typedef kfm::CFCurve< CBasicVector2 >       CBasicVector2FCurve;
typedef kfm::CFCurvePlayer< CBasicVector2 > CBasicVector2FCurvePlayer;

/// <summary>
/// �Q�����[���x�N�g��
/// </summary>
static const CBasicVector2 CBasicVector2_ZERO( 0.0f , 0.0f );

} } }

#include "Vector2.inl"