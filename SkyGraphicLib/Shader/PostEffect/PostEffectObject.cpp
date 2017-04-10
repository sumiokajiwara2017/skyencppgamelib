#include "StdAfx.h"
#include "SkyGraphicLib/Shader.h"

namespace sky { namespace lib { namespace graphic {

SkyImplementRTTI( sky::lib::graphic::CPostEffectObject , sky::lib::graphic::CShaderObject );

CPostEffectObject::CPostEffectObject() : 
m_IsEnable( skyTrue ) , 
m_PassNum( 1 )
{
}

u32 CPostEffectObject::GetPassNum()
{
	return m_PassNum;
}

void CPostEffectObject::SetEnable( skyBool isEnable )
{
	m_IsEnable = isEnable;
}

} } }
