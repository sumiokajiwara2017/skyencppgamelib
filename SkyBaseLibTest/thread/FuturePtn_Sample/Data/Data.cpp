#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "Data.h"

Data::Data( void ) :
m_IsDataSet( skyFalse ) ,
m_pLock( sky::lib::thread::ILockObject::Create() )
{
}

Data::~Data( void )
{
	sky::lib::thread::ILockObject::Delete( m_pLock );
}

void Data::SetData( const skyWString *pStr )
{
	sky::lib::thread::Synchronized sync( m_pLock );
	if ( m_IsDataSet == skyTrue )
	{
		return;
	}

	m_Data = pStr;

	m_IsDataSet = skyTrue;

	m_pLock->NotifyAll(); //待たせたな！
}

const skyWString *Data::GetData( void )
{
	sky::lib::thread::Synchronized sync( m_pLock );

	while( m_IsDataSet == skyFalse )
	{
		m_pLock->Wait();
		//データが用意されていないので待つ
	}

	return m_Data.Get();
}
