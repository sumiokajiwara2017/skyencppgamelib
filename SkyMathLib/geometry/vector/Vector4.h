#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �S�c���_�N���X
/// </summary>
template< typename T >
class CVector4 : public CGeometryBase
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
			T       W;
		};
		T v[ 4 ];
	};


	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CVector4();				//�f�t�H���g�̃R���X�g���N�^
	inline CVector4( const CVector3< T >& v , T w );	//���̃C���X�^���X����̗v�f�̒l�R�s�[�C���X�^���X����
	inline CVector4( const CVector4& v );	//���̃C���X�^���X����̗v�f�̒l�R�s�[�C���X�^���X����
	inline CVector4( T nX , T nY , T nZ , T nW );	//�v�f�ԍ����󂯎��C���X�^���X����

	/// <summary>
	/// []���Z�q�I�[�o�[���C�h
	/// </summary>
	inline T operator[]( s32 idx );

	/// <summary>
	/// �O������
	/// </summary>
	inline void Zero();

	/// <summary>
	/// �l�̃Z�b�g
	/// </summary>
	inline void Set( T x , T y , T z , T w );

	/// <summary>
	/// �R�����Ŏ擾
	/// </summary>
	inline CVector3<T> GetVector3() const;

	/// <summary>
	/// =���Z�q�I�[�o�[���C�h�i�v�f�̑���j
	/// </summary>
	inline CVector4& operator =( const CVector4& v );

	/// <summary>
	/// *=���Z�q�I�[�o�[���C�h
	/// </summary>
	inline CVector4& operator *=( T val );

	/// <summary>
	/// �x�N�g���̐��K��
	/// </summary>
	inline CVector4&	Normalize();

	/// <summary>
	/// ���������߂�
	/// </summary>
    inline T Length() const;

	/// <summary>
	/// �x�N�g���̓��ς��v�Z����
	/// </summary>
	inline T Dot( const CVector4& v ) const;
};

//��{�x�N�g���R�^
typedef CVector4< dectype >                 CBasicVector4;

/// <summary>
/// �S����ZERO�x�N�g��
/// </summary>
static const CBasicVector4 CBasicVector4_ZERO( 0.0f , 0.0f , 0.0f , 0.0f );

} } }

#include "Vector4.inl"