#include "SkyBaseLib/Controller.h"
#include "SkyBaseLib/Thread.h"
#include "SkyBaseLib/String.h"
#include "Helper/Helper.h"
#include "Host/Host.h"

/// <summary>
/// ���C���֐�
/// </summary>
int _tmain( int argc , _TCHAR *argv[] )
{
	argc;
	argv;

	//���C�u����������
	SkyLibController_->Init();

	Host	host;
	host.DoWork( L"���������܂����B\n" );
	host.DoWork( L"�_����܂��܂����B\n" );
	host.DoWork( L"�������܂����B\n" );
	SKY_PRINTF( L"------�w���͂��ׂďo����------\n" );

	IThreadManager_->WaitAllThreadEnded(); //�S�ẴX���b�h�̏I����҂�

	//���C�u����������
	SkyLibController_->Term();
}