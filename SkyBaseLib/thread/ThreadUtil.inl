namespace sky { namespace lib { namespace thread {

inline tread_id CThreadUtil::GetCurrentThreadId()
{
	return ( tread_id )::GetCurrentThreadId();
}

inline void CThreadUtil::Sleep( updateTime ms )
{
	return ::Sleep( ( DWORD )ms );
}

inline void CThreadUtil::End()
{
	::ExitThread( TRUE );
}

} } }