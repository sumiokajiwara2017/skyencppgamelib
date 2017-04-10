#pragma once

namespace sky { namespace lib { namespace framework {


//-------------------------------------< 実装 >---------------------------------------------//

class CFrameWorkLibController : public base::SkyRefObject
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize			();
	static void InitializePlatform	();	//プラットフォーム独自

	/// <summary>
	/// 末期化
	/// </summary>
	static void Dispose				();
	static void DisposePlatform		();

	/// <summary>
	/// 初期化済み判定
	/// <summary>
	static void InitAssert			();

	/// <summary>
	/// 更新
	/// <summary>
	static void Update				( updateTime time );

	/// <summary>
	/// フルスクリーンか？
	/// <summary>
	static skyBool	IsFullScreen	()			{ return m_bIsFullScreen; };
	static void	SetFullScreen	( skyBool isFull )	{ m_bIsFullScreen = isFull; };

private:

	/// <summary>
	/// フルスクリーンフラグ
	/// </summary>
	static skyBool		m_bIsFullScreen;

	/// <summary>
	/// クリティカルセクション取得
	/// </summary>
	static thread::CriticalSection *GetCriticalSection();

	/// <summary>
	/// クリティカルセクション
	/// </summary>
	static thread::CriticalSection *m_pCs;

	/// <summary>
	/// 初期化フラグ
	/// </summary>
	static skyBool m_bIsInitialized;

	//各機能封印
	CFrameWorkLibController(){};															//コンストラクタ
	CFrameWorkLibController( const CFrameWorkLibController& ){};							//コピーコンストラクタ
	virtual ~CFrameWorkLibController(){};													//デストラクタ
	CFrameWorkLibController &operator=(const CFrameWorkLibController& ){ return *this; };	//代入演算子オーバーロード

};

#define SkyFrameWorkLibControllerInitialize_()	sky::lib::framework::CFrameWorkLibController::Initialize()
#define SkyFrameWorkLibController_				sky::lib::framework::CFrameWorkLibController
#define SkyFrameWorkLibControllerDispose_()		sky::lib::framework::CFrameWorkLibController::Dispose()

} } }