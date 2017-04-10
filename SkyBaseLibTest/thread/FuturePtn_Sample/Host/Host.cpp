#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "../Data/Data.h"
#include "../Helper/Helper.h"
#include "Host.h"

Host::Host( void )
{
}

Host::~Host( void )
{
}

void Host::DoWork( const skyWString *pStr , DataSmPtr& data )
{
	IThreadManager_->CreateThread( NEW Helper( pStr , data ) );
}
