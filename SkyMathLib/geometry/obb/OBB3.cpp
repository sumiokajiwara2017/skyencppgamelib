#include "StdAfx.h"
#include "SkyMathLib/Geometry.h"

namespace sky { namespace lib { namespace math {

COBB3::COBB3() : CGeometryBase()
{
	m_Matrix.Identity();
}

skyBool COBB3::IsZero() const
{
	return ( m_Sc.Length() == 0 );
}

void COBB3::Create( const CBasicVector3Vector &points )
{
	SKY_ASSERT( points.GetNum() > 0 );

    //最大値、最小値の初期値設定
    CBasicVector3 max = points[ 0 ];
    CBasicVector3 min = points[ 0 ];

    //最大値、最小値取得ループ
	for( u32 i = 0; i < points.GetNum(); i++)
    {
        const CBasicVector3 *pos = &points[ i ];
        if( pos->X < min.X )min.X = pos->X;
        if( pos->X > max.X )max.X = pos->X;
        if( pos->Y < min.Y )min.Y = pos->Y;
        if( pos->Y > max.Y )max.Y = pos->Y;
        if( pos->Z < min.Z )min.Z = pos->Z;
        if( pos->Z > max.Z )max.Z = pos->Z;
    }

    //中心点取得
    m_Pos = ( min + max ) * 0.5f;

    //長さ取得
    m_Sc.X = CMathUtil::Abs( max.X - min.X );
    m_Sc.Y = CMathUtil::Abs( max.Y - min.Y );
    m_Sc.Z = CMathUtil::Abs( max.Z - min.Z );
}

CBasicVector3 COBB3::GetAxis( eAxis axis ) const
{
	CBasicVector3 result;

	switch ( axis )
	{
	case eAxis_x:
		result.X = m_Matrix.M11;
		result.Y = m_Matrix.M12; 
		result.Z = m_Matrix.M13;
		break;
	case eAxis_y:
		result.X = m_Matrix.M21;
		result.Y = m_Matrix.M22; 
		result.Z = m_Matrix.M23;
		break;
	case eAxis_z:
		result.X = m_Matrix.M31;
		result.Y = m_Matrix.M32; 
		result.Z = m_Matrix.M33;
		break;
	default:
		SKY_PANIC();
		break;
	}
	
	return result;
}

dectype COBB3::GetRadius( eAxis axis ) const
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
	case eAxis_z:
		result = m_Sc.Z;
		break;
	default:
		SKY_PANIC();
		break;
	}
	
	return ( dectype )( result * 0.5 );
}

void COBB3::Merge( const COBB3 &obb )
{

}

void COBB3::ClosestPtPoint( const CBasicVector3 &point , CBasicVector3 &dest ) const
{
    CBasicVector3 d = point - m_Pos;
    dest = m_Pos;
    dectype dist;
    for( s32 i = 0 ; i < 3 ; i++ )
    {
		eAxis axis = eAxis_x;
		switch( i )
		{
		case 0: axis = eAxis_x; break;
		case 1: axis = eAxis_y; break;
		case 2: axis = eAxis_z; break;
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

dectype COBB3::SqDistPoint( const CBasicVector3 &point ) const 
{
    CBasicVector3 closest;
    ClosestPtPoint( point , closest );
	dectype sqDist = ( closest - point ).Dot(closest - point );
    return sqDist;
}

dectype COBB3::IntersectPlane( const CBasicPlane &plane ) const
{
	// 平面との距離が実効直径以下なら交差
	dectype effectiveDiameter = GetEffectiveDiameter( plane.m_Normal );
	return ( CMathUtil::Abs( plane.Dot( m_Pos ) ) <= effectiveDiameter );
}

CBasicVector3 COBB3::GetExtendedAxisX() const
{
	return CBasicVector3( m_Matrix.M11 * m_Sc.X ,
			                m_Matrix.M12 * m_Sc.X , 
							m_Matrix.M13 * m_Sc.X );
}

CBasicVector3 COBB3::GetExtendedAxisY() const
{
	return CBasicVector3( m_Matrix.M21 * m_Sc.Y ,
			                m_Matrix.M22 * m_Sc.Y , 
							m_Matrix.M23 * m_Sc.Y );
}

CBasicVector3 COBB3::GetExtendedAxisZ() const
{
	return CBasicVector3( m_Matrix.M31 * m_Sc.Z ,
			                m_Matrix.M32 * m_Sc.Z , 
							m_Matrix.M33 * m_Sc.Z );
}

dectype COBB3::GetEffectiveDiameter( const CBasicVector3& direction ) const
{
	return ( CMathUtil::Abs( direction.Dot( GetExtendedAxisX() ) ) +
			    CMathUtil::Abs( direction.Dot( GetExtendedAxisY() ) ) +
		        CMathUtil::Abs( direction.Dot( GetExtendedAxisZ() ) ) );
}

dectype COBB3::GetEffectiveRadius( const CBasicVector3& direction ) const
{
	return GetEffectiveDiameter( direction ) * 0.5f;
}

CBasicVector3 COBB3::GetCorner( u32 index ) const
{
	SKY_ASSERT( ( index >= 0 ) && ( index < 8 ) );
	CBasicVector3 axisX = GetExtendedAxisX() / 2;
	CBasicVector3 axisY = GetExtendedAxisY() / 2;
	CBasicVector3 axisZ = GetExtendedAxisZ() / 2;
	if(index == 0)
	{
		return m_Pos - axisX - axisY - axisZ;
	}
	else if( index == 1 )
	{
		return m_Pos - axisX + axisY - axisZ;
	}
	else if( index == 2 )
	{
		return m_Pos + axisX + axisY - axisZ;
	}
	else if( index == 3 )
	{
		return m_Pos + axisX - axisY - axisZ;
	}
	else if( index == 4 )
	{
		return m_Pos + axisX + axisY + axisZ;
	}
	else if( index == 5 )
	{
		return m_Pos - axisX + axisY + axisZ;
	}
	else if( index == 6 )
	{
		return m_Pos - axisX - axisY + axisZ;
	}
	else if( index == 7 )
	{
		return m_Pos + axisX - axisY + axisZ;
	}
	return m_Pos;
}

void COBB3::GetCornerArray( CBasicVector3 corner[ 8 ] ) const
{
	CBasicVector3 axisX = GetExtendedAxisX() / 2;
	CBasicVector3 axisY = GetExtendedAxisY() / 2;
	CBasicVector3 axisZ = GetExtendedAxisZ() / 2;
	corner[ 0 ] = m_Pos - axisX - axisY - axisZ;
	corner[ 1 ] = m_Pos - axisX + axisY - axisZ;
	corner[ 2 ] = m_Pos + axisX + axisY - axisZ;
	corner[ 3 ] = m_Pos + axisX - axisY - axisZ;
	corner[ 4 ] = m_Pos + axisX + axisY + axisZ;
	corner[ 5 ] = m_Pos - axisX + axisY + axisZ;
	corner[ 6 ] = m_Pos - axisX - axisY + axisZ;
	corner[ 7 ] = m_Pos + axisX - axisY + axisZ;
}

COBB3 COBB3::Transform( const CRotationMatrix& matrix ) const
{
	COBB3 result;
	result.m_Matrix = matrix * m_Matrix;
	result.m_Pos = m_Pos * matrix;
	result.m_Sc = m_Sc;
	return result;
}

} } }