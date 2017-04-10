#include "StdAfx.h"
#include "SkyBaseLib/ComInterface.h"

namespace sky { namespace lib { namespace comif {

skyBool ComInterfaceUtil::m_bIsInitialized = skyFalse;

void ComInterfaceUtil::Initialize()
{
	if ( m_bIsInitialized == skyFalse )
	{
#ifdef SKYLIB_COMPILER_VC10

		//Comの初期化
		HRESULT hr = CoInitialize( NULL );

		SKY_ASSERT_MSG( hr == S_OK , _T( "It initialized it twice. " ) ); //Comの初期化に失敗しました。すでに別の処理内で初期化されているようです。SkyBaseLibではこのメソッドのみで初期化してください。

#endif

		m_bIsInitialized = skyTrue;
	}
}

void ComInterfaceUtil::Dispose()
{
	if ( m_bIsInitialized == skyTrue )
	{

#ifdef SKYLIB_COMPILER_VC10

		//Comの末期化
		CoUninitialize();

#endif

		m_bIsInitialized = skyFalse;
	}
}

} } }
