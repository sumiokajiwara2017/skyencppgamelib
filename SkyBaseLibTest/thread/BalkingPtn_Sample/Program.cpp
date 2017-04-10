#include "SkyBaseLib/Controller.h"
#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "DrawCommand/DrawCommand.h"
#include "DrawCommand/DrawCommandManager.h"
#include "DrawExecEvent/DrawExecEvent.h"
#include "DrawThread/DrawThread.h"
#include "UpdateThread/UpdateThread.h"

/// <summary>
/// ���C���֐�
/// </summary>
int _tmain( int argc , _TCHAR *argv[] )
{
argc;
argv;
	//���C�u����������
	SkyLibController_->Init();

	DrawExecEventPtr ev( NEW DrawExecEvent() );
	IThreadManager_->CreateThread( NEW DrawThread( ev ) );
	IThreadManager_->CreateThread( NEW UpdateThread( ev ) );

	IThreadManager_->WaitAllThreadEnded(); //�S�ẴX���b�h�̏I����҂�

	//���C�u����������
	SkyLibController_->Term();
}