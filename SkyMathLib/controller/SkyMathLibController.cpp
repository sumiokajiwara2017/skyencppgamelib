#include "StdAfx.h"
#include "SkyMathLib/Controller.h"

namespace sky { namespace lib { namespace math {

thread::CriticalSection *CMathLibController::m_pCs = skyNull;
skyBool CMathLibController::m_bIsInitialized = skyFalse;

thread::CriticalSection *CMathLibController::GetCriticalSection()
{
	if ( m_pCs == skyNull )
	{
		m_pCs = thread::CriticalSection::Create();
	}
	return m_pCs;
}

void CMathLibController::Initialize()
{
	thread::CriticalLock cs( GetCriticalSection() );

	if ( m_bIsInitialized == skyFalse )
	{
		//初期化
		m_bIsInitialized = skyTrue;
	}
}

void CMathLibController::InitAssert()
{
	SKY_ASSERT_MSG( m_bIsInitialized , _T( "It has not initialized it yet. " ) ); //SkyMathLibはまだ初期化していません。CMathLibController_->vInitialize()を行ってください。
}

void CMathLibController::Dispose()
{
	thread::CriticalSection::Delete( m_pCs );
	m_pCs = skyNull;

	if ( m_bIsInitialized == skyTrue )
	{
		//末期化
		m_bIsInitialized = skyFalse;
	}
}

} } }