#include "StdAfx.h"
#include "SkyBaseLib/NetWork.h"

#ifdef SKYLIB_PLATFORM_WIN32
#include <Natupnp.h>
#endif

namespace sky { namespace lib { namespace net {

#ifdef SKYLIB_PLATFORM_WIN32

void CUPnPClient::OpnePort( const skyString *name , const char *ip , u16 portNo )
{
	// �܂��b�n�l�̏�����
	HRESULT ret = ::CoInitialize( NULL );

	if ( S_OK != ret )
	{
		SKY_PANIC_MSG( "CoInitialize failed." ); 
	}

	// �C���X�^���X�擾
	IUPnPNAT *nat;
	ret = ::CoCreateInstance( CLSID_UPnPNAT , NULL, CLSCTX_ALL , IID_IUPnPNAT , ( void ** )&nat );

	if ( S_OK != ret )
	{
		SKY_PANIC_MSG( "CoCreateInstance failed." ); 
	}

	// �|�[�g�}�b�s���O�̃��X�g�擾
	IStaticPortMappingCollection *maps = NULL;
	nat->get_StaticPortMappingCollection( &maps );

	if ( maps == skyNull )
	{
		//�Z�L�����e�B�\�t�g��Windows�t�@�C���[�E�H�[���̐ݒ�ł͂�����Ă��邩�A���[�^�[��UPnP�ɑΉ����Ă��Ȃ��BLAN�P�[�u���łȂ����Ă��Ȃ��Ƃ��B
		//�Œ�IP�ɂ���Ƃ��Bhttp://www.akakagemaru.info/port/windows7upnp.html �Ƃ�
		SKY_PANIC_MSG( "get_StaticPortMappingCollection failed." ); 
	}
	else
	{
		// �A�h���X�Ɩ��O��o�^�ł���`���ɕύX
		WCHAR localIPWideString[ 16 ];
		MultiByteToWideChar( CP_ACP , 0 , ip , -1 , localIPWideString , 16 );
		BSTR localIPBstr = SysAllocString( localIPWideString );
		BSTR proto       = SysAllocString( L"TCP" );     // TCP/IP�œo�^����
		BSTR description = SysAllocString( name ); //�����̓|�[�g�}�b�s���O�ɓo�^���閼�O

		//�|�[�g�}�b�s���O��o�^
		IStaticPortMapping *map;
		ret = maps->Add( portNo , proto , portNo , localIPBstr , VARIANT_TRUE , description , &map );

		if ( S_OK != ret )
		{
			SKY_PANIC_MSG( "Add failed." ); 
		}

		// BSTR �̉��
		SysFreeString( description );
		SysFreeString( proto );
		SysFreeString( localIPBstr );

		// �I�u�W�F�N�g���
		map->Release();
		maps->Release();
	}

	nat->Release();

	// �b�n�l�̏I��
	::CoUninitialize();
}

void CUPnPClient::ClosePort( const char *ip , u16 portNo )
{
	// �܂��b�n�l�̏�����
	HRESULT ret = ::CoInitialize( NULL );

	if ( S_OK != ret )
	{
		SKY_PANIC_MSG( "CoInitialize failed." ); 
	}

	// �C���X�^���X�擾
	IUPnPNAT *nat;
	ret = ::CoCreateInstance( CLSID_UPnPNAT , NULL, CLSCTX_ALL , IID_IUPnPNAT , ( void ** )&nat );

	if ( S_OK != ret )
	{
		SKY_PANIC_MSG( "CoCreateInstance failed." ); 
	}

	// �|�[�g�}�b�s���O�̃��X�g�擾
	IStaticPortMappingCollection *maps = NULL;
	nat->get_StaticPortMappingCollection( &maps );

	if ( maps == skyNull )
	{
		//�Z�L�����e�B�\�t�g��Windows�t�@�C���[�E�H�[���̐ݒ�ł͂�����Ă��邩�A���[�^�[��UPnP�ɑΉ����Ă��Ȃ��BLAN�P�[�u���łȂ����Ă��Ȃ��Ƃ��B
		//�Œ�IP�ɂ���Ƃ��Bhttp://www.akakagemaru.info/port/windows7upnp.html �Ƃ�
		SKY_PANIC_MSG( "get_StaticPortMappingCollection failed." ); 
	}
	else
	{
		// �A�h���X�Ɩ��O��o�^�ł���`���ɕύX
		WCHAR localIPWideString[ 16 ];
		MultiByteToWideChar( CP_ACP , 0 , ip , -1 , localIPWideString , 16 );
		BSTR localIPBstr = SysAllocString( localIPWideString );
		BSTR proto = SysAllocString( L"TCP" );     // TCP/IP�œo�^����

		//�|�[�g�}�b�s���O���폜
		ret = maps->Remove( portNo , proto );

		if ( S_OK != ret )
		{
			SKY_PANIC_MSG( "Remove failed." ); 
		}

		// BSTR �̉��
		SysFreeString( proto );
		SysFreeString( localIPBstr );

		// �I�u�W�F�N�g���
		maps->Release();
	}

	nat->Release();

	// �b�n�l�̏I��
	::CoUninitialize();
}

#endif

} } }
