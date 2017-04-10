#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_TOKEN

#include "SkyBaseLib/Token.h"

TEST( token , 1 ) 
{
	SKY_BASE_LIB_INIT();

	sky::lib::token::CTokenizerM tokennizerM( 32 , 32 , 32 );

	tokennizerM.SetBuffer( "aaa bbb ccc 111 234 -567" );
	tokennizerM.SetSeparator( " \t\r\n,;\"" );
	tokennizerM.SetCutOff( "{}" );

	do
	{
		tokennizerM.Next();
		SKY_PRINTF( " %s %f %f %d \n" , tokennizerM.GetChar() , tokennizerM.GetDouble() , tokennizerM.GetFloat() , tokennizerM.GetInt() );
	}
	while( tokennizerM.IsEnd() == skyFalse );

	sky::lib::token::CTokenizerW tokennizerW( 32 , 32 , 32 );
	tokennizerW.SetBuffer( L"aaa bbb ccc 111 234 -567" );
	tokennizerW.SetSeparator( L" \t\r\n,;\"" );
	tokennizerW.SetCutOff( L"{}" );

	{
		tokennizerW.Next();
		SKY_PRINTF( L" %s %f %f %d \n" , tokennizerW.GetChar() , tokennizerW.GetDouble() , tokennizerW.GetFloat() , tokennizerW.GetInt() );
	}
	while( tokennizerW.IsEnd() == skyFalse );


	SKY_BASE_LIB_TERM();
}

#endif