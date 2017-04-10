#pragma once
//----------------------------------------------------------------------
//
// Squirrel機能ヘッダー
//
//----------------------------------------------------------------------
//(注意！ソースの中に書いてあるが、外部includeフォルダからの相対パスにしないとだめ）

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

#include "SquirrelCommon.h"

#include "Vm/SqObjectInterface.h"
#include "Func/SqFuncParam.h"
#include "Class/SqClassObject.h"
#include "Vm/SqVmObject.h"
#include "Vm/SqVmFactory.h"
#include "Util/SqUtil.h"
#include "Controller/SquirrelController.h"
#include "Api/SqDebugApi.h"
#include "Api/SqExtensionApi.h"

#endif