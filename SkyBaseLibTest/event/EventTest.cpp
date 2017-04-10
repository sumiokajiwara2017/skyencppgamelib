#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_EVENT

#include "SkyBaseLib/Event.h"
#include "SkyBaseLib/GameLoop.h"
#include "SkyBaseLib/Input.h"
#include "SkyBaseLib/Fps.h"

static const u32 FRAME_RATE = 60;

class CHogeEvent1 : public sky::lib::base::SkyObject
{

public:

	void HogeFunc ( const sky::lib::CommonSmartPointer &spSender , const sky::lib::delegate::CDelegateArgs &args )
	{
		spSender;
		args;
		SKY_PRINTF( _T( "CHoge1���Ă΂ꂽ \n" ) );
	}
};

class CHogeEvent2 : public sky::lib::base::SkyObject
{

public:

	void HogeFunc ( const sky::lib::CommonSmartPointer &spSender , const sky::lib::delegate::CDelegateArgs &args )
	{
		spSender;
		args;
		SKY_PRINTF( _T( "CHoge2���Ă΂ꂽ \n" ) );
	}
};

class CHogeEvent3 : public sky::lib::base::SkyObject
{

public:

	void HogeFunc ( const sky::lib::CommonSmartPointer &spSender , const sky::lib::delegate::CDelegateArgs &args )
	{
		spSender;
		args;
		SKY_PRINTF( _T( "CHoge3���Ă΂ꂽ \n" ) );
	}
};

static const skyString *EVENT1 = _T( "EventA" );
static const skyString *EVENT2 = _T( "EventB" );
static const skyString *EVENT3 = _T( "EventC" );

TEST( Event , 1 ) 
{
	SKY_BASE_LIB_INIT();



	//�C�x���g��o�^����
	CEventManager_->RegisterEvent( CHSC( EVENT1 ) );
	CEventManager_->RegisterEvent( CHSC( EVENT2 ) );
	CEventManager_->RegisterEvent( CHSC( EVENT3 ) );

	CHogeEvent1			hoge1;
	CHogeEvent2			hoge2;
	CHogeEvent3			hoge3;

	//�C�x���g���X�i�[��o�^����
	CEventManager_->AttachEventListener( CHSC( EVENT1 ) , sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptor< CHogeEvent1 >( &hoge1, &CHogeEvent1::HogeFunc ) ) );
	CEventManager_->AttachEventListener( CHSC( EVENT1 ) , sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptor< CHogeEvent2 >( &hoge2, &CHogeEvent2::HogeFunc ) ) );
	CEventManager_->AttachEventListener( CHSC( EVENT1 ) , sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptor< CHogeEvent3 >( &hoge3, &CHogeEvent3::HogeFunc ) ) );

	CEventManager_->AttachEventListener( CHSC( EVENT2 ) , sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptor< CHogeEvent1 >( &hoge1, &CHogeEvent1::HogeFunc ) ) );
	CEventManager_->AttachEventListener( CHSC( EVENT2 ) , sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptor< CHogeEvent2 >( &hoge2, &CHogeEvent2::HogeFunc ) ) );

	CEventManager_->AttachEventListener( CHSC( EVENT3 ) , sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptor< CHogeEvent1 >( &hoge1, &CHogeEvent1::HogeFunc ) ) );

	//FPS�R���g���[���[�쐬
	sky::lib::fps::CFpsController fpsCtrl;

	//�t���[�����[�g�̐ݒ�
	fpsCtrl.SetFrameRate( FRAME_RATE );

	SKY_PRINTF( _T( "�G�X�P�[�v�L�[���������Ă��������B \n" ) );

	//�Q�[�����[�v
	GAME_LOOP_START
	{
		//�t���[���J�n
		fpsCtrl.StartFrame();

		//���͊Ǘ��X�V
		Ckeyboard_->Update( fpsCtrl.GetPassageTime() );

		//���͎�t
		if ( Ckeyboard_->IsKeyPress( KEY_A ) )
		{
			SKY_PRINTF( _T( "�C�x���g�P�������s \n" ) );
			CEventManager_->ExecuteEvent( CHSC( EVENT1 ) );
		} 
		else if ( Ckeyboard_->IsKeyPress( KEY_B ) )
		{
			SKY_PRINTF( _T( "�C�x���g�Q�������s \n" ) );
			CEventManager_->ExecuteEvent( CHSC( EVENT2 ) );
		}
		else if ( Ckeyboard_->IsKeyPress( KEY_C ) )
		{
			SKY_PRINTF( _T( "�C�x���g�R�������s \n" ) );
			CEventManager_->ExecuteEvent( CHSC( EVENT3 ) );
		}
		else if ( Ckeyboard_->IsKeyPress( KEY_D ) )
		{
			SKY_PRINTF( _T( "�C�x���g�P���U�O�t���[����Ɏ��s \n" ) );
			CEventManager_->ExecuteEventAsync( CHSC( EVENT1 ) , sky::lib::event::DEFAULT_UNIQ_ID , sky::lib::CommonSmartPointer() , sky::lib::delegate::CDelegateArgs() , 60 );
		}
		else if ( Ckeyboard_->IsKeyPress( KEY_R ) )
		{
			SKY_PRINTF( _T( "�C�x���g�Q���P�Q�O�t���[����Ɏ��s \n" ) );
			CEventManager_->ExecuteEventAsync( CHSC( EVENT2 ) , sky::lib::event::DEFAULT_UNIQ_ID , sky::lib::CommonSmartPointer() , sky::lib::delegate::CDelegateArgs() , 120 );
		}
		else if ( Ckeyboard_->IsKeyPress( KEY_M ) )
		{
			SKY_PRINTF( _T( "�C�x���g�R���P�W�O�t���[����Ɏ��s \n" ) );
			CEventManager_->ExecuteEventAsync( CHSC( EVENT3 ) , sky::lib::event::DEFAULT_UNIQ_ID , sky::lib::CommonSmartPointer() , sky::lib::delegate::CDelegateArgs() , 180 );
		}
		else if ( Ckeyboard_->IsKeyPress( KEY_S ) )
		{
			SKY_PRINTF( _T( "�C�x���g�Q���P�Q�O�t���[����ɂR�O�t���[���Ԋu�łT����s \n" ) );
			CEventManager_->ExecuteEventAsync( CHSC( EVENT2 ) , sky::lib::event::DEFAULT_UNIQ_ID , sky::lib::CommonSmartPointer() , sky::lib::delegate::CDelegateArgs() , 120 , 5 , 30 );
		}
		else if ( Ckeyboard_->IsKeyPress( KEY_T ) )
		{
			SKY_PRINTF( _T( "�C�x���g�R���P�W�O�t���[����ɂP�Q�O�t���[���Ԋu�łR����s \n" ) );
			CEventManager_->ExecuteEventAsync( CHSC( EVENT3 ) , sky::lib::event::DEFAULT_UNIQ_ID , sky::lib::CommonSmartPointer() , sky::lib::delegate::CDelegateArgs() , 180 , 3 , 120 );
		}

		//�I������
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) )
		{
			GAME_LOOP_EXIT();
		}

		//�X�V
		CEventManager_->Update();

		//�t���[�����[�g�ێ��̂��߁A���Ԃ��]������҂�
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	SKY_BASE_LIB_TERM();
}

#endif