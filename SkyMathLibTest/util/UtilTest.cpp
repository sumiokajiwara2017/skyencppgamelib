#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_UTIL

#define PLOT_NUM 10

static FLOAT Dot2D( const sky::lib::math::CBasicVector3 &v1 , const sky::lib::math::CBasicVector3 &v2 )
{
	return v1.X * v2.X + v1.Y * v2.Y;
}

static sky::lib::math::CBasicVector3 Normalize2D( const sky::lib::math::CBasicVector3 &v )
{
	sky::lib::math::CBasicVector3 result = v;

	FLOAT magSq = v.X*v.X + v.Y*v.Y;
	if ( magSq > 0.0f )
	{
		FLOAT oneOverMag = 1.0f / sqrt( magSq );
		result.X *= oneOverMag;
		result.Y *= oneOverMag;
	}

	return result;
}

static FLOAT IncludedAngle2D__( const sky::lib::math::CBasicVector3 &v1_ , const sky::lib::math::CBasicVector3 &v2_ )
{
    //³‹K‰»
    sky::lib::math::CBasicVector3 v1 = Normalize2D( v1_ );
    sky::lib::math::CBasicVector3 v2 = Normalize2D( v2_ );

    //³‹K‰»‚³‚ê‚½ŽÒ“¯Žm‚Ì“àÏ‚ð‹‚ß‚é
    FLOAT dot = Dot2D( v1 , v2 );

	FLOAT result = acos( dot );

	if ( v1_.Y > v2_.Y )
	{
		result = 3.1415926535f * 2 - result;
	}

    //‚È‚·Šp‚ð•Ô‚·
	return result;
}

TEST( Util , 1 ) 
{
	SKY_MATH_LIB_INIT();

	sky::lib::math::CBasicVector3 vec1( 1 , 0 , 0 );
	sky::lib::math::CBasicVector3 vec2( 1 , -1 , 0 );

	SKY_PRINTF( "%f \n" , sky::lib::math::CMathUtil::RadToDeg( IncludedAngle2D__( vec1 , vec2 ) ) );

	SKY_MATH_LIB_TERM();
}

#endif