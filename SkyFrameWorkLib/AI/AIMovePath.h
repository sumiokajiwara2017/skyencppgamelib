#pragma once

namespace sky { namespace lib { namespace framework {

/// <summary>
/// パス終了タイプ
/// </summary>
enum eAIPathEndType
{
	eAIPathEndType_End = 0    , //終了
	eAIPathEndType_Repetition , //最初の座標から開始
	eAIPathEndType_Back       , //最後の座標から開始
};

/// <summary>
/// ＡＩが辿るルートパス
/// </summary>
class CAIMovePath : base::SkyObject
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CAIMovePath();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CAIMovePath();

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	CAIMovePath( const CAIMovePath& src );

	/// <summary>
	/// 代入演算子
	/// </summary>
	CAIMovePath& operator =( const CAIMovePath& src );

	/// <summary>
	/// ユーグリッドグラフと最短パスインデックスリストからルートパスを生成する
	/// </summary>
	void AddGraph( const math::CEuclidGraph &graph , math::GraphIndexList &list );

	/// <summary>
	/// パスを追加
	/// </summary>
	void AddPath( const math::CBasicVector3 &pos );

	/// <summary>
	/// カレントの目標地点を取得
	/// </summary>
	math::CBasicVector3 GetCurrentPoint();

	/// <summary>
	/// カレントの目標地点を次の目標地点に設定
	/// </summary>
	void SetNextPoint();

	/// <summary>
	/// 最後の目標地点の場合はskyTrueを返す
	/// </summary>
	skyBool IsFinalPoint();

	/// <summary>
	/// 終了時動作設定／取得
	/// </summary>
	void           SetPathEndType( eAIPathEndType ePathEnd );
	eAIPathEndType GetPathEndType() const;

	/// <summary>
	/// 描画のためのモデルを返す
	/// </summary>
	graphic::CModelPtr CreateModel( const graphic::CColor &color );

private:

	/// <summary>
	/// ステート
	/// </summary>
	enum eState
	{
		eState_Play = 0 ,
		eState_Play_Reverse ,
	} m_eState;

	/// <summary>
	/// 現在のカレント座標番号
	/// </summary>
	u32 m_CurrentPointNo;

	/// <summary>
	/// 現在のカレント座標番号
	/// </summary>
	math::CBasicVector3Vector m_PathVector;

	/// <summary>
	/// パス終了時動作
	/// </summary>
	eAIPathEndType m_ePathEnd;
};

} } }
