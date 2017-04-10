#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"

#include "SkyGraphicLib/Loader.h"
#include "SkyGraphicLib/DataBase.h"

SkyImplementRTTI( sky::lib::graphic::CBeamChargeEffect       , sky::lib::graphic::CEffectObject );

namespace sky { namespace lib { namespace graphic {

static const skyString * s_pResTable[] = { 
	_T( "data/lib/effect/BeamChargeEffect/BeamCharge_Tgt.x" ) , 
	_T( "data/lib/effect/BeamChargeEffect/Line.png" ) ,
	_T( "data/lib/effect/BeamChargeEffect/ColorPallet.png" ) ,
	_T( "data/lib/effect/BeamChargeEffect/random1024.bmp" ) ,
};
#define RES_TBL_NUM array_sizeof( s_pResTable )

struct CBeamChargeEffectMember
{
	/// <summary>
	/// ���f��
	/// </summary>
	CModelPtr m_spModel;
};

struct CBeamChargeEffectStaticMember
{
	/// <summary>
	/// �e�N�X�`��
	/// </summary>
	CTexturePtr m_spLineTexture;
	CTexturePtr m_spColorPalletTexture;
	CTexturePtr m_spRandom1024;
};
static CBeamChargeEffectStaticMember *s_pStaticMember = skyNull;
static CShaderInfoPtrHash            *s_pShaderHash = skyNull;
static CShaderInfoPtr GetShaderInfo( u32 shaderId );

CBeamChargeEffect::~CBeamChargeEffect()
{
	DEL m_pMember;
}

CBeamChargeEffect::CBeamChargeEffect()
{
	m_pMember = NEW CBeamChargeEffectMember();
	m_PassNum = 1;
}

void CBeamChargeEffect::Load()
{
	if ( m_pMember->m_spModel.IsEmpty() == skyTrue )
	{
		CXFileLoader::LoadFile( CHS( s_pResTable[ 0 ] ) );
		m_pMember->m_spModel = CDataBase::GetModelIndex( 0 );
		m_pMember->m_spModel->ActivateTraverse();
		CDataBase_::Clear();
	}
}

void CBeamChargeEffect::LoadStatic()
{
	if ( s_pStaticMember == skyNull )
	{
		s_pStaticMember = NEW CBeamChargeEffectStaticMember();

		s_pStaticMember->m_spLineTexture        = CTextureCreateName_( CHS( s_pResTable[ 1 ] ) );
		s_pStaticMember->m_spColorPalletTexture = CTextureCreateName_( CHS( s_pResTable[ 2 ] ) );
		s_pStaticMember->m_spRandom1024         = CTextureCreateName_( CHS( s_pResTable[ 3 ] ) );

		s_pStaticMember->m_spLineTexture->Load();
		s_pStaticMember->m_spColorPalletTexture->Load();
		s_pStaticMember->m_spRandom1024->Load();

		s_pShaderHash = NEW__( CShaderInfoPtrHash , CShaderInfoPtrHash( SHADER_HASH_SIZE ) );
	}
}

void CBeamChargeEffect::Destroy()
{
	m_pMember->m_spModel.Delete();
}

void CBeamChargeEffect::DestroyStatic()
{
	SAFE_DELETE( s_pStaticMember );
	SAFE_DELETE( s_pShaderHash );
}

void CBeamChargeEffect::vDraw()
{
	SKY_ASSERT( s_pStaticMember != skyNull ); //���\�[�X�ǂ�ł���g���Ă�

	//�V�F�[�_�[ID�̍\�z�iBlinnPhongShaderObject�ł̓��C�g�̏�Ԃɉ����ăV�F�[�_�[�����j
	u32 shaderId = 0;

	//�V�F�[�_�[ID���ω����Ă�����V�F�[�_�[���擾
	if ( m_spShaderInfo.IsEmpty() || m_ShaderId != shaderId )
	{
		m_spShaderInfo = GetShaderInfo( shaderId );
		m_ShaderId = shaderId; //���݂̃V�F�[�_�[��ύX
	}

	if ( m_RenderingPassNo == 0 )
	{
		//�`��
		DrawPass1();
	}
}

} } }

//===================================�yDirectX�z===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

//���_�V�F�[�_�[���͒l�\����
struct BeamChargeEffectConst
{
	//���_�ϊ��s��
    XMFLOAT4X4 WorldViewProjectionMatrix;
	FLOAT      LineSize;
	FLOAT      ShotDelay;
	FLOAT      UpSize;
	FLOAT      ShotRand;
	FLOAT      RotationSpd;
	XMFLOAT4   MaterialDiffuse;	
	XMFLOAT3   LightDiffuse;
	FLOAT      Index;
	FLOAT      morph_num;
	FLOAT      morph_t;
	FLOAT      morph_gsi;
	FLOAT      morph_len;
	FLOAT      morph_si;
	FLOAT      morph_de;
	FLOAT      morph_len_r;
	FLOAT      morph_rot;
	FLOAT      param_h;
	FLOAT      param_s;
	FLOAT      param_b;
	FLOAT      param_alpha;
	XMFLOAT3   dummy;
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
		HRESULT hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/BeamCharge.vert" ) , NULL , NULL , "mainVS" , CDevice_->GetMember()->GetVSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
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
		ConstDesc.ByteWidth			  = sizeof( BeamChargeEffectConst );
		hr = pD3DDevice->CreateBuffer( &ConstDesc, NULL, &pConstantBuffer );
		if ( FAILED( hr ) )
		{
			D3DUtil::PrintD3D11Result( hr );
			SKY_PANIC_MSG( _T( "���_�V�F�[�_�[�̃R���p�C���Ɏ��s���܂��� \n" ) );
		}
		spShaderInfo->m_spVertexConstBuffer0.SetPtr( pConstantBuffer );

		// �s�N�Z���V�F�[�_�쐬
		hr = D3DX11CompileFromFile( _T( "data/lib/shader/hlsl/BeamCharge.frag" ) , NULL , NULL , "mainPS" , CDevice_->GetMember()->GetPSVersion() , SHADER_COMPILE_OPTION , 0 , NULL , &pShader , &pError , NULL );
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

void CBeamChargeEffect::DrawPass1()
{
	//�萔�o�b�t�@�󂯓n���p�̕ϐ�
	BeamChargeEffectConst shaderConst;

	//���[���h�s�� * �r���[�s�� * �v���W�F�N�V�����s��̐ݒ�
	XMMATRIX	mat;
	D3DUtil::Convert4x3Matrix( MulMatrix4x4( m_WorldMatrix , m_spCamera->GetCameraMatrix() ) , mat );
	XMStoreFloat4x4( &shaderConst.WorldViewProjectionMatrix , XMMatrixTranspose( mat ) );

	shaderConst.LineSize  = 10;
	shaderConst.ShotDelay = 1;
	shaderConst.UpSize    = 1;
	shaderConst.ShotRand  = 1;
	shaderConst.RotationSpd = 5;
	shaderConst.MaterialDiffuse.x = 1.0f;
	shaderConst.MaterialDiffuse.y = 1.0f;
	shaderConst.MaterialDiffuse.z = 1.0f;
	shaderConst.MaterialDiffuse.w = 1.0f;
	shaderConst.LightDiffuse.x = 1.0f;
	shaderConst.LightDiffuse.y = 1.0f;
	shaderConst.LightDiffuse.z = 1.0f;
	shaderConst.Index = 2;
	shaderConst.morph_num = 6;
	shaderConst.morph_t = 1.0f;
	shaderConst.morph_gsi = 1.0f;
	shaderConst.morph_len = 3.0f;
	shaderConst.morph_si = 1.0f;
	shaderConst.morph_de = 1.0f;
	shaderConst.morph_len_r = 1.0f;
	shaderConst.morph_rot = 1.0f;
	shaderConst.param_h = CHSV_DeepskyBlue.h;
	shaderConst.param_s = CHSV_DeepskyBlue.s;
	shaderConst.param_b = CHSV_DeepskyBlue.v;

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
		shaderConst.param_alpha = spMatDrawData->GetMaterial()->Alpha;

		//�萔�p�����[�^�[���󂯓n��
		CDevice_->GetMember()->m_spD3DDeviceContext->UpdateSubresource( m_spShaderInfo->m_spVertexConstBuffer0.GetPtrNoConst() , 0, NULL , &shaderConst, 0, 0 );

		//�e�N�X�`����ݒ肷��
		s_pStaticMember->m_spRandom1024->SetShader();
		s_pStaticMember->m_spLineTexture->SetShader();
		s_pStaticMember->m_spColorPalletTexture->SetShader();

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
