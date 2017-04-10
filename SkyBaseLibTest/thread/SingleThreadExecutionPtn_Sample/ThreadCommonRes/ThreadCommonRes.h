#pragma once

/// <summary>
/// �X���b�h���ʃ��\�[�X�N���X
/// </summary>
class ThreadCommonRes
{
public:

	void Test1 ( s32 threadNo );
	void Test2 ( s32 threadNo );

	ThreadCommonRes( void ) :
	pCs( sky::lib::thread::ICriticalSection::Create() )
	{
	}

	~ThreadCommonRes( void )
	{
		sky::lib::thread::ICriticalSection::Delete( pCs );
	}

protected:
	sky::lib::thread::ICriticalSection *pCs;
};
