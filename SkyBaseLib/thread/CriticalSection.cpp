#include "StdAfx.h"
#include "SkyBaseLib/CriticalSection.h"

namespace sky { namespace lib { namespace thread {

CriticalSection *CriticalSection::Create()
{
	return NEW_NATIVE CriticalSection();
}

void CriticalSection::Delete( CriticalSection *pCs )
{
	DEL_NATIVE( CriticalSection , pCs );
}

} } }