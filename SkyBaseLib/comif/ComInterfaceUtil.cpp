#include "StdAfx.h"
#include "SkyBaseLib/ComInterface.h"

namespace sky { namespace lib { namespace comif {

skyBool ComInterfaceUtil::m_bIsInitialized = skyFalse;

void ComInterfaceUtil::Initialize()
{
	if ( m_bIsInitialized == skyFalse )
	{
#ifdef SKYLIB_COMPILER_VC10

		//Com�̏�����
		HRESULT hr = CoInitialize( NULL );

		SKY_ASSERT_MSG( hr == S_OK , _T( "It initialized it twice. " ) ); //Com�̏������Ɏ��s���܂����B���łɕʂ̏������ŏ���������Ă���悤�ł��BSkyBaseLib�ł͂��̃��\�b�h�݂̂ŏ��������Ă��������B

#endif

		m_bIsInitialized = skyTrue;
	}
}

void ComInterfaceUtil::Dispose()
{
	if ( m_bIsInitialized == skyTrue )
	{

#ifdef SKYLIB_COMPILER_VC10

		//Com�̖�����
		CoUninitialize();

#endif

		m_bIsInitialized = skyFalse;
	}
}

} } }
