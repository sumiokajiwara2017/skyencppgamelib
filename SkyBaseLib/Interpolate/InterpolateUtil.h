#pragma once

/// <summary>
/// ��ԃ^�C�v
/// </summary>
enum eInterpolateType
{
	eInterpolateType_Linear = 0 ,     //���`
	eInterpolateType_Acceleration ,   //����
	eInterpolateType_Deceleration ,   //����
	eInterpolateType_None ,           //��Ԗ����i���[�v�j
};

namespace sky { namespace lib { namespace interp {

/// <summary>
/// ��Ԗ�����ԁit��1.0f�ɂȂ����Ƃ�end�̒l��dest�ɂ�����j
/// </summary>
template< typename T >
inline T& None( T& dest, const T& start, const T& end, f32 t );

/// <summary>
/// ���`��Ԍv�Z
/// </summary>
template< typename T >
inline T& Linear( T& dest, const T& start, const T& end, f32 t );

/// <summary>
/// ������Ԍv�Z
/// </summary>
template< typename T >
inline T& Acceleration( T& dest, const T& start, const T& end, f32 t );

/// <summary>
/// ������Ԍv�Z
/// </summary>
template< typename T >
inline T& Deceleration( T& dest, const T& start, const T& end, f32 t );

/// <summary>
/// ��ԃ��W�b�N���ʎ��s
/// </summary>
template< typename T >
inline T& InterpolateCommon( T& dest, const T& start, const T& end, f32 t , eInterpolateType eType );

/// <summary>
/// �f�O���[�̋��ʐ��`��Ԍv�Z
/// start��end��0�`360�ł���K�v������
/// </summary>
inline f32& LinearDegree( f32& dest, f32 start, f32 end, f32 t );
		
/// <summary>
/// �G���~�[�g��Ԍv�Z
/// </summary>
template< typename T >
inline T& Hermite( T& dest, const T& start, const T& end, const T& tan0, const T& tan1, f32 t );
		
/// <summary>
/// �f�O���[�̋��ʃG���~�[�g���
/// start��end��0�`360�ł���K�v������
/// </summary>
inline f32& HermiteDegree( f32& dest, f32 start, f32 end, f32 tan0, f32 tan1, f32 t );

} } }

#include "InterpolateUtil.inl"