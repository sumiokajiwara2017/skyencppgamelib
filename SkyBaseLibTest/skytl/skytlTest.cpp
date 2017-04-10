#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_SKYTL

#include "SkyBaseLib/Hash.h"
#include "SkyBaseLib/SKYTL.h"

using namespace sky::lib;

#if OFF_

//テスト用関数
static void HogeFunc1( u32 param1 )
{
	SKY_PRINTF( _T( "static void HogeFunc1( u32 param1 ) call param1=%d \n" ) , param1 );
}
static u32  HogeFunc2( u32 param1 )
{ 
	SKY_PRINTF( _T( "static u32  HogeFunc2( u32 param1 ) call param1=%d \n" ) , param1 );

	return param1;
}

//テスト用クラス
class Hoge1
{
public:

	u32 m_Member1;

	skyBool Func1( u32 param1 )
	{
		SKY_PRINTF( _T( "skyBool  Hoge1::Func1( u32 param1 ) call param1=%d \n" ) , param1 );
		m_Member1 = param1;
		return skyTrue;
	}

	void Func2()
	{
		SKY_PRINTF( _T( "skyBool  Hoge1::Func2() call \n" ) );
		m_Member1 = 555;
	}

};

TEST( skytl , 1 ) 
{
	SKY_BASE_LIB_INIT();

	//static関数の遅延実行
	SmartPointer< FunctionCallBackAsync > f1( FunctionCallBackAsync_v::Create( &HogeFunc1 , 333 ) );
	f1->run();

	//static関数（戻り値あり）の遅延実行
	SmartPointer< FunctionCallBackAsync_r< u32 > > f2( FunctionCallBackAsync_r< u32 >::Create( HogeFunc2 , 777 ) );
	f2->run();
	u32 result = f2->GetResult();
	result;
	
	//クラス関数の遅延実行
	Hoge1 hoge1;
	SmartPointer< FunctionCallBackAsync > f3( FunctionCallBackAsync_v::Create( &Hoge1::Func2 , &hoge1 ) );
	f3->run();

	//クラス関数（戻り値あり）の遅延実行
	SmartPointer< FunctionCallBackAsync_r< skyBool > > f4( FunctionCallBackAsync_r< skyBool >::Create( &Hoge1::Func1 , &hoge1 , 999 ) );
	f4->run();
	skyBool resultB = f4->GetResult();
	resultB;

	SKY_BASE_LIB_TERM();
}

TEST( skytl , 2 ) 
{
	SKY_BASE_LIB_INIT();

	{
		sky::lib::CVector< u32 > vec( 32 );

		for ( u32 i = 0 ; i < 100 ; i++ )
		{
			vec.Add( i );
			printf( "add=%d \n" , vec[ i ] );
		}

		FOREACH( sky::lib::CVector< u32 > , it , vec )
		{
			printf( "get=%d \n" , *it );
		}
	}

	SKY_BASE_LIB_TERM();
}

//テスト用関数
s32 AddParams( s32 a, s32 b ) {
	return a + b;
}

//テスト用クラス
class Fuga {
public:
	s32 MulParams( s32 a, s32 b ) {
		return a * b;
	}
};

TEST( skytl , 3 ) 
{
	SKY_BASE_LIB_INIT();

//---------通常の関数のバインド

	//< 戻り値 ( 引数１の型, 引数２の型 ) >
	sky::lib::Function< s32 ( s32, s32 ) >	kFunction;

	//関数の設定
	kFunction.Reset( AddParams );
		
	//関数の実行
	s32 iResult = kFunction( 5, 10 );

//---------クラスメソッドのバインド

	//< 戻り値 ( 引数１の型, 引数２の型 ) >
	sky::lib::Function< s32 ( s32, s32 ) >	kFunction2;

	//スタック上にクラスのインスタンス生成
	Fuga	kFuga;

	//クラスメソッドをインスタンスと一緒に設定
	kFunction2.Reset( &kFuga, &Fuga::MulParams );
		
	//クラスメソッドの呼び出し
	iResult = kFunction2( 5, 10 );

	SKY_BASE_LIB_TERM();
}

typedef sky::lib::CList< u32 , u32 >  TestList;
TEST( skytl , 4 ) 
{
	SKY_BASE_LIB_INIT();

	{
		TestList list;

		for ( u32 i = 0 ; i < 100 ; i++ )
		{
			list.AddTail( i );
			printf( "add=%d \n" , i );
		}

		FOREACH( TestList , it , list )
		{
			printf( "get=%d \n" , *it );
		}
	}

	SKY_BASE_LIB_TERM();
}

typedef sky::lib::CHash< u32 , u32 >  TestHash;
TEST( skytl , 5 ) 
{
	SKY_BASE_LIB_INIT();

	hash::CHashStringPtr key1 = CHS( _T( "key1" ) );

	{
		//設定
		TestHash hash( 16 );
		hash.Set( key1 , 111 );
		printf( "set=%d \n" , 111 );

		u32 result;
		hash.Get( key1 , result);
		printf( "set=%d \n" , hash[ key1] );
	}

	SKY_BASE_LIB_TERM();
}

class Test6HogeClass
{

public:

	u32 m_Status;

	//コンストラクタ
	Test6HogeClass()
	{
		m_Status = 999;
	}

	//デストラクタ
	virtual ~Test6HogeClass()
	{
		m_Status  = 0;
	}
};

#define USE_POOL
#define TEST6_POOL_NUM				8
#define TEST6_EXEC_NUM				10240

TEST( skytl , 6 ) 
{
	SKY_BASE_LIB_INIT();

	CMemoryPool< Test6HogeClass > pool( TEST6_POOL_NUM );

	for ( u32 i = 0 ; i < TEST6_EXEC_NUM ; i++ )
	{
#ifdef USE_POOL //Poolを使ったほうが実行回数10240で320msほど早かった
		Test6HogeClass *value1 = pool.Create();
		Test6HogeClass *value2 = pool.Create();
		Test6HogeClass *value3 = pool.Create();
		Test6HogeClass *value4 = pool.Create();
		Test6HogeClass *value5 = pool.Create();
		Test6HogeClass *value6 = pool.Create();
		Test6HogeClass *value7 = pool.Create();
		Test6HogeClass *value8 = pool.Create();
		Test6HogeClass *value9 = pool.Create();
		Test6HogeClass *value10 = pool.Create();
		Test6HogeClass *value11 = pool.Create();
		Test6HogeClass *value12 = pool.Create();
		Test6HogeClass *value13 = pool.Create();
		Test6HogeClass *value14 = pool.Create();
		Test6HogeClass *value15 = pool.Create();
		Test6HogeClass *value16 = pool.Create();

		pool.Delete( value1 );
		pool.Delete( value2 );
		pool.Delete( value3 );
		pool.Delete( value4 );
		pool.Delete( value5 );
		pool.Delete( value6 );
		pool.Delete( value7 );
		pool.Delete( value8 );
		pool.Delete( value9 );
		pool.Delete( value10 );
		pool.Delete( value11 );
		pool.Delete( value12 );
		pool.Delete( value13 );
		pool.Delete( value14 );
		pool.Delete( value15 );
		pool.Delete( value16 );
#else
		Test6HogeClass *value1 = NEW Test6HogeClass();
		Test6HogeClass *value2 = NEW Test6HogeClass();
		Test6HogeClass *value3 = NEW Test6HogeClass();
		Test6HogeClass *value4 = NEW Test6HogeClass();
		Test6HogeClass *value5 = NEW Test6HogeClass();
		Test6HogeClass *value6 = NEW Test6HogeClass();
		Test6HogeClass *value7 = NEW Test6HogeClass();
		Test6HogeClass *value8 = NEW Test6HogeClass();
		Test6HogeClass *value9 = NEW Test6HogeClass();
		Test6HogeClass *value10 = NEW Test6HogeClass();
		Test6HogeClass *value11 = NEW Test6HogeClass();
		Test6HogeClass *value12 = NEW Test6HogeClass();
		Test6HogeClass *value13 = NEW Test6HogeClass();
		Test6HogeClass *value14 = NEW Test6HogeClass();
		Test6HogeClass *value15 = NEW Test6HogeClass();
		Test6HogeClass *value16 = NEW Test6HogeClass();

		DEL value1;
		DEL value2;
		DEL value3;
		DEL value4;
		DEL value5;
		DEL value6;
		DEL value7;
		DEL value8;
		DEL value9;
		DEL value10;
		DEL value11;
		DEL value12;
		DEL value13;
		DEL value14;
		DEL value15;
		DEL value16;
#endif
	}

	CMemoryPoolSize poolSize( sizeof( Test6HogeClass ) , TEST6_POOL_NUM );

	for ( u32 i = 0 ; i < TEST6_EXEC_NUM ; i++ )
	{
#ifdef USE_POOL //Poolを使ったほうが実行回数10240で320msほど早かった
		Test6HogeClass *value1 = ( Test6HogeClass * )poolSize.Create();
		Test6HogeClass *value2 = ( Test6HogeClass * )poolSize.Create();
		Test6HogeClass *value3 = ( Test6HogeClass * )poolSize.Create();
		Test6HogeClass *value4 = ( Test6HogeClass * )poolSize.Create();
		Test6HogeClass *value5 = ( Test6HogeClass * )poolSize.Create();
		Test6HogeClass *value6 = ( Test6HogeClass * )poolSize.Create();
		Test6HogeClass *value7 = ( Test6HogeClass * )poolSize.Create();
		Test6HogeClass *value8 = ( Test6HogeClass * )poolSize.Create();
		Test6HogeClass *value9 = ( Test6HogeClass * )poolSize.Create();
		Test6HogeClass *value10 = ( Test6HogeClass * )poolSize.Create();
		Test6HogeClass *value11 = ( Test6HogeClass * )poolSize.Create();
		Test6HogeClass *value12 = ( Test6HogeClass * )poolSize.Create();
		Test6HogeClass *value13 = ( Test6HogeClass * )poolSize.Create();
		Test6HogeClass *value14 = ( Test6HogeClass * )poolSize.Create();
		Test6HogeClass *value15 = ( Test6HogeClass * )poolSize.Create();
		Test6HogeClass *value16 = ( Test6HogeClass * )poolSize.Create();

		poolSize.Delete( value1 );
		poolSize.Delete( value2 );
		poolSize.Delete( value3 );
		poolSize.Delete( value4 );
		poolSize.Delete( value5 );
		poolSize.Delete( value6 );
		poolSize.Delete( value7 );
		poolSize.Delete( value8 );
		poolSize.Delete( value9 );
		poolSize.Delete( value10 );
		poolSize.Delete( value11 );
		poolSize.Delete( value12 );
		poolSize.Delete( value13 );
		poolSize.Delete( value14 );
		poolSize.Delete( value15 );
		poolSize.Delete( value16 );
#else
		Test6HogeClass *value1 = NEW Test6HogeClass();
		Test6HogeClass *value2 = NEW Test6HogeClass();
		Test6HogeClass *value3 = NEW Test6HogeClass();
		Test6HogeClass *value4 = NEW Test6HogeClass();
		Test6HogeClass *value5 = NEW Test6HogeClass();
		Test6HogeClass *value6 = NEW Test6HogeClass();
		Test6HogeClass *value7 = NEW Test6HogeClass();
		Test6HogeClass *value8 = NEW Test6HogeClass();
		Test6HogeClass *value9 = NEW Test6HogeClass();
		Test6HogeClass *value10 = NEW Test6HogeClass();
		Test6HogeClass *value11 = NEW Test6HogeClass();
		Test6HogeClass *value12 = NEW Test6HogeClass();
		Test6HogeClass *value13 = NEW Test6HogeClass();
		Test6HogeClass *value14 = NEW Test6HogeClass();
		Test6HogeClass *value15 = NEW Test6HogeClass();
		Test6HogeClass *value16 = NEW Test6HogeClass();

		DEL value1;
		DEL value2;
		DEL value3;
		DEL value4;
		DEL value5;
		DEL value6;
		DEL value7;
		DEL value8;
		DEL value9;
		DEL value10;
		DEL value11;
		DEL value12;
		DEL value13;
		DEL value14;
		DEL value15;
		DEL value16;
#endif
	}

	SKY_BASE_LIB_TERM();
}

#define TEST7_FIFO_SIZE ( 100 )

typedef sky::lib::CFifo< u32 >  TestFifo;
TEST( skytl , 7 ) 
{
	TestFifo fifo( TEST7_FIFO_SIZE );

	for ( u32 i = 0 ; i < TEST7_FIFO_SIZE ; i++ )
	{
		fifo.Push( i );
	}

	FOREACH( TestFifo , it , fifo )
	{
		printf( "push=%d \n" , *it );
	}

	for ( u32 i = 0 ; i < TEST7_FIFO_SIZE ; i++ )
	{
		u32 val;
		fifo.Pop( val );
		printf( "pop=%d \n" , val );
	}
}

#define TEST8_STACK_SIZE ( 100 )

typedef sky::lib::CStack< u32 >  TestStack;
TEST( skytl , 8 ) 
{
	TestStack stack( TEST8_STACK_SIZE );

	for ( u32 i = 0 ; i < TEST8_STACK_SIZE ; i++ )
	{
		stack.Push( i );
	}

	FOREACH( TestStack , it , stack )
	{
		printf( "push=%d \n" , *it );
	}

	for ( u32 i = 0 ; i < TEST8_STACK_SIZE ; i++ )
	{
		u32 val;
		stack.Pop( val );
		printf( "pop=%d \n" , val );
	}
}

typedef sky::lib::CList< u32 , u32 >  TestListFifo;
TEST( skytl , 9 ) 
{
	TestListFifo fifo;

	for ( u32 i = 0 ; i < TEST8_STACK_SIZE ; i++ )
	{
		fifo.AddTail( i );
	}

	FOREACH( TestListFifo , it , fifo )
	{
		printf( "push=%d \n" , *it );
	}
}

TEST( skytl , 10 ) 
{
	SKY_BASE_LIB_INIT();

	{
		sky::lib::CVector< u32 > vec( 32 );

		for ( u32 i = 0 ; i < 100 ; i++ )
		{
			vec.Add( i );
			printf( "add=%d \n" , vec[ i ] );
		}

		FOREACH( sky::lib::CVector< u32 > , it , vec )
		{
			printf( "get=%d \n" , *it );
		}

		for ( u32 i = 0 ; i < 100 ; i++ )
		{
			if ( i % 3 == 0 )
			{
				vec.Delete( i );
			}
		}

		vec.Delete( 22 );
		vec.Delete( 15 );


		FOREACH( sky::lib::CVector< u32 > , it , vec )
		{
			printf( "get=%d \n" , *it );
		}

	}

	SKY_BASE_LIB_TERM();
}

class CDirectX;
class COpenGL;

//#define DXF
#define GLF

#ifdef DXF
#define RENDERTYPE CDirectX
#endif

#ifdef GLF
#define RENDERTYPE COpenGL
#endif

template< class T = RENDERTYPE >
class CHoge : public base::SkyRefObject
{
public:
	void Print(){ SKY_PRINTF( "注意）この処理はこの環境では未実装。\n" ); };
};
typedef SmartPointer< CHoge<> >	CHogePtr;

class CHogeManager
{
	CHogePtr hoge;
};

template<>
void CHoge< CDirectX >::Print()
{
	SKY_PRINTF( "DirectX! \n" );
}

template<>
void CHoge< COpenGL >::Print()
{
	SKY_PRINTF( "OpenGL! \n" );
}

TEST( skytl , 11 ) 
{
	CHogePtr spHoge( NEW CHoge<>() );
	spHoge->Print();
}

#include "SkyBaseLib/State.h"

class CHoge
{

public:

	enum eState
	{
		START = 0 ,
		PROC ,
		END ,
		ALL ,
		ALL2 ,
	};

	skyBool Start( const fps::CFpsController & )
	{
		SKY_PRINTF( _T( "Start \n" ) );
//		m_StateManager.PushState( PROC );
		return skyTrue;
	}
	skyBool Proc( const fps::CFpsController & )
	{
		SKY_PRINTF( _T( "Proc \n" ) );
//		m_StateManager.PushState( END );
		return skyTrue;
	}
	skyBool End( const fps::CFpsController & )
	{
		SKY_PRINTF( _T( "End \n" ) );
		return skyTrue;
	}

	skyBool All( const fps::CFpsController & )
	{
		SKY_PRINTF( _T( "All \n" ) );
//		m_StateManager.PushState( PROC );
		return skyTrue;
	}
	skyBool All2( const fps::CFpsController & )
	{
		SKY_PRINTF( _T( "All2 \n" ) );
//		m_StateManager.PushState( PROC );
		return skyTrue;
	}

	state::CFunctionStateManager< CHoge , eState > m_StateManager;

	#pragma warning(push)
	#pragma warning(disable : 4355)
	CHoge() : m_StateManager( this )
	{
		//ステート関数を登録
		m_StateManager.RegisterStateData( START , sky::lib::state::CFunctionStateManager< CHoge , eState >::FUNCTIONData( &CHoge::Start ) );
		m_StateManager.RegisterState( PROC  , &CHoge::Proc );
		m_StateManager.RegisterState( END   , &CHoge::End );
		m_StateManager.RegisterState( ALL   , &CHoge::All );
		m_StateManager.RegisterState( ALL2  , &CHoge::All2 );

		//ステートをキューに投入
		m_StateManager.PushState( START );
		m_StateManager.PushState( PROC );  
		m_StateManager.PushState( END );
		m_StateManager.AddGlobalState( ALL );
		m_StateManager.AddGlobalState( ALL2 );

		//キャンセルテスト
//		m_StateManager.CancelState( START );
//		m_StateManager.CancelState( PROC );
//		m_StateManager.CancelState( END );
	}
	#pragma warning(pop)

	skyBool Update( const fps::CFpsController &fps )
	{
		return m_StateManager.Update( fps );
	}
};

TEST( skytl , 12 ) 
{
	SKY_BASE_LIB_INIT();
	
	CHoge hoge;
	
	fps::CFpsController fps;

	while( hoge.Update( fps ) )
	{
	}

	SKY_BASE_LIB_TERM();
}

TEST( skytl , 13 ) 
{
	CStack< u32 > u32Stack( 32 );

	u32Stack.Push( 0 );
	u32Stack.Push( 1 );
	u32Stack.Push( 2 );
	u32Stack.Push( 3 );
	u32Stack.Push( 4 );
	u32Stack.Push( 5 );

	SKY_PRINTF( "%d \n" , u32Stack[ 0 ] );
	SKY_PRINTF( "%d \n" , u32Stack[ 1 ] );
	SKY_PRINTF( "%d \n" , u32Stack[ 2 ] );
	SKY_PRINTF( "%d \n" , u32Stack[ 3 ] );
	SKY_PRINTF( "%d \n" , u32Stack[ 4 ] );
	SKY_PRINTF( "%d \n" , u32Stack[ 5 ] );

	SKY_PRINTF( "%d \n" , u32Stack.GetTop() );
	SKY_PRINTF( "%d \n" , u32Stack.GetBottom() );

	u32Stack.PopBottom();

	SKY_PRINTF( "%d \n" , u32Stack.GetTop() );
	SKY_PRINTF( "%d \n" , u32Stack.GetBottom() );

	u32Stack.PopBottom();

	SKY_PRINTF( "%d \n" , u32Stack.GetTop() );
	SKY_PRINTF( "%d \n" , u32Stack.GetBottom() );

	u32Stack.PopBottom();

	SKY_PRINTF( "%d \n" , u32Stack.GetTop() );
	SKY_PRINTF( "%d \n" , u32Stack.GetBottom() );

	u32Stack.PopBottom();

	SKY_PRINTF( "%d \n" , u32Stack.GetTop() );
	SKY_PRINTF( "%d \n" , u32Stack.GetBottom() );

	u32Stack.PopBottom();

	SKY_PRINTF( "%d \n" , u32Stack.GetTop() );
	SKY_PRINTF( "%d \n" , u32Stack.GetBottom() );

	u32Stack.PopBottom();
}

#endif

class CHoge1
{

public:

	skyBool Load()
	{
		SKY_PRINTF( _T( "Hoge1::Load \n" ) );
		return skyTrue;
	}

	void    Init()
	{
		SKY_PRINTF( _T( "Hoge1::Init \n" ) );
	}

	void    Start()
	{
		SKY_PRINTF( _T( "Hoge1::Start \n" ) );
	}

	skyBool Update( const fps::CFpsController & )
	{
		SKY_PRINTF( _T( "Hoge1::Update \n" ) );
		return skyTrue;
	}

	void    End()
	{
		SKY_PRINTF( _T( "Hoge1::End \n" ) );
	}
};

class CHoge2
{

public:

	skyBool Load()
	{
		SKY_PRINTF( _T( "Hoge2::Load \n" ) );
		return skyTrue;
	}

	void    Init()
	{
		SKY_PRINTF( _T( "Hoge2::Init \n" ) );
	}

	void    Start()
	{
		SKY_PRINTF( _T( "Hoge2::Start \n" ) );
	}

	skyBool Update( const fps::CFpsController & )
	{
		SKY_PRINTF( _T( "Hoge2::Update \n" ) );
		return skyTrue;
	}

	void    End()
	{
		SKY_PRINTF( _T( "Hoge2::End \n" ) );
	}
};


#include "SkyBaseLib/State.h"

enum eTestState
{
	eTestState_1 = 0,
	eTestState_2,
};

TEST( skytl , 14 ) 
{
	SKY_BASE_LIB_INIT();

	state::CFunctionDelgateStateManager< eTestState > manager;

	CHoge1 hoge1;
	state::FUNCTIONDelegateData< CHoge1 > hogeDelegate1( &hoge1 , &CHoge1::Update , &CHoge1::Init , &CHoge1::Load , &CHoge1::Start , &CHoge1::End );
//	state::FUNCTIONDelegateData< CHoge1 > hogeDelegate1( &hoge1 , &CHoge1::Update );
	manager.RegisterStateData( eTestState_1 , &hogeDelegate1 );

	CHoge2 hoge2;
	state::FUNCTIONDelegateData< CHoge2 > hogeDelegate2( &hoge2 , &CHoge2::Update , &CHoge2::Init , &CHoge2::Load , &CHoge2::Start , &CHoge2::End );
//	state::FUNCTIONDelegateData< CHoge2 > hogeDelegate2( &hoge2 , &CHoge2::Update );
	manager.RegisterStateData( eTestState_2 , &hogeDelegate2 );

	fps::CFpsController fps;

	manager.PushState( eTestState_1 );
	manager.PushState( eTestState_2 );

	hogeDelegate1.Load();
	hogeDelegate2.Load();
	while( manager.Update( fps ) )
	{
	}

	SKY_BASE_LIB_TERM();
}

#endif