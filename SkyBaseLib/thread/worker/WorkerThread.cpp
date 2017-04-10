#include "StdAfx.h"
#include "SkyBaseLib/Thread.h"

namespace sky { namespace lib { namespace thread {

void CWorkerThread::ThreadMain()
{
	m_spQueue->IncThreadCount();	

	while ( m_IsValid )
	{
		FunctionCallBackAsyncPtr fnPtr;

		//�L���[������s�v�������o���B
		fnPtr = m_spQueue->PopRequest();

		if ( fnPtr == skyNull )
		{
			m_spWaitObject->Wait();        //�҂���Ԃɓ���
			continue;
		}

		//���s���̃^�X�N���J�E���g�𑝂₷
		m_spQueue->IncExecNum();

		//���s
		fnPtr->run();

		//	���s���̃^�X�N���J�E���g�����炷
		m_spQueue->DecExecNum();
	}

	//	���s���̃X���b�h�J�E���g�����炷
	m_spQueue->DecThreadCount();
}

void CWorkerThread::SetValid( skyBool isValid )
{
	m_IsValid = isValid;
}

CWorkerThread::CWorkerThread( const CWorkQueuePtr& spQueue , u32 stackSize ) :
CThread( stackSize )
{
	m_spQueue = spQueue;
	m_IsValid = skyTrue;
	m_spWaitObject = CWaitObject::Create();
}

CWorkerThread::~CWorkerThread()
{
}

} } }