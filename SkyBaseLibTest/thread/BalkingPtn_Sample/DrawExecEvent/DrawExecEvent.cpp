#include "SkyBaseLib/Thread.h"
#include "DrawExecEvent.h"

DrawExecEvent::DrawExecEvent( void ) :
m_IsDrawExecGo( false ) ,
m_IsEnd( false )
{
}

DrawExecEvent::~DrawExecEvent( void )
{
}

void DrawExecEvent::On( void )
{
	m_IsDrawExecGo = true;
}

void DrawExecEvent::Off( void )
{
	m_IsDrawExecGo = false;
}

skyBool DrawExecEvent::Is( void )
{
	return m_IsDrawExecGo;
}

void DrawExecEvent::End( void)
{
	m_IsEnd = skyTrue;
}
skyBool DrawExecEvent::IsEnd( void)
{
	return m_IsEnd;
}

