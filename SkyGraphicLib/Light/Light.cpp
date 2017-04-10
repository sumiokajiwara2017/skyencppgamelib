#include "StdAfx.h"
#include "SkyGraphicLib/Light.h"

SkyImplementRTTI( sky::lib::graphic::CLight , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

void CLight::vRendering( CRenderingParam &param )
{
    //自分をスタックに積む
    param.LightStack.Push( thisPtr( CLightPtr ) ); //もともとこのthisが他のスマートポインタに管理されていた場合（RefCountが0以上の場合)この受け渡しをしても問題が発生しない。
}

void CLight::vPostRendering( CRenderingParam &param )
{
    //自分をスタックから消す
    param.LightStack.Pop();
}

} } }