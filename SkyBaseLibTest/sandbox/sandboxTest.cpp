#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_SANDBOX

#include "SkyBaseLib/Controller.h"

class CHogeA : public sky::lib::base::SkyRefObject
{
public:
	CHogeA(){}
	virtual ~CHogeA(){}
};
typedef sky::lib::SmartPointer< CHogeA >		CHogeAPtr;
typedef sky::lib::CList< s32 , CHogeAPtr >		CHogeAPtrList;	
typedef sky::lib::CHash< CHogeAPtr >			CHogeAPtrHash;	


TEST( sandbox , 1 ) 
{
	CrtLeakCheckInit();
	{

		//’è”‚Ìƒ[ƒh
		sky::lib::ecb::PgConst::Load();

		CHashStringManagerInitialize_();
		CPropertyManagerInitialize_();

		CHogeAPtrHash hash( 9 );

		CHogeAPtr chogePtr( NEW CHogeA() );
		hash.Set( _T( "0" ) , chogePtr );

		CPropertyManagerDispose_();
		CHashStringManagerDispose_();

		//’è”‚Ì”jŠü
		sky::lib::ecb::PgConst::Destroy();

	}
	CrtLeakCheck();
}

#endif