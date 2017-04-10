#pragma once

namespace sky { namespace lib { namespace thread { class CriticalSection; } } }


namespace sky { namespace lib { namespace controller {

//-------------------------------------< 実装 >---------------------------------------------//

/// <summary>
/// ライブラリコントロール
/// </summary>
class CBaseLibController : public base::SkyObject
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize			( u32 playerNum = 1 );

	/// <summary>
	/// 末期化
	/// </summary>
	static void Dispose				();

	/// <summary>
	/// 掃除
	/// </summary>
	static void GC					();

	/// <summary>
	/// 初期化済み判定
	/// <summary>
	static void InitAssert          ();
	static skyBool IsInit           ();

private:

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
	CBaseLibController();															//コンストラクタ
	CBaseLibController( const CBaseLibController& ){};								//コピーコンストラクタ
	virtual ~CBaseLibController();													//デストラクタ
	CBaseLibController &operator=(const CBaseLibController& ){ return *this; };		//代入演算子オーバーロード

};

} } }

//インスタンスアクセス文字列
#define SkyBaseLibControllerInitialize_( playerNo )		sky::lib::controller::CBaseLibController::Initialize( playerNo )
#define SkyBaseLibController_							sky::lib::controller::CBaseLibController
#define SkyBaseLibControllerDispose_()					sky::lib::controller::CBaseLibController::Dispose()
