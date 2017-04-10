#include "StdAfx.h"
#include "SkyGraphicLib/Camera.h"

namespace sky { namespace lib { namespace graphic {

skyBool CFrustum::Inside( const math::CBasicVector3 &point )
{
	for (int i=0; i<NumPlanes; ++i)
	{
		if ( !m_Planes[i].Inside( point ) )
			return skyFalse;
	}

	return skyTrue;
}

skyBool CFrustum::Inside( const math::CBasicVector3 &point, const dectype radius )
{
	for(int i = 0; i < NumPlanes; ++i) 
	{	
		if (!m_Planes[i].Inside( point, radius ) )
			return skyFalse;
	}	
		
	return skyTrue;
}

void CFrustum::Update( const dectype fov , const dectype aspect , const dectype nearClip , const dectype farClip , const math::CBasicVector3 &right , const math::CBasicVector3 &up , const math::CBasicVector3 &foward )
{
	m_Fov = fov;
	m_Aspect = aspect;
	m_Near = nearClip;
	m_Far = farClip;

	dectype tanFovOver2 = math::CMathUtil::Tan( m_Fov / 2.0f );
	math::CBasicVector3 nearRight = right * ( ( m_Near * tanFovOver2 ) * m_Aspect );
	math::CBasicVector3 farRight  = right * ( ( m_Far * tanFovOver2 ) * m_Aspect );
	math::CBasicVector3 nearUp    = up * ( m_Near * tanFovOver2 );
	math::CBasicVector3 farUp     = up * ( m_Far * tanFovOver2 );

	m_NearClip[0] = ( foward * m_Near ) - nearRight + nearUp;
	m_NearClip[1] = ( foward * m_Near ) + nearRight + nearUp;
	m_NearClip[2] = ( foward * m_Near ) + nearRight - nearUp;
	m_NearClip[3] = ( foward * m_Near ) - nearRight - nearUp;

	m_FarClip[0] = ( foward * m_Far ) - farRight + farUp;
	m_FarClip[1] = ( foward * m_Far ) + farRight + farUp;
	m_FarClip[2] = ( foward * m_Far ) + farRight - farUp;
	m_FarClip[3] = ( foward * m_Far ) - farRight - farUp;

	math::CBasicVector3 origin( 0.0f , 0.0f , 0.0f );
	m_Planes[ Near].CreatePlane( m_NearClip[ 2 ] , m_NearClip[ 1 ] , m_NearClip[ 0 ] );
	m_Planes[ Far ].CreatePlane( m_FarClip[ 0 ] , m_FarClip[ 1 ] , m_FarClip[ 2 ] );
	m_Planes[ Right ].CreatePlane( m_FarClip[ 2 ] , m_FarClip[ 1 ] , origin );
	m_Planes[ Top ].CreatePlane( m_FarClip[ 1 ], m_FarClip[ 0 ], origin );
	m_Planes[ Left ].CreatePlane(m_FarClip[ 0 ], m_FarClip[ 3 ], origin );
	m_Planes[ Bottom ].CreatePlane( m_FarClip[ 3 ] , m_FarClip[ 2 ] , origin );

}

} } }
