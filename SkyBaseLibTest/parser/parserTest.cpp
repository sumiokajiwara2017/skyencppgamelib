#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_PARSER

#include "SkyBaseLib/Parser.h"

//#define UNICODE_TEST

TEST( parser , 1 ) 
{
	SKY_BASE_LIB_INIT();

	sky::lib::string::CStringList liststring;

#ifdef UNICODE_TEST
	CParserUtil_::ParseStringPtn0( _T( "aaaa bbbb cccc dddd eeee" ) , liststring );
#else
	CParserUtil_::ParseStringPtn0( "aaaa bbbb cccc dddd eeee" , liststring );
#endif

	FOREACH( sky::lib::string::CStringList , it , liststring )
	{
		SKY_PRINTF( _T( "value:%s \n" ) , ( const skyString * )( *it ) );
	}

	sky::lib::CValues32Map map( 31 );
#ifdef UNICODE_TEST
	CParserUtil_::ParseStringPtn1( _T( "a0303_b0004_cd5555_oo6666_co999m" ) , map );
#else
	CParserUtil_::ParseStringPtn1( "a0303_b0004_cd5555_oo6666_co999m" , map );
#endif

	sky::lib::hash::CHashStringPtrList keyList;
	map.GetkeyList( keyList );

	FOREACH( sky::lib::hash::CHashStringPtrList , it , keyList )
	{
		SKY_PRINTF( _T( "key:%s value:%d \n" ) , (*it)->GetString() , map[ *it ] );
	}

	sky::lib::CValueBoolList list;
#ifdef UNICODE_TEST
	CParserUtil_::ParseStringPtn2( _T( "hoge_bit10101_mog" ) , _T( "bit" ) , list );
#else
	CParserUtil_::ParseStringPtn2( "hoge_bit10101_mog" , "bit" , list );
#endif

	FOREACH( sky::lib::CValueBoolList , it , list )
	{
		SKY_PRINTF( _T( "flag:%d \n" ) , *it );
	}

	sky::lib::CValueu8List listu8;
#ifdef UNICODE_TEST
	CParserUtil_::ParseStringPtn3( _T( "hoge_num123456789_mog" ) , _T( "num" ) , listu8 );
#else
	CParserUtil_::ParseStringPtn3( "hoge_num123456789_mog" , "num" , listu8 );
#endif

	FOREACH( sky::lib::CValueu8List , it , listu8 )
	{
		SKY_PRINTF( _T( "num:%d \n" ) , *it );
	}


	SKY_BASE_LIB_TERM();
}

#endif