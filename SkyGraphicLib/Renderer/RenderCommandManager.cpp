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
	//�`��r���̃R�}���h�o�b�t�@�������������
	SAFE_DELETE( m_pCmmandBuffer );

	//�L���[���������璆�g�������ď���
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

	//���̃o�b�t�@��҂�
	m_pCmmandBuffer = skyNull;
	CreateBuffer();
}

void CRenderCommandManager::PushBuffer()
{
    SKY_ASSERT_MSG( m_pCommandBufferQueue != skyNull , _T( "There is no command buffer. \n" ) );
    SKY_ASSERT_MSG( m_pCmmandBuffer != skyNull , _T( "There is no command buffer. \n" ) );

	//�����ς���������擪�i��ԌÂ������_�����O�f�[�^�j������
	if ( m_pCommandBufferQueue->IsFull() )
	{
		CRenderingCommandPtrList *popData = skyNull;
		m_pCommandBufferQueue->Pop( popData );
		DEL popData;
	}

	//�o�b�t�@�ǉ�
	m_pCommandBufferQueue->Push( m_pCmmandBuffer );

	//���̃o�b�t�@��҂�
	m_pCmmandBuffer = skyNull;

#ifndef SW_SKYGRAPHICLIB_ASYNC_RENDERING_ON
	//�L���[����R�}���h�o�b�t�@�����o���Ď��s
	CRenderCommandManager_::ExecCommand( CRenderCommandManager_::PopBuffer() );
#endif

}

/// <summary>
/// �R�}���h���s�p�X���b�h
/// </summary>
static skyBool s_isLoop = skyTrue;
class CRenderThread : public thread::CThread
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CRenderThread() : thread::CThread( RENDER_THREAD_DEFAULT_STACK_SIZE ){};

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CRenderThread(){};

	/// <summary>
	/// �X���b�h���s�֐�
	/// </summary>
	virtual void ThreadMain()
	{
		//�Q�[�����[�v
		GAME_LOOP_START
		{
			if( s_isLoop == skyFalse ) GAME_LOOP_EXIT();

			//�����`���~�v�������Ă�����`������Ȃ��B
			if ( CRenderCommandManager::m_eState == CRenderCommandManager::eState_RenderingStopRequest )
			{//��~�v�����󂯓����

				CRenderCommandManager::m_eState = CRenderCommandManager::eState_RendringStop;
			}

			if ( CRenderCommandManager::m_eState != CRenderCommandManager::eState_RendringStop )
			{
				//�L���[����R�}���h�o�b�t�@�����o���Ď��s
				CRenderCommandManager_::ExecCommand( CRenderCommandManager_::PopBuffer() );
			}

		}
		GAME_LOOP_END
	}
};
static CRenderThread *s_Thread = skyNull;

void CRenderCommandManager::RequestRenderingStop()
{
	//���łɒ�~���Ă�����Ȃɂ����Ȃ�
	if ( m_eState == CRenderCommandManager::eState_RendringStop ) return;

	m_eState = CRenderCommandManager::eState_RenderingStopRequest;
}

void CRenderCommandManager::RequestRenderingReStart()
{
	m_eState = CRenderCommandManager::eState_Rendering;
}

void CRenderCommandManager::WaiteRenderingStop()
{
	//���łɒ�~���Ă����牽�����Ȃ�
	if ( m_eState == CRenderCommandManager::eState_RendringStop ) return;

	//��~�v��
	RequestRenderingStop();

#ifndef SW_SKYGRAPHICLIB_ASYNC_RENDERING_ON
	m_eState = CRenderCommandManager::eState_RendringStop;
#endif

	//��~����܂ő҂�
	while( m_eState != CRenderCommandManager::eState_RendringStop )
	{
		//��u�҂i�X���b�h��؂�ւ���j
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

	s_isLoop = skyFalse;     //��~�w��
	s_Thread->EndWait();  //��~����܂ő҂�
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
	//NULL�������牽�������Ԃ��B
	if( pBuffer == skyNull ) return;

	//�^�X�N���s
	FOREACH( CRenderingCommandPtrListMT , it , *m_pPreRenderingCommand )
	{
		( *it )->run();
	}
	m_pPreRenderingCommand->DeleteAll();

	//�C�x���g����
	( *m_pPreRenderingEventHndl )();

	//�`��
	FOREACH( CRenderingCommandPtrList , it , *pBuffer )
	{
		( *it )->run();
	}

	//�C�x���g����
	( *m_pPostRenderingEventHndl )();

	//�^�X�N���s
	FOREACH( CRenderingCommandPtrListMT , it , *m_pPostRenderingCommand )
	{
		( *it )->run();
	}
	m_pPostRenderingCommand->DeleteAll();

	//����
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
