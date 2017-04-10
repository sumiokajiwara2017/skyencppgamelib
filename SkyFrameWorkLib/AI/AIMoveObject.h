#pragma once

namespace sky { namespace lib { namespace framework {

static const dectype AI_MOVE_OBJECT_DEFAULT_MASS = 0.1f;
static const dectype AI_MOVE_OBJECT_DEFAULT_MAXSPEED = 8.0f;
static const math::CBasicVector3 AI_MOVE_OBJECT_DEFAULT_POS( 0 , 0 , 0 );
static const math::CBasicVector3 AI_MOVE_OBJECT_DEFAULT_DIR( 0 , 0 , 1 );
static const math::CBasicVector3 AI_MOVE_OBJECT_DEFAULT_VEL( 0 , 0 , 0 );
static const dectype AI_MOVE_OBJECT_DEFAULT_WANDER_RADIUS = 3.0f;
static const dectype AI_MOVE_OBJECT_DEFAULT_WANDER_DISTANCE = 1.0f;
static const dectype AI_MOVE_OBJECT_DEFAULT_WANDER_JITTER = 5.0f;
static const dectype AI_MOVE_OBJECT_DEFAULT_WAYPOINT_SEEK_DIST = 0.5 * 0.5f; //平方
	
/// <summary>
/// ＡＩ動作クラス
/// 物理オブジェクトと非常に似ているが、移動における処理が違うため分けて作成している。
/// </summary>
class CAIMoveObject : base::SkyObject
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CAIMoveObject(){}

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CAIMoveObject() : 
		m_Mass( AI_MOVE_OBJECT_DEFAULT_MASS ) , 
		m_MaxSpeed( AI_MOVE_OBJECT_DEFAULT_MAXSPEED ) ,
		m_Position( AI_MOVE_OBJECT_DEFAULT_POS ) ,
		m_Direction( AI_MOVE_OBJECT_DEFAULT_DIR ) ,
		m_Velocity( AI_MOVE_OBJECT_DEFAULT_VEL ) ,
		m_WanderTarget( AI_MOVE_OBJECT_DEFAULT_POS ) ,
		m_WanderRadius( AI_MOVE_OBJECT_DEFAULT_WANDER_RADIUS ) ,
		m_WanderDistance( AI_MOVE_OBJECT_DEFAULT_WANDER_DISTANCE ) ,
		m_WanderJitter( AI_MOVE_OBJECT_DEFAULT_WANDER_JITTER ) ,
	    m_WaypointSeekDistSq( AI_MOVE_OBJECT_DEFAULT_WAYPOINT_SEEK_DIST )
	{}

	/// <summary>
	/// 現在の速度（移動距離＋移動方向）方向を取り出し、その方向に回転するための回転マトリックスを取得する
	/// </summary>
	math::CQuaternion GetDirectionQuaternion();

	/// <summary>
	/// 力と時間を加えて速度と位置を更新する
	/// </summary>
	void Update( const fps::CFpsController &fps , const math::CBasicVector3 &force );

	/// <summary>
	/// 質量（この値が大きいと全てのＡＩ動作で動きが緩慢になります。あまりに動きがバタバタしすぎた場合はこれを上げるとよい）
	/// </summary>
	dectype m_Mass;

	/// <summary>
	/// 最大速度（速度から方向を抜いたスカラ値で表現）
	/// </summary>
	dectype m_MaxSpeed;

	/// <summary>
	/// 位置（現在の位置）
	/// </summary>
	math::CBasicVector3 m_Position;

	/// <summary>
	/// 向き（現在の向き）
	/// </summary>
	math::CBasicVector3 m_Direction;

	/// <summary>
	/// 速度（進む距離と方向をベクトルで表現）
	/// </summary>
	math::CBasicVector3 m_Velocity;

//----徘徊行動に使用

	/// <summary>
	/// 徘徊円（この値が大きいと動きが複雑になります）
	/// </summary>
	dectype m_WanderRadius;

	/// <summary>
	/// 徘徊円の中心までの距離（この値が大きいと動きが早くなります。ただし最大スピードを越えることはないです）
	/// </summary>
	dectype m_WanderDistance;

	/// <summary>
	/// ランダム変位の最大量で、目標に毎秒で加算される（この値が大きいと動きが複雑になります）
	/// </summary>
	dectype m_WanderJitter;

	/// <summary>
	/// 徘徊円の中心点（初期値はm_Positionと同じ）
	/// </summary>
	math::CBasicVector3 m_WanderTarget;

//----経路追従行動に使用

	/// <summary>
	/// 目標地点にどれだけ近づいたら次の目標地点に行くかの距離（平方）。この値が小さければ小さいほどパスに正確に進む
	/// </summary>
	dectype m_WaypointSeekDistSq;

	/// <summary>
	/// 経路情報
	/// </summary>
	CAIMovePath m_Path; //カレントのインデックス等保持しているため各インスタンス毎にもっていたほうがいい
};

} } }