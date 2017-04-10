#include "StdAfx.h"
#include "SkyBaseLib/Thread.h"

namespace sky { namespace lib { namespace thread {

SingletonPtr< CWorkerThreadManager >	CWorkerThreadManager::Singleton;

CWorkerThreadManager::~CWorkerThreadManager()
{
}
CWorkerThreadManager::CWorkerThreadManager() : m_spQueue( NEW CWorkQueue() )
{
}

void CWorkerThreadManager::StartWorker( u32 workerNum , u32 stackSize )
{
	for( u32 i = 0 ; i < workerNum ; i++ )
	{
		CWorkerThreadPtr spWorker( NEW CWorkerThread( m_spQueue , stackSize ) );
		spWorker->Start();
		m_WorkerList.AddTail( spWorker );
	}
}

void CWorkerThreadManager::WakeUpWorker()
{
	FOREACH( CWorkerThreadPtrList , it , m_WorkerList )
	{
		( *it )->m_spWaitObject->SetSignal();
	}
}

void CWorkerThreadManager::EndWorkerRequest()
{
	FOREACH( CWorkerThreadPtrList , it , m_WorkerList )
	{
		( *it )->SetValid( skyFalse );
	}

	//起こして終了に向かってもらう。
	WakeUpWorker();
}

skyBool CWorkerThreadManager::IsEndWoker()
{
	skyBool isEnd = skyTrue;
	FOREACH( CWorkerThreadPtrList , it , m_WorkerList )
	{
		isEnd = isEnd && ( *it )->IsEnd();

		if ( isEnd == skyFalse )
		{
			break;
		}
	}
	return isEnd;
}

void CWorkerThreadManager::WaitEndWorker()
{
	skyBool isWait = skyTrue;
	while( isWait )
	{
		if ( IsEndWoker() == skyTrue )
		{
			break;
		}
		CThreadUtil_::Sleep( 1 );
	}
}

skyBool CWorkerThreadManager::IsEndWork()
{
	return ( m_spQueue->GetRequestNum() == 0 && m_spQueue->GetExecNum() );
}

void CWorkerThreadManager::WaitEndWork()
{
	skyBool isWait = skyTrue;
	while( isWait )
	{
		if ( IsEndWork() == skyTrue )
		{
			break;
		}
		CThreadUtil_::Sleep( 1 );
	}
}

void CWorkerThreadManager::RequestWork( const FunctionCallBackAsyncPtr &spWork )
{
	m_spQueue->PushRequest( spWork );

	//起こして処理をしてもらう。
	WakeUpWorker();
}

void CWorkerThreadManager::CancelWork( const FunctionCallBackAsyncPtr &spWork )
{
	m_spQueue->CanselRequest( spWork );
}

void CWorkerThreadManager::CancelWorkAll()
{
	m_spQueue->CanselAll();
}

} } }