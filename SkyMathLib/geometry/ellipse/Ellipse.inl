namespace sky { namespace lib { namespace math {

template< typename T >
CEllipse< T >::CEllipse() : CGeometryBase()
{
}

template< typename T >
CEllipse< T >::CEllipse( T x , T y, T radiusW , T radiusH , T rot ) : CGeometryBase()
{
	m_Pos.X = x;
	m_Pos.Y = y;
	m_Radius.W = radiusW;
	m_Radius.H = radiusH;
	m_Rot = rot;
}

template< typename T >
T& CEllipse< T >::X()
{
	return m_Pos.X;
}

template< typename T >
const T& CEllipse< T >::X() const
{
	return m_Pos.X;
}

template< typename T >
T& CEllipse< T >::Y()
{
	return m_Pos.Y;
}

template< typename T >
const T& CEllipse< T >::Y() const
{
	return m_Pos.Y;
}

template< typename T >
T& CEllipse< T >::RW()
{
	return m_Radius.W;
}

template< typename T >
const T& CEllipse< T >::RW() const
{
	return m_Radius.W;
}
template< typename T >
T& CEllipse< T >::RH()
{
	return m_Radius.H;
}

template< typename T >
const T& CEllipse< T >::RH() const
{
	return m_Radius.H;
}
template< typename T >
T& CEllipse< T >::ROT()
{
	return m_Rot;
}

template< typename T >
const T& CEllipse< T >::ROT() const
{
	return m_Rot;
}

template< typename T >
CEllipse< T >::~CEllipse()
{
}

template< typename T >
void CEllipse< T >::SetPos( CVector2< T >& vec )
{ 
	m_Pos = vec;
}

template< typename T >
CVector2< T >&	CEllipse< T >::GetPos()
{ 
	return m_Pos;	
}

template< typename T >
void CEllipse< T >::SetRadius( dectype radiusW , dectype radiusH )
{ 
	m_Radius.W = radiusW;
	m_Radius.H = radiusH;
}

template< typename T >
CSize< T >& CEllipse< T >::GetRadius()
{ 
	return m_Radius;
}

} } }