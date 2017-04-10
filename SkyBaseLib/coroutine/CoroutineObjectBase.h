#pragma once

namespace sky { namespace lib { namespace coroutine {

/// <summary>
/// コルーチンオブジェクトベース
/// </summary>
struct CCoroutineObjectBase : public base::SkyRefObject
{
    SkyTypeDefRTTI;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() { m_State = 0; }

	/// <summary>
	/// 現在の位置
	/// </summary>
	s32 m_State;
};

#define COROUTINE_OBJ_BEGIN_( co ) switch( co.m_State ) { case 0:
#define COROUTINE_OBJ_END_   default: break; }
#define YIELD_OBJ_( co , ret ) { co.m_State = __LINE__; return ret; case __LINE__:; }

} } }