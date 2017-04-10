#include "StdAfx.h"
#include "SkyBaseLib/Interpolate.h"

namespace sky { namespace lib { namespace interp {

void CInterpolateTaskPlayer::Update( updateTime time )
{
	if ( m_IsStop == skyFalse )
	{
		FOREACH( CInterpolateTaskBasePtrList , it , m_TaskList )
		{
			( *it )->Update( time );
		}
	}
}

void CInterpolateTaskPlayer::SetStop( skyBool isStop )
{
	m_IsStop = isStop;
}

void CInterpolateTaskPlayer::RegisterTask( const CInterpolateTaskBasePtr &pTask )
{
	m_TaskList.AddTail( pTask );
}

void CInterpolateTaskPlayer::UnRegisterTask( const CInterpolateTaskBasePtr &pTask )
{
	m_TaskList.Delete( pTask );
}

CInterpolateTaskPlayer::CInterpolateTaskPlayer() : m_IsStop( skyFalse )
{

}

} } }