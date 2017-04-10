#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// 数学ライブラリ制御クラス
/// </summary>
class CMathLibController : public base::SkyRefObject
{

public:

	/// <summary>
	/// 初期化
	/// <summary>
	static void Initialize();

	/// <summary>
	/// 末期化
	/// <summary>
	static void Dispose();

	/// <summary>
	/// 初期化済み判定
	/// <summary>
	static void InitAssert();

private:

	//各機能封印
	CMathLibController(){};													//コンストラクタ
	CMathLibController( const CMathLibController& ){};								//コピーコンストラクタ
	virtual ~CMathLibController(){};											//デストラクタ
	CMathLibController &operator=(const CMathLibController& ){ return *this; };		//代入演算子オーバーロード

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
};

} } }

//シングルトンインスタンスアクセス文字列
#define SkyMathLibControllerInitialize_()	sky::lib::math::CMathLibController::Initialize()
#define SkyMathLibController_				sky::lib::math::CMathLibController
#define SkyMathLibControllerDispose_()		sky::lib::math::CMathLibController::Dispose()
