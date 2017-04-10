#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_MOVE

using namespace sky::lib;

#include "SkyMathLib/Move.h"

TEST( Move , 1 ) 
{
	SKY_MATH_LIB_INIT();

	//点を行列で移動する
	math::CBasicVector3 point1( 0.0f , 0.0f , 0.0f );
	math::CMatrix4x3    mt , mr;
	mt.SetupTranslation( math::CBasicVector3( 5.0f , 0.0f , 0.0f ) );
	mr.SetupScale( math::CBasicVector3( 2.0f , 2.0f , 2.0f ) );
	point1 = math::CMoveUtil::VectorXMatrix< math::CBasicVector3 >( point1 , mt );
	point1 = math::CMoveUtil::VectorXMatrix< math::CBasicVector3 >( point1 , mr );
	SKY_PRINTF( _T( "%5.3f %5.3f %5.3f \n" ) , point1.X , point1.Y , point1.Z );

	//指定の点を指定の軸で回転。
	//カメラを任意の方向に向かせるために使う。結局のところ軸は最初から分かっていたほうがいい。
	//３Ｄにおける位置は毎フレーム算出したほうがいいのだ。そうでないと位置が算出できなくなってしまう。
	math::CBasicVector3 point2( 1.0f , 0.0f , 0.0f );
	math::CBasicVector3 n( 0.0f , 1.0f , 0.0f );
	point2 = math::CMoveUtil::RotPoint< math::CBasicVector3 >( point2 , n , math::CMathUtil::DegToRad( 90.0f ) );
	SKY_PRINTF( _T( "%5.3f %5.3f %5.3f \n" ) , point2.X , point2.Y , point2.Z );

	SKY_MATH_LIB_TERM();
}

#endif