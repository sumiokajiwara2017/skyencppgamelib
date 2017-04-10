#include "StdAfx.h"
#include "SkyGraphicLib/Mesh.h"

SkyImplementRTTI( sky::lib::graphic::CMeshContena , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

void CMeshContena::AttachMesh( const CMeshPtr &mesh )
{
	m_LeafList.AddTail( mesh );
}

void CMeshContena::DeleteAllMesh()
{
	m_LeafList.DeleteAll();
}

CMeshPtr CMeshContena::GetMesh( u32 meshNo )
{
	return m_LeafList[ meshNo ];
}

u32 CMeshContena::GetMeshNum() const
{
	return m_LeafList.GetNum();
}

const serialize::CSerializeInterfacePtr& CMeshContena::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CMeshContena::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CMeshContena::Clone()
{
	return Copy( CMeshContenaPtr( NEW__( CMeshContena , CMeshContena() ) ) );
}

serialize::CSerializeInterfacePtr CMeshContena::CloneSharing()
{
	return Clone();
}

void CMeshContena::Activate()
{
	ACTIVATE_COMMON();

	CreateGpuMemory();
	WirteGpuMemory();
}

void CMeshContena::Deactivate()
{
	DEACTIVATE_COMMON();

	DeleteGpuMemory();
}

void CMeshContena::vRendering ( CRenderingParam &param )
{
	//メッシュグループの処理
	FOREACH( CSceneGraphNodePtrList , it , m_LeafList )
	{
		( *it )->vRendering ( param );
	}
}

void CMeshContena::DeleteCpuMemory()
{
	//メッシュグループの処理
	FOREACH( CSceneGraphNodePtrList , it , m_LeafList )
	{
		CMesh *pMesh = ( CMesh * )( *it ).GetPtrNoConst();
		pMesh->DeleteCpuMemory();
	}
}

void CMeshContena::CreateGpuMemory()
{
	//メッシュグループの処理
	FOREACH( CSceneGraphNodePtrList , it , m_LeafList )
	{
		CMesh *pMesh = ( CMesh * )( *it ).GetPtrNoConst();
		pMesh->CreateGpuMemory();
	}
}

void CMeshContena::DeleteGpuMemory()
{
	//メッシュグループの処理
	FOREACH( CSceneGraphNodePtrList , it , m_LeafList )
	{
		CMesh *pMesh = ( CMesh * )( *it ).GetPtrNoConst();
		pMesh->DeleteGpuMemory();
	}
}

void CMeshContena::WirteGpuMemory()
{
	//メッシュグループの処理
	FOREACH( CSceneGraphNodePtrList , it , m_LeafList )
	{
		CMesh *pMesh = ( CMesh * )( *it ).GetPtrNoConst();
		pMesh->WirteGpuMemory();
	}
}

} } }
