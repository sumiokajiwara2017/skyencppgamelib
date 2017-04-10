#pragma once

namespace sky { namespace lib { namespace math {

namespace VectorUtil 
{

	/// <summary>
	/// 点の集合に対する最適な法線を計算する
	/// <summary>
	template< typename VECTOR3 >
	void ComputeBestFitNormal( const VECTOR3 v[] , s32 num , VECTOR3& dest );
}

} } }

#include "VectorUtil.inl"