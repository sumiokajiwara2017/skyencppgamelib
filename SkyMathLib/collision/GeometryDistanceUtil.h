#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �􉽊w���������Q
/// </summary>
namespace GeometryDistanceUtil
{
	/// <summary>
	/// 3D�_���畽�ʂ̍ŒZ����
	/// </summary>
	template< typename T >
	inline T Pos3ToPlane( const CVector3< T > &pos , const CPlane< T > &plane );

	/// <summary>
	/// ����ab�Ɠ_c�̋����̕���(2��)��Ԃ�
	/// </summary>
	template< typename T >
	inline T SqDistPointSegment( const CVector3< T > &a , const CVector3< T > &b , const CVector3< T > &c );

	/// <summary>
	/// ����p1,q1�Ɛ���p2,q2�̋����̕���(2��)��Ԃ�
	/// </summary>
	template< typename T >
	inline T ClosestPtSegmentSegment( const CVector3< T > &p1 , const CVector3< T > &q1 , const CVector3< T > &p2 , const CVector3< T > &q2 , T &s , T &t , CVector3< T > &c1 , CVector3< T > &c2 );
}

} } }

#include "GeometryDistanceUtil.inl"