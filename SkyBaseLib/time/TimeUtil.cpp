#include "StdAfx.h"
#include "SkyBaseLib/Time.h"
#include "SkyBaseLib/Thread.h"

namespace sky { namespace lib { namespace time {

//-------------------------------------< ���� >---------------------------------------------//

#ifdef SKYLIB_PLATFORM_WIN32

TIMECAPS CTimeUtil::m_Caps;

void CTimeUtil::Initialize()
{
#ifdef SKYLIB_IDE_VC10
	::timeGetDevCaps( &m_Caps, sizeof( TIMECAPS ) );	// ���\�擾
    ::timeBeginPeriod( m_Caps.wPeriodMin );				// ���E���\�Őݒ�
#endif
}

updateTime	CTimeUtil::GetNowTime()
{
#ifdef SKYLIB_IDE_VC10
    return ::timeGetTime();
#else
    SKY_ASSERT_MSG( CLOCKS_PER_SEC >= 1000 , _T( "Accuracy is the unit necessary of the milli. \n" ) );//�~���P�ʂ̐��x���K�v
	updateTime spec = CLOCKS_PER_SEC / 1000;
    return ::clock() / spec;
#endif
}

void CTimeUtil::Dispose()
{
#ifdef SKYLIB_IDE_VC10
    ::timeEndPeriod( m_Caps.wPeriodMin );				// �㏈��
#endif
}

void CTimeUtil::Wait( updateTime waitTime )
{
	CThreadUtil_::Sleep( waitTime );
}

#endif

} } }
