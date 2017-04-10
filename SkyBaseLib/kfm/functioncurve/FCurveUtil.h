#pragma once

namespace sky { namespace lib { namespace kfm {

/// <summary>
/// �t�@���N�V�����J�[�u�֗������Q
/// </summary>
namespace FCurveUtil
{
	/// <summary>
	/// �w��̌^�̃L�[�l�z����󂯎��A�t�@���N�V�����J�[�u�𐶐�����
	/// </summary>
	template < class T >
	CFCurve< T > *CreateFCurve( const CVectorPrimitive< T > &keys , f32 intvl = 10 , eInterpolateType eInterType = eInterpolateType_Linear );
}

} } }

#include "FCurveUtil.inl"