#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "../RequestData/RequestData.h"
#include "../RequestQueue/RequestQueue.h"
#include "ServerThread.h"

ServerThread::ServerThread( RequestQueuePtr& queue ) : m_spRq( queue )
{
}

ServerThread::~ServerThread( void )
{
}

void ServerThread::ThreadMain( void )
{
	RequestData* pData;
	for ( s32 i = 0 ; i < 10 ; i++ )
	{
		pData = m_spRq->GetRequest();

		SKY_PRINTF( L"%s" , pData->m_Data.Get() );

		DEL pData;
	}
}
