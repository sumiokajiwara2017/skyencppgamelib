#include "StdAfx.h"
#include "SkyGraphicLib/Controller.h"
#include "SkyGraphicLib/Screen.h"
#include "SkyGraphicLib/Renderer.h"
#include "SkyGraphicLib/DataBase.h"
#include "SkyGraphicLib/Loader.h"
#include "SkyGraphicLib/Device.h"

namespace sky { namespace lib { namespace graphic {

//-------------------------------------< ���� >---------------------------------------------//

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

	SKY_ASSERT_MSG( !m_bIsInitialized , _T( "It tried to initialize it twice." ) ); //�O���t�B�b�N���C�u�����������������ɂ��ւ�炸�A�܂����������悤�Ƃ��Ă��܂��B

	if ( m_bIsInitialized == skyFalse )
	{
		//�f�o�C�X�̐���
		CDeviceCreate_();

		//������
		CGraphicFilePathManagerInitialize_();
		CFBXLoaderInitialize_();
		CDataBaseInitialize_();
		CRenderCommandManagerInitialize_();
		CScreenObjectInitialize_();


		//�e��}�l�[�W���[����
		CRenderStepManagerCreate_();
		CFadeManagerCreate_();
//		CFadeManagerCreate_(); �����ŃX�v���C�g�𐶐����Ă���̂ŁA�����ł͐����ł��Ȃ��BOpenGL����������Window�n���h����K�v�Ƃ��邽�߁B

		//�V�F�[�_�[������
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
	SKY_ASSERT_MSG( m_bIsInitialized , _T( "It has already ended." ) ); //�O���t�B�b�N���C�u�����𖖊��������̂ɖ��������悤�Ƃ������A�����������ɖ��������悤�Ƃ��Ă��܂��B
	thread::CriticalSection::Delete( m_pCs );
	m_pCs = skyNull;

	if ( m_bIsInitialized == skyTrue )
	{
		//���[�h���ꂽ�v�f�͂��ׂĔj������B
		CDataBase_::Clear();

		//�V�F�[�_�[�̖�����
		CShaderObject::Dispose();

		//�e��}�l�[�W���[�폜
//		CFontManagerDispose_();
		CFadeManagerDelete_();
		CRenderStepManagerDelete_();

		//������
		CFontManagerDispose_();
		CScreenObjectDispose_();
		CRenderCommandManagerDispose_();
		CDataBaseDispose_();
		CFBXLoaderDispose_();
		CGraphicFilePathManagerDispose_();

		//�f�o�C�X�̐���
		CDeviceDelete_();

		m_bIsInitialized = skyFalse;
	}
}

void CGraphicLibController::InitAssert()
{
	SKY_ASSERT_MSG( m_bIsInitialized , _T( "A graphic library has not been initialized yet." ) ); //�O���t�B�b�N���C�u�����͂܂�����������Ă��܂���B
}

} } }
