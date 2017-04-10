#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "DrawCommand.h"
#include "DrawCommandManager.h"

sky::lib::Singleton< DrawCommandManager >	DrawCommandManager::Singleton;

DrawCommandManager::DrawCommandManager( void ) :
m_UpdateListIdx( 0 )
{
}

DrawCommandManager::~DrawCommandManager( void )
{
}

void DrawCommandManager::SwitchList( void )
{
	m_UpdateListIdx = ( m_UpdateListIdx == 0 )? 1 : 0 ;
}

DrawCommandPtrList	*DrawCommandManager::GetUpdateList( void )
{
	return &m_CommandList[ m_UpdateListIdx ];
}

DrawCommandPtrList	*DrawCommandManager::GetDrawList( void )
{
	u32 drawListIdx = ( m_UpdateListIdx == 0 )? 1 : 0 ;

	return &m_CommandList[ drawListIdx ];
}
