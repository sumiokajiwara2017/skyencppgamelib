namespace sky { namespace lib { namespace math {

template< typename T >
inline CAngleVector3< T >::CAngleVector3( u8 initType , T x , T y , T z )
{
	if ( initType == 0 )
	{
		m_X = CAngle< T >( x );
		m_Y = CAngle< T >( y );
		m_Z = CAngle< T >( z );
	}
	else if ( initType == 1 )
	{
		m_X.FromRad( x );
		m_Y.FromRad( y );
		m_Z.FromRad( z );
	}
	else if ( initType == 2 )
	{
		m_X.FromDeg( x );
		m_Y.FromDeg( y );
		m_Z.FromDeg( z );
	}
}

template< typename T >
inline CRotationMatrix &CAngleVector3< T >::ToMatrixXYZ( CRotationMatrix &dest ) const
{
	dest.SetUpXYZ( CBasicVector3( m_X.ToRad() , m_Y.ToRad() , m_Z.ToRad() ) );
	return dest;
}

template< typename T >
inline CRotationMatrix &CAngleVector3< T >::ToMatrixXZY( CRotationMatrix &dest ) const
{
	dest.SetUpXZY( CBasicVector3( m_X.ToRad() , m_Y.ToRad() , m_Z.ToRad() ) );
	return dest;
}

template< typename T >
inline CRotationMatrix &CAngleVector3< T >::ToMatrixYXZ( CRotationMatrix &dest ) const
{
	dest.SetUpYXZ( CBasicVector3( m_X.ToRad() , m_Y.ToRad() , m_Z.ToRad() ) );
	return dest;
}

template< typename T >
inline CRotationMatrix &CAngleVector3< T >::ToMatrixYZX( CRotationMatrix &dest ) const
{
	dest.SetUpYZX( CBasicVector3( m_X.ToRad() , m_Y.ToRad() , m_Z.ToRad() ) );
	return dest;
}

template< typename T >
inline CRotationMatrix &CAngleVector3< T >::ToMatrixZXY( CRotationMatrix &dest ) const
{
	dest.SetUpZXY( CBasicVector3( m_X.ToRad() , m_Y.ToRad() , m_Z.ToRad() ) );
	return dest;
}

template< typename T >
inline CRotationMatrix &CAngleVector3< T >::ToMatrixZYX( CRotationMatrix &dest ) const
{
	dest.SetUpZYX( CBasicVector3( m_X.ToRad() , m_Y.ToRad() , m_Z.ToRad() ) );
	return dest;
}

template< typename T >
inline CMatrix4x3 &CAngleVector3< T >::ToMatrixXYZ( CMatrix4x3 &dest ) const
{
	dest.SetupRotateXYZ( CBasicVector3( m_X.ToRad() , m_Y.ToRad() , m_Z.ToRad() ) );
	return dest;
}

template< typename T >
inline CMatrix4x3 &CAngleVector3< T >::ToMatrixXZY( CMatrix4x3 &dest ) const
{
	dest.SetupRotateXZY( CBasicVector3( m_X.ToRad() , m_Y.ToRad() , m_Z.ToRad() ) );
	return dest;
}

template< typename T >
inline CMatrix4x3 &CAngleVector3< T >::ToMatrixYXZ( CMatrix4x3 &dest ) const
{
	dest.SetupRotateYXZ( CBasicVector3( m_X.ToRad() , m_Y.ToRad() , m_Z.ToRad() ) );
	return dest;
}

template< typename T >
inline CMatrix4x3 &CAngleVector3< T >::ToMatrixYZX( CMatrix4x3 &dest ) const
{
	dest.SetupRotateYZX( CBasicVector3( m_X.ToRad() , m_Y.ToRad() , m_Z.ToRad() ) );
	return dest;
}

template< typename T >
inline CMatrix4x3 &CAngleVector3< T >::ToMatrixZXY( CMatrix4x3 &dest ) const
{
	dest.SetupRotateZXY( CBasicVector3( m_X.ToRad() , m_Y.ToRad() , m_Z.ToRad() ) );
	return dest;
}

template< typename T >
inline CMatrix4x3 &CAngleVector3< T >::ToMatrixZYX( CMatrix4x3 &dest ) const
{
	dest.SetupRotateZYX( CBasicVector3( m_X.ToRad() , m_Y.ToRad() , m_Z.ToRad() ) );
	return dest;
}

template< typename T >
inline void CAngleVector3< T >::ToVector( CVector3< T > &dest , T len ) const
{
#if 0

	CQuaternion qx , qy;
	qx.SetToRotateAboutY( m_Y.ToRad() );
	qy.SetToRotateAboutX( m_X.ToRad() );

	qx *= qy;

	m.GetCol2( dest );

	CMatrix4x3 m;
	m.FromQuaternion( qx );

#elif 1

	CMatrix4x3 mx , my;
	mx.SetupRotate( eAxis_x , m_Y.ToRad() );
	my.SetupRotate( eAxis_y , m_X.ToRad() );

	mx *= my;

	mx.GetCol2( dest );

#elif 0

	//Yé≤Ç≠ÉIÅ[É^ÉjÉIÉìâÒì]

	//Yé≤ÇXäpìxÇ≈âÒì]

	//Xé≤

#endif

	dest *= len;
}

template< typename T >
inline typename CVector3< T > CAngleVector3< T >::ToVector( T len ) const
{
	CVector3< T > dest;
	ToVector( dest , len );

	return dest;
}

template< typename T >
inline void CAngleVector3< T >::RotVector( CVector3< T > &dest ) const
{
	CMatrix4x3 m;
	ToMatrixYXZ( m );
	dest = m * dest;
}

template< typename T >
inline typename CVector3< T > CAngleVector3< T >::RotVector( const CVector3< T > &src ) const
{
	CMatrix4x3 m;
	ToMatrixYXZ( m );
	return m * src;
}

inline CBasicAngleVector3 LerpAngleVec_Liner( dectype t, const CBasicAngleVector3 &from , const CBasicAngleVector3 &to )
{
	return CBasicAngleVector3( from.X() + ( to.X() - from.X() ) * t,
				               from.Y() + ( to.Y() - from.Y() ) * t,
				               from.Z() + ( to.Z() - from.Z() ) * t );
}

inline CBasicAngleVector3 FaceTo( const CBasicVector3 &to )
{
	return CBasicAngleVector3( FaceXTo( to ), FaceYTo( to ) , CBasicAngle( ( s16 )0 ) ) ;
}
inline CBasicAngleVector3 FaceTo2( const CBasicVector3 &to )
{
	return CBasicAngleVector3( FaceXTo2( to ), FaceYTo( to ), CBasicAngle( ( s16 )0 ) ) ;
}

inline CBasicAngleVector3 FaceFromTo( const CBasicVector3 &from, const CBasicVector3 &to )
{
	return FaceTo( to - from ) ;
}


inline CBasicAngleVector3 faceTo( const CMatrix4x3 &rotMat )
{
	CBasicVector3	col0 , col1 , col2;

	rotMat.GetCol0( col0 );
	rotMat.GetCol1( col1 );
	rotMat.GetCol2( col2 );

	if( 
		( col0.Y == 0.0f && col1.Y == 0.0f ) ||
		( col2.X == 0.0f && col2.Z == 0.0f ) 
      )
	{
		return CBasicAngleVector3( 0 , 0 , 0 , 0 );
	}

	if( -col2.Y <= -1.0f )
	{
		return CBasicAngleVector3( 0 , RADtoANG( -util::KPI ) , RADtoANG( CMathUtil::Atan2( -col2.X , col2.Z ) ) , 0 );
	}
	else if( -col2.Y >= 1.0f )
	{
		return CBasicAngleVector3( 0 , RADtoANG( util::KPI ) , RADtoANG( -CMathUtil::Atan2( -col2.X , col2.Z ) ) , 0 );
	}

	return CBasicAngleVector3( RADtoANG( CMathUtil::Asin( -col2.Y ) ) ,
                               RADtoANG( CMathUtil::Atan2( col2.X , col2.Z ) ) ,
                               RADtoANG( CMathUtil::Atan2( col0.Y , col1.Y ) ) );
}

} } }