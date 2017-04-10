#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_DELEGATE

#include "SkyBaseLib/Delegate.h"

class CHogeManager : public sky::lib::base::SkyRefObject
{
public:
	sky::lib::delegate::CDelegateHandler m_Handl;
};
typedef sky::lib::SmartPointer< CHogeManager >		CHogeManagerPtr;

class CHoge1 : public sky::lib::base::SkyObject
{

public:

	void HogeFunc ( const sky::lib::CommonSmartPointer &spSender , const sky::lib::delegate::CDelegateArgs &args )
	{
		spSender;
		args;
		SKY_PRINTF( _T( "CHoge1‚ªŒÄ‚Î‚ê‚½ \n" ) );
	}
};

class CHoge2 : public sky::lib::base::SkyObject
{

public:

	void HogeFunc ( const sky::lib::CommonSmartPointer &spSender , const sky::lib::delegate::CDelegateArgs &args )
	{
		spSender;
		args;
		SKY_PRINTF( _T( "CHoge2‚ªŒÄ‚Î‚ê‚½ \n" ) );
	}
};

class CHoge3 : public sky::lib::base::SkyObject
{

public:

	void HogeFunc ( const sky::lib::CommonSmartPointer &spSender , const sky::lib::delegate::CDelegateArgs &args )
	{
		spSender;
		args;
		SKY_PRINTF( _T( "CHoge3‚ªŒÄ‚Î‚ê‚½ \n" ) );
	}
};

TEST( delegate , 1 ) 
{
	SKY_BASE_LIB_INIT();

	CHogeManagerPtr	spManager( NEW CHogeManager() );
	CHoge1			hoge1;
	CHoge2			hoge2;
	CHoge3			hoge3;

	//ƒfƒŠƒQ[ƒg‚ð“o˜^
	spManager->m_Handl += sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptor< CHoge1 >( &hoge1, &CHoge1::HogeFunc ) );
	spManager->m_Handl += sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptor< CHoge2 >( &hoge2, &CHoge2::HogeFunc ) );
	spManager->m_Handl += sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptor< CHoge3 >( &hoge3, &CHoge3::HogeFunc ) );

	//ƒfƒŠƒQ[ƒg‚ðŒÄ‚Ô
	spManager->m_Handl();
	spManager->m_Handl( spManager );
	spManager->m_Handl( spManager , sky::lib::delegate::CDelegateArgs( 1, 2, 3 ) );

	//ƒfƒŠƒQ[ƒg‚ð‘S”jŠü
	spManager->m_Handl.RemoveAll();

	SKY_BASE_LIB_TERM();
}

#endif