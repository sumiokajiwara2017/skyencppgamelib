#include "StdAfx.h"
#include "SkyMathLib/Collision.h"

namespace sky { namespace lib { namespace math {

CollisionData::CollisionData() : m_pGeometry( skyNull ) , m_TagBit( 0 ) , m_IsDelete( skyFalse ) , m_IsHitResultGet( skyFalse )
{
}

CollisionData::CollisionData( s32 kindId , skyBool isStopSameKindCollision , const CGeometryData* pGeometry , const ICollisionCBDelegateObjectPtr &spCollisionCBDelegate , const IExecDetectionDelegateObjectPtr &spExecDetectionDelegateObject )
{
	Initialize( kindId , isStopSameKindCollision , pGeometry , spCollisionCBDelegate , spExecDetectionDelegateObject );
}

CollisionData::CollisionData( s32 kindId , skyBool isStopSameKindCollision , const IGeometoryGetDelegateObjectPtr &spGeometoryGetDelegate , const ICollisionCBDelegateObjectPtr &spCollisionCBDelegate , const IExecDetectionDelegateObjectPtr &spExecDetectionDelegateObject )
{
	Initialize( kindId , isStopSameKindCollision , spGeometoryGetDelegate , spCollisionCBDelegate , spExecDetectionDelegateObject );
}

void CollisionData::Initialize( s32 kindId , skyBool isStopSameKindCollision , const CGeometryData* pGeometry , const ICollisionCBDelegateObjectPtr &spCollisionCBDelegate , const IExecDetectionDelegateObjectPtr &spExecDetectionDelegateObject )
{
	SKY_ASSERT( pGeometry != skyNull ); 
	SKY_ASSERT( kindId != COLLISION_KIND_ID_ALL );

	m_KindID = kindId;
	m_IsStopSameKindCollision = isStopSameKindCollision;
	m_pGeometry = pGeometry;
	m_spCollisionCBDelegate  = spCollisionCBDelegate;
	m_spExecDetectionDelegate = spExecDetectionDelegateObject;
}

void CollisionData::Initialize( s32 kindId , skyBool isStopSameKindCollision , const IGeometoryGetDelegateObjectPtr &spGeometoryGetDelegate , const ICollisionCBDelegateObjectPtr &spCollisionCBDelegate , const IExecDetectionDelegateObjectPtr &spExecDetectionDelegateObject )
{
	SKY_ASSERT( spGeometoryGetDelegate.IsEmpty() == skyFalse );
	SKY_ASSERT( kindId != COLLISION_KIND_ID_ALL );

	m_KindID = kindId;
	m_IsStopSameKindCollision = isStopSameKindCollision;
	m_pGeometry = skyNull;
	m_spGeometoryGetDelegate = spGeometoryGetDelegate;
	m_spCollisionCBDelegate  = spCollisionCBDelegate;
	m_spExecDetectionDelegate = spExecDetectionDelegateObject;
}

void CollisionData::SetGeometoryGetDelegate( const IGeometoryGetDelegateObjectPtr &spDelegate )
{
	SKY_ASSERT( spDelegate.IsEmpty() == skyFalse ); //必須です。初期化されていないか空のポインタが設定された。

	m_spGeometoryGetDelegate = spDelegate;
}

void CollisionData::SetExecDetectionDelegate( const IExecDetectionDelegateObjectPtr &spDelegate )
{
	m_spExecDetectionDelegate = spDelegate;
}

void CollisionData::SetCollisionCBDelegate( const ICollisionCBDelegateObjectPtr &spDelegate )
{
	SKY_ASSERT( spDelegate.IsEmpty() == skyFalse ); //必須です。初期化されていないか空のポインタが設定された。

	m_spCollisionCBDelegate  = spDelegate;
}

void CollisionData::SetStop( skyBool isStop )
{
	m_IsStop = isStop;
}

void CollisionData::SetStopSameKindCollision( skyBool isStop )
{
	m_IsStopSameKindCollision = isStop;
}

s32 CollisionData::GetKindId() const
{
	return m_KindID;
}

void CollisionData::SetKindId( s32 id )
{
	m_KindID = id;
}

s32  CollisionData::GetExtraId( u32 index ) const
{
	return m_ExtraId[ index ];
}

void CollisionData::SetExtraId( u32 index , s32 id )
{
	m_ExtraId[ index ] = id;
}

const CommonSmartPointer &CollisionData::GetExtraData() const
{
	return m_spExtraData;
}

void  CollisionData::SetExtraData( const CommonSmartPointer &spExtraData )
{
	m_spExtraData = spExtraData;
}

u32  CollisionData::GetTagBit()
{
	return m_TagBit;
}

void CollisionData::TagBitOn( u32 tagBit )
{
	m_TagBit |= tagBit;
}

void CollisionData::TagBitOff( u32 tagBit )
{
	m_TagBit &= ~tagBit;
}

skyBool CollisionData::IsTagBitOn( u32 tagBit )
{
	return ( tagBit & m_TagBit );
}

void CollisionData::ClearTagBit()
{
	m_TagBit = 0;
}

skyBool CollisionData::IsHitResultGet()
{
	return m_IsHitResultGet;
}

void CollisionData::SetHitResultSet( skyBool isHitResultGet )
{
	m_IsHitResultGet = isHitResultGet;
}

const CGeometryData* CollisionData::GetGeometry()
{
	const CGeometryData *pGeometory = m_pGeometry;
	if ( pGeometory == skyNull )
	{
		pGeometory = m_spGeometoryGetDelegate.GetInsNoConst()();
	}
	return pGeometory;
}

} } }
