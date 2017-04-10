#include "StdAfx.h"
#include "SkyGraphicLib/Texture.h"

SkyImplementRTTI( sky::lib::graphic::CTexture , sky::lib::graphic::CSceneGraphNode );

//===================================�yCommon�z===================================================>

namespace sky { namespace lib { namespace graphic {

const serialize::CSerializeInterfacePtr& CTexture::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CTexture::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	Copy( spContainer );
	( ( CTexturePtr )spContainer )->m_pMember = m_pMember;
	( ( CTexturePtr )spContainer )->m_spIsActivated = m_spIsActivated;
	return spContainer;
}

serialize::CSerializeInterfacePtr CTexture::Clone()
{
	return Copy( Create( hash::CHashStringPtr() ) );
}

serialize::CSerializeInterfacePtr CTexture::CloneSharing()
{
	return CopySharing( Clone() );
}

void CTexture::Activate()
{
	ACTIVATE_COMMON();
	Load();
}

void CTexture::Deactivate()
{
	DEACTIVATE_COMMON();
	UnLoad();
}

CTexture::~CTexture()
{
}

} } }

//===================================�yDirectX�z===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

/*

���̋@�\�͐F�X�������ł��B��������K�v�����闝�R���ł���܂ł��̂܂܂ɂ��Ă����܂��B

*/

namespace sky { namespace lib { namespace graphic {

//-----------------Util-------------------��

template < class T >
static void ConvertTexInfo_(  const texture::CTextureInfo &src , T &dest )
{
	//���擾
	dest.Width  = src.Width;

	//�����擾
	dest.Height = src.Height;

	//�����t�H�[�}�b�g
	if ( src.Format == texture::eTextureFormat_R8G8B8 )
	{ dest.Format = DXGI_FORMAT_R8G8B8A8_UNORM; }
	else if ( src.Format == texture::eTextureFormat_A8R8G8B8 )
	{ dest.Format = DXGI_FORMAT_R8G8B8A8_UNORM; }
	else if ( src.Format == texture::eTextureFormat_A32R32G32B32 )
	{ dest.Format = DXGI_FORMAT_BC1_UNORM; }
	else if ( src.Format == texture::eTextureFormat_P8 )
	{ dest.Format = DXGI_FORMAT_R8G8B8A8_UNORM; }
	else if ( src.Format == texture::eTextureFormat_X8R8G8B8 )
	{ dest.Format = DXGI_FORMAT_R8G8B8A8_UNORM; }

	//�~�b�v�}�b�v��
	dest.MipLevels = src.MipLevels;
}
template < class T >
static void ConvertTexInfo_( const T &src , texture::CTextureInfo &dest )
{
	//���擾
	if ( src.Width == ( UINT )texture::eTexture_with_1
	  || src.Width == ( UINT )texture::eTexture_with_2
	  || src.Width == ( UINT )texture::eTexture_with_4 
	  || src.Width == ( UINT )texture::eTexture_with_8 
      || src.Width == ( UINT )texture::eTexture_with_16
      || src.Width == ( UINT )texture::eTexture_with_32
      || src.Width == ( UINT )texture::eTexture_with_64
      || src.Width == ( UINT )texture::eTexture_with_128
      || src.Width == ( UINT )texture::eTexture_with_256
      || src.Width == ( UINT )texture::eTexture_with_512
      || src.Width == ( UINT )texture::eTexture_with_768
      || src.Width == ( UINT )texture::eTexture_with_1024 )
	{ dest.Width = ( texture::eTexture_with )src.Width; } 
	else
	{ SKY_PANIC_MSG( _T( "The width of the texture is illegal." ) ); } //�e�N�X�`���̉������s���ł��B

	//�����擾
	if ( src.Height == ( UINT )texture::eTexture_height_1
	  || src.Height == ( UINT )texture::eTexture_height_2
      || src.Height == ( UINT )texture::eTexture_height_4 
      || src.Height == ( UINT )texture::eTexture_height_8 
      || src.Height == ( UINT )texture::eTexture_height_16
      || src.Height == ( UINT )texture::eTexture_height_32
      || src.Height == ( UINT )texture::eTexture_height_64
      || src.Height == ( UINT )texture::eTexture_height_128
      || src.Height == ( UINT )texture::eTexture_height_256
      || src.Height == ( UINT )texture::eTexture_height_512
      || src.Height == ( UINT )texture::eTexture_height_768
      || src.Height == ( UINT )texture::eTexture_height_1024 )
	{ dest.Height = ( texture::eTexture_height )src.Height; } 
	else
	{ SKY_PANIC_MSG( _T( "The width of length of the texture is illegal." ) ); } //�e�N�X�`���̏c�����s���ł��B

	//�����t�H�[�}�b�g
	if ( src.Format == DXGI_FORMAT_R8G8B8A8_UNORM )
	{ 
		dest.Format = texture::eTextureFormat_R8G8B8;
	}
	else if ( src.Format == DXGI_FORMAT_R8G8B8A8_UNORM )
	{ 
		dest.Format = texture::eTextureFormat_A8R8G8B8;
	}
	else if ( src.Format == DXGI_FORMAT_R8G8B8A8_UNORM )
	{
		dest.Format = texture::eTextureFormat_A32R32G32B32;
	}
	else if ( src.Format == DXGI_FORMAT_BC1_UNORM )
	{
		dest.Format = texture::eTextureFormat_P8;
	}
	else if ( src.Format == DXGI_FORMAT_BC2_UNORM )
	{
		dest.Format = texture::eTextureFormat_P8;
	}
	else if ( src.Format == DXGI_FORMAT_R8G8B8A8_UNORM )
	{
		dest.Format = texture::eTextureFormat_X8R8G8B8;
	}
	else
	{
		SKY_PANIC_MSG( _T( "The file format of the texture is illegal." ) ); //�e�N�X�`���̃t�@�C���t�H�[�}�b�g���s���ł��B
	}

	//�~�b�v�}�b�v��
	dest.MipLevels = src.MipLevels;
}

//-----------------Util-------------------��

CTexture::CTexture() : m_pMember( NEW__( CTextureMember , CTextureMember() ) )
{
	//�e�N�X�`���T���v���X�e�[�g�̍쐬
	D3D11_SAMPLER_DESC SamlerDesc = {
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,		// D3D11_FILTER Filter;
		D3D11_TEXTURE_ADDRESS_WRAP,				// D3D11_TEXTURE_ADDRESS_MODE AddressU;
		D3D11_TEXTURE_ADDRESS_WRAP,				// D3D11_TEXTURE_ADDRESS_MODE AddressV;
		D3D11_TEXTURE_ADDRESS_WRAP,				// D3D11_TEXTURE_ADDRESS_MODE AddressW;
		0.0f,									// FLOAT MipLODBias;
		1,										// UINT MaxAnisotropy;
		D3D11_COMPARISON_ALWAYS,				// D3D11_COMPARISON_FUNC ComparisonFunc;
		{ 1.0f, 1.0f, 1.0f, 1.0f },				// FLOAT BorderColor[ 4 ];
		0.0f,									// FLOAT MinLOD;
		D3D11_FLOAT32_MAX,						// FLOAT MaxLOD;
	};
	m_pMember->m_SamplerStateDesc = SamlerDesc;
	m_pMember->ReCreateSamplerState();
}

void CTextureMember::ReCreateSamplerState()
{
	//���݂̏����������
	if ( m_spTextureSamplerState.IsEmpty() == skyFalse ) 
	{
		m_spTextureSamplerState.Release();
	}

	//�V�����X�e�[�g�����Ȃ���
	ID3D11SamplerState  *pSamlerState = NULL;
	HRESULT hr = CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst()->CreateSamplerState( &m_SamplerStateDesc, &pSamlerState );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "�e�N�X�`���T���v���[�X�e�[�g \n" ) );
	};
	m_spTextureSamplerState.SetPtr( pSamlerState );
}

void CTexture::SetTextureWrapRepeat()
{
	//�ύX���������������ύX����
	m_pMember->m_SamplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	m_pMember->m_SamplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	m_pMember->m_SamplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	//�č쐬
	m_pMember->ReCreateSamplerState();
}

void CTexture::SetTextureWrapClamp()
{
	//�ύX���������������ύX����
	m_pMember->m_SamplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	m_pMember->m_SamplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	m_pMember->m_SamplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	//�č쐬
	m_pMember->ReCreateSamplerState();
}

void CTexture::SetTextureWrapEdge()
{
	//�ύX���������������ύX����
	m_pMember->m_SamplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	m_pMember->m_SamplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	m_pMember->m_SamplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	//�č쐬
	m_pMember->ReCreateSamplerState();
}

void CTexture::SetTextureWrapBorder()
{
	//�ύX���������������ύX����
	m_pMember->m_SamplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	m_pMember->m_SamplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	m_pMember->m_SamplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;

	//�č쐬
	m_pMember->ReCreateSamplerState();
}

void CTexture::SetTextureWrapBorderColor( CColor color )
{
	//�ύX���������������ύX����
	m_pMember->m_SamplerStateDesc.BorderColor[ 0 ] = color.r;
	m_pMember->m_SamplerStateDesc.BorderColor[ 1 ] = color.g;
	m_pMember->m_SamplerStateDesc.BorderColor[ 2 ] = color.b;
	m_pMember->m_SamplerStateDesc.BorderColor[ 3 ] = color.a;

	//�č쐬
	m_pMember->ReCreateSamplerState();
}

void CTexture::Load()
{//����

	if ( m_pMember->m_spTexture.IsEmpty() == skyTrue )
	{
		if ( CPathUtil_::IsFileOn( Name.GetName() ) == skyFalse )
		{
			SKY_PANIC_MSG( _T( "The file did not exist. " ) ); //�e�N�X�`���t�@�C����ǂݍ������Ƃ��܂������A�t�@�C�������݂��܂���ł����B
			return;
		}

		stream::IStreamPtr inStream = CFileUtil_::LoadFile( Name.GetName() );

		SKY_PRINTF( _T( "%s \n" ) , Name.GetString() );

		//�e�N�X�`����ǂ�
		inStream->vOpen();
		Load( inStream );
		inStream->vClose();
	}
}

void CTexture::Load( const stream::IStreamPtr& stream )
{
	ID3D11Resource* pTexture = NULL;

	if ( m_pMember->m_spTexture.IsEmpty() == skyTrue )
	{
		//�e�N�X�`����ǂݍ���
		HRESULT hr = D3DX11CreateTextureFromMemory
		( CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst() ,
		  stream->vGetData() ,
		  stream->vGetSize() ,
		  NULL ,
		  NULL ,
		  &pTexture ,
		  NULL );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "CTexture::Load( const stream::IStreamPtr& stream )�����s���܂����B\n" ) );
		}

		//�e�N�X�`�����擾
		D3DX11_IMAGE_INFO imageInfo;
		hr = D3DX11GetImageInfoFromMemory
		( stream->vGetData() ,
		  stream->vGetSize() ,
		  NULL ,
		  &imageInfo ,
		  NULL );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "CTexture::Load( const stream::IStreamPtr& stream )�����s���܂����B\n" ) );
		}

		//�V�F�[�_�[���\�[�X�r���[�𐶐�
		ID3D11ShaderResourceView *pTextureResView;
		D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
		ShaderResourceViewDesc.Format						= imageInfo.Format;
		ShaderResourceViewDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
		ShaderResourceViewDesc.Texture2D.MostDetailedMip	= 0;
		ShaderResourceViewDesc.Texture2D.MipLevels			= D3DX11_DEFAULT;
		hr = CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst()->CreateShaderResourceView( pTexture , &ShaderResourceViewDesc, &pTextureResView );

		//�e�N�X�`������ݒ肷��
		ConvertTexInfo_< D3DX11_IMAGE_INFO >( imageInfo , m_Info );

		//�X�}�[�g�|�C���^�ɐݒ�
		m_pMember->m_spTexture.SetPtr( pTexture );
		m_pMember->m_spShaderResourceView.SetPtr( pTextureResView );
	}
}

void CTexture::Create( const texture::CTextureInfo &texInfo )
{
	SKY_ASSERT_MSG( m_pMember->m_spTexture.IsEmpty() == skyFalse , _T( "���łɍ쐬�ς݂ł��B�쐬���Ȃ����Ƃ���UnLoad()�����s���Ă��������B\n" ) );

	D3D11_TEXTURE2D_DESC desc;
	ID3D11Texture2D *pTexture = skyNull;

	//���͒l���c�R�c�p�ɕϊ�
	ConvertTexInfo_< D3D11_TEXTURE2D_DESC >( texInfo , desc );

	//�Œ�l�i�����̒l���O������J�X�^�}�C�Y�\�ɂ���ɂ�texture::CTextureInfo�ɍ��ڂ𑝂₵��ConvertTexInfo_�̒��Ŏ󂯓n�������������Ȃ��Ƃ����Ȃ��j
	desc.ArraySize          = 1;
	desc.SampleDesc.Count   = 1;                          // �T���v�����O��1�s�N�Z���̂�
	desc.SampleDesc.Quality = 0;
//	desc.Usage              = D3D11_USAGE_DYNAMIC;        // CPU�������݉\
//	desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE; // �V�F�[�_���\�[�X
//	desc.CPUAccessFlags     = D3D11_CPU_ACCESS_WRITE;     // CPU���珑�����݃A�N�Z�X��
	desc.Usage				= D3D11_USAGE_DEFAULT;
	desc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags	    = 0;
	desc.MiscFlags          = 0;

	HRESULT	hr = CDevice_->GetMember()->m_spD3DDevice->CreateTexture2D( &desc, 0, &pTexture );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "CTexture::Create( const texture::CTextureInfo &texInfo )�����s���܂����B\n" ) );
	}

	//�V�F�[�_�[���\�[�X�r���[�𐶐�
	ID3D11ShaderResourceView *pTextureResView;
	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	ShaderResourceViewDesc.Format						= desc.Format;
	ShaderResourceViewDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip	= 0;
	ShaderResourceViewDesc.Texture2D.MipLevels			= D3DX11_DEFAULT;
	hr = CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst()->CreateShaderResourceView( pTexture , &ShaderResourceViewDesc, &pTextureResView );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "CTexture::Create( const texture::CTextureInfo &texInfo )�����s���܂����B\n" ) );
	}

	//�e�N�X�`�������i�[
	m_Info = texInfo;

	//�X�}�[�g�|�C���^�ɐݒ�
	m_pMember->m_spTexture.SetPtr( pTexture );
	m_pMember->m_spShaderResourceView.SetPtr( pTextureResView );
}


void CTexture::SetShader()
{
	SKY_ASSERT_MSG( m_pMember->m_spTexture.IsEmpty() == skyFalse , _T( "This Texture not Load yet." ) );

	//�T���v���[��ݒ�
	ID3D11SamplerState* pSamplerTbl[] = { m_pMember->m_spTextureSamplerState.GetPtrNoConst() };
	CDevice_->GetMember()->m_spD3DDeviceContext->PSSetSamplers( 0, 1, pSamplerTbl );

	//�e�N�X�`���ւ̎Q�Ƃ�ݒ�
	ID3D11ShaderResourceView* pResViewTbl[] = { m_pMember->m_spShaderResourceView.GetPtrNoConst() };
    CDevice_->GetMember()->m_spD3DDeviceContext->PSSetShaderResources( 0  , 1 , pResViewTbl );
}


void CTexture::UnLoad()
{
	//�e�N�X�`����j��
	m_pMember->m_spTexture.Delete();
	m_pMember->m_spShaderResourceView.Delete();
}

//�ϊ��e�[�u��
D3DX11_IMAGE_FILE_FORMAT d3dformat[] = 
{
    D3DX11_IFF_BMP ,
    D3DX11_IFF_JPG ,
    D3DX11_IFF_PNG ,
    D3DX11_IFF_DDS ,
    D3DX11_IFF_TIFF ,
    D3DX11_IFF_GIF ,
    D3DX11_IFF_WMP ,
};


void CTexture::Save( texture::eTextureFileFormat eFileFormat )
{
	Save( eFileFormat , Name.GetName() );
}


void CTexture::Save(  texture::eTextureFileFormat eFileFormat , const hash::CHashStringPtr &path )
{
	HRESULT hr;

	// �}���`�T���v�����O���ꂽ���\�[�X���}���`�T���v�����O����Ă��Ȃ����\�[�X�ɃR�s�[����
	ID3D11Texture2D * pBackBuffer = ( ID3D11Texture2D * )m_pMember->m_spTexture.GetPtrNoConst();
	ID3D11Texture2D* pTexture = NULL;
	D3D11_TEXTURE2D_DESC texDesc;
	pBackBuffer->GetDesc( &texDesc );
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.SampleDesc.Count   = 1; //������
	texDesc.SampleDesc.Quality = 0; //���������̒l�Ȃ�R�s�[�͖{���K�v�Ȃ��B�ł����ׂăR�s�[���邱�Ƃŉ���

	// �R�s�[��p�̃e�N�X�`���[���쐬����B
	hr = CDevice_->GetMember()->m_spD3DDevice->CreateTexture2D( &texDesc, NULL, &pTexture );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "void CTexture::Save�����s���܂����B\n" ) );
	}

	CDevice_->GetMember()->m_spD3DDeviceContext->ResolveSubresource( pTexture, NULL, pBackBuffer, NULL, texDesc.Format );

	// �t�@�C���o��
	hr = D3DX11SaveTextureToFile( CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst() , pTexture , d3dformat[ eFileFormat ] , path );

	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "void CTexture::Save�����s���܂����B\n" ) );
	}
}

skyBool CTexture::IsLoaded ()
{
	return ( m_pMember->m_spTexture.IsEmpty() == skyTrue ) ? skyFalse : skyTrue;
}

} } }

#endif

//===================================�yOpenGL�z====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

CTexture::CTexture() : m_pMember( NEW__( CTextureMember , CTextureMember() ) )
{
}

void CTexture::Load()
{//����

	if ( m_pMember->m_IsLoad == skyFalse )
	{
		//�@�e�N�X�`���̃��[�h
#ifdef UNICODE
		graphic::CreateTextureFromFileW( Name.GetName()->GetString() , m_pMember->m_TexObj );
#else
		graphic::CreateTextureFromFileA( Name.GetName()->GetString() , m_pMember->m_TexObj );
#endif

		m_pMember->texId[ 0 ] = m_pMember->m_TexObj.GetHandle();

		m_Info.Width = ( texture::eTexture_with )m_pMember->m_TexObj.m_Width;
		m_Info.Height = ( texture::eTexture_height )m_pMember->m_TexObj.m_Height;
	}
}

void CTexture::Load( const stream::IStreamPtr& stream )
{
	SKY_UNUSED_ARG( stream );
}

void CTexture::Create( const texture::CTextureInfo &texInfo )
{
	SKY_UNUSED_ARG( texInfo );
}

void CTexture::SetShader()
{
}

void CTexture::UnLoad()
{
	m_pMember->m_IsLoad = skyFalse;
}

void CTexture::Save( texture::eTextureFileFormat eFileFormat )
{
	SKY_UNUSED_ARG( eFileFormat );
}

void CTexture::Save(  texture::eTextureFileFormat eFileFormat , const hash::CHashStringPtr &path )
{
	SKY_UNUSED_ARG( eFileFormat );
	SKY_UNUSED_ARG( path );
}

skyBool CTexture::IsLoaded ()
{
	return m_pMember->m_TexObj.IsLoaded();
}

void CTexture::SetTextureWrapRepeat()
{
	glBindTexture( GL_TEXTURE_2D , m_pMember->texId[ 0 ] );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T, GL_REPEAT );
	glBindTexture( GL_TEXTURE_2D , 0);
}

void CTexture::SetTextureWrapClamp()
{
	glBindTexture( GL_TEXTURE_2D , m_pMember->texId[ 0 ] );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T, GL_CLAMP );
	glBindTexture( GL_TEXTURE_2D , 0);
}

void CTexture::SetTextureWrapEdge()
{
	glBindTexture( GL_TEXTURE_2D , m_pMember->texId[ 0 ] );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glBindTexture( GL_TEXTURE_2D , 0);
}

void CTexture::SetTextureWrapBorder()
{

	glBindTexture( GL_TEXTURE_2D , m_pMember->texId[ 0 ] );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glBindTexture( GL_TEXTURE_2D , 0);
}

void CTexture::SetTextureWrapBorderColor( CColor color )
{
	glBindTexture( GL_TEXTURE_2D , m_pMember->texId[ 0 ] );
	f32 colorData[ 3 ];
	color.Copyf32Array3RGB( colorData );
	glTexParameterfv( GL_TEXTURE_2D , GL_TEXTURE_BORDER_COLOR , colorData );
	glBindTexture( GL_TEXTURE_2D , 0 );
}

} } }

#endif

