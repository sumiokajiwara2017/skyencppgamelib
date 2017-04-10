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
	SKY_ASSERT_MSG( m_ActiveStateStack.GetNum() > 0 , _T( "It tried to do in pop to no existence of the state. " ) ); //�X�e�[�g�����݂��Ȃ��̂Ƀ|�b�v���悤�Ƃ��܂����B

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
	//���߂����s����
	ExecOrderFifo();

	//�X�e�[�g���X�V����
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

	//�X�e�[�g���X�V����
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
			//�X�^�b�N�ɂ��łɃX�e�[�g���������ꍇ�ASuspend���Ă�
			if ( m_ActiveStateStack.GetNum() > 0 )
			{
				m_ActiveStateStack.GetTop()->vSuspend( pOrderInfo->spState );
			}

			//Enter���Ă�
			pOrderInfo->spState->vEnter();
			pOrderInfo->spState->vSetActive( skyTrue );

			//�X�^�b�N�ɐς�
			m_ActiveStateStack.Push( pOrderInfo->spState );

			break;
		}
		case eOrder_Pop:
		{
			//�X�^�b�N���甲��
			IStatePtr spState;
			m_ActiveStateStack.Pop( spState );

			//Exit���Ă�
			spState->vExit();
			spState->vSetActive( skyFalse );

			//�X�^�b�N�ɃX�e�[�g���������ꍇ��Resume���Ă�
			if ( m_ActiveStateStack.GetNum() > 0 )
			{
				m_ActiveStateStack.GetTop()->vResume( pOrderInfo->spState );
			}

			break;
		}
		case eOrder_Change:
		{
			//�X�^�b�N����S������(eOrder_Clear���Ăԁj
			COrderInfoPtr pOrderInfo_l( m_OrderInfoPool.Create() );
			pOrderInfo_l->eId = eOrder_Clear;
			ExecOrder( pOrderInfo_l );

			//�X�^�b�N�Ɉ�ςށieOrder_Push���Ăԁj
			pOrderInfo_l->eId = eOrder_Push;
			pOrderInfo_l->spState = pOrderInfo->spState;
			ExecOrder( pOrderInfo_l );
			m_OrderInfoPool.Delete( pOrderInfo_l );
			break;
		}
		case eOrder_Clear:
		{
			//�X�^�b�N����S������
			IStatePtr spState;
			while( m_ActiveStateStack.GetNum() > 0 )
			{
				//eOrder_Pop���Ă�
				COrderInfoPtr pOrderInfo_l( m_OrderInfoPool.Create() );
				pOrderInfo_l->eId = eOrder_Pop;
				ExecOrder( pOrderInfo_l );
				m_OrderInfoPool.Delete( pOrderInfo_l );
			}

			break;
		}
		default:
		{
			SKY_PANIC_MSG( _T( "It is not possible to come here. " ) ); //�����ɂ��邱�Ƃ͂��肦�܂���B
		}
	}
}

} } }
