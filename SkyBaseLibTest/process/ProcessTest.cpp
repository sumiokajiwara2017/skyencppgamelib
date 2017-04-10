#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_PROCESS

#include "SkyBaseLib/Process.h"
#include "SkyBaseLib/GameLoop.h"
#include "SkyBaseLib/Input.h"
#include "SkyBaseLib/Fps.h"

static const u32 FRAME_RATE = 60;

class ProcessA : public sky::lib::process::AbsProcess
{

public:

	ProcessA(){}
	virtual ~ProcessA(){}

	void	vInitialize()
	{
		SKY_PRINTF( _T( "ProcessA::vInitialize() \n" ) );
	}
	void	vDispose()
	{
		SKY_PRINTF( _T( "ProcessA::vDispose() \n" ) );
	}
	void	vPreUpdate( updateTime time )
	{
		time;
		SKY_PRINTF( _T( "ProcessA::vPreUpdate() \n" ) );
	}
	sky::lib::process::IProcess::eResult vUpdate( updateTime time )
	{
		time;
		SKY_PRINTF( _T( "ProcessA::vUpdate() \n" ) );
		return sky::lib::process::IProcess::eResult_End;
	}
	void	vPostUpdate( updateTime time )
	{
		time;
		SKY_PRINTF( _T( "ProcessA::vPostUpdate() \n" ) );
	}
};

class ProcessAChidl_1 : public sky::lib::process::AbsProcess
{

public:

	ProcessAChidl_1(){}
	virtual ~ProcessAChidl_1(){}

	void	vInitialize()
	{
		SKY_PRINTF( _T( "ProcessAChidl_1::vInitialize() \n" ) );
	}
	void	vDispose()
	{
		SKY_PRINTF( _T( "ProcessAChidl_1::vDispose() \n" ) );
	}
	void	vPreUpdate( updateTime time )
	{
		time;
		SKY_PRINTF( _T( "ProcessAChidl_1::vPreUpdate() \n" ) );
	}
	sky::lib::process::IProcess::eResult  vUpdate( updateTime time )
	{
		time;
		SKY_PRINTF( _T( "ProcessAChidl_1::vUpdate() \n" ) );
		return sky::lib::process::IProcess::eResult_End;
	}
	void	vPostUpdate( updateTime time )
	{
		time;
		SKY_PRINTF( _T( "ProcessAChidl_1::vPostUpdate() \n" ) );
	}
};

class ProcessAChidl_2 : public sky::lib::process::AbsProcess
{

public:

	ProcessAChidl_2(){}
	virtual ~ProcessAChidl_2(){}

	void	vInitialize()
	{
		SKY_PRINTF( _T( "ProcessAChidl_2::vInitialize() \n" ) );
	}
	void	vDispose()
	{
		SKY_PRINTF( _T( "ProcessAChidl_2::vDispose() \n" ) );
	}
	void	vPreUpdate( updateTime time )
	{
		time;
		SKY_PRINTF( _T( "ProcessAChidl_2::vPreUpdate() \n" ) );
	}
	sky::lib::process::IProcess::eResult  vUpdate( updateTime time )
	{
		time;
		SKY_PRINTF( _T( "ProcessAChidl_2::vUpdate() \n" ) );
		return sky::lib::process::IProcess::eResult_End;
	}
	void	vPostUpdate( updateTime time )
	{
		time;
		SKY_PRINTF( _T( "ProcessAChidl_2::vPostUpdate() \n" ) );
	}
};

class ProcessAChidl_3 : public sky::lib::process::AbsProcess
{

public:

	ProcessAChidl_3(){}
	virtual ~ProcessAChidl_3(){}

	void	vInitialize()
	{
		SKY_PRINTF( _T( "ProcessAChidl_3::vInitialize() \n" ) );
	}
	void	vDispose()
	{
		SKY_PRINTF( _T( "ProcessAChidl_3::vDispose() \n" ) );
	}
	void	vPreUpdate( updateTime time )
	{
		time;
		SKY_PRINTF( _T( "ProcessAChidl_3::vPreUpdate() \n" ) );
	}
	sky::lib::process::IProcess::eResult  vUpdate( updateTime time )
	{
		time;
		SKY_PRINTF( _T( "ProcessAChidl_3::vUpdate() \n" ) );
		return sky::lib::process::IProcess::eResult_End;
	}
	void	vPostUpdate( updateTime time )
	{
		time;
		SKY_PRINTF( _T( "ProcessAChidl_3::vPostUpdate() \n" ) );
	}
};

class ProcessB : public sky::lib::process::AbsProcess
{

public:

	ProcessB(){}
	virtual ~ProcessB(){}

	void	vInitialize()
	{
		SKY_PRINTF( _T( "ProcessB::vInitialize() \n" ) );
	}
	void	vDispose()
	{
		SKY_PRINTF( _T( "ProcessB::vDispose() \n" ) );
	}
	void	vPreUpdate( updateTime time )
	{
		time;
		SKY_PRINTF( _T( "ProcessB::vPreUpdate() \n" ) );
	}
	sky::lib::process::IProcess::eResult  vUpdate( updateTime time )
	{
		time;
		SKY_PRINTF( _T( "ProcessB::vUpdate() \n" ) );
		return sky::lib::process::IProcess::eResult_End;
	}
	void	vPostUpdate( updateTime time )
	{
		time;
		SKY_PRINTF( _T( "ProcessB::vPostUpdate() \n" ) );
	}
};

class ProcessC : public sky::lib::process::AbsProcess
{

public:

	ProcessC(){}
	virtual ~ProcessC(){}

	void	vInitialize()
	{
		SKY_PRINTF( _T( "ProcessC::vInitialize() \n" ) );
	}
	void	vDispose()
	{
		SKY_PRINTF( _T( "ProcessC::vDispose() \n" ) );
	}
	void	vPreUpdate( updateTime time )
	{
		time;
		SKY_PRINTF( _T( "ProcessC::vPreUpdate() \n" ) );
	}
	sky::lib::process::IProcess::eResult  vUpdate( updateTime time )
	{
		time;
		SKY_PRINTF( _T( "ProcessC::vUpdate() \n" ) );
		return sky::lib::process::IProcess::eResult_End;
	}
	void	vPostUpdate( updateTime time )
	{
		time;
		SKY_PRINTF( _T( "ProcessC::vPostUpdate() \n" ) );
	}
};

TEST( Process , 1 ) 
{
	SKY_BASE_LIB_INIT();

	sky::lib::process::CProcessManagerPtr processManager = sky::lib::process::CProcessManager::Create();

	sky::lib::process::IProcessPtr processA( NEW ProcessA() );
	sky::lib::process::IProcessPtr processAChild_1( NEW ProcessAChidl_1() );
	sky::lib::process::IProcessPtr processAChild_2( NEW ProcessAChidl_2() );
	sky::lib::process::IProcessPtr processAChild_3( NEW ProcessAChidl_3() );
	sky::lib::process::IProcessPtr processB( NEW ProcessB() );
	sky::lib::process::IProcessPtr processC( NEW ProcessC() );

	processA->vAttachChildProcess( processAChild_1 );
	processA->vAttachChildProcess( processAChild_2 );
	processA->vAttachChildProcess( processAChild_3 );

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
			SKY_PRINTF( _T( "�v���Z�X�`���s \n" ) );
			processManager->AttachProcess( 0 , processA );
		} 
		else if ( Ckeyboard_->IsKeyPress( KEY_B ) )
		{
			SKY_PRINTF( _T( "�v���Z�X�a���s \n" ) );
			processManager->AttachProcess( 0 , processB );
		} 
		else if ( Ckeyboard_->IsKeyPress( KEY_C ) )
		{
			SKY_PRINTF( _T( "�v���Z�X�b���s \n" ) );
			processManager->AttachProcess( 0 , processC );
		} 
		else if ( Ckeyboard_->IsKeyPress( KEY_X ) )
		{
			SKY_PRINTF( _T( "�v���Z�X�`�a�b���s \n" ) );
			processManager->AttachProcess( 0 , processA );
			processManager->AttachProcess( 0 , processB );
			processManager->AttachProcess( 0 , processC );
		} 

		//�I������
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) )
		{
			GAME_LOOP_EXIT();
		}

		//�X�V
		processManager->Update( fpsCtrl.GetPassageTime() );

		//�t���[�����[�g�ێ��̂��߁A���Ԃ��]������҂�
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

#ifdef SW_SKYLIB_NO_USE_SMART_POINTER
	DEL processA;
	DEL processAChild_1;
	DEL processAChild_2;
	DEL processAChild_3;
	DEL processB;
	DEL processC;
#endif

	SKY_BASE_LIB_TERM();
}

#endif