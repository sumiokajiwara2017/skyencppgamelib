#include "StdAfx.h"
#include "SkyBaseLib/Script.h"

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

SqObjectInterface::SqObjectInterface( HSQUIRRELVM v , HSQOBJECT *sqObj , skyBool isAddRefExec )
{
	m_IsAddRefExec = isAddRefExec;
	m_SqVm = v;
	m_SqObj = sqObj;

	if ( m_IsAddRefExec == skyTrue )
	{
		sq_addref( m_SqVm , m_SqObj );
	}
}

SqObjectInterface::SqObjectInterface( HSQUIRRELVM v )
{
	m_IsAddRefExec = skyFalse;
	m_SqVm = v;
	m_SqObj = skyNull;
}


SqObjectInterface::~SqObjectInterface()
{
	if ( m_IsAddRefExec == skyTrue )
	{
		sq_release( m_SqVm , m_SqObj );
	}
}

SqObjectInterface::operator s32 () const
{
	return sq_objtointeger( m_SqObj );
}

} } }

#endif