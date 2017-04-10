#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"


#ifdef TEST_STATE

#include "SkyBaseLib/State.h"
#include "SkyBaseLib/GameLoop.h"
#include "SkyBaseLib/Input.h"
#include "SkyBaseLib/Fps.h"
#include "SkyBaseLib/SKYTL.h"

static const u32 FRAME_RATE = 60;
static const skyString *STATEA_NAME = _T( "CHogeStateA" );
static const skyString *STATEB_NAME = _T( "CHogeStateB" );
static const skyString *STATEC_NAME = _T( "CHogeStateC" );

class CHogeStateA : public sky::lib::state::AbsState
{

public:

	CHogeStateA() : AbsState( CHS( STATEA_NAME ) ){}
	virtual ~CHogeStateA(){}

	void vEnter()
	{
		SKY_PRINTF( _T( "HogeStateA::Enter() \n" ) );
	}
	void vExit()
	{
		SKY_PRINTF( _T( "HogeStateA::Exit() \n" ) );
	}
	void vSuspend( const sky::lib::state::IStatePtr overState )
	{
		overState;
		SKY_PRINTF( _T( "HogeStateA::Suspend( const sky::lib::state::IStatePtr overState ) \n" ) );
	}
	void vResume( const sky::lib::state::IStatePtr &overState )
	{
		overState;
		SKY_PRINTF( _T( "HogeStateA::Resume( const sky::lib::state::IStatePtr &overState ) \n" ) );
	}
	void vUpdate( updateTime time )
	{
		time;
//		SKY_PRINTF( _T( "HogeStateA::Update( updateTime time ) \n" ) );
	}
};
typedef sky::lib::SmartPointer< CHogeStateA >	CHogeStateAPtr;

class CHogeStateB : public sky::lib::state::AbsState
{

public:

	CHogeStateB() : AbsState( CHS( STATEB_NAME ) ){}
	virtual ~CHogeStateB(){}

	void vEnter()
	{
		SKY_PRINTF( _T( "HogeStateB::Enter() \n" ) );
	}
	void vExit()
	{
		SKY_PRINTF( _T( "HogeStateB::Exit() \n" ) );
	}
	void vSuspend( const sky::lib::state::IStatePtr overState )
	{
		overState;
		SKY_PRINTF( _T( "HogeStateB::Suspend( const sky::lib::state::IStatePtr overState ) \n" ) );
	}
	void vResume( const sky::lib::state::IStatePtr &overState )
	{
		overState;
		SKY_PRINTF( _T( "HogeStateB::Resume( const sky::lib::state::IStatePtr &overState ) \n" ) );
	}
	void vUpdate( updateTime time )
	{
		time;
//		SKY_PRINTF( _T( "HogeStateB::Update( updateTime time ) \n" ) );
	}
};
typedef sky::lib::SmartPointer< CHogeStateB >	CHogeStateBPtr;

class CHogeStateC : public sky::lib::state::AbsState
{

public:

	CHogeStateC() : AbsState( CHS( STATEC_NAME ) ){}
	virtual ~CHogeStateC(){}

	void vEnter()
	{
		SKY_PRINTF( _T( "HogeStateC::Enter() \n" ) );
	}
	void vExit()
	{
		SKY_PRINTF( _T( "HogeStateC::Exit() \n" ) );
	}
	void vSuspend( const sky::lib::state::IStatePtr overState )
	{
		overState;
		SKY_PRINTF( _T( "HogeStateC::Suspend( const sky::lib::state::IStatePtr overState ) \n" ) );
	}
	void vResume( const sky::lib::state::IStatePtr &overState )
	{
		overState;
		SKY_PRINTF( _T( "HogeStateC::Resume( const sky::lib::state::IStatePtr &overState ) \n" ) );
	}
	void vUpdate( updateTime time )
	{
		time;
//		SKY_PRINTF( _T( "HogeStateC::Update( updateTime time ) \n" ) );
	}
};
typedef sky::lib::SmartPointer< CHogeStateC >	CHogeStateCPtr;

TEST( state , 1 ) 
{
	SKY_BASE_LIB_INIT();

	sky::lib::CDictionaryDec dic;
//	sky::lib::CDictionary< f32 > dic;

	sky::lib::CDictionaryDec dicBuf = dic;
//	sky::lib::CDictionary< f32 > dicBuf = dic;
	dic = dicBuf;

	dic.Set( 1.0f , 0 , 0 );
	dic.Set( 99.0f , 0 , 99 );
	dic.Set( 999.0f , 0 , 999 );
	dic.Set( 888.0f , 888 , 99999 );

	SKY_PRINTF( "%f \n" , dic.Get( 0 , 0 ) );
	SKY_PRINTF( "%f \n" , dic.Get( 0 , 99 ) );
	SKY_PRINTF( "%f \n" , dic.Get( 0 , 999 ) );
	SKY_PRINTF( "%f \n" , dic.Get( 888 , 99999 ) );

	dicBuf = dic;
	dic = dicBuf;


	SKY_PRINTF( "%f \n" , dic.Get( 0 , 0 ) );
	SKY_PRINTF( "%f \n" , dic.Get( 0 , 99 ) );
	SKY_PRINTF( "%f \n" , dic.Get( 0 , 999 ) );
	SKY_PRINTF( "%f \n" , dic.Get( 888 , 99999 ) );

	dic.Reset( 0 , 0 );

	CHogeStateAPtr CStateA( NEW CHogeStateA() );
	CHogeStateBPtr CStateB( NEW CHogeStateB() );
	CHogeStateCPtr CStateC( NEW CHogeStateC() );

	CStateManager_->Register( CHS( STATEA_NAME ) , CStateA );
	CStateManager_->Register( CHS( STATEB_NAME ) , CStateB );
	CStateManager_->Register( CHS( STATEC_NAME ) , CStateC );

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
		Ckeyboard_->Update( fpsCtrl );

		//���͎�t
		if ( Ckeyboard_->IsKeyPress( KEY_A ) )
		{
			SKY_PRINTF( _T( "�X�e�[�g�`�v�b�V�� \n" ) );
			CStateManager_->Push( CHS( STATEA_NAME ) );
		}
		else if ( Ckeyboard_->IsKeyPress( KEY_B ) )
		{
			SKY_PRINTF( _T( "�X�e�[�g�a�v�b�V�� \n" ) );
			CStateManager_->Push( CHS( STATEB_NAME ) );
		}
		else if ( Ckeyboard_->IsKeyPress( KEY_C ) )
		{
			SKY_PRINTF( _T( "�X�e�[�g�b�v�b�V�� \n" ) );
			CStateManager_->Push( CHS( STATEC_NAME ) );
		}
		else if ( Ckeyboard_->IsKeyPress( KEY_X ) )
		{
			SKY_PRINTF( _T( "�X�e�[�g�|�b�v \n" ) );
			CStateManager_->Pop();
		}
		else if ( Ckeyboard_->IsKeyPress( KEY_Y ) )
		{
			SKY_PRINTF( _T( "�X�e�[�g�N���A \n" ) );
			CStateManager_->Clear();
		}
		else if ( Ckeyboard_->IsKeyPress( KEY_Z ) )
		{
			SKY_PRINTF( _T( "�X�e�[�g�`�Ƀ`�F���W \n" ) );
			CStateManager_->Change( CHS( STATEA_NAME ) );
		}

		//�I������
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) )
		{
			GAME_LOOP_EXIT();
		}

		//�X�e�[�g�Ǘ��X�V
		CStateManager_->Update( fpsCtrl );

		//�t���[�����[�g�ێ��̂��߁A���Ԃ��]������҂�
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END


	SKY_BASE_LIB_TERM();
}

#endif