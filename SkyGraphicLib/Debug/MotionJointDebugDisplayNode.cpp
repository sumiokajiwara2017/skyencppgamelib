#include "StdAfx.h"
#include "SkyGraphicLib/Debug.h"

#ifdef SW_SKYLIB_DEBUG_ON

#include "SkyGraphicLib/Model.h"

SkyImplementRTTI( sky::lib::graphic::CMotionJointDebugDisplayNode , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

void CMotionJointDebugDisplayNode::SetTreeModelFromJointTree( const math::CMotionController &pMotionController , const CSceneGraphNodePtr &spModelTree )
{
	s32 jointPalletNo = pMotionController.m_spSkeleton->m_spJointPallet->GetJoint( spModelTree->Name.GetName() )->Index;

	spModelTree->SetLocalMatrix( pMotionController.m_DebugJointTransforms[ jointPalletNo ] );

	FOREACH( math::CJointPtrList , it , spModelTree->ChildList() )
	{
		SetTreeModelFromJointTree( pMotionController , *it );
	}
}

void   CMotionJointDebugDisplayNode::vRendering( CRenderingParam &param )
{
	SKY_UNUSED_ARG( param );

	//ボーンの位置をモーションコントローラーを頼りに決定する
	SetTreeModelFromJointTree( m_pMotionController , m_spModelTreeRoot );
}

void CMotionJointDebugDisplayNode::SetBaseScale( const math::CBasicVector3 &scale )
{
	m_BaseScale = scale;
}

void CMotionJointDebugDisplayNode::SetBaseRot( math::eAxis axis , dectype theta )
{
	m_BaseRot.Setup( axis , theta );
}

void CMotionJointDebugDisplayNode::CreateTreeModelFromJointTree( const math::CJointPtr &spJoint , const CSceneGraphNodePtr &spModelTree , const math::CBasicVector3 &basicScale , const math::CRotationMatrix &rot )
{
	math::CMatrix4x3 scale; scale.Identity();
	scale.SetupScale( basicScale );

	spModelTree->Name.SetName( spJoint->Name.GetName() );
	spModelTree->SetComputeLocalMatrixOn( skyFalse );

	FOREACH( math::CJointPtrList , it , spJoint->ChildList() )
	{
		CModelPtr spChildModel = graphic::CModelUtil::CreateLinePyramid( scale * rot );
		spModelTree->AttachChild( spChildModel );
		CreateTreeModelFromJointTree( *it , spChildModel , basicScale , rot );
	}
}

void CMotionJointDebugDisplayNode::SetDrawOn( skyBool isOn )
{
	if ( m_IsDrawOn != isOn )
	{
		m_IsDrawOn = isOn;

		if ( isOn == skyTrue )
		{//モデルを作る
			math::CMatrix4x3 scale; scale.Identity();
			scale.SetupScale( m_BaseScale );
			m_spModelTreeRoot = graphic::CModelUtil::CreateLinePyramid( scale * m_BaseRot );
			CreateTreeModelFromJointTree( m_pMotionController.m_spSkeleton->m_spJointRoot , m_spModelTreeRoot , m_BaseScale , m_BaseRot );
			CRenderStatePtr spRenderState = CRenderStateCreate_();
			spRenderState->AttachChild( m_spModelTreeRoot );

			//深度バッファをオフにする
			spRenderState->DisableDepthBuffer();

			//子に登録
			AttachChild( spRenderState );
		}
		else
		{//モデルを消す

			//子から削除
			ChildList().DeleteAll();

			m_spModelTreeRoot.Delete();
		}
	}
}

CMotionJointDebugDisplayNode::CMotionJointDebugDisplayNode( const math::CMotionController &pMotionController ) : 
m_pMotionController( pMotionController ) , 
m_IsDrawOn( skyFalse ) ,
m_BaseScale( 5 , 20 , 5 )
{
	m_BaseRot.Setup( math::eAxis_z , math::CMathUtil::DegToRad( 90 ) );
}

} } }

#endif