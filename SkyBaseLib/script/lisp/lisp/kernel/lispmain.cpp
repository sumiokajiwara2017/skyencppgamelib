#include "StdAfx.h"
#include "../include/lisp.h"

#ifdef SW_SKYLIB_SCRIPT_USE_LISP

#include "../include/lisp.h"

namespace sky { namespace lib {

/// <summary>
/// 帰り先格納変数
/// </summary>
static jmp_buf long_jump_env;

/// <summary>
/// 定数群
/// </summary>
#define CONS_MAX			10000
#define SYMBOL_MAX			1000
#define SYMBOL_NAME_MAX		255
#define PRINT_MAX			1000
#define READ_MAX			1000

/// <summary>
/// INTかどうか判定している
/// </summary>
#define IS_INT( n )			( ( ( int )n & 0x00000001 ) != 0 )

/// <summary>
/// シンボルかどうか判定している
/// </summary>
#define IS_SYMBOL( e )		( ( symbol_t * )e >= &symbol[ 0 ] && ( symbol_t * )e < &symbol[ symbol_pt ] )	//これは・・ポインタの範囲でsymbolだと判定している！凄い。

/// <summary>
/// コンスかどうか判定している
/// </summary>
#define IS_CONS( e )		( ( cons_t * )e >= &cons_cell[ 0 ] && ( cons_t * )e < &cons_cell[ cons_pt ] )	//これは・・ポインタの範囲でsymbolだと判定している！凄い。

/// <summary>
/// CELLをINTに変換している
/// </summary>
#define CELL2INT( e )		( ( int )e >> 1 )
#define INT2CELL( n )       ( ( cell )( ( n << 1 ) + 1 ) )

/// <summary>
/// 終わりを表すNIL
/// </summary>
#define NIL          ( ( cell ) 0 )

/// <summary>
/// カーを簡単に取り出せるマクロ
/// </summary>
#define CAR( e )     ( ( cons_t * ) e )->car_e

/// <summary>
/// クダーを簡単に取り出せるマクロ
/// </summary>
#define CDR( e )     ( ( cons_t * ) e )->cdr_e

#define CAAR( e )       CAR( CAR( e ) )
#define CDDR( e )       CDR( CDR( e ) )
#define CADR( e )       CAR( CDR( e ) )
#define CDDR( e )       CDR( CDR( e ) )
#define CADDR( e )      CAR( CDR( CDR( e ) ) )
#define CADDDR( e )     CAR( CDR( CDR( CDR( e ) ) ) )
#define CDAR(e)			CDR(CAR(e))

//プロトタイプ
void test();

/// <summary>
/// シンボル名を簡単に取り出せるマクロ
/// </summary>
#define SYMBOL_NAME( e ) ( ( ( symbol_t * ) e )->name )
#define SYMBOL2CELL( s ) ( ( cell ) s )
#define SPECIAL_TOKENS		"().'"
#define LOAD_FILE_ESCAPE	"\\f "
#define DEFAULT_FILE_EXT	".lisp"

/// <summary>
/// セル
/// </summary>
typedef void* cell;

/// <summary>
/// コンス（Lispプログラムの最小単位ぽい）
/// </summary>
typedef struct
{
  cell car_e; //カー
  cell cdr_e; //クダー
} cons_t;

static cell top_env;
static cell t_sym, func_sym, sform_sym, lambda_sym, macro_sym, quote_sym;

/// <summary>
/// コンスの集合。Lispプログラムのメモリのようなものぽい。ここが尽きたら多分終わり
/// </summary>
static cons_t	cons_cell[ CONS_MAX ];
static int		cons_pt = 0;

/// <summary>
/// シンボル名の器
/// </summary>
typedef struct
{
  char name[ SYMBOL_NAME_MAX + 1 ];
} symbol_t;

/// <summary>
/// シンボルの集合
/// </summary>
static symbol_t symbol[ SYMBOL_MAX ];
static int symbol_pt = 0;

/// <summary>
/// トークン
/// </summary>
typedef struct
{
	char kind;
	char *name;
	int  number;
} token_t;

//========================< エラー制御 >=======================================▼
/// <summary>
/// 強制終了
/// </summary>
void panic( char *msg )
{
	printf( "%s\n" , msg );
	exit( 1 );
}
/// <summary>
/// 強制終了
/// </summary>
static char error_message[ 256 ];
void throw_error( char *fmt , ... )
{
	va_list ap;
	va_start( ap , fmt );
	vsprintf( error_message , fmt , ap );
	va_end( ap );
	longjmp( long_jump_env , 1 );
}


//========================< ガベージコレクション >========================▼
/// <summary>
/// ガベージコレクション
/// </summary>
void gc()
{
}

//========================< メモリ割り当て >==============================▼
/// <summary>
/// ノードを割り当てる
/// </summary>
cell cons( cell car_a , cell cdr_a )
{
	//コンスを使い切ったら終わり
	if ( cons_pt >= CONS_MAX )  gc();

	//配列から未使用のコンスを取り出す。
	cell new_cons = &cons_cell[ cons_pt ];

	//カウンタを次の未使用領域へ
	cons_pt++;

	//カーとクダーを設定する
	CAR( new_cons ) = car_a;
	CDR( new_cons ) = cdr_a;

	return new_cons;
}

//========================< 機能関数（最初から入っている） >=======================================▼

cell car( cell e )
{
	if ( e == NIL )  return  NIL;
	return  CAR( e );
}

cell cdr( cell e )
{
	if ( e == NIL )  return  NIL;
	return  CDR( e );
}

cell concat2( cell a , cell b )
{
	if ( a == NIL )  return b;
	if ( b == NIL )  return a;

	cell p = a;
	while ( CDR( p ) != NIL )  p = CDR( p );
	CDR( p ) = b;

	return a;
}

cell append2(cell a, cell b)
{
	if ( a == NIL )  return b;

	return cons( CAR( a ) , append2( CDR( a ) , b ) );
}

cell add( cell a , cell b )
{
	return INT2CELL( CELL2INT( a ) + CELL2INT( b ) );
}

cell subtract( cell a , cell b )
{
	return INT2CELL( CELL2INT( a ) - CELL2INT( b ) );
}

cell multiply(cell a, cell b)
{
	return INT2CELL(CELL2INT(a) * CELL2INT(b));
}

cell divide(cell a, cell b) {
  return INT2CELL(CELL2INT(a) / CELL2INT(b));
}

cell eq(cell a, cell b) {
  return a == b ? t_sym : NIL;
}

cell gt(cell a, cell b) {
  return CELL2INT(a) > CELL2INT(b) ? t_sym : NIL;
}

cell assoc(cell key, cell list) {
  while (list != NIL) {
    if (key == CAAR(list))  return CAR(list);
    list = CDR(list);
  }
  return NIL;
}

cell acons( cell key , cell value , cell alist )
{
  return cons( cons( key , value ) , alist );
}

cell pairlis(cell keys, cell values) {
  cell alist = NIL;
  while (keys != NIL) {
    alist = acons(CAR(keys), values == NIL ? NIL : CAR(values), alist);
    keys = CDR(keys);
    values = CDR(values);
  }
  return alist;
}

cell list_f(cell list) {
  return list;
}

//========================< 印刷 >=======================================▼
/// <summary>
/// printbufferにコンスの内容を文字にして保存する
/// </summary>
static char print_buffer[ PRINT_MAX + 16 ];
void sprint_next( char *buff , cell e )
{
	char w[64];

	if ( strlen( buff ) >= PRINT_MAX )  return;

	if ( e == NIL )
	{
		strcat( buff , "nil" );
	} 
	else if ( IS_INT( e ) )
	{
		sprintf( w , "%d" , CELL2INT( e ) );
		strcat( buff , w );
	} 
	else if ( IS_SYMBOL( e ) )
	{
		strcat( buff , SYMBOL_NAME( e ) );
	} 
	else if ( IS_CONS( e ) )
	{
		strcat( buff , "(" );  
		while ( 1 )
		{
			sprint_next( buff , CAR( e ) );
			e = CDR( e );
			if ( e == NIL ) break;
			if ( IS_INT( e ) || IS_SYMBOL( e ) )
			{
				strcat( buff, "." );
				sprint_next( buff, e );
				break;
			}
			strcat( buff , " " );
		}
		strcat( buff , ")" );  
	} 
	else
	{
		sprintf(w, "*%x*", e);
		strcat(buff, w);
	}
}
char *sprint_e( cell e )
{
	print_buffer[0] = 0;
	sprint_next( print_buffer, e );
	return print_buffer;
}
void print_e( cell e )
{
	printf( "%s\n" , sprint_e( e ) );
}

//========================< シンボル制御 >=======================================▼
/// <summary>
/// 指定のシンボルを探す
/// </summary>
cell find_symbol( char *symbol_name )
{
	for ( int i = 0; i < SYMBOL_MAX; i++ )
	{
		if ( strcmp( symbol_name , symbol[ i ].name ) == 0 )
		{
			return SYMBOL2CELL( &symbol[ i ] );
		}
	}
	return 0;
}
/// <summary>
/// 指定のシンボルを作る
/// </summary>
cell make_symbol(char *symbol_name)
{
	cell p;
	if ( p = find_symbol( symbol_name ) )  return  p;

	if ( symbol_pt >= SYMBOL_MAX ) panic( "Symbol overflow" );
	symbol_t *s = &symbol[ symbol_pt ];
	symbol_pt++;
	strcpy( s->name , symbol_name );
	return SYMBOL2CELL( s );
}

//========================< 読込制御 >=======================================▼
static FILE *input_stream = NULL;
static char unget_c = 0;
static char read_buffer[ READ_MAX + 1 ];
static int  read_buffer_pt = -1;

int get_one_line( char *prompt )
{
	printf( prompt );
	if ( fgets( read_buffer , READ_MAX , stdin ) == NULL )  return -1;
	return 0;
}
void open_load_file( char *path )
{
	if ( strchr( path, '.' ) == NULL )  strcat( path , DEFAULT_FILE_EXT );

	if ( ( input_stream = fopen(path, "r") ) == NULL ) 
		throw_error("File not found  '%s'", path);
} 
void close_load_file()
{
	  fclose( input_stream );
	  input_stream = NULL;
} 
char get_one_char()
{
	if ( unget_c != 0 )
	{
		char c = unget_c;
		unget_c = 0;
		return c;
	}

	if ( read_buffer_pt == -1 || read_buffer_pt >= strlen( read_buffer ) )
	{
		while ( 1 )
		{
			if ( input_stream == NULL )
			{
				if ( get_one_line( "lisp> ") ) exit(1);
	
				if ( strncmp( read_buffer, LOAD_FILE_ESCAPE"\\f ", strlen( LOAD_FILE_ESCAPE ) ) == 0 )
				{
					open_load_file( strtok( &read_buffer[ strlen( LOAD_FILE_ESCAPE ) ] , "\r\n" ) );
					continue;
				}
			}
			else
			{
				if ( fgets( read_buffer , READ_MAX , input_stream ) == NULL )
				{
					close_load_file();
					continue;
				}
			}
			if ( read_buffer[ 0 ] == ';' ) continue;  // comment
			read_buffer_pt = 0;
			break;
		}
	}

	return read_buffer[ read_buffer_pt++ ];
}

void unget_one_char( char c )
{
	unget_c = c;
}

//========================< トークン解析 >=======================================▼
token_t *get_token()
{
	static token_t token;
	static char sym[ SYMBOL_NAME_MAX+1 ];
	char c;
	int  len;
	while ( isspace( c = get_one_char() ) );

	//スペシャルフォーム
	if ( strchr( SPECIAL_TOKENS , c ) )
	{
		token.kind = c;
		return &token;
	}

	//数字
	if ( isdigit( c ) )
	{
		int n = ( c - '0' );
		c = get_one_char();
		while ( isdigit( c ) )
		{
			n = n * 10 + ( c - '0' );
			c = get_one_char();
		}
		unget_one_char( c );
		token.kind = 'N';
		token.number = n;
		return &token;    
	}

	//マイナス数字
	if (c == '-')
	{
		c = get_one_char();
		if ( isdigit( c ) )
		{
			unget_one_char( c );
			token_t *t = get_token();
			t->number = - t->number;
			return t;
		}
		unget_one_char( c );
		c = '-';
	}

	//ここまで来たらトークンは文字だと断定
	len = 0;
	do
	{
		sym[ len++ ] = c;
		c = get_one_char();
	} 
	while ( ! ( strchr( SPECIAL_TOKENS , c ) || isspace( c ) ) );
	sym[ len ] = 0;
	unget_one_char( c );

	token.kind = 'S';
	token.name = sym;
	return &token;    
}

cell read_next( token_t *token )
{
	cell car_e , cdr_e , e ;
	cell list = NIL;
	if ( token->kind != '(' )
	{
		if ( token->kind == 'N' )
		{
			return INT2CELL( token->number );
		}

		// symbol
		return make_symbol( token->name );
	}

	while ( 1 )
	{
		token = get_token();
		if ( token->kind == ')' )
		{
			return list;
		}
		car_e = read_next( token );
		if ( token->kind == '.' )
		{
			// (a.b)
			token = get_token();
			cdr_e = read_next( token );
			token = get_token();
			if ( token->kind != ')' )
			{
				throw_error( ") required after '%s'" , sprint_e( cdr_e ) );
			}
			return cons( car_e , cdr_e );
		}

		// (a b)
		list = concat2( list , cons( car_e , NIL ) );
	}
}

cell read_e()
{
	cell e;
	e = read_next( get_token() );
	return e;
}

//========================< 実行 >=======================================▼

int list_len( cell list )
{
	int n = 0;
	while ( list != NIL )
	{
		list = CDR( list );
		n++;
	}
	return n;
}

cell eval( cell e , cell env );

cell eval_list( cell list , cell env )
{
	cell values = NIL;
	while ( list != NIL )
	{
		values = concat2( values , cons( eval( CAR( list ) , env ) , NIL ) );
		list = CDR( list );
	}
	return values;
}

void check_arg_count(cell f, int requires, int arg_count)
{
	if ( arg_count != requires )
	{
		throw_error("Wrong number of arguments '%s' requires %d, but %d" , sprint_e(f), requires, arg_count);
	}
}

cell apply( cell func , cell args , cell env )
{
	cell fbody = eval( func , env );
	if ( !IS_CONS( fbody ) )
	{
		throw_error( "Not function %s", sprint_e( func ) );
	}

	cell ftype = CAR( fbody );
	if ( ftype == func_sym )
	{

		int n_func_args = CELL2INT( CDDR( fbody ) );
		if ( n_func_args > 0 )
		{
			check_arg_count( func , n_func_args % 100 , list_len( args ) );
		}
		switch ( n_func_args )
		{
			case -1:
			{
				//引数１つの関数
				cell ( * func_pt )( void * ) =  ( cell ( __cdecl * )( cell ) )CELL2INT( CADR ( fbody ) );
				return ( *func_pt )( eval_list( args , env ) );
			}
			case 0:
			{
				//引数無しの関数
				cell ( * func_pt )() =  ( cell ( __cdecl * )() )CELL2INT( CADR ( fbody ) );
				return ( *func_pt )();
			}
			case 1:
			{
				//引数１つの関数
				cell ( * func_pt )(  cell ) =  ( cell ( __cdecl * )( cell ) )CELL2INT( CADR ( fbody ) );
				return ( *func_pt )( eval( CAR( args ) , env ) );
			}
			case 2:
			{
				//引数２つの関数
				cell ( * func_pt )(  cell , cell ) =  ( cell ( __cdecl * )( cell , cell ) )CELL2INT( CADR ( fbody ) );
				return ( *func_pt )( eval( CAR( args ) , env ) , eval( CADR( args ) , env ) );
			}
			case 3:
			{
				//引数３つの関数
				cell ( * func_pt )(  cell , cell , cell ) =  ( cell ( __cdecl * )( cell , cell , cell ) )CELL2INT( CADR ( fbody ) );
				return ( *func_pt )( eval( CAR( args ) , env ) , eval( CADR( args ) , env), eval( CADDR( args ) , env ) );
			}
			case 100:
			{
				//引数１つの関数
				cell ( * func_pt )(  cell ) =  ( cell ( __cdecl * )( cell ) )CELL2INT( CADR ( fbody ) );
				return ( *func_pt )( env );
			}
			case 101:
			{
				//引数２つの関数
				cell ( * func_pt )(  cell , cell ) =  ( cell ( __cdecl * )( cell , cell ) )CELL2INT( CADR ( fbody ) );
				return ( *func_pt )( eval( CAR( args ), env ), env );
			}
			case 102:
			{
				//引数３つの関数
				cell ( * func_pt )(  cell , cell , cell ) =  ( cell ( __cdecl * )( cell , cell , cell ) )CELL2INT( CADR ( fbody ) );
				return ( *func_pt )( eval( CAR( args ), env ), eval( CADR( args ), env ), env );
			}
			case 103:	
			{
				//引数４つの関数
				cell ( * func_pt )(  cell , cell , cell , cell ) =  ( cell ( __cdecl * )( cell , cell , cell, cell ) )CELL2INT( CADR ( fbody ) );
				return ( *func_pt )( eval( CAR( args ), env ), eval( CADR( args ), env ), eval( CADDR( args ) , env ) , env );
			}
		}
	}
	if ( ftype == sform_sym )
	{
		cell ( *sform_pt )( cell , cell ) =  ( cell ( __cdecl * )( cell , cell ) )CELL2INT( CDR( fbody ) );
		return ( *sform_pt )( args , env );
	}
	if ( ftype == lambda_sym )
	{
		cell params = CADR( fbody );
		cell e = CADDR( fbody );
		cell closure_env = CADDDR( fbody );
		check_arg_count( func , list_len( params ) , list_len( args ) );

		return eval( e , append2( concat2( pairlis( params , eval_list( args , env ) ) , closure_env ), env ) );
	}
	if (ftype == macro_sym)
	{
		cell params = CADR( fbody );
		cell e = CADDR( fbody );
		cell closure_env = CADDDR(fbody);
		check_arg_count(func, list_len(params), list_len(args));

		// macro展開
		cell r = eval(e, append2( concat2( pairlis( params , args ) , closure_env ) , env ) );
		return eval(r, env);
	}
	throw_error( "Not function %s" , sprint_e( func ) );
	return NIL;
}

cell eval( cell e , cell env )
{
	cell v;
	if ( e == NIL || IS_INT( e ) ) return e;
	if ( IS_SYMBOL( e ) )
	{
		v = assoc( e , env );
		if ( v == NIL ) v = assoc( e , top_env );

		if ( v != NIL )  return CDR( v );

		throw_error( "Undefined symbol '%s'" , SYMBOL_NAME( e ) );
	}
	return apply( CAR( e ) , CDR( e ), env );
}

void error_print() {
	printf("** ERROR : %s\n\n", error_message);
}

//========================< 入力 >=======================================▼

void flush_read_buffer()
{
	read_buffer_pt = -1;
}

void set_read_buffer(char *s) {
  strcpy(read_buffer, s);
  read_buffer_pt = 0;
}


//========================< 初期化 >=======================================▼

void make_self_value_symbol( char *name, cell *var )
{
	*var = make_symbol( name );
	top_env = acons( * var , * var , top_env );
}

/*
★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
Lispの関数情報生成
★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
*/
void make_func( char *name , void *func , int n_args )
{
	cell s = make_symbol( name );

	//aconsは(第三引数 (第一引数 第二引数))というリストを作る
	//関数は関数名をシンボル化し、そのシンボルに*func*というマークと、関数のアドレスと、引数の数の3つの情報を紐づけてtop_envに紐づけて実行可能な状態にLispに格納される）
	//以降リスト中にシンボルが入っているリストが見つかったらそのリストが保持してる3つの情報をもとに関数を実行する
	top_env = acons( s , cons( func_sym , 
				cons( INT2CELL( ( int )func ) , INT2CELL( n_args ) ) ) , top_env );
}

//========================< スペシャル >=======================================▼

void make_special_sform( cell *s , char *name , void *func )
{
	*s = make_symbol( name );
	top_env = acons( *s , cons( sform_sym , INT2CELL( ( int )func ) ), top_env ); 
}

cell quote_f(cell e, cell env) {
  return CAR(e);
}

cell setq(cell e, cell env) {
  cell v = CAR(e);
  cell s = CADR(e);
  if (! IS_SYMBOL(v)) throw_error("setq require symbol for 1'st argument");

  cell v_env = assoc(v, env);
  if (v_env == NIL) v_env = assoc(v, top_env);

  if (v_env != NIL) {
    CDR(v_env) = eval(s, env); 
    return v;
  }
  if (env == NIL) {
    top_env = acons(v, eval(s, env), top_env);
    return v;
  }
  throw_error("unbound variable: %s", SYMBOL_NAME(v));
}

cell cond(cell e, cell env) {
  while (e != NIL && eval(CAAR(e), env) == NIL)  e = CDR(e);
  return  (e != NIL) ? eval(CAR(CDAR(e)), env) : NIL;
}

cell lambda_f(cell e, cell env) {
  return cons(lambda_sym, concat2(e, cons(env, NIL)));
}

cell macro_f(cell e, cell env) {
  return cons(macro_sym, concat2(e, cons(env, NIL)));
  return e;
}

void make_sform(char *name, void *func)
{
	cell s = make_symbol( name );
	top_env = acons( s , cons( sform_sym , INT2CELL( ( int ) func ) ) , top_env );
}

void init()
{
  flush_read_buffer();
  cell s;
  // nil
  top_env = NIL;
  s = make_symbol("nil");
  top_env = acons(s, NIL, top_env);
  // symbols
  make_self_value_symbol("t", &t_sym);
  make_self_value_symbol("*func*", &func_sym);
  make_self_value_symbol("*sform*", &sform_sym);
  make_self_value_symbol("macro", &macro_sym);
  // primitive fuctions
  make_func("car", &car, 1);
  make_func("cdr", &cdr, 1);
  make_func("cons", &cons, 2);
  make_func("eq", &eq, 2);
  make_func("+", &add, 2);
  make_func("-", &subtract, 2);
  make_func("*", &multiply, 2);
  make_func("/", &divide, 2);
  make_func("=", &eq, 2);
  make_func(">", &gt, 2);
  make_func("assoc", &assoc, 2);
  make_func("acons", &acons, 3);
  make_func("list", &list_f, -1);
  make_func("eval", &eval, 101);
  // special-forms
  make_special_sform(&quote_sym, "quote", &quote_f);
  make_special_sform(&lambda_sym, "lambda", &lambda_f);
  make_special_sform(&macro_sym, "macro", &macro_f);
  // special-forms
  make_sform("setq", &setq);
  make_sform("cond", &cond);
}

//========================< エントリーポイント >=======================================▼

int start( int argc, char *argv[] )
{
	init();
	if ( argc == 2 && strcmp(argv[1], "test" ) == 0 )  test();

	while (1) {
	if (setjmp(long_jump_env) != 0)  error_print();
	print_e(eval(read_e(), NIL));
	}
}

//========================< テストコード >=======================================▼

void assert_str_eq(char *expected, char *value, char *msg) {
  printf(" %s -- ", msg);
  if (strcmp(expected, value) == 0) {
    printf(" OK\n");
  } else {
    printf(" NG value = %s, expected = %s\n", value, expected);
    exit(0);
  }
}

void assert_int_eq(int expected, int value, char *msg) {
  printf(" %s -- ", msg);
  if (expected == value) {
    printf(" OK\n");
  } else {
    printf(" NG value = %d, expected = %d\n", value, expected);
    exit(0);
  }
}

void assert_bool(int ok, char *msg) {
  printf(" %s -- ", msg);
  if (ok) {
    printf(" OK\n");
  } else {
    printf(" NG\n");
    exit(0);
  }
}

void assert_lisp_eq(char *expected, cell value, char *msg) {
  assert_str_eq(expected, sprint_e(value), msg);
}

void assert_char_eq(char *expected, char value, char *msg) {
  char s[2] = {value, 0};
  assert_str_eq(expected, s, msg);
}

cell lisp_obj(char *s) {
  char buff[256];
  sprintf(buff, "%s\n", s);
  set_read_buffer(buff);
  return read_next(get_token());
}

void test_make_symbol() {
  cell a = make_symbol("a");
  cell b = make_symbol("b");

  assert_str_eq("a", SYMBOL_NAME(a), "make symbol - name");
  assert_str_eq("b", SYMBOL_NAME(b), "make symbol - name");
  assert_bool(a == make_symbol("a"), "make symbol - ident");
}

void test_print() {
  cell a = make_symbol("a");
  cell b = make_symbol("b");

  assert_str_eq("123", sprint_e(INT2CELL(123)), "print(number)");
  assert_str_eq("abc", sprint_e(make_symbol("abc")), "print(symbol)");
  assert_str_eq("(a)", sprint_e(cons(a, NIL)), "print((a))");
  assert_str_eq("(a.b)", sprint_e(cons(a,b)), "print((a.b))");
  assert_str_eq("(a b)", sprint_e(cons(a,cons(b, NIL))), "print((a b))");
  assert_str_eq("(a 123 b)", sprint_e(cons(a,cons(INT2CELL(123), cons(b, NIL)))), "print((a 123 b))");
}

void test_token() {
  token_t *tk;
  set_read_buffer("(123 abcd\t-56 .-)\n");
  tk = get_token();
  assert_char_eq("(", tk->kind, "get_token (");
  tk = get_token();
  assert_char_eq("N", tk->kind, "get_token 123 (tk)");
  assert_int_eq(123, tk->number, "get_token 123 (number)");
  tk = get_token();
  assert_char_eq("S", tk->kind, "get_token abcd (tk)");
  assert_str_eq("abcd", tk->name, "get_token abcd (name)");
  tk = get_token();
  assert_char_eq("N", tk->kind, "get_token -56 (tk)");
  assert_int_eq(-56, tk->number, "get_token -56 number)");
  tk = get_token();
  assert_char_eq(".", tk->kind, "get_token .");
  tk = get_token();
  assert_char_eq("S", tk->kind, "get_token - (tk)");
  assert_str_eq("-", tk->name, "get_token - (name)");
  tk = get_token();
  assert_char_eq(")", tk->kind, "get_token )");
}

void test_read() {
  set_read_buffer("12345\n");
  assert_lisp_eq("12345", read_next(get_token()), "read 12345");
  set_read_buffer("ab+cd\n");
  assert_lisp_eq("ab+cd", read_next(get_token()), "read ab+cd");
  set_read_buffer("( )\n");
  assert_lisp_eq("nil", read_next(get_token()), "read ()");
  set_read_buffer("( a )\n");
  assert_lisp_eq("(a)", read_next(get_token()), "read (a)");
  set_read_buffer("( a . b )\n");
  assert_lisp_eq("(a.b)", read_next(get_token()), "read (a.b)");
  set_read_buffer("( a  b )\n");
  assert_lisp_eq("(a b)", read_next(get_token()), "read (a b)");
  set_read_buffer("( a  b )\n");
  assert_lisp_eq("(a b)", read_next(get_token()), "read (a b)");
  set_read_buffer("( a  b c)\n");
  assert_lisp_eq("(a b c)", read_next(get_token()), "read (a b c)");
  set_read_buffer("((a)  (b c))\n");
  assert_lisp_eq("((a) (b c))", read_next(get_token()), "read ((a) (b c))");
  set_read_buffer("(( a ( b (c  d) ) ) e)\n");
  assert_lisp_eq("((a (b (c d))) e)", read_next(get_token()), "read ((a (b (c d))) e)");
  set_read_buffer("'a\n");
  assert_lisp_eq("(quote a)", read_next(get_token()), "read 'a");
  set_read_buffer("'(a b c)\n");
  assert_lisp_eq("(quote (a b c))", read_next(get_token()), "read '(a b c)");
  set_read_buffer("((a.1)(b.1))");
  assert_lisp_eq("((a.1) (b.1))", read_next(get_token()), "read ((a.1) (b.1))");
  set_read_buffer("(c.(= n 0))");
  assert_lisp_eq("(c = n 0)", read_next(get_token()), "read (c.(= n 0))");
}

void test_function() {
  cell a = make_symbol("a");
  cell b = make_symbol("b");
  cell c = make_symbol("c");
  cell d = make_symbol("d");
  cell l = cons(a, cons(b, NIL));
  cell alist = NIL;

  assert_lisp_eq("(a b)", l, "cons(a (cons b nil))");
  assert_lisp_eq("(a)", concat2(NIL, cons(a, NIL)), "concat2(nil, (a))");
  assert_lisp_eq("(a b c)", concat2(l, cons(c, NIL)), "concat2((a b), (c))");
  assert_lisp_eq("a", car(l), "car((abc))");
  assert_lisp_eq("(b c)", cdr(l), "cdr((abc))");
  assert_int_eq(3, list_len(l), "list_len((a b))");
  assert_lisp_eq("(x y a b c)", append2(lisp_obj("(x y)"), l), "append2((x y), (a b c))");

  alist = acons(a, INT2CELL(1), alist);
  alist = acons(b, INT2CELL(2), alist);
  alist = acons(c, INT2CELL(3), alist);
  assert_lisp_eq("((c.3) (b.2) (a.1))", alist, "acons()");
  assert_lisp_eq("(b.2)", assoc(b, alist), "asooc(ok)");
  assert_lisp_eq("nil", assoc(d, alist), "asooc(nil)");
  assert_lisp_eq("((b.2) (a.1))", pairlis(lisp_obj("(a b)"), lisp_obj("(1 2)")), "pairlis");
  assert_lisp_eq("(11 22 33)", list_f(lisp_obj("(11 22 33)")), "list");

  assert_lisp_eq("5", add(lisp_obj("2"), lisp_obj("3")), "add()");
  assert_lisp_eq("1", subtract(lisp_obj("3"), lisp_obj("2")), "subtract()");
  assert_lisp_eq("6", multiply(lisp_obj("2"), lisp_obj("3")), "multiply()");
  assert_lisp_eq("2", divide(lisp_obj("10"), lisp_obj("4")), "divide()");
  assert_lisp_eq("t", gt(lisp_obj("3"), lisp_obj("2")), "gt():t");
  assert_lisp_eq("nil", gt(lisp_obj("2"), lisp_obj("3")), "gt():nil");
  assert_lisp_eq("t", eq(a, a), "eq():t");
  assert_lisp_eq("nil", eq(a, b), "eq():nil");
}

void test_special_form() {
  cell a = make_symbol("a");
  cell b = make_symbol("b");
  cell c = make_symbol("c");
  cell env = top_env; 

  env = acons(a, lisp_obj("11"), env);
  env = acons(b, NIL, env);
  setq(cons(b, cons(a, NIL)), env);
  assert_lisp_eq("11", CDR(assoc(b, env)), "setq(b a)");
  setq(cons(c, cons(INT2CELL(22), NIL)), NIL);
  assert_lisp_eq("22", CDR(assoc(c, top_env)), "setq(c 22) top level");

  assert_lisp_eq("1", cond(lisp_obj("(((> 4 3) 1) ((= 4 3) 0) (t -1))"), env), "cond 1");
  assert_lisp_eq("0", cond(lisp_obj("(((> 3 3) 1) ((= 3 3) 0) (t -1))"), env), "cond 2");
  assert_lisp_eq("-1", cond(lisp_obj("(((> 1 3) 1) ((= 1 3) 0) (t -1))"), env), "cond 3");

  assert_lisp_eq("(lambda (x y) (+ x y) ((x.1) (y.2)))",
		  lambda_f(lisp_obj("((x y) (+ x y))"), lisp_obj("((x.1) (y.2))")), "lambda");
  assert_lisp_eq(
    "(macro (c th el) (list (quote cond) (list c th) (list t el)) ((c = n 0) (th.1) (el.5)))",
    macro_f(lisp_obj("((c th el) (list 'cond (list c th) (list t el)))"),
	    lisp_obj("((c.(= n 0)) (th.1) (el.5))")), "macro");
}

void test_apply_eval() {
  cell a, b;
  make_self_value_symbol("a", &a);
  make_self_value_symbol("b", &b);
  cell l = cons(a, cons(b, NIL));
  cell env = top_env; 
  
  assert_lisp_eq("(a.b)", apply(lisp_obj("cons"), l, env), "apply(func)");
  assert_lisp_eq("(a b)", apply(lisp_obj("quote"), cons(l, NIL), env), "apply(sform)");
  assert_lisp_eq("3", apply(lambda_f(lisp_obj("((x y) (+ x y))"), env), 
				   lisp_obj("(1 2)"), env), "apply(lambda)");

  assert_lisp_eq("8", apply(lambda_f(lisp_obj("((x) (+ x n))"), cons(lisp_obj("(n.5)"), env)),
				   lisp_obj("(3)"), env), "apply(lambda+closure)");
  char *mbody = "((c th el) (list 'cond (list c th) (list t el)))";
  assert_lisp_eq("5", apply(macro_f(lisp_obj(mbody), env),
			    lisp_obj("((= n 0) 1 5)"), 
			    cons(lisp_obj("(n.2)"), env)), "macro 1");
  assert_lisp_eq("1", apply(macro_f(lisp_obj(mbody), env),
			    lisp_obj("((= n 0) 1 5)"), 
			    cons(lisp_obj("(n.0)"), env)), "macro 2");

  assert_lisp_eq("nil", eval(NIL, env), "eval(nil)");
  assert_lisp_eq("11", eval(INT2CELL(11), env), "eval(number)");
  assert_lisp_eq("a", eval(a, env), "eval(symbol value)");
  assert_lisp_eq("2", eval(a, lisp_obj("((b.1)(a.2))")), "eval(bind value)");
}

void test() {
  cell a, b, c, l;
  token_t *tk;

  printf("** Testing **\n");
  if (setjmp(long_jump_env) != 0) {
    error_print();
    exit(1);
  }

  test_make_symbol();
  test_print();
  test_token();
  test_read();
  test_function();
  test_special_form();
  test_apply_eval();
  printf("** Test end **\n");

  exit(0);
}

/*  end  */

} }

#endif