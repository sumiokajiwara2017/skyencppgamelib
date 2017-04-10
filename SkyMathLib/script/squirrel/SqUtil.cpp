#include "StdAfx.h"
#include "SkyMathLib/Script.h"

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace math { namespace squirrel {

void CSqUtil::RegisterSkyMathLibClass( HSQUIRRELVM v)
{
	sky::lib::squirrel::CSqUtil::RegisterClass( v , GetSqVector2() );
}

} } } }

#endif