#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"

SkyImplementRTTI( sky::lib::graphic::CProjectionTextureShaderObject       , sky::lib::graphic::CShaderObject );

//===================================�yCommon�z===================================================>

namespace sky { namespace lib { namespace graphic {

static CShaderInfoPtrHash *s_pShaderHash = skyNull;
static CShaderInfoPtr GetShaderInfo( u32 shaderId );

static const math::CBasicVector3 PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_POS       = math::CBasicVector3( 10.0f, 5.0f, 10.0f );
static const math::CBasicVector3 PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_TAG       = math::CBasicVector3( 0.0f , 0.0f , 0.0f );
static const math::CBasicVector3 PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_UP_VECTOR = math::CBasicVector3( 0 , 1 , 0 );
static const dectype			 PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_NEAR      = 0.25;
static const dectype			 PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_FAR	    = 20; 
static const dectype			 PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_FOV	    = math::util::KPI / 4;
static const dectype			 PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_ASPECT    = 1 / 1;

#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&m_TextureLightPosition.X , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_TextureLightPosition.Y , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_TextureLightPosition.Z , sizeof( d64 ) , isWrite ); \

skyBool CProjectionTextureShaderObject::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "model bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN( skyTrue );

	return result;
}

skyBool CProjectionTextureShaderObject::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "model bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN( skyFalse );

	return result;
}

const serialize::CSerializeInterfacePtr& CProjectionTextureShaderObject::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CProjectionTextureShaderObjectPtr )spContainer )->m_TextureLightPosition = m_TextureLightPosition;
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CProjectionTextureShaderObject::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CProjectionTextureShaderObject::Clone()
{
	return Copy( CProjectionTextureShaderObjectPtr( NEW__( CProjectionTextureShaderObject , CProjectionTextureShaderObject() ) ) );
}

serialize::CSerializeInterfacePtr CProjectionTextureShaderObject::CloneSharing()
{
	return Clone();
}

CProjectionTextureShaderObject::CProjectionTextureShaderObject() : 
m_TextureLightPosition( PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_POS ) ,
m_TextureLightTag( PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_TAG ) ,
m_TextureLightFov( PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_FOV ) ,
m_TextureLightAspect( PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_ASPECT ) ,
m_TextureLightNear( PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_NEAR ) ,
m_TextureLightFar( PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_FAR ) ,
m_TextureLightUp( PROJECTION_TEXTURE_OBJ_DEFAULT_TEXTURE_LIGHT_UP_VECTOR ) 
{
	m_PassNum = 1;
}

void CProjectionTextureShaderObject::vDraw()
{
	//�V�F�[�_�[ID�̍\�z�iProjectionTextureShaderObject�ł̓��C�g�̏�Ԃɉ����ăV�F�[�_�[�����j
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

//===================================�yDirectX�z===================================================>
#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

// �萔�o�b�t�@�̃f�[�^��`
struct ProjectionTextureShaderConst
{
	XMFLOAT4X4 World;		// ���[���h�ϊ��s��
	XMFLOAT4X4 View;		// �r���[�ϊ��s��
	XMFLOAT4X4 Projection;	// �����ϊ��s��
	XMFLOAT4X4 WorldViewProjectionTexture;	//�e�N�X�`�������s��
	XMFLOAT4   Diffuse;		// �f�B�t���[�Y�F
	FLOAT      Alpha;	// �A���t�@�l
	XMFLOAT3   dummy;	// �A���C�����g�p�_�~�[
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
		HRESULT hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/ProjectionTexture.vert" ) , NULL , NULL , "mainVS" , CDevice_->GetMember()->GetVSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
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
			{"POSITION" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
		ConstDesc.ByteWidth			  = sizeof( ProjectionTextureShaderConst );
		hr = pD3DDevice->CreateBuffer( &ConstDesc , NULL , &pConstantBuffer );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂��� \n" ) );
		}
		spShaderInfo->m_spVertexConstBuffer0.SetPtr( pConstantBuffer );

		// �s�N�Z���V�F�[�_�쐬
		hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/ProjectionTexture.frag" ) , NULL , NULL , "mainPS" , CDevice_->GetMember()->GetPSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
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

void CProjectionTextureShaderObject::Pass1Draw()
{
	//�萔�o�b�t�@�󂯓n���p�̕ϐ�
	ProjectionTextureShaderConst shaderConst;

	//���[���h�s�� * �r���[�s�� * �v���W�F�N�V�����s��̐ݒ�
	XMMATRIX dw , dv , dp;
	D3DUtil::Convert4x3Matrix( m_WorldMatrix , dw );
	D3DUtil::Convert4x3Matrix( m_spCamera->GetViewMatrix() , dv );
	D3DUtil::Convert4x3Matrix( m_spCamera->GetProjectionMatrix() , dp );
	XMStoreFloat4x4( &shaderConst.World      , XMMatrixTranspose( dw ) );
	XMStoreFloat4x4( &shaderConst.View       , XMMatrixTranspose( dv ) );
	XMStoreFloat4x4( &shaderConst.Projection , XMMatrixTranspose( dp ) );

	//�e�N�X�`�������ϊ��}�g���b�N�X
	XMMATRIX	texMat;
	math::CMatrix4x3 texViewMat_ , texProjeMat_ , texUvMat_;

	texViewMat_.SetupViewLH( m_TextureLightPosition , m_TextureLightTag , m_TextureLightUp );
	texProjeMat_.SetupProjectionLH(m_TextureLightFov , m_TextureLightAspect , m_TextureLightNear , m_TextureLightFar );
	texUvMat_.SetupTexturePositionMatrix();

	D3DUtil::Convert4x3Matrix( MulMatrix4x4Full( MulMatrix4x4( texViewMat_ , texProjeMat_ ) , texUvMat_ ) , texMat ); //�z���g�͂���ł����Ăق���
//	D3DUtil::Convert4x3Matrix( MulMatrix4x4( MulMatrix4x4( texViewMat_ , texProjeMat_ ) , texUvMat_ ) , texMat );     //���ꂪ��Ԍ����
//	D3DUtil::Convert4x3Matrix( MulMatrix4x4( texViewMat_ , texProjeMat_ ) , texMat );                                 //������܂������
	XMStoreFloat4x4( &shaderConst.WorldViewProjectionTexture , XMMatrixTranspose( texMat ) );

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

	//���b�V���O���[�v���ɕ`�悷��
	CMesh *pMesh = ( CMesh * )m_spMesh.GetPtrNoConst();

	CMaterialContena *pMaterialContena = ( CMaterialContena * )pMesh->GetMaterialContena().GetPtrNoConst();
	FOREACH( CSceneGraphNodePtrList , itMat , pMaterialContena->GetMaterialDataArray() )
	{
		CMaterialDrawDataPtr spMatDrawData = *itMat;

		//�A���t�@�l
		shaderConst.Alpha = spMatDrawData->GetMaterial()->Alpha;

		//�}�e���A�������V�F�[�_�[�Ɉ����p��
		D3DUtil::ConvertColorVector4( spMatDrawData->GetMaterial()->Diffuse , shaderConst.Diffuse );

		//�萔�p�����[�^�[���󂯓n��
		CDevice_->GetMember()->m_spD3DDeviceContext->UpdateSubresource( m_spShaderInfo->m_spVertexConstBuffer0.GetPtrNoConst() , 0, NULL , &shaderConst, 0, 0 );

		//�f�B�r���[�Y�}�b�v���g�p����
		if ( spMatDrawData->GetMaterial()->GetDiffuseMap().IsEmpty() == skyFalse )
		{
			spMatDrawData->GetMaterial()->GetDiffuseMap()->SetShader();
		}

		//�X�g���[����ݒ肷��
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( pMesh->GetVertexBuffer()->GetPositionStream() );
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

		// �V�F�[�_�̓ǂݍ���
		GLuint hm_ProgramObject0;
		sky::lib::graphic::CGLSL::CreateVertexShaderAndFragmentShader(
			hm_ProgramObject0 ,
			_T( "data/lib/shader/glsl/ProjectionTexture.vert" ), 
			_T( "data/lib/shader/glsl/ProjectionTexture.frag" ) );

		spShaderInfo->m_Pass1.m_ProgramObject = hm_ProgramObject0;
 	
		//���_�������
		spShaderInfo->m_Pass1.m_VertexLocation = glGetAttribLocation(spShaderInfo->m_Pass1.m_ProgramObject, "inPos");

		//�O���[�o�����
		spShaderInfo->m_Pass1.m_WorldLocation = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "World");
		spShaderInfo->m_Pass1.m_ViewLocation = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "View");
		spShaderInfo->m_Pass1.m_ProjectionLocation = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "Projection");
		spShaderInfo->m_Pass1.m_WvptLocation = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "WorldViewProjectionTexture");
		spShaderInfo->m_Pass1.m_DiffuseLocation = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "Diffuse");
		spShaderInfo->m_Pass1.m_AlphaLocation       = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "Alpha");
		spShaderInfo->m_Pass1.m_Texture0Location = glGetUniformLocation(spShaderInfo->m_Pass1.m_ProgramObject, "Texture0");

		// Fragment data
#ifndef SKYLIB_IDE_QTCREATER
        glBindFragDataLocation(spShaderInfo->m_Pass1.m_ProgramObject, 0, "fragColor");
#else
        glBindFragDataLocationEXT(spShaderInfo->m_Pass1.m_ProgramObject, 0, "fragColor");
#endif

		//ID�Ɋ֘A�Â��V�F�[�_�[�t�@�C���ۑ�����
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CProjectionTextureShaderObject::Pass1Draw()
{
	//�O���[�o���p�����[�^�[
	GLfloat World     [ 16 ];
	GLfloat View      [ 16 ];
	GLfloat Projection[ 16 ];
	GLfloat WorldViewProjectionTexture[ 16 ];
	GLfloat Diffuse   [ 4 ];

	//�V�F�[�_�[�̐ݒ�
	glUseProgram( m_spShaderInfo->m_Pass1.m_ProgramObject );

	//�v���W�F�N�V�����s��̐ݒ�
	m_spCamera->GetProjectionMatrix().CopyArray< f32 >( Projection );


	//���_���W�ϊ�
	m_spCamera->GetViewMatrix().CopyArray< f32 >( View );

	//���[���h�s��ݒ�
	m_WorldMatrix.CopyArray< f32 >( World );

    // ���W�ϊ��s��
    glUniformMatrix4fv( m_spShaderInfo->m_Pass1.m_WorldLocation , 1 , GL_FALSE , World );
    glUniformMatrix4fv( m_spShaderInfo->m_Pass1.m_ViewLocation , 1 , GL_FALSE , View );
    glUniformMatrix4fv( m_spShaderInfo->m_Pass1.m_ProjectionLocation , 1 , GL_FALSE , Projection ); 

	math::CMatrix4x3 texViewMat_ , texProjeMat_ , texUvMat_;
	texViewMat_.SetupViewRH( m_TextureLightPosition , m_TextureLightTag , m_TextureLightUp );
	texProjeMat_.SetupProjectionRH(m_TextureLightFov , m_TextureLightAspect , m_TextureLightNear , m_TextureLightFar );
	texUvMat_.SetupTexturePositionMatrix();

	MulMatrix4x4Full( MulMatrix4x4( texViewMat_ , texProjeMat_ ) , texUvMat_ ).CopyArray< f32 >( WorldViewProjectionTexture );
    glUniformMatrix4fv( m_spShaderInfo->m_Pass1.m_WvptLocation , 1 , GL_FALSE , WorldViewProjectionTexture ); 

	CMesh *pMesh = ( CMesh * )m_spMesh.GetPtrNoConst();
	CMaterialContena *pMaterialContena = ( CMaterialContena * )pMesh->GetMaterialContena().GetPtrNoConst();
	FOREACH( CSceneGraphNodePtrList , itMat , pMaterialContena->GetMaterialDataArray() )
	{
		CMaterialDrawDataPtr spMatDrawData = *itMat;
		CVertexBufferPtr spVertexBuffer = pMesh->GetVertexBuffer();

		//�������l��ݒ�
		glUniform1fv( m_spShaderInfo->m_Pass1.m_AlphaLocation , 1 , &spMatDrawData->GetMaterial()->Alpha );

		//�}�e���A�������V�F�[�_�[�Ɉ����p��
		Diffuse[ 0 ] = ( GLfloat )spMatDrawData->GetMaterial()->Diffuse.r;
		Diffuse[ 1 ] = ( GLfloat )spMatDrawData->GetMaterial()->Diffuse.g;
		Diffuse[ 2 ] = ( GLfloat )spMatDrawData->GetMaterial()->Diffuse.b;
		Diffuse[ 3 ] = ( GLfloat )spMatDrawData->GetMaterial()->Diffuse.a;
		glUniform4fv( m_spShaderInfo->m_Pass1.m_DiffuseLocation , 1 , Diffuse );

		//�f�B�r���[�Y�}�b�v�e�N�X�`�����g�p����
		if ( spMatDrawData->GetMaterial()->GetDiffuseMap().IsEmpty() == skyFalse )
		{
			//�e�N�X�`���̐ݒ�
			glUniform1i( m_spShaderInfo->m_Pass1.m_Texture0Location , 0 ); // �e�N�X�`�����j�b�g�ԍ� 0
			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D, spMatDrawData->GetMaterial()->GetDiffuseMap()->GetMember()->texId[ 0 ] );
		}

		//�X�g���[����ݒ肷��
		spVertexBuffer->GetPositionStream()->GetMember()->m_Location = m_spShaderInfo->m_Pass1.m_VertexLocation;
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( spVertexBuffer->GetPositionStream() );
		CVertexUtil::SetRenderBufferStream( streamList , pMesh->GetIndexBuffer() );

		//���b�V�����ɕ`��
		spVertexBuffer->Draw( spMatDrawData->m_DrawStartIndex , spMatDrawData->m_DrawEndIndex , pMesh->GetIndexBuffer() );
	}
}

#endif

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	CShaderInfoPtr spShaderInfo;

	//�V�F�[�_�[ID�ɕR�Â��V�F�[�_�[���Ȃ�������V�F�[�_�[�����
	if ( s_pShaderHash->Get( shaderId , spShaderInfo ) == skyFalse )
	{
		spShaderInfo.SetPtr( NEW CShaderInfo() );

		//���ӁjcgDestroyProgram( spShaderInfo->m_ProgramObject0.Program ); �Ŕj��
		CCg::CreateVertexFragmentCombineShaderFromFile( spShaderInfo->m_Pass1.m_ProgramObject , 
		                                            "data/lib/shader/cg/ProjectionTexture.vert" , "mainVS" ,
		                                            "data/lib/shader/cg/ProjectionTexture.frag" , "mainPS"  );

		spShaderInfo->m_Pass1.m_WorldLocation = cgGetNamedParameter ( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.World" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        spShaderInfo->m_Pass1.m_ViewLocation = cgGetNamedParameter ( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.View" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        spShaderInfo->m_Pass1.m_ProjectionLocation = cgGetNamedParameter ( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.Projection" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        spShaderInfo->m_Pass1.m_WvptLocation = cgGetNamedParameter ( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.WorldViewProjectionTexture" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        spShaderInfo->m_Pass1.m_DiffuseLocation = cgGetNamedParameter ( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.Diffuse" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        spShaderInfo->m_Pass1.m_AlphaLocation = cgGetNamedParameter ( spShaderInfo->m_Pass1.m_ProgramObject.Program , "uniforms.Alpha" );
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
        spShaderInfo->m_Pass1.m_TexCoord0Location = cgGetNamedParameter( spShaderInfo->m_Pass1.m_ProgramObject.Program , "texCoord0Location" );//���O
     	CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );

		//ID�Ɋ֘A�Â��V�F�[�_�[�t�@�C���ۑ�����
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CProjectionTextureShaderObject::Pass1Draw()
{
	//�O���[�o���p�����[�^�[
	float World     [ 16 ];
	float View      [ 16 ];
	float Projection[ 16 ];
	float WorldViewProjectionTexture[ 16 ];
	float Diffuse   [ 4 ];

	//�v���W�F�N�V�����s��̐ݒ�
	m_spCamera->GetProjectionMatrix().CopyArray< f32 >( Projection );

	//���_���W�ϊ�
	m_spCamera->GetViewMatrix().CopyArray< f32 >( View );

	//���[���h�s��ݒ�
	m_WorldMatrix.CopyArray< f32 >( World );

	math::CMatrix4x3 vptMat_ , texViewMat_ , texProjeMat_ , texUvMat_;
	texViewMat_.SetupViewRH( m_TextureLightPosition , m_TextureLightTag , m_TextureLightUp );
	texProjeMat_.SetupProjectionRH(m_TextureLightFov , m_TextureLightAspect , m_TextureLightNear , m_TextureLightFar );
	texUvMat_.SetupTexturePositionMatrix();

	MulMatrix4x4Full( MulMatrix4x4( texViewMat_ , texProjeMat_ ) , texUvMat_ ).CopyArray< f32 >( WorldViewProjectionTexture );

	//�s��ݒ�
	cgSetMatrixParameterfc( m_spShaderInfo->m_Pass1.m_ProjectionLocation , Projection );
    CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
	cgSetMatrixParameterfc( m_spShaderInfo->m_Pass1.m_ViewLocation , View );
    CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
	cgSetMatrixParameterfc( m_spShaderInfo->m_Pass1.m_WorldLocation , World );
    CCg::CheckForCgError( "�p�����[�^�[�̐ݒ�Ɏ��s���܂����B" );
	cgSetMatrixParameterfc( m_spShaderInfo->m_Pass1.m_WvptLocation , WorldViewProjectionTexture );
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

	CMesh *pMesh = ( CMesh * )m_spMesh.GetPtrNoConst();
	CMaterialContena *pMaterialContena = ( CMaterialContena * )pMesh->GetMaterialContena().GetPtrNoConst();
	FOREACH( CSceneGraphNodePtrList , itMat , pMaterialContena->GetMaterialDataArray() )
	{
		CMaterialDrawDataPtr spMatDrawData = *itMat;
		CVertexBufferPtr spVertexBuffer = pMesh->GetVertexBuffer();

		//�f�B�r���[�Y�}�b�v�e�N�X�`�����g�p����
		if ( spMatDrawData->GetMaterial()->GetDiffuseMap().IsEmpty() == skyFalse )
		{
			cgGLSetTextureParameter( m_spShaderInfo->m_Pass1.m_TexCoord0Location , spMatDrawData->GetMaterial()->GetDiffuseMap()->GetMember()->texId[ 0 ] ); //�e�N�X�`���Ƃ̊֘A�t��
			CCg::CheckForCgError( "�e�N�X�`���̐ݒ�Ɏ��s���܂����B" );
			cgGLEnableTextureParameter( m_spShaderInfo->m_Pass1.m_TexCoord0Location );
			CCg::CheckForCgError( "�e�N�X�`����L���ɂł��܂���ł����B" );
		}

		//�}�e���A�������V�F�[�_�[�Ɉ����p��
		Diffuse[ 0 ] = ( GLfloat )spMatDrawData->GetMaterial()->Diffuse.r;
		Diffuse[ 1 ] = ( GLfloat )spMatDrawData->GetMaterial()->Diffuse.g;
		Diffuse[ 2 ] = ( GLfloat )spMatDrawData->GetMaterial()->Diffuse.b;
		Diffuse[ 3 ] = ( GLfloat )spMatDrawData->GetMaterial()->Diffuse.a;
		cgSetParameter4fv( m_spShaderInfo->m_Pass1.m_DiffuseLocation , Diffuse );

		// �����x��n��
		cgSetParameter1fv( m_spShaderInfo->m_Pass1.m_AlphaLocation , &spMatDrawData->GetMaterial()->Alpha );

		//�X�g���[����ݒ肷��
		spVertexBuffer->GetPositionStream()->GetMember()->m_Location = 0;
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( spVertexBuffer->GetPositionStream() );
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
