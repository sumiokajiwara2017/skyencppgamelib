namespace sky { namespace lib { namespace math {

//-------------------------------------< 実装 >---------------------------------------------//

inline dectype CMathUtil::Sqrt( dectype val )
{
	return sqrt( val );
}

inline dectype CMathUtil::Sin( dectype asin )
{
	return sin( asin );
}

inline dectype CMathUtil::Asin( dectype sin )
{
	return asin( sin );
}

inline dectype CMathUtil::Cos( dectype acos )
{
	return cos( acos );
}

inline dectype CMathUtil::Acos( dectype cos )
{
	return acos( cos );
}

inline dectype CMathUtil::Tan( dectype atan )
{
	return tan( atan );
}

inline dectype CMathUtil::Atan( dectype tan )
{
	return atan( tan );
}

inline dectype CMathUtil::Ctan( dectype tan )
{
	return 1 / Tan( tan );
}

inline dectype CMathUtil::Atan2( dectype x , dectype y )
{
	return atan2( x , y );
}

inline dectype CMathUtil::WrapPi(dectype theta)
{
	theta += util::KPI;
	theta -= Floor( ( dectype )(theta * util::K1OVER2PI) ) * util::K2PI;
	theta -= util::KPI;
	return theta;
}

inline dectype CMathUtil::SafeAcos(dectype x)
{

	if (x <= -1.0f) {
		return util::KPI;
	}
	if (x >= 1.0f) {
		return 0.0f;
	}
	return Acos(x);
}

inline dectype	CMathUtil::Floor( dectype val )
{
	return floor( val );
}

inline dectype CMathUtil::Abs( dectype val )
{
	return abs( val );
}

inline s32 CMathUtil::FAbs( s32 val )
{
	return abs( val );
}

inline dectype CMathUtil::FAbs( dectype val )
{
	return fabs( val );
}

inline dectype CMathUtil::Ceil( dectype val )
{
	return ceil(val);
}

inline dectype CMathUtil::Round( dectype val )
{
	dectype val_w = val + 0.5f;
	val_w = Floor( val_w );

	return val_w;
}

inline dectype	CMathUtil::DegToRad( dectype deg ) 
{ 
	return deg * util::KPIOVER180;
}

inline dectype	CMathUtil::RadToDeg( dectype rad ) 
{ 
	return rad * util::K180OVERPI;
}

inline void CMathUtil::SinCos( dectype *returnSin, dectype *returnCos , dectype rad )
{
	*returnSin = Sin( rad );
	*returnCos = Cos( rad );
}

inline dectype	CMathUtil::FovToZoom( dectype fov )	
{ 
	return 1.0f / Tan( fov * .5f );
}

inline dectype	CMathUtil::ZoomToFov( dectype zoom )	
{ 
	return 2.0f * Atan( 1.0f / zoom );
}

inline dectype  CMathUtil::ConvDegIn360( s32 deg )
{
	dectype degw = ( dectype )(deg % 360);
	if ( degw < 0 )
	{
		degw += 360.0f;
	}
	return degw;
}

inline void CMathUtil::GetUnitCirclePos( dectype deg , dectype *pPosX , dectype *pPosY , dectype length )
{
	//ラジアンに変換する
	dectype rad = DegToRad( deg );

	//Cos ,Sinの値を求める
	*pPosX = Cos( rad ) * length;
	*pPosY = Sin( rad ) * length;
}

inline void CMathUtil::GetUnitCirclePosWave( dectype deg , dectype *pPosX , dectype *pPosY , dectype length )
{
	//ラジアンに変換する
	dectype rad = DegToRad( deg );

	//Cos ,Sinの値を求める
	dectype cos = Cos( rad );
	dectype sin = Sin( rad );

	dectype degConv = ConvDegIn360( ( s32 )deg );

	//どの事象に居るかで処理を分ける
	if ( degConv >= 0 && degConv <= 90 )			//第一象限
	{
		//そのまま使う
	}
	else if ( degConv >= 90 && degConv <= 180 )		//第二象限
	{
		//X値マイナス
		cos *= -1;
	}
	else if ( degConv >= 180 && degConv <= 270 )	//第三象限
	{
		//XY値マイナス
		cos *= -1;
		sin *= -1;
	}
	else if ( degConv >= 270 && degConv <= 360 )	//第四象限
	{
		//Y値マイナス
		sin *= -1;
	}

	*pPosX = cos * length;
	*pPosY = sin * length;
}


inline dectype CMathUtil::Pow( dectype k , dectype num )
{ 
	return pow( k , num );
}

inline dectype CMathUtil::Mod( dectype param1 , dectype param2 )
{
	return fmod( param1 , param2 );
}

inline skyBool CMathUtil::NearyEq( dectype src1 , dectype src2 , dectype gosa )
{
	return ( FAbs( src1 - src2 ) < gosa )? skyTrue : skyFalse;
}

template <class Type>
inline void CMathUtil::Mul( Type &value , Type min , Type max )
{
    if ( min > value )
	{
		value = min;
	}
	else if ( max < value )
	{
		value = max;
	}
}

} } }