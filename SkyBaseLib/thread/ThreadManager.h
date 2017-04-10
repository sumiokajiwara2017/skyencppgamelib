#pragma once

namespace sky { namespace lib { namespace thread {

/// <summary>
/// �X���b�h�Ǘ�
/// �X���b�h�Ǘ��ɃX���b�h��o�^����Έꊇ�ł̏I���v���A�I���҂����ł���B
/// </summary>
class CThreadManager : public base::SkyObject
{

public:

	/// <summary>
	/// �X���b�h��o�^����
	/// </summary>
	void RegisterThread( const CThreadPtr &spThread );

	/// <summary>
	/// �X���b�h��o�^��������
	/// </summary>
	void UnRegisterThread( const CThreadPtr &spThread );
	void UnRegisterThreadAll();

	/// <summary>
	/// �X���b�h�J�n
	/// </summary>
	void StartAll();

	/// <summary>
	/// �X���b�h�I���v��
	/// </summary>
	void EndAll();

	/// <summary>
	/// �X���b�h�I������
	/// </summary>
	skyBool IsEndAll();

	/// <summary>
	/// �X���b�h�I���҂�(�����Ń��[�v���ďI����҂��܂�)
	/// </summary>
	void EndWaitAll();

	/// <summary>
	/// �X���b�h�ꎞ��~
	/// </summary>
	void StopAll();

	/// <summary>
	/// �X���b�h�ꎞ��~�ĊJ
	/// </summary>
	void ReStartAll();

private:

	/// <summary>
	/// �o�^�X���b�h���X�g
	/// </summary>
	CThreadPtrList m_RegisterThreadList;

};

} } }

/*
�g�p��

using namespace sky::lib;

static void HogeFunc1( u32 param1 )
{
	SKY_PRINTF( _T( "HogeFunc1 %d \n" ) , param1 );
}

class CHogeThread : public thread::CThread
{
    SkyTypeDefRTTI;

	CHogeThread() : CThread( 1024 )
	{
	}

	void ThreadMain()
	{
		SKY_PRINTF( _T( "CHogeThread \n" ) );
	}
};
SkyImplementRTTI( CHogeThread , sky::lib::thread::CThread );

TEST( tread2 , 1 ) 
{
	SKY_BASE_LIB_INIT();


	thread::CThreadManager manager;

	//�X���b�h�𐶐����Ǘ��ɓo�^
	manager.RegisterThread( thread::CThread::Create( 1024 , FunctionCallBackAsyncPtr( FunctionCallBackAsync_v::Create( &HogeFunc1 , 333 ) ) ) );
	manager.RegisterThread( thread::CThreadPtr( NEW CHogeThread() ) );
	manager.StartAll();
	manager.EndWaitAll();
	manager.UnRegisterThreadAll();

	SKY_BASE_LIB_TERM();
}

*/