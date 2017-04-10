#pragma once

namespace sky { namespace lib { namespace state {

class IState;
typedef SmartPointer< IState >		IStatePtr;
typedef CStack< IStatePtr >			IStatePtrStack;
typedef CHash< IStatePtr >			IStatePtrHash;

//定数
static const u32 REGISTER_STATE_HASH_SIZE	= 256;		//ステート登録用ハッシュサイズ
static const u32 ACTIVE_STATE_STACK_SIZE	= 256;		//実行中のステート格納スタックサイズ
static const u32 ORDER_FIFO_SIZE			= 256;		//命令格納キューサイズ

class CStateManager;
typedef SmartPointer< CStateManager >	CStateManagerPtr;
typedef CList< u32 , CStateManagerPtr >	CStateManagerPtrList;

} } }