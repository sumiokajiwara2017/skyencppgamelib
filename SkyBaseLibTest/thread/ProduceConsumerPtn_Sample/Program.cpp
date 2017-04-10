#include "SkyBaseLib/Controller.h"
#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "Data/Data.h"
#include "Channel/Channel.h"
#include "ConsumerThread/ConsumerThread.h"
#include "ProducerThread/ProducerThread.h"

/// <summary>
/// ���C���֐�
/// </summary>
int _tmain( int argc , _TCHAR *argv[] )
{
	argc;
	argv;

	//���C�u����������
	SkyLibController_->Init();

	Channel ch;
	IThreadManager_->CreateThread( NEW ConsumerThread( &ch ) );
	IThreadManager_->CreateThread( NEW ProducerThread( &ch , L"�A�}�]���Ŕ������{" ) );
	IThreadManager_->CreateThread( NEW ProducerThread( &ch , L"�e���r�V���b�s���O�Ŕ������Ƌ�" ) );

	IThreadManager_->WaitAllThreadEnded(); //�S�ẴX���b�h�̏I����҂�

	//�v�����g
	PrintMemory( skyTrue );

	//���C�u����������
	SkyLibController_->Term();
}