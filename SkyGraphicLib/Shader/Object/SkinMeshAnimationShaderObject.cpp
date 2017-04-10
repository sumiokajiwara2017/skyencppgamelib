#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"

SkyImplementRTTI( sky::lib::graphic::CSkinMeshAnimationShaderObject       , sky::lib::graphic::CShaderObject );

//===================================�yCommon�z===================================================>

namespace sky { namespace lib { namespace graphic {

static CShaderInfoPtrHash *s_pShaderHash = skyNull;
static CShaderInfoPtr GetShaderInfo( u32 shaderId );

const serialize::CSerializeInterfacePtr& CSkinMeshAnimationShaderObject::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CSkinMeshAnimationShaderObject::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CSkinMeshAnimationShaderObject::Clone()
{
	return Copy( CSkinMeshAnimationShaderObjectPtr( NEW__( CSkinMeshAnimationShaderObject , CSkinMeshAnimationShaderObject() ) ) );
}

serialize::CSerializeInterfacePtr CSkinMeshAnimationShaderObject::CloneSharing()
{
	return Clone();
}

CSkinMeshAnimationShaderObject::CSkinMeshAnimationShaderObject()
{
	m_PassNum = 1;
}

void CSkinMeshAnimationShaderObject::vDraw()
{
	//�V�F�[�_�[ID�̍\�z�iSkinMeshAnimationShaderObject�ł̓��C�g�̏�Ԃɉ����ăV�F�[�_�[�����j
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

	//�p�X���̕`��
	if ( m_RenderingPassNo == 0 )
	{
		Pass1Draw();
	}
}

} } }

#define BONE_MAX 80

//===================================�yDirectX�z===================================================>
#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

// �萔�o�b�t�@�̃f�[�^��`
struct MotionShaderConst
{
    XMFLOAT4X4 WorldViewProjectionMatrix; // ���[���h�~�r���[�~�v���W�F�N�V����
	XMFLOAT4X4 Joints[ BONE_MAX ];
};

static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	SKY_ASSERT_MSG( s_pShaderHash != skyNull , _T( "The Shader creator has not been initialized." ) ); //�V�F�[�_�[�N���G�[�^�[������������Ă��܂���B

	CShaderInfoPtr spShaderInfo;

	//�V�F�[�_�[ID�ɕR�Â��V�F�[�_�[���Ȃ�������V�F�[�_�[�����
	if ( s_pShaderHash->Get( shaderId , spShaderInfo ) == skyFalse )
	{
		spShaderInfo.SetPtr( NEW CShaderInfo() );

		//(�������jID�Ɋ֘A�Â��V�F�[�_�[�t�@�C����T���B�t�@�C���ł������炻������[�h���ăV�F�[�_�[�Ƃ���

		//(�������jshaderId�ɉ����ăV�F�[�_�[�̃\�[�X�𕶎��񑀍�ŕω�������B

		//�f�o�C�X���擾
        ID3D11Device	*pD3DDevice	= CDevice_->GetMember()->m_spD3DDevice.GetPtrNoConst();

		ID3DBlob            *pShader = NULL;
		ID3DBlob            *pError = NULL;
		ID3D11InputLayout   *layout = NULL;
		ID3D11VertexShader  *vertexShader = NULL;
		ID3D11PixelShader	*pixelShader = NULL;

		// ���_�V�F�[�_�쐬
		HRESULT hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/SkinMeshAnimation.vert" ) , NULL , NULL , "mainVS" , CDevice_->GetMember()->GetVSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			OutputDebugStringA( ( const char * )pError->GetBufferPointer() ); pError->Release();
			SKY_PANIC_MSG( _T( "���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂��� \n" ) );
		};
		hr = pD3DDevice->CreateVertexShader( ( const DWORD * )pShader->GetBufferPointer(), pShader->GetBufferSize() , NULL , &vertexShader );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂��� \n" ) );
		};
		spShaderInfo->m_Pass1.m_spVertexShader.SetPtr( vertexShader );

		// ���_���C�A�E�g�̍쐬
		D3D11_INPUT_ELEMENT_DESC ElementDesc[] = {
			{ "SV_POSITION", 0 , DXGI_FORMAT_R32G32B32_FLOAT,    0 , 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",    0 , DXGI_FORMAT_R32G32_FLOAT,      1 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 } ,
			{ "BONE_INDEX"   , 0 , DXGI_FORMAT_R32G32B32A32_FLOAT, 2 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 } ,
			{ "BONE_WEIGHT"   , 0 , DXGI_FORMAT_R32G32B32A32_FLOAT, 3 , 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
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

        // �O���[�o���ϐ��o�b�t�@�쐬
		ID3D11Buffer  *pConstantBuffer;
		D3D11_BUFFER_DESC ConstDesc;
		ConstDesc.Usage				  = D3D11_USAGE_DEFAULT;
		ConstDesc.BindFlags			  = D3D11_BIND_CONSTANT_BUFFER;
		ConstDesc.CPUAccessFlags	  = 0;
		ConstDesc.MiscFlags			  = 0;
		ConstDesc.StructureByteStride = 0;
		ConstDesc.ByteWidth			  = sizeof( MotionShaderConst );
		hr = pD3DDevice->CreateBuffer( &ConstDesc , NULL , &pConstantBuffer );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂��� \n" ) );
		}
		spShaderInfo->m_spVertexConstBuffer0.SetPtr( pConstantBuffer );

		// �s�N�Z���V�F�[�_�쐬
		hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/SkinMeshAnimation.frag" ) , NULL , NULL , "mainPS" , CDevice_->GetMember()->GetPSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			OutputDebugStringA( (const char*)pError->GetBufferPointer() ); pError->Release();
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

		//ID�Ɋ֘A�Â��V�F�[�_�[�t�@�C���ۑ�����
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CSkinMeshAnimationShaderObject::Pass1Draw()
{
	//�萔�o�b�t�@�󂯓n���p�̕ϐ�
	MotionShaderConst shaderConst;

	//���[���h�s�� * �r���[�s�� * �v���W�F�N�V�����s��̐ݒ�
	XMMATRIX	mat;
	D3DUtil::Convert4x3Matrix( MulMatrix4x4( m_WorldMatrix , m_spCamera->GetCameraMatrix() ) , mat );
	XMStoreFloat4x4( &shaderConst.WorldViewProjectionMatrix , XMMatrixTranspose( mat ) );

	CMesh *pMesh = ( CMesh * )m_spMesh.GetPtrNoConst();

	u32 roopNum = ( BONE_MAX < pMesh->GetSkeleton()->m_spJointPallet->GetSkinTransformMatrixArray().GetNum() ) ? BONE_MAX : pMesh->GetSkeleton()->m_spJointPallet->GetSkinTransformMatrixArray().GetNum();
	for ( u32 i = 0 ; i < roopNum ; i++ )
	{
		D3DUtil::Convert4x3Matrix( pMesh->GetSkeleton()->m_spJointPallet->GetSkinTransformMatrixArray()[ i ] , mat );
		XMStoreFloat4x4( &shaderConst.Joints[ i ] , XMMatrixTranspose( mat ) );
	}

//-----�`�悷��-----��

	//�f�o�C�X���擾
	ID3D11DeviceContext	*pD3DDeviceContext	= CDevice_->GetMember()->m_spD3DDeviceContext.GetPtrNoConst();

	//IA�ɓ��̓��C�A�E�g�E�I�u�W�F�N�g��ݒ�
	pD3DDeviceContext->IASetInputLayout( m_spShaderInfo->m_Pass1.m_spVertexLayout.GetPtrNoConst() );

	//VS�ɒ��_�V�F�[�_��ݒ�
	pD3DDeviceContext->VSSetShader( m_spShaderInfo->m_Pass1.m_spVertexShader.GetPtrNoConst() , NULL , 0 );

	//VS�ɒ萔�o�b�t�@��ݒ�
	CDevice_->GetMember()->m_spD3DDeviceContext->VSSetConstantBuffers( 0, 1, m_spShaderInfo->m_spVertexConstBuffer0.GetPtrPtr() );

	//PS�Ƀs�N�Z���E�V�F�[�_��ݒ�
	pD3DDeviceContext->PSSetShader( m_spShaderInfo->m_Pass1.m_spPixelShader.GetPtrNoConst() , NULL , 0 );  // �s�N�Z���V�F�[�_�[

	// PS�ɒ萔�o�b�t�@��ݒ�
	CDevice_->GetMember()->m_spD3DDeviceContext->PSSetConstantBuffers( 0 , 1 , m_spShaderInfo->m_spVertexConstBuffer0.GetPtrPtr() );

	CMaterialContena *pMaterialContena = ( CMaterialContena * )pMesh->GetMaterialContena().GetPtrNoConst();
	FOREACH( CSceneGraphNodePtrList , itMat , pMaterialContena->GetMaterialDataArray() )
	{
		CMaterialDrawDataPtr spMatDrawData = *itMat;
		//�f�B�r���[�Y�}�b�v���g�p����
		if ( spMatDrawData->GetMaterial()->GetDiffuseMap().IsEmpty() == skyFalse )
		{
			spMatDrawData->GetMaterial()->GetDiffuseMap()->SetShader();
		}

		//�萔�p�����[�^�[���󂯓n��
		CDevice_->GetMember()->m_spD3DDeviceContext->UpdateSubresource( m_spShaderInfo->m_spVertexConstBuffer0.GetPtrNoConst() , 0, NULL , &shaderConst, 0, 0 );

		//�X�g���[����ݒ肷��
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( pMesh->GetVertexBuffer()->GetPositionStream() );
		streamList.AddTail( pMesh->GetVertexBuffer()->GetUvStream() );
		streamList.AddTail( pMesh->GetVertexBuffer()->GetIndexStream() );
		streamList.AddTail( pMesh->GetVertexBuffer()->GetWeightsStream() );
		CVertexUtil::SetRenderBufferStream( streamList , pMesh->GetIndexBuffer() , pMesh->GetVertexBuffer()->GetPrimitiveType() );

		//���_�̕`��
		pMesh->GetVertexBuffer()->Draw( spMatDrawData->m_DrawStartIndex , spMatDrawData->m_DrawEndIndex );
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

static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	SKY_ASSERT_MSG( s_pShaderHash != skyNull , _T( "The Shader creator has not been initialized." ) ); //�V�F�[�_�[�N���G�[�^�[������������Ă��܂���B

	CShaderInfoPtr spShaderInfo;

	//�V�F�[�_�[ID�ɕR�Â��V�F�[�_�[���Ȃ�������V�F�[�_�[�����
	if ( s_pShaderHash->Get( shaderId , spShaderInfo ) == skyFalse )
	{
		spShaderInfo.SetPtr( NEW CShaderInfo() );
	}

	return spShaderInfo;
}

void CSkinMeshAnimationShaderObject::Pass1Draw()
{
}

#endif

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	SKY_ASSERT_MSG( s_pShaderHash != skyNull , _T( "The Shader creator has not been initialized." ) ); //�V�F�[�_�[�N���G�[�^�[������������Ă��܂���B

	CShaderInfoPtr spShaderInfo;

	//�V�F�[�_�[ID�ɕR�Â��V�F�[�_�[���Ȃ�������V�F�[�_�[�����
	if ( s_pShaderHash->Get( shaderId , spShaderInfo ) == skyFalse )
	{
		spShaderInfo.SetPtr( NEW CShaderInfo() );

		//���ӁjcgDestroyProgram( spShaderInfo->m_ProgramObject0.Program ); �Ŕj��
		CCg::CreateVertexFragmentCombineShaderFromFile( spShaderInfo->m_Pass1.m_ProgramObject , 
		                                            "data/lib/shader/cg/SkinMeshAnimation.vert" , "mainVS" ,
		                                            "data/lib/shader/cg/SkinMeshAnimation.frag" , "mainPS"  );

		spShaderInfo->m_Pass1.m_WvpLocation = cgGetNamedParameter ( spShaderInfo->m_Pass1.m_ProgramObject.Program , "WorldViewProjection" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
		spShaderInfo->m_Pass1.m_JointsLocation = cgGetNamedParameter ( spShaderInfo->m_Pass1.m_ProgramObject.Program , "Joints" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );

        spShaderInfo->m_Pass1.m_TexCoord0Location = cgGetNamedParameter( spShaderInfo->m_Pass1.m_ProgramObject.Program , "texCoord0Location" );//���O
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );

		//ID�Ɋ֘A�Â��V�F�[�_�[�t�@�C���ۑ�����
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CSkinMeshAnimationShaderObject::Pass1Draw()
{
	CMesh *pMesh = ( CMesh * )m_spMesh.GetPtrNoConst();

	float WorldViewProjection [ 16 ];
	( MulMatrix4x4( m_WorldMatrix , m_spCamera->GetCameraMatrix() ) ).CopyArray< f32 >( WorldViewProjection );
	cgSetMatrixParameterfr( m_spShaderInfo->m_Pass1.m_WvpLocation , WorldViewProjection );
    CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );

	u32 size = pMesh->GetSkeleton()->GetJointNum();
	cgGLSetMatrixParameterArrayfr( m_spShaderInfo->m_Pass1.m_JointsLocation , 0 , size , ( const float * )pMesh->GetSkeleton()->GetSkinTransformMatrixArray().GetTopAdr() );
    CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );

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

	CVertexBufferPtr spVertexBuffer = pMesh->GetVertexBuffer();
	CMaterialContena *pMaterialContena = ( CMaterialContena * )pMesh->GetMaterialContena().GetPtrNoConst();
	FOREACH( CSceneGraphNodePtrList , itMat , pMaterialContena->GetMaterialDataArray() )
	{
		CMaterialDrawDataPtr spMatDrawData = *itMat;

		//�f�B�r���[�Y�}�b�v�e�N�X�`�����g�p����
		if ( spMatDrawData->GetMaterial()->GetDiffuseMap().IsEmpty() == skyFalse )
		{
			cgGLSetTextureParameter( m_spShaderInfo->m_Pass1.m_TexCoord0Location , spMatDrawData->GetMaterial()->GetDiffuseMap()->GetMember()->texId[ 0 ] ); //�e�N�X�`���Ƃ̊֘A�t��
			CCg::CheckForCgError( "�e�N�X�`���̐ݒ�Ɏ��s���܂����B" );
			cgGLEnableTextureParameter( m_spShaderInfo->m_Pass1.m_TexCoord0Location );
			CCg::CheckForCgError( "�e�N�X�`����L���ɂł��܂���ł����B" );
		}

		//�X�g���[����ݒ肷��
		spVertexBuffer->GetPositionStream()->GetMember()->m_Location = 0;
		spVertexBuffer->GetUvStream()->GetMember()->m_Location   = 1;
		spVertexBuffer->GetIndexStream()->GetMember()->m_Location = 2;
		spVertexBuffer->GetWeightsStream()->GetMember()->m_Location = 3;
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( spVertexBuffer->GetPositionStream() );
		streamList.AddTail( spVertexBuffer->GetUvStream() );
		streamList.AddTail( spVertexBuffer->GetIndexStream() );
		streamList.AddTail( spVertexBuffer->GetWeightsStream() );
		CVertexUtil::SetRenderBufferStream( streamList , pMesh->GetIndexBuffer() );

		//���b�V�����ɕ`��
		spVertexBuffer->Draw( 0 , 0 , pMesh->GetIndexBuffer() );

		//�f�B�r���[�Y�}�b�v�e�N�X�`����j������
		if ( spMatDrawData->GetMaterial()->GetDiffuseMap().IsEmpty() == skyFalse )
		{
			cgGLDisableTextureParameter( m_spShaderInfo->m_Pass1.m_TexCoord0Location );
			CCg::CheckForCgError( "�e�N�X�`���𖳌��ɂł��܂���ł����B" );
		}
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
