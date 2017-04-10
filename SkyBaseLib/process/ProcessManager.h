#pragma once

namespace sky { namespace lib { namespace process {

/// <summary>
/// 命令キューサイズ
/// </summary>
static const u32 ORDER_FIFO_SIZE = 128;

/// <summary>
/// プロセス管理クラス
/// プロセスは、各機能毎に持つ。Eventのように全体で一つだと、
/// 大雑把にアプリを作れるが、複雑な問題がでることが多い。
/// なのでこのクラスはstaticクラスではない。
/// </summary>
class CProcessManager : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CProcessManager();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CProcessManager();

	/// <summary>
	/// プロセスの関連付け
	/// </summary>
	void AttachProcessOrder( u32 prio , const IProcessPtr &process );

	/// <summary>
	/// プロセスの関連付け解除
	/// 子プロセスも当然登録解除される
	/// 指定のプロセスの次に実行されたプロセスも切ることができる
	/// </summary>
	void DetachProcessOrder( const IProcessPtr &process );
	void DetachProcessOrder( const hash::CHashStringPtr &name );

	/// <summary>
	/// 更新（更新をやめれば全プロセスは停止します）
	/// </summary>
	void Update( const fps::CFpsController &fps );

	/// <summary>
	/// 作成
	/// </summary>
	static CProcessManagerPtr Create();

private:

	/// <summary>
	/// 命令種別
	/// </summary>
	enum eOrder
	{
		eOrder_Attach = 0 ,
		eOrder_Detach ,
		eOrder_Detach_Name ,
	};

	/// <summary>
	/// 命令情報
	/// </summary>
	struct COrderInfo : public base::SkyRefObject
	{
		eOrder					eId;					//命令
		u32						Prio;					//優先順位
		IProcessPtr				spProcess;				//プロセス
		hash::CHashStringPtr	Name;
	};
	typedef CMemoryPool< COrderInfo >   COrderInfoPool;
	typedef COrderInfo *				COrderInfoPtr;
	typedef CFifo< COrderInfoPtr >	    COrderInfoPtrFifo;

	/// <summary>
	/// プロセスリスト
	/// </summary>
	IProcessPtrList		m_ProcessList;

	/// <summary>
	/// 命令キュー
	/// </summary>
	COrderInfoPtrFifo	m_OrderFifo;

	/// <summary>
	/// 命令プール
	/// </summary>
	COrderInfoPool		m_OrderInfoPool;

	/// <summary>
	/// 命令キューを実行する
	/// </summary>
	void ExecOrderFifo();

	/// <summary>
	/// 命令を実行する
	/// </summary>
	void ExecOrder( COrderInfoPtr pOrderInfo );

};

} } }