#include "StdAfx.h"
#include "SkyBaseLib/Thread.h"

namespace sky { namespace lib { namespace thread {

CWorkQueue::~CWorkQueue()
{
	CriticalSection::Delete( m_pCs );
}
	
void CWorkQueue::IncThreadCount()
{
	m_ThreadNum++;
}

void CWorkQueue::DecThreadCount()
{
	m_ThreadNum--;
}

u32 CWorkQueue::GetThreadCount()
{
	return m_ThreadNum;
}

void CWorkQueue::PushRequest( const FunctionCallBackAsyncPtr &spFunc )
{
	thread::CriticalLock lock( m_pCs );

	m_WorkList.AddTail( spFunc );
}

FunctionCallBackAsyncPtr CWorkQueue::PopRequest()
{
	thread::CriticalLock lock( m_pCs );

	FunctionCallBackAsyncPtr spWork;

	if ( m_WorkList.GetNum() > 0 )
	{
		spWork = m_WorkList[ 0 ];
		m_WorkList.Delete( spWork );
	}
	return spWork;
}

void CWorkQueue::CanselRequest( const FunctionCallBackAsyncPtr &spFunc )
{
	thread::CriticalLock lock( m_pCs );

	m_WorkList.Delete( spFunc );
}

void CWorkQueue::CanselAll()
{
	thread::CriticalLock lock( m_pCs );

	m_WorkList.DeleteAll();
}

u32 CWorkQueue::GetRequestNum()
{
	thread::CriticalLock lock( m_pCs );

	return m_WorkList.GetNum();
}

void CWorkQueue::IncExecNum()
{
	m_ExecNum++;
}

void CWorkQueue::DecExecNum()
{
	m_ExecNum--;
}

u32 CWorkQueue::GetExecNum()
{
	return m_ExecNum;
}

CWorkQueue::CWorkQueue()
{
	m_pCs = CriticalSection::Create();
}

} } }