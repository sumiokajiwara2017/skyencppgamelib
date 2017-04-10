#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "../Data/Data.h"
#include "../Channel/Channel.h"
#include "ConsumerThread.h"

ConsumerThread::ConsumerThread( Channel *pChannel ) :
m_pChannel( pChannel )
{
}

ConsumerThread::~ConsumerThread( void )
{
}

void ConsumerThread::ThreadMain( void )
{
	Data *pData = skyNull;
	for ( s32 i = 0 ; i < 20 ; i++ )
	{
		pData = m_pChannel->GetData();

		SKY_PRINTF( L"%s‚ª“Í‚«‚Ü‚µ‚½B\n" , pData->m_Data.Get() );

		DEL pData;
	}
}