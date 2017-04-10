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
		SKY_PRINTF( _T( "CHoge1が呼ばれた \n" ) );
	}
};

class CHogeEvent2 : public sky::lib::base::SkyObject
{

public:

	void HogeFunc ( const sky::lib::CommonSmartPointer &spSender , const sky::lib::delegate::CDelegateArgs &args )
	{
		spSender;
		args;
		SKY_PRINTF( _T( "CHoge2が呼ばれた \n" ) );
	}
};

class CHogeEvent3 : public sky::lib::base::SkyObject
{

public:

	void HogeFunc ( const sky::lib::CommonSmartPointer &spSender , const sky::lib::delegate::CDelegateArgs &args )
	{
		spSender;
		args;
		SKY_PRINTF( _T( "CHoge3が呼ばれた \n" ) );
	}
};

static const skyString *EVENT1 = _T( "EventA" );
static const skyString *EVENT2 = _T( "EventB" );
static const skyString *EVENT3 = _T( "EventC" );

TEST( Event , 1 ) 
{
	SKY_BASE_LIB_INIT();



	//イベントを登録する
	CEventManager_->RegisterEvent( CHSC( EVENT1 ) );
	CEventManager_->RegisterEvent( CHSC( EVENT2 ) );
	CEventManager_->RegisterEvent( CHSC( EVENT3 ) );

	CHogeEvent1			hoge1;
	CHogeEvent2			hoge2;
	CHogeEvent3			hoge3;

	//イベントリスナーを登録する
	CEventManager_->AttachEventListener( CHSC( EVENT1 ) , sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptor< CHogeEvent1 >( &hoge1, &CHogeEvent1::HogeFunc ) ) );
	CEventManager_->AttachEventListener( CHSC( EVENT1 ) , sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptor< CHogeEvent2 >( &hoge2, &CHogeEvent2::HogeFunc ) ) );
	CEventManager_->AttachEventListener( CHSC( EVENT1 ) , sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptor< CHogeEvent3 >( &hoge3, &CHogeEvent3::HogeFunc ) ) );

	CEventManager_->AttachEventListener( CHSC( EVENT2 ) , sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptor< CHogeEvent1 >( &hoge1, &CHogeEvent1::HogeFunc ) ) );
	CEventManager_->AttachEventListener( CHSC( EVENT2 ) , sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptor< CHogeEvent2 >( &hoge2, &CHogeEvent2::HogeFunc ) ) );

	CEventManager_->AttachEventListener( CHSC( EVENT3 ) , sky::lib::delegate::IDelegateObjectPtr( NEW sky::lib::delegate::CEventDelegateAdaptor< CHogeEvent1 >( &hoge1, &CHogeEvent1::HogeFunc ) ) );

	//FPSコントローラー作成
	sky::lib::fps::CFpsController fpsCtrl;

	//フレームレートの設定
	fpsCtrl.SetFrameRate( FRAME_RATE );

	SKY_PRINTF( _T( "エスケープキーを押下してください。 \n" ) );

	//ゲームループ
	GAME_LOOP_START
	{
		//フレーム開始
		fpsCtrl.StartFrame();

		//入力管理更新
		Ckeyboard_->Update( fpsCtrl.GetPassageTime() );

		//入力受付
		if ( Ckeyboard_->IsKeyPress( KEY_A ) )
		{
			SKY_PRINTF( _T( "イベント１同期実行 \n" ) );
			CEventManager_->ExecuteEvent( CHSC( EVENT1 ) );
		} 
		else if ( Ckeyboard_->IsKeyPress( KEY_B ) )
		{
			SKY_PRINTF( _T( "イベント２同期実行 \n" ) );
			CEventManager_->ExecuteEvent( CHSC( EVENT2 ) );
		}
		else if ( Ckeyboard_->IsKeyPress( KEY_C ) )
		{
			SKY_PRINTF( _T( "イベント３同期実行 \n" ) );
			CEventManager_->ExecuteEvent( CHSC( EVENT3 ) );
		}
		else if ( Ckeyboard_->IsKeyPress( KEY_D ) )
		{
			SKY_PRINTF( _T( "イベント１を６０フレーム後に実行 \n" ) );
			CEventManager_->ExecuteEventAsync( CHSC( EVENT1 ) , sky::lib::event::DEFAULT_UNIQ_ID , sky::lib::CommonSmartPointer() , sky::lib::delegate::CDelegateArgs() , 60 );
		}
		else if ( Ckeyboard_->IsKeyPress( KEY_R ) )
		{
			SKY_PRINTF( _T( "イベント２を１２０フレーム後に実行 \n" ) );
			CEventManager_->ExecuteEventAsync( CHSC( EVENT2 ) , sky::lib::event::DEFAULT_UNIQ_ID , sky::lib::CommonSmartPointer() , sky::lib::delegate::CDelegateArgs() , 120 );
		}
		else if ( Ckeyboard_->IsKeyPress( KEY_M ) )
		{
			SKY_PRINTF( _T( "イベント３を１８０フレーム後に実行 \n" ) );
			CEventManager_->ExecuteEventAsync( CHSC( EVENT3 ) , sky::lib::event::DEFAULT_UNIQ_ID , sky::lib::CommonSmartPointer() , sky::lib::delegate::CDelegateArgs() , 180 );
		}
		else if ( Ckeyboard_->IsKeyPress( KEY_S ) )
		{
			SKY_PRINTF( _T( "イベント２を１２０フレーム後に３０フレーム間隔で５回実行 \n" ) );
			CEventManager_->ExecuteEventAsync( CHSC( EVENT2 ) , sky::lib::event::DEFAULT_UNIQ_ID , sky::lib::CommonSmartPointer() , sky::lib::delegate::CDelegateArgs() , 120 , 5 , 30 );
		}
		else if ( Ckeyboard_->IsKeyPress( KEY_T ) )
		{
			SKY_PRINTF( _T( "イベント３を１８０フレーム後に１２０フレーム間隔で３回実行 \n" ) );
			CEventManager_->ExecuteEventAsync( CHSC( EVENT3 ) , sky::lib::event::DEFAULT_UNIQ_ID , sky::lib::CommonSmartPointer() , sky::lib::delegate::CDelegateArgs() , 180 , 3 , 120 );
		}

		//終了判定
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) )
		{
			GAME_LOOP_EXIT();
		}

		//更新
		CEventManager_->Update();

		//フレームレート維持のため、時間が余ったら待つ
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	SKY_BASE_LIB_TERM();
}

#endif