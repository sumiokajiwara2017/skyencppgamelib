#include "SkyBaseLib/Thread.h"
#include "../ThreadCommonRes/ThreadCommonRes.h"
#include "UserThread.h"

UserThread::UserThread( const sky::lib::SmartPointerMT< ThreadCommonRes >& res , s32 threadNo ) :
m_Res( res ) , m_ThreadNo( threadNo )
{
}

void UserThread::ThreadMain( void )
{
	for ( s32 i = 0 ; i < 100 ; i++ )
	{
		m_Res->Test1( m_ThreadNo );
		m_Res->Test2( m_ThreadNo );
	}
}