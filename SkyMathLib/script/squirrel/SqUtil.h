#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace math { namespace squirrel {

/// <summary>
/// squirrel共通処理群
/// </summary>
class CSqUtil
{

public:

	/// <summary>
	/// SkyMathLibのバインドクラスをVMに登録する
	/// </summary>
	static void RegisterSkyMathLibClass( HSQUIRRELVM v);
};

} } } }

//バインドクラス定義情報取得
extern sky::lib::squirrel::SqClassObject GetSqVector2();

#endif