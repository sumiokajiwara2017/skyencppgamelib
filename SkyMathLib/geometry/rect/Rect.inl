namespace sky { namespace lib { namespace math {

template< typename T >
CRect< T >::CRect() : CGeometryBase() ,
m_Pos( 0, 0 ) ,
m_kSize( 0, 0 )
{
}

template< typename T >
CRect< T >::CRect( T x , T y, T w, T h ) : CGeometryBase()
{
	m_Pos.X = x;
	m_Pos.Y = y;
	m_kSize.W = w;
	m_kSize.H = h;
}

template< typename T >
CRect< T >::~CRect()
{
}

template< typename T >
T& CRect< T >::X()
{
	return m_Pos.X;
}

template< typename T >
const T& CRect< T >::X() const
{
	return m_Pos.X;
}

template< typename T >
T& CRect< T >::Y()
{
	return m_Pos.Y;
}

template< typename T >
const T& CRect< T >::Y() const
{
	return m_Pos.Y;
}

template< typename T >
T& CRect< T >::W()
{
	return m_kSize.W;
}

template< typename T >
const T& CRect< T >::W() const
{
	return m_kSize.W;
}

template< typename T >
T& CRect< T >::H()
{
	return m_kSize.H;
}

template< typename T >
const T& CRect< T >::H() const
{
	return m_kSize.H;
}

template< typename T >
void CRect< T >::SetPos( const CVector2< T >& vec )
{ 
	m_Pos = vec;
}
template< typename T >
void CRect< T >::AddPos( const CVector2< T >& vec )
{ 
	m_Pos += vec;
}
template< typename T >
 const CVector2< T >& CRect< T >::GetPos() const
{ 
	return m_Pos;
}
template< typename T >
void CRect< T >::SetSize( const CSize< T >& size )
{ 
	m_kSize = size;
}
template< typename T >
void CRect< T >::SetSize( const CVector2< T > &size )
{
	m_kSize.W = size.X;
	m_kSize.H = size.Y;
}
template< typename T >
 const CSize< T >& CRect< T >::GetSize() const
{ 
	return m_kSize;
}
template< typename T >
 CVector2< T > CRect< T >::GetSizeVec2() const
{ 
	return CVector2< T >( m_kSize.W , m_kSize.H );
}
template< typename T >
void CRect< T >::Set( const CVector2< T >& vec, const CSize< T >& size )
{ 
	m_Pos = vec; m_kSize = size;
}
template< typename T >
void CRect< T >::Set( T x , T y, T w, T h )
{
	m_Pos.X = x; m_Pos.Y = y; m_kSize.W = w; m_kSize.H = h;
}

template< typename T >
skyBool CRect< T >::DotHitCheck( const CVector2< T >& dot ) const
{
	if (
		( dot.X <= ( m_Pos.X + m_kSize.W ) ) &&
		( dot.X >= m_Pos.X  ) &&
		( dot.Y <= ( m_Pos.Y + m_kSize.H ) ) &&
		( dot.Y >= m_Pos.Y  ) ) {
		return skyTrue;
	}
	return skyFalse;
}

template< typename T >
skyBool CRect< T >::RectHitCheck( const CRect< T >& rect ) const
{
	if ( ( m_Pos.X <= ( rect.GetPos().X + rect.GetSize().W ) ) &&
		( ( m_Pos.X + m_kSize.W ) >= rect.GetPos().X  ) &&
		( m_Pos.Y <= ( rect.GetPos().Y + rect.GetSize().H ) ) &&
		( ( m_Pos.Y + m_kSize.H ) >= rect.GetPos().Y  ) )
	{
		return skyTrue;
	}
	return skyFalse;
}

template< typename T >
void CRect< T >::GetCenterPoint( CVector2< T >& centerPos ) const
{
	centerPos.X = m_Pos.X + ( m_kSize.W / 2 );
	centerPos.Y = m_Pos.Y + ( m_kSize.H / 2 );
}

template< typename T >
skyBool CRect< T >::InsideCheck( const CRect& rect ) const
{
	if ( ( rect.GetPos().X <= m_Pos.X && rect.GetPos().X + rect.GetSize().W >= m_Pos.X + m_kSize.W ) &&
		 ( rect.GetPos().Y <= m_Pos.Y && rect.GetPos().Y + rect.GetSize().H >= m_Pos.Y + m_kSize.H ) )
	{
		return skyTrue;
	}
	else
	{
		return skyFalse;
	}
}

template< typename T >
skyBool	CRect< T >::IsEmpty() const
{
	return ( m_Pos.IsEmpty() && m_kSize.IsEmpty() );
}

template< typename T >
void CRect< T >::GetPos( CVector2< T > rect[] )
{
	rect[ 0 ] = m_Pos;
    rect[ 1 ] = CVector2< T >( m_Pos.X + m_kSize.W , m_Pos.Y );
    rect[ 2 ] = CVector2< T >( m_Pos.X , m_Pos.Y + m_kSize.H );
    rect[ 3 ] = CVector2< T >( m_Pos.X + m_kSize.W , m_Pos.Y + m_kSize.H );
}

} } }