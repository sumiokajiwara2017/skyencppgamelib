#pragma once

namespace sky { namespace lib {

class FunctionCallBackAsync : public base::SkyRefObject
{
/**

使い方
		
using namespace sky::lib;

//テスト用関数
static void HogeFunc1( u32 param1 ){ param1; }
static u32  HogeFunc2( u32 param1 ){ return param1; }

//テスト用クラス
class Hoge1
{
public:

	u32 m_Member1;

	skyBool Func1( u32 param1 )
	{
		m_Member1 = param1;
		return skyTrue;
	}

	void Func2()
	{
		m_Member1 = 555;
	}

};

TEST( skytl , 1 ) 
{
	SkyBaseLibControllerCreate_();	//ライブラリコントローラーの作成
	SkyBaseLibController_->Init();	//ライブラリの初期化

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

	SkyBaseLibController_->Term();	//ライブラリの末期化
	SkyBaseLibControllerDelete_();	//ライブラリコントローラの削除
}
*/
public:

	/// <summary>
	/// 関数実行
	/// </summary>
	virtual	void run() = 0;

	/// <summary>
	/// 実行終了判定
	/// </summary>
	skyBool	IsEnd() const { return m_IsEnd; }

	/// <summary>
	/// リセットEnd
	/// </summary>
	void ResetEnd(){ m_IsEnd = skyFalse; }

	/// <summary>
	/// コンストラクタ
	/// </summary>
	FunctionCallBackAsync() : m_IsEnd( skyFalse ){}

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~FunctionCallBackAsync(){}

	/// <summary>
	/// コールバックするthisポインタだけあとで変更する
	/// </summary>
	template < class T >
	void Bind( T * t )
	{ 
		inner_bind( ( void * ) t );
	}

protected:

	/// <summary>
	/// 実行終了フラグ
	/// </summary>
	skyBool m_IsEnd;

	virtual void inner_bind( void * )
	{
		SKY_PANIC_MSG( _T( "not have no static function." ) );
	}
};

template < class A1 , class Arg1 >
class FunctionCallBackAsync_vg1;
template < class A1 , class A2 , class Arg1 , class Arg2 >
class FunctionCallBackAsync_vg2;
template < class A1 , class A2 , class A3 , class Arg1 , class Arg2 , class Arg3 >
class FunctionCallBackAsync_vg3;
template < class A1 , class A2 , class A3 , class A4 ,
    class Arg1 , class Arg2 , class Arg3 , class Arg4>
class FunctionCallBackAsync_vg4;
template < class A1 , class A2 , class A3 , class A4 , class A5,
    class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5 >
class FunctionCallBackAsync_vg5;
template < class ObjectType>
class FunctionCallBackAsync_vm0;
template < class ObjectType , class A1 , class Arg1 >
class FunctionCallBackAsync_vm1;
template < class ObjectType , class A1 , class A2 , class Arg1 , class Arg2 >
class FunctionCallBackAsync_vm2;
template < class ObjectType , class A1 , class A2 , class A3 ,
    class Arg1 , class Arg2 , class Arg3 >
class FunctionCallBackAsync_vm3;
template < class ObjectType , class A1 , class A2 , class A3 , class A4 ,
    class Arg1 , class Arg2 , class Arg3 , class Arg4 >
class FunctionCallBackAsync_vm4;
template < class ObjectType , class A1 , class A2 , class A3 , class A4 , class A5,
    class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5 >
class FunctionCallBackAsync_vm5;

//テンプレートTypedef郡
typedef SmartPointer< FunctionCallBackAsync >			FunctionCallBackAsyncPtr;		//スマートポインタ定義
typedef CFifo< FunctionCallBackAsyncPtr >				FunctionCallBackAsyncPtrFifo;	//Fifo定義
typedef CList< s32 , FunctionCallBackAsyncPtr >			FunctionCallBackAsyncPtrList;	//List定義
typedef CListMT< s32 , FunctionCallBackAsyncPtr >		FunctionCallBackAsyncPtrListMT;	//List定義

/// <summary>
/// 戻り値なしのコールバック関数を作成するためのテンプレートクラス
/// </summary>
class FunctionCallBackAsync_v : public FunctionCallBackAsync
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~FunctionCallBackAsync_v(){}

	/// <summary>
	/// 戻り値なし、グローバル関数、引数なし関数用
	/// </summary>
	static FunctionCallBackAsync_v *Create( void ( * f )() );

	template < class Arg1 , class A1 >
	static FunctionCallBackAsync_v *Create( void ( * f )( A1 ) , const Arg1 & a1 )
	{
		/// <summary>
		/// 戻り値なし、グローバル関数、引数１つの関数用
		/// </summary>
		return NEW_() FunctionCallBackAsync_vg1< A1 , Arg1 >( f , a1 );
	}

	template < class Arg1 , class Arg2 , class A1 , class A2 >
	static FunctionCallBackAsync_v* Create( void ( * f )( A1 , A2 ) , const Arg1& a1 , const Arg2& a2 )
	{
		/// <summary>
		/// 戻り値なし、グローバル関数、引数２つの関数用
		/// </summary>
		return NEW_() FunctionCallBackAsync_vg2< A1 , A2 , Arg1 , Arg2 >( f , a1 , a2 );
	}

	template < class Arg1 , class Arg2 , class Arg3 , class A1 , class A2 , class A3 >
	static FunctionCallBackAsync_v* Create( void ( * f )( A1 , A2 , A3 ) ,
		 const Arg1 & a1 , const Arg2 & a2 , const Arg3 & a3 )
	{
		/// <summary>
		/// 戻り値なし、グローバル関数、引数３つの関数用
		/// </summary>
		return NEW_() FunctionCallBackAsync_vg3< A1 , A2 , A3 , Arg1 , Arg2 , Arg3 >( f , a1 , a2 , a3 );
	}
	template < class Arg1 , class Arg2 , class Arg3 , class Arg4 , class A1 , class A2 , class A3 , class A4 >
	static FunctionCallBackAsync_v* Create( void ( * f )( A1 , A2 , A3 , A4 ) ,
		 const Arg1& a1 , const Arg2& a2 , const Arg3& a3 , const Arg4& a4 )
	{
		/// <summary>
		/// 戻り値なし、グローバル関数、引数４つの関数用
		/// </summary>
		return NEW_() FunctionCallBackAsync_vg4< A1 , A2 , A3 , A4 , Arg1 , Arg2 , Arg3 , Arg4 >( f , a1 , a2 , a3 , a4 );
	}
	template < class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5 , class A1 , class A2 , class A3 , class A4 , class A5 >
	static FunctionCallBackAsync_v* Create( void ( * f )( A1 , A2 , A3 , A4 , A5 ) ,
		 const Arg1& a1 , const Arg2& a2 , const Arg3& a3 , const Arg4& a4 , const Arg5& a5 )
	{
		/// <summary>
		/// 戻り値なし、グローバル関数、引数５つの関数用
		/// </summary>
		return NEW_() FunctionCallBackAsync_vg5< A1 , A2 , A3 , A4 , A5 , Arg1 , Arg2 , Arg3 , Arg4 , Arg5 >( f , a1 , a2 , a3 , a4 , a5 );	
	}

	template < class obj >
	static FunctionCallBackAsync_v* Create( void ( obj::* f )() , obj* o )
	{
		/// <summary>
		/// 戻り値なし、メンバ関数、引数なし
		/// </summary>
		return NEW_() FunctionCallBackAsync_vm0< obj >( f , o );
	}

	template < class obj , class Arg1 , class A1 >
	static FunctionCallBackAsync_v* Create( void ( obj::* f )( A1 ) , obj* o , const Arg1& a1 )
	{
		/// <summary>
		/// 戻り値なし、メンバ関数、引数１つの関数用
		/// </summary>
		return NEW_() FunctionCallBackAsync_vm1< obj , A1 , Arg1 >( f , o , a1 );
	}

	template < class obj , class Arg1 , class Arg2 , class A1 , class A2 >
	static FunctionCallBackAsync_v* Create( void ( obj::* f )( A1 , A2 ) , obj* o ,
		 const Arg1& a1 , const Arg2& a2 )
	{
		/// <summary>
		/// 戻り値なし、メンバ関数、引数２つの関数用
		/// </summary>
		return NEW_() FunctionCallBackAsync_vm2< obj , A1 , A2 , Arg1 , Arg2 >( f , o , a1 , a2 );
	}

	template < class obj , class Arg1 , class Arg2 , class Arg3 , class A1 , class A2 , class A3 >
	static FunctionCallBackAsync_v* Create( void ( obj::* f )( A1 , A2 , A3 ) , obj* o ,
		 const Arg1& a1 , const Arg2& a2 , const Arg3& a3 )
	{
		/// <summary>
		/// 戻り値なし、メンバ関数、引数３つの関数用
		/// </summary>
		return NEW_() FunctionCallBackAsync_vm3< obj , A1 , A2 , A3 , Arg1 , Arg2 , Arg3 >( f , o , a1 , a2 , a3 );
	}

	template < class obj , class Arg1 , class Arg2 , class Arg3 , class Arg4 , class A1 , class A2 , class A3 , class A4 >
	static FunctionCallBackAsync_v* Create( void ( obj::* f )( A1 , A2 , A3 , A4 ) ,
		obj* o , const Arg1& a1 , const Arg2& a2 , const Arg3& a3 , const Arg4& a4 )
	{
		/// <summary>
		/// 戻り値なし、メンバ関数、引数４つの関数用
		/// </summary>
		return NEW_() FunctionCallBackAsync_vm4< obj , A1 , A2 , A3 , A4 , Arg1 , Arg2 , Arg3 , Arg4 >( f , o , a1 , a2 , a3 , a4 );
	}

	template < class obj , class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5 ,
	class A1 , class A2 , class A3 , class A4 , class A5 >
	static FunctionCallBackAsync_v* Create( void ( obj::* f )( A1 , A2 , A3 , A4 , A5 ) ,
		obj* o , const Arg1& a1 , const Arg2& a2 , const Arg3& a3 , const Arg4& a4 , const Arg5& a5 )
	{
		/// <summary>
		/// 戻り値なし、メンバ関数、引数５つの関数用
		/// </summary>
		return NEW_() FunctionCallBackAsync_vm5< obj , A1 , A2 , A3 , A4 , A5 , Arg1 , Arg2 , Arg3 , Arg4 , Arg5 >( f , o , a1 , a2 , a3 , a4 , a5 );
	}
};

template < class Result , class A1 , class Arg1 >
class FunctionCallBackAsync_rg1;
template < class Result , class A1 , class A2 , class Arg1 , class Arg2 >
class FunctionCallBackAsync_rg2;
template < class Result , class A1 , class A2 , class A3 , class Arg1 , class Arg2 , class Arg3 >
class FunctionCallBackAsync_rg3;
template < class Result , class A1 , class A2 , class A3 , class A4 ,
    class Arg1 , class Arg2 , class Arg3 , class Arg4>
class FunctionCallBackAsync_rg4;
template < class Result , class A1 , class A2 , class A3 , class A4 , class A5 ,
    class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5 >
class FunctionCallBackAsync_rg5;
template < class Result , class ObjectType>
class FunctionCallBackAsync_rm0;
template < class Result , class ObjectType , class A1 , class Arg1 >
class FunctionCallBackAsync_rm1;
template < class Result , class ObjectType , class A1 , class A2 , class Arg1 , class Arg2 >
class FunctionCallBackAsync_rm2;
template < class Result , class ObjectType , class A1 , class A2 , class A3 ,
    class Arg1 , class Arg2 , class Arg3 >
class FunctionCallBackAsync_rm3;
template < class Result , class ObjectType , class A1 , class A2 , class A3 , class A4 ,
    class Arg1 , class Arg2 , class Arg3 , class Arg4 >
class FunctionCallBackAsync_rm4;
template < class Result , class ObjectType , class A1 , class A2 , class A3 , class A4 ,
    class A5 , class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5 >
class FunctionCallBackAsync_rm5;

/// <summary>
/// 戻りありのコールバック関数を作成するためのテンプレートクラス
/// </summary>
template < class Result >
class FunctionCallBackAsync_r : public FunctionCallBackAsync
{
public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~FunctionCallBackAsync_r(){}


	Result GetResult() const { return _r; }
	Result GetResultWait() const 
	{ 
		while( this->IsEnd() == false ){}
		return _r;
	}

	/// <summary>
	/// 戻り値あり、グローバル関数、引数なし
	/// </summary>
	static FunctionCallBackAsync_r* Create( Result ( * f )() );

	template < class Arg1 , class A1 >
	static FunctionCallBackAsync_r* Create( Result ( * f )( A1 ) , const Arg1& a1 )
	{
		/// <summary>
		/// 戻り値あり、グローバル関数、引数１つの関数用
		/// </summary>
		return NEW_() FunctionCallBackAsync_rg1< Result , A1 , Arg1 >( f , a1 );
	}

	template < class Arg1 , class Arg2 , class A1 , class A2 >
	static FunctionCallBackAsync_r* Create( Result ( * f )( A1 , A2 ) ,
		 const Arg1& a1 , const Arg2& a2 )
	{
		/// <summary>
		/// 戻り値あり、グローバル関数、引数２つの関数用
		/// </summary>
		return NEW_() FunctionCallBackAsync_rg2< Result, A1 , A2 , Arg1 , Arg2 >( f , a1 , a2 );
	}

	template < class Arg1 , class Arg2 , class Arg3 , class A1 , class A2 , class A3 >
	static FunctionCallBackAsync_r* Create( Result ( * f )( A1 , A2 , A3 ) ,
		 const Arg1& a1 , const Arg2& a2 , const Arg3& a3 )
	{
		/// <summary>
		/// 戻り値あり、グローバル関数、引数３つの関数用
		/// </summary>
		return NEW_() FunctionCallBackAsync_rg3< Result, A1 , A2 , A3 , Arg1 , Arg2 , Arg3 >( f , a1 , a2 );
	}

	template < class Arg1 , class Arg2 , class Arg3 , class Arg4 ,
		class A1 , class A2 , class A3 , class A4 >
	static FunctionCallBackAsync_r* Create( Result ( * f )( A1 , A2 , A3 , A4 ) ,
		 const Arg1& a1 , const Arg2& a2 , const Arg3& a3 , const Arg4& a4 )
	{
		/// <summary>
		/// 戻り値あり、グローバル関数、引数４つの関数用
		/// </summary>
		return NEW_() FunctionCallBackAsync_rg4< Result, A1 , A2 , A3 , A4 , Arg1 , Arg2 , Arg3 , Arg4 >( f , a1 , a2 , a3 , a4 );	}

	template < class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5,
		class A1 , class A2 , class A3 , class A4 , class A5 >
	static FunctionCallBackAsync_r* Create( Result ( * f )( A1 , A2 , A3 , A4 , A5 ) ,
		 const Arg1& a1 , const Arg2& a2 , const Arg3& a3 , const Arg4& a4 , const Arg5& a5 )
	{
		/// <summary>
		/// 戻り値あり、グローバル関数、引数５つの関数用
		/// </summary>
		return NEW_() FunctionCallBackAsync_rg5< Result, A1 , A2 , A3 , A4 , A5 , Arg1 , Arg2 , Arg3 , Arg4 , Arg5 >( f , a1 , a2 , a3 , a4 , a5 );
	}

	template < class obj >
	static FunctionCallBackAsync_r* Create( Result ( obj::* f )() , obj* o )
	{
		/// <summary>
		/// 戻り値あり、メンバ関数、引数０つの関数用
		/// </summary>
		return NEW_() FunctionCallBackAsync_rm0< Result, obj >( f , o );
	}

	template < class obj, class Arg1 , class A1 >
	static FunctionCallBackAsync_r* Create( Result ( obj::* f )( A1 ) , obj* o , const Arg1& a1 )
	{
		/// <summary>
		/// 戻り値あり、メンバ関数、引数１つの関数用
		/// </summary>
		return : NEW_() FunctionCallBackAsync_rm1< Result, obj, A1 , Arg1 >( f , o , a1 );
	}

	template < class obj, class Arg1 , class Arg2 , class A1 , class A2 >
	static FunctionCallBackAsync_r* Create( Result ( obj::* f )( A1 , A2 ) , obj* o ,
		 const Arg1& a1 , const Arg2& a2 )
	{
		/// <summary>
		/// 戻り値あり、メンバ関数、引数２つの関数用
		/// </summary>
		return NEW_() FunctionCallBackAsync_rm2< Result, obj, A1 , A2 , Arg1 , Arg2 >( f , o , a1 , a2 );
	}

	template < class obj, class Arg1 , class Arg2 , class Arg3 ,
		class A1 , class A2 , class A3 >
	static FunctionCallBackAsync_r* Create( Result ( obj::* f )( A1 , A2 , A3 ) , obj* o ,
		 const Arg1& a1 , const Arg2& a2 , const Arg3& a3 )
	{
		/// <summary>
		/// 戻り値あり、メンバ関数、引数３つの関数用
		/// </summary>
		return NEW_() FunctionCallBackAsync_rm3< Result, obj, A1 , A2 , A3 , Arg1 , Arg2 , Arg3 >( f , o , a1 , a2 , a3 );
	}

	template < class obj, class Arg1 , class Arg2 , class Arg3 , class Arg4 ,
		class A1 , class A2 , class A3 , class A4 >
	static FunctionCallBackAsync_r* Create( Result ( obj::* f )( A1 , A2 , A3 , A4 ) ,
		obj* o , const Arg1& a1 , const Arg2& a2 , const Arg3& a3 , const Arg4& a4 )
	{
		/// <summary>
		/// 戻り値あり、メンバ関数、引数４つの関数用
		/// </summary>
		return NEW_() FunctionCallBackAsync_rm4< Result, obj, A1 , A2 , A3 , A4 , Arg1 , Arg2 , Arg3 , Arg4 >( f , o , a1 , a2 , a3 , a4 );
	}

	template < class obj, class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5,
		class A1 , class A2 , class A3 , class A4 , class A5 >
	static FunctionCallBackAsync_r* Create( Result ( obj::* f )( A1 , A2 , A3 , A4 , A5 ) ,
		obj* o , const Arg1& a1 , const Arg2& a2 , const Arg3& a3 , const Arg4& a4 , const Arg5& a5 )
	{
		/// <summary>
		/// 戻り値あり、メンバ関数、引数５つの関数用
		/// </summary>
		return NEW_() FunctionCallBackAsync_rm5< Result, obj, A1 , A2 , A3 , A4 , A5 , Arg1 , Arg2 , Arg3 , Arg4 , Arg5 >( f , o , a1 , a2 , a3 , a4 , a5 );
	}

protected:

	Result _r;
};

/// <summary>
/// 戻り値なし、グローバル関数、引数なし関数用
/// </summary>
class FunctionCallBackAsync_vg0 : public FunctionCallBackAsync_v
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_vg0(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef void ( * function_type )();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_vg0( function_type f ) : _f( f ){ }

		/// <summary>
		/// コールバック実行
		/// </summary>
		virtual void run(){ _f(); }

	protected:

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;
};

/// <summary>
/// 戻り値なし、グローバル関数、引数１つの関数用
/// </summary>
template < class A1 , class Arg1 >
class FunctionCallBackAsync_vg1 : public FunctionCallBackAsync_v
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_vg1(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef void ( * function_type ) ( A1 );

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_vg1( function_type f , Arg1 a1 ) : _f( f ) , _a1( a1 ) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
		virtual void run(){ _f( _a1 ); m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;

		/// <summary>
		/// 引数
		/// </summary>
		Arg1 _a1;
};

/// <summary>
/// 戻り値なし、グローバル関数、引数２つの関数用
/// </summary>
template < class A1 , class A2 , class Arg1 , class Arg2 >
class FunctionCallBackAsync_vg2 : public FunctionCallBackAsync_v
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_vg2(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef void ( * function_type ) ( A1 , A2 );

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_vg2( function_type f , Arg1 a1 , Arg2 a2 )
			: _f( f ) , _a1( a1 ) , _a2( a2 ) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
		virtual void run(){ _f( _a1 , _a2 ); m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;

		/// <summary>
		/// 引数
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
};

/// <summary>
/// 戻り値なし、グローバル関数、引数３つの関数用
/// </summary>
template < class A1 , class A2 , class A3 , class Arg1 , class Arg2 , class Arg3 >
class FunctionCallBackAsync_vg3 : public FunctionCallBackAsync_v
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_vg3(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef void ( * function_type ) ( A1 , A2 , A3 );

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_vg3( function_type f , Arg1 a1 , Arg2 a2 , Arg3 a3 )
			: _f( f ) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
		virtual void run(){ _f( _a1 , _a2 , _a3 ); m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;

		/// <summary>
		/// 引数
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
};

/// <summary>
/// 戻り値なし、グローバル関数、引数４つの関数用
/// </summary>
template < class A1 , class A2 , class A3 , class A4 ,
	class Arg1 , class Arg2 , class Arg3 , class Arg4>
class FunctionCallBackAsync_vg4 : public FunctionCallBackAsync_v
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_vg4(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef void ( * function_type ) ( A1 , A2 , A3 , A4 );

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_vg4( function_type f , Arg1 a1 , Arg2 a2 , Arg3 a3 , Arg4 a4 )
			: _f( f ) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) , _a4( a4 ) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
		virtual void run(){ _f( _a1 , _a2 , _a3 , _a4 ); m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
		Arg4 _a4;
};

/// <summary>
/// 戻り値なし、グローバル関数、引数５つの関数用
/// </summary>
template < class A1 , class A2 , class A3 , class A4 , class A5,
	class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5 >
class FunctionCallBackAsync_vg5 : public FunctionCallBackAsync_v
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_vg5(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef void ( * function_type ) ( A1 , A2 , A3 , A4 , A5 );

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_vg5( function_type f , Arg1 a1 , Arg2 a2 , Arg3 a3
			, Arg4 a4 , Arg5 a5 )
			: _f( f ) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) , _a4( a4 ) , _a5( a5 ) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
		virtual void run(){ _f( _a1 , _a2 , _a3 , _a4 , _a5 ); m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
		Arg4 _a4;
		Arg5 _a5;
};

/// <summary>
/// 戻り値あり、グローバル関数、引数なし関数用
/// </summary>
template < class Result>
class FunctionCallBackAsync_rg0 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_rg0(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef Result ( * function_type ) ();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_rg0( function_type f) : _f( f ) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
        virtual void run(){ this->_r = _f(); this->m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;
};

/// <summary>
/// 戻り値あり、グローバル関数、引数１つ関数用
/// </summary>
template < class Result , class A1 , class Arg1 >
class FunctionCallBackAsync_rg1 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_rg1(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef Result ( * function_type ) ( A1 );

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_rg1( function_type f , Arg1 a1 ) : _f( f ) , _a1( a1 ) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
        virtual void run(){ this->_r = _f( _a1 ); this->m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;
		Arg1 _a1;
};

/// <summary>
/// 戻り値あり、グローバル関数、引数２つ関数用
/// </summary>
template < class Result , class A1 , class A2 , class Arg1 , class Arg2 >
class FunctionCallBackAsync_rg2 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_rg2(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef Result ( * function_type ) ( A1 , A2 );

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_rg2( function_type f , Arg1 a1 , Arg2 a2 )
			: _f( f ) , _a1( a1 ) , _a2( a2 ) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
        virtual void run(){ this->_r = _f( _a1 , _a2 ); this->m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;

		/// <summary>
		/// 引数
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
};

/// <summary>
/// 戻り値あり、グローバル関数、引数３つ関数用
/// </summary>
template < class Result , class A1 , class A2 , class A3 , class Arg1 , class Arg2 , class Arg3 >
class FunctionCallBackAsync_rg3 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_rg3(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef Result ( * function_type ) ( A1 , A2 , A3 );

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_rg3( function_type f , Arg1 a1 , Arg2 a2 , Arg3 a3 )
			: _f( f ) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
        virtual void run(){ this->_r=_f( _a1 , _a2 , _a3 ); this->m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;

		/// <summary>
		/// 引数
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
};

/// <summary>
/// 戻り値あり、グローバル関数、引数４つ関数用
/// </summary>
template < class Result , class A1 , class A2 , class A3 , class A4 ,
	class Arg1 , class Arg2 , class Arg3 , class Arg4>
class FunctionCallBackAsync_rg4 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_rg4(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef Result ( * function_type ) ( A1 , A2 , A3 , A4 );

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_rg4( function_type f , Arg1 a1 , Arg2 a2 , Arg3 a3 , Arg4 a4 )
			: _f( f ) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) , _a4( a4 ) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
        virtual void run(){ this->_r=_f( _a1 , _a2 , _a3 , _a4 ); this->m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;

		/// <summary>
		/// 引数
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
		Arg4 _a4;
};

/// <summary>
/// 戻り値あり、グローバル関数、引数５つ関数用
/// </summary>
template < class Result , class A1 , class A2 , class A3 , class A4 , class A5 ,
	class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5 >
class FunctionCallBackAsync_rg5 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_rg5(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef Result ( * function_type ) ( A1 , A2 , A3 , A4 , A5 );

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_rg5( function_type f , Arg1 a1 , Arg2 a2 , Arg3 a3
			, Arg4 a4 , Arg5 a5 )
			: _f( f ) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) , _a4( a4 ) , _a5( a5 ) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
        virtual void run(){ this->_r=_f( _a1 , _a2 , _a3 , _a4 , _a5 ); this->m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;

		/// <summary>
		/// 引数
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
		Arg4 _a4;
		Arg5 _a5;
};

/// <summary>
/// 戻り値なし、メンバ関数、引数なし関数用
/// </summary>
template < class ObjectType>
class FunctionCallBackAsync_vm0 : public FunctionCallBackAsync_v
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_vm0(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef void ( ObjectType::* function_type ) ();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_vm0( function_type f ,ObjectType*o) : _f( f ) , _o(o) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
		virtual void run(){ ( _o->*_f)(); m_IsEnd = skyTrue; }

	protected:

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }
		ObjectType* _o;	//	こいつが先頭でないと書き換えできない
		function_type _f;
};

/// <summary>
/// 戻り値なし、メンバ関数、引数１つ関数用
/// </summary>
template < class ObjectType , class A1 , class Arg1 >
class FunctionCallBackAsync_vm1 : public FunctionCallBackAsync_v
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_vm1(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef void ( ObjectType::* function_type ) ( A1 );

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_vm1( function_type f ,ObjectType*o , Arg1 a1 )
			: _f( f ) , _o(o) , _a1( a1 ) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
		virtual void run(){ ( _o->*_f)( _a1 ); m_IsEnd = skyTrue; }

	protected:

		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }

		/// <summary>
		/// コールバックをする関数が所属するオブジェクト
		/// </summary>
		ObjectType* _o;

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;

		/// <summary>
		/// 引数
		/// </summary>
		Arg1 _a1;
};

/// <summary>
/// 戻り値なし、メンバ関数、引数２つ関数用
/// </summary>
template < class ObjectType , class A1 , class A2 , class Arg1 , class Arg2 >
class FunctionCallBackAsync_vm2 : public FunctionCallBackAsync_v
{

	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_vm2(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef void ( ObjectType::* function_type ) ( A1 , A2 );

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_vm2( function_type f ,ObjectType*o , Arg1 a1 , Arg2 a2 )
			: _f( f ) , _o(o) , _a1( a1 ) , _a2( a2 ) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
		virtual void run(){ ( _o->*_f)( _a1 , _a2 ); m_IsEnd = skyTrue; }

	protected:

		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }

		/// <summary>
		/// コールバックをする関数が所属するオブジェクトの参照
		/// </summary>
		ObjectType* _o;

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;

		/// <summary>
		/// 引数
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
};

/// <summary>
/// 戻り値なし、メンバ関数、引数３つ関数用
/// </summary>
template < class ObjectType , class A1 , class A2 , class A3 ,
	class Arg1 , class Arg2 , class Arg3 >
class FunctionCallBackAsync_vm3 : public FunctionCallBackAsync_v
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_vm3(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef void ( ObjectType::* function_type ) ( A1 , A2 , A3 );

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_vm3( function_type f ,ObjectType*o , Arg1 a1 , Arg2 a2 , Arg3 a3 )
			: _f( f ) , _o(o) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
		virtual void run(){ ( _o->*_f)( _a1 , _a2 , _a3 ); m_IsEnd = skyTrue; }

	protected:

		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }

		/// <summary>
		/// コールバックをする関数が所属するオブジェクトの参照
		/// </summary>
		ObjectType* _o;

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;

		/// <summary>
		/// 引数
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
};

/// <summary>
/// 戻り値なし、メンバ関数、引数４つ関数用
/// </summary>
template < class ObjectType , class A1 , class A2 , class A3 , class A4 ,
	class Arg1 , class Arg2 , class Arg3 , class Arg4 >
class FunctionCallBackAsync_vm4 : public FunctionCallBackAsync_v
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_vm4(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef void ( ObjectType::* function_type ) ( A1 , A2 , A3 , A4 );

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_vm4( function_type f ,ObjectType*o , Arg1 a1 , Arg2 a2
			, Arg3 a3 , Arg4 a4 )
			: _f( f ) , _o(o) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) , _a4( a4 ) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
		virtual void run(){ ( _o->*_f)( _a1 , _a2 , _a3 , _a4 ); m_IsEnd = skyTrue; }

	protected:

		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }

		/// <summary>
		/// コールバックをする関数が所属するオブジェクトの参照
		/// </summary>
		ObjectType* _o;

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;

		/// <summary>
		/// 引数
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
		Arg4 _a4;
};

/// <summary>
/// 戻り値なし、メンバ関数、引数５つ関数用
/// </summary>
template < class ObjectType , class A1 , class A2 , class A3 , class A4 , class A5,
	class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5 >
class FunctionCallBackAsync_vm5 : public FunctionCallBackAsync_v
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_vm5(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef void ( ObjectType::* function_type ) ( A1 , A2 , A3 , A4 , A5 );

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_vm5( function_type f ,ObjectType*o , Arg1 a1 , Arg2 a2
			, Arg3 a3 , Arg4 a4 , Arg5 a5 )
			: _f( f ) , _o(o) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) , _a4( a4 ) , _a5( a5 ) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
		virtual void run(){ ( _o->*_f)( _a1 , _a2 , _a3 , _a4 , _a5 ); m_IsEnd = skyTrue; }

	protected:

		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }

		/// <summary>
		/// コールバックをする関数が所属するオブジェクトの参照
		/// </summary>
		ObjectType* _o;

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;

		/// <summary>
		/// 引数
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
		Arg4 _a4;
		Arg5 _a5;
};

/// <summary>
/// 戻り値あり、メンバ関数、引数なし関数用
/// </summary>
template < class Result , class ObjectType>
class FunctionCallBackAsync_rm0 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_rm0(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef Result ( ObjectType::* function_type ) ();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_rm0( function_type f ,ObjectType*o) : _f( f ) , _o(o) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
        virtual void run(){ this->_r = ( _o->*_f)(); this->m_IsEnd = skyTrue; }

	protected:

		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }

		/// <summary>
		/// コールバックをする関数が所属するオブジェクトの参照
		/// </summary>
		ObjectType* _o;

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;
};

/// <summary>
/// 戻り値あり、メンバ関数、引数１つ関数用
/// </summary>
template < class Result , class ObjectType , class A1 , class Arg1 >
class FunctionCallBackAsync_rm1 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_rm1(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef Result ( ObjectType::* function_type ) ( A1 );

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_rm1( function_type f ,ObjectType*o , Arg1 a1 )
			: _f( f ) , _o(o) , _a1( a1 ) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
        virtual void run(){ this->_r = ( _o->*_f)( _a1 ); this->m_IsEnd = skyTrue; }

	protected:

		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }

		/// <summary>
		/// コールバックをする関数が所属するオブジェクトの参照
		/// </summary>
		ObjectType* _o;

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;

		/// <summary>
		/// 引数
		/// </summary>
		Arg1 _a1;
};

/// <summary>
/// 戻り値あり、メンバ関数、引数２つ関数用
/// </summary>
template < class Result , class ObjectType , class A1 , class A2 , class Arg1 , class Arg2 >
class FunctionCallBackAsync_rm2 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_rm2(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef Result ( ObjectType::* function_type ) ( A1 , A2 );

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_rm2( function_type f ,ObjectType*o , Arg1 a1 , Arg2 a2 )
			: _f( f ) , _o(o) , _a1( a1 ) , _a2( a2 ) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
        virtual void run(){ this->_r = ( _o->*_f)( _a1 , _a2 ); this->m_IsEnd = skyTrue; }

	protected:

		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }

		/// <summary>
		/// コールバックをする関数が所属するオブジェクトの参照
		/// </summary>
		ObjectType* _o;

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;

		/// <summary>
		/// 引数
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
};

/// <summary>
/// 戻り値あり、メンバ関数、引数３つ関数用
/// </summary>
template < class Result , class ObjectType , class A1 , class A2 , class A3 ,
	class Arg1 , class Arg2 , class Arg3 >
class FunctionCallBackAsync_rm3 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_rm3(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef Result ( ObjectType::* function_type ) ( A1 , A2 , A3 );

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_rm3( function_type f ,ObjectType*o , Arg1 a1 , Arg2 a2 , Arg3 a3 )
			: _f( f ) , _o(o) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
        virtual void run(){ this->_r=( _o->*_f)( _a1 , _a2 , _a3 ); this->m_IsEnd = skyTrue; }

	protected:

		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }

		/// <summary>
		/// コールバックをする関数が所属するオブジェクトの参照
		/// </summary>
		ObjectType* _o;

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;

		/// <summary>
		/// 引数
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
};

/// <summary>
/// 戻り値あり、メンバ関数、引数４つ関数用
/// </summary>
template < class Result , class ObjectType , class A1 , class A2 , class A3 , class A4 ,
	class Arg1 , class Arg2 , class Arg3 , class Arg4 >
class FunctionCallBackAsync_rm4 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_rm4(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef Result ( ObjectType::* function_type ) ( A1 , A2 , A3 , A4 );

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_rm4( function_type f ,ObjectType*o ,
			Arg1 a1 , Arg2 a2 , Arg3 a3 , Arg4 a4 )
			: _f( f ) , _o(o) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) , _a4( a4 ) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
        virtual void run(){ this->_r=( _o->*_f)( _a1 , _a2 , _a3 , _a4 ); this->m_IsEnd = skyTrue; }

	protected:

		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }

		/// <summary>
		/// コールバックをする関数が所属するオブジェクトの参照
		/// </summary>
		ObjectType* _o;

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;

		/// <summary>
		/// 引数
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
		Arg4 _a4;
};

/// <summary>
/// 戻り値あり、メンバ関数、引数５つ関数用
/// </summary>
template < class Result , class ObjectType , class A1 , class A2 , class A3 , class A4 ,
	class A5 , class Arg1 , class Arg2 , class Arg3 , class Arg4 , class Arg5 >
class FunctionCallBackAsync_rm5 : public FunctionCallBackAsync_r< Result >
{
	public:

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~FunctionCallBackAsync_rm5(){}

		/// <summary>
		/// コールバックをする関数の型
		/// </summary>
		typedef Result ( ObjectType::* function_type ) ( A1 , A2 , A3 , A4 , A5 );

		/// <summary>
		/// コンストラクタ
		/// </summary>
		FunctionCallBackAsync_rm5( function_type f ,ObjectType*o ,
			Arg1 a1 , Arg2 a2 , Arg3 a3 , Arg4 a4 , Arg5 a5 )
			: _f( f ) , _o(o) , _a1( a1 ) , _a2( a2 ) , _a3( a3 ) , _a4( a4 ) , _a5( a5 ) {}

		/// <summary>
		/// コールバック実行
		/// </summary>
        virtual void run(){ this->_r=( _o->*_f)( _a1 , _a2 , _a3 , _a4 , _a5 ); this->m_IsEnd = skyTrue; }

	protected:

		virtual void inner_bind( void * p ){ _o = ( ObjectType*)p; }

		/// <summary>
		/// コールバックをする関数が所属するオブジェクトの参照
		/// </summary>
		ObjectType* _o;

		/// <summary>
		/// コールバックをする関数
		/// </summary>
		function_type _f;

		/// <summary>
		/// 引数
		/// </summary>
		Arg1 _a1;
		Arg2 _a2;
		Arg3 _a3;
		Arg4 _a4;
		Arg5 _a5;
};

} }
