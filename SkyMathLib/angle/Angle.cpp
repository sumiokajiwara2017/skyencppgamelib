#include "StdAfx.h"
#include "SkyMathLib/Angle.h"

namespace sky { namespace lib { namespace math {

#define SIN_TBL_SIZE ( sizeof( vf_sin_tbl ) / sizeof( vf_sin_tbl[ 0 ] ) )

template< typename T >
typename T CAngle< T >::Sin( const CAngle ang )
{
	u16 r = ang.m_Angle;
	return vf_sin_tbl[ r / ( 65536 / SIN_TBL_SIZE ) ];
}
template< typename T >
typename T CAngle< T >::Cos( const CAngle ang )
{
	u16 r = ang.m_Angle + ( 65536 / 4 );
	return vf_sin_tbl[ r / ( 65536 / SIN_TBL_SIZE ) ];
}

} } }