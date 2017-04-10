#include "StdAfx.h"
#include "SkyGraphicLib/DepthStencil.h"

#include "SkyGraphicLib/Renderer.h"

SkyImplementRTTI( sky::lib::graphic::CDepthStencilBuffer , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

//===================================�yCommon�z====================================================>

void CDepthStencilBuffer::vRendering( CRenderingParam &param )
{
    SKY_ASSERT_MSG( this->GetRefCnt() > 0 , _T( "This instance must use smartpointer. \n" ) );

	//�����������_�[�f�[�^�̕`��i�`���Ԃ𕪂���V�[���O���t�m�[�h�v�f���I���Ƃ��ɔ��������o�͂���j
	param.AlphaDataRendering();

	//�������X�^�b�N�ɐς�
	param.DepthStencilBufferStack.Push( thisPtr( CDepthStencilBufferPtr ) ); //���Ƃ��Ƃ���this�����̃X�}�[�g�|�C���^�ɊǗ�����Ă����ꍇ�iRefCount��0�ȏ�̏ꍇ)���̎󂯓n�������Ă���肪�������Ȃ��B

	//�����_�����O�^�[�Q�b�g�͂��̂܂܂ɁA������[�x�o�b�t�@�ɂ���
	CRenderCommand::SetRenderTarget( param.RenderTargetStack.GetTop() , param.DepthStencilBufferStack.GetTop() );

	//�������g���N���A����
	CRenderCommand::ClearDepthStencilBuffer( param.DepthStencilBufferStack.GetTop() );
}

void CDepthStencilBuffer::vPostRendering( CRenderingParam &param )
{
	//�����������_�[�f�[�^�̕`��i�`���Ԃ𕪂���V�[���O���t�m�[�h�v�f���I���Ƃ��ɔ��������o�͂���j
	param.AlphaDataRendering();

	//�������X�^�b�N����O��
	param.DepthStencilBufferStack.Pop();

	if ( param.RenderTargetStack.GetNum() > 0 )
	{
		//�����_�����O�^�[�Q�b�g����O��
		CRenderCommand::SetRenderTarget( param.RenderTargetStack.GetTop() , param.DepthStencilBufferStack.GetTop() );
	}
	else
	{
		//�[�x�X�e���V���o�b�t�@���Ȃ���ԂŃ����_�����O
		CRenderCommand::SetRenderTarget( param.RenderTargetStack.GetTop() , CDepthStencilBufferPtr() );
	}
}

CTexturePtr CDepthStencilBuffer::GetDepthStencilTexture()
{
	return m_spTexture;
}

const serialize::CSerializeInterfacePtr& CDepthStencilBuffer::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CDepthStencilBufferPtr )spContainer )->m_Size = m_Size;
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CDepthStencilBuffer::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CDepthStencilBuffer::Clone()
{
	return Copy( CDepthStencilBufferPtr( NEW__( CDepthStencilBuffer , CDepthStencilBuffer() ) ) );
}

serialize::CSerializeInterfacePtr CDepthStencilBuffer::CloneSharing()
{
	return Clone();
}

#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&m_Size.W , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Size.H , sizeof( d64 ) , isWrite ); \

skyBool CDepthStencilBuffer::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "depthstencilbuffer bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN( skyTrue );

	return result;
}

skyBool CDepthStencilBuffer::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "depthstencilbuffer bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN( skyFalse );

	return result;
}


void CDepthStencilBuffer::Activate()
{
	ACTIVATE_COMMON();
}

void CDepthStencilBuffer::Deactivate()
{
	DEACTIVATE_COMMON();
}

//===================================�yDirectX�z===================================================>
#ifdef SKYLIB_RENDER_DIRECTX11

CDepthStencilBuffer::CDepthStencilBuffer()
{
	m_pMember = NEW__( CDepthStencilBufferMember , CDepthStencilBufferMember() );
}


CDepthStencilBuffer::~CDepthStencilBuffer()
{
	DEL m_pMember;
}

void CDepthStencilBuffer::SetUp( const math::CBasicSize &size )
{
	m_Size = size;

    ID3D11Texture2D         *pDepthStencil      = skyNull;
    ID3D11DepthStencilView  *pDepthStencilView  = skyNull;

	//�e�N�X�`�����Ȃ���΍��
	if ( m_spTexture.IsEmpty() )
	{
		m_spTexture = CTextureCreate_();
	}

	//�e�N�X�`���N���X�̃C���X�^���X�𐶐�
	m_spTexture = CTextureCreate_();

//-------------�[�x�o�b�t�@�֘A-----------------------------------------------------��

	// �[�x�^�X�e���V���p�e�N�X�`���̍쐬
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	CMemoryUtil_::ClearZero( &depthStencilDesc , sizeof( depthStencilDesc ) );
	depthStencilDesc.Width              = ( UINT )size.W;
	depthStencilDesc.Height             = ( UINT )size.H;
	depthStencilDesc.MipLevels          = 1;
	depthStencilDesc.ArraySize          = 1;
	depthStencilDesc.Format             = DXGI_FORMAT_R24G8_TYPELESS; //������DXGI_FORMAT_D24_UNORM_S8_UINT���ƃG���[( D3D11_BIND_SHADER_RESOURCE ���w�肳��Ă��邽�ߊO���ƃG���[�͂��Ȃ��j
	depthStencilDesc.SampleDesc.Count   = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage              = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags          = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags     = 0; //�������P���ƃG���[
	depthStencilDesc.MiscFlags          = 0;
	HRESULT hr = CDevice_->GetMember()->m_spD3DDevice->CreateTexture2D( &depthStencilDesc , NULL , &pDepthStencil );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "�����_�����O�^�[�Q�b�g�̍쐬�Ɏ��s���܂����B\n" ) );
	}
	m_spTexture->GetMember()->m_spTexture.SetPtr( pDepthStencil ); //���g��u��������
	
	// �[�x�^�X�e���V���r���[�쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC spDepthStencilViewDesc;
	CMemoryUtil_::ClearZero( &spDepthStencilViewDesc , sizeof( spDepthStencilViewDesc ) );	
	spDepthStencilViewDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	spDepthStencilViewDesc.ViewDimension      = D3D11_DSV_DIMENSION_TEXTURE2D;
	spDepthStencilViewDesc.Flags              = 0;
	spDepthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = CDevice_->GetMember()->m_spD3DDevice->CreateDepthStencilView( pDepthStencil , &spDepthStencilViewDesc , &pDepthStencilView );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "�����_�����O�^�[�Q�b�g�̍쐬�Ɏ��s���܂����B\n" ) );
	}

	//�X�}�[�g�|�C���^�Ǘ��ɂ���
	m_pMember->m_spDepthStencilView.SetPtr( pDepthStencilView );

	// �[�x�^�X�e���V�����V�F�[�_�[�̃p�����[�^�[�Ƃ��Ďg�p�ł���r���[�̍쐬
	ID3D11ShaderResourceView *pTextureResView_DepthStencil;
	D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceViewDesc;
	ShaderResourceViewDesc.Format						= DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	ShaderResourceViewDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MostDetailedMip	= 0;
	ShaderResourceViewDesc.Texture2D.MipLevels			= D3DX11_DEFAULT;
	hr = CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst()->CreateShaderResourceView( m_spTexture->GetMember()->m_spTexture.GetPtrNoConst() , &ShaderResourceViewDesc, &pTextureResView_DepthStencil );
	if ( FAILED( hr ) )
	{
		D3DUtil::PrintD3D11Result( hr );
		SKY_PANIC_MSG( _T( "void CRenderTargetMember::CreateColorBuffer�����s���܂����B\n" ) );
	}
	m_spTexture->GetMember()->m_spShaderResourceView.SetPtr( pTextureResView_DepthStencil );  //���g��u��������
}


void CDepthStencilBuffer::ClearDepthStencilBuffer()
{
	//�[�x�^�X�e���V���o�b�t�@�̃N���A
	CDevice_->GetMember()->m_spD3DDeviceContext->ClearDepthStencilView( m_pMember->m_spDepthStencilView.GetPtrNoConst() , D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL , 1.0f , 0 );
}


void CDepthStencilBuffer::ClearDepthBuffer()
{
	//�[�x�o�b�t�@�̃N���A
	CDevice_->GetMember()->m_spD3DDeviceContext->ClearDepthStencilView( m_pMember->m_spDepthStencilView.GetPtrNoConst() , D3D11_CLEAR_DEPTH , 1.0f , 0 );
}


void CDepthStencilBuffer::ClearStencilBuffer()
{
	//�X�e���V���o�b�t�@�̃N���A
	CDevice_->GetMember()->m_spD3DDeviceContext->ClearDepthStencilView( m_pMember->m_spDepthStencilView.GetPtrNoConst() , D3D11_CLEAR_STENCIL , 1.0f , 0 );
}


void CDepthStencilBuffer::Release()
{
	m_pMember->m_spDepthStencilView.Release();
	m_spTexture.Delete();
}


void CDepthStencilBuffer::Release_( CDepthStencilBuffer *pNode )
{
	pNode->Release();
	FOREACH( CDepthStencilBufferPtrList , it , pNode->m_spCloneList )
	{
		Release_( ( *it ).GetPtrNoConst() );
	}
}


void CDepthStencilBuffer::Rettach_( CDepthStencilBuffer *pNode )
{
	FOREACH( CDepthStencilBufferPtrList , it , pNode->m_spCloneList )
	{
		( *it )->m_pMember->m_spDepthStencilView = pNode->m_pMember->m_spDepthStencilView;
		( *it )->m_spTexture                     = pNode->m_spTexture;

		Rettach_( ( *it ).GetPtrNoConst() );
	}
}


void CDepthStencilBuffer::ReSize( u32 newWidth , u32 newHeight )
{
	Release_( this );
	SetUp( math::CBasicSize( ( dectype )newWidth , ( dectype )newHeight ) );
	Rettach_( this );
}

#endif
//===================================�yOpenGL�z====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

void CDepthStencilBuffer::SetUp( const math::CBasicSize &rect )
{
	SKY_UNUSED_ARG( rect );

	// �f�v�X�e�X�g��L���ɂ���
	glEnable( GL_DEPTH_TEST );
}


void CDepthStencilBuffer::ClearDepthStencilBuffer()
{
	// �f�v�X�o�b�t�@�̃N���A�l�̐ݒ�
	glClearDepth( 1.0f );

	//�f�v�X�t�@���N�i�������A��O�ɂ�����̂ŏ�`���Ă����j
	glDepthFunc( GL_LEQUAL );

	//�[�x�o�b�t�@�̃N���A
	glClear( GL_DEPTH_BUFFER_BIT );
}


void CDepthStencilBuffer::ClearDepthBuffer()
{
	// �f�v�X�o�b�t�@�̃N���A�l�̐ݒ�
	glClearDepth( 1.0f );

	//�f�v�X�t�@���N�i�������A��O�ɂ�����̂ŏ�`���Ă����j
	glDepthFunc( GL_LEQUAL );

	//�[�x�o�b�t�@�̃N���A
	glClear( GL_DEPTH_BUFFER_BIT );
}


void CDepthStencilBuffer::ClearStencilBuffer()
{
	// �f�v�X�o�b�t�@�̃N���A�l�̐ݒ�
	glClearDepth( 1.0f );

	//�f�v�X�t�@���N�i�������A��O�ɂ�����̂ŏ�`���Ă����j
	glDepthFunc( GL_LEQUAL );

	//�[�x�o�b�t�@�̃N���A
	glClear( GL_DEPTH_BUFFER_BIT );
}


void CDepthStencilBuffer::ReSize( u32 newWidth , u32 newHeight )
{
	m_Size.W = ( dectype )newWidth;
	m_Size.H = ( dectype )newHeight;
}


CDepthStencilBuffer::CDepthStencilBuffer()
{
}


CDepthStencilBuffer::~CDepthStencilBuffer()
{
}

#endif

} } }

