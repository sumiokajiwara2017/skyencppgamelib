#include "StdAfx.h"
#include "SkyGraphicLib/ViewPort.h"

#include "SkyGraphicLib/Renderer.h"

SkyImplementRTTI( sky::lib::graphic::CViewPort , sky::lib::graphic::CSceneGraphNode );

namespace sky { namespace lib { namespace graphic {

//===================================�yCommon�z====================================================>

void CViewPort::vRendering( CRenderingParam &param )
{
    SKY_ASSERT_MSG( this->GetRefCnt() > 0 , _T( "This Format is invalid.\n" ) );

	//�����������_�[�f�[�^�̕`��i�`���Ԃ𕪂���V�[���O���t�m�[�h�v�f���I���Ƃ��ɔ��������o�͂���j
	param.AlphaDataRendering();

	//�������X�^�b�N�ɐς�
	param.ViewPortStack.Push( thisPtr( CViewPortPtr ) ); //���Ƃ��Ƃ���this�����̃X�}�[�g�|�C���^�ɊǗ�����Ă����ꍇ�iRefCount��0�ȏ�̏ꍇ)���̎󂯓n�������Ă���肪�������Ȃ��B

	//�������r���[�|�[�g��K�p����i�ꎞ�I�Ƀr���[�|�[�g��u��������j
	CRenderCommand::SetViewPort( param.ViewPortStack.GetTop() );
}

void CViewPort::vPostRendering( CRenderingParam &param )
{
	//�����������_�[�f�[�^�̕`��i�`���Ԃ𕪂���V�[���O���t�m�[�h�v�f���I���Ƃ��ɔ��������o�͂���j
	param.AlphaDataRendering();

	//�X�^�b�N����߂�
	param.ViewPortStack.Pop();

	if ( param.ViewPortStack.GetNum() > 0 )
	{
		//�ꎞ�I�ɒu���������r���[�|�[�g�����Ƃɖ߂�
		CRenderCommand::SetViewPort( param.ViewPortStack.GetTop() );
	}
}

void CViewPort::SetRect( const math::CBasicRect &rect )
{
	m_Rect = rect;
}

const math::CBasicRect&  CViewPort::GetRect()
{
	return m_Rect;
}

void CViewPort::SetDepth( dectype minDepth , dectype maxDepth )
{
	m_MinDepth = minDepth;
	m_MaxDepth = maxDepth;
}

void CViewPort::ReSize( u32 width , u32 height )
{
	m_Rect.W() = ( dectype )width;
	m_Rect.H() = ( dectype )height;
}

void CViewPort::ReSizeSc( dectype wSc , dectype hSc )
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
	stream->vWriteReadStream( ( u8 * )&m_Rect.H() , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_MinDepth , sizeof( d64 ) , isWrite ); \
	stream->vWriteReadStream( ( u8 * )&m_MaxDepth , sizeof( d64 ) , isWrite );

skyBool CViewPort::SaveBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "viewport bin save. \n" ) );

	skyBool result = this->CSceneGraphNode::SaveBinary( stream );
	GETSET_BIN( skyTrue );

	return result;
}

skyBool CViewPort::LoadBinary( const stream::IStreamPtr& stream )
{
	SKY_PRINTF( _T( "viewport bin load. \n" ) );

	skyBool result = this->CSceneGraphNode::LoadBinary( stream );
	GETSET_BIN( skyFalse );

	return result;
}

const serialize::CSerializeInterfacePtr& CViewPort::Copy( const serialize::CSerializeInterfacePtr& spContainer )
{
	this->CSceneGraphNode::Copy( spContainer );
	( ( CViewPortPtr )spContainer )->m_Rect = m_Rect;
	( ( CViewPortPtr )spContainer )->m_MinDepth = m_MinDepth;
	( ( CViewPortPtr )spContainer )->m_MaxDepth = m_MaxDepth;
	return spContainer;
}

const serialize::CSerializeInterfacePtr& CViewPort::CopySharing( const serialize::CSerializeInterfacePtr& spContainer )
{
	return Copy( spContainer );
}

serialize::CSerializeInterfacePtr CViewPort::Clone()
{
	return Copy( CViewPortPtr( NEW__( CViewPort , CViewPort() ) ) );
}

serialize::CSerializeInterfacePtr CViewPort::CloneSharing()
{
	return Clone();
}

//===================================�yDirectX�z===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11


void CViewPort::SetRenderer()
{
	D3D11_VIEWPORT d3dRect;
	d3dRect.TopLeftX = ( FLOAT )m_Rect.X();
	d3dRect.TopLeftY = ( FLOAT )m_Rect.Y();
	d3dRect.Width    = ( FLOAT )m_Rect.W();
	d3dRect.Height   = ( FLOAT )m_Rect.H();
	d3dRect.MinDepth = ( FLOAT )m_MinDepth;
	d3dRect.MaxDepth = ( FLOAT )m_MaxDepth;

    CDevice_->GetMember()->m_spD3DDeviceContext->RSSetViewports( 1 , &d3dRect );
}

#endif

//===================================�yOpenGL�z====================================================>

#ifdef SKYLIB_RENDER_OPENGL4


void CViewPort::SetRenderer()
{
	//�r���[�|�[�g
	glViewport( ( GLsizei )m_Rect.X() , ( GLsizei )m_Rect.Y() , ( GLint )m_Rect.W() , ( GLint )m_Rect.H() );
}

#endif

} } }
