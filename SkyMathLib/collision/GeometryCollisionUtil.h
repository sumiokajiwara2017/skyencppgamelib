#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// Šô‰½ŠwÕ“Ë•Ö—˜ˆ—ŒQ
/// memo
/// ƒQ[ƒ€‚ÌÕ“Ë”»’è‚ÍÅ‰‚É’Pƒ‹éŒ`AŸ‚É‰~‚ÌÕ“Ë”»’è‚ğs‚¤B
/// </summary>
namespace GeometryCollisionUtil {

	/// <summary>
	/// 2D“_‚Æ2D“_‚ÌÕ“Ë”»’è
	/// </summary>
	template< typename T >
	inline skyBool Pos2CrossPos2( const CVector2< T > &pos1 , const CVector2< T > &pos2 , CollisionResultInfo *info = skyNull );

	/// <summary>
	/// 2D“_‚Æ2D“_‚ğ’Ê‚é’¼ü‚ÌŠp“x‚ğƒ‰ƒWƒAƒ“‚Å•Ô‚·
	/// </summary>
	template< typename T >
	inline T Pos2AndPos2Angle( const CVector2< T > &pos1 , const CVector2< T > &pos2 );

	/// <summary>
	/// 2D“_‚©‚ç2Dü‚Ì‚üƒxƒNƒgƒ‹‚ğ‹‚ß‚é( “_‚Æü‚ª’¼s‚ÅŒğ‚í‚ç‚È‚¢ê‡A‚üƒxƒNƒgƒ‹‚Í‘¶İ‚µ‚È‚¢‚Ì‚ÅskyFalse‚ª•Ô‚é)
	/// </summary>
	template< typename T >
	inline skyBool Pos2ToLineVector( const CVector2< T > &pos , const CLine2< T > &line , CVector2< T > &dest );

	/// <summary>
	/// 2D“_‚ª2Dü‚Ìn“_‚©‚çI“_‚ÖL‚Ñ‚éƒxƒNƒgƒ‹‚ğis•ûŒü‚Æ‚µ‚Ä¶( 1 )A‰E( - 1 )‚Ç‚¿‚ç‘¤‚É‚ ‚é‚©A’¼üã( 0 )‚É‚ ‚é‚©‚ğ”»’è‚·‚é
	/// </summary>
	template< typename T >
	inline T Pos2ToLineSide( const CVector2< T > &pos , const CLine2< T > &line );

	/// <summary>
	/// 2Dü‚Æ2Dü‚ÌŒğ·”»’è
	/// 0: Œğ·‚µ‚Ä‚¢‚È‚¢ 1: ˆê“_‚ÅŒğ·‚µ‚Ä‚¢‚é 2: ˆê“_ˆÈã‚ÅŒğ·‚µ‚Ä‚¢‚é
	/// </summary>
	template< typename T >
	inline s32 Line2CrossLine2( const CLine2< T > &line1 , const CLine2< T > &line2 , CollisionResultInfo *info = skyNull );

	/// <summary>
	/// 2D“_‚Æ2Dü‚ÌÕ“Ë”»’è
	/// </summary>
	template< typename T >
	inline skyBool Point2CrossLine2( const CVector2< T > &point ,  const CLine2< T > &line );

	/// <summary>
	/// ‰~‚Æ‰~‚ÌÕ“Ë”»’è
	/// </summary>
	template< typename T >
	inline skyBool CircleCrossCircle( const CCircle< T > &circle1 , const CCircle< T > &circle2 );

	/// <summary>
	/// 2D“_‚Æ‹éŒ`‚ÌÕ“Ë”»’è
	/// </summary>
	template< typename T >
	inline skyBool Point2CrossRect( const CVector2< T > &pos , const CRect< T > &rect );

	/// <summary>
	/// ‹éŒ`‚Æ‹éŒ`‚ÌÕ“Ë”»’è(¶ãÀ•W‚Æ•‚Æ‚‚³j
	/// </summary>
	template< typename T >
	inline skyBool RectCrossRect( const CRect< T > &rect1 , const CRect< T > &rect2 );

	/// <summary>
	/// lŠpŒ`‚ÆlŠpŒ`‚ÌÕ“Ë”»’è(’†SÀ•W‚Æ•‚Æ‚‚³j
	/// </summary>
	template< typename T >
	inline skyBool RectCrossRect( const CVector2< T > &center1 , const CSize< T > &size1 , const CVector2< T > &center2 , const CSize< T > &size2 );

	/// <summary>
	/// 2D“_‚Æ‰~‚ÌÕ“Ë”»’è
	/// </summary>
	template< typename T >
	inline skyBool Point2CrossCircle( const CVector2< T > &point , const CCircle< T > &circle );

	/// <summary>
	/// 2Dü‚Æ‰~‚ÌÕ“Ë”»’è
	/// </summary>
	template< typename T >
	inline skyBool Line2CrossCircle( const CLine2< T > &line , const CCircle< T > &circle );

	/// <summary>
	/// 2D“_‚Æ2DƒŒƒC‚Æ‚ÌÕ“Ë”»’è
	/// </summary>
	template< typename T >
	inline skyBool Point2CrossRay2( const CVector2< T > &point , const CRay2< T > &ray2 );

	/// <summary>
	/// 2D“_‚Æ‘È‰~‚ÌÕ“Ë”»’è
	/// </summary>
	template< typename T >
	inline skyBool Point2CrossEllipse( const CVector2< T > &point , const CEllipse< T > &ellipse );

	/// <summary>
	/// ‘È‰~‚Æ‘È‰~‚ÌÕ“Ë”»’è
	/// </summary>
	template< typename T >
	inline skyBool EllipseCrossEllipse( const CEllipse< T > &ellipse1 , const CEllipse< T > &ellipse2 );

	/// <summary>
	/// 3D“_‚Æ3D“_‚ÌÕ“Ë”»’è
	/// </summary>
	template< typename T >
	inline skyBool Pos3CrossPos3( const CVector3< T > &pos1 , const CVector3< T > &pos2 );

	/// <summary>
	/// 3D“_‚Æ3DƒŒƒC‚ÌÕ“Ë”»’è
	/// </summary>
	template< typename T >
	inline skyBool Pos3CrossRay3( const CVector3< T > &pos , const CRay3< T > &ray );

	/// <summary>
	/// 3D“_‚Æ3Dü‚ÌÕ“Ë”»’è
	/// </summary>
	template< typename T >
	inline skyBool Pos3CrossLine3( const CVector3< T > &pos , const CLine3< T > &line );

	/// <summary>
	/// 3D“_‚Æ‹…‚ÌÕ“Ë”»’è
	/// </summary>
	template< typename T >
	inline skyBool Pos3CrossSphere( const CVector3< T > &pos , const CSphere< T > &sphere );

	/// <summary>
	/// 3Dü‚Æ3Dü‚ÌÕ“Ë
	/// </summary>
	template< typename T >
	inline skyBool Line3CrossLine3( const CLine3< T > &line1 , const CLine3< T > &line2 );

	/// <summary>
	/// 3Dü‚Æ3DüŒQ‚ÌÕ“Ë
	/// </summary>
	inline skyBool Line3CrossLine3Fence( const CBasicLine3 &line , const math::CBasicVector3Vector &fence , skyBool isHitCheck2D = skyFalse );

	/// <summary>
	/// •½–Êã‚Ì3D“_‚ÆOŠpŒ`‚Ì“àŠO”»’è
	/// </summary>
	template< typename T >
	inline skyBool PlanePos3ToTriangleInSide( const CPlane< T > &plane , const CTriangle3< T > &triangle );

	/// <summary>
	/// 3DƒŒƒC‚Æ•½–Ê‚ÌÕ“Ë•Œğ“_æ“¾(skyTrue:Œğ“_‚ ‚èBskyFalse:Œğ“_‚È‚µj
	/// </summary>
	template< typename T >
	inline skyBool Ray3CrossPlane( const CRay3< T > &ray , const CPlane< T > &plane , CollisionResultInfo *info = skyNull );

	/// <summary>
	/// 3DƒŒƒC‚ÆOŠpŒ`‚ÌÕ“Ë•Œğ“_æ“¾(skyTrue:Œğ“_‚ ‚èBskyFalse:Œğ“_‚È‚µj
	/// </summary>
	template< typename T >
	inline skyBool Ray3CrossTriangle3( const CRay3< T > &ray , const CTriangle3< T > &triangle , CollisionResultInfo *info = skyNull );

	/// <summary>
	/// ‹…‚Æ‹…‚ÌÕ“Ë”»’è
	/// </summary>
	template< typename T >
	inline skyBool SphereCrossSphere( const CSphere< T > &spher1 , const CSphere< T > &spher2 );

	/// <summary>
	/// ‹…‚ÆOŠpŒ`‚ÌÕ“Ë”»’è
	/// </summary>
	template< typename T >
	inline skyBool SphereCrossTriangle3( const CSphere< T > &sphere , const CTriangle3< T > &triangle );

	/// <summary>
	/// ‹…‚Æ•½–Ê‚ÌÕ“Ë”»’è
	/// </summary>
	template< typename T >
	inline skyBool SphereCrossPlane( const CSphere< T > &sphere , const CPlane< T > &plane );

	/// <summary>
	/// ‹…‚Æ3Dü•ª‚ÌÕ“Ë”»’è
	/// </summary>
	template< typename T >
	inline skyBool SphereCrossLine3( const CSphere< T > &sphere , const CLine3< T > &line );

	/// <summary>
	/// ‹…‚Æ3DƒŒƒC‚ÌÕ“Ë”»’è
	/// </summary>
	template< typename T >
	inline skyBool SphereCrossRay3( const CSphere< T > &sphere , const CRay3< T > &ray , CollisionResultInfo *info = skyNull );

	/// <summary>
	/// ‹…‚ÆƒJƒvƒZƒ‹‚ÌÕ“Ë”»’è
	/// </summary>
	template< typename T >
	inline skyBool SphereCrossCapsule3( const CSphere< T > &sphere , const Capsule3< T > &capsule );

	/// <summary>
	/// ƒJƒvƒZƒ‹‚ÆƒJƒvƒZƒ‹‚ÌÕ“Ë”»’è
	/// </summary>
	template< typename T >
	inline skyBool Capsule3CrossCapsule3( const Capsule3< T > &capsule1 , const Capsule3< T > &capsule2 );

	/// <summary>
	/// ‚`‚`‚a‚a‚Q‚Æ“_‚ÌÕ“Ë”»’è
	/// </summary>
	inline skyBool AABB2CrossPoint2( const CAABB2 &aabb2 , const CBasicVector2 &p );

	/// <summary>
	/// ‚`‚`‚a‚a‚Q‚Æü•ª‚Q‚ÌÕ“Ë”»’è
	/// </summary>
	inline skyBool AABB2CrossLine2( const CAABB2 &aabb2 , const CBasicLine2 &line , CollisionResultInfo *info = skyNull );
	
	/// <summary>
	/// ‚`‚`‚a‚a‚Q‚Æ‚`‚`‚a‚a‚Q‚ÌÕ“Ë”»’èiŒğ·‚µ‚Ä‚¢‚Ä‚©‚Âˆø”‚Éw’è‚ª‚ ‚ê‚ÎŒğ·‚É‚æ‚Á‚Ä¶‚¶‚½‚`‚`‚a‚a‚Q‚ğ•Ô‚·j
	/// </summary>
	inline skyBool AABB2CrossAABB2( const CAABB2 &aabb1 ,  const CAABB2 &aabb2 , CAABB2 *boxIntersect = skyNull );

	/// <summary>
	/// ‚`‚`‚a‚a‚R‚Æ“_‚ÌÕ“Ë”»’è
	/// </summary>
	inline skyBool AABB3CrossPoint3( const CAABB3 &aabb3 , const CBasicVector3 &p );

	/// <summary>
	/// ‚`‚`‚a‚a‚R‚Æ‹…‚ÌÕ“Ë”»’è
	/// </summary>
	inline skyBool AABB3CrossSphere( const CAABB3 &aabb3 , const CBasicSphere &sphere );

	/// <summary>
	/// ‚`‚`‚a‚a‚R‚Æü•ª‚R‚ÌÕ“Ë”»’è
	/// </summary>
	inline skyBool AABB3CrossLine3( const CAABB3 &aabb3 , const CBasicLine3 &line , CollisionResultInfo *info = skyNull );

	/// <summary>
	/// ‚`‚`‚a‚a‚R‚Æ•½–Ê‚ÌÕ“Ë”»’è
	/// </summary>
	inline skyBool AABB3CrossPlane( const CAABB3 &aabb3 , const CBasicPlane &plane );

	/// <summary>
	/// ‚`‚`‚a‚a‚R‚ÆOŠpŒ`‚R‚ÌÕ“Ë”»’è
	/// </summary>
	inline skyBool AABB3CrossTriangle3( const CAABB3 &aabb3 , const CBasicTriangle3 &triangle );

	/// <summary>
	/// ‚`‚`‚a‚a‚R‚Æ‚`‚`‚a‚a‚R‚ÌÕ“Ë”»’è
	/// </summary>
	inline skyBool AABB3CrossAABB3( const CAABB3 &aabb1 , const CAABB3 &aabb2 , CAABB3 *boxsIntersect = skyNull );

	/// <summary>
	/// ‚`‚`‚a‚a‚R‚Æ‚n‚a‚a‚R‚ÌÕ“Ë”»’èi¦’·‚¢‚`‚`‚a‚aA‚n‚a‚a“¯m‚¾‚Æ”²‚¯‚Ü‚·B–Ê‚ÌÕ“Ë”»’è‚ğs‚Á‚Ä‚¢‚È‚¢ˆ×‚Å‚·B–³‚¢‚æ‚è‚©‚Íƒ}ƒV‚Æ‚¢‚¤À‘•‚Å‚·Bj
	/// </summary>
	inline skyBool AABB3CrossOBB3( const CAABB3 &aabb3 , const COBB3 &obb );

	/// <summary>
	/// ‚n‚a‚a‚Q‚Æü•ª‚Q‚ÌÕ“Ë”»’è
	/// </summary>
	inline skyBool OBB2CrossLine2( const COBB2 &obb2 , const CBasicLine2 &line );

	/// <summary>
	/// ‚n‚a‚a‚Q‚Æ‚n‚a‚a‚Q‚ÌÕ“Ë”»’è
	/// </summary>
	inline skyBool OBB2CrossOBB2( const COBB2 &obb1 , const COBB2 &obb2 );

	/// <summary>
	/// ‚n‚a‚a‚R‚Æ‹…‚ÌÕ“Ë”»’è
	/// </summary>
	inline skyBool OBB3CrossSphere( const COBB3 &obb , const CBasicSphere &sphere , CollisionResultInfo *info = skyNull ); //clossPoint‚ÍÀÛ‚Í‚n‚a‚a‚R‚Ì“_‚Ì’†‚Å‚à‚Á‚Æ‚à‹…‚Ì’†S‚É‹ß‚¢“_Õ“Ë“_‚Ì‹ß—’l

	/// <summary>
	/// ‚n‚a‚a‚R‚Æü•ª‚R‚ÌÕ“Ë”»’è
	/// </summary>
	inline skyBool OBB3CrossLine3( const COBB3 &obb , const CBasicLine3 &line );

	/// <summary>
	/// ‚n‚a‚a‚R‚Æ°‚ÌÕ“Ë”»’è
	/// </summary>
	inline skyBool OBB3CrossPlane( const COBB3 &obb , const CBasicPlane &plane );

	/// <summary>
	/// ‚n‚a‚a‚R‚Æ‚n‚a‚a‚R‚ÌÕ“Ë”»’è
	/// </summary>
	inline skyBool OBB3CrossOBB3( const COBB3 &obb1 , const COBB3 &obb2 );

}

} } }

#include "GeometryCollisionUtil.inl"
