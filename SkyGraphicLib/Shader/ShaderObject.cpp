#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"

SkyImplementRTTI( sky::lib::graphic::CShaderObject       , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

void CShaderObject::Initialize()
{

#if  defined( SKYLIB_RENDER_OPENGL4 ) && defined( SW_SKYGRAPHICLIB_USE_CG_ON )
	CCg::Initialize();
#endif

}

void CShaderObject::Dispose()
{
#if  defined( SKYLIB_RENDER_OPENGL4 ) && defined( SW_SKYGRAPHICLIB_USE_CG_ON )
	CCg::Dispose();
#endif

}

void CShaderObject::vRendering( CRenderingParam &param )
{
    SKY_ASSERT_MSG( this->GetRefCnt() > 0 , _T( "It failed in making the device context. \n" ) );

	//自分をスタックに積む
	param.ShaderStack.Push( thisPtr( CShaderObjectPtr ) ); //もともとこのthisが他のスマートポインタに管理されていた場合（RefCountが0以上の場合)この受け渡しをしても問題が発生しない。
}

void CShaderObject::vPostRendering( CRenderingParam &param )
{
	//スタックから取る
	param.ShaderStack.Pop();
}

CShaderObject::CShaderObject() : m_PassNum( 0 )
{
	
}

} } }
