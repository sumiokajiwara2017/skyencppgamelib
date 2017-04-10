#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// コリジョン管理
/// コリジョン管理を複数持つことで処理を分散させたほうがいいかもしれないです。
/// 注意）ＡＡＢＢとＯＢＢを両方使用して一つのアプリケーションを作成するのは基本ＮＧです。
/// ＡＡＢＢとＯＢＢで衝突判定が安定して取れないからです（中途半端なものは実装している。確実に抜ける）
/// </summary>
class CollisionManager : base::SkyObject
{

public:

	/// <summary>
	/// コリジョンデータの登録依頼
	/// 実際の登録はUpdateが呼ばれた際に行われる。コリジョン通知のコールバック時にコリジョンデータの
	/// 登録または履きが行われるとリスト操作に支障がでる可能性があるので、間をおいて一斉に登録、削除を処理する
	/// </summary>
	void ResisterDataRequest( const CollisionDataPtr &data );

	/// <summary>
	/// コリジョンデータの登録解除
	/// 実際の登録はUpdateが呼ばれた際に行われる。コリジョン通知のコールバック時にコリジョンデータの
	/// 登録または履きが行われるとリスト操作に支障がでる可能性があるので、間をおいて一斉に登録、削除を処理する
	/// </summary>
	void UnResisterDataRequest( const CollisionDataPtr &data );

	/// <summary>
	/// 判定（登録されいてるデータ同士で衝突判定を実施）
	/// </summary>
	void Detection( s32 kindId = COLLISION_KIND_ID_ALL , u32 tagBit = 0 );

	/// <summary>
	/// 判定（登録されいてるデータ同士で衝突判定を実施）
	/// ヒットしたデータにタグを立てることができる
	/// </summary>
	skyBool Detection( const CollisionDataPtr &data , s32 kindId = COLLISION_KIND_ID_ALL , u32 tagBit = 0 , CollisionDataPtrList *hitList = skyNull );

	/// <summary>
	/// タグクリア
	/// </summary>
	void ClearTagBit();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 指定の種別のデータをリストで返す
	/// </summary>
	const CollisionDataPtrList &GetKindDataList( s32 kindId , CollisionDataPtrList &dest );

private:

	/// <summary>
	/// リクエスト命令種別
	/// </summary>
	enum eRequestKind
	{
		eRequestKind_Resister = 0 ,
		eRequestKind_UnResister ,
	};

	/// <summary>
	/// リクエスト命令情報
	/// </summary>
	struct stRequestData
	{
		eRequestKind     m_eRequestKind;
		CollisionDataPtr m_spCollisionData;
	};
	typedef CList< u32 , stRequestData >	CRequestDataList;

	/// <summary>
	/// 要求リスト
	/// </summary>
	CRequestDataList m_RequestList;

	/// <summary>
	/// コリジョンデータリスト
	/// </summary>
	CollisionDataPtrList m_CollisionDataList;

	/// <summary>
	/// 各種ジオメトリの衝突判定
	/// </summary>
	skyBool Collision_point2( const CBasicVector2 *pPoint2 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_point3( const CBasicVector3 *pPoint3 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_point4( const CBasicVector4 *pPoint4 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_line2( const CBasicLine2 *pLine2 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_line3( const CBasicLine3 *pLine3 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_ray2( const CBasicRay2 *pRay2 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_ray3( const CBasicRay3 *pRay3 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_circle( const CBasicCircle *pCircle , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_sphere( const CBasicSphere *pSphere , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_ellipse( const CBasicEllipse *pEllipse , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_plane( const CBasicPlane *pPlane , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_triangle2( const CBasicTriangle2 *pTriangle2 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_triangle3( const CBasicTriangle3 *pTriangle3 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_capsule2( const CBasicCapsule2 *pCapsule2 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_capsule3( const CBasicCapsule3 *pCapsule3 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_rect( const CBasicRect *pRect , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_aabb2( const CAABB2 *pAABB2 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_aabb3( const CAABB3 *pAABB3 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_obb2( const COBB2 *pOBB2 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
	skyBool Collision_obb3( const COBB3 *pOBB3 , eGeometoryType eType , const CollisionDataPtr &data , s32 kindId , u32 tagBit , CollisionDataPtrList *pHitList = skyNull );
};

} } }
