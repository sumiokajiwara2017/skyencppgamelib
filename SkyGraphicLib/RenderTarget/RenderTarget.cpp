#include "StdAfx.h"
#include "SkyGraphicLib/RenderTarget.h"

#include "SkyGraphicLib/Renderer.h"
#include "SkyGraphicLib/Controller.h"

SkyImplementRTTI( sky::lib::graphic::CRenderTarget , sky::lib::graphic::CSceneGraphNode );

//===================================�yCommon�z===================================================>

namespace sky { namespace lib { namespace graphic {

const serialize::CSerializeInterfacePtr& CRenderTarget::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CRenderTargetPtr )spContainer )->m_Size = m_Size;
	( ( CRenderTargetPtr )spContainer )->m_ClearColor = m_ClearColor;
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CRenderTarget::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	Copy( spContainer );
	( ( CRenderTargetPtr )spContainer )->m_pMember = m_pMember;
	( ( CRenderTargetPtr )spContainer )->m_spIsActivated = m_spIsActivated;
	return spContainer;
}

serialize::CSerializeInterfacePtr CRenderTarget::Clone()
{
	return Copy( CRenderTargetPtr( NEW__( CRenderTarget , CRenderTarget() ) ) );
}

serialize::CSerializeInterfacePtr CRenderTarget::CloneSharing()
{
	return CopySharing( Clone() );
}

#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&m_Size.W , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Size.H , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_ClearColor.r , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_ClearColor.g , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_ClearColor.b , sizeof( f32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_ClearColor.a , sizeof( f32 ) , isWrite ); \

skyBool CRenderTarget::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "rendertarget bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN( skyTrue );

	return result;
}

skyBool CRenderTarget::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "rendertarget bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN( skyFalse );

	return result;
}

CRenderTarget::CRenderTarget() : 
m_Size( 0 , 0 ) , 
m_ClearColor( graphic::CColor_DeepBlue ) ,
m_pMember( NEW__( CRenderTargetMember , CRenderTargetMember ) ) ,
m_IsFrameBuffer( skyFalse )
{ 
}
 
CRenderTarget::~CRenderTarget()
{ 
	Release();
}

CTexturePtr CRenderTarget::GetColorBufferTexture()
{
    return m_spColorBufferTexture;
}

//===================================�yDirectX�z===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

void CRenderTargetMember::CreateColorBufferFromSwapChain( CTexturePtr &spColorBufferTexture )
{
	ID3D11RenderTargetView  *pRenderTaget       = skyNull;

	//�e�N�X�`������������J������
	if ( spColorBufferTexture.IsEmpty() )
	{
		//�e�N�X�`���N���X�̃C���X�^���X�𐶐�
		spColorBufferTexture = CTextureCreate_();
	}

//-------------�o�b�N�o�b�t�@�֘A�iWindow�ɕ`�悷�邽�߂̂��́j-------------------��

	//�X���b�v�`�F�C���̃o�b�N�o�b�t�@���擾
	ID3D11Texture2D *pBackBuffer;
	HRESULT hr = m_spSwapChain->GetBuffer( 0 , __uuidof( ID3D11Texture2D ) , ( LPVOID * )&pBackBuffer );    
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "�����_�����O�^�[�Q�b�g�̍쐬�Ɏ��s���܂����B\n" ) );
	}

	D3D11_TEXTURE2D_DESC backbufferDesc;
	pBackBuffer->GetDesc( &backbufferDesc );
	spColorBufferTexture->GetMember()->m_spTexture.SetPtr( pBackBuffer );
	spColorBufferTexture->GetInfo().Width = ( texture::eTexture_with )backbufferDesc.Width;
	spColorBufferTexture->GetInfo().Height = ( texture::eTexture_height )backbufferDesc.Height;

	// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset( &rtvDesc, 0, sizeof( rtvDesc ) );
	rtvDesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;

	//�����_�[�^�[�Q�b�g�̐���
	hr = CDevice_->GetMember()->m_spD3DDevice->CreateRenderTargetView( spColorBufferTexture->GetMember()->m_spTexture.GetPtrNoConst() , &rtvDesc, &pRenderTaget );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "�����_�����O�^�[�Q�b�g�̍쐬�Ɏ��s���܂����B\n" ) );
	}

	//�X���b�v�`�F�C���̃f�B�X�N���v�^�擾
	DXGI_SWAP_CHAIN_DESC desc;
	m_spSwapChain->GetDesc( &desc );

	//�����_�[�^�[�Q�b�g���V�F�[�_�[�Ńe�N�X�`���Ƃ��Ďg�p���邽�߂̃V�F�[�_�[���\�[�X�r���[�𐶐�
	ID3D11ShaderResourceView *pTextureResView_RenderTarget;
	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	ShaderResourceViewDesc.Format						= desc.BufferDesc.Format;
	ShaderResourceViewDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip	= 0;
	ShaderResourceViewDesc.Texture2D.MipLevels			= D3DX11_DEFAULT;
	hr = CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst()->CreateShaderResourceView( spColorBufferTexture->GetMember()->m_spTexture.GetPtrNoConst() , &ShaderResourceViewDesc, &pTextureResView_RenderTarget );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "void CRenderTargetMember::CreateColorBuffer�����s���܂����B\n" ) );
	}
	spColorBufferTexture->GetMember()->m_spShaderResourceView.SetPtr( pTextureResView_RenderTarget );

	//�X�}�[�g�|�C���^�Ǘ��ɂ���
	m_spRenderTargetView.SetPtr( pRenderTaget );
}

 
void CRenderTarget::SetUpFrameBuffer( const CWindowPtr &spWindow )
{
	m_IsFrameBuffer = skyTrue;

	HRESULT hr = S_OK;

	//�T�C�Y�̐ݒ�
	RECT rect;
	GetClientRect( spWindow->GetMember()->GetWindowHndl() , &rect );
	m_Size.W = ( dectype )( rect.right  - rect.left );
	m_Size.H = ( dectype )( rect.bottom - rect.top );

	IDXGISwapChain          *pSwapChain         = skyNull;

	//�v���[���g�p�����[�^�[�̏�����
	DXGI_SWAP_CHAIN_DESC d3DSwapChainDesc;
	CMemoryUtil_::ClearZero( &d3DSwapChainDesc , sizeof( d3DSwapChainDesc ) );

	//�X���b�v�`�F�[���p�p�����[�^�[����
	d3DSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;	// �o�b�N �o�b�t�@�̃t�H�[�}�b�g
	d3DSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;			    // ���t���b�V�����[�g�̕��q
	d3DSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;				// ���t���b�V�����[�g�̕���

	if ( SkyGraphicLibController_::IsFullScreen() )
	{//�t���X�N���[�����[�h

		d3DSwapChainDesc.Windowed = 0;											// �v�h�m�c�n�v���[�h�n�e�e
	}
	else
	{//Window���[�h
		d3DSwapChainDesc.Windowed = 1;											// �v�h�m�c�n�v���[�h�n�m
	}

	d3DSwapChainDesc.BufferDesc.Width                  = ( UINT )m_Size.W;							        // �X���b�v �`�F�[���̃o�b�N �o�b�t�@�̕��ƍ��� (�s�N�Z���P��)
	d3DSwapChainDesc.BufferDesc.Height                 = ( UINT )m_Size.H;							        // �X���b�v �`�F�[���̃o�b�N �o�b�t�@�̕��ƍ��� (�s�N�Z���P��)
	d3DSwapChainDesc.BufferDesc.Format                 = DXGI_FORMAT_R8G8B8A8_UNORM;			// �o�b�N�o�b�t�@�̃t�H�[�}�b�g
	d3DSwapChainDesc.BufferDesc.RefreshRate.Denominator= 1;										// ���t���b�V�����[�g�̕��q
	d3DSwapChainDesc.BufferDesc.RefreshRate.Numerator  = 60;										// ���t���b�V�����[�g�̕���
	d3DSwapChainDesc.BufferDesc.Scaling                = DXGI_MODE_SCALING_CENTERED;			// �X�L�������C���̃p�����[�^�[
	d3DSwapChainDesc.BufferDesc.ScanlineOrdering       = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;		// �X�L�������C���̃p�����[�^�[
	d3DSwapChainDesc.SampleDesc.Count                  = 1;										// �}���`�T���v���̃J�E���g
	d3DSwapChainDesc.SampleDesc.Quality                = 0;										// �}���`�T���v���̕i��
	d3DSwapChainDesc.BufferUsage                       = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;			// �o�b�N�o�b�t�@�̃t���O
	d3DSwapChainDesc.BufferCount                       = 3;										// �o�b�N�o�b�t�@��
	d3DSwapChainDesc.OutputWindow						= spWindow->GetMember()->GetWindowHndl();     // �֘A�t����E�B���h�E�̃n���h��
	d3DSwapChainDesc.Windowed							= 1;										// �E�B���h�E���[�h�ŋN�����邩�ۂ�
	d3DSwapChainDesc.SwapEffect                        = DXGI_SWAP_EFFECT_DISCARD;					// ��ʍX�V�̕��@
	d3DSwapChainDesc.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// �X���b�v�`�F�C���̃t���O

	//�X���b�v�`�F�C���̍쐬
	hr = CDevice_->GetMember()->m_spDXGIFactory->CreateSwapChain( CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst() , &d3DSwapChainDesc , &pSwapChain );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "�����_�����O�^�[�Q�b�g�̍쐬�Ɏ��s���܂����B\n" ) );
	}
	m_pMember->m_spSwapChain.SetPtr( pSwapChain );

	//�o�b�t�@�쐬
	m_pMember->CreateColorBufferFromSwapChain( m_spColorBufferTexture );

	//�t���X�N���[���ݒ������i�ꉞ�j
	if ( SkyGraphicLibController_::IsFullScreen() )
	{
		m_pMember->m_spSwapChain->SetFullscreenState( TRUE , NULL );
	}
	else
	{
		m_pMember->m_spSwapChain->SetFullscreenState( FALSE , NULL );
	}
}

 
void CRenderTarget::SetUpTexture( const math::CBasicSize &size )
{
	m_IsFrameBuffer = skyFalse;

	ID3D11RenderTargetView  *pRenderTaget       = skyNull;

	//�e�N�X�`���N���X�̃C���X�^���X�𐶐�
	m_spColorBufferTexture = CTextureCreate_();

	//�T�C�Y�̐ݒ�
	m_spColorBufferTexture->m_Info.Width  = ( texture::eTexture_with )( u32 )size.W;
	m_spColorBufferTexture->m_Info.Height = ( texture::eTexture_height )( u32 )size.H;

	ID3D11Texture2D *pTexture = skyNull;
	D3D11_TEXTURE2D_DESC desc;
	desc.Width              = ( UINT )size.W;
	desc.Height             = ( UINT )size.H;
	desc.MipLevels          = 1;
	desc.ArraySize          = 1;
	desc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count   = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage              = D3D11_USAGE_DEFAULT;
	desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags     = 0; //�������P���ƃG���[
	desc.MiscFlags          = 0;
	HRESULT	hr = CDevice_->GetMember()->m_spD3DDevice->CreateTexture2D( &desc, 0, &pTexture );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "CTexture::Create( const texture::CTextureInfo &texInfo )�����s���܂����B\n" ) );
	}
	m_spColorBufferTexture->GetMember()->m_spTexture.OverWrite( pTexture ); //�X�}�[�g�|�C���^�̒��g�̃A�h���X���㏑������

	//�T�C�Y�̓e�N�X�`���̃T�C�Y��ݒ�
	m_Size = size;

	//�����_�[�^�[�Q�b�g���V�F�[�_�[�Ńe�N�X�`���Ƃ��Ďg�p���邽�߂̃V�F�[�_�[���\�[�X�r���[�𐶐�
	ID3D11ShaderResourceView *pTextureResView_RenderTarget;
	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	ShaderResourceViewDesc.Format						= desc.Format;
	ShaderResourceViewDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip	= 0;
	ShaderResourceViewDesc.Texture2D.MipLevels			= desc.MipLevels;
	hr = CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst()->CreateShaderResourceView( m_spColorBufferTexture->GetMember()->m_spTexture.GetPtrNoConst() , &ShaderResourceViewDesc, &pTextureResView_RenderTarget );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "void CRenderTargetMember::CreateColorBuffer�����s���܂����B\n" ) );
	}
	m_spColorBufferTexture->GetMember()->m_spShaderResourceView.OverWrite( pTextureResView_RenderTarget );

	// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset( &rtvDesc, 0, sizeof( rtvDesc ) );
	rtvDesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;

	//�����_�[�^�[�Q�b�g�r���[�̐ݒ�
	hr = CDevice_->GetMember()->m_spD3DDevice->CreateRenderTargetView( m_spColorBufferTexture->GetMember()->m_spTexture.GetPtrNoConst() , &rtvDesc, &pRenderTaget );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "�����_�����O�^�[�Q�b�g�̍쐬�Ɏ��s���܂����B\n" ) );
	}
	m_pMember->m_spRenderTargetView.SetPtr( pRenderTaget );
}
 
void CRenderTarget::ClearColorBuffer()
{
	//�J���[�o�b�t�@�̃N���A
	FLOAT clearColor[ 4 ] = { m_ClearColor.r , m_ClearColor.g , m_ClearColor.b , m_ClearColor.a };//RGBA
	CDevice_->GetMember()->m_spD3DDeviceContext->ClearRenderTargetView( m_pMember->m_spRenderTargetView.GetPtrNoConst() , clearColor );
}

 
void CRenderTarget::SetRenderer( const CDepthStencilBufferPtr &spBuffer )
{
	//������
	CDevice_->GetMember()->m_spD3DDeviceContext->OMSetRenderTargets( 0 , NULL , NULL );

	//�n���ꂽ�l�͓����ŕێ������B�}�C�t���[�����K�v�͂Ȃ��B
	ID3D11RenderTargetView* pTbl[] = { m_pMember->m_spRenderTargetView.GetPtrNoConst() };

	//�[�x�X�e���V���o�b�t�@������ΐݒ肷��B�Ȃ��ꍇ�͑O��֌W�����������Ȃ�B
	ID3D11DepthStencilView *pDepthStencil = NULL;
	if ( spBuffer.IsEmpty() == skyFalse )
	{
		pDepthStencil = spBuffer->GetMember()->m_spDepthStencilView.GetPtrNoConst();
	}

	CDevice_->GetMember()->m_spD3DDeviceContext->OMSetRenderTargets( 1, pTbl, pDepthStencil );
}

 
void CRenderTarget::Release()
{
	//����
	CDevice_->GetMember()->m_spD3DDeviceContext->OMSetRenderTargets( 0 , NULL , NULL );

	//�����[�X
	m_pMember->m_spSwapChain.Release();
	m_pMember->m_spRenderTargetView.Release();
	m_spColorBufferTexture.Delete();
}

 
void	CRenderTarget::Present()
{
	if ( m_pMember->m_spSwapChain.IsEmpty() )
	{
//		SKY_PANIC_MSG( _T( "�e�N�X�`�������_�����O�^�[�Q�b�g�͉�ʂɒ��ړ]�����邱�Ƃ��ł��܂���B�V�F�[�_�[�̃p�����[�^�[�Ƃ��Ďg�p���Ă��������B\n" ) );
		return; //�����X�L�b�v����
	}

	HRESULT hr = m_pMember->m_spSwapChain->Present( skyNull , skyNull );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "void CRenderTarget::Present�Ɏ��s���܂����B\n" ) );
	}
}

 
void CRenderTarget::ReSize( u32 newWidth, u32 newHeight )
{
	if ( m_pMember->m_spSwapChain.IsEmpty() )
	{
		SKY_PANIC_MSG( _T( "���̏����̓e�N�X�`�������_�����O�^�[�Q�b�g�ɂ͑Ή����Ă��܂���B�K�v�ȏꍇ�������Ă��������B\n" ) );
		return;
	}

	//�T�C�Y�̐ݒ�
	m_Size.W = ( dectype )newWidth;
	m_Size.H = ( dectype )newHeight;

	//�����_�����O�^�[�Q�b�g�̔j��
	CDevice_->GetMember()->m_spD3DDeviceContext->OMSetRenderTargets( 0 , NULL , NULL );
	m_pMember->m_spRenderTargetView.Release();
	m_spColorBufferTexture->GetMember()->m_spShaderResourceView.Delete();
	m_spColorBufferTexture->GetMember()->m_spTexture.Delete();

	//�X���b�v�`�F�C���̃��T�C�Y�ύX
	DXGI_SWAP_CHAIN_DESC desc;
	m_pMember->m_spSwapChain->GetDesc( &desc );
	m_pMember->m_spSwapChain->ResizeBuffers( desc.BufferCount , ( UINT )m_Size.W , ( UINT )m_Size.H , desc.BufferDesc.Format , 0 );

	//�o�b�t�@�쐬
	m_pMember->CreateColorBufferFromSwapChain( m_spColorBufferTexture );
}

#endif
//===================================�yOpenGL�z====================================================>

/*
�i���傱���ƃ���

�y�o�b�N�o�b�t�@�̒l���e�N�X�`���ɏ������ޏ����z
void glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
�`��ΏۂɂȂ��Ă���t���[���o�b�t�@�̓��e�̈ꕔ���e�N�X�`���������ɃR�s�[���܂��Dtarget �ɂ� GL_TEXTURE_2D ���w�肵�܂��D
level �̓~�b�v�}�b�v�̉𑜓x���x���ŁC0 ���x�[�X�́i�ł��𑜓x�̍����j���x���ŁC�~�b�v�}�b�v���g�p���Ȃ����� 0 ���w�肵�Ă����܂��D
xoffset �� yoffset �̓R�s�[��̃e�N�X�`����̈ʒu���w�肵�܂��Dx �� y �̓R�s�[���̃t���[���o�b�t�@��̈ʒu�ł��Dwidth �� height �̓R�s�[����̈�̕��ƍ����ł��D

�i�g�p��P
glReadBuffer( GL_BACK ); //���͌��w��BGL_BACK�̓o�b�N�o�b�t�@�BglReadBuffer(GL_FRONT)���Ă��Ƃ����炭�t�����g�o�b�t�@
glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 10, 10, 0, 0, 310, 230 );//�������邱�ƂŃe�N�X�`�����ݒ肳��Ă��邱�Ƃɂ��Ȃ�i�����炭�jGL_TEXTURE_2D�͏������ݐ��\��GL_TEXTURE_CUBE_MAP_POSITIVE_X�ȂǂƂ���ƃL���[�u�}�b�v�Ɏg�����肷��
glBegin( GL_QUADS );{
    glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -2.0f, -1.0f,  0.0f );
    glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -2.0f,  1.0f,  0.0f );
    glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  0.0f,  1.0f,  0.0f );
    glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  0.0f, -1.0f,  0.0f );
}glEnd();

�i�g�p��Q 
glReadBuffer( GL_BACK ); //���͌��w��BGL_BACK�̓o�b�N�o�b�t�@�BglReadBuffer(GL_FRONT)���Ă��Ƃ����炭�t�����g�o�b�t�@
glBindTexture( GL_TEXTURE_2D, *�e�N�X�`��ID* );�@//�������ݐ�Ƀe�N�X�`�����w�肵�Ă���
glCopyTexSubImage2D( GL_TEXTURE_2D, 0, 10, 10, 0, 0, 310, 230 );//�������邱�ƂŃe�N�X�`�����ݒ肳��Ă��邱�Ƃɂ��Ȃ�i�����炭�j
glBegin( GL_QUADS );{
    glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -2.0f, -1.0f,  0.0f );
    glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -2.0f,  1.0f,  0.0f );
    glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  0.0f,  1.0f,  0.0f );
    glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  0.0f, -1.0f,  0.0f );
}glEnd();

*�e�N�X�`��ID��*�̃e�N�X�`���Ƀo�b�N�o�b�t�@�̒l����������

*/

#ifdef SKYLIB_RENDER_OPENGL4


void CRenderTarget::Release()
{
    if ( m_pMember->m_FOB.IsEmpty() == skyFalse )
    {
        m_pMember->m_FOB->Dispose();
		m_pMember->m_FOB.Delete();
    }

	if ( m_pMember->m_hDC != skyNull )
	{
		wglMakeCurrent( m_pMember->m_hDC , 0 );
		wglDeleteContext( m_pMember->m_hRC );
		ReleaseDC( m_pMember->m_nWindowHndl, m_pMember->m_hDC );

		if ( m_pMember->m_GlewC != skyNull )
		{
			CGlew::DeleteContext( m_pMember->m_GlewC );
		}
	}
}
 
void CRenderTarget::SetUpFrameBuffer( const CWindowPtr &spWindow )
{
	m_IsFrameBuffer = skyTrue;

	m_pMember->m_nWindowHndl = spWindow->GetMember()->GetWindowHndl(); 

	// �f�o�C�X�R���e�L�X�g�̎擾�A�ݒ�
	m_pMember->m_hDC = GetDC( m_pMember->m_nWindowHndl );

	if( NULL == m_pMember->m_hDC )
	{
        SKY_PANIC_MSG( _T( "It failed in making the device context. \n" ) );
	}

	PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR),   // size of this pfd 
		1,                     // version number 
		PFD_DRAW_TO_WINDOW |   // support window 
		PFD_SUPPORT_OPENGL |   // support OpenGL 
		PFD_DOUBLEBUFFER,      // double buffered 
		PFD_TYPE_RGBA,         // RGBA type 
		24,                    // 24-bit color depth 
		0, 0, 0, 0, 0, 0,      // color bits ignored 
		0,                     // no alpha buffer 
		0,                     // shift bit ignored 
		0,                     // no accumulation buffer //�A�L�������[�V�����o�b�t�@���g�p����Ƃ��͂������g�p���悤�I( 24 )?
		0, 0, 0, 0,            // accum bits ignored     //�A�L�������[�V�����o�b�t�@���g�p����Ƃ��͂������g�p���悤�I( 8 , 8 , 8 , 0 )?
		32,                    // 32-bit z-buffer 
		0,                     // no stencil buffer      //�X�e���V���o�b�t�@���g�p����Ƃ��͂������w�肵�悤�I
		0,                     // no auxiliary buffer 
		PFD_MAIN_PLANE,        // main layer 
		0,                     // reserved 
		0, 0, 0                // layer masks ignored 
	}; 

	int iPixelFormat;
	if( 0 == ( iPixelFormat = ::ChoosePixelFormat( m_pMember->m_hDC, &pfd ) ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	if( TRUE != ::SetPixelFormat( m_pMember->m_hDC, iPixelFormat, &pfd ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	// �����_�����O�R���e�L�X�g�̍쐬
	if( 0 == ( m_pMember->m_hRC = ::wglCreateContext( m_pMember->m_hDC ) ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

	// �����_�����O�R���e�L�X�g���J�����g�̃f�o�C�X�R���e�L�X�g�ɐݒ�
	if( TRUE != ::wglMakeCurrent( m_pMember->m_hDC, m_pMember->m_hRC ) )
	{
		SKY_PANIC_MSG( _T( "\n" ) );
	}

#ifndef GLEW_MX

   // GLEW������
   CGlew::Initialize();

#else

	m_pMember->m_GlewC = CGlew::CreateContext();
	CGlew::Initialize( m_pMember->m_GlewC );

#endif

	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *vender = glGetString(GL_VENDOR);
	const GLubyte *renderer = glGetString(GL_RENDERER);

	SKY_PRINTF( "OpenGL   %s \n" , version );
	SKY_PRINTF( "Vender   %s \n" , vender );
	SKY_PRINTF( "Renderer %s \n" , renderer );

	m_pMember->m_IsFrameBuffer = skyTrue;
}

void CRenderTarget::SetUpTexture( const math::CBasicSize &size )
{
	m_IsFrameBuffer = skyFalse;

	m_pMember->m_IsFrameBuffer = skyFalse;

	m_pMember->m_FOB = CFramebufferObject::Create();
	m_pMember->m_FOB->Initialize( size );

	//�e�N�X�`���N���X�̃C���X�^���X�𐶐�
	m_spColorBufferTexture = CTextureCreate_();
	m_spColorBufferTexture->GetMember()->texId[ 0 ] = m_pMember->m_FOB->GetTextureId();
	m_spColorBufferTexture->GetMember()->m_IsLoad = skyTrue;
	m_spColorBufferTexture->GetInfo().Width = ( texture::eTexture_with )( u32 )size.W;
	m_spColorBufferTexture->GetInfo().Height = ( texture::eTexture_height )( u32 )size.H;
}
 
void CRenderTarget::ClearColorBuffer()
{
	// �N���A�J���[�̐ݒ�
	glClearColor( m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a );

	//�J���[�o�b�t�@�Ƃ̃N���A
	glClear( GL_COLOR_BUFFER_BIT );
}

 
void CRenderTarget::SetRenderer( const CDepthStencilBufferPtr &spBuffer )
{
	SKY_UNUSED_ARG( spBuffer );

	if ( m_pMember->m_IsFrameBuffer == skyTrue )
	{
		// �h���C�o�Ƀo�O�̂���O���t�B�b�N�J�[�h�ɂ����Ă�SDI�̍ŏ��̕`��ƁA
		// ���ׂẴO���t�B�b�N�J�[�h�ɂ����Ă�MDI�`��ɂ�����
		// �ȉ��̖��߂́A���ʂ𔭊�����B
		wglMakeCurrent( m_pMember->m_hDC, m_pMember->m_hRC );
		CGlew::SetCurrentContext( m_pMember->m_GlewC );
	}
	else
	{
		m_pMember->m_FOB->PreRender();

		//�[�x�o�b�t�@�̃N���A
		glClear( GL_DEPTH_BUFFER_BIT );
	}
}

 
void CRenderTarget::Present()
{
	if ( m_pMember->m_IsFrameBuffer == skyTrue )
	{
		//�o�b�N�o�b�t�@���t�����g��
		SwapBuffers( m_pMember->m_hDC );
	}
	else
	{
		m_pMember->m_FOB->PostRender();
	}
}

void CRenderTarget::ReSize( u32 newWidth, u32 newHeight )
{
	SKY_UNUSED_ARG( newWidth );
	SKY_UNUSED_ARG( newHeight );
}

#endif

//===================================�yCommon�z====================================================>

skyBool CRenderTarget::IsFrameBufferRenderTarget()
{
	return m_IsFrameBuffer;
}

void CRenderTarget::vRendering( CRenderingParam &param )
{
    SKY_ASSERT_MSG( this->GetRefCnt() > 0 , _T( "This instance must use smartpointer. \n" ) );

    //�����������_�[�f�[�^�̕`��i�`���Ԃ𕪂���V�[���O���t�m�[�h�v�f���I���Ƃ��ɔ��������o�͂���j
    param.AlphaDataRendering();

    //�������X�^�b�N�ɐς�
    param.RenderTargetStack.Push( thisPtr( CRenderTargetPtr ) ); //���Ƃ��Ƃ���this�����̃X�}�[�g�|�C���^�ɊǗ�����Ă����ꍇ�iRefCount��0�ȏ�̏ꍇ)���̎󂯓n�������Ă���肪�������Ȃ��B

    //�����̃J���[�o�b�t�@���N���A
    CRenderCommand::ClearColorBuffer( param.RenderTargetStack.GetTop() );

    //�����������_�����O�^�[�Q�b�g�ɓK�p����i�^�[�Q�b�g���ꎞ�u��������j
    CRenderCommand::SetRenderTarget( param.RenderTargetStack.GetTop() , param.DepthStencilBufferStack.GetTop() );
}


void CRenderTarget::vPostRendering( CRenderingParam &param )
{
    //�����������_�[�f�[�^�̕`��i�`���Ԃ𕪂���V�[���O���t�m�[�h�v�f���I���Ƃ��ɔ��������o�͂���j
    param.AlphaDataRendering();

    //�����_�����O�^�[�Q�b�g���f�B�X�v���C�ɕ\������
    CRenderCommand::SendDisplayRenderTarget( thisPtr( CRenderTargetPtr ) );

    //�������X�^�b�N�������
    param.RenderTargetStack.Pop();

	if ( param.RenderTargetStack.GetNum() > 0 )
	{
		//�ꎞ�u���������^�[�Q�b�g�����ɖ߂�
		CRenderCommand::SetRenderTarget( param.RenderTargetStack.GetTop() , param.DepthStencilBufferStack.GetTop() );
	}
}

} } }
