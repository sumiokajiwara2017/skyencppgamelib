#include "StdAfx.h"
#include "SkyGraphicLib/Mesh.h"

SkyImplementRTTI( sky::lib::graphic::CMesh , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

CVertexBufferPtr CMesh::GetVertexBuffer( skyBool isRequired )
{
#ifdef SW_SKYLIB_DEBUG_ON
	if ( m_Branch.IsEmpty() == skyTrue && isRequired == skyTrue )
	{
		SKY_PANIC_MSG( _T( "VertexBuffer is not found." ) );
	}
#endif
	return m_Branch;
}

void CMesh::AttachVertexBuffer( const CVertexBufferPtr &spVertex )
{
	m_Branch = spVertex;
}

void CMesh::AttachStream( eVertexBufferStreamType eType , const CVertexBufferStreamPtr& stream )
{
	if ( stream.IsEmpty() ) return;

	stream->ID = ( s32 )eType;
	AttachBranch( stream );
}

void CMesh::DetachStream( eVertexBufferStreamType eType )
{
	SceneGraphNodeUtil::DetachBranchID< CVertexBufferStream >( thisPtr( CSceneGraphNodePtr ) , ( s32 )eType );
}

CVertexBufferStreamPtr CMesh::GetStream( eVertexBufferStreamType eType , skyBool isRequired )
{
	CVertexBufferStreamPtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachBranchID< CVertexBufferStream , CVertexBufferStreamPtr >( thisPtr( CSceneGraphNodePtr ) , ( s32 )eType , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "buffer is not found." ) );
	}
#endif
	return spResult;
}

void CMesh::DeleteCpuMemory()
{
	//頂点バッファの処理
	GetVertexBuffer()->DeleteCpuMemory();

	//インデックスバッファの処理
	GetIndexBuffer()->DeleteCpuMemory();
}

void CMesh::CreateGpuMemory()
{
	//頂点バッファの処理
	GetVertexBuffer()->CreateGpuMemory();

	//インデックスバッファの処理
	GetIndexBuffer()->CreateGpuMemory();
}

void CMesh::DeleteGpuMemory()
{
	//頂点バッファの処理
	GetVertexBuffer()->DeleteGpuMemory();

	//インデックスバッファの処理
	GetIndexBuffer()->DeleteGpuMemory();
}

void CMesh::WirteGpuMemory()
{
	//頂点バッファの処理
	GetVertexBuffer()->WirteGpuMemory();

	//インデックスバッファの処理
	GetIndexBuffer()->WirteGpuMemory();
}

void CMesh::AttachIndexBuffer( const CIndexBufferPtr &spIndexBuffer )
{
	this->AttachLeaf( spIndexBuffer );
	m_IndexBufferCash = spIndexBuffer;
}

CIndexBufferPtr CMesh::GetIndexBuffer( skyBool isRequired )
{
	if( m_IndexBufferCash.IsEmpty() == skyTrue )
	{
		CIndexBufferPtr spResult;
		skyBool isFound = SceneGraphNodeUtil::SeaachLeaf< CIndexBuffer , CIndexBufferPtr >( thisPtr( CSceneGraphNodePtr ) , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
		if ( isRequired == skyTrue )
		{
			SKY_ASSERT_MSG( isFound , _T( "not found." ) );
		}
#endif
		m_IndexBufferCash = spResult;
	}
	return m_IndexBufferCash;
}

void CMesh::AttachMaterialContena( const CMaterialContenaPtr &spMaterialContena )
{
	this->AttachLeaf( spMaterialContena );
	m_MaterialContenaCash = spMaterialContena;
}

CMaterialContenaPtr CMesh::GetMaterialContena( skyBool isRequired )
{
	if( m_MaterialContenaCash.IsEmpty() == skyTrue )
	{
		CMaterialContenaPtr spResult;
		skyBool isFound = SceneGraphNodeUtil::SeaachLeaf< CMaterialContena , CMaterialContenaPtr >( thisPtr( CSceneGraphNodePtr ) , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
		if ( isRequired == skyTrue )
		{
			SKY_ASSERT_MSG( isFound , _T( "not found." ) );
		}
#endif
		m_MaterialContenaCash = spResult;
	}
	return m_MaterialContenaCash;
}

void CMesh::AttachSkeleton( const math::CSkeletonPtr &spSkeleton )
{
	m_spJoinbPallet = spSkeleton;
}

math::CSkeletonPtr CMesh::GetSkeleton()
{
	return m_spJoinbPallet;
}

#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&m_Dummy , sizeof( u32 ) , isWrite ); \
			
skyBool CMesh::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "meshgroup bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN( skyTrue );

	return result;
}
			
skyBool CMesh::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "meshgroup bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN( skyFalse );

	return result;
}

const serialize::CSerializeInterfacePtr& CMesh::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CMeshPtr )spContainer )->m_Dummy = m_Dummy;
	return spContainer;
}
const serialize::CSerializeInterfacePtr& CMesh::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CMesh::Clone()
{
	return Copy( CMeshPtr( NEW__( CMesh , CMesh() ) ) );
}

serialize::CSerializeInterfacePtr CMesh::CloneSharing()
{
	return Clone();
}

} } }