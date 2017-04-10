#include "StdAfx.h"
#include "SkyBaseLib/SKYTL.h"

namespace sky { namespace lib {

FunctionCallBackAsync_v* FunctionCallBackAsync_v::Create( void ( * f )() )
{
	return NEW_() FunctionCallBackAsync_vg0( f );
}

template < class Result >
FunctionCallBackAsync_r< Result >* FunctionCallBackAsync_r< Result >::Create( Result ( * f )() )
{
//	return NEW FunctionCallBackAsync_rg0<Result>( f );
	return NEW_() FunctionCallBackAsync_rg0<Result>( f );
}

} }