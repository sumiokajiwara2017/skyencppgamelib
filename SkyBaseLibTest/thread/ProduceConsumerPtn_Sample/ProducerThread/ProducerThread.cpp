#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "../Data/Data.h"
#include "../Channel/Channel.h"
#include "ProducerThread.h"

ProducerThread::ProducerThread( Channel *pChannel , skyWString *pName ) :
m_pChannel( pChannel ),
m_Name( pName )
{
}

ProducerThread::~ProducerThread( void )
{
}

void ProducerThread::ThreadMain( void )
{
	Data *pData = skyNull;
	for ( s32 i = 0 ; i < 10 ; i++ )
	{
		pData = NEW Data();
		SKY_PRINTF( L"%s ‚ð”­‘—‚µ‚½B\n" , m_Name.Get() );
		skyWString buf[ 128 ];
		pData->m_Data = IStringAdptr_->Sprintf( buf , 128 , L"%s" , m_Name.Get() );
		m_pChannel->PutData( pData );

		ThreadWin32::GetThread()->Sleep( 1000 );
	}
}