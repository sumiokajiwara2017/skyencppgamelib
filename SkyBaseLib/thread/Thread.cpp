#include "StdAfx.h"
#include "SkyBaseLib/Thread.h"

#include <process.h> 

namespace sky { namespace lib { namespace thread {

#ifdef SKYLIB_PLATFORM_WIN32

CThread::CThread( u32 stackSize )
{
	Constractor( stackSize );
}

CThread::CThread(  u32 stackSize , const FunctionCallBackAsyncPtr &spFunction )
{
	Constractor( stackSize );
	m_spFunction = spFunction;
}

CThread::~CThread()
{

}

CThreadPtr CThread::Create( u32 stackSize , const FunctionCallBackAsyncPtr &spFunction )
{
	return CThreadPtr( NEW CThread( stackSize , spFunction ) );
}

void CThread::Constractor( u32 stackSize )
{
//	m_ThreadHndl = ::CreateThread(
	m_ThreadHndl = ( HANDLE )::_beginthreadex(   //�v���C�}���X���b�h�ȊO�̃X���b�h��C�����^�C�����C�u�������g�p����ꍇ�́A CreateThread�֐��ł͂Ȃ��A_beginthreadex�֐����g���K�v������܂��B
	NULL ,                                   //�Z�L�����e�B�����\���̂ւ̃|�C���^���w�肵�܂����̍\���̂��g���̂� Windows NT �̃t�@�C���V�X�e���̏ꍇ�ł�
	stackSize ,                              //�X���b�h�̃X�^�b�N�T�C�Y 
	ThreadEntry_ ,							 //�X���b�h�̃G���g���[�֐�
	( LPVOID )this ,                         //�X���b�h�̈����i�����Ɏ����̃A�h���X��u�����ƂɈӖ�������j
	CREATE_SUSPENDED ,                       //���f��ԂŐ����iStart�֐��ŊJ�n���邽�߁j
	&m_ThreadId );                           //�X���b�hID�擾

	if ( m_ThreadHndl == NULL || m_ThreadId == NULL )
	{
		m_State = eState_Error;
		SKY_PANIC_MSG( _T( "CreateThread failed." ) );
		return;
	}

	m_State = eState_Suspend;
}

void CThread::ThreadMain()
{
	//�e���ŃI�[�o�[���C�h���Ďg�p���Ă��������B
}

void CThread::Start()
{
	ReStart();
}

void CThread::End()
{
	BOOL ret = ::TerminateThread( m_ThreadHndl , FALSE );

	if ( ret != TRUE )
	{
		m_State = eState_Error;
		SKY_PANIC_MSG( _T( "End failed." ) );
	}
}

skyBool CThread::IsEnd()
{
	skyBool result = skyTrue;

	DWORD exitCode;
	BOOL ret = ::GetExitCodeThread( m_ThreadHndl , &exitCode );

	if ( ret != TRUE )
	{
		m_State = eState_Error;
		SKY_PANIC_MSG( _T( "IsEnd failed." ) );
	}

	else if ( exitCode == STILL_ACTIVE )
	{
		result = skyFalse;
	}

	return result;
}

void CThread::EndWait()
{
	skyBool isExec = skyTrue;
	while( isExec )
	{
		if ( IsEnd() == skyTrue )
		{
			break;
		}

		//������Ƒ҂��Ă݂�
		CThreadUtil_::Sleep( 1 );
	}
}

void CThread::Stop()
{
	if ( m_State != eState_Suspend )
	{
		::SuspendThread( m_ThreadHndl );
		m_State = eState_Suspend;
	}
}

void CThread::ReStart()
{
	if ( m_State == eState_Suspend )
	{
		::ResumeThread( m_ThreadHndl );
		m_State = eState_Exec;
	}
}

unsigned __stdcall CThread::ThreadEntry_( void* pParam )
{
	CThread *pThis = ( CThread * )pParam;

	if ( pThis->m_spFunction.IsEmpty() == skyTrue )
	{
		pThis->ThreadMain();
	}
	else
	{
		pThis->m_spFunction->run();
	}

	pThis->m_State = eState_End;

	return 0;
}

tread_id CThread::GetId()
{
	return m_ThreadId;
}

tread_hndl CThread::GetHndl()
{
	return m_ThreadHndl;
}

#endif

} } }