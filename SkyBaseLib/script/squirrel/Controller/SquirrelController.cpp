#include "StdAfx.h"
#include "SkyBaseLib/Script.h"

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

skyBool		SquirrelController::m_IsInit = skyFalse;


void SquirrelController::Initialize()
{
	m_IsInit = skyTrue;
	SqVmFactoryCreate_();
}

void SquirrelController::Dispose()
{
	SqVmFactoryDelete_();
	m_IsInit = skyFalse;
}

skyBool SquirrelController::IsInit()
{
	return m_IsInit;
}

} } }

#endif