#include "StdAfx.h"
#include "SkyBaseLib/Event.h"

#include "SkyBaseLib/Input.h"
#include "SkyBaseLib/Controller.h"

SkyImplementRTTI( sky::lib::event::CEventManager , sky::lib::base::SkyRefObject );

namespace sky { namespace lib { namespace event {

SingletonPtr< CEventManager >	CEventManager::SingletonMember;

CEventManagerPtr CEventManager::Create()
{
	return CEventManagerPtr( NEW__( CEventManager , CEventManager() ) );
}

void CEventManager::SetOneFrameEventExecNum( u32 eventExecNum )
{
	m_EventExecNum = eventExecNum;
}

void CEventManager::RegisterEvent( const EventId &eventId )
{
	SKY_ASSERT_MSG( eventId.IsEmpty() == skyFalse , _T( "Event ID is empty. " ) ); //Event ID is empty.
    SKY_ASSERT_MSG( m_pHandlerHash->IsGet( eventId ) == skyFalse , _T( "This event ID has already been registered. [ %s ]" ) , eventId->GetString() );

	m_pHandlerHash->Set( eventId , m_pEventDataPool->Create() );
}
	
void CEventManager::UnRegisterEvent( const EventId &eventId )
{
	SKY_ASSERT_MSG( eventId.IsEmpty() == skyFalse , _T( "Event ID is empty." ) );
    SKY_ASSERT_MSG( m_pHandlerHash->IsGet( eventId ) == skyTrue , _T( "This event ID has not been registered yet.[ %s ]" ) , eventId->GetString() );

	CEventDataPtr pEventData;
	m_pHandlerHash->Get( eventId , pEventData );
	m_pEventDataPool->Delete( pEventData );
	m_pHandlerHash->Delete( eventId );
}

void CEventManager::DeleteAllEvent()
{
	m_pHandlerHash->DeleteAll();
}

void CEventManager::DisableEvent( const EventId &eventId )
{
	SKY_ASSERT_MSG( eventId.IsEmpty() == skyFalse , _T( "Event ID is empty." ) );
    SKY_ASSERT_MSG( m_pHandlerHash->IsGet( eventId ) == skyTrue , _T( "This event ID has not been registered yet.[ %s ]" ) , eventId->GetString() );

	m_pHandlerHash->Get( eventId )->IsEnable = skyFalse;
}

void CEventManager::EnableEvent( const EventId &eventId )
{
	SKY_ASSERT_MSG( eventId.IsEmpty() == skyFalse , _T( "Event ID is empty." ) );
    SKY_ASSERT_MSG( m_pHandlerHash->IsGet( eventId ) == skyTrue , _T( "This event ID has not been registered yet.[ %s ]" ) , eventId->GetString() );

	m_pHandlerHash->Get( eventId )->IsEnable = skyTrue;
}

void CEventManager::AttachEventListener( const EventId &eventId , const delegate::IDelegateObjectPtr &spListener )
{
	SKY_ASSERT_MSG( eventId.IsEmpty() == skyFalse , _T( "Event ID is empty." ) );
    SKY_ASSERT_MSG( m_pHandlerHash->IsGet( eventId ) == skyTrue , _T( "This event ID has not been registered yet.[ %s ]" ) , eventId->GetString() );

	( *m_pHandlerHash )[ eventId ]->EventHandler += spListener;
}

void CEventManager::DetachEventListener( const EventId &eventId , const delegate::IDelegateObjectPtr &spListener )
{
	SKY_ASSERT_MSG( eventId.IsEmpty() == skyFalse , _T( "Event ID is empty." ) );
    SKY_ASSERT_MSG( m_pHandlerHash->IsGet( eventId ) == skyTrue , _T( "This event ID has not been registered yet.[ %s ]" ) , eventId->GetString() );

	( *m_pHandlerHash )[ eventId ]->EventHandler -= spListener;
}

void CEventManager::ExecuteEvent( const EventId &eventId , const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args )
{
	SKY_ASSERT_MSG( eventId.IsEmpty() == skyFalse , _T( "Event ID is empty." ) );
    SKY_ASSERT_MSG( m_pHandlerHash->IsGet( eventId ) == skyTrue , _T( "This event ID has not been registered yet.[ %s ]" ) , eventId->GetString() );

	( *m_pHandlerHash )[ eventId ]->EventHandler( spSender , args );
}

void CEventManager::ExecuteEventAsync( const EventId &eventId , u32 uniqId , const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args , u32 startIntvl, u32 execCount, u32 execIntvl )
{
	SKY_ASSERT_MSG( eventId.IsEmpty() == skyFalse , _T( "Event ID is empty." ) );
    SKY_ASSERT_MSG( m_pHandlerHash->IsGet( eventId ) == skyTrue , _T( "This event ID has not been registered yet.[ %s ]" ) , eventId->GetString() );

	CEventExecuteData data;
	data.eventId     = eventId;
	data.spSender    = spSender;
	data.Args        = args;
	data.StartIntvl  = startIntvl;
	data.ExecIntvl   = execIntvl;
	data.IsCancel    = skyFalse;
	data.ExecCount   = execCount;
	data.UniqId      = uniqId;
	m_pEventFifo->AddTail( data );
}

void CEventManager::CanselExecuteEventAsync( const EventId &eventId , u32 uniqId )
{
	SKY_ASSERT_MSG( eventId.IsEmpty() == skyFalse , _T( "Event ID is empty." ) );
    SKY_ASSERT_MSG( m_pHandlerHash->IsGet( eventId ) == skyTrue , _T( "This event ID has not been registered yet.[ %s ]" ) , eventId->GetString() );

	//�C�x���gID�ƃ��j�[�NID����v�������̂����ׂăL�����Z���t���O�𗧂Ă�
	FOREACH( CEventExecuteDataFifo , it , *m_pEventFifo )
	{
		if ( ( *it ).eventId == eventId && ( *it ).UniqId == uniqId )
		{
			( *it ).IsCancel = skyTrue;
		}
	}
}

void CEventManager::AllCanselExecuteEventAsync()
{
	//Fifo�̒���S������
	m_pEventFifo->DeleteAll();
}

void CEventManager::Update( const fps::CFpsController &fps )
{
	u32 eventExecNum = 0;

	FOREACH_NOINC( CEventExecuteDataFifo , it , *m_pEventFifo )
	{
		//�w��̐��C�x���g�����s������I���
		if ( eventExecNum >= m_EventExecNum )
		{
			break;
		}

		//�����ȃC�x���g������������Ď���
		if ( ( *m_pHandlerHash )[ ( *it ).eventId ]->IsEnable == skyFalse )
		{
			it = m_pEventFifo->Erases( it );
			continue;
		}

		//�L�����Z��������������Ď���
		if ( ( *it ).IsCancel == skyTrue )
		{
			it = m_pEventFifo->Erases( it );
			continue;
		}

		//�x�����s�J�E���g���O�łȂ���΃J�E���g�_�E�����Ď���
		if ( ( *it ).StartIntvl > 0 )
		{
			( *it ).StartIntvl--;
			it++;
			continue;
		}

		//���s�Ԋu�J�E���^�[���O�łȂ���΃J�E���g�_�E�����Ď���
		if ( ( *it ).ExecIntvlCounter > 0 )
		{
			( *it ).ExecIntvlCounter--;
			it++;
			continue;
		}

		//�C�x���g���s
		( *it ).Args.m_Fps = fps;
		( *m_pHandlerHash )[ ( *it ).eventId ]->EventHandler( ( *it ).spSender , ( *it ).Args );

		//���s�񐔂����炷
		( *it ).ExecCount--;

		//���s�񐔂�0�ɂȂ����烊�X�g�������
		if ( ( *it ).ExecCount <= 0 )
		{
			it = m_pEventFifo->Erases( it );
		}
		else
		{//���̎��s�Ԋu���܂�
			( *it ).ExecIntvlCounter = ( *it ).ExecIntvl;
			it++;
		}

		eventExecNum++;
	}
}

void CEventManager::SetOutLogStream( stream::IStreamPtr &stream )
{
	SKY_ASSERT_MSG( stream.IsEmpty() , _T( "The stream is empty. " ) ); //The stream is empty. 

	*m_spOutLogStream = stream;
}

void CEventManager::SetSaveExecBinary	( stream::IStreamPtr &stream )
{
	SKY_ASSERT_MSG( stream.IsEmpty() , _T( "The stream is empty. " ) );

	*m_spSaveExecuteBinaryStream = stream;
}

void CEventManager::PlayExecBinary  	( stream::IStreamPtr &stream )
{
	SKY_ASSERT_MSG( stream.IsEmpty() , _T( "The stream is empty. " ) );
}

CEventManager::CEventManager()
{
	m_pHandlerHash              = NEW_() CEventDataPtrHash( HAHS_SIZE );
	m_pEventFifo                = NEW_() CEventExecuteDataFifo( FIFO_SIZE );
	m_spOutLogStream            = NEW_() stream::IStreamPtr();
	m_spSaveExecuteBinaryStream = NEW_() stream::IStreamPtr();
	m_pEventDataPool            = NEW_() CEventDataPool( HAHS_SIZE );

	//�V�X�e���C�x���g��o�^����
	RegisterEvent( CHSC( SYSTEM_EVENT_EXIT ) );
	RegisterEvent( CHSC( SYSTEM_EVENT_WINDOW_MOVE ) );
	RegisterEvent( CHSC( SYSTEM_EVENT_WINDOW_RESIZE ) );
}

CEventManager::~CEventManager()
{
	//�V�X�e���C�x���g��o�^��������
	UnRegisterEvent( CHSC( SYSTEM_EVENT_WINDOW_RESIZE ) );
	UnRegisterEvent( CHSC( SYSTEM_EVENT_WINDOW_MOVE ) );
	UnRegisterEvent( CHSC( SYSTEM_EVENT_EXIT ) );

	//�����Ə���
	FOREACH( CEventDataPtrHash , it , *m_pHandlerHash )
	{
		m_pEventDataPool->Delete( *it );
	}
	DEL m_pHandlerHash;
	DEL m_pEventFifo;
	DEL m_spOutLogStream;
	DEL m_spSaveExecuteBinaryStream;
	DEL m_pEventDataPool;
}

} } }

#ifdef SKYLIB_PLATFORM_WIN32

//----------------------- < Window�v���V�[�W�� >---------------------------��
//Windows�͊��荞�݂͑S���������甭�M�I
LRESULT WINAPI SkyWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if ( SkyBaseLibController_::IsInit() == skyTrue )
	{
		switch( msg )
		{
			case WM_DESTROY:
				{
				CEventManager_->ExecuteEvent( CHSC( sky::lib::event::SYSTEM_EVENT_EXIT ) );
				break;
				}
			case WM_LBUTTONDOWN:
				{
				CMouse_->EventExec_LBtnDown( 0 , ( short )GET_X_LPARAM( lParam ), ( short )GET_Y_LPARAM( lParam ) , ( wndType )hWnd );
				break;
				}
			case WM_LBUTTONUP:
				{
				CMouse_->EventExec_LBtnUp( 0 , ( short )GET_X_LPARAM( lParam ), ( short )GET_Y_LPARAM( lParam ) , ( wndType )hWnd );
				break;
				}
			case WM_RBUTTONDOWN:
				{
				CMouse_->EventExec_RBtnDown( 0 , ( short )GET_X_LPARAM( lParam ), ( short )GET_Y_LPARAM( lParam ) , ( wndType )hWnd );
				break;
				}
			case WM_RBUTTONUP:
				{
				CMouse_->EventExec_RBtnUp( 0 , ( short )GET_X_LPARAM( lParam ), ( short )GET_Y_LPARAM( lParam ) , ( wndType )hWnd );
				break;
				}
			case WM_MBUTTONDOWN:
				{
				CMouse_->EventExec_CBtnDown( 0 , ( short )GET_X_LPARAM( lParam ), ( short )GET_Y_LPARAM( lParam ) , ( wndType )hWnd );
				break;
				}
			case WM_MBUTTONUP:
				{
				CMouse_->EventExec_CBtnUp( 0 , ( short )GET_X_LPARAM( lParam ), ( short )GET_Y_LPARAM( lParam ) , ( wndType )hWnd );
				break;
				}
			case WM_MOUSEWHEEL:
				{
                CMouse_->EventExec_HWRotation( 0 , ( short )GET_X_LPARAM( lParam ), ( short )HIWORD( lParam ) , ( short )HIWORD( wParam ) , ( eSkyMouseKeyWheelWithBtnKind )( short )HIWORD( wParam ) , ( wndType )hWnd );
				break;
				}
			case WM_MOUSEMOVE:
				{
				CMouse_->EventExec_Move( 0 , ( short )GET_X_LPARAM( lParam ), ( short )GET_Y_LPARAM( lParam ) , ( wndType )hWnd );
				break;
				}
			case WM_MOVE:
				{
    			sky::lib::delegate::CDelegateArgs args;
				//�w���W�Ƃx���W
				args.Sets32( LOWORD( lParam ) , HIWORD( lParam ) , ( s32 )hWnd );
				CEventManager_->ExecuteEvent( CHSC( sky::lib::event::SYSTEM_EVENT_WINDOW_MOVE ) , sky::lib::CommonSmartPointer() , args );
				break;
				}
			case WM_SIZE:
				{
//				SKY_PRINTF( _T( "Event:WM_SIZE \n" ) );
    			sky::lib::delegate::CDelegateArgs args;
				//���ƍ���
				args.Sets32( LOWORD( lParam ) , HIWORD( lParam ) , ( s32 )hWnd );
				CEventManager_->ExecuteEvent( CHSC( sky::lib::event::SYSTEM_EVENT_WINDOW_RESIZE ) , sky::lib::CommonSmartPointer() , args );
				break;
				}

		}
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

//----------------------- < Window�v���V�[�W�� >---------------------------��

#endif
