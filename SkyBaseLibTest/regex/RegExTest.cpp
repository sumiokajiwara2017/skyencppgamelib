#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_REGEX

#include "SkyBaseLib/Controller.h"
#include "SkyBaseLib/RegEx.h"


TEST( regex , 1 ) 
{
	SKY_BASE_LIB_INIT();

	//���K�\�����s
	u32 result = IRegEx_->Execute( _T( "[a-z]" ) , _T( "a" ) ); 
	result;

	//���K�\�����s
	sky::lib::regex::RegResultList resultList;
	IRegEx_->Execute( _T( "l" ) , _T( "Polytechnic University of Turin" ) , resultList );
	FOREACH( sky::lib::regex::RegResultList , it , resultList )
	{
		SKY_PRINTF( _T( "regex result=[ %s ]\n" ) , ( *it ).Get() );
	}

	SKY_BASE_LIB_TERM();
}

#endif