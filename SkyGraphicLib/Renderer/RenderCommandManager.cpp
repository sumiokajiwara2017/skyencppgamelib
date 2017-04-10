#include "StdAfx.h"
#include "SkyGraphicLib/Renderer.h"

#include "SkyBaseLib/Thread.h"
#include "SkyBaseLib/GameLoop.h"

namespace sky { namespace lib { namespace graphic {

CRenderingCommandPtrList*        CRenderCommandManager::m_pCmmandBuffer           = skyNull;
CRenderingCommandPtrListPtrFifo* CRenderCommandManager::m_pCommandBufferQueue     = skyNull;
CRenderCommandManager::eState    CRenderCommandManager::m_eState                  = CRenderCommandManager::eState_Stop;
delegate::CDelegateHandler*      CRenderCommandManager::m_pPreRenderingEventHndl  = skyNull;
delegate::CDelegateHandler*      CRenderCommandManager::m_pPostRenderingEventHndl = skyNull;
CRenderingCommandPtrListMT*      CRenderCommandManager::m_pPreRenderingCommand    = skyNull;
CRenderingCommandPtrListMT*      CRenderCommandManager::m_pPostRenderingCommand   = skyNull;

void CRenderCommandManager::Initialize()
{
	m_pCmmandBuffer = skyNull;
	m_pCommandBufferQueue = skyNull;

	m_pPreRenderingEventHndl  = NEW__( delegate::CDelegateHandler , delegate::CDelegateHandler() );
	m_pPostRenderingEventHndl = NEW__( delegate::CDelegateHandler , delegate::CDelegateHandler() );

	m_pPreRenderingCommand = NEW__( CRenderingCommandPtrListMT , CRenderingCommandPtrListMT() );
	m_pPostRenderingCommand = NEW__( CRenderingCommandPtrListMT , CRenderingCommandPtrListMT() );
}

void CRenderCommandManager::Dispose()
{
	//描画途中のコマンドバッファがあったら消す
	SAFE_DELETE( m_pCmmandBuffer );

	//キューがあったら中身を消して消す
	if ( m_pCommandBufferQueue != skyNull )
	{
		FOREACH( CRenderingCommandPtrListPtrFifo , it , *m_pCommandBufferQueue )
		{
			DEL *it;
		}

		SAFE_DELETE( m_pCommandBufferQueue );
	}
	m_eState = eState_Stop;

	SAFE_DELETE( m_pPreRenderingCommand );
	SAFE_DELETE( m_pPostRenderingCommand );

	SAFE_DELETE( m_pPreRenderingEventHndl );
	SAFE_DELETE( m_pPostRenderingEventHndl );
}

void CRenderCommandManager::CreateQueue( u32 queueSize )
{
	m_pCommandBufferQueue = NEW__( CRenderingCommandPtrListPtrFifo , CRenderingCommandPtrListPtrFifo( queueSize ) );
}

void CRenderCommandManager::CreateBuffer()
{
	if ( m_pCmmandBuffer != skyNull )
	{
        SKY_PRINTF( _T( "The command buffer still remains.\n" ) );
		return;
	}

	m_pCmmandBuffer = NEW__( CRenderingCommandPtrList , CRenderingCommandPtrList() );
}

void CRenderCommandManager::InvokeCommand( const CRenderingCommandPtr &command )
{
    SKY_ASSERT_MSG( m_pCmmandBuffer != skyNull , _T( "There is no command buffer. \n" ) );

	m_pCmmandBuffer->AddTail( command );
}

void CRenderCommandManager::FlashBuffer()
{
	if ( m_pCmmandBuffer == skyNull ) return;

	ExecCommand( m_pCmmandBuffer );

	//次のバッファを待つ
	m_pCmmandBuffer = skyNull;
	CreateBuffer();
}

void CRenderCommandManager::PushBuffer()
{
    SKY_ASSERT_MSG( m_pCommandBufferQueue != skyNull , _T( "There is no command buffer. \n" ) );
    SKY_ASSERT_MSG( m_pCmmandBuffer != skyNull , _T( "There is no command buffer. \n" ) );

	//いっぱいだったら先頭（一番古いレンダリングデータ）を消す
	if ( m_pCommandBufferQueue->IsFull() )
	{
		CRenderingCommandPtrList *popData = skyNull;
		m_pCommandBufferQueue->Pop( popData );
		DEL popData;
	}

	//バッファ追加
	m_pCommandBufferQueue->Push( m_pCmmandBuffer );

	//次のバッファを待つ
	m_pCmmandBuffer = skyNull;

#ifndef SW_SKYGRAPHICLIB_ASYNC_RENDERING_ON
	//キューからコマンドバッファを取り出して実行
	CRenderCommandManager_::ExecCommand( CRenderCommandManager_::PopBuffer() );
#endif

}

/// <summary>
/// コマンド実行用スレッド
/// </summary>
static skyBool s_isLoop = skyTrue;
class CRenderThread : public thread::CThread
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CRenderThread() : thread::CThread( RENDER_THREAD_DEFAULT_STACK_SIZE ){};

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CRenderThread(){};

	/// <summary>
	/// スレッド実行関数
	/// </summary>
	virtual void ThreadMain()
	{
		//ゲームループ
		GAME_LOOP_START
		{
			if( s_isLoop == skyFalse ) GAME_LOOP_EXIT();

			//もし描画停止要求が着ていたら描画をしない。
			if ( CRenderCommandManager::m_eState == CRenderCommandManager::eState_RenderingStopRequest )
			{//停止要求を受け入れる

				CRenderCommandManager::m_eState = CRenderCommandManager::eState_RendringStop;
			}

			if ( CRenderCommandManager::m_eState != CRenderCommandManager::eState_RendringStop )
			{
				//キューからコマンドバッファを取り出して実行
				CRenderCommandManager_::ExecCommand( CRenderCommandManager_::PopBuffer() );
			}

		}
		GAME_LOOP_END
	}
};
static CRenderThread *s_Thread = skyNull;

void CRenderCommandManager::RequestRenderingStop()
{
	//すでに停止していたらなにもしない
	if ( m_eState == CRenderCommandManager::eState_RendringStop ) return;

	m_eState = CRenderCommandManager::eState_RenderingStopRequest;
}

void CRenderCommandManager::RequestRenderingReStart()
{
	m_eState = CRenderCommandManager::eState_Rendering;
}

void CRenderCommandManager::WaiteRenderingStop()
{
	//すでに停止していたら何もしない
	if ( m_eState == CRenderCommandManager::eState_RendringStop ) return;

	//停止要求
	RequestRenderingStop();

#ifndef SW_SKYGRAPHICLIB_ASYNC_RENDERING_ON
	m_eState = CRenderCommandManager::eState_RendringStop;
#endif

	//停止するまで待つ
	while( m_eState != CRenderCommandManager::eState_RendringStop )
	{
		//一瞬待つ（スレッドを切り替える）
		CTimeUtil_::Wait( 1 );
	}
}

void CRenderCommandManager::StartThread()
{
#ifdef SW_SKYGRAPHICLIB_ASYNC_RENDERING_ON

	s_isLoop = skyTrue;
	s_Thread = NEW__( s_RenderCommandManagerAllocatorInfo , CRenderThread , CRenderThread() );
	CThreadManager_::CreateThread( s_Thread );
#endif
	m_eState = eState_Rendering;
}

void CRenderCommandManager::StopThread()
{
	if ( s_Thread == skyNull ) return;

	s_isLoop = skyFalse;     //停止指示
	s_Thread->EndWait();  //停止するまで待つ
}

CRenderingCommandPtrList* CRenderCommandManager::PopBuffer()
{
    SKY_ASSERT_MSG( m_pCommandBufferQueue != skyNull , _T( "There is no command buffer Queue.\n" ) );

	CRenderingCommandPtrList *pBuffer;
	if ( m_pCommandBufferQueue->Pop( pBuffer ) )
	{
		return pBuffer;
	}
	else
	{
		return skyNull;
	}
}

void CRenderCommandManager::ExecCommand( CRenderingCommandPtrList *pBuffer )
{
	//NULLだったら何もせず返す。
	if( pBuffer == skyNull ) return;

	//タスク実行
	FOREACH( CRenderingCommandPtrListMT , it , *m_pPreRenderingCommand )
	{
		( *it )->run();
	}
	m_pPreRenderingCommand->DeleteAll();

	//イベント発生
	( *m_pPreRenderingEventHndl )();

	//描画
	FOREACH( CRenderingCommandPtrList , it , *pBuffer )
	{
		( *it )->run();
	}

	//イベント発生
	( *m_pPostRenderingEventHndl )();

	//タスク実行
	FOREACH( CRenderingCommandPtrListMT , it , *m_pPostRenderingCommand )
	{
		( *it )->run();
	}
	m_pPostRenderingCommand->DeleteAll();

	//消す
	DEL pBuffer;
}

skyBool CRenderCommandManager::IsRenderingExec()
{
	return ( m_eState == CRenderCommandManager::eState_Rendering );
}

delegate::CDelegateHandler& CRenderCommandManager::PreRendaringEvent()
{
	return *m_pPreRenderingEventHndl;
}

delegate::CDelegateHandler& CRenderCommandManager::PostRendaringEvent()
{
	return *m_pPostRenderingEventHndl;
}

void CRenderCommandManager::AddPreRenderingExecEvent( s32 prio , const CRenderingCommandPtr &spFunc )
{
	m_pPreRenderingCommand->AddTail( prio , spFunc );
}

void CRenderCommandManager::AddPostRenderingExecEvent( s32 prio , const CRenderingCommandPtr &spFunc )
{
	m_pPostRenderingCommand->AddTail( prio , spFunc );
}

} } }
