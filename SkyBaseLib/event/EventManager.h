#pragma once

namespace sky { namespace lib { namespace event {

//-------------------------------------< 実装 >---------------------------------------------//

class CEventManager;
typedef SmartPointer< CEventManager >	CEventManagerPtr;
typedef CList< u32 , CEventManagerPtr >	CEventManagerPtrList;

/// <summary>
/// イベント管理クラス
/// </summary>
class CEventManager : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CEventManager();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CEventManager();

	//シングルトン機能を付属
	friend class SingletonPtr< CEventManager >;
	static SingletonPtr< CEventManager > SingletonMember;

	/// <summary>
	/// １フレームで実行するイベントの数
	/// </summary>
	void SetOneFrameEventExecNum( u32 eventExecNum );

	/// <summary>
	/// イベントを登録する
	/// </summary>
	void RegisterEvent		( const EventId &eventId );
	
	/// <summary>
	/// イベントを登録を解除する
	/// </summary>
	void UnRegisterEvent		( const EventId &eventId );

	/// <summary>
	/// イベントを全破棄する
	/// </summary>
	void DeleteAllEvent	();

	/// <summary>
	/// イベントを無効にする
	/// </summary>
	void DisableEvent	( const EventId &eventId );

	/// <summary>
	/// イベントを有効にする
	/// </summary>
	void EnableEvent		( const EventId &eventId );

	/// <summary>
	/// イベントリスナーを登録
	/// </summary>
	void AttachEventListener( const EventId &eventId , const delegate::IDelegateObjectPtr &spListener );

	/// <summary>
	/// イベントリスナーを登録解除
	/// </summary>
	void DetachEventListener( const EventId &eventId , const delegate::IDelegateObjectPtr &spListener );

	/// <summary>
	/// イベントを同期で発生させる
	/// </summary>
	void ExecuteEvent	( const EventId &eventId , const CommonSmartPointer &spSender = CommonSmartPointer() , const delegate::CDelegateArgs &args = delegate::CDelegateArgs() );

	/// <summary>
	/// イベントを遅延で発生させる（実行のタイミングはゲームフレームワークでCEventManager::Update()がどこで呼ばれるのかで決まる。ゲームループの最初で呼ぶのが分かりやすいと考える）
	/// </summary>
	void ExecuteEventAsync	( const EventId &eventId , u32 uniqId = DEFAULT_UNIQ_ID , const CommonSmartPointer &spSender = CommonSmartPointer() , const delegate::CDelegateArgs &args = delegate::CDelegateArgs() , u32 startIntvl = 0 , u32 execCount = 1 , u32 execIntvl = 0 );

	/// <summary>
	/// 非同期発生させたイベントを取り消しする
	/// </summary>
	void CanselExecuteEventAsync	( const EventId &eventId , u32 uniqId = DEFAULT_UNIQ_ID );

	/// <summary>
	/// 非同期発生させたイベントを全取り消しする
	/// </summary>
	void AllCanselExecuteEventAsync	();

	/// <summary>
	/// 更新
	/// </summary>
	void Update( const fps::CFpsController &fps );

	/// <summary>
	/// イベント実行ログ出力ストリーム設定
	/// </summary>
	void SetOutLogStream		( stream::IStreamPtr &stream );

	/// <summary>
	/// イベント実行をバイナリ出力するためのストリームを設定
	/// </summary>
	void SetSaveExecBinary	( stream::IStreamPtr &stream );

	/// <summary>
	/// イベント実行ログを読み込みイベントを再生する
	/// </summary>
	void PlayExecBinary  	( stream::IStreamPtr &stream );

//Static

	/// <summary>
	/// インスタンス生成
	/// EventManagerはシステムに1つ存在するが、インスタンスを作成してローカルで使用することもできる
	/// </summary>
    static CEventManagerPtr Create();

private:

	/// <summary>
	/// イベント登録情報
	/// </summary>
	struct CEventData
	{
		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~CEventData(){}

		/// <summary>
		/// イベントID
		/// </summary>
		EventId eventId;

		/// <summary>
		/// 有効無効フラグ
		/// </summary>
		skyBool IsEnable;

		/// <summary>
		/// イベントハンドラ
		/// </summary>
		delegate::CDelegateHandler EventHandler;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		CEventData() : IsEnable( skyTrue ) {}

	};
	typedef CMemoryPool< CEventData >			CEventDataPool;
	typedef CEventData *						CEventDataPtr;
	typedef CHash< CEventDataPtr >				CEventDataPtrHash;

	/// <summary>
	/// イベント再生情報
	/// </summary>
	struct CEventExecuteData
	{
		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~CEventExecuteData(){}

		/// <summary>
		/// イベント送り元
		/// </summary>
		CommonSmartPointer spSender;
			
		/// <summary>
		/// デリゲートパラメーター
		/// </summary>
		delegate::CDelegateArgs Args;

		/// <summary>
		/// イベントID
		/// </summary>
		EventId eventId;

		/// <summary>
		/// ユニークID
		/// </summary>
		u32 UniqId;

		/// <summary>
		/// 有効無効フラグ
		/// </summary>
		skyBool IsEnable;

		/// <summary>
		/// キャンセルフラグ
		/// </summary>
		skyBool IsCancel;

		/// <summary>
		/// 開始待ちフレーム数
		/// </summary>
		u32 StartIntvl;

		/// <summary>
		/// 実行回数( -1 は無限）
		/// </summary>
		s32 ExecCount;

		/// <summary>
		/// 実行間隔フレーム数
		/// </summary>
		u32 ExecIntvl;

		/// <summary>
		/// 実行間隔フレーム数カウンター
		/// </summary>
		u32 ExecIntvlCounter;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		CEventExecuteData() :
			StartIntvl( 0 ) ,
			IsCancel( skyFalse ) ,
			IsEnable( skyTrue ) ,
			ExecCount( 1 ) ,
			ExecIntvl( 0 ) ,
			ExecIntvlCounter( 0 )
		{}

		/// <summary>
		/// 代入演算子オーバーライド
		/// </summary>
		virtual CEventExecuteData& operator =( const CEventExecuteData& src )
		{
			spSender        = src.spSender;
			Args            = src.Args;
			eventId         = src.eventId;
			IsEnable        = src.IsEnable;
			IsCancel        = src.IsCancel;
			StartIntvl      = src.StartIntvl;
			ExecCount       = src.ExecCount;
			ExecIntvl       = src.ExecIntvl;
			return *this;
		}

	};
	typedef CList< u32 , CEventExecuteData >		CEventExecuteDataFifo;

	/// <summary>
	/// イベント実行数
	/// </summary>
	u32								m_EventExecNum;

	/// <summary>
	/// イベント登録情報テーブル
	/// </summary>
	CEventDataPtrHash				*m_pHandlerHash;

	/// <summary>
	/// イベント登録情報プール
	/// </summary>
	CEventDataPool					*m_pEventDataPool;

	/// <summary>
	/// イベント実行情報キュー
	/// </summary>
	CEventExecuteDataFifo			*m_pEventFifo;

	/// <summary>
	/// ログ出力ストリーム
	/// </summary>
	stream::IStreamPtr				*m_spOutLogStream;

	/// <summary>
	/// 再生バイナリ出力ストリーム
	/// </summary>
	stream::IStreamPtr				*m_spSaveExecuteBinaryStream;

};

//インスタンスアクセス文字列
#define CEventManagerCreate_()	sky::lib::event::CEventManager::SingletonMember.CreateInstance()
#define CEventManager_			sky::lib::event::CEventManager::SingletonMember.GetInstance()
#define CEventManagerDelete_()	sky::lib::event::CEventManager::SingletonMember.DeleteInstance()

} } }
