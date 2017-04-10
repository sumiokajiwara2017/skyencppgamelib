#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_COROUTINE

#include "SkyBaseLib/Coroutine.h"


class CHoge : public sky::lib::coroutine::CCoroutineBase
{

private:

	//vExecute()���Ŏg���ϐ��͂��ׂă����o�ϐ��ɂ���B
	u32 i;
	u32 j;

public:

	/// <summary>
	/// �����������悤�ɏ��������܂��傤�B
	/// </summary>
	void Initialize()
	{ 
		this->sky::lib::coroutine::CCoroutineBase::Initialize();
		i = 0;
		j = 0;
	}

	s32 vExecute( void* /* pParam */ )
	{
		COROUTINE_BEGIN_;

//		u32 i = 0; //���[�J���ϐ��̒l�͕ێ��ł��Ȃ��i�R���[�`���Ȃ̂ɁI�j�N���X�̃����o�Ɏ����܂��傤�B�X�^�b�N�ɗ��߂��蕪����₷���B
//		u32 j = 0; //���[�J���ϐ��̒l�͕ێ��ł��Ȃ��i�R���[�`���Ȃ̂ɁI�j�N���X�̃����o�Ɏ����܂��傤�B�X�^�b�N�ɗ��߂��蕪����₷���B

		i++;
		SKY_PRINTF( _T( "�����P [ i=%d ] \n" ) , i );

		YIELD_( i * 100 );

		i++;
		SKY_PRINTF( _T( "�����Q [ i=%d ] \n" ) , i );

		YIELD_( i * 100 );

		i++;
		SKY_PRINTF( _T( "�����R [ i=%d ] \n" ) , i );

		YIELD_( i * 100 );

		i++;
		SKY_PRINTF( _T( "�����S [ i=%d ] \n" ) , i );

		YIELD_( i * 100 );

		for ( j = 0 ; j < 5 ; j++ ) //j���������N���X�̃C���X�^���X�����o�ɂ��Ăˁ��c
		{
			i++;
			SKY_PRINTF( _T( "�����T [ i=%d ] \n" ) , i );

			YIELD_( i * 100 );
		}

		COROUTINE_END_;

		return sky::lib::coroutine::COROUTINE_END_VALUE; 
	}
};

TEST( coroutine , 1 ) 
{
	SKY_BASE_LIB_INIT();

	CHoge hoge;

	//Execute���ĂԂ��тɏ����������̏������i��
	s32 ret;

	hoge.Initialize();//��Ώ�����
	while ( ( ret = hoge.vExecute( skyNull ) ) != sky::lib::coroutine::COROUTINE_END_VALUE )
	{
		SKY_PRINTF( _T( "���f �߂�l=%d \n" ) , ret );
	}
	SKY_PRINTF( _T( "�P��ڏI�� \n" ) );

	hoge.Initialize();//��Ώ�����
	while ( ( ret = hoge.vExecute( skyNull ) ) != sky::lib::coroutine::COROUTINE_END_VALUE )
	{
		SKY_PRINTF( _T( "���f �߂�l=%d \n" ) , ret );
	}
	SKY_PRINTF( _T( "�Q��ڏI�� \n" ) );

	SKY_BASE_LIB_TERM();
}

struct CHogeObj : public sky::lib::coroutine::CCoroutineObjectBase
{
	//vExecute()���Ŏg���ϐ��͂��ׂă����o�ϐ��ɂ���B
	u32 i;
	u32 j;

	/// <summary>
	/// �����������悤�ɏ��������܂��傤�B
	/// </summary>
	void Initialize()
	{ 
		this->sky::lib::coroutine::CCoroutineObjectBase::Initialize();
		i = 0;
		j = 0;
	}
};

//�N���X��struct�̃����o�ɂ��Ȃ��ƃR���p�C���G���[�ɂȂ�
struct HogeHoge
{

//���ʂ̊֐����R���[�`��������B
static s32 HogeFunction( CHogeObj &hoge )
{
	COROUTINE_OBJ_BEGIN_( hoge );

//		u32 i = 0; //���[�J���ϐ��̒l�͕ێ��ł��Ȃ��i�R���[�`���Ȃ̂ɁI�j�N���X�̃����o�Ɏ����܂��傤�B�X�^�b�N�ɗ��߂��蕪����₷���B
//		u32 j = 0; //���[�J���ϐ��̒l�͕ێ��ł��Ȃ��i�R���[�`���Ȃ̂ɁI�j�N���X�̃����o�Ɏ����܂��傤�B�X�^�b�N�ɗ��߂��蕪����₷���B

	hoge.i++;
	SKY_PRINTF( _T( "�����P [ i=%d ] \n" ) , hoge.i );

	YIELD_OBJ_( hoge , hoge.i * 100 );

	hoge.i++;
	SKY_PRINTF( _T( "�����Q [ i=%d ] \n" ) , hoge.i );

	YIELD_OBJ_( hoge , hoge.i * 100 );

	hoge.i++;
	SKY_PRINTF( _T( "�����R [ i=%d ] \n" ) , hoge.i );

	YIELD_OBJ_( hoge , hoge.i * 100 );

	hoge.i++;
	SKY_PRINTF( _T( "�����S [ i=%d ] \n" ) , hoge.i );

	YIELD_OBJ_( hoge , hoge.i * 100 );

	for ( hoge.j = 0 ; hoge.j < 5 ; hoge.j++ ) //j���������N���X�̃C���X�^���X�����o�ɂ��Ăˁ��c
	{
		hoge.i++;
		SKY_PRINTF( _T( "�����T [ i=%d ] \n" ) , hoge.i );

		YIELD_OBJ_( hoge , hoge.i * 100 );
	}

	COROUTINE_OBJ_END_;

	return sky::lib::coroutine::COROUTINE_END_VALUE; 
}

};

TEST( coroutine , 2 ) 
{
	SKY_BASE_LIB_INIT();

	CHogeObj hoge;

	//Execute���ĂԂ��тɏ����������̏������i��
	s32 ret;

	hoge.Initialize();//��Ώ�����
	while ( ( ret = HogeHoge::HogeFunction( hoge ) ) != sky::lib::coroutine::COROUTINE_END_VALUE )
	{
		SKY_PRINTF( _T( "���f �߂�l=%d \n" ) , ret );
	}
	SKY_PRINTF( _T( "�P��ڏI�� \n" ) );

	hoge.Initialize();//��Ώ�����
	while ( ( ret = HogeHoge::HogeFunction( hoge ) ) != sky::lib::coroutine::COROUTINE_END_VALUE )
	{
		SKY_PRINTF( _T( "���f �߂�l=%d \n" ) , ret );
	}
	SKY_PRINTF( _T( "�Q��ڏI�� \n" ) );

	SKY_BASE_LIB_TERM();
}


#endif