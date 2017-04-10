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
			SKY_PRINTF( L"%s�������ݒ� \n", m_Name.Get() );
			if ( i & 1 )
			{
				m_pComRes->Write( L"��������ł݂����B" );
			}
			else
			{
				m_pComRes->Write( L"��������ł݂��񂾂��E�E�B " );
			}
		}
		m_pRWLock->WriteUnLock();

		ThreadWin32::GetThread()->Sleep( 3000 );	//�R�b�ɂP�񏑂�����
	}
}