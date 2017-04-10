#include "StdAfx.h"
#include "SkyGraphicLib/WindowSystem.h"

namespace sky { namespace lib { namespace graphic {

SingletonPtr< CWindowSystemManager >	CWindowSystemManager::SingletonMember;

CWindowSystemNodePtr CWindowSystemManager::CreateWindowNode()
{
	CWindowSystemNodePtr spNode = CWindowSystemNodePtr( NEW CWindowSystemNode() );
	m_SceneGraphAttachNode->AttachChild( spNode );
	return spNode;
}

void CWindowSystemManager::DestroyWindowNode( const CWindowSystemNodePtr &spNode )
{
	m_SceneGraphAttachNode->DetachChild( spNode );
}

CWindowSystemManager::CWindowSystemManager()
{
	//マウスのイベント登録
	m_spMouseEventDelegate.SetPtr( NEW__( delegate::CEventDelegateAdaptor< CWindowSystemManager > , delegate::CEventDelegateAdaptor< CWindowSystemManager >( this , &CWindowSystemManager::MouseEventDelegate ) ) );
	CMouse_->AttachInputListener( m_spMouseEventDelegate );

	//キーボードイベント登録
	m_spKeyBoradEventDelegate.SetPtr( NEW__( delegate::CEventDelegateAdaptor< CWindowSystemManager > , delegate::CEventDelegateAdaptor< CWindowSystemManager >( this , &CWindowSystemManager::KeyBoradEventDelegate ) ) );
	Ckeyboard_->AttachInputListener( m_spKeyBoradEventDelegate );

	//シーングラフルート作成
	m_SceneGraphAttachNode = CSceneGraphNode::Create();
}

void CWindowSystemManager::AttachWindowGroupRootNode( const CWindowSystemNodePtr &spNode )
{
	m_WindowGroupRootNodeList.AddTail( spNode );
}

void CWindowSystemManager::DetachWindowGroupRootNode( const CWindowSystemNodePtr &spNode )
{
	m_WindowGroupRootNodeList.Delete( spNode );
}

void CWindowSystemManager::ToggleSelectWindowGroup()
{
}

void CWindowSystemManager::MouseEventDelegate( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args )
{
}

void CWindowSystemManager::KeyBoradEventDelegate( const CommonSmartPointer &spSender , const delegate::CDelegateArgs &args )
{
}

} } }