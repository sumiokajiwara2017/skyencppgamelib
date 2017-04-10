namespace sky { namespace lib { namespace math {

template< typename T >
CPlane< T >::CPlane() : CGeometryBase() , d( 0.0f )
{
}

template< typename T >
CPlane< T >::CPlane( const CVector3< T > &normal , const CVector3< T > &pos ) : CGeometryBase()
{
	CreatePlane( normal , pos );
}

template< typename T >
CPlane< T >::CPlane( const CVector3< T > &pos1 , const CVector3< T > &pos2 , const CVector3< T > &pos3 ) : CGeometryBase()
{
	CreatePlane( pos1 , pos2 , pos3 );
}

template< typename T >
CPlane< T >::CPlane( const CTriangle3< T > &triangle ) : CGeometryBase()
{
	CreatePlane( triangle );
}

template< typename T >
void CPlane< T >::CreatePlane( const CVector3< T > &pos1 , const CVector3< T > &pos2 , const CVector3< T > &pos3 )
{
	//同じ点があったら平面の計算ができない
	SKY_ASSERT_MSG( pos1 != pos2 , _T( "pos is same." ) );
	SKY_ASSERT_MSG( pos2 != pos3 , _T( "pos is same." ) );
	SKY_ASSERT_MSG( pos3 != pos1 , _T( "pos is same." ) );

	//時計回りの２辺のベクトルを計算する
	CVector3< T > vec1 = pos2 - pos1;
	CVector3< T > vec2 = pos3 - pos2;

	//法線を求める
	m_Normal = Cross<T>( vec1 , vec2 );
	m_Normal.Normalize();

	//任意の点1個と法線の内積でdを出す。
	d = pos1.Dot( m_Normal );

	//平面上の点を一つ保存しておく
	m_Pos = pos1;
}

template< typename T >
void CPlane< T >::CreatePlane( const CTriangle3< T > &triangle )
{
	CreatePlane( triangle.m_Pos[ 0 ] , triangle.m_Pos[ 1 ] , triangle.m_Pos[ 2 ] );
}

template< typename T >
void CPlane< T >::CreatePlane( const CVector3< T > &normal , const CVector3< T > &pos )
{
	m_Normal = normal;
	m_Normal.Normalize();

	d = -( ( m_Normal.X * pos.X ) + ( m_Normal.Y * pos.Y ) + ( m_Normal.Z * pos.Z ) ); 

	//平面上の点を一つ保存しておく
	m_Pos = pos;
}

template< typename T >
void CPlane< T >::CreatePlane( CVector3< T > pPoints[] , u32 pointNum )
{

//------------最適な法線abcを求める

    m_Normal.Zero();

	const CVector3< T > *p = &pPoints[ pointNum-1 ];

	for ( u32 i = 0 ; i < pointNum ; i++ )
	{
		const CVector3< T > *c = &pPoints[ i ];

		m_Normal.X += ( p->Z + c->Z ) * ( p->Y - c->Y );
		m_Normal.Y += ( p->X + c->X ) * ( p->Z - c->Z );
		m_Normal.Z += ( p->Y + c->Y ) * ( p->X - c->X );

		p = c;
	}

	m_Normal.Normalize();

//------------最適なdを求める

	T dw = 0;
	for ( u32 i = 0 ; i < pointNum ; i++ )
	{
		const CVector3< T > *c = &pPoints[ i ];

        dw += c->Dot( m_Normal );
	}

	d = dw / pointNum;

//------------平面上の点を一つ保存しておく
	m_Pos = pPoints[ 0 ];

}

template< typename T >
s32 CPlane< T >::ColPointPlaneSide( CVector3< T > &pos ) const
{
    T				aw;
    CVector3< T >	vec;

	vec = pos - m_Pos;
	aw = vec.Dot( m_Normal );
	
    if( aw > 0.0 )        //表側
	{
        return 1;
	}
    else if( aw < 0.0 )   //裏側
	{
        return -1;
	}
    else                 //平面上
	{
        return 0;
	}
}

template< typename T >
skyBool CPlane< T >::Inside( const CVector3< T > &point ) const
{
	T result = Dot( point );

	return (result >= 0.0f);
}

template< typename T >
skyBool CPlane< T >::Inside( const CVector3< T > &point, const T radius ) const
{
	T dDistance = Dot( point );

	return (dDistance >= -radius);
}

template< typename T >
T CPlane< T >::Dot( const CVector3< T > &point ) const
{
	return m_Normal.Dot( point ) + d;
}

template< typename T >
typename CPlane< T > &CPlane< T >::Normalize()
{
	CVector4< T > vec4( m_Normal.X , m_Normal.Y , m_Normal.Z , d );

	vec4.Normalize();

	m_Normal.X = vec4.X;
	m_Normal.Y = vec4.Y; 
	m_Normal.Z = vec4.Z;
	d          = vec4.W;

	return *this;
}

template< typename T >
CPlane< T >::operator const CVector4< T >() const
{
	return CVector4< T >( m_Normal.X , m_Normal.Y , m_Normal.Z , d );
}

} } }
