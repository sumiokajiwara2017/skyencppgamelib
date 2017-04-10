#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_MATRIX

using namespace sky::lib;

#include "SkyMathLib/Matrix.h"

//行列から３軸の向きを取り出す（角度は取り出せません。回転情報の入力値はちゃんと取っておきましょう！）
TEST( Matrix , rotation ) 
{
	SKY_MATH_LIB_INIT();

	math::CMatrix4x3 mat , matSc , matRotx , matRoty , matRotz , matTrans;
	matSc.Identity();
	matRotx.Identity();
	matRoty.Identity();
	matRotz.Identity();
	matTrans.Identity();

	//拡大縮小して移動もする
	matSc.SetupScale( math::CVector3_d64( 3.0f , 5.0f , 6.5f ) );
//	matRotx.SetupRotate( math::eAxis_x , math::CMathUtil::DegToRad( 60 ) );
//	matRoty.SetupRotate( math::eAxis_y , math::CMathUtil::DegToRad( 60 ) );
//	matRotz.SetupRotate( math::eAxis_z , math::CMathUtil::DegToRad( 60 ) );
	matTrans.SetupTranslation( math::CVector3_d64( 2.0f , 4.0f , 6.0f ) );

	//合成マトリックスを作る
	mat = matSc * matRotx * matRoty * matRotz * matTrans;

	math::CVector3_d64 direction;
	mat.GetDirection( math::eAxis_x , direction );
	SKY_PRINTF( _T( "Direction X( %f , %f , %f )  \n" ) , direction.X , direction.Y , direction.Z );
	mat.GetDirection( math::eAxis_y , direction );
	SKY_PRINTF( _T( "Direction Y( %f , %f , %f )  \n" ) , direction.X , direction.Y , direction.Z );
	mat.GetDirection( math::eAxis_z , direction );
	SKY_PRINTF( _T( "Direction Z( %f , %f , %f )  \n" ) , direction.X , direction.Y , direction.Z );

	SKY_MATH_LIB_TERM();
}


//行列から３軸の拡大縮小情報を取り出す
TEST( Matrix , scale ) 
{
	SKY_MATH_LIB_INIT();

	math::CMatrix4x3 mat , matSc , matRotx , matRoty , matRotz , matTrans;
	matSc.Identity();
	matRotx.Identity();
	matRoty.Identity();
	matRotz.Identity();
	matTrans.Identity();

	//拡大縮小して回転して移動もする
	matSc.SetupScale( math::CVector3_d64( 3.0f , 5.0f , 6.5f ) );
	matRotx.SetupRotate( math::eAxis_x , math::CMathUtil::DegToRad( 60 ) );
	matRoty.SetupRotate( math::eAxis_y , math::CMathUtil::DegToRad( 60 ) );
	matRotz.SetupRotate( math::eAxis_z , math::CMathUtil::DegToRad( 60 ) );
	matTrans.SetupTranslation( math::CVector3_d64( 2.0f , 4.0f , 9.0f ) );

	//合成マトリックスを作る
	mat = matSc * matRotx * matRoty * matRotz * matTrans;

	SKY_PRINTF( _T( "Scale X( %f ) \n" ) , mat.GetScale( math::eAxis_x ) );
	SKY_PRINTF( _T( "Scale Y( %f ) \n" ) , mat.GetScale( math::eAxis_y ) );
	SKY_PRINTF( _T( "Scale Z( %f ) \n" ) , mat.GetScale( math::eAxis_z ) );


	SKY_MATH_LIB_TERM();
}

//行列から３軸の移動情報を取り出す
TEST( Matrix , translate ) 
{
	SKY_MATH_LIB_INIT();

	math::CMatrix4x3 mat , matSc , matRotx , matRoty , matRotz , matTrans;
	matSc.Identity();
	matRotx.Identity();
	matRoty.Identity();
	matRotz.Identity();
	matTrans.Identity();

	//拡大縮小して回転して移動もする
	matSc.SetupScale( math::CVector3_d64( 3.0f , 5.0f , 6.5f ) );
	matRotx.SetupRotate( math::eAxis_x , math::CMathUtil::DegToRad( 60 ) );
	matRoty.SetupRotate( math::eAxis_y , math::CMathUtil::DegToRad( 60 ) );
	matRotz.SetupRotate( math::eAxis_z , math::CMathUtil::DegToRad( 60 ) );
	matTrans.SetupTranslation( math::CVector3_d64( 2.0f , 4.0f , 6.0f ) );

	//合成マトリックスを作る
	mat = matSc * matRotx * matRoty * matRotz * matTrans;

	SKY_PRINTF( _T( "Translation X( %f ) \n" ) , mat.GetTranslation( math::eAxis_x ) );
	SKY_PRINTF( _T( "Translation Y( %f ) \n" ) , mat.GetTranslation( math::eAxis_y ) );
	SKY_PRINTF( _T( "Translation Z( %f ) \n" ) , mat.GetTranslation( math::eAxis_z ) );

	SKY_MATH_LIB_TERM();
}

#endif