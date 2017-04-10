#pragma once

namespace sky { namespace lib { namespace graphic {

	/// <summary>
	/// �T���v���[�f�[�^
	/// �e�T���v���[�̎g�����̓|�X�g�G�t�F�N�g�ɂ��
	/// </summary>
	struct CSamplerData
	{
		/// <summary>
		/// �T�u�T���v���[�O
		/// </summary>
		CTexturePtr m_Sampler0;

		/// <summary>
		/// �T�u�T���v���[�P
		/// </summary>
		CTexturePtr m_Sampler1;

		/// <summary>
		/// �T�u�T���v���[�Q
		/// </summary>
		CTexturePtr m_Sampler2;

		/// <summary>
		/// ������Z�q
		/// </summary>
		CSamplerData& operator =( const CSamplerData& sampler )
		{
			m_Sampler0 = sampler.m_Sampler0;
			m_Sampler1 = sampler.m_Sampler1;
			m_Sampler2 = sampler.m_Sampler2;

			return *this;
		}
	};

} } }

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// �V�F�[�_�[�C���X�^���X�i�[Hash�T�C�Y
/// </summary>
static const u32 SHADER_HASH_SIZE = 31;

/// <summary>
/// �c�R�c���_�V�F�[�_�[
/// </summary>
typedef SmartPointerCom< ID3D11VertexShader > ID3D11VertexShaderPtr;

/// <summary>
/// �c�R�c�V�F�[�_�[�O���[�o���ϐ��o�b�t�@
/// </summary>
typedef SmartPointerCom< ID3D11Buffer >       ID3DXConstantBufferPtr;

/// <summary>
/// �c�R�c�s�N�Z���V�F�[�_�[
/// </summary>
typedef SmartPointerCom< ID3D11PixelShader >  ID3D11PixelShaderPtr;

/// <summary>
/// �c�R�c�s�N�Z���V�F�[�_�[
/// </summary>
typedef SmartPointerCom< ID3D11InputLayout >  ID3D11InputLayoutPtr;

/// <summary>
///  �p�X�����
/// </summary>
struct PassInfo
{
	ID3D11VertexShaderPtr  m_spVertexShader;
	ID3D11PixelShaderPtr   m_spPixelShader;
	ID3D11InputLayoutPtr   m_spVertexLayout;
};

/// <summary>
///  �V�F�[�_�[���N���X
/// </summary>
struct CShaderInfo : public base::SkyRefObject
{
	//�V�F�[�_�[�p�X���
	PassInfo           m_Pass1;
	PassInfo           m_Pass2;
	PassInfo           m_Pass3;

	/// <summary>
	///  �O���[�o���ϐ��e�[�u��
	/// </summary>
	ID3DXConstantBufferPtr m_spVertexConstBuffer0;
	ID3DXConstantBufferPtr m_spVertexConstBuffer1;
	ID3DXConstantBufferPtr m_spVertexConstBuffer2;
	ID3DXConstantBufferPtr m_spVertexConstBuffer3;
	ID3DXConstantBufferPtr m_spVertexConstBuffer4;
	ID3DXConstantBufferPtr m_spVertexConstBuffer5;
	ID3DXConstantBufferPtr m_spVertexConstBuffer6;
	ID3DXConstantBufferPtr m_spVertexConstBuffer7;
	ID3DXConstantBufferPtr m_spVertexConstBuffer8;
};
typedef SmartPointer< CShaderInfo >	CShaderInfoPtr;		//�X�}�[�g�|�C���^�Œ�`
typedef CHash< CShaderInfoPtr >		CShaderInfoPtrHash;	//Hash��`

} } }

// �V�F�[�_�̃R���p�C�� �I�v�V����
#if defined( DEBUG ) || defined( _DEBUG )
static const u16 SHADER_COMPILE_OPTION = D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION | D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR;
#else
static const u16 SHADER_COMPILE_OPTION = D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR;
#endif

#endif

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// �V�F�[�_�[�C���X�^���X�i�[Hash�T�C�Y
/// </summary>
static const u32 SHADER_HASH_SIZE = 31;

/// <summary>
///  UBO���
/// </summary>
struct UBOInfo
{
	GLuint id[1];
	GLuint index;
	GLint  blockSize;
};
#define UBO_MAX_NUM 16

/// <summary>
///  �p�X�����
/// </summary>
struct PassInfo
{
#ifdef SW_SKYGRAPHICLIB_USE_GLSL_ON
	GLuint             m_ProgramObject;
#else
	CCgCombineProgram  m_ProgramObject;
#endif

#ifdef SW_SKYGRAPHICLIB_USE_GLSL_ON
#define LOCATION GLint
#endif
#ifdef SW_SKYGRAPHICLIB_USE_CG_ON
#define LOCATION CGparameter
#endif

	//���_�����i�e���f���̊e���_���̏��j
	LOCATION  m_VertexLocation;
	LOCATION  m_NormalLocation;
	LOCATION  m_TexCoord0Location;

	//�C���X�^���X�����i�e���f�����̏��j
    LOCATION m_WvpLocation;
    LOCATION m_WvptLocation;
    LOCATION m_WorldLocation;
    LOCATION m_ViewLocation;
    LOCATION m_ProjectionLocation;
	LOCATION m_JointsLocation;
    LOCATION m_Texture0Location;
    LOCATION m_Texture1Location;
    LOCATION m_Texture2Location;

	//�}�e���A��
    LOCATION m_AmbientLocation;
    LOCATION m_DiffuseLocation;
    LOCATION m_SpecularLocation;
    LOCATION m_AlphaLocation;
    LOCATION m_PowerLocation;

	//���C�g
    LOCATION m_LightAmbientLocation;
    LOCATION m_LightDiffuseLocation;
	LOCATION m_LightSpecularLocation;

	//�x�N�g��
    LOCATION m_LightDirLocation;
    LOCATION m_ViewDirLocation;

	//���R�Ɏg���Ă悵�B�z��ɂ��Ȃ������͖̂�肪�N����̂�������������B
	LOCATION m_FreeLocation0;
	LOCATION m_FreeLocation1;
	LOCATION m_FreeLocation2;
	LOCATION m_FreeLocation3;
	LOCATION m_FreeLocation4;
	LOCATION m_FreeLocation5;
	LOCATION m_FreeLocation6;
	LOCATION m_FreeLocation7;
	LOCATION m_FreeLocation8;
	LOCATION m_FreeLocation9;

	UBOInfo  m_Ubo[ UBO_MAX_NUM ];
};

/// <summary>
///  �V�F�[�_�[���N���X
/// </summary>
struct CShaderInfo : public base::SkyRefObject
{
	//�V�F�[�_�[�p�X���
	PassInfo           m_Pass1;
	PassInfo           m_Pass2;
	PassInfo           m_Pass3;
};
typedef SmartPointer< CShaderInfo >	CShaderInfoPtr;		//�X�}�[�g�|�C���^�Œ�`
typedef CHash< CShaderInfoPtr >		CShaderInfoPtrHash;	//Hash��`

#define BUFFER_OFFSET( bytes ) ( ( GLubyte * )NULL + ( bytes ) )

} } }

#endif
