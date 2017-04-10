#include "StdAfx.h"
#include "SkyBaseLib/State.h"

SkyImplementRTTI( sky::lib::state::IState   , sky::lib::base::SkyRefObject );
SkyImplementRTTI( sky::lib::state::AbsState , sky::lib::state::IState );

namespace sky { namespace lib { namespace state {

SingletonPtr< CStateManager >	CStateManager::Singleton;

CStateManagerPtr CStateManager::Create()
{
	return CStateManagerPtr( NEW__( CStateManager , CStateManager() ) );
}

CStateManager::CStateManager() :
	m_OrderInfoPool( ORDER_FIFO_SIZE ) ,
	m_RegisterStateHash( REGISTER_STATE_HASH_SIZE ) ,
	m_ActiveStateStack( ACTIVE_STATE_STACK_SIZE ) ,
	m_OrderFifo( ORDER_FIFO_SIZE )
{
}

void CStateManager::Register( const hash::CHashStringPtr &name , const IStatePtr &spState )
{
	m_RegisterStateHash.Set( name , spState );
	spState->vAttatchManager( CStateManagerPtr( this , skyFalse , this->GetRefCnt() ) );
}

void CStateManager::UnRegister( const hash::CHashStringPtr &name )
{
	m_RegisterStateHash.Get( name )->vDetachManager();
	m_RegisterStateHash.Delete( name );
}

void CStateManager::Push( const hash::CHashStringPtr &name )
{
	PushOrder( eOrder_Push , ( m_RegisterStateHash )[ name ] );
}

void CStateManager::Pop()
{
	SKY_ASSERT_MSG( m_ActiveStateStack.GetNum() > 0 , _T( "It tried to do in pop to no existence of the state. " ) ); //ステートが存在しないのにポップしようとしました。

	PushOrder( eOrder_Pop );
}

void CStateManager::Change( const hash::CHashStringPtr &name )
{
	PushOrder( eOrder_Change , ( m_RegisterStateHash )[ name ] );
}

void CStateManager::Clear()
{
	PushOrder( eOrder_Clear );
}

void CStateManager::Update( const fps::CFpsController &fps )
{
	//命令を実行する
	ExecOrderFifo();

	//ステートを更新する
	FOREACH( IStatePtrStack , it , m_ActiveStateStack )
	{
		( *it )->vUpdate( fps );
	}
}

void CStateManager::PushOrder( eOrder eId , const IStatePtr  &spState )
{
	COrderInfoPtr pOrderInfo( m_OrderInfoPool.Create() );
	pOrderInfo->eId = eId;
	pOrderInfo->spState = spState;
	m_OrderFifo.Push( pOrderInfo );
}

void CStateManager::ExecOrderFifo()
{
	COrderInfoPtr pOrderInfo;
	while( m_OrderFifo.Pop( pOrderInfo ) )
	{
		ExecOrder( pOrderInfo );
		m_OrderInfoPool.Delete( pOrderInfo ); 
	}
}

skyBool CStateManager::IsUpState( const hash::CHashStringPtr &name , const hash::CHashStringPtr &upname )
{
	skyBool result = skyFalse;

	//ステートを更新する
	skyBool isStateFind = skyFalse;
	FOREACH_REVERS( IStatePtrStack , it , m_ActiveStateStack )
	{
		if ( isStateFind == skyTrue )
		{
			if ( ( *it )->GetName() == upname )
			{
				result = skyTrue;
			}
			break;
		}

		if ( ( *it )->GetName() == name )
		{
			isStateFind = skyTrue;
			continue;
		}
	}

	return result;
}

void CStateManager::ExecOrder( COrderInfoPtr pOrderInfo )
{
	switch( pOrderInfo->eId )
	{
		case eOrder_Push:
		{
			//スタックにすでにステートがあった場合、Suspendを呼ぶ
			if ( m_ActiveStateStack.GetNum() > 0 )
			{
				m_ActiveStateStack.GetTop()->vSuspend( pOrderInfo->spState );
			}

			//Enterを呼ぶ
			pOrderInfo->spState->vEnter();
			pOrderInfo->spState->vSetActive( skyTrue );

			//スタックに積む
			m_ActiveStateStack.Push( pOrderInfo->spState );

			break;
		}
		case eOrder_Pop:
		{
			//スタックから抜く
			IStatePtr spState;
			m_ActiveStateStack.Pop( spState );

			//Exitを呼ぶ
			spState->vExit();
			spState->vSetActive( skyFalse );

			//スタックにステートがあった場合はResumeを呼ぶ
			if ( m_ActiveStateStack.GetNum() > 0 )
			{
				m_ActiveStateStack.GetTop()->vResume( pOrderInfo->spState );
			}

			break;
		}
		case eOrder_Change:
		{
			//スタックから全部抜く(eOrder_Clearを呼ぶ）
			COrderInfoPtr pOrderInfo_l( m_OrderInfoPool.Create() );
			pOrderInfo_l->eId = eOrder_Clear;
			ExecOrder( pOrderInfo_l );

			//スタックに一個積む（eOrder_Pushを呼ぶ）
			pOrderInfo_l->eId = eOrder_Push;
			pOrderInfo_l->spState = pOrderInfo->spState;
			ExecOrder( pOrderInfo_l );
			m_OrderInfoPool.Delete( pOrderInfo_l );
			break;
		}
		case eOrder_Clear:
		{
			//スタックから全部抜く
			IStatePtr spState;
			while( m_ActiveStateStack.GetNum() > 0 )
			{
				//eOrder_Popを呼ぶ
				COrderInfoPtr pOrderInfo_l( m_OrderInfoPool.Create() );
				pOrderInfo_l->eId = eOrder_Pop;
				ExecOrder( pOrderInfo_l );
				m_OrderInfoPool.Delete( pOrderInfo_l );
			}

			break;
		}
		default:
		{
			SKY_PANIC_MSG( _T( "It is not possible to come here. " ) ); //ここにくることはありえません。
		}
	}
}

} } }
