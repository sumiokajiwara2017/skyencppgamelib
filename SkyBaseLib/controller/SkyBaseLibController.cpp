#include "StdAfx.h"
#include "SkyBaseLib/Controller.h"
#include "SkyBaseLib/Memory.h"
#include "SkyBaseLib/Input.h"
#include "SkyBaseLib/Thread.h"
#include "SkyBaseLib/Event.h"
#include "SkyBaseLib/Directory.h"
#include "SkyBaseLib/Const.h"
#include "SkyBaseLib/ReLoad.h"
#include "SkyBaseLib/Args.h"
#include "SkyBaseLib/RegEx.h"
#include "SkyBaseLib/ComInterface.h"
#include "SkyBaseLib/Script.h"
#include "SkyBaseLib/State.h"
#include "SkyBaseLib/NetWork.h"

namespace sky { namespace lib { namespace controller {

//-------------------------------------< ���� >---------------------------------------------//

thread::CriticalSection *CBaseLibController::m_pCs = skyNull;
skyBool CBaseLibController::m_bIsInitialized = skyFalse;

thread::CriticalSection *CBaseLibController::GetCriticalSection()
{
	if ( m_pCs == skyNull )
	{
		m_pCs = thread::CriticalSection::Create();
	}
	return m_pCs;
}

void CBaseLibController::Initialize( u32 playerNum )
{
	thread::CriticalLock cs( GetCriticalSection() );

	if ( m_bIsInitialized == skyFalse )
	{
		//IPlatformParam_; ����̓G���g���[�֐��̒���ł���Ă���B

		//�萔�̃��[�h
		cons::PgConst::Load();

		//������
		CDebugUtilInit_();
		CWorkerThreadManagerCreate_();
		CkeyboardInitialize_( playerNum );
		CMouseInitialize_( playerNum );
		CPadInitialize_( playerNum );
		CFileUtilInitialize_();
		CHashStringManagerInitialize_();
		CPropertyManagerInitialize_();
		CEventManagerCreate_();
		CSoketInitialize_();
#ifdef SKYLIB_COMPILER_VC10
        IRegExCreate_();
#endif
		CReLoadObjectManagerInitialize_();
		CArgsManagerCreate_();
		CStateManagerCreate_();
		CTimeUtilInitialize_();
		ComInterfaceUtilInitialize_();
#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL
		SquirrelControllerInitialize_();
#endif
		CBindFileManagerInitialize_();
		CDefaultFileManagerInitialize_();
		CFilePoolContenaInitialize_();

		//���[�J�[�X���b�h�����i�X���b�h�͐ϋɓI�Ɏg�p���܂��傤�j
		CWorkerThreadManager_->StartWorker();

		m_bIsInitialized = skyTrue;
	}
}

void CBaseLibController::GC()
{
	CHashStringManager_::GC();
}

void CBaseLibController::InitAssert()
{
	SKY_ASSERT_MSG( m_bIsInitialized , _T( "SkyLib has not initialized it yet." ) ); //SkyLib�͂܂����������Ă��܂���BSkyOnigurumaLibController_->Init()���s���Ă��������B
}

skyBool CBaseLibController::IsInit()
{
	return m_bIsInitialized;
}

void CBaseLibController::Dispose()
{
	//�I���҂�
	CWorkerThreadManager_->WaitEndWork();

	//���[�J�[�X���b�h�I���v��
	CWorkerThreadManager_->EndWorkerRequest();

	//���[�J�[�X���b�h�I���҂�
	CWorkerThreadManager_->WaitEndWorker();

	thread::CriticalSection::Delete( m_pCs );
	m_pCs = skyNull;

	if ( m_bIsInitialized == skyTrue )
	{
		//������
		CFilePoolContenaDispose_();
		CDefaultFileManagerDispose_();
		CBindFileManagerDispose_();
#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL
		SquirrelControllerDispose_();
#endif
		ComInterfaceUtilDispose_();
		CTimeUtilDispose_();
		CStateManagerDelete_();
		CArgsManagerDelete_();
		CReLoadObjectManagerDispose_();
#ifdef SKYLIB_COMPILER_VC10
        IRegExDelete_();
#endif
		CSoketDispose_();
		CEventManagerDelete_();
		CPropertyManagerDispose_();
		CHashStringManagerDispose_();
		CFileUtilDispose_();
		CkeyboardDispose_();
		CMouseDispose_();
		CPadDispose_();
		CWorkerThreadManagerDelete_();
		CDebugUtilDispose_();
		//�萔�̔j��
		cons::PgConst::Destroy();

		m_bIsInitialized = skyFalse;
	}
}

} } }
