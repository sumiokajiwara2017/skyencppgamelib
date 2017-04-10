#include "StdAfx.h"
#include "SkyGraphicLib/Nil.h"

SkyImplementRTTI( sky::lib::graphic::CNilNode , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

skyBool CNilNode::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "nil bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );

	return result;
}

skyBool CNilNode::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "nil bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );

	return result;
}

const serialize::CSerializeInterfacePtr& CNilNode::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CNilNode::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CNilNode::Clone()
{
	return Copy( CNilNodePtr( NEW__( CNilNode , CNilNode() ) ) );
}

serialize::CSerializeInterfacePtr CNilNode::CloneSharing()
{
	return Clone();
}

void CNilNode::Activate()
{
	ACTIVATE_COMMON();
}

void CNilNode::Deactivate()
{
	DEACTIVATE_COMMON();
}

CNilNode::CNilNode()
{
	//ÉWÉáÉCÉìÉgÇçÏê¨Ç∑ÇÈ
	SetJointUse();
}

} } }