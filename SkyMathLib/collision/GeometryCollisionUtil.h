#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// 幾何学衝突便利処理群
/// memo
/// ゲームの衝突判定は最初に単純矩形、次に円の衝突判定を行う。
/// </summary>
namespace GeometryCollisionUtil {

	/// <summary>
	/// 2D点と2D点の衝突判定
	/// </summary>
	template< typename T >
	inline skyBool Pos2CrossPos2( const CVector2< T > &pos1 , const CVector2< T > &pos2 , CollisionResultInfo *info = skyNull );

	/// <summary>
	/// 2D点と2D点を通る直線の角度をラジアンで返す
	/// </summary>
	template< typename T >
	inline T Pos2AndPos2Angle( const CVector2< T > &pos1 , const CVector2< T > &pos2 );

	/// <summary>
	/// 2D点から2D線の垂線ベクトルを求める( 点と線が直行で交わらない場合、垂線ベクトルは存在しないのでskyFalseが返る)
	/// </summary>
	template< typename T >
	inline skyBool Pos2ToLineVector( const CVector2< T > &pos , const CLine2< T > &line , CVector2< T > &dest );

	/// <summary>
	/// 2D点が2D線の始点から終点へ伸びるベクトルを進行方向として左( 1 )、右( - 1 )どちら側にあるか、直線上( 0 )にあるかを判定する
	/// </summary>
	template< typename T >
	inline T Pos2ToLineSide( const CVector2< T > &pos , const CLine2< T > &line );

	/// <summary>
	/// 2D線と2D線の交差判定
	/// 0: 交差していない 1: 一点で交差している 2: 一点以上で交差している
	/// </summary>
	template< typename T >
	inline s32 Line2CrossLine2( const CLine2< T > &line1 , const CLine2< T > &line2 , CollisionResultInfo *info = skyNull );

	/// <summary>
	/// 2D点と2D線の衝突判定
	/// </summary>
	template< typename T >
	inline skyBool Point2CrossLine2( const CVector2< T > &point ,  const CLine2< T > &line );

	/// <summary>
	/// 円と円の衝突判定
	/// </summary>
	template< typename T >
	inline skyBool CircleCrossCircle( const CCircle< T > &circle1 , const CCircle< T > &circle2 );

	/// <summary>
	/// 2D点と矩形の衝突判定
	/// </summary>
	template< typename T >
	inline skyBool Point2CrossRect( const CVector2< T > &pos , const CRect< T > &rect );

	/// <summary>
	/// 矩形と矩形の衝突判定(左上座標と幅と高さ）
	/// </summary>
	template< typename T >
	inline skyBool RectCrossRect( const CRect< T > &rect1 , const CRect< T > &rect2 );

	/// <summary>
	/// 四角形と四角形の衝突判定(中心座標と幅と高さ）
	/// </summary>
	template< typename T >
	inline skyBool RectCrossRect( const CVector2< T > &center1 , const CSize< T > &size1 , const CVector2< T > &center2 , const CSize< T > &size2 );

	/// <summary>
	/// 2D点と円の衝突判定
	/// </summary>
	template< typename T >
	inline skyBool Point2CrossCircle( const CVector2< T > &point , const CCircle< T > &circle );

	/// <summary>
	/// 2D線と円の衝突判定
	/// </summary>
	template< typename T >
	inline skyBool Line2CrossCircle( const CLine2< T > &line , const CCircle< T > &circle );

	/// <summary>
	/// 2D点と2Dレイとの衝突判定
	/// </summary>
	template< typename T >
	inline skyBool Point2CrossRay2( const CVector2< T > &point , const CRay2< T > &ray2 );

	/// <summary>
	/// 2D点と楕円の衝突判定
	/// </summary>
	template< typename T >
	inline skyBool Point2CrossEllipse( const CVector2< T > &point , const CEllipse< T > &ellipse );

	/// <summary>
	/// 楕円と楕円の衝突判定
	/// </summary>
	template< typename T >
	inline skyBool EllipseCrossEllipse( const CEllipse< T > &ellipse1 , const CEllipse< T > &ellipse2 );

	/// <summary>
	/// 3D点と3D点の衝突判定
	/// </summary>
	template< typename T >
	inline skyBool Pos3CrossPos3( const CVector3< T > &pos1 , const CVector3< T > &pos2 );

	/// <summary>
	/// 3D点と3Dレイの衝突判定
	/// </summary>
	template< typename T >
	inline skyBool Pos3CrossRay3( const CVector3< T > &pos , const CRay3< T > &ray );

	/// <summary>
	/// 3D点と3D線の衝突判定
	/// </summary>
	template< typename T >
	inline skyBool Pos3CrossLine3( const CVector3< T > &pos , const CLine3< T > &line );

	/// <summary>
	/// 3D点と球の衝突判定
	/// </summary>
	template< typename T >
	inline skyBool Pos3CrossSphere( const CVector3< T > &pos , const CSphere< T > &sphere );

	/// <summary>
	/// 3D線と3D線の衝突
	/// </summary>
	template< typename T >
	inline skyBool Line3CrossLine3( const CLine3< T > &line1 , const CLine3< T > &line2 );

	/// <summary>
	/// 3D線と3D線群の衝突
	/// </summary>
	inline skyBool Line3CrossLine3Fence( const CBasicLine3 &line , const math::CBasicVector3Vector &fence , skyBool isHitCheck2D = skyFalse );

	/// <summary>
	/// 平面上の3D点と三角形の内外判定
	/// </summary>
	template< typename T >
	inline skyBool PlanePos3ToTriangleInSide( const CPlane< T > &plane , const CTriangle3< T > &triangle );

	/// <summary>
	/// 3Dレイと平面の衝突＆交点取得(skyTrue:交点あり。skyFalse:交点なし）
	/// </summary>
	template< typename T >
	inline skyBool Ray3CrossPlane( const CRay3< T > &ray , const CPlane< T > &plane , CollisionResultInfo *info = skyNull );

	/// <summary>
	/// 3Dレイと三角形の衝突＆交点取得(skyTrue:交点あり。skyFalse:交点なし）
	/// </summary>
	template< typename T >
	inline skyBool Ray3CrossTriangle3( const CRay3< T > &ray , const CTriangle3< T > &triangle , CollisionResultInfo *info = skyNull );

	/// <summary>
	/// 球と球の衝突判定
	/// </summary>
	template< typename T >
	inline skyBool SphereCrossSphere( const CSphere< T > &spher1 , const CSphere< T > &spher2 );

	/// <summary>
	/// 球と三角形の衝突判定
	/// </summary>
	template< typename T >
	inline skyBool SphereCrossTriangle3( const CSphere< T > &sphere , const CTriangle3< T > &triangle );

	/// <summary>
	/// 球と平面の衝突判定
	/// </summary>
	template< typename T >
	inline skyBool SphereCrossPlane( const CSphere< T > &sphere , const CPlane< T > &plane );

	/// <summary>
	/// 球と3D線分の衝突判定
	/// </summary>
	template< typename T >
	inline skyBool SphereCrossLine3( const CSphere< T > &sphere , const CLine3< T > &line );

	/// <summary>
	/// 球と3Dレイの衝突判定
	/// </summary>
	template< typename T >
	inline skyBool SphereCrossRay3( const CSphere< T > &sphere , const CRay3< T > &ray , CollisionResultInfo *info = skyNull );

	/// <summary>
	/// 球とカプセルの衝突判定
	/// </summary>
	template< typename T >
	inline skyBool SphereCrossCapsule3( const CSphere< T > &sphere , const Capsule3< T > &capsule );

	/// <summary>
	/// カプセルとカプセルの衝突判定
	/// </summary>
	template< typename T >
	inline skyBool Capsule3CrossCapsule3( const Capsule3< T > &capsule1 , const Capsule3< T > &capsule2 );

	/// <summary>
	/// ＡＡＢＢ２と点の衝突判定
	/// </summary>
	inline skyBool AABB2CrossPoint2( const CAABB2 &aabb2 , const CBasicVector2 &p );

	/// <summary>
	/// ＡＡＢＢ２と線分２の衝突判定
	/// </summary>
	inline skyBool AABB2CrossLine2( const CAABB2 &aabb2 , const CBasicLine2 &line , CollisionResultInfo *info = skyNull );
	
	/// <summary>
	/// ＡＡＢＢ２とＡＡＢＢ２の衝突判定（交差していてかつ引数に指定があれば交差によって生じたＡＡＢＢ２を返す）
	/// </summary>
	inline skyBool AABB2CrossAABB2( const CAABB2 &aabb1 ,  const CAABB2 &aabb2 , CAABB2 *boxIntersect = skyNull );

	/// <summary>
	/// ＡＡＢＢ３と点の衝突判定
	/// </summary>
	inline skyBool AABB3CrossPoint3( const CAABB3 &aabb3 , const CBasicVector3 &p );

	/// <summary>
	/// ＡＡＢＢ３と球の衝突判定
	/// </summary>
	inline skyBool AABB3CrossSphere( const CAABB3 &aabb3 , const CBasicSphere &sphere );

	/// <summary>
	/// ＡＡＢＢ３と線分３の衝突判定
	/// </summary>
	inline skyBool AABB3CrossLine3( const CAABB3 &aabb3 , const CBasicLine3 &line , CollisionResultInfo *info = skyNull );

	/// <summary>
	/// ＡＡＢＢ３と平面の衝突判定
	/// </summary>
	inline skyBool AABB3CrossPlane( const CAABB3 &aabb3 , const CBasicPlane &plane );

	/// <summary>
	/// ＡＡＢＢ３と三角形３の衝突判定
	/// </summary>
	inline skyBool AABB3CrossTriangle3( const CAABB3 &aabb3 , const CBasicTriangle3 &triangle );

	/// <summary>
	/// ＡＡＢＢ３とＡＡＢＢ３の衝突判定
	/// </summary>
	inline skyBool AABB3CrossAABB3( const CAABB3 &aabb1 , const CAABB3 &aabb2 , CAABB3 *boxsIntersect = skyNull );

	/// <summary>
	/// ＡＡＢＢ３とＯＢＢ３の衝突判定（※長いＡＡＢＢ、ＯＢＢ同士だと抜けます。面の衝突判定を行っていない為です。無いよりかはマシという実装です。）
	/// </summary>
	inline skyBool AABB3CrossOBB3( const CAABB3 &aabb3 , const COBB3 &obb );

	/// <summary>
	/// ＯＢＢ２と線分２の衝突判定
	/// </summary>
	inline skyBool OBB2CrossLine2( const COBB2 &obb2 , const CBasicLine2 &line );

	/// <summary>
	/// ＯＢＢ２とＯＢＢ２の衝突判定
	/// </summary>
	inline skyBool OBB2CrossOBB2( const COBB2 &obb1 , const COBB2 &obb2 );

	/// <summary>
	/// ＯＢＢ３と球の衝突判定
	/// </summary>
	inline skyBool OBB3CrossSphere( const COBB3 &obb , const CBasicSphere &sphere , CollisionResultInfo *info = skyNull ); //clossPointは実際はＯＢＢ３の点の中でもっとも球の中心に近い点衝突点の近似値

	/// <summary>
	/// ＯＢＢ３と線分３の衝突判定
	/// </summary>
	inline skyBool OBB3CrossLine3( const COBB3 &obb , const CBasicLine3 &line );

	/// <summary>
	/// ＯＢＢ３と床の衝突判定
	/// </summary>
	inline skyBool OBB3CrossPlane( const COBB3 &obb , const CBasicPlane &plane );

	/// <summary>
	/// ＯＢＢ３とＯＢＢ３の衝突判定
	/// </summary>
	inline skyBool OBB3CrossOBB3( const COBB3 &obb1 , const COBB3 &obb2 );

}

} } }

#include "GeometryCollisionUtil.inl"
