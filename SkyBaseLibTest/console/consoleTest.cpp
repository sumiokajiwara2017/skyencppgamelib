#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_CONSOLE

#include "SkyBaseLib/Console.h"
#include "SkyBaseLib/Locale.h"

TEST( console , 1 ) 
{
	SKY_BASE_LIB_INIT();

	sky::lib::consol::CConsolStream console;
	console.vOpen();

	console.vWriteStream( _T( "http://www.google.co.jp/search?sourceid=chrome&ie=UTF-8&q=CONOUT+Unicode\n" ) );
	console.vWriteStream( _T( "100 \n" ) );

	skyString st[ 64 ];
	console.vReadStream( st , 64 );

	console.vWriteStream( _T( "“ü—ÍŒ‹‰Ê=%s" ) , st );


	console.vClose();

	SKY_BASE_LIB_TERM();
}

#endif