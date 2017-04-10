#include "StdAfx.h"
#include "SkyGraphicLib/Model.h"

#include "SkyGraphicLib/Renderer.h"

SkyImplementRTTI( sky::lib::graphic::CModel , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

//-------------------------------------< 実装 >---------------------------------------------//

void CModel::AttachMeshContena( const CMeshContenaPtr &spMesh )
{
	AttachBranch( spMesh );
}

CMeshContenaPtr CModel::GetMeshContena( skyBool isRequired )
{
	CMeshContenaPtr spResult;
	skyBool isFound = SceneGraphNodeUtil::SeaachBranch< CMeshContena , CMeshContenaPtr >( GetBranchRoot() , spResult );

#ifdef SW_SKYLIB_DEBUG_ON
	if ( isRequired == skyTrue )
	{
		SKY_ASSERT_MSG( isFound , _T( "Mesh is not found." ) );
	}
#endif
	return spResult;
}

void CModel::AttachShader( const CShaderObjectPtr &effectPtr )
{
	AttachLeaf( effectPtr );
}

void CModel::ClearAndAttachShader( const CShaderObjectPtr &effectPtr )
{
	DetachShader();
	AttachLeaf( effectPtr );
}

void CModel::DetachShader()
{
	SceneGraphNodeUtil::DetachLeafAll< CShaderObject >( thisPtr( CSceneGraphNodePtr ) );
}

void CModel::RenderingImple( CRenderingParam &param , const CShaderObjectPtr &shader )
{
	CExtraDataPtr spExtraData;
	if ( param.ExtraDataStack.IsEmpty() == skyFalse )
	{
		spExtraData = param.ExtraDataStack.GetTop();
	}

	for ( u32 i = 0 ; i < shader->GetPassNum() ; i++ )
	{
		for ( u32 j = 0 ; j < GetMeshContena()->GetMeshNum() ; j++ )
		{
			if ( IsAlpha() )
			{
				param.AlphaDataList.AddTail( FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &CRenderCommand::RenderingMesh , 
					CRenderCommand::RenderingMeshParam( i , param.ViewPortStack.GetTop() , param.CameraStack.GetTop() , shader , GetMeshContena()->GetMesh( j ) , param.MatrixStack.GetTop() , spExtraData , param.LightStack.GetTop() ) , IS_COMMAND ) ) );
			}
			else
			{
				CRenderCommand::RenderingMesh( 
					CRenderCommand::RenderingMeshParam( i , param.ViewPortStack.GetTop() , param.CameraStack.GetTop() , shader , GetMeshContena()->GetMesh( j ) , param.MatrixStack.GetTop() , spExtraData , param.LightStack.GetTop() ) );
			}
		}
	}
}

void CModel::vRendering( CRenderingParam &param )
{
	SKY_ASSERT_MSG( m_IsSetUp == skyTrue ,  _T( "It has not setup yet." ) );

	//もしメッシュがなければ空モデルと判断して描画を見送る
	if ( GetMeshContena( skyFalse ).IsEmpty() == skyFalse )
	{
		//メッシュのvRenderingを呼ぶ（基本Leaf,BranchのvRenderingは呼ばれない。呼ぶ必要がある特殊ケースはChildのvRenderingの実装内で呼ぶこと）
		GetMeshContena()->vRendering( param );

		//シェーダースタックに積まれているか？
		if ( param.ShaderStack.GetNum() > 0 )
		{//スタックのシェーダーのみ描画

			//スタックに積まれたシェーダーの描画
			FOREACH( CSceneGraphNodePtrStack , it , param.ShaderStack )
			{
				RenderingImple( param , *it );
			}
		}
		else
		{//アタッチシェーダーのみ描画

			//アタッチされたシェーダーによる描画
			FOREACH( CSceneGraphNodePtrList , it , m_LeafList )
			{
				if( SkyIsKindOf( CShaderObject , ( *it ).GetPtrNoConst() ) )
				{
					RenderingImple( param , *it );
				}
			}
		}
	}
}

#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&m_IsSetUp , sizeof( s32 ) , isWrite );

skyBool CModel::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "model bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN( skyTrue );

	return result;
}

skyBool CModel::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "model bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN( skyFalse );

	return result;
}

const serialize::CSerializeInterfacePtr &CModel::Copy( const serialize::CSerializeInterfacePtr &spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CModelPtr )spContainer )->m_IsSetUp = m_IsSetUp;
	( ( CModelPtr )spContainer )->m_LocalSize = m_LocalSize;
	( ( CModelPtr )spContainer )->m_LocalHalfSize = m_LocalHalfSize;
	( ( CModelPtr )spContainer )->m_LocalYUnderLength = m_LocalYUnderLength;
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CModel::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CModel::Clone()
{
	return Copy( CModelPtr( NEW__( CModel , CModel() ) ) );
}

serialize::CSerializeInterfacePtr CModel::CloneSharing()
{
	return Clone();
}

void CModel::Activate()
{
	SetJointUse();
	m_IsSetUp = skyTrue;

	//バウンディングボックスを空に
	m_AABB3.Empty();
}

void CModel::Deactivate()
{
}

CModel::CModel() : CSceneGraphNode()
{
	Constractor();
}

CModel::CModel( CSceneGraphNode &spNode ) : CSceneGraphNode( spNode )
{ 
	Constractor();
}

void CModel::Constractor()
{
	m_IsDebugNormalDispOn = skyFalse;
}

void CModel::ComputeAABB3()
{//メッシュと頂点バッファは存在することが前提

	CVertexBufferPtr spVertexBuffer = GetMeshContena()->GetMesh()->GetVertexBuffer();
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		m_AABB3.Merge( spVertexBuffer->GetPositionStream()->GetPosition( i )->Get() );
	}
}

void CModel::ComputeOBB3()
{
	CVertexBufferPtr spVertexBuffer = GetMeshContena()->GetMesh()->GetVertexBuffer();
	math::CBasicVector3Vector vector;
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		vector.Add( spVertexBuffer->GetPositionStream()->GetPosition( i )->Get() );
	}

	m_OBB3.Create( vector );
}

void CModel::ComputeBS()
{
	CVertexBufferPtr spVertexBuffer = GetMeshContena()->GetMesh()->GetVertexBuffer();
	math::CBasicVector3Vector vector;
	for ( vrtxtype i = 0 ; i < spVertexBuffer->GetVertexNum() ; i++ )
	{
		vector.Add( spVertexBuffer->GetPositionStream()->GetPosition( i )->Get() );
	}

	m_BS.CreateFromPoints( vector );
}

#ifdef SW_SKYLIB_DEBUG_ON

void CModel::SetDebugDisplayNormalOn( skyBool isOn , float length )
{
	CVertexBufferStreamPtr spNormalBuffer = GetMeshContena()->GetMesh()->GetVertexBuffer()->GetNormalStream();

	if ( spNormalBuffer.IsEmpty() == skyFalse && m_IsDebugNormalDispOn != isOn )
	{
		if ( isOn == skyTrue )
		{
			CVertexBufferStreamPtr spVertexBuffer = GetMeshContena()->GetMesh()->GetVertexBuffer()->GetVertexNormalStream();
			for ( vrtxtype i = 0 ; i < spVertexBuffer->GetNumPosition() ; i++ )
			{
				math::CBasicVector3 pos1   = spVertexBuffer->GetPosition( i )->Get();
				math::CBasicVector3 normal = spNormalBuffer->GetNormal( i )->Get();
				math::CBasicVector3 pos2   = pos1 + normal.Normalize() * length;

				math::CBasicVector3Vector vector;
				vector.Add( pos1 );
				vector.Add( pos2 );
				CModelPtr spLine = CModelUtil::CreateLine3D( vector , CColor_Lightsteelblue );
				spLine->Name.SetName( CHS( _T( "N" ) ) );
				AttachChild( spLine );
			}
		}
		else
		{
			FOREACH_NOINC( math::CJointPtrList , it , m_ChildList )
			{
				if ( ( *it )->Name == CHS( _T( "N" ) ) )
				{
					it = m_ChildList.Erases( it );
				}
				else
				{
					it++;
				}
			}
		}
		m_IsDebugNormalDispOn = isOn;
	}
}

#endif

void CModel::ComputeSizeData()
{
	math::CBasicVector3 maxSize( FLT_MIN , FLT_MIN , FLT_MIN );
	math::CBasicVector3 minSize( FLT_MAX , FLT_MAX , FLT_MAX );

	CMeshContenaPtr spMeshContena = GetMeshContena();
	for( u32 i = 0 ; i < spMeshContena->GetMeshNum() ; i++ )
	{
		CMeshPtr spMesh = spMeshContena->GetMesh( i );
		CVertexBufferPtr spVertex = spMesh->GetVertexBuffer();
		CVertexBufferStreamPtr spPositionStream = spVertex->GetPositionStream();
		for ( vrtxtype j = 0 ; j < spPositionStream->GetNumPosition() ; j++ )
		{
			vertex::CVertexFmt_Pos *pPos = spPositionStream->GetPosition( j );

			//Ｘ軸の最大候補
			if ( maxSize.X < pPos->x )
			{
				maxSize.X = pPos->x;
			}

			//Ｙ軸の最大候補
			if ( maxSize.Y < pPos->y )
			{
				maxSize.Y = pPos->y;
			}

			//Ｚ軸の最大候補
			if ( maxSize.Z < pPos->z )
			{
				maxSize.Z = pPos->z;
			}

			//Ｘ軸の最小候補
			if ( minSize.X > pPos->x )
			{
				minSize.X = pPos->x;
			}

			//Ｙ軸の最小候補
			if ( minSize.Y > pPos->y )
			{
				minSize.Y = pPos->y;
			}

			//Ｚ軸の最小候補
			if ( minSize.Z > pPos->z )
			{
				maxSize.Z = pPos->z;
			}
		}
	}

	m_LocalSize.X = maxSize.X - minSize.X;
	m_LocalSize.Y = maxSize.Y - minSize.Y;
	m_LocalSize.Z = maxSize.Z - minSize.Z;

	m_LocalHalfSize = m_LocalSize / 2;

	m_LocalYUnderLength = -minSize.Y;
}

CMaterialPtr CModel::GetMaterial( u32 meshNo , u32 materialNo )
{
	return GetMeshContena()->GetMesh( meshNo )->GetMaterialContena()->GetMaterial( materialNo );
}

void CModel::AttachDiffuseMap( const graphic::CTexturePtr &spTexture , u32 meshNo , u32 materialNo )
{
	GetMeshContena()->GetMesh( meshNo )->GetMaterialContena()->GetMaterial( materialNo )->AttachDiffuseMap( spTexture );
}

void CModel::SetDiffuse( const CColor &color , u32 meshNo , u32 materialNo )
{
	GetMeshContena()->GetMesh( meshNo )->GetMaterialContena()->GetMaterial( materialNo )->Diffuse = color;
}

void CModel::SetAlphaValue( f32 alpha , u32 meshNo , u32 materialNo )
{
	if ( math::CMathUtil::NearyEq( alpha , 1.0f , 0.00001f ) )
	{
		SetAlpha( skyFalse );
	}
	else
	{
		SetAlpha( skyTrue );
	}

	GetMeshContena()->GetMesh( meshNo )->GetMaterialContena()->GetMaterial( materialNo )->Alpha = alpha;
}

} } }