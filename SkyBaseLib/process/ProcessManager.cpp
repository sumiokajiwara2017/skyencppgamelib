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
	//要求の実行
	ExecOrderFifo();

	//前更新
	FOREACH( IProcessPtrList , it , m_ProcessList )
	{
		( *it )->vPreUpdate( fps );
		( *it )->vPreUpdateChildProcess( fps );
	}

	//更新
	FOREACH( IProcessPtrList , it , m_ProcessList )
	{
		if ( ( *it )->vUpdate( fps ) == IProcess::eResult_End )
		{
			//関連付け解除要求を出す
			DetachProcessOrder( ( *it ) );
		}

		//子プロセスも実行する
		( *it )->vUpdateChildProcess( fps );
	}

	//後更新
	FOREACH( IProcessPtrList , it , m_ProcessList )
	{
		( *it )->vPostUpdate( fps );
		( *it )->vPostUpdateChildProcess( fps );
	}
}

void CProcessManager::ExecOrderFifo()
{
	//要求をすべて実行する
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
			//プロセスリストに追加
			m_ProcessList.AddTail( pOrderInfo->Prio , pOrderInfo->spProcess );

			//vInitializeを実行
			pOrderInfo->spProcess->vInitialize();
			pOrderInfo->spProcess->vInitializeChildProcess();

			break;
		}
		case eOrder_Detach:
		{
			//vDisposeを実行
			pOrderInfo->spProcess->vDispose();
			pOrderInfo->spProcess->vDisposeChildProcess();

			//次に実行するプロセスをプロセスリストに追加要求を出す。
			FOREACH( IProcessPtrList , it , pOrderInfo->spProcess->GetNextProcessList() )
			{
				//優先順位は親のプロセスから引き継ぎ実行
				AttachProcessOrder( pOrderInfo->spProcess->vGetPrio() , *it );
			}

			//プロセスリストから消す
			m_ProcessList.Delete( pOrderInfo->spProcess );

			break;
		}
		case eOrder_Detach_Name:
		{
			//指定の名前と一致したプロセスを削除します。
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
			SKY_PANIC_MSG( _T( "It is not possible to come here." ) ); //ここにくることはありえません。
		}
	}
}

} } }