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

	//ダブルバッファ
	CRenderTargetPtr spRenderTargets[ 2 ];

	spRenderTargets[ 0 ] = spRenderTarget;
	spRenderTargets[ 1 ] = m_spBufferRenderTarget;

	CRenderTargetPtr spLastRenderTarget;
	
	//一つでもポストエフェクトが存在すればポストエフェクトを実行する
	if ( m_PostEffectStack.GetNum() > 0 )
	{
		//ポストエフェクトを順番にかけていく
		FOREACH( CPostEffectObjectPtrList , it , m_PostEffectStack )
		{
			//パスの数を取得
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

				//交換
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
	//スプライトを作る(シェーダーは使用しないので特例で空のシェーダーCShaderObjectPtr()を設定する）
	m_BufferSizeSprite = CSpriteUtil::CreateSquare( math::CBasicVector2( width , height ) , CColor_White , RENDERING_2D_PRIO_FAST , CVertexBuffer::Create() , CIndexBuffer::Create() , CNullShaderCreate_() );

	//レンダーバッファコピー用ポストエフェクトを作成する
	m_spBufferCopyPostEffect = CPostEffectTextureMappingCreate_();

	//カメラ行列を作る
	m_spCamera = CameraCreate_( graphic::eCameraKind_Satellite );
	m_spCamera->SetOrtho( skyTrue );
	m_spCamera->SetDisplaySize( width , height );
}

CPostEffectManager::CPostEffectManager()
{
}

} } }
