#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

/// <summary>
/// Squirrelの組み込みデバッグ関数群
/// </summary>
class SqDebugApi : public base::SkyObject
{

public:

	/// <summary>
	/// Ａｐｉ名をストリームに返す
	/// </summary>
	static void PrintApiName( stream::IStreamPtr &spStream );

	/// <summary>
	/// ＡｐｉをＶＭに登録する
	/// </summary>
	static void RegisterApi( HSQUIRRELVM v );
};

//インスタンスアクセス文字列
#define SqDebugApi_			sky::lib::squirrel::SqDebugApi

} } }

#endif