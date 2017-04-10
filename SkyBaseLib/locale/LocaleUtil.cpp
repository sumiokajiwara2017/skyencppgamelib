#include "StdAfx.h"
#include "SkyBaseLib/Locale.h"

namespace sky { namespace lib { namespace locale {

//-------------------------------------< ŽÀ‘• >---------------------------------------------//

#include <locale.h>

const char *CLocaleUtil::s_ccLocalTbl[ Country_Max ] =
{
    "English" ,
    "French" ,
    "Japanese" ,
};

void CLocaleUtil::SetCharacterLocale( Country country )
{
	setlocale( LC_CTYPE , s_ccLocalTbl[ country ] );
}
void CLocaleUtil::SetMoneyLocale( Country country )
{
	setlocale( LC_MONETARY , s_ccLocalTbl[ country ] );
}
void CLocaleUtil::SetNumberLocale( Country country )
{
	setlocale( LC_NUMERIC , s_ccLocalTbl[ country ] );
}
void CLocaleUtil::SetTimeLocale( Country country )
{
	setlocale( LC_TIME , s_ccLocalTbl[ country ] );
}
void CLocaleUtil::SetAllLocale( Country country )
{
	setlocale( LC_ALL , s_ccLocalTbl[ country ] );
}

} } }
