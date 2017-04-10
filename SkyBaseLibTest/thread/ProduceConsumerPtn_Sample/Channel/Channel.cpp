#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "../Data/Data.h"
#include "Channel.h"

Channel::Channel( void ) :
m_DataFifo( 1024 ) ,
m_pLock( sky::lib::thread::ILockObject::Create() )
{
}

Channel::~Channel( void )
{
	sky::lib::thread::ILockObject::Delete( m_pLock );
}

Data* Channel::GetData( void )
{
	sky::lib::thread::Synchronized g( m_pLock );

	//�f�[�^�����������̂������ő҂�
	while( m_DataFifo.IsNoData() )
	{
		m_pLock->Wait();
	}

	Data *pResult = skyNull;
	m_DataFifo.Pop( pResult );

	m_pLock->NotifyAll(); //�ʒm����
	
	return pResult;
}

void Channel::PutData( Data* pData )
{
	sky::lib::thread::Synchronized g( m_pLock );

	//�f�[�^���擾�����̂�҂�
	while( m_DataFifo.GetPushNum() > BUFFER_LENGTH )
	{
		m_pLock->Wait();
	}

	m_DataFifo.Push( pData );

	m_pLock->NotifyAll(); //�ʒm����
}
