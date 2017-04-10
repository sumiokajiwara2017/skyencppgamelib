#include "StdAfx.h"
#include "SkyBaseLib/Process.h"

namespace sky { namespace lib { namespace process {

SkyImplementRTTI( sky::lib::process::IProcess        , sky::lib::base::SkyRefObject );
SkyImplementRTTI( sky::lib::process::CProcessManager , sky::lib::base::SkyRefObject );

CProcessManagerPtr CProcessManager::Create()
{
	return CProcessManagerPtr( NEW__( CProcessManager , CProcessManager() ) );
}

CProcessManager::CProcessManager() : 
m_OrderFifo( ORDER_FIFO_SIZE ) ,
m_OrderInfoPool( ORDER_FIFO_SIZE )
{

}

CProcessManager::~CProcessManager()
{
}

void CProcessManager::AttachProcessOrder( u32 prio , const IProcessPtr &process )
{
	COrderInfoPtr order = m_OrderInfoPool.Create();
	order->Prio = prio;
	order->eId = eOrder_Attach;
	order->spProcess = process;
	m_OrderFifo.Push( order );

	process->vSetPrio( prio );
	process->vAttachManager( CProcessManagerPtr( this , skyFalse , this->GetRefCnt() ) );
}

void CProcessManager::DetachProcessOrder( const IProcessPtr &process )
{
	process->vDetachManager();
	COrderInfoPtr order = m_OrderInfoPool.Create();
	order->eId = eOrder_Detach;
	order->spProcess = process;
	m_OrderFifo.Push( order );
}

void CProcessManager::DetachProcessOrder( const hash::CHashStringPtr &name )
{
	COrderInfoPtr order = m_OrderInfoPool.Create();
	order->eId = eOrder_Detach_Name;
	order->Name = name;
	m_OrderFifo.Push( order );
}

void CProcessManager::Update( const fps::CFpsController &fps )
{
	//�v���̎��s
	ExecOrderFifo();

	//�O�X�V
	FOREACH( IProcessPtrList , it , m_ProcessList )
	{
		( *it )->vPreUpdate( fps );
		( *it )->vPreUpdateChildProcess( fps );
	}

	//�X�V
	FOREACH( IProcessPtrList , it , m_ProcessList )
	{
		if ( ( *it )->vUpdate( fps ) == IProcess::eResult_End )
		{
			//�֘A�t�������v�����o��
			DetachProcessOrder( ( *it ) );
		}

		//�q�v���Z�X�����s����
		( *it )->vUpdateChildProcess( fps );
	}

	//��X�V
	FOREACH( IProcessPtrList , it , m_ProcessList )
	{
		( *it )->vPostUpdate( fps );
		( *it )->vPostUpdateChildProcess( fps );
	}
}

void CProcessManager::ExecOrderFifo()
{
	//�v�������ׂĎ��s����
	COrderInfoPtr pOrderInfo;
	while( m_OrderFifo.Pop( pOrderInfo ) )
	{
		ExecOrder( pOrderInfo );
		m_OrderInfoPool.Delete( pOrderInfo ); 
	}
}

void CProcessManager::ExecOrder( COrderInfoPtr pOrderInfo )
{
	switch( pOrderInfo->eId )
	{
		case eOrder_Attach:
		{
			//�v���Z�X���X�g�ɒǉ�
			m_ProcessList.AddTail( pOrderInfo->Prio , pOrderInfo->spProcess );

			//vInitialize�����s
			pOrderInfo->spProcess->vInitialize();
			pOrderInfo->spProcess->vInitializeChildProcess();

			break;
		}
		case eOrder_Detach:
		{
			//vDispose�����s
			pOrderInfo->spProcess->vDispose();
			pOrderInfo->spProcess->vDisposeChildProcess();

			//���Ɏ��s����v���Z�X���v���Z�X���X�g�ɒǉ��v�����o���B
			FOREACH( IProcessPtrList , it , pOrderInfo->spProcess->GetNextProcessList() )
			{
				//�D�揇�ʂ͐e�̃v���Z�X��������p�����s
				AttachProcessOrder( pOrderInfo->spProcess->vGetPrio() , *it );
			}

			//�v���Z�X���X�g�������
			m_ProcessList.Delete( pOrderInfo->spProcess );

			break;
		}
		case eOrder_Detach_Name:
		{
			//�w��̖��O�ƈ�v�����v���Z�X���폜���܂��B
			FOREACH_NOINC( IProcessPtrList , it , m_ProcessList )
			{
				if ( pOrderInfo->Name == ( *it )->vGetName() ) 
				{
					( *it )->vDispose();
					( *it )->vDisposeChildProcess();
					it = m_ProcessList.Erases( it );
				}
				else
				{
					it++;
				}
			}
			break;
		}
		default:
		{
			SKY_PANIC_MSG( _T( "It is not possible to come here." ) ); //�����ɂ��邱�Ƃ͂��肦�܂���B
		}
	}
}

} } }