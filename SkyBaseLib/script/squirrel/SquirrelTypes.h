#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

//#define SW_USE_SQ_STD_LIB

namespace sky { namespace lib { namespace squirrel {

/// <summary>
/// 定数
/// </summary>
namespace sq_const
{
	static const u32  VM_THREAD_STACK_SIZE		= 32;	//最大同時スレッド起動数
	static const u32  VM_THREAD_FUNCNAME_LEN	= 64;	//関数名の最大の長さ
	static const u32  FUNC_PARAM_MAX_NUM		= 32;	//Squirrel関数パラメーター数最大
};

} } }

#endif