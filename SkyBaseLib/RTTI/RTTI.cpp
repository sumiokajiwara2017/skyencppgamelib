#include "StdAfx.h"
#include "SkyBaseLib/RTTI.h"

#include "SkyBaseLib/String.h"

namespace sky { namespace lib {

CRTTI::CRTTI( const skyMString* pName, const CRTTI* pBaseRTTI ) :
    m_pName( pName ), m_pBaseRTTI( pBaseRTTI )
{
}

skyBool CRTTI::CopyName( skyMString* pNameBuffer, u32 maxSize) const
{
    const skyMString* pName = GetName();
    if ( !pName || !pNameBuffer )
    {
		CStringUtil_::Copy( pNameBuffer , maxSize , "\0" );
        return skyFalse;
    }

    CStringUtil_::Copy( pNameBuffer , maxSize , pName );
    return skyTrue;
}

} }
