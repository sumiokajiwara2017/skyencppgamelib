namespace sky { namespace lib { namespace math {

namespace GeometryUtil {

inline void ConvertCPosToLTPos( CBasicVector2 &dest , const CBasicVector2 &srcLTPos , const CBasicSize &srcScreenSize )
{
	dest.X =  srcLTPos.X - ( srcScreenSize.W / 2 );
	dest.Y =  srcLTPos.Y - ( srcScreenSize.H / 2 );

	dest.Y *= -1.0f;
}

inline void ConvertCPosToLTPosLT( CBasicVector2 &dest , const CBasicVector2 &srcLTPos , const CBasicSize &srcPolSize , const CBasicSize &srcScreenSize )
{
	dest.X =  srcLTPos.X - ( srcScreenSize.W / 2 ) + ( srcPolSize.W / 2 );
	dest.Y =  srcLTPos.Y - ( srcScreenSize.H / 2 ) + ( srcPolSize.H / 2 );

	dest.Y *= -1.0f;
}

inline math::CBasicVector3 ConvertRoopPos( const math::CBasicVector3 &max , const math::CBasicVector3 &min , const math::CBasicVector3 &pos )
{
	math::CBasicVector3 result = pos;


	//最小限界地点を超えてしまったら最大限界地点に移動
	if ( pos.X < min.X ) result.X = max.X;
	if ( pos.X > max.X ) result.X = min.X; 
	if ( pos.Y < min.Y ) result.Y = max.Y; 
	if ( pos.Y > max.Y ) result.Y = min.Y; 
	if ( pos.Z < min.Z ) result.Z = max.Z; 
	if ( pos.Z > max.Z ) result.Z = min.Z; 

	return result;
}

template< typename T >
inline CBasicVector3 ConvertToHomogeneous( const CBasicVector2 &src )
{
	return CBasicVector3( src.X , src.Y , 1 );
}

template< typename T >
inline CBasicVector2 ConvertFromHomogeneous( const CBasicVector3 &src )
{
	return CBasicVector2( src.X , src.Y ) / src.Z;
}

template< typename T >
inline CBasicVector4 ConvertToHomogeneous( const CBasicVector3 &src )
{
	return CBasicVector4( src.X , src.Y , src.Z , 1 );
}

template< typename T >
inline CBasicVector3 ConvertFromHomogeneous( const CBasicVector4 &src )
{
	return CBasicVector3( src.X , src.Y , src.Z ) / src.W;
}

}

} } }