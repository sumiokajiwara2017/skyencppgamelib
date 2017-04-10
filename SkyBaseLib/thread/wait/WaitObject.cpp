#include "StdAfx.h"
#include "SkyBaseLib/Thread.h"

namespace sky { namespace lib { namespace thread {

CWaitObject::~CWaitObject()
{
	CriticalSection::Delete( m_pCs );
	if ( m_Handle != NULL )
	{
		::CloseHandle( m_Handle );
	}
}

CWaitObject::CWaitObject()
{
	m_Handle = ::CreateEvent( NULL , FALSE , FALSE , NULL );
	m_IsSignal = skyFalse;
	m_pCs = CriticalSection::Create();
}

void CWaitObject::SetSignal()
{
	thread::CriticalLock lock( m_pCs );

	::SetEvent( m_Handle );
	m_IsSignal = skyTrue;
}

void CWaitObject::SetUnSignal()
{
	thread::CriticalLock lock( m_pCs );

	::ResetEvent( m_Handle );
	m_IsSignal = skyFalse;
}

eWaitResultKind	CWaitObject::Wait( s32 time )
{
	u32 wakeUpResult;

	SetUnSignal();

	if ( time == -1 )
	{
		wakeUpResult = ::WaitForSingleObject( m_Handle , INFINITE );
	} 
	else
	{
		wakeUpResult = ::WaitForSingleObject( m_Handle , time );
	}

	eWaitResultKind result;
	switch ( wakeUpResult )
	{
		case WAIT_OBJECT_0	:	//	所有権が獲得できた
		case WAIT_ABANDONED :	//	解放されたイベント
			result = eWaitResultKind_Signaled;
			break;

		case WAIT_TIMEOUT	:	//	タイムアウトになった
			result = eWaitResultKind_TimeOut;
			break;

		default:
			SKY_PANIC_MSG( _T( "WaitForSingleObject error. EventWin32 state error." ) );
			result = eWaitResultKind_Error;
			break;
	}

	return result;
}

skyBool	CWaitObject::IsSignal() const
{
	thread::CriticalLock lock( m_pCs );

	return m_IsSignal;
}

CWaitObjectPtr CWaitObject::Create()
{
	return CWaitObjectPtr( NEW CWaitObject() );
}

} } }