#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"

#include "SkyGraphicLib/Loader.h"
#include "SkyGraphicLib/DataBase.h"

namespace sky { namespace lib { namespace graphic {

struct CDecalManagerMember
{
	/// <summary>
	/// デカールリスト
	/// </summary>
	CDecalPtrList m_DecalList;

	/// <summary>
	/// デカール貼りつけ対象モデルリスト
	/// </summary>
	CModelPtrList m_spModelList;
};

struct CDecalData
{
	CTexturePtr spTexture;
};

CDecalManager::~CDecalManager()
{
	DEL m_pMember;
}

CDecalManager::CDecalManager()
{
	m_pMember = NEW CDecalManagerMember();
}

void CDecalManager::AttachModel( const CModelPtr &spModel )
{
	m_pMember->m_spModelList.AddTail( spModel );
}

void CDecalManager::DetachModel( const CModelPtr &spModel )
{
	m_pMember->m_spModelList.Delete( spModel );
}

void CDecalManager::CreateDecal( const CTexturePtr &spTexture , const math::CBasicVector3 &pos , const math::CBasicVector3 &norm , math::CBasicVector3 &tang , f32 distance , f32 width , f32 height , f32 depth , updateTime destoryTime )
{//未完成

	//モデル全部と衝突判定を行う
	FOREACH( CModelPtrList , it , m_pMember->m_spModelList )
	{
		//衝突していればデカールを発生させる

		CDecalPtr spDecal = CDecal::Create();
		spDecal->AttachTexture( spTexture );
		spDecal->SetDestoryTime( destoryTime );
		spDecal->Create( pos , norm , tang , width , height , depth , ( *it )->GetMeshContena()->GetMesh() );

		m_pMember->m_DecalList.AddTail( spDecal );
	}
}

void CDecalManager::DestoryDecal( const CDecalPtr &spDecal )
{
	m_pMember->m_DecalList.Delete( spDecal );
}

void CDecalManager::DestoryAllDecal()
{
	m_pMember->m_DecalList.DeleteAll();
}

void CDecalManager::DrawDecal( CRenderingParam &param )
{
	CExtraDataPtr spExtraData;
	if ( param.ExtraDataStack.IsEmpty() == skyFalse )
	{
		spExtraData = param.ExtraDataStack.GetTop();
	}

	FOREACH( CDecalPtrList , it , m_pMember->m_DecalList )
	{
		for ( u32 i = 0 ; i < ( *it )->GetPassNum() ; i++ )
		{
			if ( ( *it )->IsAlpha() )
			{
				param.AlphaDataList.AddTail( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::RenderingEffect , 
					CRenderCommand::RenderingEffectParam( i , param.ViewPortStack.GetTop() , param.CameraStack.GetTop() , *it , param.MatrixStack.GetTop() , spExtraData , param.LightStack.GetTop() ) , IS_COMMAND ) ) );
			}
			else
			{
				CRenderCommand::RenderingEffect( 
					CRenderCommand::RenderingEffectParam( i , param.ViewPortStack.GetTop() , param.CameraStack.GetTop() , *it , param.MatrixStack.GetTop() , spExtraData , param.LightStack.GetTop() ) );
			}
		}
	}
}

} } }