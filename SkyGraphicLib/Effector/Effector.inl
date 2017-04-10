#pragma once

namespace sky { namespace lib { namespace graphic {

inline void CEffector::SetProjection( const math::CBasicVector3 &n )
{
	m_LocalMatrix.SetupProject( n );
}
inline void CEffector::SetProjection( const math::CBasicPlane &p , const math::CBasicVector3 &lightPos )
{
	m_LocalMatrix.SetupProject( p , lightPos );
}
inline void CEffector::MulProjection( const math::CBasicVector3 &n )
{
	math::CMatrix4x3 m;
	m.SetupProject( n );
	m_LocalMatrix *= m;
}
inline void CEffector::MulProjection( const math::CBasicPlane &p , const math::CBasicVector3 &lightPos )
{
	math::CMatrix4x3 m;
	m.SetupProject( p , lightPos );
	m_LocalMatrix *= m;
}

inline void CEffector::SetReflect( math::eAxis axis , dectype k )
{
	m_LocalMatrix.SetupReflect( axis , k );
}
inline void CEffector::SetReflect( const math::CBasicVector3 &n )
{
	m_LocalMatrix.SetupReflect( n );
}
inline void CEffector::MulReflect( math::eAxis axis , dectype k )
{
	math::CMatrix4x3 m;
	m.SetupReflect( axis , k );
	m_LocalMatrix *= m;
}
inline void CEffector::MulReflect( const math::CBasicVector3 &n )
{
	math::CMatrix4x3 m;
	m.SetupReflect( n );
	m_LocalMatrix *= m;
}

inline void CEffector::SetShear( math::eAxis axis , dectype s , dectype t )
{
	m_LocalMatrix.SetupShear( axis , s , t );
}
inline void CEffector::MulShear( math::eAxis axis , dectype s , dectype t )
{
	math::CMatrix4x3 m;
	m.SetupShear( axis , s , t );
	m_LocalMatrix *= m;
}

} } }