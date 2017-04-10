#pragma once

namespace sky { namespace lib { namespace framework {

/// <summary>
/// 操舵処理群
/// 実例で学ぶゲームAIプログラミングを参考に作成
/// </summary>
namespace CAISteeringBehaviorsUtil
{
	/// <summary>
	///【探索行動】
	/// </summary>
	skyBool Seek_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj , dectype enableDistance = FLT_MAX );

	/// <summary>
	///【逃走行動】
	/// </summary>
	skyBool Flee_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj , dectype enableDistance = FLT_MAX );

	/// <summary>
	///【到着行動】
	/// decの値が大きいほど緩やかに目標に到達する
	/// </summary>
	void Arrive_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj , s32 dec = 8 );

	/// <summary>
	///【追跡行動】
	/// 追跡対象の位置、速度、速さ、向きを利用して追跡する
	/// </summary>
	void Pursuit_Move( const fps::CFpsController &fps , const CAIMoveObject &target , CAIMoveObject &obj );
	void Pursuit_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , const math::CBasicVector3 &targetVelocity , dectype targetSpeed , const math::CBasicVector3 &direction , CAIMoveObject &obj );

	/// <summary>
	///【逃避行動】
	/// </summary>
	void Evade_Move( const fps::CFpsController &fps , const CAIMoveObject &target , CAIMoveObject &obj );
	void Evade_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , const math::CBasicVector3 &targetVelocity , dectype targetSpeed , const math::CBasicVector3 &direction , CAIMoveObject &obj );

	/// <summary>
	///【徘徊行動】
	/// </summary>
	void Wander_Move( const fps::CFpsController &fps , CAIMoveObject &obj , const math::CMatrix4x3 &objWorldMatrix , math::eAxis axis = ( math::eAxis )0 );

	/// <summary>
    ///【障害回避行動】
	/// </summary>
	void ObstacleAvoidance_Move( const fps::CFpsController &fps , CAIMoveObject &obj , math::CollisionManager &collisionManager , const math::COBB3 &objObb3 );

	/// <summary>
	///【壁回避行動】
	/// </summary>
	void WallAvoidance_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj );

	/// <summary>
	///【介入行動】
	/// </summary>
	void Interpose_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj );

	/// <summary>
	///【隠身行動】
	/// </summary>
	void Hide_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , const math::CollisionDataPtrList &obstacleList , CAIMoveObject &obj );

	/// <summary>
	///【経路追従行動】
	/// </summary>
	void FollowPath_Move( const fps::CFpsController &fps , CAIMoveObject &obj );

	/// <summary>
	///【オフセット追跡行動】
	/// </summary>
	void OffsetPursuit_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj );

//----便利関数

	/// <summary>
	/// オブジェクトの目標方向への回転時間を計算する
	/// </summary>
	dectype TurnAroundTime( const math::CBasicVector3 &targetPos , CAIMoveObject &obj , dectype coefficient = 0.8f );
}

} } }