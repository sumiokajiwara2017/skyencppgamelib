#include "StdAfx.h"
#include "SkyGraphicLib/SceneGraph.h"

#include "SkyGraphicLib/Renderer.h"

SkyImplementRTTI( sky::lib::graphic::CSceneGraphNode , sky::lib::graphic::CEffector );

namespace sky { namespace lib { namespace graphic {

void CRenderingParam::AlphaDataRendering()
{
	//�Ȃ�������Ȃɂ����Ȃ�
	if ( AlphaDataList.GetNum() == 0 ) return;

	CRenderStatePtr spRenderState = CRenderStateCreate_();
	spRenderState->EnableTextureBlending();

	//�X�e�[�g��ݒ�
	CRenderCommand::SetRenderState( spRenderState );

	//�������̂��͎̂�O���烌���_�����O����鏈���������B
	FOREACH( CAlphaRenderingCommandPtrList , it , AlphaDataList )
	{
		( *it )->run();
	}

	AlphaDataList.DeleteAll();

	//�X�e�[�g��߂�
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
	//�C���f���g�����Z
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

		//�J�����O����Ă�����X�L�b�v
		if ( spSgNode->IsAppCulling() == skyTrue ) continue;

		//�[�x�o�b�t�@�N���A�ݒ�ɂȂ��Ă�����N���A
		if ( spSgNode->IsDepthBufferClear() == skyTrue )
		{
			CRenderCommand::ClearDepthBuffer( param.DepthStencilBufferStack.GetTop() );
		}

		//�X�e���V���o�b�t�@�N���A�ݒ�ɂȂ��Ă�����N���A
		if ( spSgNode->IsStencilBufferClear() == skyTrue )
		{
			CRenderCommand::ClearStencilBuffer( param.DepthStencilBufferStack.GetTop() );
		}

		//�W���C���g������΍s��X�^�b�N��Push����
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

		spSgNode->vRendering( param );        //�e
		spSgNode->RenderingChild( param );    //�q�B
		spSgNode->vPostRendering( param );    //�e

		//�W���C���g������΍s��X�^�b�N����Pop����
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

	//�C���f���g�����Z
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
		m_Branch->AttachBranch( spBranch ); //�ċA
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
		//�}�ɂ���Ɏ}���������炻�̎}�������ɏグ��
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
		m_Branch->DetachBranch( spBranch ); //�ċA
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

	//�}�̃f�o�b�O�v�����g
	if ( pNode->GetBranchRoot().IsEmpty() == skyFalse )
	{
		NodeTreePrintTraverseImple( pNode->GetBranchRoot().GetPtrNoConst() , indent + 1 , eRelationKind_Branch );
	}

	//�t�̃f�o�b�O�v�����g
	FOREACH( CSceneGraphNodePtrList , it , pNode->LeafList() )
	{
		NodeTreePrintTraverseImple( ( *it ).GetPtrNoConst() , indent + 1 , eRelationKind_Leaf );
	}

	//�q�̃f�o�b�O�v�����g
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

	//�}�̃f�o�b�O�v�����g
	if ( pNode->GetBranchRoot().IsEmpty() == skyFalse )
	{
		InfoPrintTraverseImple( pNode->GetBranchRoot().GetPtrNoConst() );
	}

	//�t�̃f�o�b�O�v�����g
	FOREACH( CSceneGraphNodePtrList , it , pNode->LeafList() )
	{
		InfoPrintTraverseImple( ( *it ).GetPtrNoConst() );
	}

	//�q�̃f�o�b�O�v�����g
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

	//�������O���Ȃ���΃f�t�H���g�̖��O��t����
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

	//���O�擾
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

	//��������
	if ( isSharing == skyTrue )
	{
		node = spNode->CloneSharing();
	}
	else
	{
		node = spNode->Clone();
	}

	//�}�𕡐�����
	if ( spNode->GetBranchRoot().IsEmpty() == skyFalse )
	{
		node->AttachBranch( CloneTraverseImple( spNode->GetBranchRoot() , isSharing ) );
	}

	//�t�𕡐�����
	FOREACH( CSceneGraphNodePtrList , it , spNode->LeafList() )
	{
		node->AttachLeaf( CloneTraverseImple( *it , isSharing ) );
	}

	//�q�𕡐�����
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
	//����������
	spNode->Activate();

	//�}������������
	if ( spNode->GetBranchRoot().IsEmpty() == skyFalse )
	{
		ActivateTraverseImple( spNode->GetBranchRoot() );
	}

	//�t������������
	FOREACH( CSceneGraphNodePtrList , it , spNode->LeafList() )
	{
		ActivateTraverseImple( *it );
	}

	//�q������������
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
