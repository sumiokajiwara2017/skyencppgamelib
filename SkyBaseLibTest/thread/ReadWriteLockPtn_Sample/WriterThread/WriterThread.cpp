#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "../CommonResource/CommonResource.h"
#include "WriterThread.h"

WriterThread::WriterThread( sky::lib::thread::ReadWriteLock *pRWLock , CommonResource *pComRes , skyWString *pName ) :
m_pRWLock( pRWLock ) ,
m_pComRes( pComRes ) ,
m_Name( pName )
{
}

WriterThread::~WriterThread( void )
{
}

void WriterThread::ThreadMain( void )
{
	for ( s32 i = 0 ; i < 5 ; i++ )
	{
		sky::lib::WString;
		m_pRWLock->WriteLock();
		{
			SKY_PRINTF( L"%s書き込み中 \n", m_Name.Get() );
			if ( i & 1 )
			{
				m_pComRes->Write( L"書き込んでみたぞ。" );
			}
			else
			{
				m_pComRes->Write( L"書き込んでみたんだが・・。 " );
			}
		}
		m_pRWLock->WriteUnLock();

		ThreadWin32::GetThread()->Sleep( 3000 );	//３秒に１回書き込み
	}
}