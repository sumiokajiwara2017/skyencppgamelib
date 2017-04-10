#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "../CommonResource/CommonResource.h"
#include "ReaderThread.h"

ReaderThread::ReaderThread( sky::lib::thread::ReadWriteLock *pRWLock , CommonResource *pComRes , skyWString *pName ) :
m_pRWLock( pRWLock ) ,
m_pComRes( pComRes ) ,
m_Name( pName )
{
}

ReaderThread::~ReaderThread( void )
{
}

void ReaderThread::ThreadMain( void )
{
	for ( s32 i = 0 ; i < 30 ; i++ )
	{
		sky::lib::WString wStr;
		m_pRWLock->ReadLock();
		{
			SKY_PRINTF( L"%s‚ª“Ç‚Ýž‚Ý’† \n" , m_Name.Get() );
			wStr = m_pComRes->Read();
			SKY_PRINTF( L"%s‚ª“Ç‚Ýž‚ÝŠ®—¹w %s x \n" , m_Name.Get() , wStr.Get() );
		}
		m_pRWLock->ReadUnLock();
	}
}