#pragma once

namespace sky { namespace lib { namespace math {

namespace VectorUtil 
{

	/// <summary>
	/// �_�̏W���ɑ΂���œK�Ȗ@�����v�Z����
	/// <summary>
	template< typename VECTOR3 >
	void ComputeBestFitNormal( const VECTOR3 v[] , s32 num , VECTOR3& dest );
}

} } }

#include "VectorUtil.inl"