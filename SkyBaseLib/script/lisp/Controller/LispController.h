#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_LISP

namespace sky { namespace lib { namespace lisp {

namespace lispcontroller_const
{
	static const u32 INPUT_MAX_CHAR_NUM = 1024;
};

enum eLispUpdateMode
{
	eLispUpdateMode_InputConsole = 0 ,
	eLispUpdateMode_InputFileExec,
};

/// <summary>
/// Lispコントローラー
/// </summary>
class LispController : public base::SkyRefObject
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	LispController			();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~LispController	();

	/// <summary>
	/// Lispの初期化
	/// </summary>
	skyBool vIsInitialize	();

	/// <summary>
	/// Lispの末期化
	/// </summary>
	skyBool vIsDispose		();

	/// <summary>
	/// ファイル読込
	/// </summary>

	/// <summary>
	/// 関数実行
	/// </summary>

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// シングルトン機能付加
	/// <summary>
	friend class Singleton< LispController >;
	static Singleton< LispController > Singleton;

private:

	/// <summary>
	/// 初期化フラグ
	/// </summary>
	skyBool		m_IsInit;

};

} } }

#else

#define LispControllerCreate_()	( ()0)
#define LispController_			( ()0)
#define LispControllerDelete_()	( ()0)

#endif