#include "StdAfx.h"
#include "SkyBaseLib/Script.h"

#ifdef SW_SKYLIB_SCRIPT_USE_LISP

namespace sky { namespace lib { namespace lisp {

LispController::LispController() :
m_IsInit( skyFalse )
{
}

LispController::~LispController	()
{
	if ( m_IsInit == skyFalse )
	{

		m_IsInit = skyTrue;
	}
}

skyBool LispController::vIsInitialize()
{
	if ( m_IsInit == skyTrue )
	{

		m_IsInit = skyFalse;
	}
	return skyTrue;
}

skyBool LispController::vIsDispose	()
{
	return skyTrue;
}

void LispController::Update()
{
	skyString buffer[ lispcontroller_const::INPUT_MAX_CHAR_NUM ];

	_gettchar();
}

} } }

#endif