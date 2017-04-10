#pragma once

namespace sky { namespace lib { namespace graphic {

//-------------------------------< �C���^�[�t�F�[�X >----------------------------------------//

/// <summary>
/// �����_�����O�R�}���h�����Q
/// </summary>
class CRenderCommand : public base::SkyObject
{

public:

	/// <summary>
	/// �f�o�C�X�̏�����
	/// </summary>
	static void InitializeDevice( CDevicePtr pDevice , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// �E�B���h�E�̏���
	/// </summary>
	static void SetRadyWindow( const CWindowPtr &spWindow , const math::CBasicRect &rect , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// �t���[���o�b�t�@�����_�����O�^�[�Q�b�g�̏���
	/// </summary>
	static void SetRadyFrameBufferRenderTarget( const CRenderTargetPtr &spRenderTarget , const CWindowPtr &spWindow  , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// �e�N�X�`�������_�����O�^�[�Q�b�g�̏���
	/// </summary>
	static void SetRadyTextureRenderTarget( const CRenderTargetPtr &spRenderTarget , const math::CBasicSize &size , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// �k�x�X�e���V���o�b�t�@�̏���
	/// </summary>
	static void SetRadyDepthStencilBuffer( const CDepthStencilBufferPtr &spBuffer , const CRenderTargetPtr &spRenderTarget , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// �w��̃����_�����O�^�[�Q�b�g�������_�����O�^�[�Q�b�g�ɐݒ肷��
	/// </summary>
	static void SetRenderTarget( const CRenderTargetPtr &spRenderTarget , const CDepthStencilBufferPtr &spBuffer/* = CDepthStencilBufferPtr() */ , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// �����_�����O�^�[�Q�b�g���v���C�}���o�b�t�@�ɓ]������i�\�ɏo���j
	/// </summary>
	static void SendDisplayRenderTarget( const CRenderTargetPtr &spRenderTarget , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// �����_�����O�^�[�Q�b�g�̃J���[�o�b�t�@���N���A
	/// </summary>
	static void ClearColorBuffer( const CRenderTargetPtr &spRenderTarget , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// �[�x�^�X�e���V���o�b�t�@�̃N���A
	/// </summary>
    static void ClearDepthStencilBuffer( const CDepthStencilBufferPtr &spBuffer , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// �[�x�o�b�t�@�̃N���A
	/// </summary>
    static void ClearDepthBuffer( const CDepthStencilBufferPtr &spBuffer , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// �X�e���V���o�b�t�@�̃N���A
	/// </summary>
    static void ClearStencilBuffer( const CDepthStencilBufferPtr &spBuffer , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// �r���[�|�[�g�̐ݒ�
	/// </summary>
	static void SetViewPort( const CViewPortPtr &spViewPort , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// �V�U�����O��`�̐ݒ�
	/// </summary>
	static void SetScissorRect( const CScissorRectPtr &spScissor , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// �V�U�����O��`�̃N���A
	/// </summary>
	static void ClearScissorRect( const CScissorRectPtr &spScissor , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// �V�[�������_�����O�J�n
	/// </summary>
	static void BeginRenderingScene( const skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// �V�[�������_�����O�I��
	/// </summary>
	static void EndRenderingScene( const skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// �����_�[�X�e�[�g�̐ݒ�
	/// </summary>
	static void SetRenderState( const CRenderStatePtr &state , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// �����_�[�X�e�[�g��߂�
	/// </summary>
	static void ReverseRenderState( const CRenderStatePtr &state , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// ���b�V���̕`��
	/// </summary>
	struct RenderingMeshParam
	{
		u32                    m_PassNo;
		CViewPortPtr           m_spViewPort;
		CameraPtr              m_spCamera;
		CShaderObjectPtr       m_spShader;
		CLightPtr              m_spLight;
		math::CMatrix4x3       m_w;
		CExtraDataPtr          m_ExtraData;
		CMeshPtr               m_spMesh;
		math::CMatrix4x3Array  m_SkinMatrix;

		//�R���X�g���N�^
		RenderingMeshParam( u32 passNo , const CViewPortPtr &spViewPort , const CameraPtr &spCamera , const CShaderObjectPtr &spShader , const CMeshPtr &spMesh , const math::CMatrix4x3 &w , const CExtraDataPtr &spExtra , const CLightPtr &spLight ) :
		m_PassNo( passNo ) ,
		m_spViewPort( spViewPort ) ,
		m_spCamera( spCamera ) ,
		m_spShader( spShader ) ,
		m_spMesh( spMesh ) ,
		m_w( w ) ,
		m_ExtraData( spExtra ) ,
		m_spLight( spLight )
		{}
	};
	static void RenderingMesh( const RenderingMeshParam &param , skyBool isCommand = IS_COMMAND );

	struct RenderingPostEffectMeshParam
	{
		u32                  m_PassNo;
		CTexturePtr			 m_spRenderTargetTexture;
		CSamplerData         m_SamplerData;
		CameraPtr            m_spCamera;
		CPostEffectObjectPtr m_spShader;
		CMeshContenaPtr      m_spMesh;
		CExtraDataPtr        m_ExtraData;

		//�R���X�g���N�^
		RenderingPostEffectMeshParam( u32 passNo , const CTexturePtr &spRenderTargetTexture , const CSamplerData& sampler , const CameraPtr &spCamera , const CPostEffectObjectPtr &spShader , const CMeshContenaPtr &spMesh ) :
		m_PassNo( passNo ) ,
		m_spRenderTargetTexture( spRenderTargetTexture ) ,
		m_SamplerData( sampler ) ,
		m_spCamera( spCamera ) ,
		m_spShader( spShader ) ,
		m_spMesh( spMesh )
		{}
	};
	static void RenderingPostEffectMesh( const RenderingPostEffectMeshParam &param , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// �G�t�F�N�g�̕`��
	/// </summary>
	struct RenderingEffectParam
	{
		u32                    m_PassNo;
		CViewPortPtr           m_spViewPort;
		CameraPtr              m_spCamera;
		CEffectObjectPtr       m_spEffect;
		CLightPtr              m_spLight;
		math::CMatrix4x3       m_w;
		CExtraDataPtr          m_ExtraData;
		CMeshPtr               m_spMesh;
		math::CMatrix4x3Array  m_SkinMatrix;

		//�R���X�g���N�^
		RenderingEffectParam( u32 passNo , const CViewPortPtr &spViewPort , const CameraPtr &spCamera , const CEffectObjectPtr &spEffect , const math::CMatrix4x3 &w , const CExtraDataPtr &spExtra , const CLightPtr &spLight ) :
		m_PassNo( passNo ) ,
		m_spViewPort( spViewPort ) ,
		m_spCamera( spCamera ) ,
		m_spEffect( spEffect ) ,
		m_w( w ) ,
		m_ExtraData( spExtra ) ,
		m_spLight( spLight )
		{}
	};
	static void RenderingEffect( const RenderingEffectParam &param , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// �e�N�X�`���̃��[�h
	/// </summary>
	static void LoadTexture( const CTexturePtr &spTex , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// ���b�V���̎g�pGPU�������쐬
	/// </summary>
	static void CreateMeshGpuMemory( const CMeshContenaPtr &spVertex , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// ���b�V���̎g�pGPU�������ւ̍폜
	/// </summary>
	static void DeleteMeshCpuMemory( const CMeshContenaPtr &spVertex , skyBool isCommand = IS_COMMAND );

	/// <summary>
	/// ���b�V���̎g�pGPU�������ւ̏�������
	/// </summary>
	static void WirteMeshGpuMemory( const CMeshContenaPtr &spVertex , skyBool isCommand = IS_COMMAND );

/*
	/// <summary>
	/// �ėp�R�}���h���s�i�R�}���h���������������֐��I�u�W�F�N�g�ɂ��ēn���Ă��������B�j
	/// </summary>
	static void Invoke( const CRenderingCommandPtr &command , skyBool isCommand = IS_COMMAND );
*/
};

} } }

#define CRenderCommand_	sky::lib::graphic::CRenderCommand



