#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_GEOMETORY

using namespace sky::lib;

#include "SkyMathLib/Geometry.h"
#include "SkyMathLib/Collision.h"

#if OFF_

//線と線の交差判定
TEST( Geometry , 1 ) 
{
	SKY_MATH_LIB_INIT();

//	math::CBasicLine2   line1( math::CBasicVector2( -1.0f , 0.0f ) , math::CBasicVector2( 1.0f , 0.0f ) ); //原点で交差
//	math::CBasicLine2   line2( math::CBasicVector2( 0.0f , -1.0f ) , math::CBasicVector2( 0.0f , 1.0f ) ); 
//	math::CBasicLine2   line1( math::CBasicVector2( -1.0f , 0.0f ) , math::CBasicVector2( 1.0f , 0.0f ) ); //一致
//	math::CBasicLine2   line2( math::CBasicVector2( -1.0f , 0.0f ) , math::CBasicVector2( 1.0f , 0.0f ) );
	math::CBasicLine2   line1( math::CBasicVector2( -1.0f , 1.0f ) , math::CBasicVector2( 1.0f , 1.0f ) ); //並行（交差しない）
	math::CBasicLine2   line2( math::CBasicVector2( -1.0f , 0.0f ) , math::CBasicVector2( 1.0f , 0.0f ) );
	math::CBasicVector2 point;

	s32 result = math::GeometryCollisionUtil::Line2CrossLine2< dectype >( line1 , line2 );

	SKY_MATH_LIB_TERM();
}

//円と円の交差判定
TEST( Geometry , 2 ) 
{
	SKY_MATH_LIB_INIT();

	math::CBasicCircle circle1( 0.0f , 0.0f , 1 );
	math::CBasicCircle circle2( 0.0f , 2.1f , 1 );

	skyBool result = math::GeometryCollisionUtil::CircleCrossCircle< dectype >( circle1 , circle2 );

	SKY_MATH_LIB_TERM();
}

//点と円の交差判定
TEST( Geometry , 3 ) 
{
	SKY_MATH_LIB_INIT();

	math::CBasicVector2 point( 0.0f , 0.0f  );
	math::CBasicCircle circle( 0.0f , 0.9f , 1 );

	skyBool result = math::GeometryCollisionUtil::Point2CrossCircle< dectype >( point , circle );

	SKY_MATH_LIB_TERM();
}

//点とレイの交差判定
TEST( Geometry , 4 ) 
{
	SKY_MATH_LIB_INIT();

	math::CBasicRay2 ray2( math::CBasicVector2( 0.0f , 0.0f ) , math::CBasicVector2( 1.0f , 1.0f ) );
	math::CBasicVector2 point( 2.5f , 2.0f );

	skyBool result = math::GeometryCollisionUtil::Point2CrossRay2< dectype >( point , ray2 );

	SKY_MATH_LIB_TERM();
}

//点と線分の交差判定
TEST( Geometry , 5 ) 
{
	SKY_MATH_LIB_INIT();

	math::CBasicLine2 line2( math::CBasicVector2( 0.0f , 0.0f ) , math::CBasicVector2( 1.0f , 1.0f ) );
	math::CBasicVector2 point( 0.1f , 0.0f );

	skyBool result = math::GeometryCollisionUtil::Point2CrossLine2< dectype >( point , line2 );

	SKY_MATH_LIB_TERM();
}

//矩形と矩形の衝突判定
TEST( Geometry , 6 ) 
{
	SKY_MATH_LIB_INIT();

	math::CBasicRect rect1( 1 , 1 , 1 , 1 );
	math::CBasicRect rect2( 1.9 , 1.9 , 0.9 , 0.9 );

	skyBool result = math::GeometryCollisionUtil::RectCrossRect< dectype >( rect1 , rect2 );

	SKY_MATH_LIB_TERM();
}

//矩形と矩形の衝突判定
TEST( Geometry , 7 ) 
{
	SKY_MATH_LIB_INIT();

	math::CBasicVector2 vec1( 0 , 0 );
	math::CBasicSize    size1( 1 , 1 );
	math::CBasicVector2 vec2( 1 , 1.1 );
	math::CBasicSize    size2( 1 , 1 );

	skyBool result = math::GeometryCollisionUtil::RectCrossRect< dectype >( vec1 , size1 , vec2 , size2 );

	SKY_MATH_LIB_TERM();
}

//矩形と矩形の衝突判定
TEST( Geometry , 8 ) 
{
	SKY_MATH_LIB_INIT();

	math::CBasicLine2 line2( math::CBasicVector2( -2.0f , 0.0f ) , math::CBasicVector2( -1.0f , 0.5f ) );
	math::CBasicCircle circle( 0.5f , 0.5f , 1.0 );

	skyBool result = math::GeometryCollisionUtil::Line2CrossCircle< dectype >( line2 , circle );

	SKY_MATH_LIB_TERM();
}

//楕円と点の衝突判定
TEST( Geometry , 9 ) 
{
	SKY_MATH_LIB_INIT();

	math::CBasicVector2 vec( 12 , 12 );
	math::CBasicEllipse ellipse( 11 , 11 , 3 , 6 , 0 );

	skyBool result = math::GeometryCollisionUtil::Point2CrossEllipse< dectype >( vec , ellipse );

	SKY_MATH_LIB_TERM();
}

//楕円と点の衝突判定
TEST( Geometry , 10 ) 
{
	SKY_MATH_LIB_INIT();

	math::CBasicEllipse ellipse1( 11 , 11 , 3 , 6 , 0 );
	math::CBasicEllipse ellipse2( 11 , 11 , 3 , 6 , 0 );

	skyBool result = math::GeometryCollisionUtil::EllipseCrossEllipse< dectype >( ellipse1 , ellipse2 );

	SKY_MATH_LIB_TERM();
}

//点と点の衝突判定
TEST( Geometry , 11 ) 
{
	SKY_MATH_LIB_INIT();

	math::CBasicVector2 pos2_1( 12 , 11 );
	math::CBasicVector2 pos2_2( 11 , 11 );

	skyBool result = math::GeometryCollisionUtil::Pos2CrossPos2< dectype >( pos2_1 , pos2_2 );

	math::CBasicVector3 pos3_1( 12 , 11 , 0 );
	math::CBasicVector3 pos3_2( 11 , 11 , 0 );

	result = math::GeometryCollisionUtil::Pos3CrossPos3< dectype >( pos3_1 , pos3_2 );


	SKY_MATH_LIB_TERM();
}

//3D点とレイの衝突判定
TEST( Geometry , 12 ) 
{
	SKY_MATH_LIB_INIT();

	math::CBasicVector3 vec3( 2.0f , 2.0f , 2.0f );
	math::CBasicRay3    ray3( math::CBasicVector3( 0 , 0 , 0 ) , math::CBasicVector3( 1.0f , 1.0f , 1.0f ) );

	skyBool result = math::GeometryCollisionUtil::Pos3CrossRay3< dectype >( vec3 , ray3 );

	SKY_MATH_LIB_TERM();
}

//3D点と3D線の衝突判定
TEST( Geometry , 13 ) 
{
	SKY_MATH_LIB_INIT();

	math::CBasicVector3 vec3( 0.5f , 0.5f , 0.5f );
	math::CBasicLine3   line3( math::CBasicVector3( -1.0f , -1.0f , -1.0f ) , math::CBasicVector3( 1.0f , 1.0f , 1.0f ) );

	skyBool result = math::GeometryCollisionUtil::Pos3CrossLine3< dectype >( vec3 , line3 );

	SKY_MATH_LIB_TERM();
}

//3D点と3D線の距離ベクトル
TEST( Geometry , 14 ) 
{
	SKY_MATH_LIB_INIT();

	math::CBasicVector3 vec3( 1.0f , 1.0f , 0.0f );
	math::CBasicLine3   line3( math::CBasicVector3( -1.0f , 1.0f , 0.0f ) , math::CBasicVector3( 1.0f , -1.0f , 0.0f ) );

	math::CBasicVector3 result = math::GeometryCollisionUtil::Pos3ToLine3< dectype >( vec3 , line3 );

	SKY_MATH_LIB_TERM();
}

//3D線と3D線の衝突
TEST( Geometry , 15 ) 
{
	SKY_MATH_LIB_INIT();

	math::CBasicLine3   line1( math::CBasicVector3( -1.0f , 0.0f , 0.0f ) , math::CBasicVector3( 1.0f , 0.0f , 0.0f ) );
	math::CBasicLine3   line2( math::CBasicVector3( 0.0f , 1.0f , 0.0f ) , math::CBasicVector3( 0.0f , -1.0f , 0.1f ) );

	skyBool result = math::GeometryCollisionUtil::Line3CrossLine3< dectype >( line1 , line2 );

	SKY_MATH_LIB_TERM();
}

//3Dレイと平面の衝突
TEST( Geometry , 16 ) 
{
	SKY_MATH_LIB_INIT();

	math::CBasicRay3   ray( math::CBasicVector3( 1.0f , 1.0f , 1.0f ) , math::CBasicVector3( 0.0f , 0.0f , 1.0f ) );
	math::CBasicPlane  plane( math::CBasicVector3( 0.0f , 5.0f , 0.0f ) , math::CBasicVector3( 0.0f , -5.0f , 0.0f ) , math::CBasicVector3( 5.0f , 0.0f , 0.0f ) );
	math::CBasicVector3 cross;

	skyBool result = math::GeometryCollisionUtil::Ray3CrossPlane< dectype >( ray , plane );

	SKY_MATH_LIB_TERM();
}

//点と面の距離
TEST( Geometry , 17 ) 
{
	SKY_MATH_LIB_INIT();

	math::CBasicVector3 pos( 1.0f , 1.0f , -2.0f );
	math::CBasicPlane  plane( math::CBasicVector3( 0.0f , 5.0f , 0.0f ) , math::CBasicVector3( 0.0f , -5.0f , 0.0f ) , math::CBasicVector3( 5.0f , 0.0f , 0.0f ) );

	dectype result = math::GeometryDistanceUtil::Pos3ToPlane< dectype >( pos , plane );

	SKY_MATH_LIB_TERM();
}

//2点を通る直線の角度
TEST( Geometry , 18 ) 
{
	SKY_MATH_LIB_INIT();

	math::CBasicVector2 pos1( 0.0f , 0.0f );
	math::CBasicVector2 pos2( 1.0f , 1.0f );

	dectype result = math::GeometryCollisionUtil::Pos2AndPos2Angle< dectype >( pos1 , pos2 );

	dectype degresult = math::CMathUtil::RadToDeg( result );

	SKY_MATH_LIB_TERM();
}

//点から線分に降りた垂線ベクトル
TEST( Geometry , 19 ) 
{
	SKY_MATH_LIB_INIT();

	math::CBasicVector2 pos( 2.0f , 1.0f );
	math::CBasicLine2 line( math::CBasicVector2( -1.0f , 0.0f ) , math::CBasicVector2( 2.0f , 0.0f ) );
	math::CBasicVector2 vec;

	skyBool result = math::GeometryCollisionUtil::Pos2ToLineVector< dectype >( pos , line , vec );

	SKY_MATH_LIB_TERM();
}

//点から線分に降りた垂線ベクトル
TEST( Geometry , 20 ) 
{
	SKY_MATH_LIB_INIT();

	math::CBasicVector2 pos( 0.0f , 0.0f );
	math::CBasicLine2 line( math::CBasicVector2( -1.0f , 0.0f ) , math::CBasicVector2( 2.0f , 0.0f ) );

	dectype result = math::GeometryCollisionUtil::Pos2ToLineSide< dectype >( pos , line );

	SKY_MATH_LIB_TERM();
}

//三角形の内外判定
TEST( Geometry , 21 ) 
{
	SKY_MATH_LIB_INIT();

	math::CBasicPlane   plane( math::CBasicVector3( 22.0f , 0.0f , 0.0f ) , math::CBasicVector3( 0.0f , -5.0f , 0.0f ) , math::CBasicVector3( 5.0f , 0.0f , 0.0f ) );
	math::CBasicTriangle3 triangle( math::CBasicVector3( 0.0f , 5.0f , 0.0f ) , math::CBasicVector3( 0.0f , -5.0f , 0.0f ) , math::CBasicVector3( 5.0f , 0.0f , 0.0f ) );

	skyBool result = math::GeometryCollisionUtil::PlanePos3ToTriangleInSide< dectype >( plane , triangle );

	SKY_MATH_LIB_TERM();
}

//球とレイの衝突判定
TEST( Geometry , 22 ) 
{
	SKY_MATH_LIB_INIT();

	math::CBasicSphere shpere( 0 , 2 , 0 , 3 );
	math::CBasicRay3   ray( math::CBasicVector3( 0 , 0 , -10 ) , math::CBasicVector3( 0 , 0 , 1 ) );

	skyBool result = math::GeometryCollisionUtil::SphereCrossRay3< dectype >( shpere , ray );

	SKY_MATH_LIB_TERM();
}

#endif

//球とレイの衝突判定
TEST( Geometry , 23 ) 
{
	SKY_MATH_LIB_INIT();

	math::CBasicVector2 vec1( 0 , 1 );
	math::CBasicVector2 vec2( 1 , 1 );

	vec1.Normalize();
	vec2.Normalize();

	float dot = vec1.Dot( vec2 );

	SKY_MATH_LIB_TERM();
}

#endif