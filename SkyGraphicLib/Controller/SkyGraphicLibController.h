#pragma once

namespace sky { namespace lib { namespace graphic {


//-------------------------------------< 実装 >---------------------------------------------//

class CGraphicLibController : public base::SkyRefObject
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize			();

	/// <summary>
	/// 末期化
	/// </summary>
	static void Dispose				();

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
	CGraphicLibController(){};															//コンストラクタ
	CGraphicLibController( const CGraphicLibController& ){};							//コピーコンストラクタ
	virtual ~CGraphicLibController(){};													//デストラクタ
	CGraphicLibController &operator=(const CGraphicLibController& ){ return *this; };	//代入演算子オーバーロード

};

#define SkyGraphicLibControllerInitialize_()	sky::lib::graphic::CGraphicLibController::Initialize()
#define SkyGraphicLibController_				sky::lib::graphic::CGraphicLibController
#define SkyGraphicLibControllerDispose_()		sky::lib::graphic::CGraphicLibController::Dispose()

} } }