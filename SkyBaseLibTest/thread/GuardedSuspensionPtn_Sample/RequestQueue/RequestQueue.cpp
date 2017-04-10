#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "../RequestData/RequestData.h"
#include "RequestQueue.h"

RequestQueue::RequestQueue( void ) :
m_spRequestFifo( 1024 ) ,
m_pLock( sky::lib::thread::ILockObject::Create() )
{
}

RequestQueue::~RequestQueue( void )
{
	sky::lib::thread::ILockObject::Delete( m_pLock );
}

void RequestQueue::PutRequest( RequestData *pRequest )
{
	sky::lib::thread::Synchronized s( m_pLock ); //同期

	SKY_PRINTF( L"データを投入します。 \n" );
	m_spRequestFifo.Push( pRequest );

	m_pLock->NotifyAll();
}

RequestData *RequestQueue::GetRequest( void )
{
	sky::lib::thread::Synchronized s( m_pLock );	//同期

	//データが存在しない場合はくるまで待つ
	while( m_spRequestFifo.IsNoData() )
	{
		SKY_PRINTF( L"データ待ちに入ります。 \n" );
		m_pLock->Wait();
	}
	SKY_PRINTF( L"データが来たので取り出します。 \n" );
	
	RequestData *pResult;
	m_spRequestFifo.Pop( pResult );

	return pResult;
}
