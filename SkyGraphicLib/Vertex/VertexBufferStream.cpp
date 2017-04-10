#include "StdAfx.h"
#include "SkyGraphicLib/Vertex.h"

//===================================yCommonz====================================================>

SkyImplementRTTI( sky::lib::graphic::CVertexBufferStream , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&m_Size    , sizeof( u32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_UseSize , sizeof( u32 ) , isWrite ); \

skyBool CVertexBufferStream::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "vertexbuffer bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN( skyTrue );

	stream->vWriteReadStream( m_pCpuMemoryBuffer , m_Size , skyTrue );

	return result;
}

skyBool CVertexBufferStream::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "vertexbuffer bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN( skyFalse );

	m_pCpuMemoryBuffer = NEW_ARRAY__( u8 , m_Size );
	stream->vWriteReadStream( m_pCpuMemoryBuffer , m_Size , skyFalse );

	return result;
}

const serialize::CSerializeInterfacePtr& CVertexBufferStream::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CVertexBufferStreamPtr )spContainer )->m_Size = m_Size;
	( ( CVertexBufferStreamPtr )spContainer )->m_UseSize = m_UseSize;

    ( ( CVertexBufferStreamPtr )spContainer )->m_pCpuMemoryBuffer = NEW_ARRAY__( u8 , m_Size );
	CMemoryUtil_::Copy( ( ( CVertexBufferStreamPtr )spContainer )->m_pCpuMemoryBuffer , m_pCpuMemoryBuffer , m_Size );

	return spContainer;
}

const serialize::CSerializeInterfacePtr& CVertexBufferStream::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	Copy( spContainer );
	( ( CVertexBufferStreamPtr )spContainer )->m_pMember = m_pMember;
	( ( CVertexBufferStreamPtr )spContainer )->m_spIsActivated = m_spIsActivated;
	return spContainer;
}

serialize::CSerializeInterfacePtr CVertexBufferStream::Clone()
{
	return Copy( CVertexBufferStreamPtr( NEW__( CVertexBufferStream , CVertexBufferStream() ) ) );
}

serialize::CSerializeInterfacePtr CVertexBufferStream::CloneSharing()
{
	return CopySharing( Create( hash::CHashStringPtr() ) );
}

void CVertexBufferStream::Activate()
{
	ACTIVATE_COMMON();

	CreateGpuMemory();
	WirteGpuMemory();
}

void CVertexBufferStream::Deactivate()
{
	DEACTIVATE_COMMON();

	DeleteGpuMemory();
}

void CVertexBufferStream::InfoPrint()
{
	SKY_PRINTF( "%s " , GetRTTI()->GetName() );
	SKY_PRINTF( _T( "name=[ %s ] " ) , Name.GetString() );
	switch( ID )
	{
	case eVertexBufferStreamType_Position:     //CVertexBuffer Default
		InfoPrintPosition();
		break;
	case eVertexBufferStreamType_Normal:       //CMaterial Default
		InfoPrintNormal();
		break;
	case eVertexBufferStreamType_Uv:           //CMaterial Default
		InfoPrintUv();
		break;
	case eVertexBufferStreamType_Color:        //CMaterial Default
		InfoPrintColor();
		break;
	case eVertexBufferStreamType_Index:
		InfoPrintIndex();
		break;
	case eVertexBufferStreamType_Weights:
		InfoPrintWeights();
		break;
	}
}

void CVertexBufferStream::InfoPrintPosition()
{
	SKY_PRINTF( _T( "Position[ %d ] : " ) , m_Size / sizeof( vertex::CVertexFmt_Pos ) );
	for ( vrtxtype i = 0 ; i < m_Size / sizeof( vertex::CVertexFmt_Pos ) ; i++ )
	{
		SKY_PRINTF( _T( "[ %f %f %f ]" ) , GetPosition( i )->x , GetPosition( i )->y, GetPosition( i )->z );
	}
	SKY_PRINTF( _T( "\n" ) );
}
void CVertexBufferStream::InfoPrintNormal()
{
	SKY_PRINTF( _T( "Normal[ %d ] : " ) , m_Size / sizeof( vertex::CVertexFmt_Normal ) );
	for ( vrtxtype i = 0 ; i < m_Size / sizeof( vertex::CVertexFmt_Normal ) ; i++ )
	{
		SKY_PRINTF( _T( "[ %f %f %f ]" ) , GetNormal( i )->nx , GetNormal( i )->ny, GetNormal( i )->nz );
	}
	SKY_PRINTF( _T( "\n" ) );
}
void CVertexBufferStream::InfoPrintUv()
{
	SKY_PRINTF( _T( "Uv[ %d ] : " ) , m_Size / sizeof( vertex::CVertexFmt_UV ) );
	for ( vrtxtype i = 0 ; i < m_Size / sizeof( vertex::CVertexFmt_UV ) ; i++ )
	{
		SKY_PRINTF( _T( "[ %f %f ]" ) , GetUv( i )->tu , GetUv( i )->tv );
	}
	SKY_PRINTF( _T( "\n" ) );
}
void CVertexBufferStream::InfoPrintColor()
{
	SKY_PRINTF( _T( "Color[ %d ] : " ) , m_Size / sizeof( vertex::CVertexFmt_Color ) );
	for ( vrtxtype i = 0 ; i < m_Size / sizeof( vertex::CVertexFmt_Color ) ; i++ )
	{
		SKY_PRINTF( _T( "[ %f %f %f %f ]" ) , GetColor( i )->color[ 0 ] , GetColor( i )->color[ 1 ] , GetColor( i )->color[ 2 ] , GetColor( i )->color[ 3 ] );
	}
	SKY_PRINTF( _T( "\n" ) );
}
void CVertexBufferStream::InfoPrintIndex()
{
	SKY_PRINTF( _T( "Index[ %d ] : " ) , m_Size / sizeof( vertex::CVertexFmt_Index ) );
	for ( vrtxtype i = 0 ; i < m_Size / sizeof( vertex::CVertexFmt_Index ) ; i++ )
	{
		SKY_PRINTF( _T( "[ %d %d %d %d ]" ) , GetIndex( i )->index[ 0 ] , GetIndex( i )->index[ 1 ] , GetIndex( i )->index[ 2 ] , GetIndex( i )->index[ 3 ] );
	}
	SKY_PRINTF( _T( "\n" ) );
}
void CVertexBufferStream::InfoPrintWeights()
{
	SKY_PRINTF( _T( "Weights[ %d ] : " ) , m_Size / sizeof( vertex::CVertexFmt_Weights ) );
	for ( vrtxtype i = 0 ; i < m_Size / sizeof( vertex::CVertexFmt_Weights ) ; i++ )
	{
		SKY_PRINTF( _T( "[ %f %f %f %f ]" ) , GetWeights( i )->weights[ 0 ] , GetWeights( i )->weights[ 1 ] , GetWeights( i )->weights[ 2 ] , GetWeights( i )->weights[ 3 ] );
	}
	SKY_PRINTF( _T( "\n" ) );
}

} } }
