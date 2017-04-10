#include "StdAfx.h"
#include "SkyBaseLib/Thread.h"

namespace sky { namespace lib { namespace thread {

void CThreadManager::RegisterThread( const CThreadPtr &spThread )
{
	m_RegisterThreadList.AddTail( spThread );
}

void CThreadManager::UnRegisterThread( const CThreadPtr &spThread )
{
	m_RegisterThreadList.Delete( spThread );
}

void CThreadManager::UnRegisterThreadAll()
{
	m_RegisterThreadList.DeleteAll();
}

void CThreadManager::StartAll()
{
	FOREACH( CThreadPtrList , it , m_RegisterThreadList )
	{
		( *it )->Start();
	}
}

void CThreadManager::EndAll()
{
	FOREACH( CThreadPtrList , it , m_RegisterThreadList )
	{
		( *it )->End();
	}
}

skyBool CThreadManager::IsEndAll()
{
	skyBool isEnd = skyTrue;
	FOREACH( CThreadPtrList , it , m_RegisterThreadList )
	{
		isEnd = isEnd && ( *it )->IsEnd();

		if ( isEnd == skyFalse )
		{
			break;
		}
	}
	return isEnd;
}

void CThreadManager::EndWaitAll()
{
	FOREACH( CThreadPtrList , it , m_RegisterThreadList )
	{
		( *it )->EndWait();
	}
}

void CThreadManager::StopAll()
{
	FOREACH( CThreadPtrList , it , m_RegisterThreadList )
	{
		( *it )->Stop();
	}
}

void CThreadManager::ReStartAll()
{
	FOREACH( CThreadPtrList , it , m_RegisterThreadList )
	{
		( *it )->ReStart();
	}
}

} } }