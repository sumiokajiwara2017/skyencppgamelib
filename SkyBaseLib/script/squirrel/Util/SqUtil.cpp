#include "StdAfx.h"
#include "SkyBaseLib/Script.h"
#include "SkyBaseLib/Directory.h"

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

using namespace squtil_const;

//-------------------------------------< 実装 >---------------------------------------------//

static void SqDefaultPrintf( HSQUIRRELVM v , const SQChar* s , ... )
{
	v;

	//バッファ確保
	skyString s_Buf[ SQ_PRINTF_MSG_LENGHT_WCHAR ];

	//文字列をバッファに格納
	VA_LIST( s_Buf , SQ_PRINTF_MSG_LENGHT_WCHAR , s );

	SKY_PRINTF( s_Buf );
}

void CSqUtil::DefaultPrint( HSQUIRRELVM v , const skyString *s , ... )
{
	//バッファ確保
	skyString s_Buf[ SQ_PRINTF_MSG_LENGHT_WCHAR ];

	//文字列をバッファに格納
	VA_LIST( s_Buf , SQ_PRINTF_MSG_LENGHT_WCHAR , s );

	//プリント
	SqDefaultPrintf( v , s_Buf );
}

HSQUIRRELVM CSqUtil::CreateSQVM( u32 stackSize )
{
	//VM作成
	HSQUIRRELVM v; 
	v = sq_open( stackSize );

	//最初の sqstd_seterrorhandlers() 関数は標準のエラーハンドラを仮想マシンに設定する命令です。
	//標準のものを使いたくない場合は自分自身で用意することが可能です。
#ifdef SW_USE_SQ_STD_LIB
	sqstd_seterrorhandlers( v );
#endif

	//2行目の sq_setprintfunc() は仮想マシンに自作の出力関数をセットする命令です。
	//こちらも標準の print 命令が存在しているのでそちらを使ってもOKですが、それ以外の出力をしたい場合はこの関数で設定します。
	sq_setprintfunc( v , SqDefaultPrintf , SqDefaultPrintf );

	//ルートテーブルの設定
	sq_pushroottable( v );

	return v;
}

void CSqUtil::DestroySQVM( HSQUIRRELVM v )
{
	//VM破棄
	sq_close( v );
}

void CSqUtil::RegisterScriptFileAndExecuteFunc( HSQUIRRELVM v , const hash::CHashStringPtr &pFilePath , const hash::CHashStringPtr &pFuncName , SqFuncParam &param , SqFuncParam &outParam )
{
	//nutファイルの実行
	RegisterScriptFile( v, pFilePath );

	//関数の実行
	ExecuteFunc( v, pFuncName , param , outParam );
}

void CSqUtil::RegisterScriptFile( HSQUIRRELVM &v , const hash::CHashStringPtr &pFilePath )
{
	//ファイル存在チェック
	if ( !CPathUtil_::IsFileOn( pFilePath->GetString() ) )
	{
        SKY_PANIC_MSG( _T( "The file is not found. filePath=[%s]" ), pFilePath->GetString() );
	}

#ifdef SW_USE_SQ_STD_LIB
	//nutファイルの実行
	s32 top = sq_gettop(v);						// 呼び出し前のスタックサイズを保存
	sq_pushroottable(v);
	if( !SQ_SUCCEEDED( sqstd_dofile( v , pFilePath->GetString() , 0 , 1 ) ) ) //ファイルの実行
	{
        SKY_PANIC_MSG( _T( "It failed in the execution of the file.fileName=[%s]" ), pFilePath->GetString() );
	}
	sq_settop( v , top ); // 元のスタックサイズに戻す
#endif
}

void CSqUtil::ExecuteFunc( HSQUIRRELVM &v , const hash::CHashStringPtr &pFuncName , SqFuncParam &inParam , SqFuncParam &outParam )
{
	s32 top = sq_gettop(v);						// 呼び出し前のスタックサイズを保存
	sq_pushroottable(v);						// ルートテーブルをpush
	sq_pushstring(v, pFuncName->GetString() , -1);			// 関数をスタックに乗せる

	if (SQ_SUCCEEDED(sq_get( v , -2 ) ) ) {			// 関数をルートテーブルから取得

		sq_pushroottable( v );					// "this"をpush (この例ではルートテーブルに)

		CSqUtil_::RegisterStdLib( v );

		inParam.PushSquirrelStack( v );

		if ( !SQ_SUCCEEDED( sq_call( v , inParam.GetParamNum() + 1 , SQTrue , SQTrue) ) )	// 関数を呼ぶ(this分+1する）
		{
            SKY_PANIC_MSG( _T( "It failed in the execution of the file.It failed in the execution of the function.funcName=[%s]" ), pFuncName->GetString() );
		}
	}

	outParam.PopSquirrelStack( v );
	sq_settop( v , top ); // 元のスタックサイズに戻す
}

HSQUIRRELVM CSqUtil::CreateNewThread( HSQUIRRELVM v , u32 stackSize )
{
	//新しいスレッドの作成
	HSQUIRRELVM  new_v = sq_newthread( v , stackSize );				//第1引数として渡されたVMのフレンドVMを新たに作成し、それをスレッドオブジェクトとしてスタックにpushする。

	//ルートテーブルの設定
	sq_pushroottable( new_v );

	return new_v;
}

void CSqUtil::RegisterStdLib( HSQUIRRELVM v )
{

#ifdef SW_USE_SQ_STD_LIB
	if ( !SQ_SUCCEEDED( sqstd_register_iolib( v ) ) ) //I/Oライブラリ
	{
		SKY_PANIC_MSG( _T( "It failed in initialization. " ) );
	}
	if ( !SQ_SUCCEEDED( sqstd_register_bloblib( v ) ) ) //blobライブラリ
	{
		SKY_PANIC_MSG( _T( "It failed in initialization. " ) );
	}
	if ( !SQ_SUCCEEDED( sqstd_register_stringlib( v ) ) ) //文字列ライブラリ
	{
		SKY_PANIC_MSG( _T( "It failed in initialization. " ) );
	}
	if ( !SQ_SUCCEEDED( sqstd_register_systemlib( v ) ) ) //システムライブラリ
	{
		SKY_PANIC_MSG( _T( "It failed in initialization. " ) );
	}
	if ( !SQ_SUCCEEDED( sqstd_register_mathlib( v ) ) ) //数学ライブラリ
	{
		SKY_PANIC_MSG( _T( "It failed in initialization. " ) );
	}
#endif
}

void CSqUtil::RegisterSkyBaseLibApi( HSQUIRRELVM v)
{
	SqDebugApi_::RegisterApi( v );
	SqExtensionApi_::RegisterApi( v );
}

void CSqUtil::RegisterSkyBaseLibClass( HSQUIRRELVM v)
{
	RegisterClass( v , GetSqInputManager() );
	RegisterClass( v , GetSqCDosUtil() );
}

void CSqUtil::RegisterNativeFunc( HSQUIRRELVM v , SQFUNCTION f ,  const skyString *fname )
{
	sq_pushroottable( v );
	sq_pushstring( v , fname , -1 );
	sq_newclosure( v , f , 0 );    // 新しい関数を作成
	sq_createslot( v , -3 );
	sq_pop(v, 1);
}

void CSqUtil::RegisterScriptString( HSQUIRRELVM v , const hash::CHashStringPtr &pScript )
{
	sq_compilebuffer( v , pScript->GetString() , ( s32 )CStringUtil_::Len( pScript->GetString() ) * sizeof( SQChar ) , _T( "compile" ) , 1 );	//スクリプトをコンパイル
	sq_pushroottable( v );
	sq_call( v , 1 , 1 , 1 ); //実行
}

void CSqUtil::PushParam( HSQUIRRELVM v , const SqParamObject &param  )
{
	switch ( param.GetType() )
	{
		case eParamType_Int:
		{
			sq_pushinteger( v , param.m_Param.GetInt() );
			break;
		}
		case eParamType_Float:
		{
			sq_pushfloat( v , param.m_Param.GetFloat() );
			break;
		}
		case eParamType_String:
		{
			sq_pushstring( v , param.m_Param.GetString() , -1);
			break;
		}
		case eParamType_Bool:
		{
			sq_pushbool( v , param.m_Param.GetBool() );
			break;
		}
		case eParamType_Void:
		{
			if ( param.m_Param.GetVoid() != skyNull )
			{
				sq_pushuserpointer( v , param.m_Param.GetVoid() );
			}
			else
			{
				sq_pushnull( v );
			}
			break;
		}
		case eParamType_Array:
		{
			//配列を作成
			sq_newarray( v , 0 );
			SqParamObjectList *pList = param.m_Param.GetArray();
			FOREACH( SqParamObjectList , it , *pList )
			{
				PushParam( v , ( *it ) ); //再帰
				sq_arrayappend( v , -2 );
			}
			break;
		}
		case eParamType_Table:
		{
			SqParamObjectHash *pHash = param.m_Param.GetTable();

			//テーブルを作成
			sq_newtable( v );

			hash::CHashStringPtrList keyList;
			pHash->GetkeyList( keyList );
			FOREACH( hash::CHashStringPtrList , it , keyList )
			{
				sq_pushstring( v , ( *it )->GetString() , -1 );					//キーを設定
				PushParam( v , ( * pHash )[ ( *it )->GetString() ] );	//再帰
				sq_createslot( v , -3 );										//キーと値のセットをテーブルに設定
			}
			break;
		}
		case eParamType_CppFunction:
		{
			SqCppFunc *pFunc = param.m_Param.GetCppFunc();

			sq_newtable( v );

			sq_pushstring( v , pFunc->m_fname->GetString() , -1 );
			sq_newclosure( v , pFunc->m_f , 0 );    // 新しい関数を作成
			sq_setparamscheck( v , SQ_MATCHTYPEMASKSTRING , pFunc->m_typemask->GetString() );
			sq_setnativeclosurename( v , -1 , pFunc->m_fname->GetString() );
			sq_createslot( v , -3 );

			break;
		}
		case eParamType_Class:
		{
			SqClassObject *pClass = param.m_Param.GetClass();
			RegisterClass( v ,*pClass );

			//ルートテーブルから自分を探す
			sq_pushroottable( v );										//ルートテーブルを設定
			sq_pushstring( v , pClass->m_kName->GetString() , -1 );		//クラスの名前を検索キーにする
			sq_get( v , -2 );											//クラス検索（検索結果はスタックの一番上にある
			HSQOBJECT cs;
			sq_getstackobj( v , -1 , &cs );								//クラスを取り出す
			sq_pop( v , 2 );											//スタックからクラスとルートテーブルを抜く
			sq_pushobject( v , cs );									//クラスをPushする
			sq_createinstance( v, -1 );									//クラスのインスタンスを作成する
			HSQOBJECT ins;
			sq_getstackobj( v , -1 , &ins );							//インスタンスを取り出す
			sq_addref( v , &ins );										//インスタンスの参照カウンタを上げる（そうしないと下のsq_popで消されてしまう）
			sq_pop( v , 2 );											//スタックからインスタンスとルートテーブルを抜く
			sq_pushobject( v , ins );									//インスタンスをPushする

			break;
		}

	}
}

void CSqUtil::GetParam( HSQUIRRELVM v , SqParamObject &param )
{
	tagSQObjectType rs = sq_gettype( v , -1 );
	switch( rs )
	{
		case OT_NULL:
			param = ( void * )skyNull;
#ifdef SW_DISPLAY_SQ_LOG
            CSqUtil_::DefaultPrint( v , _T( "skyNull returned.\n" ) );
#endif
			break;
		case OT_INTEGER:
			{
				SQInteger  ret;
				sq_getinteger( v , -1 , &ret );
				param = ( s32 )ret;
#ifdef SW_DISPLAY_SQ_LOG
                CSqUtil_::DefaultPrint( v , _T( "Type SQInteger returnd. ret=[ %d ]\n" ) , ret );
#endif
				break;
			}
		case OT_FLOAT:
			{
				SQFloat  ret;
				sq_getfloat( v , -1 , &ret );
				param = ( f32 )ret;
#ifdef SW_DISPLAY_SQ_LOG
                CSqUtil_::DefaultPrint( v , _T( "Type SQFloat returned. ret=[ %f ]\n" ) , ret );
#endif
				break;
			}
		case OT_STRING:
			{
				const SQChar *ret;
				sq_getstring( v , -1 , &ret );
				param = ( skyString * )ret;
#ifdef SW_DISPLAY_SQ_LOG
                CSqUtil_::DefaultPrint( v , _T( "Type SQChar returned. ret=[ %s ]\n" ) , ret );
#endif
				break;
			}
		case OT_BOOL:
			{
				SQBool  ret;
				sq_getbool( v , -1 , &ret );
				param = ( skyBool )ret;
#ifdef SW_DISPLAY_SQ_LOG
                CSqUtil_::DefaultPrint( v , _T( "Type SQBool returned. ret=[ %d ]\n" ) , ret );
#endif
				break;
			}
		case OT_USERPOINTER:
			{
				SQUserPointer  ret;
				sq_getuserpointer( v , -1 , &ret );
				param = ( void * )ret;
#ifdef SW_DISPLAY_SQ_LOG
                CSqUtil_::DefaultPrint( v , _T( "Type SQUserPointer returned. ret=[ %d ]\n" ) , ret );
#endif
				break;
			}
		case OT_ARRAY:
			{
				SqParamObjectList *ret = NEW__( SqParamObjectList , SqParamObjectList() );

				sq_pushnull(v);							// null反復子
				while( SQ_SUCCEEDED( sq_next( v , -2 ) ) )
				{
					//再帰
					SqParamObject arrayp;
					GetParam( v , arrayp );

					ret->AddTail( arrayp );
    
					sq_pop(v, 1); // 次の反復の前にキーと値をpopする
				}
				sq_pop(v, 1); // null反復子をpop

				//戻り値として返す
				param = ( SqParamObjectList * )ret;

				break;
			}
		case OT_TABLE:
			{
				SqParamObjectHash *ret = NEW__( SqParamObjectHash , SqParamObjectHash( 31 ) );

				sq_pushnull(v);							// null反復子
				while( SQ_SUCCEEDED( sq_next( v , -2 ) ) )
				{
					//再帰
					SqParamObject hashp;
					GetParam( v , hashp ); //再帰

					//値をスタックからPOPする
					sq_pop( v , 1 ); //値をポップ

					//キーを文字として取得
					const SQChar *keySt;
					sq_getstring( v , -1 , &keySt );
					sq_pop( v , 1 ); //キーをポップ

					//Hashに格納
					ret->Set( keySt , hashp );
#ifdef SW_DISPLAY_SQ_LOG
                    CSqUtil_::DefaultPrint( v , _T( "Type SQTable returnd. ret=[ %s ]\n" ) , keySt );
#endif
				}
				sq_pop(v, 1); // null反復子をpop

				param = ( SqParamObjectHash * )ret;
				break;
			}
		case OT_CLASS:
			{
				break;
			}
		case OT_NATIVECLOSURE:
			{
				break;
			}
		case OT_USERDATA:
		case OT_CLOSURE:
		case OT_GENERATOR:
		case OT_INSTANCE:
		case OT_WEAKREF:
		case OT_OUTER:
		case OT_THREAD:
		case OT_FUNCPROTO:
		default:
#ifdef SW_DISPLAY_SQ_LOG
            CSqUtil_::DefaultPrint( v, _T( "It is a value not supported.\n" ) );
#endif
//			SKY_PANIC_MSG( _T( "サポート外の型の戻り値が返りました。" ) );
			return; //Popしない
	}
}

void CSqUtil::RegisterClass( HSQUIRRELVM v , const SqClassObject &classObj )
{
	//クラス名を設定
	sq_pushstring( v , classObj.m_kName->GetString() , -1 );

//---ここで親をスタックに積む▽

	if ( classObj.m_ParentName.IsEmpty() == skyFalse )
	{//ルートテーブルから親を探す
		sq_pushroottable( v );													//ルートテーブルを設定
		sq_pushstring( v , classObj.m_ParentName->GetString() , -1 );		//親クラスの名前を検索キーにする
		if ( SQ_SUCCEEDED( sq_get( v , -2 ) ) )									//検索
		{//見つけた
			HSQOBJECT obj;
			sq_getstackobj( v , -1 , &obj );									//親クラスを取り出す
			sq_pop( v , 2 );													//スタックから親クラスとルートテーブルを抜く
			sq_pushobject( v , obj );											//親クラスをPushする（継承の準備ができた）
		}
		else
		{
			SKY_PANIC_MSG( _T( "Please set parent class (%s) to VM previously. " ) , classObj.m_ParentName->GetString() ); //親クラス%sを継承するには、親クラスを先にVMに設定してください。
		}
	}

//---ここで親をスタックに積む△

	//クラスを作成
	sq_newclass(v,  classObj.m_ParentName.IsEmpty() == skyFalse );

	//メンバ関数を設定する
	for ( u32 i = 0 ; i < classObj.m_CppMethodList.GetNum() ; i++ )
	{
		sq_pushstring( v , classObj.m_CppMethodList[ i ].m_fname->GetString() , -1 );
		sq_newclosure( v , classObj.m_CppMethodList[ i ].m_f , 0 );
		sq_setparamscheck( v , SQ_MATCHTYPEMASKSTRING , classObj.m_CppMethodList[ i ].m_typemask->GetString() );
		sq_setnativeclosurename( v , -1 , classObj.m_CppMethodList[ i ].m_fname->GetString() );
		sq_createslot( v , -3 );
	}

	//メンバ変数を設定する
	hash::CHashStringPtrList keyList;
	classObj.m_MemberHash.GetkeyList( keyList );
	FOREACH( hash::CHashStringPtrList , it , keyList )
	{
		sq_pushstring( v , ( *it )->GetString() , -1 );								//キーを設定
		PushParam( v , ( classObj.m_MemberHash )[ ( *it )->GetString() ] );		//再帰
		sq_createslot( v , -3 );													//キーと値のセットをテーブルに設定
	}

	sq_createslot( v, -3 );

}
////////////////////////////////////////////////
//Sq用レキシカルアナライザー（ストリーム版） //
///////////////////////////////////////////////
static u32 s_CompiledFileSize = 0;
SQInteger SqLex_Stream( SQUserPointer destStream )
{//ストリームで文字が流れている。

	stream::IStreamPtr *pspStream = ( stream::IStreamPtr * )destStream;

	SKY_ASSERT_MSG( ( *pspStream )->vIsOpen() , _T( "This stream has not been opened. " ) );

	//入力ストーリームのサイズが０だった場合エラー
	SKY_ASSERT_MSG( ( *pspStream )->vGetSize() > 0 , _T( "It tried to compile an empty file. " ) ); //空のファイルをコンパイルしようとしました。
	SKY_ASSERT_MSG( ( *pspStream )->vGetSize() > 4 , _T( "The size of the file is too small. " ) ); //極端に短いファイルをコンパイルしようとしました。

	SQChar ret = 0;

	( *pspStream )->vReadStream( ( u8 * )&ret , sizeof( SQChar )  );

	//BOMの場合は読み込んで返さない
	if ( ret == BOM )
	{
		( *pspStream )->vReadStream( ( u8 * )&ret , sizeof( SQChar )  );
	}

	return ret;
}

//////////////////////////////////////////////
//Sq用書き込み関数（ストリーム版）          //
//////////////////////////////////////////////
SQInteger SqWrite_Stream( SQUserPointer destStream ,SQUserPointer pSrcData , SQInteger srcDataSize )
{
	stream::IStreamPtr *pspStream = ( stream::IStreamPtr * )destStream;

	SKY_ASSERT_MSG( ( *pspStream )->vIsOpen() , _T( "This stream has not been opened." ) );

	//０がきたらこの処理は０を返す。０を返せばおそらく終了する？
	if ( srcDataSize == 0 ) return 0;

	return ( *pspStream )->vWriteStream( ( u8 * )pSrcData , ( u32 )srcDataSize );
}

////////////////////////////////////////////
//Sq用読込関数（ストリーム版）              //
////////////////////////////////////////////
SQInteger SqRead_Stream( SQUserPointer destStream ,SQUserPointer pSrcData , SQInteger srcDataSize )
{
	stream::IStreamPtr *pspStream = ( stream::IStreamPtr * )destStream;

	SKY_ASSERT_MSG( ( *pspStream )->vIsOpen() , _T( "This stream has not been opened." ) );

	return ( *pspStream )->vReadStream( ( u8 * )pSrcData , ( u32 )srcDataSize );
}


SqObjectInterfacePtr CSqUtil::Compile( HSQUIRRELVM v , const stream::IStreamPtr &pSrc )
{
	s32 top = sq_gettop(v);						// 呼び出し前のスタックサイズを保存

	if( SQ_FAILED( sq_compile( v, SqLex_Stream, ( SQUserPointer * )&pSrc, pSrc->vGetName(), SQFalse ) ) )
	{
		SKY_PANIC_MSG( _T( "It failed in the Sq compilation. " ) ); //It failed in the Sq compilation. 
	}

	HSQOBJECT po;
	sq_getstackobj( v , -1 , &po );

	sq_settop( v , top ); // 元のスタックサイズに戻す

	return SqObjectInterfacePtr( NEW__( SqObjectInterface , SqObjectInterface( v , &po , skyTrue ) ) );
}

void CSqUtil::CompileOutStream( HSQUIRRELVM v , const stream::IStreamPtr &pSrc , const stream::IStreamPtr &pDest )
{
	s32 top = sq_gettop(v);						// 呼び出し前のスタックサイズを保存

	if( SQ_FAILED( sq_compile( v, SqLex_Stream, ( SQUserPointer * )&pSrc, pSrc->vGetName(), SQFalse ) ) )
	{
		SKY_PANIC_MSG( _T( "It failed in the Sq compilation. " ) );
	}

	if( SQ_FAILED( sq_writeclosure ( v , SqWrite_Stream , ( SQUserPointer * )&pDest ) ) )
	{
		SKY_PANIC_MSG( _T( "It failed in the output of the file that had compiled Sq. " ) ); //Sqコンパイル済みファイルの出力に失敗しました。
	}

	sq_settop( v , top ); // 元のスタックサイズに戻す
}

void CSqUtil::RegisterNativeDebugHook( HSQUIRRELVM v, SQDEBUGHOOK debughook )
{
	sq_setnativedebughook( v, debughook );
}
void CSqUtil::RegisterDebugHook( HSQUIRRELVM v )
{
	sq_setdebughook( v );
}
void CSqUtil::SetDebug( HSQUIRRELVM v , skyBool isOn )
{
	sq_enabledebuginfo( v , isOn );
}

} } }

void *sq_vm_malloc( SQUnsignedInteger size )
{	
	return SkyMalloc__( size );
}

void *sq_vm_realloc( void *p, SQUnsignedInteger oldsize, SQUnsignedInteger size )
{ 
	SKY_UNUSED_ARG( oldsize );
	return  SkyRealloc__( p, size );
}

void sq_vm_free( void *p, SQUnsignedInteger size )
{ 
	SKY_UNUSED_ARG( size );
	SkyFree( p );
}

#endif


