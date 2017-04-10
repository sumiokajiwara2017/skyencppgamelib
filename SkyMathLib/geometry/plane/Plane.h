#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// ��ʕ��ʃN���X
/// ���ʂƂ́A���ʏ��1�_�Ƃ��̖@���ł���킳���B���L�����������W���[
/// ��ʕ��ʕ����� ax + by + cz = d
/// �@��(a,b,c) �� d �͕��ʏ�̓_(x,y,z)�Ɩ@���̓��ςȂ̂ŁA���ʏ�̓_(x,y,z)��d������Ζ@�����瓱���o����B
/// ���ʕ��ʂ�\���l�Ƃ��Ă�4�̐���abcd������΂����Ƃ������ƂɂȂ�̂ŁA���̃N���X�̃����o��abcd�ɂȂ��Ă���B
/// ����Ŗʂ̕\�������A�ʂƓ_�̋����A�ʂƓ_�̊p�x�ȂǏd�v�ȏ����v�Z�ł���B
/// �Q�lURL : http://marupeke296.com/COL_Basic_No3_WhatIsPlaneEquation.html
/// </summary>
template< typename T >
class CPlane : public CGeometryBase
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CPlane();
	CPlane( const CVector3< T > &normal , const CVector3< T > &pos );
	CPlane( const CVector3< T > &pos1 , const CVector3< T > &pos2 , const CVector3< T > &pos3 );
	CPlane( const CTriangle3< T > &triangle );

	/// <summary>
	/// �R�_���畽�ʂ��쐬����
	/// </summary>
	void CreatePlane( const CVector3< T > &pos1 , const CVector3< T > &pos2 , const CVector3< T > &pos3 );

	/// <summary>
	/// �@���Ɠ_���畽�ʂ��쐬����
	/// </summary>
	void CreatePlane( const CVector3< T > &normal , const CVector3< T > &pos );

	/// <summary>
	/// �O�p�`���畽�ʂ��쐬����
	/// </summary>
	void CreatePlane( const CTriangle3< T > &triangle );

	/// <summary>
	/// �C�ӂ̐��̓_����œK�Ȗʂ��쐬����
	/// </summary>
	void CreatePlane( CVector3< T > pPoints[] , u32 pointNum );

	/// <summary>
	/// �_���ʂ̕\�Ɨ��ǂ���ɂ��邩���肷��(1:�\ -1:�� 0:���ʏ�j
	/// </summary>
	s32 ColPointPlaneSide( CVector3< T > &pos ) const;

	/// <summary>
	/// �_���ʂ̓����ɂ��邩���肷��
	/// </summary>
	skyBool Inside( const CVector3< T > &point ) const;

	/// <summary>
	/// �����ʂ̓����ɂ��邩���肷��
	/// </summary>
	skyBool Inside( const CVector3< T > &point, const T radius ) const;

	/// <summary>
	/// �_�ƕ��ʂ̓��ς��v�Z����
	/// ���� (a, b, c, d) �� 3D �x�N�g�� (x, y, z) ���w�肵���ꍇ�A���̊֐��̖߂�l�� a*x + b*y + c*z + d*1 
	/// </summary>
	T Dot( const CVector3< T > &point ) const;

	/// <summary>
	/// ���K��
	/// </summary>
	CPlane &Normalize();

	/// <summary>
	/// �L���X�g���Z�q�I�[�o�[���C�h
	/// </summary>
    operator const CVector4< T >() const;

	/// <summary>
	/// ���ʂ̖@��(��ʕ��ʕ������ɂ�����a , b, c )
	/// </summary>
	CVector3< T >	m_Normal;

	/// <summary>
	/// �@��n�ƕ��ʏ�̓_�Ƃ̓���
	/// </summary>
	T				d;

	/// <summary>
	/// ���ʏ�̓_�i�쐬���Ɏ���Ă����j
	/// </summary>
	CVector3< T >	m_Pos;

};

//��{�~�^
typedef CPlane< dectype >     CBasicPlane;

} } }

#include "Plane.inl"
