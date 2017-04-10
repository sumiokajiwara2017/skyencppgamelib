#include "StdAfx.h"
#include "SkyMathLib/Geometry.h"

namespace sky { namespace lib { namespace math {

COBB2::COBB2() : CGeometryBase()
{
}

skyBool COBB2::IsZero() const
{
	return ( m_Sc.Length() == 0 );
}

void COBB2::Create( const CBasicVector2Vector &points )
{
	SKY_ASSERT( points.GetNum() > 0 );

    //最大値、最小値の初期値設定
    CBasicVector2 max = points[ 0 ];
    CBasicVector2 min = points[ 0 ];

    //最大値、最小値取得ループ
	for( u32 i = 0; i < points.GetNum(); i++)
    {
        const CBasicVector2 *pos = &points[ i ];
        if( pos->X < min.X )min.X = pos->X;
        if( pos->X > max.X )max.X = pos->X;
        if( pos->Y < min.Y )min.Y = pos->Y;
        if( pos->Y > max.Y )max.Y = pos->Y;
    }

    //中心点取得
    m_Pos = ( min + max ) * 0.5f;

    //長さ取得
    m_Sc.X = CMathUtil::Abs( max.X - min.X );
    m_Sc.Y = CMathUtil::Abs( max.Y - min.Y );
}

CBasicVector2 COBB2::GetAxis( eAxis axis ) const
{
	CBasicVector2 result;

	switch ( axis )
	{
	case eAxis_x:
		result.X = m_Matrix.M11;
		result.Y = m_Matrix.M12; 
		break;
	case eAxis_y:
		result.X = m_Matrix.M21;
		result.Y = m_Matrix.M22; 
		break;
	default:
		SKY_PANIC();
		break;
	}
	
	return result;
}

dectype COBB2::GetRadius( eAxis axis ) const
{
	dectype result = 1.0f;

	switch ( axis )
	{
	case eAxis_x:
		result = m_Sc.X;
		break;
	case eAxis_y:
		result = m_Sc.Y;
		break;
	default:
		SKY_PANIC();
		break;
	}
	
	return ( dectype )( result * 0.5 );
}

void COBB2::Merge( const COBB2 &obb )
{

}

void COBB2::ClosestPtPoint( const CBasicVector2 &point , CBasicVector2 &dest ) const
{
    CBasicVector2 d = point - m_Pos;
    dest = m_Pos;
    dectype dist;
    for( s32 i = 0 ; i < 2 ; i++ )
    {
		eAxis axis = eAxis_x;
		switch( i )
		{
		case 0: axis = eAxis_x; break;
		case 1: axis = eAxis_y; break;
		}

		dist = d.Dot( GetAxis( axis ) );

        if( dist > GetRadius( axis ) )
        {
            dist = GetRadius( axis );
        }
        if( dist < -GetRadius( axis ) )
        {
            dist = -GetRadius( axis );
        }

        dest += GetAxis( axis ) * dist;
    }
}

dectype COBB2::SqDistPoint( const CBasicVector2 &point ) const 
{
    CBasicVector2 closest;
    ClosestPtPoint( point , closest );
	dectype sqDist = ( closest - point ).Dot(closest - point );
    return sqDist;
}

CBasicVector2 COBB2::GetExtendedAxisX() const
{
	return CBasicVector2( m_Matrix.M11 * m_Sc.X ,
			                m_Matrix.M12 * m_Sc.X );
}

CBasicVector2 COBB2::GetExtendedAxisY() const
{
	return CBasicVector2( m_Matrix.M21 * m_Sc.Y ,
			                m_Matrix.M22 * m_Sc.Y );
}

dectype COBB2::GetEffectiveDiameter( const CBasicVector2& direction ) const
{
	return ( CMathUtil::Abs( direction.Dot( GetExtendedAxisX() ) ) +
			    CMathUtil::Abs( direction.Dot( GetExtendedAxisY() ) ) );
}

dectype COBB2::GetEffectiveRadius( const CBasicVector2& direction ) const
{
	return GetEffectiveDiameter( direction ) * 0.5f;
}

CBasicVector2 COBB2::GetCorner(int index) const
{
	SKY_ASSERT( ( index >= 0 ) && ( index < 4 ) );
	CBasicVector2 axisX = GetExtendedAxisX() / 2;
	CBasicVector2 axisY = GetExtendedAxisY() / 2;
	if(index == 0)
	{
		return m_Pos - axisX - axisY;
	}
	else if( index == 1 )
	{
		return m_Pos - axisX + axisY;
	}
	else if( index == 2 )
	{
		return m_Pos + axisX + axisY;
	}
	else if( index == 3 )
	{
		return m_Pos + axisX - axisY;
	}
	return m_Pos;
}

void COBB2::GetCornerArray( CBasicVector2 corner[ 4 ] ) const
{
	CBasicVector2 axisX = GetExtendedAxisX() / 2;
	CBasicVector2 axisY = GetExtendedAxisY() / 2;
	corner[ 0 ] = m_Pos - axisX - axisY;
	corner[ 1 ] = m_Pos - axisX + axisY;
	corner[ 2 ] = m_Pos + axisX + axisY;
	corner[ 3 ] = m_Pos + axisX - axisY;
}

COBB2 COBB2::Transform( const CMatrix3x2& matrix ) const
{
	COBB2 result;
	result.m_Matrix = matrix * m_Matrix;
	result.m_Pos = m_Pos * matrix;
	result.m_Sc = m_Sc;
	return result;
}

} } }