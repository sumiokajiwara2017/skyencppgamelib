#include "StdAfx.h"
#include "SkyGraphicLib/Scissor.h"

#include "SkyGraphicLib/Renderer.h"

//===================================�yCommon�z====================================================>

SkyImplementRTTI( sky::lib::graphic::CScissorRect , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

void CScissorRect::vRendering( CRenderingParam &param )
{
    SKY_ASSERT_MSG( this->GetRefCnt() > 0 , _T( "It failed in making the device context.  \n" ) );

	//�����������_�[�f�[�^�̕`��i�`���Ԃ𕪂���V�[���O���t�m�[�h�v�f���I���Ƃ��ɔ��������o�͂���j
	param.AlphaDataRendering();

	//Window�̕��ƍ������擾���Ă���
	m_WindowSize = ( ( CWindowPtr )param.Window )->GetRect().GetSize();

	//�������X�^�b�N�ɐς�
	param.ScissorRectStack.Push( thisPtr( CSceneGraphNodePtr ) ); //���Ƃ��Ƃ���this�����̃X�}�[�g�|�C���^�ɊǗ�����Ă����ꍇ�iRefCount��0�ȏ�̏ꍇ)���̎󂯓n�������Ă���肪�������Ȃ��B

	//�������V�U�����O��`��K�p����i�ꎞ�I�ɃV�U�����O��`��u��������j
	CRenderCommand::SetScissorRect( param.ScissorRectStack.GetTop() );
}

void CScissorRect::vPostRendering( CRenderingParam &param )
{
	//�����������_�[�f�[�^�̕`��i�`���Ԃ𕪂���V�[���O���t�m�[�h�v�f���I���Ƃ��ɔ��������o�͂���j
	param.AlphaDataRendering();

	//�X�^�b�N����߂�
	param.ScissorRectStack.Pop();

	if ( param.ScissorRectStack.GetNum() > 0 )
	{
		//�ꎞ�I�ɒu���������V�U�����O��`�����Ƃɖ߂�
		CRenderCommand::SetScissorRect( param.ScissorRectStack.GetTop() );
	}
	else
	{
		//�e���Ȃ���΃V�U�����O��`���N���A����
		CRenderCommand::ClearScissorRect( thisPtr( CSceneGraphNodePtr ) );
	}
}

void CScissorRect::SetRect( const math::CBasicRect &rect )
{
	m_Rect = rect;
}

void CScissorRect::ReSizeSc( dectype wSc , dectype hSc )
{
	m_Rect.X() *= wSc;
	m_Rect.Y() *= hSc;
	m_Rect.W() *= wSc;
	m_Rect.H() *= hSc;
}

#define GETSET_BIN( isWrite ) \
	stream->vWriteReadStream( ( u8 * )&m_Rect.X() , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Rect.Y() , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Rect.W() , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_Rect.H() , sizeof( d64 ) , isWrite );

skyBool CScissorRect::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "scissorrect bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN( skyTrue );

	return result;
}

skyBool CScissorRect::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "scissorrect bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN( skyFalse );

	return result;
}

const serialize::CSerializeInterfacePtr& CScissorRect::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CScissorRectPtr )spContainer )->m_Rect = m_Rect;
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CScissorRect::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CScissorRect::Clone()
{
	return Copy( CScissorRectPtr( NEW__( CScissorRect , CScissorRect() ) ) );
}

serialize::CSerializeInterfacePtr CScissorRect::CloneSharing()
{
	return Clone();
}

#ifdef SW_SKYLIB_DEBUG_ON

void CScissorRect::NodeTreePrint( u32 indent , eRelationKind eKind )
{
	SKY_UNDER_CONSTRUCTION( eKind );

	SKY_PRINT_INDENT( indent * 2 );
	SKY_PRINTF( "%s " , GetRTTI()->GetName() );
	SKY_PRINTF( _T( "name=[ %s ] rect[ %4.3f , %4.3f , %4.3f , %4.3f ] \n" ) , Name.GetString() , m_Rect.X() , m_Rect.Y() , m_Rect.W() , m_Rect.H() );
}

#endif

//===================================�yDirectX�z===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

void CScissorRect::SetRenderer()
{
	D3D11_RECT d3dRect[ scissor::SCISSOR_MAX_NUM ];
	d3dRect[ 0 ].left   = ( LONG )m_Rect.X();
	d3dRect[ 0 ].top    = ( LONG )m_Rect.Y();
	d3dRect[ 0 ].bottom = ( LONG )m_Rect.Y() + ( LONG )m_Rect.H();
	d3dRect[ 0 ].right  = ( LONG )m_Rect.X() + ( LONG )m_Rect.W();
	CDevice_->GetMember()->m_spD3DDeviceContext->RSSetScissorRects( 1 , d3dRect );
}

void CScissorRect::ClearRenderer()
{
	CDevice_->GetMember()->m_spD3DDeviceContext->RSSetScissorRects( 0 , NULL );
}

#endif

//===================================�yOpenGL�z====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

void CScissorRect::SetRenderer()
{
    glEnable( GL_SCISSOR_TEST );

    //���W�͍��㌴�_�œn���Ă���̂ō������_�ɕύX����
	dectype openGLPosY = m_WindowSize.H - m_Rect.Y() - m_Rect.H();
	dectype W = ( m_Rect.W() < 1 ) ? 1 : m_Rect.W();
	dectype H = ( m_Rect.H() < 1 ) ? 1 : m_Rect.H();

	glScissor( ( GLsizei )m_Rect.X() , 
			    ( GLsizei )openGLPosY , 
				( GLint )( W ) , 
				( GLint )( H ) );
}

void CScissorRect::ClearRenderer()
{
    glDisable( GL_SCISSOR_TEST );
}

#endif

} } }
