#include "StdAfx.h"
#include "SkyGraphicLib/Effector.h"

SkyImplementRTTI( sky::lib::graphic::CEffector , sky::lib::math::CJoint );

namespace sky { namespace lib { namespace graphic {

CEffector::CEffector( const CEffectorPtr &spParent ) : CJoint( spParent )
{
}

} } }