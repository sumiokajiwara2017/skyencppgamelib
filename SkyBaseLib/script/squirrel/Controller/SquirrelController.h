#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

/// <summary>
/// ライブラリコントローラー
/// </summary>
class SquirrelController : public base::SkyObject
{

public:

	/// <summary>
	/// SkySquirrelLibの初期化
	/// </summary>
	static void Initialize							();

	/// <summary>
	/// SkySquirrelLibの末期化
	/// </summary>
	static void Dispose							();

	/// <summary>
	/// SkySquirrelLibの初期化確認
	/// </summary>
	static skyBool IsInit							();

private:

	/// <summary>
	/// 初期化フラグ
	/// </summary>
	static skyBool		m_IsInit;

};

} } }

#define SquirrelControllerInitialize_() sky::lib::squirrel::SquirrelController::Initialize()
#define SquirrelController_             sky::lib::squirrel::SquirrelController
#define SquirrelControllerDispose_()    sky::lib::squirrel::SquirrelController::Dispose()
#endif