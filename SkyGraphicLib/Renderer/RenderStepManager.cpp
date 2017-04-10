#include "StdAfx.h"
#include "SkyGraphicLib/Renderer.h"

namespace sky { namespace lib { namespace graphic {

SingletonPtr< CRenderStepManager >	CRenderStepManager::SingletonMember;

void CRenderStepManager::AttachStep( RenderStep_Prio prio , CRenderStepPtr &spStep )
{
	m_kStepList.AddTail( prio, spStep );
	m_kStepHash.Set( spStep->Name.GetName() , spStep );

	spStep->m_RenderStepManager = thisPtr( CRenderStepManagerPtr );
}
CRenderStepPtr CRenderStepManager::Get( const hash::CHashStringPtr &spHash )
{
	return m_kStepHash[ spHash ];
}

void CRenderStepManager::AttachScene( dectype prio , const CScenePtr &spScene )
{
	m_kSceneList.AddTail( prio , spScene );
}

void CRenderStepManager::AttachFrontScene( const CScenePtr &spScene )
{
	m_kSceneList.AddFront( spScene );
}

void CRenderStepManager::AttachTailScene( const CScenePtr &spScene )
{
	m_kSceneList.AddTail( spScene );
}

void CRenderStepManager::DetachScene( const CScenePtr &spScene )
{
	m_kSceneList.Delete( spScene );
}

CScenePtrList &CRenderStepManager::GetSceneList()
{
	return m_kSceneList;
}

void CRenderStepManager::DeleteAllScene()
{
	m_kSceneList.DeleteAll();
}

void CRenderStepManager::Rendering( const fps::CFpsController &fps )
{
	//レンダリングステップの実行
	FOREACH( CRenderStepPtrList , it , m_kStepList )
	{
		( *it )->Rendering( fps );
	}
}

} } }
