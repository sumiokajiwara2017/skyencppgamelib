#include "SkyBaseLib/Controller.h"
#include "SkyBaseLib/Thread.h"

static void Work( s32 n )
{
	SKY_PRINTF( L"1����%d�܂ł̘a�����߂܂��B�X���b�h�ԍ�:%d \n" , n , IThreadAdptr_->GetCurrentThreadId() );

	s32 total = 0;
	for ( s32 i = 0 ; i <= n ; i++ )
	{
		total += i;
		::Sleep( 10 );		//�v�Z�Ɏ��Ԃ������邱�Ƃ�\��
	}

	SKY_PRINTF( L"1����%d�܂ł̘a��%d�ł��B�X���b�h�ԍ�%d \n" , n , total , IThreadAdptr_->GetCurrentThreadId() );
}

/// <summary>
/// ���C���֐�
/// </summary>
int _tmain( int argc , _TCHAR *argv[] )
{
	argc;
	argv;

	//���C�u����������
	SkyLibController_->Init();

	sky::lib::thread::ThreadPool	threadPool;
	threadPool.StartThread( 3 );				// �X���b�h���쐬���đҋ@�B

	for ( s32 i = 0 ; i < 10 ; i++ )
	{
		sky::lib::FunctionCallBack *fn( sky::lib::FunctionCallBack_v::Create( Work , i * 100 + 10 ) );
		threadPool.Invoke( fn );				// �ҋ@�����Ă���X���b�h�ɏ���������B
	}

	threadPool.WaitAllWorkEnded();							// ���ׂĂ̎d���̎��s����������̂�҂B
	threadPool.ClearAllThread();							// ���ׂẴX���b�h��j������B

	SKY_PRINTF( L"�S���̎d�����I���܂����B  \n" );

	//�v�����g
	PrintMemory( skyTrue );

	//���C�u����������
	SkyLibController_->Term();
}