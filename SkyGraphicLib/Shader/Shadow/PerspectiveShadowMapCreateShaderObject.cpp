#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"

//===================================�yCommon�z===================================================>

SkyImplementRTTI( sky::lib::graphic::CPerspectiveShadowMapCreateShaderObject       , sky::lib::graphic::CShaderObject );

namespace sky { namespace lib { namespace graphic {

static CShaderInfoPtrHash *s_pShaderHash = skyNull;
static CShaderInfoPtr GetShaderInfo( u32 shaderId );

skyBool CPerspectiveShadowMapCreateShaderObject::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_UNDER_CONSTRUCTION( stream );
	return skyFalse;
}

skyBool CPerspectiveShadowMapCreateShaderObject::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_UNDER_CONSTRUCTION( stream );
	return skyFalse;
}

const serialize::CSerializeInterfacePtr& CPerspectiveShadowMapCreateShaderObject::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CPerspectiveShadowMapCreateShaderObject::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CPerspectiveShadowMapCreateShaderObject::Clone()
{
	return Copy( CPerspectiveShadowMapCreateShaderObjectPtr( NEW__( CPerspectiveShadowMapCreateShaderObject , CPerspectiveShadowMapCreateShaderObject() ) ) );
}

serialize::CSerializeInterfacePtr CPerspectiveShadowMapCreateShaderObject::CloneSharing()
{
	return Clone();
}

CPerspectiveShadowMapCreateShaderObject::CPerspectiveShadowMapCreateShaderObject()
{
	m_PassNum = 1;
	m_LightView.Identity();
	m_LightProjection.Identity();
	m_LightViewProjection.Identity();
	m_PsmMatrix.Identity();
	m_View.Identity();
	m_Projection.Identity();
	m_ViewProjection.Identity();
	m_WorldSpaceLight = math::CBasicVector4_ZERO;
	m_NearClip = 1.0f;
	m_FarClip = 50.0f;
}

void CPerspectiveShadowMapCreateShaderObject::SetShadowLightPosition( const math::CBasicVector4 &lightPos )
{
	m_WorldSpaceLight = lightPos;
}

void CPerspectiveShadowMapCreateShaderObject::vDraw()
{
	//�V�F�[�_�[ID�̍\�z�iCPerspectiveShadowMapCreateShaderObject�ł̓��C�g�̏�Ԃɉ����ăV�F�[�_�[�����j
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

	//�X�V����
	UpdateShadowMatrix();

	//�p�X���̕`��
	if ( m_RenderingPassNo == 0 )
	{
		Pass1Draw();
	}
}

void CPerspectiveShadowMapCreateShaderObject::UpdateShadowMatrix()
{
	//���[���h��Ԃ̃��C�g
	math::CBasicVector4 wsLight = m_WorldSpaceLight;
	wsLight.Normalize();

	//�J�����̍s������߂�
	CalcCameraMtx(wsLight);

	// World Space�@���@Post Perspective Space
	math::CBasicVector4 ppsLight = MulMatrix4x4( wsLight, m_ViewProjection );

	//���C�g�̍s������߂�
	if ( math::CMathUtil::Abs( ppsLight.W ) < DBL_EPSILON )
	{
		//���s�����̏ꍇ
		CalcLightMtx_ParalleLight(ppsLight);
	}
	else
	{
		//�_�����̏ꍇ
		CalcLightMtx_PointLight(ppsLight);
	}
			
	//PSM�ɗp����s������߂�
	m_PsmMatrix = MulMatrix4x4Full( m_ViewProjection , m_LightViewProjection );

}

void CPerspectiveShadowMapCreateShaderObject::CalcCameraMtx( const math::CBasicVector4 &wsLight )
{
#ifdef VIEW_LEFT_HAND
	//�r���[�s��E�ˉe�s����쐬
	m_View.SetupViewLH( m_spCamera->GetData().m_Eye, m_spCamera->GetData().m_Tag, m_spCamera->GetData().m_UpVec );
#endif

#ifdef VIEW_RIGHT_HAND
	//�r���[�s��E�ˉe�s����쐬
	m_View.SetupViewRH( m_spCamera->GetData().m_Eye, m_spCamera->GetData().m_Tag, m_spCamera->GetData().m_UpVec );
#endif

	dectype d = 0.0f;

	// Check1 : �߃N���b�v�ʂ܂ł̍ŏ�����
	if ( m_NearClip < MIN_Z_NEAR )
	{
		d = MIN_Z_NEAR - m_NearClip;
	}

	// Check2 : Viewer�̌�납��e���L���X�g����I�u�W�F�N�g�����邩�H
	math::CBasicVector3 col2;
	math::CBasicVector4 viewDir( m_View.GetCol2( col2 ) , 0.0f );
	viewDir.Normalize();

	//���C�g�Ǝ����x�N�g���̂Ȃ��p���Z�o
	dectype dot = wsLight.Dot( viewDir );

	//�J�����̌�납�烉�C�g�������邩����
	if ( dot < 0.0f && ( f32 )math::CMathUtil::Abs(wsLight.Y) > FLT_EPSILON)
	{
		dectype t = -( MAX_Y_SCENE - m_spCamera->GetData().m_Eye.Y ) / wsLight.Y;
		d = TemplateUtil::Max< dectype >( d , t * ( dectype )math::CMathUtil::Sqrt(wsLight.X * wsLight.X + wsLight.Z * wsLight.Z ) );
	}
#ifdef VIEW_LEFT_HAND
	//�r���[�s��E�ˉe�s����쐬
	m_Projection.SetupProjectionLH( m_spCamera->GetData().m_Fov , m_spCamera->GetData().m_Aspect , m_NearClip , m_FarClip );
#endif

#ifdef VIEW_RIGHT_HAND
	//�r���[�s��E�ˉe�s����쐬
	m_Projection.SetupProjectionRH( m_spCamera->GetData().m_Fov , m_spCamera->GetData().m_Aspect , m_NearClip , m_FarClip );
#endif

	//������̒������K�v������
	if ( d > 0.0f )
	{
		//z���ɉ����ăX���C�h�o�b�N
		m_View.Tz -= d;
#ifdef VIEW_LEFT_HAND
		m_Projection.SetupProjectionLH( m_spCamera->GetData().m_Fov , m_spCamera->GetData().m_Aspect , m_NearClip + d, m_FarClip + d);
#endif

#ifdef VIEW_RIGHT_HAND
		m_Projection.SetupProjectionRH( m_spCamera->GetData().m_Fov , m_spCamera->GetData().m_Aspect , m_NearClip + d, m_FarClip + d);
#endif
	}

	//�r���[�ˉe�s����쐬
	m_ViewProjection = MulMatrix4x4Full( m_View , m_Projection );
}


void CPerspectiveShadowMapCreateShaderObject::CalcLightMtx_ParalleLight( const math::CBasicVector4 &ppsLight )
{
#ifdef VIEW_LEFT_HAND
	m_LightView.SetupViewLH( math::CBasicVector3_ZERO , ppsLight.GetVector3() , math::CBasicVector3_UPZ );
	m_LightProjection.SetupProjectionOffsetCenterLH( -SQRT2, SQRT2, -SQRT2, SQRT2, -SQRT2, SQRT2 );
#endif

#ifdef VIEW_RIGHT_HAND
	m_LightView.SetupViewRH( math::CBasicVector3_ZERO , -ppsLight.GetVector3() , math::CBasicVector3_UPZ );
	m_LightProjection.SetupProjectionOffsetCenterRH( -SQRT2, SQRT2, -SQRT2, SQRT2, -SQRT2, SQRT2 );
#endif

	//���C�g�̃r���[�ˉe�s����쐬
	m_LightViewProjection = MulMatrix4x4Full( m_LightView , m_LightProjection );
}

void CPerspectiveShadowMapCreateShaderObject::CalcLightMtx_PointLight( const math::CBasicVector4 &ppsLight_ )
{
	//���[�N�ɕ��荞��
	math::CBasicVector4 ppsLight = ppsLight_;

	//�u���b�N�z�[�����ǂ����`�F�b�N
	if ( ppsLight.W < 0.0f )
	{
		//�}�C�i�X�������Ĕ��]������
		math::CMatrix4x3 scale;
		scale.SetupScale( math::CBasicVector3( 1.0f , 1.0f , -1.0f ) );
		ppsLight = MulMatrix4x4( ppsLight , scale );
	}

	//W�����Ŋ����ĕϊ�
	ppsLight *= ( 1.0f / ppsLight.W );
	ppsLight.W = 0.0f;

	//���C�g�̎�����𒲐�
	dectype radius = SQRT3;
	dectype dist = math::CMathUtil::Sqrt( ppsLight.Dot( ppsLight ) );
	dectype fov = 2.0f * ( dectype )math::CMathUtil::Atan( radius / dist );
	dectype nearDist = TemplateUtil::Max< dectype >( dist - radius , 0.0001f );
	dectype farDist = dist + radius;

#ifdef VIEW_LEFT_HAND
	//���C�g�̃r���[�s��E�ˉe�s����쐬
	m_LightView.SetupViewLH( ppsLight.GetVector3() , math::CBasicVector3_ZERO , math::CBasicVector3_UPZ );
	m_LightProjection.SetupProjectionLH( fov, 1.0f, nearDist, farDist );
#endif

#ifdef VIEW_RIGHT_HAND
	//���C�g�̃r���[�s��E�ˉe�s����쐬
	m_LightView.SetupViewRH( ppsLight.GetVector3() , -math::CBasicVector3_ZERO , math::CBasicVector3_UPZ );
	m_LightProjection.SetupProjectionRH( fov, 1.0f, nearDist, farDist );
#endif

	//���C�g�̃r���[�ˉe�s����쐬
	m_LightViewProjection = MulMatrix4x4( m_LightView , m_LightProjection );

}


} } }

//===================================�yDirectX�z===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

// �萔�o�b�t�@�̃f�[�^��`
struct CPerspectiveShadowMapCreateShaderConst
{
    XMFLOAT4X4 World;
    XMFLOAT4X4 PsmMatrix;
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
		HRESULT hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/PerspectiveShadowMapCreate.vert" ) , NULL , NULL , "mainVS" , CDevice_->GetMember()->GetVSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
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
			{ "SV_POSITION", 0 , DXGI_FORMAT_R32G32B32_FLOAT,  0 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",      0 , DXGI_FORMAT_R32G32B32_FLOAT,  1 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",    0 , DXGI_FORMAT_R32G32_FLOAT,     1 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 } ,
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
		ConstDesc.ByteWidth			  = sizeof( CPerspectiveShadowMapCreateShaderConst );
		hr = pD3DDevice->CreateBuffer( &ConstDesc , NULL , &pConstantBuffer );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂��� \n" ) );
		}
		spShaderInfo->m_spVertexConstBuffer0.SetPtr( pConstantBuffer );

		// �s�N�Z���V�F�[�_�쐬
		hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/PerspectiveShadowMapCreate.frag" ) , NULL , NULL , "mainPS" , CDevice_->GetMember()->GetPSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
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

void CPerspectiveShadowMapCreateShaderObject::Pass1Draw()
{
	//�萔�o�b�t�@�󂯓n���p�̕ϐ�
	CPerspectiveShadowMapCreateShaderConst shaderConst;

	//���[���h�s�� * �r���[�s�� * �v���W�F�N�V�����s��̐ݒ�
	XMMATRIX dw;
	D3DUtil::Convert4x3Matrix( m_WorldMatrix , dw );
	XMStoreFloat4x4( &shaderConst.World , XMMatrixTranspose( dw ) );

	XMMATRIX dPsm;
	D3DUtil::Convert4x3Matrix( m_PsmMatrix , dPsm );
	XMStoreFloat4x4( &shaderConst.PsmMatrix , XMMatrixTranspose( dPsm ) );

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

	CMesh *pMesh = ( CMesh * )m_spMesh.GetPtrNoConst();
	CMaterialContena *pMaterialContena = ( CMaterialContena * )pMesh->GetMaterialContena().GetPtrNoConst();
	FOREACH( CSceneGraphNodePtrList , itMat , pMaterialContena->GetMaterialDataArray() )
	{
		CMaterialDrawDataPtr spMatDrawData = *itMat;

		//�萔�p�����[�^�[���󂯓n��
		CDevice_->GetMember()->m_spD3DDeviceContext->UpdateSubresource( m_spShaderInfo->m_spVertexConstBuffer0.GetPtrNoConst() , 0, NULL , &shaderConst, 0, 0 );

		//�X�g���[����ݒ肷��
		CVertexBufferStreamPtrList streamList;
		streamList.AddTail( pMesh->GetVertexBuffer()->GetPositionStream() );
		streamList.AddTail( pMesh->GetVertexBuffer()->GetNormalStream() );
		streamList.AddTail( pMesh->GetVertexBuffer()->GetUvStream() );
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

		//ID�Ɋ֘A�Â��V�F�[�_�[�t�@�C���ۑ�����
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CPerspectiveShadowMapCreateShaderObject::Pass1Draw()
{
}

#endif //SW_SKYGRAPHICLIB_USE_GLSL_ON

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	CShaderInfoPtr spShaderInfo;

	//�V�F�[�_�[ID�ɕR�Â��V�F�[�_�[���Ȃ�������V�F�[�_�[�����
	if ( s_pShaderHash->Get( shaderId , spShaderInfo ) == skyFalse )
	{
		spShaderInfo.SetPtr( NEW CShaderInfo() );

		//ID�Ɋ֘A�Â��V�F�[�_�[�t�@�C���ۑ�����
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CPerspectiveShadowMapCreateShaderObject::Pass1Draw()
{
}

#endif // SW_SKYGRAPHICLIB_USE_CG_ON

} } }

#endif // SKYLIB_RENDER_OPENGL4
