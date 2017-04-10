#include "SkyBaseLib/Controller.h"
#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "RequestData/RequestData.h"
#include "RequestQueue/RequestQueue.h"
#include "ClientThread/ClientThread.h"
#include "ServerThread/ServerThread.h"

/// <summary>
/// ���C���֐�
/// </summary>
int _tmain( int argc , _TCHAR *argv[] )
{
argc;
argv;

	//���C�u����������
	SkyLibController_->Init();

	RequestQueuePtr rq( NEW RequestQueue() );
	IThreadManager_->CreateThread( NEW ClientThread( rq ) );
	IThreadManager_->CreateThread( NEW ServerThread( rq ) );

	IThreadManager_->WaitAllThreadEnded(); //�S�ẴX���b�h�̏I����҂�

	//���C�u����������
	SkyLibController_->Term();
}