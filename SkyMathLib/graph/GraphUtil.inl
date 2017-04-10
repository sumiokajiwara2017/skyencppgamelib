namespace sky { namespace lib { namespace math {

namespace CGraphUtil
{

inline CBasicVector3 &GetStraightLinePoint( CBasicVector3 &dest , dectype beginX , dectype endX , dectype now , dectype a , dectype b , dectype z )
{
	SKY_ASSERT_MSG( now >= 0.0f , _T( "Please set now to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG(  now <= 1.0f , _T( "Please set now to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( beginX < endX , _T( "BeginX must be larger than endX." ) );

	//xを求める
	dectype x = beginX + ( ( endX - beginX ) * now );

	//方程式に設定
	dectype y = a * x + b;

	return dest.Set( x , y , z );
}

inline CBasicVector3Vector &DotsStraightLine( CBasicVector3Vector &dest , dectype beginX , dectype endX , dectype accuracy , dectype a , dectype b , dectype z )
{
	SKY_ASSERT_MSG( accuracy >= 0.0f , _T( "Please set accuracy to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( accuracy <= 1.0f , _T( "Please set accuracy to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( beginX < endX , _T( "BeginX must be larger than endX." ) );

	skyBool isPlot = skyTrue;
	dectype now = 0.0f;
	while( isPlot )
	{
		CBasicVector3 plot;
		GetStraightLinePoint( plot , beginX , endX , now , a , b , z );

		dest.Add( plot );

		now += accuracy;

		if ( now > 1.0f )
		{
			if ( now - 1.0f < 0.0001 ) //誤差範囲の場合は1.0fとみなす
			{
				now = 1.0f;
			}
			else
			{
				isPlot = skyFalse;
			}
		}
	}

	return dest;
}

inline CBasicVector3 &GetQuadraticCurvePoint( CBasicVector3 &dest , dectype beginX , dectype endX , dectype now , dectype a , dectype b , dectype z )
{
	SKY_ASSERT_MSG( now >= 0.0f , _T( "Please set now to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG(  now <= 1.0f , _T( "Please set now to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( beginX < endX , _T( "BeginX must be larger than endX." ) );

	//xを求める
	dectype x = beginX + ( ( endX - beginX ) * now );

	//方程式に設定
	dectype y = a * ( x * x ) + b;

	return dest.Set( x , y , z );
}

inline CBasicVector3Vector &DotsQuadraticCurve( CBasicVector3Vector &dest , dectype beginX , dectype endX , dectype accuracy , dectype a , dectype b , dectype z )
{
	SKY_ASSERT_MSG( accuracy >= 0.0f , _T( "Please set accuracy to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( accuracy <= 1.0f , _T( "Please set accuracy to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( beginX < endX , _T( "BeginX must be larger than endX." ) );

	skyBool isPlot = skyTrue;
	dectype now = 0.0f;
	while( isPlot )
	{
		CBasicVector3 plot;
		GetQuadraticCurvePoint( plot , beginX , endX , now , a , b , z );

		dest.Add( plot );

		now += accuracy;

		if ( now > 1.0f )
		{
			if ( now - 1.0f < 0.0001 ) //誤差範囲の場合は1.0fとみなす
			{
				now = 1.0f;
			}
			else
			{
				isPlot = skyFalse;
			}
		}
	}

	return dest;
}


inline CBasicVector3 &GetCubicCurvePoint( CBasicVector3 &dest , dectype beginX , dectype endX , dectype now , dectype a , dectype b , dectype z )
{
	SKY_ASSERT_MSG( now >= 0.0f , _T( "Please set now to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG(  now <= 1.0f , _T( "Please set now to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( beginX < endX , _T( "BeginX must be larger than endX." ) );

	//xを求める
	dectype x = beginX + ( ( endX - beginX ) * now );

	//方程式に設定
	dectype y = a * ( x * x * x ) + b;

	return dest.Set( x , y , z );
}

inline CBasicVector3Vector &DotsCubicCurveCurve( CBasicVector3Vector &dest , dectype beginX , dectype endX , dectype accuracy , dectype a , dectype b , dectype z )
{
	SKY_ASSERT_MSG( accuracy >= 0.0f , _T( "Please set accuracy to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( accuracy <= 1.0f , _T( "Please set accuracy to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( beginX < endX , _T( "BeginX must be larger than endX." ) );

	skyBool isPlot = skyTrue;
	dectype now = 0.0f;
	while( isPlot )
	{
		CBasicVector3 plot;
		GetCubicCurvePoint( plot , beginX , endX , now , a , b , z );

		dest.Add( plot );

		now += accuracy;

		if ( now > 1.0f )
		{
			if ( now - 1.0f < 0.0001 ) //誤差範囲の場合は1.0fとみなす
			{
				now = 1.0f;
			}
			else
			{
				isPlot = skyFalse;
			}
		}
	}

	return dest;
}

inline CBasicVector3 &GetCirclePoint( CBasicVector3 &dest , dectype r , dectype now , dectype beginRad , dectype endRad , dectype z )
{
	SKY_ASSERT_MSG( now >= 0.0f , _T( "Please set now to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG(  now <= 1.0f , _T( "Please set now to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( r > 0.0f , _T( "Please set r over 0 \n" ) );
	SKY_ASSERT_MSG( beginRad < endRad , _T( "beginRad must be larger than endRad." ) );

	dectype rad = beginRad + ( ( endRad - beginRad ) * now );

	dectype x = r * CMathUtil::Cos( rad );
	dectype y = r * CMathUtil::Sin( rad );

	return dest.Set( x , y , z );
}

inline CBasicVector3Vector &DotsCircle( CBasicVector3Vector &dest , dectype r , dectype beginRad , dectype endRad , dectype accuracy , dectype z )
{
	SKY_ASSERT_MSG( accuracy >= 0.0f , _T( "Please set accuracy to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( accuracy <= 1.0f , _T( "Please set accuracy to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( r > 0.0f , _T( "Please set r over 0 \n" ) );
	SKY_ASSERT_MSG( beginRad < endRad , _T( "beginRad must be larger than endRad." ) );

	skyBool isPlot = skyTrue;
	dectype now = 0.0f;
	while( isPlot )
	{
		CBasicVector3 plot;
		GetCirclePoint( plot , r , now , beginRad , endRad , z );

		dest.Add( plot );

		now += accuracy;

		if ( now > 1.0f )
		{
			if ( now - 1.0f < 0.0001 ) //誤差範囲の場合は1.0fとみなす
			{
				now = 1.0f;
			}
			else
			{
				isPlot = skyFalse;
			}
		}
	}

	return dest;
}

inline CBasicVector3 &GetEllipticalPoint( CBasicVector3 &dest , dectype ar , dectype br , dectype now , dectype beginRad , dectype endRad , dectype z )
{
	SKY_ASSERT_MSG( now >= 0.0f , _T( "Please set now to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG(  now <= 1.0f , _T( "Please set now to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( ar > 0.0f , _T( "Please set ar over 0 \n" ) );
	SKY_ASSERT_MSG( br > 0.0f , _T( "Please set br over 0 \n" ) );
	SKY_ASSERT_MSG( beginRad < endRad , _T( "beginRad must be larger than endRad." ) );

	dectype rad = beginRad + ( ( endRad - beginRad ) * now );

	dectype x = ar * CMathUtil::Cos( rad );
	dectype y = br * CMathUtil::Sin( rad );

	return dest.Set( x , y , z );
}

inline CBasicVector3Vector &DotsElliptical( CBasicVector3Vector &dest , dectype ar , dectype br , dectype beginRad , dectype endRad , dectype accuracy , dectype z )
{
	SKY_ASSERT_MSG( accuracy >= 0.0f , _T( "Please set accuracy to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( accuracy <= 1.0f , _T( "Please set accuracy to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( ar > 0.0f , _T( "Please set ar over 0 \n" ) );
	SKY_ASSERT_MSG( br > 0.0f , _T( "Please set br over 0 \n" ) );
	SKY_ASSERT_MSG( beginRad < endRad , _T( "beginRad must be larger than endRad." ) );

	skyBool isPlot = skyTrue;
	dectype now = 0.0f;
	while( isPlot )
	{
		CBasicVector3 plot;
		GetEllipticalPoint( plot , ar , br , now , beginRad , endRad , z );

		dest.Add( plot );

		now += accuracy;

		if ( now > 1.0f )
		{
			if ( now - 1.0f < 0.0001 ) //誤差範囲の場合は1.0fとみなす
			{
				now = 1.0f;
			}
			else
			{
				isPlot = skyFalse;
			}
		}
	}

	return dest;
}

inline CBasicVector3 &GetHyperbolaPoint( CBasicVector3 &dest , dectype a , dectype b , dectype now , dectype beginRad , dectype endRad , dectype z )
{
	SKY_ASSERT_MSG( now >= 0.0f , _T( "Please set now to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG(  now <= 1.0f , _T( "Please set now to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( a > 0.0f , _T( "Please set a over 0 \n" ) );
	SKY_ASSERT_MSG( b > 0.0f , _T( "Please set b over 0 \n" ) );
	SKY_ASSERT_MSG( beginRad < endRad , _T( "beginRad must be larger than endRad." ) );

	dectype rad = beginRad + ( ( endRad - beginRad ) * now );

	dectype x = a / CMathUtil::Cos( rad );
	dectype y = b * CMathUtil::Tan( rad );

	return dest.Set( x , y , z );
}

inline CBasicVector3Vector &DotsHyperbola( CBasicVector3Vector &dest , dectype a , dectype b , dectype beginRad , dectype endRad , dectype accuracy , dectype z )
{
	SKY_ASSERT_MSG( accuracy >= 0.0f , _T( "Please set accuracy to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( accuracy <= 1.0f , _T( "Please set accuracy to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( a > 0.0f , _T( "Please set a over 0 \n" ) );
	SKY_ASSERT_MSG( b > 0.0f , _T( "Please set b over 0 \n" ) );
	SKY_ASSERT_MSG( beginRad < endRad , _T( "beginRad must be larger than endRad." ) );

	skyBool isPlot = skyTrue;
	dectype now = 0.0f;
	while( isPlot )
	{
		CBasicVector3 plot;
		GetHyperbolaPoint( plot , a , b , now , beginRad , endRad , z );

		dest.Add( plot );

		now += accuracy;

		if ( now > 1.0f )
		{
			if ( now - 1.0f < 0.0001 ) //誤差範囲の場合は1.0fとみなす
			{
				now = 1.0f;
			}
			else
			{
				isPlot = skyFalse;
			}
		}
	}

	return dest;
}

inline CBasicVector3 &GetTrochoidPoint( CBasicVector3 &dest , dectype rm , dectype rd , dectype now , dectype beginRad , dectype endRad , dectype z )
{
	SKY_ASSERT_MSG( now >= 0.0f , _T( "Please set now to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG(  now <= 1.0f , _T( "Please set now to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( beginRad < endRad , _T( "beginRad must be larger than endRad." ) );

	dectype rad = beginRad + ( ( endRad - beginRad ) * now );

	dectype x = rm * rad - rd * CMathUtil::Sin( rad );
	dectype y = rm - rd * CMathUtil::Cos( rad );

	return dest.Set( x , y , z );
}

inline CBasicVector3Vector &DotsTrochoid( CBasicVector3Vector &dest , dectype rm , dectype rd , dectype beginRad , dectype endRad , dectype accuracy , dectype z )
{
	SKY_ASSERT_MSG( accuracy >= 0.0f , _T( "Please set accuracy to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( accuracy <= 1.0f , _T( "Please set accuracy to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( beginRad < endRad , _T( "beginRad must be larger than endRad." ) );

	skyBool isPlot = skyTrue;
	dectype now = 0.0f;
	while( isPlot )
	{
		CBasicVector3 plot;
		GetTrochoidPoint( plot , rm , rd , now , beginRad , endRad , z );

		dest.Add( plot );

		now += accuracy;

		if ( now > 1.0f )
		{
			if ( now - 1.0f < 0.0001 ) //誤差範囲の場合は1.0fとみなす
			{
				now = 1.0f;
			}
			else
			{
				isPlot = skyFalse;
			}
		}
	}

	return dest;
}

inline CBasicVector3 &GetClothoidPoint( CBasicVector3 &dest , dectype r , dectype now , dectype beginRad , dectype endRad , dectype z )
{
	SKY_ASSERT_MSG( now >= 0.0f , _T( "Please set now to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG(  now <= 1.0f , _T( "Please set now to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( beginRad < endRad , _T( "beginRad must be larger than endRad." ) );

	dectype rad = beginRad + ( ( endRad - beginRad ) * now );

	dectype x = r * ( CMathUtil::Cos( ( rad * rad ) / 2 ) * rad );
	dectype y = r * ( CMathUtil::Sin( ( rad * rad ) / 2 ) * rad );

	return dest.Set( x , y , z );
}

inline CBasicVector3Vector &DotsClothoid( CBasicVector3Vector &dest , dectype r , dectype beginRad , dectype endRad , dectype accuracy , dectype z )
{
	SKY_ASSERT_MSG( accuracy >= 0.0f , _T( "Please set accuracy to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( accuracy <= 1.0f , _T( "Please set accuracy to 0.0f - 1.0f \n" ) );
	SKY_ASSERT_MSG( beginRad < endRad , _T( "beginRad must be larger than endRad." ) );

	skyBool isPlot = skyTrue;
	dectype now = 0.0f;
	while( isPlot )
	{
		CBasicVector3 plot;
		GetClothoidPoint( plot , r , now , beginRad , endRad , z );

		dest.Add( plot );

		now += accuracy;

		if ( now > 1.0f )
		{
			if ( now - 1.0f < 0.0001 ) //誤差範囲の場合は1.0fとみなす
			{
				now = 1.0f;
			}
			else
			{
				isPlot = skyFalse;
			}
		}
	}

	return dest;
}

inline void ConvertFromPolarCoordinatesToRectangularCoordinate( CBasicVector2 &dest , const CBasicVector2 &src )
{
	dest.X = src.X * CMathUtil::Cos( src.Y );
	dest.Y = src.X * CMathUtil::Sin( src.Y );
}

inline void ConvertFromRectangularCoordinateToPolarCoordinates( CBasicVector2 &dest , const CBasicVector2 &src )
{
	//半径を求める
	dest.X = CMathUtil::Sqrt( ( src.X * src.X ) + ( src.Y * src.Y ) );

	if ( src.X == 0 && src.Y == 0 )
	{
		dest.Y = 0;
	}
	else
	{
		//角度を求める
		dest.Y = CMathUtil::Atan( src.Y / src.X );
	}

	//第一象限
	if( src.X >= 0 && src.Y >= 0 )
	{
		//そのまま
	}
	//第二象限(180を足す）
	else if ( src.X < 0 && src.Y >= 0 )
	{
		dest.Y += util::KPI;
	}
	//第三象限(180を足す）
	else if ( src.X < 0 && src.Y < 0 )
	{
		dest.Y += util::KPI;
	}
	//第四象限(360を足す）
	else if ( src.X >= 0 && src.Y < 0 )
	{
		dest.Y += util::K2PI;
	}
}

inline void ConvertFromPolarCoordinatesToRectangularCoordinate( CBasicVector3 &dest , const CBasicVector3 &src )
{
	dest.X = src.X * CMathUtil_::Cos( src.Z ) * CMathUtil_::Cos( src.Y );
	dest.Y = src.X * CMathUtil_::Cos( src.Z ) * CMathUtil_::Sin( src.Y );
	dest.Z = src.X * CMathUtil_::Sin( src.Z );
}

inline void ConvertFromRectangularCoordinateToPolarCoordinates( CBasicVector3 &dest , const CBasicVector3 &src )
{
	dest.X = CMathUtil::Sqrt( ( src.X * src.X ) + ( src.Y * src.Y ) + ( src.Z * src.Z ) );
	dest.Y = CMathUtil::Atan2( src.Y , src.X );
	dest.Z = CMathUtil::Atan2( src.Z , CMathUtil_::Sqrt( ( src.X * src.X ) + ( src.Y * src.Y ) ) );
}

}

} } }