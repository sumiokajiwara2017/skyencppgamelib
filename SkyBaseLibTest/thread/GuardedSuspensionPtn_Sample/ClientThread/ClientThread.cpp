#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "../RequestData/RequestData.h"
#include "../RequestQueue/RequestQueue.h"
#include "ClientThread.h"

ClientThread::ClientThread( RequestQueuePtr& queue ) : m_spRq( queue )
{
}

ClientThread::~ClientThread( void )
{
}

void ClientThread::ThreadMain( void )
{
	RequestData *pReq;
	skyWString buf[ 128 ];
	for ( s32 i = 0 ; i < 10 ; i++ )
	{
		pReq = NEW RequestData();

		IStringAdptr_->Sprintf( buf , 128 , L"%d ”Ô–Ú‚Ìƒf[ƒ^‚Å‚· \n" , i );

		pReq->m_Data = buf;

		m_spRq->PutRequest( pReq );

		ThreadWin32::GetThread()->Sleep( 2000 );
	}
}
