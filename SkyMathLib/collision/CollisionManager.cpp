#include "StdAfx.h"
#include "SkyMathLib/Collision.h"

namespace sky { namespace lib { namespace math {

void CollisionManager::ResisterDataRequest( const CollisionDataPtr &data )
{
	stRequestData &requestData = m_RequestList.AddTail();

	requestData.m_eRequestKind = eRequestKind_Resister;
	requestData.m_spCollisionData = data;
}

void CollisionManager::UnResisterDataRequest( const CollisionDataPtr &data )
{
	stRequestData &requestData = m_RequestList.AddTail();

	requestData.m_eRequestKind = eRequestKind_UnResister;
	requestData.m_spCollisionData = data;
}

void CollisionManager::Detection( s32 kindId , u32 tagBit )
{
	CollisionDataPtrList endList;

	FOREACH_NOINC( CollisionDataPtrList , it , m_CollisionDataList )
	{
		//アイテムを変数に格納
		CollisionDataPtr pTemp = *it;

		//判定済みリスト
		endList.AddTail( *it );

		//判定リストから削除
		it = m_CollisionDataList.Erases( it );

		//幾何形状を取得する
		const CGeometryData *pGeometory = pTemp->m_pGeometry;
		if ( pGeometory == skyNull )
		{
			pGeometory = pTemp->m_spGeometoryGetDelegate.GetInsNoConst()();
		}

		//衝突判定処理
		switch( pGeometory->m_eType )
		{
			case eGeometoryType_point2:
				Collision_point2( ( const CBasicVector2 * )pGeometory->m_Geometry , pGeometory->m_eType , pTemp , kindId , tagBit );
				break;
			case eGeometoryType_point3:
				Collision_point3( ( const CBasicVector3 * )pGeometory->m_Geometry , pGeometory->m_eType , pTemp , kindId , tagBit );
				break;
			case eGeometoryType_point4:
				Collision_point4( ( const CBasicVector4 * )pGeometory->m_Geometry , pGeometory->m_eType , pTemp , kindId , tagBit );
				break;
			case eGeometoryType_circle:
				Collision_circle( ( const CBasicCircle * )pGeometory->m_Geometry , pGeometory->m_eType , pTemp , kindId , tagBit );
				break;
			case eGeometoryType_sphere:
				Collision_sphere( ( const CBasicSphere * )pGeometory->m_Geometry , pGeometory->m_eType , pTemp , kindId , tagBit );
				break;
			case eGeometoryType_ellipse:
				Collision_ellipse( ( const CBasicEllipse * )pGeometory->m_Geometry , pGeometory->m_eType , pTemp , kindId , tagBit );
				break;
			case eGeometoryType_line2:
				Collision_line2( ( const CBasicLine2 * )pGeometory->m_Geometry , pGeometory->m_eType , pTemp , kindId , tagBit );
				break;
			case eGeometoryType_line3:
				Collision_line3( ( const CBasicLine3 * )pGeometory->m_Geometry , pGeometory->m_eType , pTemp , kindId , tagBit );
				break;
			case eGeometoryType_ray2:
				Collision_ray2( ( const CBasicRay2 * )pGeometory->m_Geometry , pGeometory->m_eType , pTemp , kindId , tagBit );
				break;
			case eGeometoryType_ray3:
				Collision_ray3( ( const CBasicRay3 * )pGeometory->m_Geometry , pGeometory->m_eType , pTemp , kindId , tagBit );
				break;
			case eGeometoryType_plane:
				Collision_plane( ( const CBasicPlane * )pGeometory->m_Geometry , pGeometory->m_eType , pTemp , kindId , tagBit );
				break;
			case eGeometoryType_triangle2:
				Collision_triangle2( ( const CBasicTriangle2 * )pGeometory->m_Geometry , pGeometory->m_eType , pTemp , kindId , tagBit );
				break;
			case eGeometoryType_triangle3:
				Collision_triangle3( ( const CBasicTriangle3 * )pGeometory->m_Geometry , pGeometory->m_eType , pTemp , kindId , tagBit );
				break;
			case eGeometoryType_capsule2:
				Collision_capsule2( ( const CBasicCapsule2 * )pGeometory->m_Geometry , pGeometory->m_eType , pTemp , kindId , tagBit );
				break;
			case eGeometoryType_capsule3:
				Collision_capsule3( ( const CBasicCapsule3 * )pGeometory->m_Geometry , pGeometory->m_eType , pTemp , kindId , tagBit );
				break;
			case eGeometoryType_rect:
				Collision_rect( ( const CBasicRect * )pGeometory->m_Geometry , pGeometory->m_eType , pTemp , kindId , tagBit );
				break;
			case eGeometoryType_aabb2:
				Collision_aabb2( ( const CAABB2 * )pGeometory->m_Geometry , pGeometory->m_eType , pTemp , kindId , tagBit );
				break;
			case eGeometoryType_aabb3:
				Collision_aabb3( ( const CAABB3 * )pGeometory->m_Geometry , pGeometory->m_eType , pTemp , kindId , tagBit );
				break;
			case eGeometoryType_obb2:
				Collision_obb2( ( const COBB2 * )pGeometory->m_Geometry , pGeometory->m_eType , pTemp , kindId , tagBit );
				break;
			case eGeometoryType_obb3:
				Collision_obb3( ( const COBB3 * )pGeometory->m_Geometry , pGeometory->m_eType , pTemp , kindId , tagBit );
				break;
		}
	}

	//判定リストに再び追加する（この処理なんとかならないかな）
	FOREACH( CollisionDataPtrList , it , endList )
	{
		m_CollisionDataList.AddTail( *it );
	}
}

skyBool CollisionManager::Detection( const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *hitList )
{
	skyBool result = skyFalse;

	//幾何形状を取得する
	const CGeometryData *pGeometory = data->m_pGeometry;
	if ( pGeometory == skyNull )
	{
		pGeometory = data->m_spGeometoryGetDelegate.GetInsNoConst()();
	}

	//衝突判定処理
	switch( pGeometory->m_eType )
	{
		case eGeometoryType_point2:
			result = Collision_point2( ( const CBasicVector2 * )pGeometory->m_Geometry , pGeometory->m_eType , data , kindId , tagBit , hitList );
			break;
		case eGeometoryType_point3:
			result = Collision_point3( ( const CBasicVector3 * )pGeometory->m_Geometry , pGeometory->m_eType , data , kindId , tagBit , hitList );
			break;
		case eGeometoryType_point4:
			result = Collision_point4( ( const CBasicVector4 * )pGeometory->m_Geometry , pGeometory->m_eType , data , kindId , tagBit , hitList );
			break;
		case eGeometoryType_circle:
			result = Collision_circle( ( const CBasicCircle * )pGeometory->m_Geometry , pGeometory->m_eType , data , kindId , tagBit , hitList );
			break;
		case eGeometoryType_sphere:
			result = Collision_sphere( ( const CBasicSphere * )pGeometory->m_Geometry , pGeometory->m_eType , data , kindId , tagBit , hitList );
			break;
		case eGeometoryType_ellipse:
			result = Collision_ellipse( ( const CBasicEllipse * )pGeometory->m_Geometry , pGeometory->m_eType , data , kindId , tagBit , hitList );
			break;
		case eGeometoryType_line2:
			result = Collision_line2( ( const CBasicLine2 * )pGeometory->m_Geometry , pGeometory->m_eType , data , kindId , tagBit , hitList );
			break;
		case eGeometoryType_line3:
			result = Collision_line3( ( const CBasicLine3 * )pGeometory->m_Geometry , pGeometory->m_eType , data , kindId , tagBit , hitList );
			break;
		case eGeometoryType_ray2:
			result = Collision_ray2( ( const CBasicRay2 * )pGeometory->m_Geometry , pGeometory->m_eType , data , kindId , tagBit , hitList );
			break;
		case eGeometoryType_ray3:
			result = Collision_ray3( ( const CBasicRay3 * )pGeometory->m_Geometry , pGeometory->m_eType , data , kindId , tagBit , hitList );
			break;
		case eGeometoryType_plane:
			result = Collision_plane( ( const CBasicPlane * )pGeometory->m_Geometry , pGeometory->m_eType , data , kindId , tagBit , hitList );
			break;
		case eGeometoryType_triangle2:
			result = Collision_triangle2( ( const CBasicTriangle2 * )pGeometory->m_Geometry , pGeometory->m_eType , data , kindId , tagBit , hitList );
			break;
		case eGeometoryType_triangle3:
			result = Collision_triangle3( ( const CBasicTriangle3 * )pGeometory->m_Geometry , pGeometory->m_eType , data , kindId , tagBit , hitList );
			break;
		case eGeometoryType_capsule2:
			result = Collision_capsule2( ( const CBasicCapsule2 * )pGeometory->m_Geometry , pGeometory->m_eType , data , kindId , tagBit , hitList );
			break;
		case eGeometoryType_capsule3:
			result = Collision_capsule3( ( const CBasicCapsule3 * )pGeometory->m_Geometry , pGeometory->m_eType , data , kindId , tagBit , hitList );
			break;
		case eGeometoryType_rect:
			result = Collision_rect( ( const CBasicRect * )pGeometory->m_Geometry , pGeometory->m_eType , data , kindId , tagBit , hitList );
			break;
		case eGeometoryType_aabb2:
			result = Collision_aabb2( ( const CAABB2 * )pGeometory->m_Geometry , pGeometory->m_eType , data , kindId , tagBit , hitList );
			break;
		case eGeometoryType_aabb3:
			result = Collision_aabb3( ( const CAABB3 * )pGeometory->m_Geometry , pGeometory->m_eType , data , kindId , tagBit , hitList );
			break;
		case eGeometoryType_obb2:
			result = Collision_obb2( ( const COBB2 * )pGeometory->m_Geometry , pGeometory->m_eType , data , kindId , tagBit , hitList );
			break;
		case eGeometoryType_obb3:
			result = Collision_obb3( ( const COBB3 * )pGeometory->m_Geometry , pGeometory->m_eType , data , kindId , tagBit , hitList );
			break;
	}

	return result;
}

void CollisionManager::ClearTagBit()
{
	FOREACH( CollisionDataPtrList , it , m_CollisionDataList )
	{
		( *it )->ClearTagBit();
	}
}

void CollisionManager::Update()
{
	//要求を実行
	FOREACH( CRequestDataList , it , m_RequestList )
	{
		switch ( ( *it ).m_eRequestKind )
		{
		case eRequestKind_Resister:
			m_CollisionDataList.AddTail( ( *it ).m_spCollisionData );
			break;

		case eRequestKind_UnResister:
			m_CollisionDataList.Delete( ( *it ).m_spCollisionData );
			break;
		}
	}

	//要求リストをクリア
	m_RequestList.DeleteAll();
}

const CollisionDataPtrList &CollisionManager::GetKindDataList( s32 kindId , CollisionDataPtrList &dest )
{
	FOREACH( CollisionDataPtrList , it , m_CollisionDataList )
	{
		if ( ( *it )->GetKindId() == kindId )
		{
			dest.AddTail( *it );
		}
	}
	return dest;
}

#define PRE_COLLISION_PROC() \
const CGeometryData *pGeometory = ( *it )->m_pGeometry; \
if ( pGeometory == skyNull ) \
{ \
	pGeometory = ( *it )->m_spGeometoryGetDelegate.GetInsNoConst()(); \
} \
skyBool isCollision = skyFalse; \
if ( ( kindId == COLLISION_KIND_ID_ALL || kindId == ( *it )->GetKindId() ) && \
	data != ( *it ) && \
	( data == skyNull || \
	( data->m_IsStopSameKindCollision == skyTrue && data->m_KindID == ( *it )->m_KindID ) == skyFalse && \
	( data->m_spExecDetectionDelegate.IsEmpty() == skyTrue || data->m_spExecDetectionDelegate.GetInsNoConst()( ( *it ) ) == skyTrue ) ) \
	) \
{ \
	isCollision = skyFalse; \
	CollisionResultInfo info; \
	CollisionResultInfo *pInfo = skyNull; \
	if ( data->IsHitResultGet() == skyTrue ) \
	{ \
		pInfo = &info; \
	}

#define POST_COLLISION_PROC( geometory , type ) \
CGeometryData geoData( type , static_cast< const sky::lib::math::CGeometryBase * >( geometory ) ); \
result = result || isCollision; \
if ( isCollision == skyTrue ) \
{ \
	( *it )->TagBitOn( tagBit ); \
	if ( pHitList != skyNull ) pHitList->AddTail( ( *it ) ); \
	if ( data != skyNull && data->m_spCollisionCBDelegate.IsEmpty() == skyFalse ) \
	{ \
		data->m_spCollisionCBDelegate.GetInsNoConst()( &geoData , *it , pInfo ); \
	} \
	if ( ( *it )->m_spCollisionCBDelegate.IsEmpty() == skyFalse ) \
	{ \
		( *it )->m_spCollisionCBDelegate.GetInsNoConst()( &geoData , data , pInfo ); \
	} \
	result = skyTrue; \
} \
}

skyBool CollisionManager::Collision_point2( const CBasicVector2 *pPoint2 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList )
{
	skyBool result = skyFalse;

	FOREACH( CollisionDataPtrList , it , m_CollisionDataList )
	{
		PRE_COLLISION_PROC()

			//衝突判定処理
			switch( pGeometory->m_eType )
			{
				case eGeometoryType_point2:
					{
						isCollision = GeometryCollisionUtil::Pos2CrossPos2< dectype >( *pPoint2 , *( const CBasicVector2 * )pGeometory , pInfo );
					}
				case eGeometoryType_circle:
					{
						isCollision = GeometryCollisionUtil::Point2CrossCircle< dectype >( *pPoint2 , *( const CBasicCircle * )pGeometory );
					}
					break;
				case eGeometoryType_ellipse:
					{
						isCollision = GeometryCollisionUtil::Point2CrossEllipse< dectype >( *pPoint2 , *( const CBasicEllipse * )pGeometory );
					}
					break;
				case eGeometoryType_line2:
					{
						isCollision = GeometryCollisionUtil::Point2CrossLine2< dectype >( *pPoint2 , *( const CBasicLine2 * )pGeometory );
					}
					break;
				case eGeometoryType_ray2:
					{
						isCollision = GeometryCollisionUtil::Point2CrossRay2< dectype >( *pPoint2 , *( const CBasicRay2 * )pGeometory );
					}
					break;
				case eGeometoryType_triangle2:
					{
					}
					break;
				case eGeometoryType_capsule2:
					{
					}
					break;
				case eGeometoryType_rect:
					{
						isCollision = GeometryCollisionUtil::Point2CrossRect< dectype >( *pPoint2 , *( const CBasicRect * )pGeometory );
					}
					break;
				case eGeometoryType_aabb2:
					{
						isCollision = GeometryCollisionUtil::AABB2CrossPoint2( *( const CAABB2 * )pGeometory , *pPoint2 );
					}
					break;
				case eGeometoryType_obb2:
					{
					}
					break;
			}

		POST_COLLISION_PROC( pPoint2 , eType )
	}

	return result;
}

skyBool CollisionManager::Collision_point3( const CBasicVector3 *pPoint3 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList )
{
	skyBool result = skyFalse;

	FOREACH( CollisionDataPtrList , it , m_CollisionDataList )
	{
		PRE_COLLISION_PROC()

			//衝突判定処理
			switch( pGeometory->m_eType )
			{
				case eGeometoryType_point3:
					{
						isCollision = GeometryCollisionUtil::Pos3CrossPos3< dectype >( *pPoint3 , *( const CBasicVector3 * )pGeometory );
					}
				case eGeometoryType_sphere:
					{
						isCollision = GeometryCollisionUtil::Pos3CrossSphere< dectype >( *pPoint3 , *( const CBasicSphere * )pGeometory );
					}
					break;
				case eGeometoryType_line3:
					{
						isCollision = GeometryCollisionUtil::Pos3CrossLine3< dectype >( *pPoint3 , *( const CBasicLine3 * )pGeometory );
					}
					break;
				case eGeometoryType_ray3:
					{
						isCollision = GeometryCollisionUtil::Pos3CrossRay3< dectype >( *pPoint3 , *( const CBasicRay3 * )pGeometory );
					}
					break;
				case eGeometoryType_plane:
					{
					}
					break;
				case eGeometoryType_triangle3:
					{
					}
					break;
				case eGeometoryType_capsule3:
					{
					}
					break;
				case eGeometoryType_aabb3:
					{
						isCollision = GeometryCollisionUtil::AABB3CrossPoint3( *( const CAABB3 * )pGeometory , *pPoint3 );
					}
					break;
				case eGeometoryType_obb3:
					{
					}
					break;
			}

		POST_COLLISION_PROC( pPoint3 , eType )
	}

	return result;
}

skyBool CollisionManager::Collision_point4( const CBasicVector4 *pPoint4 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList )
{//一応枠は用意するが実装の予定はない
	SKY_UNUSED_ARG( pPoint4 );
	SKY_UNUSED_ARG( pHitList );
	SKY_UNUSED_ARG( tagBit );
	SKY_UNUSED_ARG( kindId );
	SKY_UNUSED_ARG( data );
	skyBool result = skyFalse;
	return result;
}

skyBool CollisionManager::Collision_circle( const CBasicCircle *pCircle , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList )
{
	skyBool result = skyFalse;

	FOREACH( CollisionDataPtrList , it , m_CollisionDataList )
	{
		PRE_COLLISION_PROC()

			//衝突判定処理
			switch( pGeometory->m_eType )
			{
				case eGeometoryType_point2:
					{
						isCollision = GeometryCollisionUtil::Point2CrossCircle< dectype >( *( const CBasicVector2 * )pGeometory , *pCircle );
					}
				case eGeometoryType_circle:
					{
						isCollision = GeometryCollisionUtil::CircleCrossCircle< dectype >( *pCircle , *( const CBasicCircle * )pGeometory );
					}
					break;
				case eGeometoryType_ellipse:
					{
					}
					break;
				case eGeometoryType_line2:
					{
						isCollision = GeometryCollisionUtil::Line2CrossCircle< dectype >( *( const CBasicLine2 * )pGeometory , *pCircle );
					}
					break;
				case eGeometoryType_ray2:
					{
					}
					break;
				case eGeometoryType_triangle2:
					{
					}
					break;
				case eGeometoryType_capsule2:
					{
					}
					break;
				case eGeometoryType_rect:
					{
					}
					break;
				case eGeometoryType_aabb2:
					{
					}
					break;
				case eGeometoryType_obb2:
					{
					}
					break;
			}

		POST_COLLISION_PROC( pCircle , eType )
	}

	return result;
}
skyBool CollisionManager::Collision_line2( const CBasicLine2 *pLine2 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList )
{
	skyBool result = skyFalse;

	FOREACH( CollisionDataPtrList , it , m_CollisionDataList )
	{
		PRE_COLLISION_PROC()

			//衝突判定処理
			switch( pGeometory->m_eType )
			{
				case eGeometoryType_point2:
					{
						isCollision = GeometryCollisionUtil::Point2CrossLine2< dectype >( *( const CBasicVector2 * )pGeometory , *pLine2 );
					}
				case eGeometoryType_circle:
					{
						isCollision = GeometryCollisionUtil::Line2CrossCircle< dectype >( *pLine2 , *( const CBasicCircle * )pGeometory );
					}
					break;
				case eGeometoryType_ellipse:
					{
					}
					break;
				case eGeometoryType_line2:
					{
						isCollision = ( GeometryCollisionUtil::Line2CrossLine2< dectype >( *pLine2 , *( const CBasicLine2 * )pGeometory , pInfo ) == 0 ) ? skyFalse : skyTrue;
					}
					break;
				case eGeometoryType_ray2:
					{
					}
					break;
				case eGeometoryType_triangle2:
					{
					}
					break;
				case eGeometoryType_capsule2:
					{
					}
					break;
				case eGeometoryType_rect:
					{
					}
					break;
				case eGeometoryType_aabb2:
					{
						isCollision = GeometryCollisionUtil::AABB2CrossLine2( *( const CAABB2 * )pGeometory , *pLine2 , pInfo );
					}
					break;
				case eGeometoryType_obb2:
					{
						isCollision = GeometryCollisionUtil::OBB2CrossLine2( *( const COBB2 * )pGeometory , *pLine2 );
					}
					break;
			}

		POST_COLLISION_PROC( pLine2 , eType )
	}

	return result;
}
skyBool CollisionManager::Collision_line3( const CBasicLine3 *pLine3 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList )
{
	skyBool result = skyFalse;

	FOREACH( CollisionDataPtrList , it , m_CollisionDataList )
	{
		PRE_COLLISION_PROC()

			//衝突判定処理
			switch( pGeometory->m_eType )
			{
				case eGeometoryType_point3:
					{
						isCollision = GeometryCollisionUtil::Pos3CrossLine3< dectype >( *( const CBasicVector3 * )pGeometory , *pLine3 );
					}
				case eGeometoryType_sphere:
					{
						isCollision = GeometryCollisionUtil::SphereCrossLine3< dectype >( *( const CBasicSphere * )pGeometory , *pLine3 );
					}
					break;
				case eGeometoryType_line3:
					{
						isCollision = GeometryCollisionUtil::Line3CrossLine3< dectype >( *pLine3 , *( const CBasicLine3 * )pGeometory );
					}
					break;
				case eGeometoryType_ray3:
					{
					}
					break;
				case eGeometoryType_plane:
					{
					}
					break;
				case eGeometoryType_triangle3:
					{
					}
					break;
				case eGeometoryType_capsule3:
					{
					}
					break;
				case eGeometoryType_aabb3:
					{
						isCollision = GeometryCollisionUtil::AABB3CrossLine3( *( const CAABB3 * )pGeometory , *pLine3 , pInfo );
					}
					break;
				case eGeometoryType_obb3:
					{
						isCollision = GeometryCollisionUtil::OBB3CrossLine3( *( const COBB3 * )pGeometory , *pLine3 );
					}
					break;
			}

		POST_COLLISION_PROC( pLine3 , eType )
	}

	return result;
}
skyBool CollisionManager::Collision_ray2( const CBasicRay2 *pRay2 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList )
{
	skyBool result = skyFalse;

	FOREACH( CollisionDataPtrList , it , m_CollisionDataList )
	{
		PRE_COLLISION_PROC()

			//衝突判定処理
			switch( pGeometory->m_eType )
			{
				case eGeometoryType_point2:
					{
						isCollision = GeometryCollisionUtil::Point2CrossRay2< dectype >( *( const CBasicVector2 * )pGeometory , *pRay2 );
					}
				case eGeometoryType_circle:
					{
					}
					break;
				case eGeometoryType_ellipse:
					{
					}
					break;
				case eGeometoryType_line2:
					{
					}
					break;
				case eGeometoryType_ray2:
					{
					}
					break;
				case eGeometoryType_triangle2:
					{
					}
					break;
				case eGeometoryType_capsule2:
					{
					}
					break;
				case eGeometoryType_rect:
					{
					}
					break;
				case eGeometoryType_aabb2:
					{
					}
					break;
				case eGeometoryType_obb2:
					{
					}
					break;
			}

		POST_COLLISION_PROC( pRay2 , eType )
	}

	return result;
}
skyBool CollisionManager::Collision_ray3( const CBasicRay3 *pRay3 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList )
{
	skyBool result = skyFalse;

	FOREACH( CollisionDataPtrList , it , m_CollisionDataList )
	{
		PRE_COLLISION_PROC()

			//衝突判定処理
			switch( pGeometory->m_eType )
			{
				case eGeometoryType_point3:
					{
						isCollision = GeometryCollisionUtil::Pos3CrossRay3< dectype >( *( const CBasicVector3 * )pGeometory , *pRay3 );
					}
				case eGeometoryType_sphere:
					{
						isCollision = GeometryCollisionUtil::SphereCrossRay3< dectype >( *( const CBasicSphere * )pGeometory , *pRay3 , pInfo );
					}
					break;
				case eGeometoryType_line3:
					{
					}
					break;
				case eGeometoryType_ray3:
					{
					}
					break;
				case eGeometoryType_plane:
					{
						isCollision = GeometryCollisionUtil::Ray3CrossPlane< dectype >( *pRay3 , *( const CBasicPlane * )pGeometory , pInfo );
					}
					break;
				case eGeometoryType_triangle3:
					{
						isCollision = GeometryCollisionUtil::Ray3CrossTriangle3< dectype >( *pRay3 , *( const CBasicTriangle3 * )pGeometory , pInfo );
					}
					break;
				case eGeometoryType_capsule3:
					{
					}
					break;
				case eGeometoryType_aabb3:
					{
					}
					break;
				case eGeometoryType_obb3:
					{
					}
					break;
			}

		POST_COLLISION_PROC( pRay3 , eType )
	}

	return result;
}
skyBool CollisionManager::Collision_sphere( const CBasicSphere *pSphere , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList )
{
	skyBool result = skyFalse;

	FOREACH( CollisionDataPtrList , it , m_CollisionDataList )
	{
		PRE_COLLISION_PROC()

			//衝突判定処理
			switch( pGeometory->m_eType )
			{
				case eGeometoryType_point3:
					{
						isCollision = GeometryCollisionUtil::Pos3CrossSphere< dectype >( *( const CBasicVector3 * )pGeometory , *pSphere );
					}
				case eGeometoryType_sphere:
					{
						isCollision = GeometryCollisionUtil::SphereCrossSphere< dectype >( *pSphere , *( const CBasicSphere * )pGeometory );
					}
					break;
				case eGeometoryType_line3:
					{
						isCollision = GeometryCollisionUtil::SphereCrossLine3< dectype >( *pSphere , *( const CBasicLine3 * )pGeometory );
					}
					break;
				case eGeometoryType_ray3:
					{
						isCollision = GeometryCollisionUtil::SphereCrossRay3< dectype >( *pSphere , *( const CBasicRay3 * )pGeometory , pInfo );
					}
					break;
				case eGeometoryType_plane:
					{
						isCollision = GeometryCollisionUtil::SphereCrossPlane< dectype >( *pSphere , *( const CBasicPlane * )pGeometory );
					}
					break;
				case eGeometoryType_triangle3:
					{
						isCollision = GeometryCollisionUtil::SphereCrossTriangle3< dectype >( *pSphere , *( const CBasicTriangle3 * )pGeometory );
					}
					break;
				case eGeometoryType_capsule3:
					{
						isCollision = GeometryCollisionUtil::SphereCrossCapsule3< dectype >( *pSphere , *( const CBasicCapsule3 * )pGeometory );
					}
					break;
				case eGeometoryType_aabb3:
					{
						isCollision = GeometryCollisionUtil::AABB3CrossSphere( *( const CAABB3 * )pGeometory , *pSphere );
					}
					break;
				case eGeometoryType_obb3:
					{
						isCollision = GeometryCollisionUtil::OBB3CrossSphere( *( const COBB3 * )pGeometory , *pSphere , pInfo );
					}
					break;
			}

		POST_COLLISION_PROC( pSphere , eType )
	}

	return result;
}
skyBool CollisionManager::Collision_ellipse( const CBasicEllipse *pEllipse , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList )
{
	skyBool result = skyFalse;

	FOREACH( CollisionDataPtrList , it , m_CollisionDataList )
	{
		PRE_COLLISION_PROC()

			//衝突判定処理
			switch( pGeometory->m_eType )
			{
				case eGeometoryType_point2:
					{
						isCollision = GeometryCollisionUtil::Point2CrossEllipse< dectype >( *( const CBasicVector2 * )pGeometory , *pEllipse );
					}
				case eGeometoryType_circle:
					{
					}
					break;
				case eGeometoryType_ellipse:
					{
						isCollision = GeometryCollisionUtil::EllipseCrossEllipse< dectype >( *pEllipse , *( const CBasicEllipse * )pGeometory );
					}
					break;
				case eGeometoryType_line2:
					{
					}
					break;
				case eGeometoryType_ray2:
					{
					}
					break;
				case eGeometoryType_triangle2:
					{
					}
					break;
				case eGeometoryType_capsule2:
					{
					}
					break;
				case eGeometoryType_rect:
					{
					}
					break;
				case eGeometoryType_aabb2:
					{
					}
					break;
				case eGeometoryType_obb2:
					{
					}
					break;
			}

		POST_COLLISION_PROC( pEllipse , eType )
	}

	return result;
}
skyBool CollisionManager::Collision_plane( const CBasicPlane *pPlane , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList )
{
	skyBool result = skyFalse;

	FOREACH( CollisionDataPtrList , it , m_CollisionDataList )
	{
		PRE_COLLISION_PROC()

			//衝突判定処理
			switch( pGeometory->m_eType )
			{
				case eGeometoryType_point3:
					{
					}
				case eGeometoryType_sphere:
					{
						isCollision = GeometryCollisionUtil::SphereCrossPlane< dectype >( *( const CBasicSphere * )pGeometory , *pPlane );
					}
					break;
				case eGeometoryType_line3:
					{
					}
					break;
				case eGeometoryType_ray3:
					{
						isCollision = GeometryCollisionUtil::Ray3CrossPlane< dectype >( *( const CBasicRay3 * )pGeometory , *pPlane , pInfo );
					}
					break;
				case eGeometoryType_plane:
					{
					}
					break;
				case eGeometoryType_triangle3:
					{
					}
					break;
				case eGeometoryType_capsule3:
					{
					}
					break;
				case eGeometoryType_aabb3:
					{
						isCollision = GeometryCollisionUtil::AABB3CrossPlane( *( const CAABB3 * )pGeometory , *pPlane );
					}
					break;
				case eGeometoryType_obb3:
					{
						isCollision = GeometryCollisionUtil::OBB3CrossPlane(  *( const COBB3 * )pGeometory , *pPlane );
					}
					break;
			}

		POST_COLLISION_PROC( pPlane , eType )
	}

	return result;
}
skyBool CollisionManager::Collision_triangle2( const CBasicTriangle2 *pTriangle2 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList )
{
	skyBool result = skyFalse;

	FOREACH( CollisionDataPtrList , it , m_CollisionDataList )
	{
		PRE_COLLISION_PROC()

			//衝突判定処理
			switch( pGeometory->m_eType )
			{
				case eGeometoryType_point2:
					{
					}
				case eGeometoryType_circle:
					{
					}
					break;
				case eGeometoryType_ellipse:
					{
					}
					break;
				case eGeometoryType_line2:
					{
					}
					break;
				case eGeometoryType_ray2:
					{
					}
					break;
				case eGeometoryType_triangle2:
					{
					}
					break;
				case eGeometoryType_capsule2:
					{
					}
					break;
				case eGeometoryType_rect:
					{
					}
					break;
				case eGeometoryType_aabb2:
					{
					}
					break;
				case eGeometoryType_obb2:
					{
					}
					break;
			}

		POST_COLLISION_PROC( pTriangle2 , eType )
	}

	return result;
}
skyBool CollisionManager::Collision_triangle3( const CBasicTriangle3 *pTriangle3 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList )
{
	skyBool result = skyFalse;

	FOREACH( CollisionDataPtrList , it , m_CollisionDataList )
	{
		PRE_COLLISION_PROC()

			//衝突判定処理
			switch( pGeometory->m_eType )
			{
				case eGeometoryType_point3:
					{
					}
				case eGeometoryType_sphere:
					{
						isCollision = GeometryCollisionUtil::SphereCrossTriangle3< dectype >( *( const CBasicSphere * )pGeometory , *pTriangle3 );
					}
					break;
				case eGeometoryType_line3:
					{
					}
					break;
				case eGeometoryType_ray3:
					{
						isCollision = GeometryCollisionUtil::Ray3CrossTriangle3< dectype >( *( const CBasicRay3 * )pGeometory , *pTriangle3 , pInfo );
					}
					break;
				case eGeometoryType_plane:
					{
					}
					break;
				case eGeometoryType_triangle3:
					{
					}
					break;
				case eGeometoryType_capsule3:
					{
					}
					break;
				case eGeometoryType_aabb3:
					{
						isCollision = GeometryCollisionUtil::AABB3CrossTriangle3( *( const CAABB3 * )pGeometory , *pTriangle3 );
					}
					break;
				case eGeometoryType_obb3:
					{
					}
					break;
			}

		POST_COLLISION_PROC( pTriangle3 , eType )
	}

	return result;
}
skyBool CollisionManager::Collision_capsule2( const CBasicCapsule2 *pCapsule2 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList )
{
	skyBool result = skyFalse;

	FOREACH( CollisionDataPtrList , it , m_CollisionDataList )
	{
		PRE_COLLISION_PROC()

			//衝突判定処理
			switch( pGeometory->m_eType )
			{
				case eGeometoryType_point2:
					{
					}
				case eGeometoryType_circle:
					{
					}
					break;
				case eGeometoryType_ellipse:
					{
					}
					break;
				case eGeometoryType_line2:
					{
					}
					break;
				case eGeometoryType_ray2:
					{
					}
					break;
				case eGeometoryType_triangle2:
					{
					}
					break;
				case eGeometoryType_capsule2:
					{
					}
					break;
				case eGeometoryType_rect:
					{
					}
					break;
				case eGeometoryType_aabb2:
					{
					}
					break;
				case eGeometoryType_obb2:
					{
					}
					break;
			}

		POST_COLLISION_PROC( pCapsule2 , eType )
	}

	return result;
}
skyBool CollisionManager::Collision_capsule3( const CBasicCapsule3 *pCapsule3 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList )
{
	skyBool result = skyFalse;

	FOREACH( CollisionDataPtrList , it , m_CollisionDataList )
	{
		PRE_COLLISION_PROC()

			//衝突判定処理
			switch( pGeometory->m_eType )
			{
				case eGeometoryType_point3:
					{
					}
				case eGeometoryType_sphere:
					{
						isCollision = GeometryCollisionUtil::SphereCrossCapsule3< dectype >( *( const CBasicSphere * )pGeometory , *pCapsule3 );
					}
					break;
				case eGeometoryType_line3:
					{
					}
					break;
				case eGeometoryType_ray3:
					{
					}
					break;
				case eGeometoryType_plane:
					{
					}
					break;
				case eGeometoryType_triangle3:
					{
					}
					break;
				case eGeometoryType_capsule3:
					{
						isCollision = GeometryCollisionUtil::Capsule3CrossCapsule3< dectype >( *pCapsule3 , *( const CBasicCapsule3 * )pGeometory );
					}
					break;
				case eGeometoryType_aabb3:
					{
					}
					break;
				case eGeometoryType_obb3:
					{
					}
					break;
			}

		POST_COLLISION_PROC( pCapsule3 , eType )
	}

	return result;
}
skyBool CollisionManager::Collision_rect( const CBasicRect *pRect , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList )
{
	skyBool result = skyFalse;

	FOREACH( CollisionDataPtrList , it , m_CollisionDataList )
	{
		PRE_COLLISION_PROC()

			//衝突判定処理
			switch( pGeometory->m_eType )
			{
				case eGeometoryType_point2:
					{
						isCollision = GeometryCollisionUtil::Point2CrossRect< dectype >( *( const CBasicVector2 * )pGeometory , *pRect );
					}
				case eGeometoryType_circle:
					{
					}
					break;
				case eGeometoryType_ellipse:
					{
					}
					break;
				case eGeometoryType_line2:
					{
					}
					break;
				case eGeometoryType_ray2:
					{
					}
					break;
				case eGeometoryType_triangle2:
					{
					}
					break;
				case eGeometoryType_capsule2:
					{
					}
					break;
				case eGeometoryType_rect:
					{
						isCollision = GeometryCollisionUtil::RectCrossRect< dectype >( *pRect , *( const CBasicRect * )pGeometory );
					}
					break;
				case eGeometoryType_aabb2:
					{
					}
					break;
				case eGeometoryType_obb2:
					{
					}
					break;
			}

		POST_COLLISION_PROC( pRect , eType )
	}

	return result;
}
skyBool CollisionManager::Collision_aabb2( const CAABB2 *pAABB2 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList )
{
	skyBool result = skyFalse;

	FOREACH( CollisionDataPtrList , it , m_CollisionDataList )
	{
		PRE_COLLISION_PROC()

			//衝突判定処理
			switch( pGeometory->m_eType )
			{
				case eGeometoryType_point2:
					{
						isCollision = GeometryCollisionUtil::AABB2CrossPoint2( *pAABB2 , *( const CBasicVector2 * )pGeometory );
					}
				case eGeometoryType_circle:
					{
					}
					break;
				case eGeometoryType_ellipse:
					{
					}
					break;
				case eGeometoryType_line2:
					{
						isCollision = GeometryCollisionUtil::AABB2CrossLine2( *pAABB2 , *( const CBasicLine2 * )pGeometory , pInfo );
					}
					break;
				case eGeometoryType_ray2:
					{
					}
					break;
				case eGeometoryType_triangle2:
					{
					}
					break;
				case eGeometoryType_capsule2:
					{
					}
					break;
				case eGeometoryType_rect:
					{
					}
					break;
				case eGeometoryType_aabb2:
					{
						isCollision = GeometryCollisionUtil::AABB2CrossAABB2( *pAABB2 , *( const CAABB2 * )pGeometory );
					}
					break;
				case eGeometoryType_obb2:
					{
					}
					break;
			}

		POST_COLLISION_PROC( pAABB2 , eType )
	}

	return result;
}
skyBool CollisionManager::Collision_aabb3( const CAABB3 *pAABB3 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList )
{
	skyBool result = skyFalse;

	FOREACH( CollisionDataPtrList , it , m_CollisionDataList )
	{
		PRE_COLLISION_PROC()

			//衝突判定処理
			switch( pGeometory->m_eType )
			{
				case eGeometoryType_point3:
					{
						isCollision = GeometryCollisionUtil::AABB3CrossPoint3( *pAABB3 , *( const CBasicVector3 * )pGeometory );
					}
				case eGeometoryType_sphere:
					{
						isCollision = GeometryCollisionUtil::AABB3CrossSphere( *pAABB3 , *( const CBasicSphere * )pGeometory );
					}
					break;
				case eGeometoryType_line3:
					{
						isCollision = GeometryCollisionUtil::AABB3CrossLine3( *pAABB3 , *( const CBasicLine3 * )pGeometory , pInfo );
					}
					break;
				case eGeometoryType_ray3:
					{
					}
					break;
				case eGeometoryType_plane:
					{
						isCollision = GeometryCollisionUtil::AABB3CrossPlane( *pAABB3 , *( const CBasicPlane * )pGeometory );
					}
					break;
				case eGeometoryType_triangle3:
					{
						isCollision = GeometryCollisionUtil::AABB3CrossTriangle3( *pAABB3 , *( const CBasicTriangle3 * )pGeometory );
					}
					break;
				case eGeometoryType_capsule3:
					{
					}
					break;
				case eGeometoryType_aabb3:
					{
						isCollision = GeometryCollisionUtil::AABB3CrossAABB3( *pAABB3 , *( const CAABB3 * )pGeometory );
					}
					break;
				case eGeometoryType_obb3:
					{
						isCollision = GeometryCollisionUtil::AABB3CrossOBB3( *pAABB3 , *( const COBB3 * )pGeometory );
					}
					break;
			}

		POST_COLLISION_PROC( pAABB3 , eType )
	}

	return result;
}
skyBool CollisionManager::Collision_obb2( const COBB2 *pOBB2 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList )
{
	skyBool result = skyFalse;

	FOREACH( CollisionDataPtrList , it , m_CollisionDataList )
	{
		PRE_COLLISION_PROC()

			//衝突判定処理
			switch( pGeometory->m_eType )
			{
				case eGeometoryType_point2:
					{
					}
				case eGeometoryType_circle:
					{
					}
					break;
				case eGeometoryType_ellipse:
					{
					}
					break;
				case eGeometoryType_line2:
					{
						isCollision = GeometryCollisionUtil::OBB2CrossLine2( *pOBB2 , *( const CBasicLine2 * )pGeometory );
					}
					break;
				case eGeometoryType_ray2:
					{
					}
					break;
				case eGeometoryType_triangle2:
					{
					}
					break;
				case eGeometoryType_capsule2:
					{
					}
					break;
				case eGeometoryType_rect:
					{
					}
					break;
				case eGeometoryType_aabb2:
					{
					}
					break;
				case eGeometoryType_obb2:
					{
						isCollision = GeometryCollisionUtil::OBB2CrossOBB2( *pOBB2 , *( const COBB2 * )pGeometory );
					}
					break;
			}

		POST_COLLISION_PROC( pOBB2 , eType )
	}

	return result;
}
skyBool CollisionManager::Collision_obb3( const COBB3 *pOBB3 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList )
{
	skyBool result = skyFalse;

	FOREACH( CollisionDataPtrList , it , m_CollisionDataList )
	{
		PRE_COLLISION_PROC()

			//衝突判定処理
			switch( pGeometory->m_eType )
			{
				case eGeometoryType_point3:
					{
					}
				case eGeometoryType_sphere:
					{
						isCollision = GeometryCollisionUtil::OBB3CrossSphere( *pOBB3 , *( const CBasicSphere * )pGeometory , pInfo );
					}
					break;
				case eGeometoryType_line3:
					{
						isCollision = GeometryCollisionUtil::OBB3CrossLine3( *pOBB3 , *( const CBasicLine3 * )pGeometory );
					}
					break;
				case eGeometoryType_ray3:
					{
					}
					break;
				case eGeometoryType_plane:
					{
						isCollision = GeometryCollisionUtil::OBB3CrossPlane( *pOBB3 , *( const CBasicPlane * )pGeometory );
					}
					break;
				case eGeometoryType_triangle3:
					{
					}
					break;
				case eGeometoryType_capsule3:
					{
					}
					break;
				case eGeometoryType_aabb3:
					{
						isCollision = GeometryCollisionUtil::AABB3CrossOBB3( *( const CAABB3 * )pGeometory , *pOBB3 );
					}
					break;
				case eGeometoryType_obb3:
					{
						isCollision = GeometryCollisionUtil::OBB3CrossOBB3( *pOBB3 , *( const COBB3 * )pGeometory );
					}
					break;
			}

		POST_COLLISION_PROC( pOBB3 , eType )
	}

	return result;
}

} } }
