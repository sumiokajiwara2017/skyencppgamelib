#include "StdAfx.h"
#include "SkyGraphicLib/SceneGraph.h"

#include "SkyGraphicLib/Renderer.h"

SkyImplementRTTI( sky::lib::graphic::CSceneGraphNode , sky::lib::graphic::CEffector );

namespace sky { namespace lib { namespace graphic {

void CRenderingParam::AlphaDataRendering()
{
	//なかったらなにもしない
	if ( AlphaDataList.GetNum() == 0 ) return;

	CRenderStatePtr spRenderState = CRenderStateCreate_();
	spRenderState->EnableTextureBlending();

	//ステートを設定
	CRenderCommand::SetRenderState( spRenderState );

	//半透明のものは手前からレンダリングされる処理を書く。
	FOREACH( CAlphaRenderingCommandPtrList , it , AlphaDataList )
	{
		( *it )->run();
	}

	AlphaDataList.DeleteAll();

	//ステートを戻す
	CRenderCommand::ReverseRenderState( spRenderState );
}

void CSceneGraphNode::SetPrio( dectype prio )
{
	SetPosition_Z( CONVERT_PRIO_TO_Z( prio ) );
}

CSceneGraphNode::CSceneGraphNode( const CSceneGraphNodePtr &spParent ) : CEffector( spParent )
{
	m_IsCulling             = skyFalse;
	m_IsAlpha               = skyFalse;
	m_IsDepthBufferClear    = skyFalse;
	m_IsStencilBufferClear  = skyFalse;
	m_IsLockedRotAndScale   = skyFalse;
	m_IsJointUse              = skyFalse;
	m_IsSetUp               = skyFalse;
	m_IsLockedJoint          = skyFalse;
#ifdef SW_SKYLIB_DEBUG_ON
	m_IsNodeTreePrintOn     = skyFalse;
#endif
	ID = 0;
}

void CSceneGraphNode::RenderingChild( CRenderingParam &param )
{
	//インデントを加算
	param.Indent++;

#ifdef SW_SKYLIB_DEBUG_ON

	if ( m_IsNodeTreePrintOn == skyTrue )
	{
		param.IsNodeTreePrint = skyTrue;
		SKY_PRINTF( "<================= Debug Start ==================> \n" );
	}

	if ( param.IsNodeTreePrint == skyTrue )
	{
		NodeTreePrint( param.Indent );
	}

#endif

	FOREACH( math::CJointPtrList , it , m_ChildList )
	{

		CSceneGraphNodePtr spSgNode = ( *it );

		//カリングされていたらスキップ
		if ( spSgNode->IsAppCulling() == skyTrue ) continue;

		//深度バッファクリア設定になっていたらクリア
		if ( spSgNode->IsDepthBufferClear() == skyTrue )
		{
			CRenderCommand::ClearDepthBuffer( param.DepthStencilBufferStack.GetTop() );
		}

		//ステンシルバッファクリア設定になっていたらクリア
		if ( spSgNode->IsStencilBufferClear() == skyTrue )
		{
			CRenderCommand::ClearStencilBuffer( param.DepthStencilBufferStack.GetTop() );
		}

		//ジョイントがあれば行列スタックにPushする
		if ( spSgNode->IsJointUse() == skyTrue )
		{
	        param.MatrixStack.Push();

			if ( m_IsLockedJoint == skyTrue )
			{
				param.MatrixStack.LoadMatrix( spSgNode->ComputeLocalMatrix() ); 
			}
			else if( spSgNode->m_IsLockedRotAndScale == skyTrue )
			{
				param.MatrixStack.LoadAndMultLocalBeforeMatrixTransOnly( spSgNode->ComputeLocalMatrix() );
			}
			else
			{
				param.MatrixStack.LoadAndMultLocalBeforeMatrix( spSgNode->ComputeLocalMatrix() );
			}
		}

		spSgNode->vRendering( param );        //親
		spSgNode->RenderingChild( param );    //子達
		spSgNode->vPostRendering( param );    //親

		//ジョイントがあれば行列スタックからPopする
		if ( spSgNode->IsJointUse() == skyTrue )
		{
			param.MatrixStack.Pop();
		}
	}

#ifdef SW_SKYLIB_DEBUG_ON

	if ( m_IsNodeTreePrintOn == skyTrue )
	{
		param.IsNodeTreePrint = skyFalse;
		m_IsNodeTreePrintOn = skyFalse;
		SKY_PRINTF( "<================= Debug End ==================> \n" );
	}

#endif

	//インデントを減算
	param.Indent--;
}

CSceneGraphNodePtr CSceneGraphNode::AttachLeaf( u32 key, const CSceneGraphNodePtr &spLeaf )
{
	m_LeafList.AddTail( key, spLeaf );
	spLeaf->AttachParent( thisPtr( CSceneGraphNodePtr ) );
	return spLeaf;
}

CSceneGraphNodePtr CSceneGraphNode::AttachLeaf( const CSceneGraphNodePtr &spLeaf )
{
	m_LeafList.AddTail( spLeaf );
	spLeaf->AttachParent( thisPtr( CSceneGraphNodePtr ) );
	return spLeaf;
}

CSceneGraphNodePtr CSceneGraphNode::AttachLeafFront( const CSceneGraphNodePtr &spLeaf )
{
	m_LeafList.AddFront( spLeaf );
	spLeaf->AttachParent( thisPtr( CSceneGraphNodePtr ) );
	return spLeaf;
}

void CSceneGraphNode::DetachLeaf( const CSceneGraphNodePtr &spLeaf )
{
	m_LeafList.Delete( spLeaf );
	spLeaf->DetachParent();
}

void CSceneGraphNode::DetachAllLeaf()
{
	FOREACH( CSceneGraphNodePtrList , it , m_LeafList )
	{
		DetachLeaf( ( *it ) );
	}
}

void CSceneGraphNode::AttachBranch( const CSceneGraphNodePtr &spBranch )
{
	if ( m_Branch.IsEmpty() == skyTrue )
	{
		m_Branch = spBranch;
		m_Branch->AttachParent( thisPtr( CSceneGraphNodePtr ) );
	}
	else
	{
		m_Branch->AttachBranch( spBranch ); //再帰
	}
}


void CSceneGraphNode::DetachBranch()
{
	m_Branch.Delete();
}

void CSceneGraphNode::DetachBranch( const CSceneGraphNodePtr &spBranch )
{
	if ( m_Branch.IsEmpty() == skyTrue ) return;

	if ( m_Branch == spBranch )
	{
		//枝にさらに枝があったらその枝を一歩上に上げる
		if ( m_Branch->GetBranchRoot().IsEmpty() == skyFalse )
		{
			m_Branch = m_Branch->GetBranchRoot();
		}
		else
		{
			m_Branch.Delete();
		}
	}
	else
	{
		m_Branch->DetachBranch( spBranch ); //再帰
	}
}

const CSceneGraphNodePtr &CSceneGraphNode::GetBranchRoot()
{
	return m_Branch;
}

#ifdef SW_SKYLIB_DEBUG_ON

void CSceneGraphNode::NodeTreePrint( u32 indent , eRelationKind eKind )
{
	SKY_PRINT_INDENT( indent * 2 );
	if ( eKind == eRelationKind_Branch )
	{
		SKY_PRINTF( "branch=>" );
	}
	else if ( eKind == eRelationKind_Leaf )
	{
		SKY_PRINTF( "leaf=>" );
	}
	else if ( eKind == eRelationKind_Child )
	{
		SKY_PRINTF( "Child=>" );
	}
	SKY_PRINTF( "%s " , GetRTTI()->GetName() );
	SKY_PRINTF( _T( "name=[ %s , %p ] \n" ) , Name.GetString() , this );
}

void CSceneGraphNode::NodeTreePrintTraverse( u32 indent )
{
	NodeTreePrintTraverseImple( this , indent , eRelationKind_Root );
}

void CSceneGraphNode::NodeTreePrintTraverseImple( CSceneGraphNode *pNode , u32 indent , eRelationKind eKind )
{
	pNode->NodeTreePrint( indent , eKind );

	//枝のデバッグプリント
	if ( pNode->GetBranchRoot().IsEmpty() == skyFalse )
	{
		NodeTreePrintTraverseImple( pNode->GetBranchRoot().GetPtrNoConst() , indent + 1 , eRelationKind_Branch );
	}

	//葉のデバッグプリント
	FOREACH( CSceneGraphNodePtrList , it , pNode->LeafList() )
	{
		NodeTreePrintTraverseImple( ( *it ).GetPtrNoConst() , indent + 1 , eRelationKind_Leaf );
	}

	//子のデバッグプリント
	FOREACH( math::CJointPtrList , it , pNode->ChildList() )
	{
		NodeTreePrintTraverseImple( SkyDynamicCast( CSceneGraphNode , ( *it ).GetPtrNoConst() ) , indent + 1 , eRelationKind_Child );
	}
}

void CSceneGraphNode::InfoPrint()
{
	NodeTreePrint( 0 );
}

void CSceneGraphNode::InfoPrintTraverse()
{
	InfoPrintTraverseImple( this );
}

void CSceneGraphNode::InfoPrintTraverseImple( CSceneGraphNode *pNode )
{
	pNode->InfoPrint();

	//枝のデバッグプリント
	if ( pNode->GetBranchRoot().IsEmpty() == skyFalse )
	{
		InfoPrintTraverseImple( pNode->GetBranchRoot().GetPtrNoConst() );
	}

	//葉のデバッグプリント
	FOREACH( CSceneGraphNodePtrList , it , pNode->LeafList() )
	{
		InfoPrintTraverseImple( ( *it ).GetPtrNoConst() );
	}

	//子のデバッグプリント
	FOREACH( math::CJointPtrList , it , pNode->ChildList() )
	{
		InfoPrintTraverseImple( SkyDynamicCast( CSceneGraphNode , ( *it ).GetPtrNoConst() ) );
	}
}


#endif

#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&namesize , sizeof( u32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )name , namesize , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&ID , sizeof( s32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_IsCulling , sizeof( s32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_IsDepthBufferClear , sizeof( s32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_IsStencilBufferClear , sizeof( s32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_IsAlpha , sizeof( s32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_IsLockedRotAndScale , sizeof( s32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_IsJointUse , sizeof( s32 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_IsLockedJoint , sizeof( s32 ) , isWrite ); \

skyBool CSceneGraphNode::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "scenegraphnode bin save. \n" ) );

	//もし名前がなければデフォルトの名前を付ける
	if ( Name.GetName().IsEmpty() == skyTrue )
	{
		Name.SetName( CHS( _T( "none" ) ) );
	}

	skyBool result = skyTrue;

	u32              namesize = Name.GetNameSize();
	const skyString* name     = Name.GetName()->GetString();

	SKY_PRINTF( _T( "%d name = %s \n" ) , namesize , Name.GetName()->GetString() );

	GETSET_BIN( skyTrue );

	return result;
}
			
skyBool CSceneGraphNode::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "scenegraphnode bin load. \n" ) );

	skyBool result = skyTrue;

	u32 namesize;
	skyString name[ PATH_LEN_MAX ] = _T( "none" );
	GETSET_BIN( skyFalse );

	//名前取得
	Name.SetName( CHS( name ) );

	return result;
}

const serialize::CSerializeInterfacePtr &CSceneGraphNode::Copy( const serialize::CSerializeInterfacePtr &spContainer )
{
	( ( CSceneGraphNodePtr )spContainer)->Name = Name;
	( ( CSceneGraphNodePtr )spContainer)->ID = ID;
	( ( CSceneGraphNodePtr )spContainer)->m_IsCulling = m_IsCulling;
    ( ( CSceneGraphNodePtr )spContainer)->m_IsDepthBufferClear = m_IsDepthBufferClear;
    ( ( CSceneGraphNodePtr )spContainer)->m_IsStencilBufferClear = m_IsStencilBufferClear;
	( ( CSceneGraphNodePtr )spContainer)->m_IsAlpha = m_IsAlpha;
    ( ( CSceneGraphNodePtr )spContainer)->m_IsLockedRotAndScale = m_IsLockedRotAndScale;
	( ( CSceneGraphNodePtr )spContainer)->m_IsLockedJoint = m_IsLockedJoint;
return spContainer;
}

const serialize::CSerializeInterfacePtr &CSceneGraphNode::CopySharing( const serialize::CSerializeInterfacePtr &spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CSceneGraphNode::Clone()
{
	return Copy( CSceneGraphNodePtr( NEW__( CSceneGraphNode , CSceneGraphNode() ) ) );
}

serialize::CSerializeInterfacePtr CSceneGraphNode::CloneSharing()
{
	return Clone();
}

serialize::CSerializeInterfacePtr CSceneGraphNode::CloneTraverse()
{
	return CloneTraverseImple( thisPtr( CSceneGraphNodePtr ) , skyFalse );
}

serialize::CSerializeInterfacePtr CSceneGraphNode::CloneSharingTraverse()
{
	return CloneTraverseImple( thisPtr( CSceneGraphNodePtr ) , skyTrue );
}


CSceneGraphNodePtr CSceneGraphNode::CloneTraverseImple( const CSceneGraphNodePtr &spNode , skyBool isSharing  )
{
	CSceneGraphNodePtr node;

	//複製する
	if ( isSharing == skyTrue )
	{
		node = spNode->CloneSharing();
	}
	else
	{
		node = spNode->Clone();
	}

	//枝を複製する
	if ( spNode->GetBranchRoot().IsEmpty() == skyFalse )
	{
		node->AttachBranch( CloneTraverseImple( spNode->GetBranchRoot() , isSharing ) );
	}

	//葉を複製する
	FOREACH( CSceneGraphNodePtrList , it , spNode->LeafList() )
	{
		node->AttachLeaf( CloneTraverseImple( *it , isSharing ) );
	}

	//子を複製する
	FOREACH( math::CJointPtrList , it , spNode->ChildList() )
	{
		node->AttachChild( CloneTraverseImple(  *it , isSharing ) );
	}

	return node;
}

void CSceneGraphNode::ActivateTraverse()
{
	ActivateTraverseImple( thisPtr( CSceneGraphNodePtr ) );
}

void CSceneGraphNode::ActivateTraverseImple( const CSceneGraphNodePtr &spNode )
{
	//活性化する
	spNode->Activate();

	//枝を活性化する
	if ( spNode->GetBranchRoot().IsEmpty() == skyFalse )
	{
		ActivateTraverseImple( spNode->GetBranchRoot() );
	}

	//葉を活性化する
	FOREACH( CSceneGraphNodePtrList , it , spNode->LeafList() )
	{
		ActivateTraverseImple( *it );
	}

	//子を活性化する
	FOREACH( math::CJointPtrList , it , spNode->ChildList() )
	{
		ActivateTraverseImple( *it );
	}
}

CSceneGraphNodePtrList &CSceneGraphNode::LeafList()
{
	return m_LeafList;
}

void CSceneGraphNode::SetJointUse( skyBool isUse )
{
	m_IsJointUse = isUse;
}

skyBool CSceneGraphNode::IsJointUse()
{
	return m_IsJointUse;
}

} } }
