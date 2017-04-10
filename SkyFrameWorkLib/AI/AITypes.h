#pragma once

namespace sky { namespace lib { namespace framework {

/// <summary>
/// 移動タイプ（移動タイプ毎に一つ一つ実行されるのではなく、大抵は優先順位をつけたタイプの組み合わせで使用することになる）
/// </summary>
enum eAIMoveType
{
	eAIMoveType_Seek                 = 1 << 0  , //【探索行動】指定のターゲットに近づく
	eAIMoveType_Flee                 = 1 << 1  , //【逃走行動】指定のターゲットから遠ざかる
	eAIMoveType_Arrive               = 1 << 2  , //【到着行動】指定のターゲットに近づき、到達したら止まる
	eAIMoveType_Pursuit              = 1 << 3  , //【追跡行動】指定のターゲットに近づく。Seekとの違いは現在の相手の位置に向かうのではなく相手の速度を見て到達予想地点を想定し、そこに移動する
	eAIMoveType_Evade                = 1 << 4  , //【逃避行動】指定のターゲットから遠ざかるFleeとの違いは相手の速度をみて相手の到達地点から逃げる
	eAIMoveType_Wander               = 1 << 5  , //【徘徊行動】ランダムで効果的に徘徊しているかのように自然に移動する
    eAIMoveType_ObstacleAvoidance    = 1 << 6  , //【障害回避行動】経路上に存在する障害物を避けて移動する
	eAIMoveType_WallAvoidance        = 1 << 7  , //【壁回避行動】平面との衝突を避けて移動する
	eAIMoveType_Interpose            = 1 << 8  , //【介入行動】２つのエージェント、もしくは２つの地点、もしくはエージェントと１つの点の真ん中に移動する
	eAIMoveType_Hide                 = 1 << 9  , //【隠身行動】障害物が自分と相手との間に常に存在するように行動する
	eAIMoveType_FollowPath           = 1 << 10 , //【経路追従行動】指定パス（座標群）に沿って移動する
	eAIMoveType_OffsetPursuit        = 1 << 11   //【オフセット追跡行動】目標から一定距離はなれて追跡する
};

} } }