#pragma once

namespace sky { namespace lib { namespace thread {

#ifdef SKYLIB_PLATFORM_WIN32

/// <summary>
/// スレッドID
/// </summary>
typedef unsigned tread_id;

/// <summary>
/// スレッドハンドル
/// </summary>
typedef HANDLE tread_hndl;

#endif

/// <summary>
///	Event待ちメソッドの戻り値の種類
/// </summary>
enum eWaitResultKind
{
	eWaitResultKind_Signaled = 0 ,				//オブジェクトがシグナル状態になった
	eWaitResultKind_TimeOut ,					//待ち時間タイムアウトになった
	eWaitResultKind_Error						//それ以外（ありえないはず）
};

/// <summary>
/// ワーカーデフォルト数
/// </summary>
static const u32 DEFAULT_WORKER_NUM = 2;

/// <summary>
/// ワーカーデフォルトスタックサイズ
/// </summary>
static const u32 DEFAULT_STACK_SIZE = 1024 * 2;

} } }