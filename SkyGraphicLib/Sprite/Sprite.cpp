#include "StdAfx.h"
#include "SkyGraphicLib/Sprite.h"

#include "SkyGraphicLib/Renderer.h"

SkyImplementRTTI( sky::lib::graphic::CSprite , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

//-------------------------------------< ���� >---------------------------------------------//

void CSprite::RenderingImple( CRenderingParam &param , const CShaderObjectPtr &shader )
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

void CSprite::vRendering( CRenderingParam &param )
{
	SKY_ASSERT_MSG( m_IsSetUp == skyTrue ,  _T( "It has not setup yet." ) );

	//�������b�V�����Ȃ���΋�X�v���C�g�Ɣ��f���ĕ`���������
	if ( GetMeshContena( skyFalse ).IsEmpty() == skyFalse )
	{
		//���b�V����vRendering���Ăԁi��{Leaf,Branch��vRendering�͌Ă΂�Ȃ��B�ĂԕK�v���������P�[�X��Child��vRendering�̎������ŌĂԂ��Ɓj
		GetMeshContena()->vRendering( param );

		//�������㌴�_�������ꍇ�́A�n���ꂽ���W�����㌴�_���W���Ɖ��肵��Y�ړ������𔽓]����
		CameraPtr spCamera = param.CameraStack.GetTop();
		if ( spCamera->IsLeftTopCenter() && param.MatrixStack.GetTop().Ty > 0 )
		{
			param.MatrixStack.GetTop().Ty *= -1;
		}

		//�V�F�[�_�[�X�^�b�N�ɐς܂�Ă��邩�H
		if ( param.ShaderStack.GetNum() > 0 )
		{//�X�^�b�N�̃V�F�[�_�[�̂ݕ`��

			//�X�^�b�N�ɐς܂ꂽ�V�F�[�_�[�̕`��
			FOREACH( CSceneGraphNodePtrStack , it , param.ShaderStack )
			{
				RenderingImple( param , *it );
			}
		}
		else
		{//�A�^�b�`�V�F�[�_�[�̂ݕ`��

			//�A�^�b�`���ꂽ�V�F�[�_�[�ɂ��`��
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

void CSprite::ClearAndAttachShader( const CShaderObjectPtr &effectPtr )
{
	DetachShader();
	AttachLeaf( effectPtr );
}

void CSprite::AttachMeshContena( const CMeshContenaPtr &spMesh )
{
	AttachBranch( spMesh );
}

CMeshContenaPtr CSprite::GetMeshContena( skyBool isRequired )
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

void CSprite::AttachShader( const CShaderObjectPtr &effectPtr )
{
	AttachLeaf( effectPtr );
}

void CSprite::DetachShader()
{
	SceneGraphNodeUtil::DetachLeafAll< CShaderObject >( thisPtr( CSceneGraphNodePtr ) );
}

#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&m_IsSetUp , sizeof( s32 ) , isWrite );

skyBool CSprite::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "model bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN( skyTrue );

	return result;
}

skyBool CSprite::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "model bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN( skyFalse );

	return result;
}

const serialize::CSerializeInterfacePtr &CSprite::Copy( const serialize::CSerializeInterfacePtr &spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CSpritePtr )spContainer )->m_IsSetUp = m_IsSetUp;
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CSprite::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CSprite::Clone()
{
	return Copy( CSpritePtr( NEW__( CSprite , CSprite() ) ) );
}

serialize::CSerializeInterfacePtr CSprite::CloneSharing()
{
	return Clone();
}

#ifdef SW_SKYLIB_DEBUG_ON

void CSprite::NodeTreePrint( u32 indent , eRelationKind eKind )
{
	SKY_UNUSED_ARG( eKind );

	SKY_PRINT_INDENT( indent * 2 );
	SKY_PRINTF( "%s " , GetRTTI()->GetName() );
}

#endif

CSprite::CSprite() : CSceneGraphNode()
{
}

CSprite::CSprite( CSceneGraphNode &spNode ) : CSceneGraphNode( spNode )
{ 
}

void CSprite::SetUp()
{

	if ( m_IsSetUp == skyFalse )
	{
		//�W���C���g�̓f�t�H���g�Ŏg��
		SetJointUse();
		m_IsSetUp = skyTrue;
	}
}

CMaterialPtr CSprite::GetMaterial( u32 meshNo , u32 materialNo )
{
	return GetMeshContena()->GetMesh( meshNo )->GetMaterialContena()->GetMaterial( materialNo );
}

void CSprite::AttachDiffuseMap( const graphic::CTexturePtr &spTexture , u32 meshNo , u32 materialNo )
{
	GetMeshContena()->GetMesh( meshNo )->GetMaterialContena()->GetMaterial( materialNo )->AttachDiffuseMap( spTexture );
}

void CSprite::SetDiffuse( const CColor &color , u32 meshNo , u32 materialNo )
{
	GetMeshContena()->GetMesh( meshNo )->GetMaterialContena()->GetMaterial( materialNo )->Diffuse = color;
}

void CSprite::SetAlphaValue( f32 alpha , u32 meshNo , u32 materialNo )
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
