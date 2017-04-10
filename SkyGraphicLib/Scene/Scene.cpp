#include "StdAfx.h"
#include "SkyGraphicLib/Scene.h"

SkyImplementRTTI( sky::lib::graphic::CScene , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

void CScene::SetUp()
{
	AttachBranch( CSceneGraphNode::Create( CHS( _T( "default_root" ) ) ) );
}

CRenderTargetPtr CScene::GetRenderTarget( skyBool isRequired )
{
	CRenderTargetPtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachLeaf< CRenderTarget , CRenderTargetPtr >( thisPtr( CSceneGraphNodePtr ) , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "RenderTarget is not found." ) );
	}
#endif
	return spResult;
}

void CScene::SetRenderTarget( const CRenderTargetPtr &spRenderTarget )
{
	SceneGraphNodeUtil::DetachLeafAll< CRenderTarget >( thisPtr( CSceneGraphNodePtr ) );
		 
	AttachLeaf( spRenderTarget );
}

CDepthStencilBufferPtr CScene::GetDepthStencilBuffer( skyBool isRequired )
{
	CDepthStencilBufferPtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachLeaf< CDepthStencilBuffer , CDepthStencilBufferPtr >( thisPtr( CSceneGraphNodePtr ) , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "DepthStencilBuffer is not found." ) );
	}
#endif
	return spResult;
}

void CScene::SetDepthStencilBuffer( const CDepthStencilBufferPtr &spDepthStencilBuffer )
{
	SceneGraphNodeUtil::DetachLeafAll< CDepthStencilBuffer >( thisPtr( CSceneGraphNodePtr ) );

	AttachLeaf( spDepthStencilBuffer );
}

CViewPortPtr CScene::GetViewPort( skyBool isRequired )
{
	CViewPortPtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachLeaf< CViewPort , CViewPortPtr >( thisPtr( CSceneGraphNodePtr ) , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "ViewPort is not found." ) );
	}
#endif
	return spResult;
}

void CScene::SetViewPort( const CViewPortPtr &spViewPort )
{
	SceneGraphNodeUtil::DetachLeafAll< CViewPort >( thisPtr( CSceneGraphNodePtr ) );

	AttachLeaf( spViewPort );
}

CScissorRectPtr CScene::GetScissorRect( skyBool isRequired )
{
	CScissorRectPtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachLeaf< CScissorRect , CScissorRectPtr >( thisPtr( CSceneGraphNodePtr ) , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "ScissorRect is not found." ) );
	}
#endif
	return spResult;
}

void CScene::SetScissorRect( const CScissorRectPtr &spScissorRect )
{
	SceneGraphNodeUtil::DetachLeafAll< CScissorRect >( thisPtr( CSceneGraphNodePtr ) );

	AttachLeaf( spScissorRect );
}

CameraManagerPtr CScene::GetCameraManager( skyBool isRequired )
{
	CameraManagerPtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachLeaf< CameraManager , CameraManagerPtr >( thisPtr( CSceneGraphNodePtr ) , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "CameraManager is not found." ) );
	}
#endif
	return spResult;
}

void CScene::SetCameraManager( const CameraManagerPtr &spCameraManager )
{
	AttachLeaf( spCameraManager );
}

void CScene::SetLight( const CLightPtr &spLight )
{
	SceneGraphNodeUtil::DetachLeafAll< CLight >( thisPtr( CSceneGraphNodePtr ) );

	AttachLeaf( spLight );
}

CLightPtr CScene::GetLight( skyBool isRequired )
{
	CLightPtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachLeaf< CLight , CLightPtr >( thisPtr( CSceneGraphNodePtr ) , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "CLight is not found." ) );
	}
#endif
	return spResult;
}

void CScene::SetUpPostEffect()
{
	m_spPostEffectManager = CPostEffectManagerCreate_();
	m_spPostEffectManager->CreateSprite( GetViewPort()->GetRect().W() , GetViewPort()->GetRect().H() );

	graphic::CRenderTargetPtr spSamplerRenderTarget = graphic::CRenderTarget::Create();
	spSamplerRenderTarget->SetUpTexture( math::CBasicSize( GetViewPort()->GetRect().W() , GetViewPort()->GetRect().H() ) );
	spSamplerRenderTarget->SetClearColor( graphic::CColor_White );
	m_spPostEffectManager->SetBufferRenderTarget( spSamplerRenderTarget );
}

const CPostEffectManagerPtr &CScene::GetPostEffectManager()
{
	return m_spPostEffectManager;
}

CRenderStatePtr CScene::GetRenderState( skyBool isRequired )
{
	CRenderStatePtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachLeaf< CRenderState , CRenderStatePtr >( thisPtr( CSceneGraphNodePtr ) , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "RenderState is not found." ) );
	}
#endif
	return spResult;
}

void CScene::SetRenderState( const CRenderStatePtr &spRenderState )
{
	SceneGraphNodeUtil::DetachLeafAll< CRenderState >( thisPtr( CSceneGraphNodePtr ) );

	AttachLeaf( spRenderState );
}

CSceneGraphNodePtr CScene::GetSceneGraphRoot( skyBool isRequired )
{
	CSceneGraphNodePtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachBranch< CSceneGraphNode , CSceneGraphNodePtr >( thisPtr( CSceneGraphNodePtr ) , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "SceneGraphRoot is not found." ) );
	}
#endif
	return spResult;
}

#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&m_IsOff , sizeof( s32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_IsDepthBufferClear , sizeof( s32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_IsStencilBufferClear , sizeof( s32 ) , isWrite ); \

skyBool CScene::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "scene bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN( skyTrue );

	return result;
}
			
skyBool CScene::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "scene bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN( skyFalse );

	return result;
}

const serialize::CSerializeInterfacePtr& CScene::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CScenePtr )spContainer )->m_IsOff = m_IsOff;
	( ( CScenePtr )spContainer )->m_IsDepthBufferClear = m_IsDepthBufferClear;
	( ( CScenePtr )spContainer )->m_IsStencilBufferClear = m_IsStencilBufferClear;
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CScene::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CScene::Clone()
{
	return Copy( CScenePtr( NEW__( CScene , CScene() ) ) );
}

serialize::CSerializeInterfacePtr CScene::CloneSharing()
{
	return Clone();
}

void CScene::Activate()
{
	ACTIVATE_COMMON();
}

void CScene::Deactivate()
{
	DEACTIVATE_COMMON();
}

} } }