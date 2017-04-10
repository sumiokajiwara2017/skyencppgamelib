#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"

#include "SkyBaseLib/Rand.h"
#include "SkyGraphicLib/Loader.h"
#include "SkyGraphicLib/DataBase.h"

SkyImplementRTTI( sky::lib::graphic::CBillboardBeam       , sky::lib::graphic::CEffectObject );

namespace sky { namespace lib { namespace graphic {

struct CBillboardBeamMember
{
	/// <summary>
	/// ���f��
	/// </summary>
	CModelPtr m_spModel;

	/// <summary>
	/// �e�N�X�`��
	/// </summary>
	CTexturePtr m_spTexture;

	/// <summary>
	/// ��
	/// </summary>
	f32 m_Width;

	/// <summary>
	/// �m�[�h��
	/// </summary>
	vrtxtype m_NodeNum;

	/// <summary>
	/// �J�E���g
	/// </summary>
	f32 m_Count;

	/// <summary>
	/// ����_
	/// </summary>
	math::CBasicVector3Vector m_NodePos;
};

static CShaderInfoPtrHash *s_pShaderHash = skyNull;
static CShaderInfoPtr GetShaderInfo( u32 shaderId );

CBillboardBeam::~CBillboardBeam()
{
	DEL m_pMember;
}

CBillboardBeam::CBillboardBeam()
{
	m_pMember = NEW CBillboardBeamMember();
	m_PassNum = 1;
	m_pMember->m_Count = 0.0f;
	SetJointUse( skyTrue );
	SetAlpha( skyTrue );
}

void CBillboardBeam::Initialize( vrtxtype nodeNum , dectype width , const CTexturePtr &spTexture )
{
	SKY_ASSERT( nodeNum >= 2 );

	m_pMember->m_NodeNum = nodeNum;
	m_pMember->m_Width = width;
	m_pMember->m_NodePos.Reset( nodeNum );

	for( u32 i = 0 ; i < nodeNum ; i++ )
	{
		f32 y = ( 0.2f * i );
		m_pMember->m_NodePos[ i ] = math::CBasicVector3( 0 , y , 0 );
	}

	u32 nVnum  = ( nodeNum * 2 );	//���_��
	u32 nFace  = ( nVnum - 2 );	    //�|���S����
	u32 n4Face = ( nFace / 2 );	    //�l�p�`��
	f32 dv     = 1.0f / ( nodeNum - 1 );

	math::CBasicVector3Vector posArray;
	math::CBasicVector2Vector uvArray;
	graphic::CVector_CColor   colorArray;
	CVector_u16               indexArray;

	posArray.Reset( nVnum );
	uvArray.Reset( nVnum );
	colorArray.Reset( nVnum );
	indexArray.Reset( nFace * 3 );

	//���_���쐬
	for( u32 i = 0 ; i < nodeNum ; i++ )
	{
		s32 nOff2 = i * 2;

		//�ʒu
		posArray[ nOff2 ].X     = m_pMember->m_NodePos[ i ].X - width; 
		posArray[ nOff2 ].Y     = m_pMember->m_NodePos[ i ].Y;
		posArray[ nOff2 ].Z     = m_pMember->m_NodePos[ i ].Z;
		posArray[ nOff2 + 1 ].X = m_pMember->m_NodePos[ i ].X + width; 
		posArray[ nOff2 + 1 ].Y = m_pMember->m_NodePos[ i ].Y;
		posArray[ nOff2 + 1 ].Z = m_pMember->m_NodePos[ i ].Z;

		//�t�u
		uvArray[ nOff2 ].X = 0.0;
		uvArray[ nOff2 ].Y = dv * ( i );
		uvArray[ nOff2 + 1 ].X = 1.0;
		uvArray[ nOff2 + 1 ].Y = dv * ( i );

		//���_�F
		u8 a = ( u8 )( 255.0f * ( 1.0f- dv * ( i ) ) ) ;
		colorArray[ nOff2 ]     = CColor( 1.0f , 1.0f , 1.0f , ( colortype )a / ( colortype )255 );
		colorArray[ nOff2 + 1 ] = CColor( 1.0f , 1.0f , 1.0f , ( colortype )a / ( colortype )255 );
	}

	//�C���f�b�N�X���쐬
	for( u16 i = 0 ; i < n4Face ; i++ )
	{
		u16 nOff6 = ( i * 6 );
		u16 nOff2 = ( i * 2 );

		indexArray[ nOff6 + 0 ] = nOff2 + 0;
		indexArray[ nOff6 + 1 ] = nOff2 + 3;
		indexArray[ nOff6 + 2 ] = nOff2 + 1;

		indexArray[ nOff6 + 3 ] = nOff2 + 0;
		indexArray[ nOff6 + 4 ] = nOff2 + 2;
		indexArray[ nOff6 + 5 ] = nOff2 + 3;
	}

	//�X�v���C�g�̍쐬
	m_pMember->m_spModel = graphic::CModelUtil::Create( posArray , indexArray , uvArray , colorArray , spTexture );
}

void CBillboardBeam::Dispose()
{
}

void CBillboardBeam::SetNodePos( u32 no , const math::CBasicVector3 &pos )
{
	SKY_ASSERT( no < m_pMember->m_NodeNum );

	m_pMember->m_NodePos[ no ] = pos;
}

void CBillboardBeam::vDraw()
{
	math::CMatrix4x3 viewMat = m_spCamera->GetViewMatrix();

	//�J�����̈ʒu
	math::CBasicVector3 vC = m_spCamera->GetData().m_Eye; 

	CVertexBufferStreamPtr posStream = m_pMember->m_spModel->GetMeshContena()->GetMesh()->GetVertexBuffer()->GetPositionStream();

	math::CBasicVector3 pos;
	for( vrtxtype i = 1 ; i < ( m_pMember->m_NodeNum - 1 ) ; i++ )
	{
		//�|�C���g
		math::CBasicVector3 vO = m_pMember->m_NodePos[ i - 1 ];
		math::CBasicVector3 vP = m_pMember->m_NodePos[ i ];
		math::CBasicVector3 vQ = m_pMember->m_NodePos[ i + 1 ];

		//�J�����֌������x�N�g��
		math::CBasicVector3 vZ = ( vC - vP ).Normalize();

		//�ڐ��x�N�g��
		math::CBasicVector3 vT = ( vQ - vO ).Normalize();

		math::CBasicVector3 vR = Cross( vT , vZ );
		math::CBasicVector3 vG = vP + ( vR * m_pMember->m_Width );   
		math::CBasicVector3 vF = vP - ( vR * m_pMember->m_Width );   

		//�ʒu
		vrtxtype nOff2 = i * 2;

		//�ʒu��ύX
		posStream->WritePosition( nOff2 , vG );
		posStream->WritePosition( nOff2 + 1 , vF );
	}

	//�擪
	{
		math::CBasicVector3 vP = m_pMember->m_NodePos[ 0 ];
		math::CBasicVector3 vQ = m_pMember->m_NodePos[ 1 ];

		//�J�����֌������x�N�g��
		math::CBasicVector3 vZ = ( vC - vP ).Normalize();

		//�ڐ��x�N�g��
		math::CBasicVector3 vT = ( vQ - vP ).Normalize();

		//
		math::CBasicVector3 vR = Cross( vT , vZ );
		math::CBasicVector3 vG =  vP + ( vR * m_pMember->m_Width );   
		math::CBasicVector3 vF =  vP - ( vR * m_pMember->m_Width );   

		//�ʒu
		vrtxtype nOff2 = 0;

		posStream->WritePosition( nOff2 , vG );
		posStream->WritePosition( nOff2 + 1 , vF );
	}

	//���K
	{
		math::CBasicVector3 vO = m_pMember->m_NodePos[ m_pMember->m_NodeNum - 2 ];
		math::CBasicVector3 vP = m_pMember->m_NodePos[ m_pMember->m_NodeNum - 1 ];

		//�J�����֌������x�N�g��
		math::CBasicVector3 vZ = ( vC - vP ).Normalize();

		//�ڐ��x�N�g��
		math::CBasicVector3 vT = ( vP - vO ).Normalize();

		//
		math::CBasicVector3 vR = Cross( vT , vZ );
		math::CBasicVector3 vG =  vP + ( vR * m_pMember->m_Width );   
		math::CBasicVector3 vF =  vP - ( vR * m_pMember->m_Width );   

		//�ʒu
		vrtxtype nOff2 = ( m_pMember->m_NodeNum - 1 ) * 2;

		posStream->WritePosition( nOff2 , vG );
		posStream->WritePosition( nOff2 + 1 , vF );
	}

	//GPU�ɔ��f
	posStream->WirteGpuMemory();

	//�V�F�[�_�[ID�̍\�z�iBlinnPhongShaderObject�ł̓��C�g�̏�Ԃɉ����ăV�F�[�_�[�����j
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

	if ( m_RenderingPassNo == 0 )
	{
		//�`��
		DrawPass1();
	}
}

void CBillboardBeamUtil::Move_Thunder( const CBillboardBeamPtr &spEffect )
{
	math::CBasicVector3 p0;

	for( s32 i = 0 ; i < spEffect->m_pMember->m_NodeNum ; i++ )
	{
		p0.X = ( ( dectype )( rand() % 255 ) / 255.0f ) * 0.4f;
		p0.Y = -1.0f + ( f32 )( i ) * 0.2f;
		p0.Z = ( f32 )( i ) * 0.2f;

		spEffect->SetNodePos( i , p0 );
	}
}

//������]
void CBillboardBeamUtil::Move_Rot( const CBillboardBeamPtr &spEffect )
{
	spEffect->m_pMember->m_Count += 0.02f;

	math::CBasicVector3 p0;
	for( s32 i = 0 ; i < spEffect->m_pMember->m_NodeNum ; i++ )
	{
		p0.X = (cos( spEffect->m_pMember->m_Count - ((dectype)i * 0.1f) ) * 1.0f);
		p0.Y = (sin( spEffect->m_pMember->m_Count * 1.5f - ((dectype)i * 0.1f) ) * 1.0f);
		p0.Z = 1.0f + (sin( spEffect->m_pMember->m_Count - ((dectype)i * 0.1f) ) * 1.0f);

		spEffect->SetNodePos( i , p0 );
	}
}

} } }

//===================================�yDirectX�z===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

//���_�V�F�[�_�[���͒l�\����
struct BillboardBeamShaderConst
{
	//���_�ϊ��s��
    XMFLOAT4X4 WorldViewProjectionMatrix;
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

		ID3DBlob            *pShader      = NULL;
		ID3DBlob            *pError       = NULL;
		ID3D11InputLayout   *layout       = NULL;
		ID3D11VertexShader  *vertexShader = NULL;
		ID3D11PixelShader	*pixelShader  = NULL;

		// ���_�V�F�[�_�쐬
		HRESULT hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/BillboardBeam.vert" ) , NULL , NULL , "mainVS" , CDevice_->GetMember()->GetVSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
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

		// ���_���C�A�E�g�̍쐬
		D3D11_INPUT_ELEMENT_DESC ElementDesc[] = {
			{ "SV_POSITION", 0 , DXGI_FORMAT_R32G32B32_FLOAT   , 0 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 } ,
			{ "COLOR"      , 0 , DXGI_FORMAT_R32G32B32A32_FLOAT, 1 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 } ,
			{ "TEXCOORD"   , 0 , DXGI_FORMAT_R32G32_FLOAT,       2 , 0 , D3D11_INPUT_PER_VERTEX_DATA, 0 } ,
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
		D3D11_BUFFER_DESC ConstDesc;
		ID3D11Buffer  *pConstantBuffer;
		ConstDesc.Usage				  = D3D11_USAGE_DEFAULT;
		ConstDesc.BindFlags			  = D3D11_BIND_CONSTANT_BUFFER;
		ConstDesc.CPUAccessFlags	  = 0;
		ConstDesc.MiscFlags			  = 0;
		ConstDesc.StructureByteStride = 0;
		ConstDesc.ByteWidth			  = sizeof( BillboardBeamShaderConst );
		hr = pD3DDevice->CreateBuffer( &ConstDesc, NULL, &pConstantBuffer );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂��� \n" ) );
		}
		spShaderInfo->m_spVertexConstBuffer0.SetPtr( pConstantBuffer );

		// �s�N�Z���V�F�[�_�쐬
		hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/BillboardBeam.frag" ) , NULL , NULL , "mainPS" , CDevice_->GetMember()->GetPSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
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
			SKY_PANIC_MSG( _T( "���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂��� \n" ) );
		};
		pShader->Release();
		spShaderInfo->m_Pass1.m_spPixelShader.SetPtr( pixelShader );

		//ID�Ɋ֘A�Â��V�F�[�_�[�t�@�C���ۑ�����
		s_pShaderHash->Set( shaderId , spShaderInfo );
	}

	return spShaderInfo;
}

void CBillboardBeam::DrawPass1()
{
	//�萔�o�b�t�@�󂯓n���p�̕ϐ�
	BillboardBeamShaderConst shaderConst;

	//���[���h�s�� * �r���[�s�� * �v���W�F�N�V�����s��̐ݒ�
	XMMATRIX	mat;
	D3DUtil::Convert4x3Matrix( MulMatrix4x4( m_WorldMatrix , m_spCamera->GetCameraMatrix() ) , mat );
	XMStoreFloat4x4( &shaderConst.WorldViewProjectionMatrix , XMMatrixTranspose( mat ) );

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

	CMesh *pMesh = ( CMesh * )m_pMember->m_spModel->GetMeshContena()->GetMesh().GetPtrNoConst();
	CMaterialContena *pMaterialContena = ( CMaterialContena * )pMesh->GetMaterialContena().GetPtrNoConst();
	FOREACH( CSceneGraphNodePtrList , itMat , pMaterialContena->GetMaterialDataArray() )
	{
		CMaterialDrawDataPtr spMatDrawData = *itMat;
		//�A���t�@�l
		shaderConst.Alpha = spMatDrawData->GetMaterial()->Alpha;

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
		streamList.AddTail( pMesh->GetVertexBuffer()->GetColorStream() );
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

#ifdef SKYLIB_RENDER_OPENGL4

#ifdef SW_SKYGRAPHICLIB_USE_GLSL_ON

namespace sky { namespace lib { namespace graphic {

static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	return CShaderInfoPtr();
}

} } }

#endif

#ifdef SW_SKYGRAPHICLIB_USE_CG_ON

namespace sky { namespace lib { namespace graphic {

static CShaderInfoPtr GetShaderInfo( u32 shaderId )
{
	return CShaderInfoPtr();
}

} } }

#endif

#endif
