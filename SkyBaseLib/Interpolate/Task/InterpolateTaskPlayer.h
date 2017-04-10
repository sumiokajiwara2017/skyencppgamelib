#pragma once

namespace sky { namespace lib { namespace interp {

/// <summary>
/// 補間タスク再生機
/// </summary>
class CInterpolateTaskPlayer : public base::SkyRefObject
{

friend class CInterpolateTaskBase;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CInterpolateTaskPlayer(){};

	/// <summary>
	/// 更新
	/// </summary>
	void Update( updateTime time );

	/// <summary>
	/// 停止設定（Updateのtimeを0にすれば同様の効果が得られる）
	/// </summary>
	void SetStop( skyBool isStop );

	/// <summary>
	/// タスク登録
	/// </summary>
	void RegisterTask( const CInterpolateTaskBasePtr &pTask );

	/// <summary>
	/// タスク登録解除
	/// </summary>
	void UnRegisterTask( const CInterpolateTaskBasePtr &pTask );

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CInterpolateTaskPlayer();

private:

	/// <summary>
	/// 停止フラグ
	/// </summary>
	skyBool m_IsStop;

	/// <summary>
	/// タスクリスト
	/// </summary>
	CInterpolateTaskBasePtrList m_TaskList;
};

} } }
