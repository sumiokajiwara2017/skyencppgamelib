#include "StdAfx.h"
#include "SkyFrameWorkLib/AI.h"

namespace sky { namespace lib { namespace framework {

CAIMovePath::~CAIMovePath()
{
}

CAIMovePath::CAIMovePath() : m_CurrentPointNo( 0 ) , m_ePathEnd( eAIPathEndType_End ) , m_eState( eState_Play )
{

}

CAIMovePath::CAIMovePath( const CAIMovePath& src )
{
	*this = src;
};

CAIMovePath& CAIMovePath::operator =( const CAIMovePath& src )
{
	m_CurrentPointNo = src.m_CurrentPointNo;
	m_PathVector     = src.m_PathVector;
	m_ePathEnd       = src.m_ePathEnd;
	return *this;

}

void CAIMovePath::AddGraph( const math::CEuclidGraph &graph , math::GraphIndexList &list )
{
	FOREACH( math::GraphIndexList , it , list )
	{
		AddPath( graph.GetNode( *it ).m_Position );
	}
}

void CAIMovePath::AddPath( const math::CBasicVector3 &pos )
{
	m_PathVector.Add( pos );
}

math::CBasicVector3 CAIMovePath::GetCurrentPoint()
{
	if ( m_PathVector.GetNum() > 0 )
	{
		return m_PathVector[ m_CurrentPointNo ];
	}
	else
	{
		return math::CBasicVector3_ZERO;
	}
}

void CAIMovePath::SetNextPoint()
{
	if ( IsFinalPoint() == skyFalse )
	{
		switch( m_eState )
		{
		case eState_Play :
			 m_CurrentPointNo++;
			break;
		case eState_Play_Reverse :
			m_CurrentPointNo--;
			break;
		}
	}
	else
	{
		switch( m_ePathEnd )
		{
			case eAIPathEndType_End :
				//何もしない
				break;
			case eAIPathEndType_Repetition :

				m_CurrentPointNo = 0;

				break;
			case eAIPathEndType_Back :
				//ステートを逆にする

				m_eState = ( m_eState == eState_Play ) ? eState_Play_Reverse : eState_Play;

				break;
		}
	}
}

skyBool CAIMovePath::IsFinalPoint()
{
	if ( m_eState == eState_Play )
	{
		return ( m_PathVector.GetNum() - 1 == m_CurrentPointNo );
	}
	else if ( m_eState == eState_Play_Reverse )
	{
		return ( 0 == m_CurrentPointNo );
	}
	return skyFalse;
}

void CAIMovePath::SetPathEndType( eAIPathEndType ePathEnd )
{
	m_ePathEnd = ePathEnd;
}

eAIPathEndType CAIMovePath::GetPathEndType() const
{
	return m_ePathEnd;
}

graphic::CModelPtr CAIMovePath::CreateModel( const graphic::CColor &color )
{
	return graphic::CModelUtil::CreateLine3D( m_PathVector , color );
}

} } }