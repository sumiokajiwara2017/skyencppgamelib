namespace sky { namespace lib { namespace math {

template< typename T >
CSphere< T >::CSphere() : CGeometryBase() , m_Radius( 0.0f )
{
}

template< typename T >
CSphere< T >::CSphere( T x , T y , T z , T radius ) : CGeometryBase()
{
	m_Pos.X = x;
	m_Pos.Y = y;
	m_Pos.Z = z;
	m_Radius = radius;
}

template< typename T >
skyBool CSphere< T >::IsZero() const
{
	return ( m_Radius == 0 );
}

template< typename T >
T& CSphere< T >::X()
{
	return m_Pos.X;
}

template< typename T >
const T& CSphere< T >::X() const
{
	return m_Pos.X;
}

template< typename T >
T& CSphere< T >::Y()
{
	return m_Pos.Y;
}

template< typename T >
const T& CSphere< T >::Y() const
{
	return m_Pos.Y;
}

template< typename T >
T& CSphere< T >::Z()
{
	return m_Pos.Z;
}

template< typename T >
const T& CSphere< T >::Z() const
{
	return m_Pos.Z;
}

template< typename T >
T& CSphere< T >::R()
{
	return m_Radius;
}

template< typename T >
const T& CSphere< T >::R() const
{
	return m_Radius;
}


template< typename T >
CSphere< T >::~CSphere()
{
}

template< typename T >
void CSphere< T >::SetPos( const CVector3< T >& vec )
{ 
	m_Pos = vec;
}

template< typename T >
void CSphere< T >::AddPos( const CVector3< T >& vec )
{ 
	m_Pos += vec;
}

template< typename T >
const CVector3< T >&	CSphere< T >::GetPos() const
{ 
	return m_Pos;	
}

template< typename T >
void CSphere< T >::SetRadius( dectype radius )
{ 
	m_Radius = radius;
}

template< typename T >
void CSphere< T >::AddRadius( dectype radius )
{ 
	m_Radius += radius;
}

template< typename T >
T CSphere< T >::GetRadius() const
{ 
	return m_Radius;
}

template< typename T >
const CSphere< T > &CSphere< T >::Merge( const CSphere< T > &sphere )
{//Lamp

	CVector3< T > direction = sphere.m_Pos - m_Pos;
	// 同じ位置に存在する球
	if( direction.Length() == 0 )
	{
		if( sphere.m_Radius > m_Radius )
		{ 
			m_Radius = sphere.m_Radius;
		}
		return *this;
	}

	T distance = direction.Length();

	// ターゲットが自球の内側にある
	if( m_Radius >= sphere.m_Radius + distance )
	{ 
		return *this;
	}

	// ターゲットの内側に自球がある
	if( sphere.m_Radius >= m_Radius + distance )
	{
		*this = sphere;
		return *this;
	}

	// 球がお互い重なっていない
	direction *= ( 1.f / distance );
	m_Pos = ( ( m_Pos - direction * m_Radius ) +	( sphere.m_Pos + direction * sphere.m_Radius ) ) * 0.5f;
	m_Radius = ( m_Radius + sphere.m_Radius + distance ) * 0.5f;
	return *this;
}

template< typename T >
const CSphere< T > &CSphere< T >::Merge( const CVector3< T > &point )
{//Lamp

	CVector3< T > direction = point - m_Pos;

	// 中心と同じ位置に存在
	if( direction.Length() == 0 )
	{ 
		return *this;
	}
	T distance = direction.Length();

	// ベクトルが球内なら何もしない
	if( distance <= m_Radius )
	{ 
		return *this;
	}

	// ベクトルが球外にあるので中心を動かし、調整する
	direction *= ( 1.f / distance );

	m_Pos = ( ( m_Pos - direction * m_Radius ) + point ) * 0.5f;

	m_Radius = ( m_Radius + distance ) * 0.5f;

	return *this;
}

template< typename T >
const CSphere< T > &CSphere< T >::Append( const CVector3< T > &point )
{//Lamp
	CVector3< T > direction = point - m_Pos;

	// 中心と同じ位置に存在
	if( direction.Length() == 0 )
	{ 
		return *this;
	}

	T distance = direction.Length();

	// 球外なら拡張。中心は動かさない
	if( distance > m_Radius )
	{ 
		m_Radius = distance;
	}

	return *this;
}

template< typename T >
const CSphere< T > &CSphere< T >::CreateFromPoints( const CVectorPrimitive< CVector3< T > > &points )
{//元々のコードがSIMDで最適化されているためベクトルじゃなくてもいい部分がベクトルになってる可能性がある。
    SKY_ASSERT( points.GetNum() > 0 );

    //最大値、最小値の初期値設定
    CVector3< T > max = points[ 0 ];
    CVector3< T > min = points[ 0 ];

    //最大値、最小値取得ループ
	for( u32 i = 0; i < points.GetNum(); i++)
    {
        const CVector3< T > *pos = &points[ i ];
        if( pos->X < min.X )min.X = pos->X;
        if( pos->X > max.X )max.X = pos->X;
        if( pos->Y < min.Y )min.Y = pos->Y;
        if( pos->Y > max.Y )max.Y = pos->Y;
        if( pos->Z < min.Z )min.Z = pos->Z;
        if( pos->Z > max.Z )max.Z = pos->Z;
    }

    //中心点取得
    m_Pos = ( min + max ) * 0.5f;

    //最大値、最小値取得ループ
    m_Radius = 0;
	for( u32 i = 0; i < points.GetNum(); i++)
    {
		m_Radius = TemplateUtil::Max< T >( m_Radius , ( points[ i ] - m_Pos ).Length() );
    }
	return *this;
}

} } }