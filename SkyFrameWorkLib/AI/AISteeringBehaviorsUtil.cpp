#include "StdAfx.h"
#include "SkyFrameWorkLib/AI.h"

namespace sky { namespace lib { namespace framework {

/// <summary>
/// 操舵処理群
/// </summary>
namespace CAISteeringBehaviorsUtil
{

skyBool Seek_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj , dectype enableDistance )
{
	skyBool result = skyFalse;

	math::CBasicVector3 force = math::CBasicVector3_ZERO;
	if ( math::DistanceSquared( targetPos , obj.m_Position ) <= enableDistance * enableDistance ) //平方根の計算を省くため平方空間で比較
	{
		math::CBasicVector3 desiredVelocity = ( targetPos - obj.m_Position ).Normalize() * obj.m_MaxSpeed;
		force = ( desiredVelocity - obj.m_Velocity );
		result = skyTrue;
	}

	//更新
	obj.Update( fps , force );

	return result;
}

skyBool Flee_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj , dectype enableDistance )
{
	skyBool result = skyFalse;

	math::CBasicVector3 force = math::CBasicVector3_ZERO;
	if ( math::DistanceSquared( targetPos , obj.m_Position ) <= enableDistance * enableDistance ) //平方根の計算を省くため平方空間で比較
	{
		math::CBasicVector3 desiredVelocity = ( obj.m_Position - targetPos ).Normalize() * obj.m_MaxSpeed;
		force = ( desiredVelocity - obj.m_Velocity );
		result = skyTrue;
	}

	//更新
	obj.Update( fps , force );

	return result;
}

void Arrive_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj , s32 dec )
{
	math::CBasicVector3 force = math::CBasicVector3_ZERO;

	//目標の方向に向くためのベクトルを計算
	math::CBasicVector3 toTargetVec = targetPos - obj.m_Position;

	//目標位置までの距離を計算する
	dectype dist = toTargetVec.Length();

	if ( dist > 0 )
	{
		//Decelerationはint型で列挙定数化されているので、
		//この値は減速に関する粒度の細かい調整を提供するために必要とされる
		const dectype decelerationTweaker = 0.3;

		//指定された望ましい減速で目標に到達するのに必要な速度を計算する
		dectype speed = dist / ( ( dectype )dec * decelerationTweaker );

		//速度が最大速度を超えないようにする
		speed = TemplateUtil::Min< dectype >( speed , obj.m_MaxSpeed );

		//ここからは探索行動のように処理がされる
		//ただし、長さ（dist)をわざわざ計算しているToTargetベクトルを正規化する必要はない
		math::CBasicVector3 desiredVelocity = toTargetVec * speed / dist;

		force = desiredVelocity - obj.m_Velocity;
	}

	//更新
	obj.Update( fps , force );
}

void Pursuit_Move( const fps::CFpsController &fps , const CAIMoveObject &target , CAIMoveObject &obj )
{
	Pursuit_Move( fps , target.m_Position , target.m_Velocity , target.m_MaxSpeed , target.m_Direction , obj );
}

void Pursuit_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , const math::CBasicVector3 &targetVelocity , dectype targetSpeed , const math::CBasicVector3 &direction , CAIMoveObject &obj )
{
	//逃げる側が前に降り、おのエージェントを向いている場合、単にそれに向かって探索行動する
	math::CBasicVector3 toEvader = targetPos - obj.m_Position;

	dectype relativeHeading = obj.m_Direction.Dot( direction );

	if ( ( toEvader.Dot( obj.m_Direction ) > 0 ) &&
		( relativeHeading < -0.95 ) ) //acos( 0.95 )= 18degs
	{
		Seek_Move( fps , targetPos , obj );
	}
	else
	{
		//正面に居ないので逃げる側がどこにいるかを予測する
		//先読み時間は逃げるがわと追いかけるガわの間の距離に比例し、エージェントの速度の合計に反比例する
		dectype lookAheadTime = toEvader.Length() / ( obj.m_MaxSpeed + targetSpeed );

		//回転時間も考慮に入れる
		lookAheadTime += TurnAroundTime( targetPos , obj );

		//ここで予測された逃げるガわの位置に探索行動をする
		Seek_Move( fps , targetPos + targetVelocity * lookAheadTime , obj );
	}
}

void Evade_Move( const fps::CFpsController &fps , const CAIMoveObject &target , CAIMoveObject &obj )
{
	Evade_Move( fps , target.m_Position , target.m_Velocity , target.m_MaxSpeed , target.m_Direction , obj );
}

void Evade_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , const math::CBasicVector3 &targetVelocity , dectype targetSpeed , const math::CBasicVector3 &direction , CAIMoveObject &obj )
{
	math::CBasicVector3 toPursuer = targetPos - obj.m_Position;

	//先読み時間は追う側と逃げるがわの間の距離に比例し、エージェントの速度の合計に反比例する
	dectype lookAheadTime = toPursuer.Length() / ( obj.m_MaxSpeed + targetSpeed );

	//回転時間も考慮に入れる
//	lookAheadTime += TurnAroundTime( targetPos , obj ); //これを解放すると逃げる精度が増す

	//ここで王側に関して予測した将来の位置から逃げる
	Flee_Move( fps , targetPos + targetVelocity * lookAheadTime , obj );
}

void Wander_Move( const fps::CFpsController &fps , CAIMoveObject &obj , const math::CMatrix4x3 &objWorldMatrix , math::eAxis lockAxis )
{
	//小さなラインダムベクトルを目標の位置に加える
	obj.m_WanderTarget += math::CBasicVector3
		( 
		( lockAxis & math::eAxis_x ) ? 0 : RandUtil::RandomClamped() * obj.m_WanderJitter , 
		( lockAxis & math::eAxis_y ) ? 0 : RandUtil::RandomClamped() * obj.m_WanderJitter , 
		( lockAxis & math::eAxis_z ) ? 0 : RandUtil::RandomClamped() * obj.m_WanderJitter );

	//新しいベクトルを単位円の後ろに再投影する
	obj.m_WanderTarget.Normalize();
	
	//ベクトルの長さを徘徊円の半径と同じになるまで増やす
	obj.m_WanderTarget *= obj.m_WanderRadius;

	//目標をエージェントの前の位置に移動する
	math::CBasicVector3 targetLocal = obj.m_WanderTarget + math::CBasicVector3( 0 , 0 , obj.m_WanderDistance );

	//目標をワールド空間へ投影する
	math::CBasicVector3 targetWorld = targetLocal * objWorldMatrix;

	math::CBasicVector3 force = targetWorld - obj.m_Position;

	//更新
	obj.Update( fps , force );
}

void ObstacleAvoidance_Move( const fps::CFpsController &fps , CAIMoveObject &obj , math::CollisionManager &collisionManager , const math::COBB3 &objObb3 )
{
	//速度に合わせてＯＢＢを作成する

	//ＯＢＢと衝突判定を行う

	//ヒットしたデータをobjのローカル空間に移動する

	//ローカル空間に移動したオブジェクトとＺ軸レイの衝突判定を取り一番近いオブジェクトを特定する。さらに交差点も取得しておく

	//一番近いオブジェクトの交差点に応じて操舵力を計算する（Ｘ座標がプラスなら自分は左に、マイナスなら自分は右に、距離が近ければ力を強めに計算する）
}

void WallAvoidance_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj )
{
}

void Interpose_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj )
{
}

static math::CBasicVector3 GetHidingPosition_Sphere( const math::CBasicSphere &obstacle , const math::CBasicVector3 &targetPos )
{
	// エージェントが選択した障害物のバウンディング半径からどれくらい遠くにあるかを計算する
	const dectype distanceFromBoundary = 8.0f;

	dectype distAway = obstacle.GetRadius() + distanceFromBoundary;

	//目標からオブジェクトへの向きを計算する
	math::CBasicVector3 toOb = ( obstacle.GetPos() - targetPos ).Normalize();

	//それを適切な大きさにスケーリングし、障害物の現在の位置に加えて隠れる場所を得る
	return ( toOb * distAway ) + obstacle.GetPos();
}

void Hide_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , const math::CollisionDataPtrList &obstacleList , CAIMoveObject &obj )
{
	dectype distToClosest = FLT_MAX;
	math::CBasicVector3 bestHidingSpot;

	for ( u32 i = 0 ; i < obstacleList.GetNum() ; i++ )
	{
		//幾何形状を取得する
		const math::CGeometryBase *pGeometory = obstacleList[ i ]->GetGeometry();

		//衝突判定処理
		math::CBasicVector3 hideSpot;
		switch( pGeometory->GetGeometoryType() )
		{
			//球の場合の隠れる処理
		case math::eGeometoryType_sphere:

				hideSpot = GetHidingPosition_Sphere( *( const math::CBasicSphere * )pGeometory , targetPos );
				break;
		default:
			continue;
		}

		//平方空間で作業し、エージェントに最も近い隠れ場所を見つける
		dectype dist = math::DistanceSquared( hideSpot , obj.m_Position );

		if ( dist < distToClosest )
		{
			distToClosest = dist;
			bestHidingSpot = hideSpot;
		}
	}

	//適切な障害物が見つからない場合は目標から逃避行動をする
	if ( distToClosest == FLT_MAX )
	{
		Flee_Move( fps , targetPos , obj );
	}
	else
	{
		Arrive_Move( fps , bestHidingSpot , obj );
	}
}

void FollowPath_Move( const fps::CFpsController &fps , CAIMoveObject &obj )
{
	//十分に現在の目標に近づいたら次の目標に設定する（処理負荷軽減のため平方同士の比較）
	if ( math::DistanceSquared( obj.m_Path.GetCurrentPoint() , obj.m_Position ) < obj.m_WaypointSeekDistSq )
	{
		obj.m_Path.SetNextPoint();
	}

	//最後のパスだった場合はそこに到着する
	if ( obj.m_Path.IsFinalPoint() && obj.m_Path.GetPathEndType() == eAIPathEndType_End )
	{
		Arrive_Move( fps , obj.m_Path.GetCurrentPoint() , obj );
	}
	else
	{
		Seek_Move( fps , obj.m_Path.GetCurrentPoint() , obj );
	}
}

void OffsetPursuit_Move( const fps::CFpsController &fps , const math::CBasicVector3 &targetPos , CAIMoveObject &obj )
{
}

dectype TurnAroundTime( const math::CBasicVector3 &targetPos , CAIMoveObject &obj , dectype coefficient )
{
	//目標に対して正規化したベクトルを計算する
	math::CBasicVector3 toTarget = ( targetPos - obj.m_Position ).Normalize();

	//自分の向きとベクトルの内積を取る
	dectype dot = obj.m_Direction.Dot( toTarget );

	return ( dot - 1.0f ) * -coefficient;
}

}

} } }