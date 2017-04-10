#pragma once

namespace sky { namespace lib { namespace state {

//-------------------------------< インターフェース >----------------------------------------//

/// <summary>
/// ステートクラス
/// </summary>
class IState : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// 開始
	/// </summary>
	virtual void vEnter   () = 0;

	/// <summary>
	/// 終了
	/// </summary>
	virtual void vExit    () = 0;

	/// <summary>
	/// 中断(自分より上にStateが乗せられた時に呼ばれる。引数は乗せられたステートの名前）
	/// </summary>
	virtual void vSuspend ( const IStatePtr overState ) = 0;

	/// <summary>
	/// 再開(自分より上のStateが取られた時呼ばれる。引数は乗せられていたステートの名前）
	/// </summary>
	virtual void vResume  ( const IStatePtr &overState ) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void vUpdate    ( const fps::CFpsController &fps ) = 0;

	/// <summary>
	/// 名前取得
	/// </summary>
	virtual const hash::CHashStringPtr &GetName() = 0;

	/// <summary>
	/// 自分が実行中かどうか判定する
	/// </summary>
	virtual skyBool vIsActive() = 0;

	/// <summary>
	/// 管理の関連付け／関連付け解除／取得
	/// </summary>
	virtual void vAttatchManager( const CStateManagerPtr &spManager ) = 0;
	virtual void vDetachManager() = 0;
	virtual const CStateManagerPtr &vGetManager() = 0;

	/// <summary>
	/// 管理が使用（ユーザーは使用しないでください）
	/// </summary>
	virtual void vSetActive( skyBool isActive ) = 0;
};

//-----------------------------------< 抽象実装 >-------------------------------------------//

class AbsState : public IState
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// コンストラクタ（名前必須）
	/// </summary>
	AbsState( const hash::CHashStringPtr &name )
	{
		m_Name = name;
	}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~AbsState(){};

	/// <summary>
	/// 名前の取得
	/// </summary>
	const hash::CHashStringPtr &GetName(){ return m_Name; }

	/// <summary>
	/// 管理の設定取得
	/// </summary>
	void vAttatchManager( const CStateManagerPtr &spManager ) { m_spManager = spManager; }
	void vDetachManager()                                     { m_spManager.Delete(); }
	const CStateManagerPtr &vGetManager()                     { return m_spManager; }

	/// <summary>
	/// 仮想関数の実装要求(未実装も許可するためにここで実装を書く）
	/// </summary>
	virtual void vEnter   (){}
	virtual void vExit    (){}
	virtual void vSuspend ( const lib::state::IStatePtr overState ){ SKY_UNUSED_ARG( overState ); }
	virtual void vResume  ( const lib::state::IStatePtr &overState ){ SKY_UNUSED_ARG( overState ); }
	virtual void vUpdate  ( const fps::CFpsController &fps ){ SKY_UNUSED_ARG( fps ); }
	virtual skyBool vIsActive(){ return m_IsActive; }
	virtual void vSetActive( skyBool isActive ){ m_IsActive = isActive; }

protected:

	/// <summary>
	/// 自分の管理者
	/// </summary>
	CStateManagerPtr        m_spManager;

private:

	/// <summary>
	/// 名前
	/// </summary>
	hash::CHashStringPtr	m_Name;

	/// <summary>
	/// 実行中フラグ
	/// </summary>
	skyBool                 m_IsActive;

};

} } }
