#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"

#include "SkyGraphicLib/Renderer.h"
#include "SkyGraphicLib/Sprite.h"
#include "SkyGraphicLib/Screen.h"

SkyImplementRTTI( sky::lib::graphic::CPostEffectManager , sky::lib::base::SkyRefObject );

namespace sky { namespace lib { namespace graphic {

void CPostEffectManager::SetSampler( const CSamplerData &sampler )
{
	m_SamplerData = sampler;
}

void CPostEffectManager::PushPostEffect( const CPostEffectObjectPtr &spPostEffect )
{
	m_PostEffectStack.AddFront( spPostEffect );
}

void CPostEffectManager::PopPostEffect()
{
	m_PostEffectStack.DeleteIdx( 0 );
}

void CPostEffectManager::SetBufferRenderTarget( const CRenderTargetPtr &spBufferRenderTarget )
{
	SKY_ASSERT_MSG( spBufferRenderTarget->IsFrameBufferRenderTarget() == skyFalse , "PostEffect Use Scene RenderTarget is TextureRenderTarget Only." );

	m_spBufferRenderTarget = spBufferRenderTarget;
}

void CPostEffectManager::Rendering( const CRenderTargetPtr &spRenderTarget , const CDepthStencilBufferPtr &spDepthStencilBuffer )
{
	SKY_ASSERT_MSG( spRenderTarget->IsFrameBufferRenderTarget() == skyFalse , "PostEffect Use Scene RenderTarget is TextureRenderTarget Only." );
	SKY_ASSERT_MSG( m_spBufferRenderTarget.IsEmpty() == skyFalse , "No PostEffect Buffer." );
	SKY_ASSERT_MSG( m_spBufferRenderTarget->IsFrameBufferRenderTarget() == skyFalse , "PostEffect Use Scene RenderTarget is TextureRenderTarget Only." );

	//�_�u���o�b�t�@
	CRenderTargetPtr spRenderTargets[ 2 ];

	spRenderTargets[ 0 ] = spRenderTarget;
	spRenderTargets[ 1 ] = m_spBufferRenderTarget;

	CRenderTargetPtr spLastRenderTarget;
	
	//��ł��|�X�g�G�t�F�N�g�����݂���΃|�X�g�G�t�F�N�g�����s����
	if ( m_PostEffectStack.GetNum() > 0 )
	{
		//�|�X�g�G�t�F�N�g�����Ԃɂ����Ă���
		FOREACH( CPostEffectObjectPtrList , it , m_PostEffectStack )
		{
			//�p�X�̐����擾
			u32 passNum = ( *it )->GetPassNum();

			for ( u32 i = 0 ; i < passNum ; i++ )
			{
				CRenderCommand::SetRenderTarget( spRenderTargets[ 1 ] , spDepthStencilBuffer );

				CRenderCommand::RenderingPostEffectMesh( 
					CRenderCommand::RenderingPostEffectMeshParam( i , 
					spRenderTargets[ 0 ]->GetColorBufferTexture() , 
					m_SamplerData , m_spCamera , ( *it ) , ( ( CSpritePtr )m_BufferSizeSprite )->GetMeshContena() ) );

				CRenderCommand::SendDisplayRenderTarget( spRenderTargets[ 1 ] );

				spLastRenderTarget = spRenderTargets[ 1 ];

				//����
				TemplateUtil::Swap( spRenderTargets[ 0 ] , spRenderTargets[ 1 ] );
			}
		}

		if ( spLastRenderTarget.IsEmpty() == skyFalse && spLastRenderTarget != spRenderTarget )
		{
			CRenderCommand::SetRenderTarget( spRenderTarget , spDepthStencilBuffer );

			CRenderCommand::RenderingPostEffectMesh( 
				CRenderCommand::RenderingPostEffectMeshParam( 0 , 
				m_spBufferRenderTarget->GetColorBufferTexture() , 
				m_SamplerData , m_spCamera , m_spBufferCopyPostEffect , ( ( CSpritePtr )m_BufferSizeSprite )->GetMeshContena() ) );

			CRenderCommand::SendDisplayRenderTarget( spRenderTarget );
		}
	}
}

void CPostEffectManager::SetEnablePostEffect( u32 stackNo , skyBool isEnable )
{
	m_PostEffectStack.GetIdx( stackNo )->SetEnable( isEnable );
}

void CPostEffectManager::CreateSprite(  dectype width ,  dectype height )
{
	//�X�v���C�g�����(�V�F�[�_�[�͎g�p���Ȃ��̂œ���ŋ�̃V�F�[�_�[CShaderObjectPtr()��ݒ肷��j
	m_BufferSizeSprite = CSpriteUtil::CreateSquare( math::CBasicVector2( width , height ) , CColor_White , RENDERING_2D_PRIO_FAST , CVertexBuffer::Create() , CIndexBuffer::Create() , CNullShaderCreate_() );

	//�����_�[�o�b�t�@�R�s�[�p�|�X�g�G�t�F�N�g���쐬����
	m_spBufferCopyPostEffect = CPostEffectTextureMappingCreate_();

	//�J�����s������
	m_spCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	m_spCamera->SetOrtho( skyTrue );
	m_spCamera->SetDisplaySize( width , height );
}

CPostEffectManager::CPostEffectManager()
{
}

} } }
