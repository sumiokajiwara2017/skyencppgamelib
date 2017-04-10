#include "StdAfx.h"
#include "SkyGraphicLib/Controller.h"
#include "SkyGraphicLib/Screen.h"
#include "SkyGraphicLib/Renderer.h"
#include "SkyGraphicLib/DataBase.h"
#include "SkyGraphicLib/Loader.h"
#include "SkyGraphicLib/Device.h"

namespace sky { namespace lib { namespace graphic {

//-------------------------------------< 実装 >---------------------------------------------//

thread::CriticalSection *CGraphicLibController::m_pCs = skyNull;
skyBool CGraphicLibController::m_bIsInitialized = skyFalse;
skyBool CGraphicLibController::m_bIsFullScreen = skyFalse;

thread::CriticalSection *CGraphicLibController::GetCriticalSection()
{
	if ( m_pCs == skyNull )
	{
		m_pCs = thread::CriticalSection::Create();
	}
	return m_pCs;
}

void CGraphicLibController::Initialize()
{
	thread::CriticalLock cs( GetCriticalSection() );

	SKY_ASSERT_MSG( !m_bIsInitialized , _T( "It tried to initialize it twice." ) ); //グラフィックライブラリを初期化したにも関わらず、また初期化しようとしています。

	if ( m_bIsInitialized == skyFalse )
	{
		//デバイスの生成
		CDeviceCreate_();

		//初期化
		CGraphicFilePathManagerInitialize_();
		CFBXLoaderInitialize_();
		CDataBaseInitialize_();
		CRenderCommandManagerInitialize_();
		CScreenObjectInitialize_();


		//各種マネージャー生成
		CRenderStepManagerCreate_();
		CFadeManagerCreate_();
//		CFadeManagerCreate_(); 内部でスプライトを生成しているので、ここでは生成できない。OpenGLが初期化にWindowハンドルを必要とするため。

		//シェーダー初期化
		CShaderObject::Initialize();

		m_bIsInitialized = skyTrue;

#ifdef SKYLIB_RENDER_DIRECTX11
		SKY_PRINTF( "Graphic by DirectX \n" );
#endif
#ifdef SKYLIB_RENDER_OPENGL4
		SKY_PRINTF( "Graphic by OpenGL  \n" );
#endif
	}
}

void CGraphicLibController::Dispose()
{
	SKY_ASSERT_MSG( m_bIsInitialized , _T( "It has already ended." ) ); //グラフィックライブラリを末期化したのに末期化しようとしたか、初期化せずに末期化しようとしています。
	thread::CriticalSection::Delete( m_pCs );
	m_pCs = skyNull;

	if ( m_bIsInitialized == skyTrue )
	{
		//ロードされた要素はすべて破棄する。
		CDataBase_::Clear();

		//シェーダーの末期化
		CShaderObject::Dispose();

		//各種マネージャー削除
//		CFontManagerDispose_();
		CFadeManagerDelete_();
		CRenderStepManagerDelete_();

		//末期化
		CFontManagerDispose_();
		CScreenObjectDispose_();
		CRenderCommandManagerDispose_();
		CDataBaseDispose_();
		CFBXLoaderDispose_();
		CGraphicFilePathManagerDispose_();

		//デバイスの生成
		CDeviceDelete_();

		m_bIsInitialized = skyFalse;
	}
}

void CGraphicLibController::InitAssert()
{
	SKY_ASSERT_MSG( m_bIsInitialized , _T( "A graphic library has not been initialized yet." ) ); //グラフィックライブラリはまだ初期化されていません。
}

} } }
