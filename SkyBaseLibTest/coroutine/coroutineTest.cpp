#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_COROUTINE

#include "SkyBaseLib/Coroutine.h"


class CHoge : public sky::lib::coroutine::CCoroutineBase
{

private:

	//vExecute()中で使う変数はすべてメンバ変数にする。
	u32 i;
	u32 j;

public:

	/// <summary>
	/// 正しく動くように初期化しましょう。
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

//		u32 i = 0; //ローカル変数の値は保持できない（コルーチンなのに！）クラスのメンバに持ちましょう。スタックに溜めるより分かりやすい。
//		u32 j = 0; //ローカル変数の値は保持できない（コルーチンなのに！）クラスのメンバに持ちましょう。スタックに溜めるより分かりやすい。

		i++;
		SKY_PRINTF( _T( "処理１ [ i=%d ] \n" ) , i );

		YIELD_( i * 100 );

		i++;
		SKY_PRINTF( _T( "処理２ [ i=%d ] \n" ) , i );

		YIELD_( i * 100 );

		i++;
		SKY_PRINTF( _T( "処理３ [ i=%d ] \n" ) , i );

		YIELD_( i * 100 );

		i++;
		SKY_PRINTF( _T( "処理４ [ i=%d ] \n" ) , i );

		YIELD_( i * 100 );

		for ( j = 0 ; j < 5 ; j++ ) //jももちろんクラスのインスタンスメンバにしてね☆彡
		{
			i++;
			SKY_PRINTF( _T( "処理５ [ i=%d ] \n" ) , i );

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

	//Executeを呼ぶたびに少しずつ内部の処理が進む
	s32 ret;

	hoge.Initialize();//絶対初期化
	while ( ( ret = hoge.vExecute( skyNull ) ) != sky::lib::coroutine::COROUTINE_END_VALUE )
	{
		SKY_PRINTF( _T( "中断 戻り値=%d \n" ) , ret );
	}
	SKY_PRINTF( _T( "１回目終了 \n" ) );

	hoge.Initialize();//絶対初期化
	while ( ( ret = hoge.vExecute( skyNull ) ) != sky::lib::coroutine::COROUTINE_END_VALUE )
	{
		SKY_PRINTF( _T( "中断 戻り値=%d \n" ) , ret );
	}
	SKY_PRINTF( _T( "２回目終了 \n" ) );

	SKY_BASE_LIB_TERM();
}

struct CHogeObj : public sky::lib::coroutine::CCoroutineObjectBase
{
	//vExecute()中で使う変数はすべてメンバ変数にする。
	u32 i;
	u32 j;

	/// <summary>
	/// 正しく動くように初期化しましょう。
	/// </summary>
	void Initialize()
	{ 
		this->sky::lib::coroutine::CCoroutineObjectBase::Initialize();
		i = 0;
		j = 0;
	}
};

//クラスかstructのメンバにしないとコンパイルエラーになる
struct HogeHoge
{

//普通の関数をコルーチン化する。
static s32 HogeFunction( CHogeObj &hoge )
{
	COROUTINE_OBJ_BEGIN_( hoge );

//		u32 i = 0; //ローカル変数の値は保持できない（コルーチンなのに！）クラスのメンバに持ちましょう。スタックに溜めるより分かりやすい。
//		u32 j = 0; //ローカル変数の値は保持できない（コルーチンなのに！）クラスのメンバに持ちましょう。スタックに溜めるより分かりやすい。

	hoge.i++;
	SKY_PRINTF( _T( "処理１ [ i=%d ] \n" ) , hoge.i );

	YIELD_OBJ_( hoge , hoge.i * 100 );

	hoge.i++;
	SKY_PRINTF( _T( "処理２ [ i=%d ] \n" ) , hoge.i );

	YIELD_OBJ_( hoge , hoge.i * 100 );

	hoge.i++;
	SKY_PRINTF( _T( "処理３ [ i=%d ] \n" ) , hoge.i );

	YIELD_OBJ_( hoge , hoge.i * 100 );

	hoge.i++;
	SKY_PRINTF( _T( "処理４ [ i=%d ] \n" ) , hoge.i );

	YIELD_OBJ_( hoge , hoge.i * 100 );

	for ( hoge.j = 0 ; hoge.j < 5 ; hoge.j++ ) //jももちろんクラスのインスタンスメンバにしてね☆彡
	{
		hoge.i++;
		SKY_PRINTF( _T( "処理５ [ i=%d ] \n" ) , hoge.i );

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

	//Executeを呼ぶたびに少しずつ内部の処理が進む
	s32 ret;

	hoge.Initialize();//絶対初期化
	while ( ( ret = HogeHoge::HogeFunction( hoge ) ) != sky::lib::coroutine::COROUTINE_END_VALUE )
	{
		SKY_PRINTF( _T( "中断 戻り値=%d \n" ) , ret );
	}
	SKY_PRINTF( _T( "１回目終了 \n" ) );

	hoge.Initialize();//絶対初期化
	while ( ( ret = HogeHoge::HogeFunction( hoge ) ) != sky::lib::coroutine::COROUTINE_END_VALUE )
	{
		SKY_PRINTF( _T( "中断 戻り値=%d \n" ) , ret );
	}
	SKY_PRINTF( _T( "２回目終了 \n" ) );

	SKY_BASE_LIB_TERM();
}


#endif