#include "StdAfx.h"
#include "SkyFrameWorkLib/Controller.h"
#include "SkyFrameWorkLib/UI.h"
#include "SkyFrameWorkLib/UndoRedo.h"

namespace sky { namespace lib { namespace framework {

//-------------------------------------< ���� >---------------------------------------------//

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

	SKY_ASSERT_MSG( !m_bIsInitialized , _T( "It has already been initialized." ) ); //�O���t�B�b�N���C�u�����������������ɂ��ւ�炸�A�܂����������悤�Ƃ��Ă��܂��B

	if ( m_bIsInitialized == skyFalse )
	{
//		CFontManagerInitialize_(); �������ŃX�v���C�g�𐶐����Ă���̂ł����ł͐����ł��Ȃ��BOpenGL����������Window�n���h����K�v�Ƃ��邽�߁B

		m_bIsInitialized = skyTrue;
	}
}

void CFrameWorkLibController::Dispose()
{
	SKY_ASSERT_MSG( m_bIsInitialized , _T( "It has already ended." ) ); //�O���t�B�b�N���C�u�����𖖊��������̂ɖ��������悤�Ƃ������A�����������ɖ��������悤�Ƃ��Ă��܂��B
	thread::CriticalSection::Delete( m_pCs );
	m_pCs = skyNull;

	if ( m_bIsInitialized == skyTrue )
	{
		m_bIsInitialized = skyFalse;
	}
}

void CFrameWorkLibController::InitAssert()
{
	SKY_ASSERT_MSG( m_bIsInitialized , _T( "A graphic library has not been initialized yet." ) ); //�O���t�B�b�N���C�u�����͂܂�����������Ă��܂���B
}

} } }