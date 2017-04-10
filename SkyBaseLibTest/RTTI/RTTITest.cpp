#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_RTTI

#include "SkyBaseLib/RTTI.h"

class HogeBase
{
	SkyTypeDefRootRTTI( HogeBase );
};
SkyImplementRootRTTI( HogeBase );

class Hoge1 : public HogeBase
{
    SkyTypeDefRTTI;
};
SkyImplementRTTI( Hoge1 , HogeBase );

class Hoge2 : public HogeBase
{
    SkyTypeDefRTTI;
};
SkyImplementRTTI( Hoge2 , HogeBase );


TEST( RTTI , 1 ) 
{
	SKY_BASE_LIB_INIT();

	HogeBase *pHogeBase;
	Hoge1     hoge1;
	Hoge2     hoge2;

	pHogeBase = &hoge1;

    if ( SkyIsKindOf( Hoge1 , pHogeBase ) )
	{
		SKY_PRINTF( _T( "正しく型判定しました。\n" ) );
	}

	Hoge1 *pHoge1 = SkyDynamicCast( Hoge1 , pHogeBase );
	if ( pHoge1 != skyNull )
	{
		SKY_PRINTF( _T( "正しく型キャストが成功しました。" ) );
	}

	Hoge2 *pHoge2 = SkyDynamicCast( Hoge2 , pHogeBase );
	if ( pHoge2 == skyNull )
	{
		SKY_PRINTF( _T( "正しく型キャストが失敗しました。" ) );
	}

	SKY_BASE_LIB_TERM();
}

#endif