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

//-------------------------------------< 実装 >---------------------------------------------//

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
		//IPlatformParam_; これはエントリー関数の直後でやっている。

		//定数のロード
		cons::PgConst::Load();

		//初期化
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

		//ワーカースレッド生成（スレッドは積極的に使用しましょう）
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
	SKY_ASSERT_MSG( m_bIsInitialized , _T( "SkyLib has not initialized it yet." ) ); //SkyLibはまだ初期化していません。SkyOnigurumaLibController_->Init()を行ってください。
}

skyBool CBaseLibController::IsInit()
{
	return m_bIsInitialized;
}

void CBaseLibController::Dispose()
{
	//終了待ち
	CWorkerThreadManager_->WaitEndWork();

	//ワーカースレッド終了要求
	CWorkerThreadManager_->EndWorkerRequest();

	//ワーカースレッド終了待ち
	CWorkerThreadManager_->WaitEndWorker();

	thread::CriticalSection::Delete( m_pCs );
	m_pCs = skyNull;

	if ( m_bIsInitialized == skyTrue )
	{
		//末期化
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
		//定数の破棄
		cons::PgConst::Destroy();

		m_bIsInitialized = skyFalse;
	}
}

} } }
