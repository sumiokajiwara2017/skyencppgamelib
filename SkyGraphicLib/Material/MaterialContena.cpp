#include "StdAfx.h"
#include "SkyGraphicLib/Material.h"

SkyImplementRTTI( sky::lib::graphic::CMaterialContena , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

CMaterialPtr CMaterialDrawData::GetMaterial( skyBool isRequired )
{

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( m_Branch.IsEmpty() == skyFalse , _T( "CMaterialDrawData is not found." ) );
	}
#endif
	return m_Branch;
}

const serialize::CSerializeInterfacePtr& CMaterialDrawData::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CMaterialDrawDataPtr )spContainer )->m_DrawStartIndex = m_DrawStartIndex;
	( ( CMaterialDrawDataPtr )spContainer )->m_DrawEndIndex = m_DrawEndIndex;
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CMaterialDrawData::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CMaterialDrawData::Clone()
{
	return Copy( CMaterialDrawDataPtr( NEW__( CMaterialDrawData , CMaterialDrawData() ) ) );
}

serialize::CSerializeInterfacePtr CMaterialDrawData::CloneSharing()
{
	return Clone();
}

//----------------------------------------------------------------------------------------------------------------------------------

void CMaterialContena::AddMaterial( const CMaterialPtr &spMaterial , vrtxtype startIndex , vrtxtype endIndex )
{
	CMaterialDrawDataPtr spNewData = CMaterialDrawDataCreate_();
	spNewData->m_DrawStartIndex    = startIndex;
	spNewData->m_DrawEndIndex      = endIndex;
	spNewData->AttachBranch( spMaterial );
	AttachLeaf( spNewData );
}

CMaterialPtr CMaterialContena::GetMaterial( u32 index )
{
	SKY_ASSERT( index < m_LeafList.GetNum() );

	return GetMaterialData( index )->GetMaterial();
}

CMaterialDrawDataPtr CMaterialContena::GetMaterialData( u32 index )
{
	SKY_ASSERT( index < m_LeafList.GetNum() );

	return m_LeafList[ index ];
}

void CMaterialContena::SetDrawIndex( u32 index , vrtxtype drawStartIndex , vrtxtype drawEndIndex )
{
	SKY_ASSERT( index < m_LeafList.GetNum() );

	GetMaterialData( index )->m_DrawStartIndex = drawStartIndex;
	GetMaterialData( index )->m_DrawEndIndex = drawEndIndex;
}

CSceneGraphNodePtrList &CMaterialContena::GetMaterialDataArray()
{
	return this->m_LeafList;
}

const serialize::CSerializeInterfacePtr& CMaterialContena::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CMaterialContena::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CMaterialContena::Clone()
{
	return Copy( CMaterialContenaPtr( NEW__( CMaterialContena , CMaterialContena() ) ) );
}

serialize::CSerializeInterfacePtr CMaterialContena::CloneSharing()
{
	return Clone();
}

} } }