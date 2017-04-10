#pragma once

namespace sky { namespace lib { namespace kfm {

/// <summary>
/// ファンクションカーブ便利処理群
/// </summary>
namespace FCurveUtil
{
	/// <summary>
	/// 指定の型のキー値配列を受け取り、ファンクションカーブを生成する
	/// </summary>
	template < class T >
	CFCurve< T > *CreateFCurve( const CVectorPrimitive< T > &keys , f32 intvl = 10 , eInterpolateType eInterType = eInterpolateType_Linear );
}

} } }

#include "FCurveUtil.inl"