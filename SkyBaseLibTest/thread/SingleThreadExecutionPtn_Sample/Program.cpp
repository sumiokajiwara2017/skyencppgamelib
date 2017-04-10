#include "SkyBaseLib/Controller.h"
#include "SkyBaseLib/Thread.h"
#include "ThreadCommonRes/ThreadCommonRes.h"
#include "UserThread/UserThread.h"

/// <summary>
/// ���C���֐�
/// </summary>
int _tmain( int argc , _TCHAR *argv[] )
{
	argc;
	argv;

	//���C�u����������
	SkyLibController_->Init();

	sky::lib::SmartPointerMT< ThreadCommonRes > res( NEW ThreadCommonRes() );
	IThreadManager_->CreateThread( NEW UserThread( res , 111 ) );
	IThreadManager_->CreateThread( NEW UserThread( res , 222 ) );

	IThreadManager_->WaitAllThreadEnded(); //�S�ẴX���b�h�̏I����҂�

	//�v�����g
	PrintMemory( skyTrue );

	//���C�u����������
	SkyLibController_->Term();
}