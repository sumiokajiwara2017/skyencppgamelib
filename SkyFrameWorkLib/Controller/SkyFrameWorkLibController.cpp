#include "StdAfx.h"
#include "SkyFrameWorkLib/Controller.h"
#include "SkyFrameWorkLib/UI.h"
#include "SkyFrameWorkLib/UndoRedo.h"

namespace sky { namespace lib { namespace framework {

//-------------------------------------< 実装 >---------------------------------------------//

thread::CriticalSection *CFrameWorkLibController::m_pCs = skyNull;
skyBool CFrameWorkLibController::m_bIsInitialized = skyFalse;
skyBool CFrameWorkLibController::m_bIsFullScreen = skyFalse;

thread::CriticalSection *CFrameWorkLibController::GetCriticalSection()
{
	if ( m_pCs == skyNull )
	{
		m_pCs = thread::CriticalSection::Create();
	}
	return m_pCs;
}

void CFrameWorkLibController::Initialize()
{
	thread::CriticalLock cs( GetCriticalSection() );

	SKY_ASSERT_MSG( !m_bIsInitialized , _T( "It has already been initialized." ) ); //グラフィックライブラリを初期化したにも関わらず、また初期化しようとしています。

	if ( m_bIsInitialized == skyFalse )
	{
//		CFontManagerInitialize_(); ※内部でスプライトを生成しているのでここでは生成できない。OpenGLが初期化にWindowハンドルを必要とするため。

		m_bIsInitialized = skyTrue;
	}
}

void CFrameWorkLibController::Dispose()
{
	SKY_ASSERT_MSG( m_bIsInitialized , _T( "It has already ended." ) ); //グラフィックライブラリを末期化したのに末期化しようとしたか、初期化せずに末期化しようとしています。
	thread::CriticalSection::Delete( m_pCs );
	m_pCs = skyNull;

	if ( m_bIsInitialized == skyTrue )
	{
		m_bIsInitialized = skyFalse;
	}
}

void CFrameWorkLibController::InitAssert()
{
	SKY_ASSERT_MSG( m_bIsInitialized , _T( "A graphic library has not been initialized yet." ) ); //グラフィックライブラリはまだ初期化されていません。
}

} } }