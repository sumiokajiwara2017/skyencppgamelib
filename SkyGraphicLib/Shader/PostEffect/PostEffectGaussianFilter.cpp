#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"

#include "SkyGraphicLib/Sprite.h"

SkyImplementRTTI( sky::lib::graphic::CPostEffectGaussianFilter , sky::lib::graphic::CPostEffectObject );

//===================================�yCommon�z===================================================>

namespace sky { namespace lib { namespace graphic {

static CShaderInfoPtrHash *s_pShaderHash = skyNull;
static CShaderInfoPtr GetShaderInfo( u32 shaderId );

void CPostEffectGaussianFilter::ComputeGaussWeights( f32 dispersion )
{
    f32 total = 0.0f;
    const int NUM_WEIGHT = 8;

    for( s32 i = 0 ; i < NUM_WEIGHT ; ++i )
    {
        f32 pos = 1.0f + 2.0f * ( f32 )i;
        m_Weights[ i ] = expf( -0.5f * ( pos * pos ) / dispersion );
        total += 2.0f * m_Weights[ i ];
    }

    f32 invTotal = 1.0f / total;
    for( s32 i = 0 ; i < NUM_WEIGHT ; ++i )
    {
        m_Weights[ i ] *= invTotal;
    }
}

void CPostEffectGaussianFilter::SetDispersion( f32 value )
{
	m_Dispersion_sq = value;
}

CPostEffectGaussianFilter::CPostEffectGaussianFilter() : m_Dispersion_sq( 5.0f )
{
	ComputeGaussWeights( m_Dispersion_sq * m_Dispersion_sq );
	m_PassNum = 2;
}

void CPostEffectGaussianFilter::vRendering( u32 passNo , const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &spCameraMatrix )
{

	//�V�F�[�_�[ID�̍\�z�iBasicShaderObject�ł̓��C�g�̏�Ԃɉ����ăV�F�[�_�[�����j
	u32 shaderId = 0;

	//�V�F�[�_�[ID���ω����Ă�����V�F�[�_�[���擾
	if ( m_spShaderInfo.IsEmpty() || m_ShaderId != shaderId )
	{
		//Hash��������΂���
		if ( s_pShaderHash == skyNull )
		{
			s_pShaderHash = NEW__( CShaderInfoPtrHash , CShaderInfoPtrHash( SHADER_HASH_SIZE ) );
		}

		m_spShaderInfo = GetShaderInfo( shaderId );
		m_ShaderId = shaderId; //���݂̃V�F�[�_�[��ύX
	}

	switch( passNo )
	{
	case 0:

		//�p�X�P
		Pass1Draw( spRenderTargetTexture , samplerData , spMesh , spCameraMatrix );

		break;
	case 1:

		//�p�X�Q
		Pass2Draw( spRenderTargetTexture , samplerData , spMesh , spCameraMatrix );

		break;
	}
}

} } }

//===================================�yDirectX�z===================================================>
#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

//-------------------------------------------------------------------------------------------------------------------------------

//���_�V�F�[�_�[���͒l�\����
struct CPostEffectGaussianFilterConst
{
	//���_�ϊ��s��
    XMFLOAT4X4 WorldViewProjectionMatrix;
	FLOAT      Weights[ 8 ];
	FLOAT      OffsetX;
	FLOAT      OffsetY;
	FLOAT      w;
	FLOAT      h;
};

static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	SKY_ASSERT_MSG( s_pShaderHash != skyNull , _T( "The Shader creator has not been initialized." ) ); //�V�F�[�_�[�N���G�[�^�[������������Ă��܂���B

	CShaderInfoPtr spShaderInfo;

	//�V�F�[�_�[ID�ɕR�Â��V�F�[�_�[���Ȃ�������V�F�[�_�[�����
	if ( s_pShaderHash->Get( shaderId , spShaderInfo ) == skyFalse )
	{
		spShaderInfo.SetPtr( NEW CShaderInfo() );

		//�f�o�C�X���擾
        ID3D11Device	*pD3DDevice	= CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst();

		ID3DBlob            *pShader      = NULL;
		ID3DBlob            *pError       = NULL;
		ID3D11InputLayout   *layout       = NULL;
		ID3D11VertexShader  *vertexShader = NULL;
		ID3D11PixelShader	*pixelShader  = NULL;

		// ���_���C�A�E�g�̍쐬
		D3D11_INPUT_ELEMENT_DESC ElementDesc[] = {
			{ "POSITION" , 0 , DXGI_FORMAT_R32G32B32_FLOAT   , 0 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 } ,
			{ "TEXCOORD" , 0 , DXGI_FORMAT_R32G32_FLOAT,       1 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 } ,
		};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		// ���_�V�F�[�_�i�p�X�P�p�j�쐬
		HRESULT hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/GaussianFilter.vert" ) , NULL , NULL , "mainVS_Pass1" , CDevice_->GetMember()->GetVSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			OutputDebugStringA( ( const char * )pError->GetBufferPointer() ); pError->Release();
			SKY_PANIC_MSG( _T( "���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂��� \n" ) );
		};
		hr = pD3DDevice->CreateVertexShader( (const DWORD*)pShader->GetBufferPointer(), pShader->GetBufferSize() , NULL , &vertexShader );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂��� \n" ) );
		};
		spShaderInfo->m_Pass1.m_spVertexShader.SetPtr( vertexShader );

		//���C�A�E�g�̍쐬
		hr =CDevice_->GetMember()->m_spD3DDevice->CreateInputLayout(
										ElementDesc ,
										array_sizeof( ElementDesc ) ,
										pShader->GetBufferPointer() ,
										pShader->GetBufferSize() ,
										&layout );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂��� \n" ) );
		}
		spShaderInfo->m_Pass1.m_spVertexLayout.SetPtr( layout );
		pShader->Release();

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		// ���_�V�F�[�_�i�p�X�Q�p�j�쐬
		hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/GaussianFilter.vert" ) , NULL , NULL , "mainVS_Pass2" , CDevice_->GetMember()->GetVSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			OutputDebugStringA( ( const char * )pError->GetBufferPointer() ); pError->Release();
			SKY_PANIC_MSG( _T( "���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂��� \n" ) );
		};
		hr = pD3DDevice->CreateVertexShader( (const DWORD*)pShader->GetBufferPointer(), pShader->GetBufferSize() , NULL , &vertexShader );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂��� \n" ) );
		};
		spShaderInfo->m_Pass2.m_spVertexShader.SetPtr( vertexShader );

		//���C�A�E�g�̍쐬
		hr =CDevice_->GetMember()->m_spD3DDevice->CreateInputLayout(
										ElementDesc ,
										array_sizeof( ElementDesc ) ,
										pShader->GetBufferPointer() ,
										pShader->GetBufferSize() ,
										&layout );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂��� \n" ) );
		}
		spShaderInfo->m_Pass2.m_spVertexLayout.SetPtr( layout );
		pShader->Release();

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

        // �O���[�o���ϐ��o�b�t�@�쐬
		D3D11_BUFFER_DESC ConstDesc;
		ID3D11Buffer  *pConstantBuffer;
		ConstDesc.Usage				= D3D11_USAGE_DEFAULT;
		ConstDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
		ConstDesc.CPUAccessFlags	= 0;
		ConstDesc.MiscFlags			= 0;
		ConstDesc.StructureByteStride = 0;
		ConstDesc.ByteWidth			= sizeof( CPostEffectGaussianFilterConst );
		hr = pD3DDevice->CreateBuffer( &ConstDesc, NULL, &pConstantBuffer );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂��� \n" ) );
		}
		spShaderInfo->m_spVertexConstBuffer0.SetPtr( pConstantBuffer );

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		// �s�N�Z���V�F�[�_�쐬
		hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/GaussianFilter.frag" ) , NULL , NULL , "mainPS_Pass1" , CDevice_->GetMember()->GetPSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
		if ( FAILED( hr ) )
		{
			OutputDebugStringA( (const char*)pError->GetBufferPointer() ); pError->Release();
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "�s�N�Z���V�F�[�_�[�̃R���p�C���Ɏ��s���܂��� \n" ) );
		};
		hr = pD3DDevice->CreatePixelShader( ( const DWORD* )pShader->GetBufferPointer() , pShader->GetBufferSize() , NULL , &pixelShader );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "�s�N�Z���V�F�[�_�[�̃R���p�C���Ɏ��s���܂��� \n" ) );
		};
		pShader->Release();
		spShaderInfo->m_Pass1.m_spPixelShader.SetPtr( pixelShader );

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		// �s�N�Z���V�F�[�_�쐬
		hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/GaussianFilter.frag" ) , NULL , NULL , "mainPS_Pass2" , CDevice_->GetMember()->GetPSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
		if ( FAILED( hr ) )
		{
			OutputDebugStringA( (const char*)pError->GetBufferPointer() ); pError->Release();
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "�s�N�Z���V�F�[�_�[�̃R���p�C���Ɏ��s���܂��� \n" ) );
		};
		hr = pD3DDevice->CreatePixelShader( ( const DWORD* )pShader->GetBufferPointer() , pShader->GetBufferSize() , NULL , &pixelShader );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "�s�N�Z���V�F�[�_�[�̃R���p�C���Ɏ��s���܂��� \n" ) );
		};
		pShader->Release();
		spShaderInfo->m_Pass2.m_spPixelShader.SetPtr( pixelShader );

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

		//ID�Ɋ֘A�Â��V�F�[�_�[�t�@�C���ۑ�����
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CPostEffectGaussianFilter::Pass1Draw( const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &cameraMatrix )
{
	SKY_UNUSED_ARG( samplerData );

//-----�p�����[�^�[�ݒ�-----��

	//�萔�o�b�t�@�󂯓n���p�̕ϐ�
	CPostEffectGaussianFilterConst shaderConst;

	//���[���h�s�� * �r���[�s�� * �v���W�F�N�V�����s��̐ݒ�
	XMMATRIX mat;
	D3DUtil::Convert4x3Matrix( cameraMatrix , mat );
	XMStoreFloat4x4( &shaderConst.WorldViewProjectionMatrix , XMMatrixTranspose( mat ) );

	//�E�F�C�g�̐ݒ�
	memory::CMemoryUtil::Copy( shaderConst.Weights , m_Weights ,  sizeof( m_Weights ) );

	//�I�t�Z�b�g�̐ݒ�
	shaderConst.OffsetX = 16.0f / spRenderTargetTexture->GetInfo().Width;
	shaderConst.OffsetY = 16.0f / spRenderTargetTexture->GetInfo().Height;

	//�T�C�Y�̐ݒ�
	shaderConst.w = ( FLOAT )spRenderTargetTexture->GetInfo().Width;
	shaderConst.h = ( FLOAT )spRenderTargetTexture->GetInfo().Height;

//-----�`�悷��-----��

	//�f�o�C�X���擾
	ID3D11DeviceContext	*pD3DDeviceContext	= CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst();

	//IA�ɓ��̓��C�A�E�g�E�I�u�W�F�N�g��ݒ�
	pD3DDeviceContext->IASetInputLayout( m_spShaderInfo->m_Pass1.m_spVertexLayout.GetPtrNoConst() );

	//VS�ɒ��_�V�F�[�_��ݒ�
	pD3DDeviceContext->VSSetShader( m_spShaderInfo->m_Pass1.m_spVertexShader.GetPtrNoConst() , NULL , 0 );

	//VS�ɒ萔�o�b�t�@��ݒ�
	CDevice_->GetMember()->m_spD3DDeviceContext->VSSetConstantBuffers( 0 , 1 , m_spShaderInfo->m_spVertexConstBuffer0.GetPtrPtr() );

	//PS�Ƀs�N�Z���E�V�F�[�_��ݒ�
	pD3DDeviceContext->PSSetShader( m_spShaderInfo->m_Pass1.m_spPixelShader.GetPtrNoConst() , NULL , 0 );

	// PS�ɒ萔�o�b�t�@��ݒ�
	CDevice_->GetMember()->m_spD3DDeviceContext->PSSetConstantBuffers( 0 , 1 , m_spShaderInfo->m_spVertexConstBuffer0.GetPtrPtr() );

	FOREACH( CSceneGraphNodePtrList , it , spMesh->LeafList() )
	{
		CMesh *pMesh = ( CMesh * )( *it ).GetPtrNoConst();
		CMaterialContena *pMaterialContena = ( CMaterialContena * )pMesh->GetMaterialContena().GetPtrNoConst();
		FOREACH( CSceneGraphNodePtrList , itMat , pMaterialContena->GetMaterialDataArray() )
		{
			CMaterialDrawDataPtr spMatDrawData = *itMat;

			//�萔�p�����[�^�[���󂯓n��
			CDevice_->GetMember()->m_spD3DDeviceContext->UpdateSubresource( m_spShaderInfo->m_spVertexConstBuffer0.GetPtrNoConst() , 0, NULL , &shaderConst, 0, 0 );

			//�T���v���[��n��
			spRenderTargetTexture->SetShader();

			//�X�g���[����ݒ肷��
			CVertexBufferStreamPtrList streamList;
			streamList.AddTail( pMesh->GetVertexBuffer()->GetPositionStream() );
			streamList.AddTail( pMesh->GetVertexBuffer()->GetUvStream() );
			CVertexUtil::SetRenderBufferStream( streamList , pMesh->GetIndexBuffer() , pMesh->GetVertexBuffer()->GetPrimitiveType() );

			//���_�̕`��
			pMesh->GetVertexBuffer()->Draw( spMatDrawData->m_DrawStartIndex , spMatDrawData->m_DrawEndIndex );
		}
	}

	//�e��V�F�[�_�[�̃N���A
	pD3DDeviceContext->VSSetShader( NULL , NULL , 0 ); // ���_�V�F�[�_�[
	pD3DDeviceContext->PSSetShader( NULL , NULL , 0 ); // �s�N�Z���V�F�[�_�[
}

void CPostEffectGaussianFilter::Pass2Draw( const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &cameraMatrix )
{
	SKY_UNUSED_ARG( samplerData );

//-----�p�����[�^�[�ݒ�-----��

	//�萔�o�b�t�@�󂯓n���p�̕ϐ�
	CPostEffectGaussianFilterConst shaderConst;

	//���[���h�s�� * �r���[�s�� * �v���W�F�N�V�����s��̐ݒ�
	XMMATRIX mat;
	D3DUtil::Convert4x3Matrix( cameraMatrix , mat );
	XMStoreFloat4x4( &shaderConst.WorldViewProjectionMatrix , XMMatrixTranspose( mat ) );

	//�E�F�C�g�̐ݒ�
	memory::CMemoryUtil::Copy( shaderConst.Weights , m_Weights ,  sizeof( m_Weights ) );

	//�I�t�Z�b�g�̐ݒ�
	shaderConst.OffsetX = 16.0f / spRenderTargetTexture->GetInfo().Width;
	shaderConst.OffsetY = 16.0f / spRenderTargetTexture->GetInfo().Height;

	//�T�C�Y�̐ݒ�
	shaderConst.w = ( FLOAT )spRenderTargetTexture->GetInfo().Width;
	shaderConst.h = ( FLOAT )spRenderTargetTexture->GetInfo().Height;

//-----�`�悷��-----��

	//�f�o�C�X���擾
	ID3D11DeviceContext	*pD3DDeviceContext	= CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst();

	//IA�ɓ��̓��C�A�E�g�E�I�u�W�F�N�g��ݒ�
	pD3DDeviceContext->IASetInputLayout( m_spShaderInfo->m_Pass2.m_spVertexLayout.GetPtrNoConst() );

	//VS�ɒ��_�V�F�[�_��ݒ�
	pD3DDeviceContext->VSSetShader( m_spShaderInfo->m_Pass2.m_spVertexShader.GetPtrNoConst() , NULL , 0 );

	//VS�ɒ萔�o�b�t�@��ݒ�
	CDevice_->GetMember()->m_spD3DDeviceContext->VSSetConstantBuffers( 0, 1, m_spShaderInfo->m_spVertexConstBuffer0.GetPtrPtr() );

	//PS�Ƀs�N�Z���E�V�F�[�_��ݒ�
	pD3DDeviceContext->PSSetShader( m_spShaderInfo->m_Pass2.m_spPixelShader.GetPtrNoConst() , NULL , 0 );  // �s�N�Z���V�F�[�_�[

	// PS�ɒ萔�o�b�t�@��ݒ�
	CDevice_->GetMember()->m_spD3DDeviceContext->PSSetConstantBuffers( 0 , 1 , m_spShaderInfo->m_spVertexConstBuffer0.GetPtrPtr() );

	FOREACH( CSceneGraphNodePtrList , it , spMesh->LeafList() )
	{
		CMesh *pMesh = ( CMesh * )( *it ).GetPtrNoConst();
		CMaterialContena *pMaterialContena = ( CMaterialContena * )pMesh->GetMaterialContena().GetPtrNoConst();
		FOREACH( CSceneGraphNodePtrList , itMat , pMaterialContena->GetMaterialDataArray() )
		{
			CMaterialDrawDataPtr spMatDrawData = *itMat;

			//�萔�p�����[�^�[���󂯓n��
			CDevice_->GetMember()->m_spD3DDeviceContext->UpdateSubresource( m_spShaderInfo->m_spVertexConstBuffer0.GetPtrNoConst() , 0, NULL , &shaderConst, 0, 0 );

			//�T���v���[��n��
			spRenderTargetTexture->SetShader();

			//�X�g���[����ݒ肷��
			CVertexBufferStreamPtrList streamList;
			streamList.AddTail( pMesh->GetVertexBuffer()->GetPositionStream() );
			streamList.AddTail( pMesh->GetVertexBuffer()->GetUvStream() );
			CVertexUtil::SetRenderBufferStream( streamList , pMesh->GetIndexBuffer() , pMesh->GetVertexBuffer()->GetPrimitiveType() );

			//���_�̕`��
			pMesh->GetVertexBuffer()->Draw( spMatDrawData->m_DrawStartIndex , spMatDrawData->m_DrawEndIndex );
		}
	}

	//�e��V�F�[�_�[�̃N���A
	pD3DDeviceContext->VSSetShader( NULL , NULL , 0 ); // ���_�V�F�[�_�[
	pD3DDeviceContext->PSSetShader( NULL , NULL , 0 ); // �s�N�Z���V�F�[�_�[
}

} } }

#endif

//===================================�yOpenGL�z====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

#ifdef SW_SKYGRAPHICLIB_USE_GLSL_ON

//�O���[�o���p�����[�^�[
static GLfloat PVWMatrix [ 16 ];
static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	SKY_ASSERT_MSG( s_pShaderHash != skyNull , _T( "The Shader creator has not been initialized." ) ); //�V�F�[�_�[�N���G�[�^�[������������Ă��܂���B

	CShaderInfoPtr spShaderInfo;

	//�V�F�[�_�[ID�ɕR�Â��V�F�[�_�[���Ȃ�������V�F�[�_�[�����
	if ( s_pShaderHash->Get( shaderId , spShaderInfo ) == skyFalse )
	{
		spShaderInfo.SetPtr( NEW CShaderInfo() );

//-------------Pass1------------------��

		// �V�F�[�_�̓ǂݍ���
		sky::lib::graphic::CGLSL::CreateVertexShaderAndFragmentShader(
			spShaderInfo->m_Pass1.m_ProgramObject ,
			_T( "data/lib/shader/glsl/GaussianFilterPass1.vert" ) , 
			_T( "data/lib/shader/glsl/GaussianFilterPass1.frag" ) );
 	
		//���_�������
		spShaderInfo->m_Pass1.m_VertexLocation = glGetAttribLocation(spShaderInfo->m_Pass1.m_ProgramObject, "inPos");
		spShaderInfo->m_Pass1.m_TexCoord0Location = glGetAttribLocation(spShaderInfo->m_Pass1.m_ProgramObject, "inTexcoord0");

		//�O���[�o�����
		spShaderInfo->m_Pass1.m_WvpLocation   = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "PVWMatrix");
		spShaderInfo->m_Pass1.m_FreeLocation0 = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "Weights0"); //vec4
		spShaderInfo->m_Pass1.m_FreeLocation1 = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "Weights1"); //vec4
		spShaderInfo->m_Pass1.m_FreeLocation2 = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "OffsetX");  //float
		spShaderInfo->m_Pass1.m_FreeLocation3 = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "OffsetY");  //float
		spShaderInfo->m_Pass1.m_FreeLocation4 = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "w");        //float
		spShaderInfo->m_Pass1.m_FreeLocation5 = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "h");        //float

		spShaderInfo->m_Pass1.m_Texture0Location = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "Texture0");

		// Fragment data
#ifndef SKYLIB_IDE_QTCREATER
        glBindFragDataLocation(spShaderInfo->m_Pass1.m_ProgramObject, 0, "fragColor");
#else
        glBindFragDataLocationEXT(spShaderInfo->m_Pass1.m_ProgramObject, 0, "fragColor");
#endif

//-------------Pass2------------------��

		// �V�F�[�_�̓ǂݍ���
		sky::lib::graphic::CGLSL::CreateVertexShaderAndFragmentShader(
			spShaderInfo->m_Pass2.m_ProgramObject ,
			_T( "data/lib/shader/glsl/GaussianFilterPass2.vert" ) , 
			_T( "data/lib/shader/glsl/GaussianFilterPass2.frag" ) );
 	
		//���_�������
		spShaderInfo->m_Pass2.m_VertexLocation = glGetAttribLocation(spShaderInfo->m_Pass2.m_ProgramObject, "inPos");
		spShaderInfo->m_Pass2.m_TexCoord0Location = glGetAttribLocation(spShaderInfo->m_Pass2.m_ProgramObject, "inTexcoord0");

		//�O���[�o�����
		spShaderInfo->m_Pass2.m_WvpLocation   = glGetUniformLocation(spShaderInfo->m_Pass2.m_ProgramObject, "PVWMatrix");
		spShaderInfo->m_Pass2.m_FreeLocation0 = glGetUniformLocation(spShaderInfo->m_Pass2.m_ProgramObject, "Weights0"); //vec4
		spShaderInfo->m_Pass2.m_FreeLocation1 = glGetUniformLocation(spShaderInfo->m_Pass2.m_ProgramObject, "Weights1"); //vec4
		spShaderInfo->m_Pass2.m_FreeLocation2 = glGetUniformLocation(spShaderInfo->m_Pass2.m_ProgramObject, "OffsetX");  //float
		spShaderInfo->m_Pass2.m_FreeLocation3 = glGetUniformLocation(spShaderInfo->m_Pass2.m_ProgramObject, "OffsetY");  //float
		spShaderInfo->m_Pass2.m_FreeLocation4 = glGetUniformLocation(spShaderInfo->m_Pass2.m_ProgramObject, "w");        //float
		spShaderInfo->m_Pass2.m_FreeLocation5 = glGetUniformLocation(spShaderInfo->m_Pass2.m_ProgramObject, "h");        //float

		spShaderInfo->m_Pass2.m_Texture0Location = glGetUniformLocation(spShaderInfo->m_Pass2.m_ProgramObject, "Texture0");

		// Fragment data
#ifndef SKYLIB_IDE_QTCREATER
        glBindFragDataLocation(spShaderInfo->m_Pass2.m_ProgramObject, 0, "fragColor");
#else
        glBindFragDataLocationEXT(spShaderInfo->m_Pass2.m_ProgramObject, 0, "fragColor");
#endif

		//ID�Ɋ֘A�Â��V�F�[�_�[�t�@�C���ۑ�����
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CPostEffectGaussianFilter::Pass1Draw( const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &cameraMatrix )
{
	SKY_UNUSED_ARG( samplerData );

	//�V�F�[�_�[�̐ݒ�
	glUseProgram( m_spShaderInfo->m_Pass1.m_ProgramObject );

	//���[���h�s��ݒ�( ���� ���ӁI View , Projection �s��Ƃ̊|���Z��MulMatrix4x4�֐��ōs�����ƁiM44�̗v�f���K�p�Ȃ��߁j���� �j
	cameraMatrix.CopyArray< f32 >( PVWMatrix );

    // ���W�ϊ��s��
    glUniformMatrix4fv( m_spShaderInfo->m_Pass1.m_WvpLocation , 1 , GL_FALSE , PVWMatrix );

	//�E�F�C�g�̐ݒ�
	GLfloat weights0[ 4 ];
	GLfloat weights1[ 4 ];
	memory::CMemoryUtil::Copy( weights0 , m_Weights , sizeof( f32 ) * 4 );
	memory::CMemoryUtil::Copy( weights1 , &m_Weights[ 4 ] , sizeof( f32 ) * 4 );
	glUniform4fv( m_spShaderInfo->m_Pass2.m_FreeLocation0 , 1 , weights0 );
	glUniform4fv( m_spShaderInfo->m_Pass2.m_FreeLocation1 , 1 , weights1 );

	//�I�t�Z�b�g�̐ݒ�
	f32 offsetX = 16.0f / spRenderTargetTexture->GetInfo().Width;
	f32 offsetY = 16.0f / spRenderTargetTexture->GetInfo().Height;
	glUniform1fv( m_spShaderInfo->m_Pass1.m_FreeLocation2 , 1 , &offsetX );
	glUniform1fv( m_spShaderInfo->m_Pass1.m_FreeLocation3 , 1 , &offsetY );

	//�T�C�Y�̐ݒ�
	f32 w = ( f32 )spRenderTargetTexture->GetInfo().Width;
	f32 h = ( f32 )spRenderTargetTexture->GetInfo().Height;
	glUniform1fv( m_spShaderInfo->m_Pass1.m_FreeLocation4 , 1 , &w );
	glUniform1fv( m_spShaderInfo->m_Pass1.m_FreeLocation5 , 1 , &h );

	FOREACH( CSceneGraphNodePtrList , it , spMesh->LeafList() )
	{
		CMesh *pMesh = ( CMesh * )( *it ).GetPtrNoConst();
		CVertexBufferPtr spVertexBuffer = pMesh->GetVertexBuffer();

		//�e�N�X�`���̐ݒ�
		glUniform1i( m_spShaderInfo->m_Pass1.m_Texture0Location , 0 ); // �e�N�X�`�����j�b�g�ԍ� 0
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, spRenderTargetTexture->GetMember()->texId[ 0 ] );

		//�X�g���[����ݒ肷��
		spVertexBuffer->GetPositionStream()->GetMember()->m_Location = m_spShaderInfo->m_Pass1.m_VertexLocation;
		spVertexBuffer->GetUvStream()->GetMember()->m_Location = m_spShaderInfo->m_Pass1.m_TexCoord0Location;
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( spVertexBuffer->GetPositionStream() );
		streamList.AddTail( spVertexBuffer->GetUvStream() );
		CVertexUtil::SetRenderBufferStream( streamList , pMesh->GetIndexBuffer() );

		//���b�V�����ɕ`��
		spVertexBuffer->Draw( 0 , 0 , pMesh->GetIndexBuffer() );
	}
}

void CPostEffectGaussianFilter::Pass2Draw( const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &cameraMatrix )
{
	SKY_UNUSED_ARG( samplerData );

	//�V�F�[�_�[�̐ݒ�
	glUseProgram( m_spShaderInfo->m_Pass2.m_ProgramObject );

	//���[���h�s��ݒ�( ���� ���ӁI View , Projection �s��Ƃ̊|���Z��MulMatrix4x4�֐��ōs�����ƁiM44�̗v�f���K�p�Ȃ��߁j���� �j
	cameraMatrix.CopyArray< f32 >( PVWMatrix );

    // ���W�ϊ��s��
    glUniformMatrix4fv( m_spShaderInfo->m_Pass2.m_WvpLocation , 1 , GL_FALSE , PVWMatrix );

	//�E�F�C�g�̐ݒ�
	GLfloat weights0[ 4 ];
	GLfloat weights1[ 4 ];
	memory::CMemoryUtil::Copy( weights0 , m_Weights , sizeof( f32 ) * 4 );
	memory::CMemoryUtil::Copy( weights1 , &m_Weights[ 4 ] , sizeof( f32 ) * 4 );
	glUniform4fv( m_spShaderInfo->m_Pass2.m_FreeLocation0 , 1 , weights0 );
	glUniform4fv( m_spShaderInfo->m_Pass2.m_FreeLocation1 , 1 , weights1 );

	//�I�t�Z�b�g�̐ݒ�
	f32 offsetX = 16.0f / spRenderTargetTexture->GetInfo().Width;
	f32 offsetY = 16.0f / spRenderTargetTexture->GetInfo().Height;
	glUniform1fv( m_spShaderInfo->m_Pass2.m_FreeLocation2 , 1 , &offsetX );
	glUniform1fv( m_spShaderInfo->m_Pass2.m_FreeLocation3 , 1 , &offsetY );

	//�T�C�Y�̐ݒ�
	f32 w = ( f32 )spRenderTargetTexture->GetInfo().Width;
	f32 h = ( f32 )spRenderTargetTexture->GetInfo().Height;
	glUniform1fv( m_spShaderInfo->m_Pass2.m_FreeLocation4 , 1 , &w );
	glUniform1fv( m_spShaderInfo->m_Pass2.m_FreeLocation5 , 1 , &h );

	FOREACH( CSceneGraphNodePtrList , it , spMesh->LeafList() )
	{
		CMesh *pMesh = ( CMesh * )( *it ).GetPtrNoConst();
		CVertexBufferPtr spVertexBuffer = pMesh->GetVertexBuffer();

		//�e�N�X�`���̐ݒ�
		glUniform1i( m_spShaderInfo->m_Pass2.m_Texture0Location , 0 ); // �e�N�X�`�����j�b�g�ԍ� 0
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, spRenderTargetTexture->GetMember()->texId[ 0 ] );

		//�X�g���[����ݒ肷��
		spVertexBuffer->GetPositionStream()->GetMember()->m_Location = m_spShaderInfo->m_Pass2.m_VertexLocation;
		spVertexBuffer->GetUvStream()->GetMember()->m_Location       = m_spShaderInfo->m_Pass2.m_TexCoord0Location;
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( spVertexBuffer->GetPositionStream() );
		streamList.AddTail( spVertexBuffer->GetUvStream() );
		CVertexUtil::SetRenderBufferStream( streamList , pMesh->GetIndexBuffer() );

		//���b�V�����ɕ`��
		spVertexBuffer->Draw( 0 , 0 , pMesh->GetIndexBuffer() );
	}
}

#endif

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

static float PVWMatrix [ 16 ];
static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	CShaderInfoPtr spShaderInfo;

	//�V�F�[�_�[ID�ɕR�Â��V�F�[�_�[���Ȃ�������V�F�[�_�[�����
	if ( s_pShaderHash->Get( shaderId , spShaderInfo ) == skyFalse )
	{
		spShaderInfo.SetPtr( NEW CShaderInfo() );

//---------Pass1�p----------

		//���ӁjcgDestroyProgram( spShaderInfo->m_ProgramObject0.Program ); �Ŕj��
		CCg::CreateVertexFragmentCombineShaderFromFile( spShaderInfo->m_Pass1.m_ProgramObject , 
		                                            "data/lib/shader/cg/GaussianFilter.vert" , "mainVS_Pass1" ,
		                                            "data/lib/shader/cg/GaussianFilter.frag" , "mainPS_Pass1"  );

		spShaderInfo->m_Pass1.m_WvpLocation = cgGetNamedParameter( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.WorldViewProjectionMatrix" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        spShaderInfo->m_Pass1.m_FreeLocation0 = cgGetNamedParameter( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.Weights0" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        spShaderInfo->m_Pass1.m_FreeLocation1 = cgGetNamedParameter( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.Weights1" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        spShaderInfo->m_Pass1.m_FreeLocation2 = cgGetNamedParameter( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.OffsetX" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        spShaderInfo->m_Pass1.m_FreeLocation3 = cgGetNamedParameter( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.OffsetY" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        spShaderInfo->m_Pass1.m_FreeLocation4 = cgGetNamedParameter( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.w" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        spShaderInfo->m_Pass1.m_FreeLocation5 = cgGetNamedParameter( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.h" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );

        spShaderInfo->m_Pass1.m_TexCoord0Location = cgGetNamedParameter( spShaderInfo->m_Pass1.m_ProgramObject.Program , "texCoord0Location" );//���O
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );

//---------Pass2�p----------

		//���ӁjcgDestroyProgram( spShaderInfo->m_ProgramObject0.Program ); �Ŕj��
		CCg::CreateVertexFragmentCombineShaderFromFile( spShaderInfo->m_Pass2.m_ProgramObject , 
		                                            "data/lib/shader/cg/GaussianFilter.vert" , "mainVS_Pass2" ,
		                                            "data/lib/shader/cg/GaussianFilter.frag" , "mainPS_Pass2"  );

		spShaderInfo->m_Pass2.m_WvpLocation = cgGetNamedParameter( spShaderInfo->m_Pass2.m_ProgramObject.Program , "uniforms.WorldViewProjectionMatrix" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        spShaderInfo->m_Pass2.m_FreeLocation0 = cgGetNamedParameter( spShaderInfo->m_Pass2.m_ProgramObject.Program , "uniforms.Weights0" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        spShaderInfo->m_Pass2.m_FreeLocation1 = cgGetNamedParameter( spShaderInfo->m_Pass2.m_ProgramObject.Program , "uniforms.Weights1" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        spShaderInfo->m_Pass2.m_FreeLocation2 = cgGetNamedParameter( spShaderInfo->m_Pass2.m_ProgramObject.Program , "uniforms.OffsetX" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        spShaderInfo->m_Pass2.m_FreeLocation3 = cgGetNamedParameter( spShaderInfo->m_Pass2.m_ProgramObject.Program , "uniforms.OffsetY" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        spShaderInfo->m_Pass2.m_FreeLocation4 = cgGetNamedParameter( spShaderInfo->m_Pass2.m_ProgramObject.Program , "uniforms.w" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        spShaderInfo->m_Pass2.m_FreeLocation5 = cgGetNamedParameter( spShaderInfo->m_Pass2.m_ProgramObject.Program , "uniforms.h" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );

        spShaderInfo->m_Pass2.m_TexCoord0Location = cgGetNamedParameter( spShaderInfo->m_Pass2.m_ProgramObject.Program , "texCoord0Location" );//���O
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );

		//ID�Ɋ֘A�Â��V�F�[�_�[�t�@�C���ۑ�����
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CPostEffectGaussianFilter::Pass1Draw( const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &cameraMatrix )
{
	SKY_UNUSED_ARG( samplerData );

	//���[���h�s��ݒ�( ���� ���ӁI View , Projection �s��Ƃ̊|���Z��MulMatrix4x4�֐��ōs�����ƁiM44�̗v�f���K�p�Ȃ��߁j���� �j
	cameraMatrix.CopyArray< f32 >( PVWMatrix );

	//�s��ݒ�
	cgSetMatrixParameterfr( m_spShaderInfo->m_Pass1.m_WvpLocation , PVWMatrix );
    CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );

	//�E�F�C�g�̐ݒ�
	GLfloat weights0[ 4 ];
	GLfloat weights1[ 4 ];
	memory::CMemoryUtil::Copy( weights0 , m_Weights , sizeof( f32 ) * 4 );
	memory::CMemoryUtil::Copy( weights1 , &m_Weights[ 4 ] , sizeof( f32 ) * 4 );
	cgSetParameter4fv( m_spShaderInfo->m_Pass1.m_FreeLocation0 , weights0 );
	cgSetParameter4fv( m_spShaderInfo->m_Pass1.m_FreeLocation1 , weights1 );

	//�I�t�Z�b�g�̐ݒ�
	f32 offsetX = 16.0f / spRenderTargetTexture->GetInfo().Width;
	f32 offsetY = 16.0f / spRenderTargetTexture->GetInfo().Height;
	cgSetParameter1fv( m_spShaderInfo->m_Pass1.m_FreeLocation2 , &offsetX );
	cgSetParameter1fv( m_spShaderInfo->m_Pass1.m_FreeLocation3 , &offsetY );

	//�T�C�Y�̐ݒ�
	f32 w = ( f32 )spRenderTargetTexture->GetInfo().Width;
	f32 h = ( f32 )spRenderTargetTexture->GetInfo().Height;
	cgSetParameter1fv( m_spShaderInfo->m_Pass1.m_FreeLocation4 , &w );
	cgSetParameter1fv( m_spShaderInfo->m_Pass1.m_FreeLocation5 , &h );

	const CGprofile profile0 = cgGetProgramDomainProfile( m_spShaderInfo->m_Pass1.m_ProgramObject.Program , 0 );
    CCg::CheckForCgError( "�V�F�[�_�[�̏����Ɏ��s���܂����B" );
	const CGprofile profile1 = cgGetProgramDomainProfile( m_spShaderInfo->m_Pass1.m_ProgramObject.Program , 1 );
    CCg::CheckForCgError( "�V�F�[�_�[�̏����Ɏ��s���܂����B" );

	//���_�V�F�[�_�[��ݒ�
	cgGLBindProgram( m_spShaderInfo->m_Pass1.m_ProgramObject.Program );
    CCg::CheckForCgError( "�V�F�[�_�[�̏����Ɏ��s���܂����B" );
    cgGLEnableProfile( profile0 );
    CCg::CheckForCgError( "�V�F�[�_�[�̏����Ɏ��s���܂����B" );
    cgGLEnableProfile( profile1 );
    CCg::CheckForCgError( "�V�F�[�_�[�̏����Ɏ��s���܂����B" );

	FOREACH( CSceneGraphNodePtrList , it , spMesh->LeafList() )
	{
		CMesh *pMesh = ( CMesh * )( *it ).GetPtrNoConst();
		CVertexBufferPtr spVertexBuffer = pMesh->GetVertexBuffer();

		//�f�B�r���[�Y�}�b�v�e�N�X�`�����g�p����
        cgGLSetTextureParameter( m_spShaderInfo->m_Pass1.m_TexCoord0Location , spRenderTargetTexture->GetMember()->texId[ 0 ] ); //�e�N�X�`���Ƃ̊֘A�t��
        CCg::CheckForCgError( "�e�N�X�`���̐ݒ�Ɏ��s���܂����B" );
        cgGLEnableTextureParameter( m_spShaderInfo->m_Pass1.m_TexCoord0Location );
        CCg::CheckForCgError( "�e�N�X�`����L���ɂł��܂���ł����B" );

		//�X�g���[����ݒ肷��
		spVertexBuffer->GetPositionStream()->GetMember()->m_Location = 0;
		spVertexBuffer->GetUvStream()->GetMember()->m_Location   = 1;
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( spVertexBuffer->GetPositionStream() );
		streamList.AddTail( spVertexBuffer->GetUvStream() );
		CVertexUtil::SetRenderBufferStream( streamList , pMesh->GetIndexBuffer() );

		//���b�V�����ɕ`��
		spVertexBuffer->Draw( 0 , 0 , pMesh->GetIndexBuffer() );

        cgGLDisableTextureParameter( m_spShaderInfo->m_Pass1.m_TexCoord0Location );
        CCg::CheckForCgError( "�e�N�X�`���𖳌��ɂł��܂���ł����B" );
	}

	//�V�F�[�_�[��ݒ����
    cgGLDisableProfile( profile0 );
    CCg::CheckForCgError( "�V�F�[�_�[�̊J���Ɏ��s���܂����B" );
    cgGLDisableProfile( profile1 );
    CCg::CheckForCgError( "�V�F�[�_�[�̊J���Ɏ��s���܂����B" );
	cgGLUnbindProgram( profile0 );
    CCg::CheckForCgError( "�V�F�[�_�[�̊J���Ɏ��s���܂����B" );
	cgGLUnbindProgram( profile1 );
    CCg::CheckForCgError( "�V�F�[�_�[�̊J���Ɏ��s���܂����B" );
}

void CPostEffectGaussianFilter::Pass2Draw( const CTexturePtr &spRenderTargetTexture , const CSamplerData &samplerData , const CMeshContenaPtr &spMesh , const math::CMatrix4x3 &cameraMatrix )
{
	SKY_UNUSED_ARG( samplerData );

	//���[���h�s��ݒ�( ���� ���ӁI View , Projection �s��Ƃ̊|���Z��MulMatrix4x4�֐��ōs�����ƁiM44�̗v�f���K�p�Ȃ��߁j���� �j
	cameraMatrix.CopyArray< f32 >( PVWMatrix );

	//�s��ݒ�
	cgSetMatrixParameterfr( m_spShaderInfo->m_Pass2.m_WvpLocation , PVWMatrix );
    CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );

	//�E�F�C�g�̐ݒ�
	GLfloat weights0[ 4 ];
	GLfloat weights1[ 4 ];
	memory::CMemoryUtil::Copy( weights0 , m_Weights , sizeof( f32 ) * 4 );
	memory::CMemoryUtil::Copy( weights1 , &m_Weights[ 4 ] , sizeof( f32 ) * 4 );
	cgSetParameter4fv( m_spShaderInfo->m_Pass2.m_FreeLocation0 , weights0 );
	cgSetParameter4fv( m_spShaderInfo->m_Pass2.m_FreeLocation1 , weights1 );

	//�I�t�Z�b�g�̐ݒ�
	f32 offsetX = 16.0f / spRenderTargetTexture->GetInfo().Width;
	f32 offsetY = 16.0f / spRenderTargetTexture->GetInfo().Height;
	cgSetParameter1fv( m_spShaderInfo->m_Pass2.m_FreeLocation2 , &offsetX );
	cgSetParameter1fv( m_spShaderInfo->m_Pass2.m_FreeLocation3 , &offsetY );

	//�T�C�Y�̐ݒ�
	f32 w = ( f32 )spRenderTargetTexture->GetInfo().Width;
	f32 h = ( f32 )spRenderTargetTexture->GetInfo().Height;
	cgSetParameter1fv( m_spShaderInfo->m_Pass2.m_FreeLocation4 , &w );
	cgSetParameter1fv( m_spShaderInfo->m_Pass2.m_FreeLocation5 , &h );

	const CGprofile profile0 = cgGetProgramDomainProfile( m_spShaderInfo->m_Pass2.m_ProgramObject.Program , 0 );
    CCg::CheckForCgError( "�V�F�[�_�[�̏����Ɏ��s���܂����B" );
	const CGprofile profile1 = cgGetProgramDomainProfile( m_spShaderInfo->m_Pass2.m_ProgramObject.Program , 1 );
    CCg::CheckForCgError( "�V�F�[�_�[�̏����Ɏ��s���܂����B" );

	//���_�V�F�[�_�[��ݒ�
	cgGLBindProgram( m_spShaderInfo->m_Pass2.m_ProgramObject.Program );
    CCg::CheckForCgError( "�V�F�[�_�[�̏����Ɏ��s���܂����B" );
    cgGLEnableProfile( profile0 );
    CCg::CheckForCgError( "�V�F�[�_�[�̏����Ɏ��s���܂����B" );
    cgGLEnableProfile( profile1 );
    CCg::CheckForCgError( "�V�F�[�_�[�̏����Ɏ��s���܂����B" );

	FOREACH( CSceneGraphNodePtrList , it , spMesh->LeafList() )
	{
		CMesh *pMesh = ( CMesh * )( *it ).GetPtrNoConst();
		CVertexBufferPtr spVertexBuffer = pMesh->GetVertexBuffer();

		//�f�B�r���[�Y�}�b�v�e�N�X�`�����g�p����
        cgGLSetTextureParameter( m_spShaderInfo->m_Pass2.m_TexCoord0Location , spRenderTargetTexture->GetMember()->texId[ 0 ] ); //�e�N�X�`���Ƃ̊֘A�t��
        CCg::CheckForCgError( "�e�N�X�`���̐ݒ�Ɏ��s���܂����B" );
        cgGLEnableTextureParameter( m_spShaderInfo->m_Pass2.m_TexCoord0Location );
        CCg::CheckForCgError( "�e�N�X�`����L���ɂł��܂���ł����B" );

		//�X�g���[����ݒ肷��
		spVertexBuffer->GetPositionStream()->GetMember()->m_Location = 0;
		spVertexBuffer->GetUvStream()->GetMember()->m_Location   = 1;
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( spVertexBuffer->GetPositionStream() );
		streamList.AddTail( spVertexBuffer->GetUvStream() );
		CVertexUtil::SetRenderBufferStream( streamList , pMesh->GetIndexBuffer() );

		//���b�V�����ɕ`��
		spVertexBuffer->Draw( 0 , 0 , pMesh->GetIndexBuffer() );

        cgGLDisableTextureParameter( m_spShaderInfo->m_Pass2.m_TexCoord0Location );
        CCg::CheckForCgError( "�e�N�X�`���𖳌��ɂł��܂���ł����B" );
	}

	//�V�F�[�_�[��ݒ����
    cgGLDisableProfile( profile0 );
    CCg::CheckForCgError( "�V�F�[�_�[�̊J���Ɏ��s���܂����B" );
    cgGLDisableProfile( profile1 );
    CCg::CheckForCgError( "�V�F�[�_�[�̊J���Ɏ��s���܂����B" );
	cgGLUnbindProgram( profile0 );
    CCg::CheckForCgError( "�V�F�[�_�[�̊J���Ɏ��s���܂����B" );
	cgGLUnbindProgram( profile1 );
    CCg::CheckForCgError( "�V�F�[�_�[�̊J���Ɏ��s���܂����B" );
}

#endif

} } }

#endif
