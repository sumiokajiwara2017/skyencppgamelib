#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// Šô‰½Šw‹——£ˆ—ŒQ
/// </summary>
namespace GeometryDistanceUtil
{
	/// <summary>
	/// 3D“_‚©‚ç•½–Ê‚ÌÅ’Z‹——£
	/// </summary>
	template< typename T >
	inline T Pos3ToPlane( const CVector3< T > &pos , const CPlane< T > &plane );

	/// <summary>
	/// ü•ªab‚Æ“_c‚Ì‹——£‚Ì•½•û(2æ)‚ğ•Ô‚·
	/// </summary>
	template< typename T >
	inline T SqDistPointSegment( const CVector3< T > &a , const CVector3< T > &b , const CVector3< T > &c );

	/// <summary>
	/// ü•ªp1,q1‚Æü•ªp2,q2‚Ì‹——£‚Ì•½•û(2æ)‚ğ•Ô‚·
	/// </summary>
	template< typename T >
	inline T ClosestPtSegmentSegment( const CVector3< T > &p1 , const CVector3< T > &q1 , const CVector3< T > &p2 , const CVector3< T > &q2 , T &s , T &t , CVector3< T > &c1 , CVector3< T > &c2 );
}

} } }

#include "GeometryDistanceUtil.inl"