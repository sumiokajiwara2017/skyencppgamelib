#include "SkyBaseLib/Controller.h"
#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "CommonResource/CommonResource.h"
#include "WriterThread/WriterThread.h"
#include "ReaderThread/ReaderThread.h"

/// <summary>
/// ���C���֐�
/// </summary>
int _tmain( int argc , _TCHAR *argv[] )
{
	argc;
	argv;

	//���C�u����������
	SkyLibController_->Init();

	sky::lib::thread::ReadWriteLock	rwl;
	CommonResource					comres;

	IThreadManager_->CreateThread( NEW WriterThread( &rwl , &comres , L"�������ݒS��" ) );

	IThreadManager_->CreateThread( NEW ReaderThread( &rwl , &comres , L"�ǂݍ��ݒS���P" ) );
	IThreadManager_->CreateThread( NEW ReaderThread( &rwl , &comres , L"�ǂݍ��ݒS���Q" ) );
	IThreadManager_->CreateThread( NEW ReaderThread( &rwl , &comres , L"�ǂݍ��ݒS���R" ) );

	IThreadManager_->WaitAllThreadEnded(); //�S�ẴX���b�h�̏I����҂�

	//�v�����g
	PrintMemory( skyTrue );

	//���C�u����������
	SkyLibController_->Term();
}