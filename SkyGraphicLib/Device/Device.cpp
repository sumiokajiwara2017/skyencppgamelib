#include "StdAfx.h"
#include "SkyGraphicLib/Device.h"

#include "SkyGraphicLib/Window.h"

//===================================�yDirectX�z===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

struct FeatureLevelString
{
	D3D_FEATURE_LEVEL eLevel;
	const skyMString  *pVS;
	const skyMString  *pPS;
};
static const FeatureLevelString  s_FeatureLevel[] =
{
	{ D3D_FEATURE_LEVEL_11_0 , "vs_5_0"           , "ps_5_0" } ,
	{ D3D_FEATURE_LEVEL_10_1 , "vs_4_1"           , "ps_4_1" } ,
	{ D3D_FEATURE_LEVEL_10_0 , "vs_4_0"           , "ps_4_0" } ,
	{ D3D_FEATURE_LEVEL_9_3  , "vs_4_0_level_9_3" , "ps_4_0_level_9_3" } ,
	{ D3D_FEATURE_LEVEL_9_2  , "vs_4_0_level_9_1" , "ps_4_0_level_9_1" } ,
	{ D3D_FEATURE_LEVEL_9_1  , "vs_4_0_level_9_1" , "ps_4_0_level_9_1" } ,
};

const skyMString* CDeviceMember::GetVSVersion()
{
	const skyMString *pResult = skyNull;

	for ( u32 i = 0 ; i < array_sizeof( s_FeatureLevel ) ; i++ )
	{
		if ( m_FeatureLevel == s_FeatureLevel[ i ].eLevel )
		{
			pResult = s_FeatureLevel[ i ].pVS;
			break;
		}
	}
	SKY_ASSERT_MSG( pResult != skyNull , _T( "It is a version outside the support." ) ); //�T�|�[�g�O�̃o�[�W�����ł��B
	return pResult;
}

const skyMString* CDeviceMember::GetPSVersion()
{
	const skyMString *pResult = skyNull;

	for ( u32 i = 0 ; i < array_sizeof( s_FeatureLevel ) ; i++ )
	{
		if ( m_FeatureLevel == s_FeatureLevel[ i ].eLevel )
		{
			pResult = s_FeatureLevel[ i ].pPS;
			break;
		}
	}
	SKY_ASSERT_MSG( pResult != skyNull , _T( "It is a version outside the support." ) );
	return pResult;
}

void CDevice::Initialize()
{
	if ( m_IsInit == skyFalse )
	{
		ID3D11Device*        pDevice        = skyNull;
		ID3D11DeviceContext* pDeviceContext = skyNull;
        IDXGIDevice1*        pDXGIDevice    = skyNull;
        IDXGIAdapter*        pDXGIAdapter   = skyNull;
        IDXGIFactory*        pDXGIFactory   = skyNull;

		D3D_FEATURE_LEVEL FeatureLevels[] = {
			D3D_FEATURE_LEVEL_11_0,        // DirectX11�Ή�GPU
			D3D_FEATURE_LEVEL_10_1,        // DirectX10.1�Ή�GPU
			D3D_FEATURE_LEVEL_10_0,        // DirectX10�Ή�GPU
			D3D_FEATURE_LEVEL_9_3,        // DirectX9/ShaderModel3�Ή�GPU
			D3D_FEATURE_LEVEL_9_2,        // DirectX9/ShaderModel2�Ή�GPU
			D3D_FEATURE_LEVEL_9_1,        // DirectX9/ShaderModel2�Ή�GPU
		};

		//�f�o�C�X�̍쐬
		if ( FAILED ( D3D11CreateDevice( 
			NULL , 
			D3D_DRIVER_TYPE_HARDWARE , 
			NULL , 
			0 , 
			FeatureLevels , 
			array_sizeof( FeatureLevels ) , 
			D3D11_SDK_VERSION , 
			&pDevice , 
			&m_pMember->m_FeatureLevel , 
			&pDeviceContext ) ) )
		{
			SKY_PANIC_MSG( _T( "It failed in the initialization of D3D." ) ); //D3D�̏������Ɏ��s���܂����BCreateDevice�ɂăf�o�C�X�̍쐬�ɑS�p�^�[�����s�����悤�ł��B
		}

		//�R���|�[�l���g���T�|�[�g����C���^�[�t�F�[�X���擾����
		if ( pDevice->QueryInterface( __uuidof( IDXGIDevice1 ), ( void** )&pDXGIDevice ) )
		{
			SKY_PANIC_MSG( _T( "It failed in the initialization of D3D." ) ); //D3D�̏������Ɏ��s���܂����BQueryInterface�����s�����悤�ł��B
		}

		//�A�_�v�^�[�̐���
		if( FAILED( pDXGIDevice->GetAdapter( &pDXGIAdapter ) ) )
		{
			SKY_PANIC_MSG( _T( "It failed in the initialization of D3D." ) ); //D3D�̏������Ɏ��s���܂����BGetAdapter�����s�����悤�ł��B
		}

		//�t�@�N�g���[�̐���
		if( FAILED( pDXGIAdapter->GetParent( __uuidof( IDXGIFactory ) , ( void** )&pDXGIFactory ) ) )
		{
			SKY_PANIC_MSG( _T( "It failed in the initialization of D3D." ) ); //D3D�̏������Ɏ��s���܂����BGetParent�����s�����悤�ł��B
		}

		//�쐬�����c�R�c�֘A�̃I�u�W�F�N�g���X�}�[�g�|�C���^�Ɋi�[����
		m_pMember->m_spD3DDevice.SetPtr( pDevice );
		m_pMember->m_spD3DDeviceContext.SetPtr( pDeviceContext );
		m_pMember->m_DxDevice1.SetPtr( pDXGIDevice );
		m_pMember->m_spDXGIAdapter.SetPtr( pDXGIAdapter );
		m_pMember->m_spDXGIFactory.SetPtr( pDXGIFactory );

		m_IsInit = skyTrue;
	}
}

void CDevice::Dispose()
{
}


void CDevice::BeginRendering()
{
    //Begin�Ɉ�����n�����ƂŁA�f�o�t����l���擾������A�f�o�t�̏������ׂ��v���ł���B
//  CDevice_->GetMember()->m_spD3DDeviceContext->Begin( NULL );
}


void CDevice::EndRendering()
{
	//Begin�Ɉ�����n�����ƂŁA�f�o�t����l���擾������A�f�o�t�̏������ׂ��v���ł���B
//  CDevice_->GetMember()->m_spD3DDeviceContext->End( NULL );
}


skyBool CDevice::IsInit			() const { return m_IsInit; };

} } }

#endif
//===================================�yOpenGL�z====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

void CDevice::Initialize(){}
void CDevice::Dispose(){}
void CDevice::BeginRendering(){}
void CDevice::EndRendering(){}
skyBool CDevice::IsInit			() const { return skyFalse; };

} } }

#endif

//===================================�yCommon�z===================================================>

namespace sky { namespace lib { namespace graphic {

CDevicePtr CDevice::m_pMySelf = skyNull;

CDevice::CDevice() : m_IsInit( skyFalse )
{
	m_pMember = NEW__( CDeviceMember , CDeviceMember );
};

CDevice::~CDevice()
{
    DEL m_pMember;
}

void CDevice::CreateInstance()
{
    SKY_ASSERT_MSG( m_pMySelf == skyNull , _T( "It has already been created." ) ); //�쐬�ς�
    m_pMySelf = NEW__( CDevice , CDevice() );
}

CDevicePtr CDevice::GetInstance()
{
    SKY_ASSERT_MSG( m_pMySelf != skyNull , _T( "It has not been created yet." ) ); //�܂��쐬����Ă��Ȃ�
    return m_pMySelf;
}

void CDevice::DeleteInstance()
{
    SKY_ASSERT_MSG( m_pMySelf != skyNull , _T( "It has not been created yet." ) ); //�܂��쐬����Ă��Ȃ�
    DEL m_pMySelf;
    m_pMySelf = skyNull;
}

} } }
