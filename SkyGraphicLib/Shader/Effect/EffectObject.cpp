#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"

#include "SkyGraphicLib/Renderer.h"

SkyImplementRTTI( sky::lib::graphic::CEffectObject       , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

void CEffectObject::vRendering( CRenderingParam &param )
{
	CExtraDataPtr spExtraData;
	if ( param.ExtraDataStack.IsEmpty() == skyFalse )
	{
		spExtraData = param.ExtraDataStack.GetTop();
	}

	for ( u32 i = 0 ; i < GetPassNum() ; i++ )
	{
		if ( IsAlpha() )
		{
			param.AlphaDataList.AddTail( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::RenderingEffect , 
				CRenderCommand::RenderingEffectParam( i , param.ViewPortStack.GetTop() , param.CameraStack.GetTop() , thisPtr( CEffectObjectPtr ) , param.MatrixStack.GetTop() , spExtraData , param.LightStack.GetTop() ) , IS_COMMAND ) ) );
		}
		else
		{
			CRenderCommand::RenderingEffect( 
				CRenderCommand::RenderingEffectParam( i , param.ViewPortStack.GetTop() , param.CameraStack.GetTop() , thisPtr( CEffectObjectPtr ) , param.MatrixStack.GetTop() , spExtraData , param.LightStack.GetTop() ) );
		}
	}
}

} } }
