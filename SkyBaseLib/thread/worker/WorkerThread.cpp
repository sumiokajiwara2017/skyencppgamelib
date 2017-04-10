#include "StdAfx.h"
#include "SkyBaseLib/Thread.h"

namespace sky { namespace lib { namespace thread {

void CWorkerThread::ThreadMain()
{
	m_spQueue->IncThreadCount();	

	while ( m_IsValid )
	{
		FunctionCallBackAsyncPtr fnPtr;

		//キューから実行要求を取り出す。
		fnPtr = m_spQueue->PopRequest();

		if ( fnPtr == skyNull )
		{
			m_spWaitObject->Wait();        //待ち状態に入る
			continue;
		}

		//実行中のタスク数カウントを増やす
		m_spQueue->IncExecNum();

		//実行
		fnPtr->run();

		//	実行中のタスク数カウントを減らす
		m_spQueue->DecExecNum();
	}

	//	実行中のスレッドカウントを減らす
	m_spQueue->DecThreadCount();
}

void CWorkerThread::SetValid( skyBool isValid )
{
	m_IsValid = isValid;
}

CWorkerThread::CWorkerThread( const CWorkQueuePtr& spQueue , u32 stackSize ) :
CThread( stackSize )
{
	m_spQueue = spQueue;
	m_IsValid = skyTrue;
	m_spWaitObject = CWaitObject::Create();
}

CWorkerThread::~CWorkerThread()
{
}

} } }