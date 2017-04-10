namespace sky { namespace lib { namespace kfm {

namespace FCurveUtil
{

template < class T >
CFCurve< T > *CreateFCurve( const CVectorPrimitive< T > &keys , f32 intvl , eInterpolateType eInterType )
{
	CFCurve< T > *result = NEW CFCurve< T >();

	for( u32 i = 0 ; i < keys.GetNum() ; i++ )
	{
		result->AddKey( intvl , keys[ i ] , eInterType ); 
	}

	return result;
}

}

} } }