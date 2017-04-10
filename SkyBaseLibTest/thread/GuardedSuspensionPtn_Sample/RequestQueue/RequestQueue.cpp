#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "../RequestData/RequestData.h"
#include "RequestQueue.h"

RequestQueue::RequestQueue( void ) :
m_spRequestFifo( 1024 ) ,
m_pLock( sky::lib::thread::ILockObject::Create() )
{
}

RequestQueue::~RequestQueue( void )
{
	sky::lib::thread::ILockObject::Delete( m_pLock );
}

void RequestQueue::PutRequest( RequestData *pRequest )
{
	sky::lib::thread::Synchronized s( m_pLock ); //����

	SKY_PRINTF( L"�f�[�^�𓊓����܂��B \n" );
	m_spRequestFifo.Push( pRequest );

	m_pLock->NotifyAll();
}

RequestData *RequestQueue::GetRequest( void )
{
	sky::lib::thread::Synchronized s( m_pLock );	//����

	//�f�[�^�����݂��Ȃ��ꍇ�͂���܂ő҂�
	while( m_spRequestFifo.IsNoData() )
	{
		SKY_PRINTF( L"�f�[�^�҂��ɓ���܂��B \n" );
		m_pLock->Wait();
	}
	SKY_PRINTF( L"�f�[�^�������̂Ŏ��o���܂��B \n" );
	
	RequestData *pResult;
	m_spRequestFifo.Pop( pResult );

	return pResult;
}
