#pragma once

namespace sky { namespace lib {

//! boost::function + bind のように使える関数オブジェクト
/*
	使い方
	
	#include "SkyBaseLib/SKYTL.h"
	
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

void TEST() 
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
	s32 iResult = kFunction2( 5, 10 );

	SKY_BASE_LIB_TERM();
}

*/

#define SKY_TEMPLATE_PARAMS_1  class T0
#define SKY_TEMPLATE_PARAMS_2  class T0, class T1
#define SKY_TEMPLATE_PARAMS_3  class T0, class T1, class T2
#define SKY_TEMPLATE_PARAMS_4  class T0, class T1, class T2, class T3
#define SKY_TEMPLATE_PARAMS_5  class T0, class T1, class T2, class T3, class T4
#define SKY_TEMPLATE_PARAMS_6  class T0, class T1, class T2, class T3, class T4, class T5
#define SKY_TEMPLATE_PARAMS_7  class T0, class T1, class T2, class T3, class T4, class T5, class T6

#define SKY_TEMPLATE_ARGS_1   T0
#define SKY_TEMPLATE_ARGS_2   T0, T1
#define SKY_TEMPLATE_ARGS_3   T0, T1, T2
#define SKY_TEMPLATE_ARGS_4   T0, T1, T2, T3
#define SKY_TEMPLATE_ARGS_5   T0, T1, T2, T3, T4
#define SKY_TEMPLATE_ARGS_6   T0, T1, T2, T3, T4, T5
#define SKY_TEMPLATE_ARGS_7   T0, T1, T2, T3, T4, T5, T6

#define SKY_FUNCTION_PARAMS_1  T0 t0
#define SKY_FUNCTION_PARAMS_2  T0 t0, T1 t1
#define SKY_FUNCTION_PARAMS_3  T0 t0, T1 t1, T2 t2
#define SKY_FUNCTION_PARAMS_4  T0 t0, T1 t1, T2 t2, T3 t3
#define SKY_FUNCTION_PARAMS_5  T0 t0, T1 t1, T2 t2, T3 t3, T4 t4
#define SKY_FUNCTION_PARAMS_6  T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5
#define SKY_FUNCTION_PARAMS_7  T0 t0, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6

#define SKY_FUNCTION_ARGS_1   t0
#define SKY_FUNCTION_ARGS_2   t0, t1
#define SKY_FUNCTION_ARGS_3   t0, t1, t2
#define SKY_FUNCTION_ARGS_4   t0, t1, t2, t3
#define SKY_FUNCTION_ARGS_5   t0, t1, t2, t3, t4
#define SKY_FUNCTION_ARGS_6   t0, t1, t2, t3, t4, t5
#define SKY_FUNCTION_ARGS_7   t0, t1, t2, t3, t4, t5, t6

#define SKY_FUNCTION_BUFFER_SIZE	32

#define SKY_FUNCTION_CLASS_N(Count)											\
template <class R, SKY_TEMPLATE_PARAMS_##Count>								\
class Function<R (SKY_TEMPLATE_ARGS_##Count)> : public base::SkyRefObject {	\
private:																	\
	typedef R result_type;													\
	typedef result_type (*Func)( SKY_FUNCTION_PARAMS_##Count );				\
																			\
private:																	\
	class Invoker {															\
	public:																	\
		virtual ~Invoker() {}												\
		virtual result_type operator()( SKY_FUNCTION_PARAMS_##Count ) = 0;	\
		virtual Invoker* Clone( skyString* buffer, u32 size ) = 0;		\
	};																		\
																			\
	template< typename T >													\
	class MemFunc : public Invoker {										\
	private:																\
		typedef result_type( T::*Func )( SKY_FUNCTION_PARAMS_##Count );		\
																			\
	private:																\
		T*		client_;													\
		Func	func_;														\
																			\
	public:																	\
		MemFunc( T* client, Func func ) :									\
			client_	( client ),												\
			func_	( func )												\
		{}																	\
		virtual result_type operator()( SKY_FUNCTION_PARAMS_##Count ) {		\
			return ( client_->*func_ )( SKY_FUNCTION_ARGS_##Count );		\
		}																	\
		virtual Invoker* Clone( skyString* buffer, u32 size ) {				\
			SKY_UNUSED_ARG( size );												\
			SKY_ASSERT( sizeof( MemFunc< T > ) < size );					\
			return new( buffer ) MemFunc< T >( client_, func_ );			\
		}																	\
	};																		\
																			\
	class StdFunc : public Invoker {										\
	private:																\
		Func	func_;														\
																			\
	public:																	\
		StdFunc( Func func ) : func_( func ) {}								\
		virtual result_type operator()( SKY_FUNCTION_PARAMS_##Count ) {		\
			return func_( SKY_FUNCTION_ARGS_##Count );						\
		}																	\
		virtual Invoker* Clone( skyString* buffer, u32 size ) {				\
			SKY_UNUSED_ARG( size );												\
			SKY_ASSERT( sizeof( StdFunc ) < size );							\
			return new( buffer ) StdFunc( func_ );							\
		}																	\
	};																		\
																			\
private:																	\
	Invoker*	invoker_;													\
	skyString	buffer_[ SKY_FUNCTION_BUFFER_SIZE ];							\
																			\
public:																		\
	Function() : invoker_( 0 ) {}											\
																			\
	Function( const Function& other ) {										\
		Reset( other );														\
	}																		\
																			\
	Function( Func func ) {													\
		Reset( func );														\
	}																		\
																			\
	bool operator == ( const Function& other ) const {						\
		return this == &other;												\
	}																		\
	bool operator != ( const Function& other ) const {						\
		return !( *this == other );											\
	}																		\
																			\
	template< typename T, typename F >										\
	Function( T* t, F func ) {												\
		Reset( t, func );													\
	}																		\
																			\
	~Function() {}															\
																			\
	const Function& operator=( const Function& other ) {					\
		if ( this != &other ) {												\
			Reset( other );													\
		}																	\
		return *this;														\
	}																		\
																			\
	const Function& operator=( Func func ) {								\
		Reset( func );														\
		return *this;														\
	}																		\
																			\
	result_type operator()(SKY_FUNCTION_PARAMS_##Count) {					\
		if ( invoker_ ) {													\
			return (*invoker_)( SKY_FUNCTION_ARGS_##Count );					\
		}																	\
		return result_type();												\
	}																		\
																			\
	void Reset( const Function& other ) {									\
		if ( other.invoker_ ) {												\
			invoker_ = other.invoker_->Clone( buffer_, sizeof( buffer_ ) );	\
		} else {															\
			invoker_ = 0;													\
		}																	\
	}																		\
																			\
	void Reset( Func func = 0 ) {											\
		if ( func ) {														\
			SKY_ASSERT( sizeof( StdFunc ) < sizeof( buffer_ ) );				\
			invoker_ = new( buffer_ ) StdFunc( func );						\
		} else {															\
			invoker_ = 0;													\
		}																	\
	}																		\
																			\
	template< typename T, typename F >										\
	void Reset( T* t, F func ) {											\
		SKY_ASSERT( t != 0 && func != 0 );									\
		SKY_ASSERT( sizeof( MemFunc< T > ) < sizeof( buffer_ ) );			\
		invoker_ = new( buffer_ ) MemFunc< T >( t, func );					\
	}																		\
																			\
	bool Empty() const {													\
		return !invoker_;													\
	}																		\
};																			\

template <class Signature>
class Function;
	
//! 引数無し用に特殊化
template <class R>
class Function< R (void)> : public base::SkyRefObject
{
private:
	typedef R result_type;
	typedef result_type (*Func)();
		
private:
	class Invoker {
	public:
		virtual ~Invoker() {}
		virtual result_type operator()() = 0;
		virtual Invoker* Clone( skyString* buffer, u32 size ) = 0;
	};
		
	template< typename T >
	class MemFunc : public Invoker {
	private:
		typedef result_type( T::*Func )();
			
	private:
		T*		client_;
		Func	func_;
			
	public:
		MemFunc( T* client, Func func ) :
			client_	( client ),
			func_	( func )
		{}
		virtual result_type operator()() {
			return ( client_->*func_ )();
		}
		virtual Invoker* Clone( skyString* buffer, u32 size ) {
			SKY_ASSERT( sizeof( MemFunc< T > ) < size );
			return new( buffer ) MemFunc< T >( client_, func_ );
		}
	};
		
	class StdFunc : public Invoker {
	private:
		Func	func_;
			
	public:
		StdFunc( Func func ) : func_( func ) {}
		virtual result_type operator()() {
			return func_();
		}
		virtual Invoker* Clone( skyString* buffer, u32 size ) {
			SKY_UNUSED_ARG( size );
			SKY_ASSERT( sizeof( StdFunc ) < size );
			return new( buffer ) StdFunc( func_ );
		}
	};
		
private:
	Invoker*	invoker_;
	skyString		buffer_[ SKY_FUNCTION_BUFFER_SIZE ];
		
public:
	Function() : invoker_( 0 ) {}
		
	Function( const Function& other ) {
		Reset( other );
	}
		
	Function( Func func ) {
		Reset( func );
	}
		
	template< typename T, typename F >
	Function( T* t, F func ) {
		Reset( t, func );
	}
		
	~Function() {}
		
	bool operator == ( const Function& other ) const {
		return this == &other;
	}
	bool operator != ( const Function& other ) const {
		return !( *this == other );
	}
		
	const Function& operator=( const Function& other ) {
		if ( this != &other ) {
			Reset( other );
		}
		return *this;
	}
		
	const Function& operator=( Func func ) {
		Reset( func );
		return *this;
	}
		
	result_type operator()() {
		if ( invoker_ ) {
			return (*invoker_)();
		}
		return result_type();
	}
		
	void Reset( const Function& other ) {
		if ( other.invoker_ ) {
			invoker_ = other.invoker_->Clone( buffer_, sizeof( buffer_ ) );
		} else {
			invoker_ = 0;
		}
	}
		
	void Reset( Func func = 0 ) {
		if ( func ) {
			SKY_ASSERT( sizeof( StdFunc ) < sizeof( buffer_ ) );
			invoker_ = new( buffer_ ) StdFunc( func );
		} else {
			invoker_ = 0;
		}
	}
		
	template< typename T, typename F >
	void Reset( T* t, F func ) {
		SKY_ASSERT( t != 0 && func != 0 );
		SKY_ASSERT( sizeof( MemFunc< T > ) < sizeof( buffer_ ) );
		invoker_ = new( buffer_ ) MemFunc< T >( t, func );
	}
		
	bool Empty() const {
		return !invoker_;
	}
};
	
SKY_FUNCTION_CLASS_N( 1 )
SKY_FUNCTION_CLASS_N( 2 )
SKY_FUNCTION_CLASS_N( 3 )
SKY_FUNCTION_CLASS_N( 4 )
SKY_FUNCTION_CLASS_N( 5 )
SKY_FUNCTION_CLASS_N( 6 )
SKY_FUNCTION_CLASS_N( 7 )

#undef SKY_TEMPLATE_PARAMS_1
#undef SKY_TEMPLATE_PARAMS_2
#undef SKY_TEMPLATE_PARAMS_3
#undef SKY_TEMPLATE_PARAMS_4
#undef SKY_TEMPLATE_PARAMS_5
#undef SKY_TEMPLATE_PARAMS_6
#undef SKY_TEMPLATE_PARAMS_7

#undef SKY_TEMPLATE_ARGS_1
#undef SKY_TEMPLATE_ARGS_2
#undef SKY_TEMPLATE_ARGS_3
#undef SKY_TEMPLATE_ARGS_4
#undef SKY_TEMPLATE_ARGS_5
#undef SKY_TEMPLATE_ARGS_6
#undef SKY_TEMPLATE_ARGS_7

#undef SKY_FUNCTION_PARAMS_1
#undef SKY_FUNCTION_PARAMS_2
#undef SKY_FUNCTION_PARAMS_3
#undef SKY_FUNCTION_PARAMS_4
#undef SKY_FUNCTION_PARAMS_5
#undef SKY_FUNCTION_PARAMS_6
#undef SKY_FUNCTION_PARAMS_7

#undef SKY_FUNCTION_ARGS_1
#undef SKY_FUNCTION_ARGS_2
#undef SKY_FUNCTION_ARGS_3
#undef SKY_FUNCTION_ARGS_4
#undef SKY_FUNCTION_ARGS_5
#undef SKY_FUNCTION_ARGS_6
#undef SKY_FUNCTION_ARGS_7

#undef SKY_FUNCTION_CLASS_N

#undef SKY_FUNCTION_BUFFER_SIZE

} } 
