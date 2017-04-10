#include "StdAfx.h"
#include "SkyGraphicLib/WindowSystem.h"

namespace sky { namespace lib { namespace graphic {

void CWindowSystemNode::vRendering	( CRenderingParam &param )
{
	//座標を設定する

	//スプライトとして機能する
	this->CSprite::vRendering( param );
}

void CWindowSystemNode::SetUpFontDispMode( const skyString *headerfont )
{
}

void CWindowSystemNode::SetFont( const skyString *font )
{
}

void CWindowSystemNode::BindFontAddress( const skyString *font )
{
}

void CWindowSystemNode::SetUpf32DispMode( const skyString *headerfont , f32 initValue )
{
}

void CWindowSystemNode::Setf32Value( f32 value )
{
}

void CWindowSystemNode::Bindf32Address( f32 *pValue )
{
}

void CWindowSystemNode::SetUps32DispMode( const skyString *headerfont , s32 initValue )
{
}

void CWindowSystemNode::Sets32Value( s32 value )
{
}

void CWindowSystemNode::Binds32Address( s32 *pValue )
{
}

void CWindowSystemNode::AttachChild( const CWindowSystemNodePtr &spNode )
{
	m_ChildList.AddTail( spNode );
}

void CWindowSystemNode::DetachChild( const CWindowSystemNodePtr &spNode )
{
	m_ChildList.Delete( spNode );
}

void CWindowSystemNode::AttachBeforeBrotherNodeList( const CWindowSystemNodePtr &spNode )
{
}

void CWindowSystemNode::AttachAfterBrotherNodeList( const CWindowSystemNodePtr &spNode )
{

}

void CWindowSystemNode::DetachBrotherNodeList()
{
}

CWindowSystemNode::CWindowSystemNode( dectype posX , dectype posY , dectype width , dectype height )
{
	//位置とサイズを設定
	m_UniqId = WINDOW_SYSTEM_INVALID_NODE_ID;
	m_Pos.X = posX;
	m_Pos.Y = posY;
	m_Size.W = width;
	m_Size.H = height;
}

} } }