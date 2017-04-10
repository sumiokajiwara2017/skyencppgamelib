#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_SCRIPT

#include "SkyBaseLib/Script.h"
#include "SkyBaseLib/ReLoad.h"

static const skyString *TEST1_NUTFILE_PATH = _T( "data/test/script/FunctionCall.nut" );

//スクリプトを上の関数をＣ＋＋から呼ぶ
TEST( Script , 1 ) 
{
	SKY_BASE_LIB_INIT();

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

	sky::lib::time::CTimerObject loadTime( _T( "スクリプト読込" ) );

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 24 );
	spSqVm->RegisterScriptFile( CHS( TEST1_NUTFILE_PATH ) );

	loadTime.~CTimerObject();

	{ sky::lib::time::CTimerObject callTime( _T( "script call." ) );

	sky::lib::squirrel::SqFuncParam inParam;		//引数
	sky::lib::squirrel::SqFuncParam outParam;		//戻り値
	inParam.SetParam( ( s32 )1 );
	inParam.SetParam( ( f32 )99.5f );
	inParam.SetParam( CHS( _T("go!" ) ) );

	sky::lib::squirrel::SqParamObjectList arrayParam;
	arrayParam.AddTail( _T( "aaa" ) );
	arrayParam.AddTail( _T( "bbb" ) );
	arrayParam.AddTail( _T( "ccc" ) );
	inParam.SetParam( &arrayParam );

	sky::lib::squirrel::SqParamObjectHash tableParam( 3 );
	tableParam.Set( _T( "key1" ) , _T("val1" ) );
	tableParam.Set( _T( "key2" ) , _T("val2" ) );
	tableParam.Set( _T( "key3" ) , _T("val3" ) );
	inParam.SetParam( &tableParam );

	spSqVm->ExecuteFuncSync( CHS( _T( "foo" ) ), inParam , outParam ); //関数実行

	}

	SqVmFactory_->Delete( spSqVm );

#endif

	SKY_BASE_LIB_TERM();
}

static const skyString *TEST2_NUTFILE_PATH = _T( "data/test/script/CoroutineCall.nut" );

//C++上でスクリプトのコルーチンを生成して実行する
TEST( Script , 2 ) 
{
	SKY_BASE_LIB_INIT();

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );
	spSqVm->RegisterScriptFile( CHS( TEST2_NUTFILE_PATH ) );

	//引数を作成
	sky::lib::squirrel::SqFuncParam inParam;		//引数
	sky::lib::squirrel::SqFuncParam outParam;		//戻り値
	inParam.SetParam( (s32)5 );

	//子スレッドの作成
	sky::lib::squirrel::SqVmObjectPtr child[5];
	for ( u32 i = 0 ; i<5; i++ )
	{
		child[ i ] = spSqVm->CreateAndRegisterChildVm( 1024 , spSqVm );
	}

	//実行準備
	for ( u32 i = 0 ; i<5; i++ )
	{
		child[ i ]->InitExecuteFuncASync();
	}

	//実行(子スレッドを全部実行し、全部が終了したら終わる）
	BOOL isExec = TRUE;
	while( isExec ) 
	{
		isExec = FALSE;
		for ( u32 i = 0 ; i<5; i++ )
		{
			if ( child[ i ]->ExecuteFuncASync( CHS( _T( "foo_thread" ) ), inParam , outParam ) != sky::lib::squirrel::SqVmObject::eThreadStatus_End )
			{
				isExec = TRUE; //一個でも実行が終わっていなかったら終わらない
			}
		}
	}

	//子スレッドの削除（メモリ破壊！）
	spSqVm->ChildAllPop();

	//削除
	SqVmFactory_->Delete( spSqVm );

#endif

	SKY_BASE_LIB_TERM();
}

#include "SkyBaseLib/GameLoop.h"
#include "SkyBaseLib/Input.h"
#include "SkyBaseLib/Fps.h"

static const skyString *TEST3_NUTFILE_PATH = _T( "data/test/script/ClassBind.nut" );

//スクリプト上で入力受付をする（制御をスクリプトにやらせる）
TEST( Script , 3 ) 
{
	SKY_BASE_LIB_INIT();

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );
	spSqVm->RegisterScriptFile( CHS( TEST3_NUTFILE_PATH ) );
	spSqVm->InitExecuteFuncASync();

	//クラスを登録する
	spSqVm->RegisterClass( GetSqInputManager() );

	sky::lib::squirrel::SqFuncParam inParam;		//引数
	sky::lib::squirrel::SqFuncParam outParam;		//戻り値

	//FPSコントローラー作成
	sky::lib::fps::CFpsController fpsCtrl;

	SKY_PRINTF( _T( "please push key 'A' \n" ) );

	//ゲームループ
	GAME_LOOP_START
	{
		//フレーム開始
		fpsCtrl.StartFrame();

		//入力管理更新
		Ckeyboard_->Update( fpsCtrl.GetPassageTime() );

		//終了判定
		{ sky::lib::time::CTimerObject callTime( _T( "script call" ) );

		if ( spSqVm->ExecuteFuncASync( CHS( _T( "foo_classbind" ) ), inParam , outParam ) == sky::lib::squirrel::SqVmObject::eThreadStatus_End )
		{
			GAME_LOOP_EXIT();
		}

		}

		//フレームレート維持のため、時間が余ったら待つ
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

#endif

	SKY_BASE_LIB_TERM();
}

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

//実行するSqスクリプト（ファイルではなく埋め込み文字であるのがポイント）
static const skyString TEST4_Script[] =
_T( " function foo( i , f ) " ) //fは一時テーブルであるそこに登録されたネイティブクロージャーを呼ぶ。ほんとうはfがジェネレータであったらいいのかもしれないが。。
_T( " { " )
_T( " print(\"i=\" + i + \" \\n\" );" )
_T( " cppfoo();" )
_T( " f.cppfoo2();" )
_T( " return \" 成功 \" " )
_T( " } " )
_T( " print( \" RegisterScriptString実行時に実行されるのがポイント \\n \" ); " );

//実行関数
SQInteger cppfoo(HSQUIRRELVM v)
{
	v;
	SKY_PRINTF( _T( "cppfoo成功 \n" ) );
	return 0;
};

//実行関数
SQInteger cppfoo2(HSQUIRRELVM v)
{
	v;
	SKY_PRINTF( _T( "cppfoo2成功2 \n" ) );
	return 0;
};

#endif

//文字列からスクリプトをロードして実行する
TEST( Script , 4 ) 
{
	SKY_BASE_LIB_INIT();

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );

	spSqVm->RegisterNativeFunc( cppfoo , _T( "cppfoo" ) );
	spSqVm->RegisterScriptString( CHS( TEST4_Script ) );

	sky::lib::squirrel::SqFuncParam inParam;		//引数
	sky::lib::squirrel::SqFuncParam outParam;		//戻り値
	inParam.SetParam( (s32)1 );
	sky::lib::squirrel::SqCppFunc func( cppfoo2 , CHS( _T( "cppfoo2" ) ) );
	inParam.SetParam( &func );

	{ sky::lib::time::CTimerObject callTime( _T( "script call." ) );

	spSqVm->ExecuteFuncSync( CHS( _T( "foo" ) ), inParam , outParam ); //関数実行

	}

	SqVmFactory_->Delete( spSqVm );

#endif

	SKY_BASE_LIB_TERM();

}

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

SQInteger menberfunc0(HSQUIRRELVM v)
{
	v;
	SKY_PRINTF( _T( "memberfunc0成功 \n" ) );
	return 0;
}

SQInteger menberfunc1(HSQUIRRELVM v)
{
	v;
	SKY_PRINTF( _T( "memberfunc1成功 \n" ) );
	return 0;
}

#endif

static const skyString *TEST5_NUTFILE_PATH = _T( "data/test/script/ClassParam.nut" );

//クラスを作成して登録し、そのまま実行する。
TEST( Script , 5 ) 
{
	SKY_BASE_LIB_INIT();

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );
	spSqVm->RegisterScriptFile( CHS( TEST5_NUTFILE_PATH ) );

	//親クラスを作る
	sky::lib::squirrel::SqClassObject classobjBase( CHS( _T( "HogeBase" ) ) );					//HogeBaseという親クラスを作る
	classobjBase.AddMethod( CHS( _T( "memberfunc0" ) ) , menberfunc0 );							//memberfunc0というメソッドをつける

	//親クラスを登録する
	spSqVm->RegisterClass( classobjBase );

	//クラスを作る
	sky::lib::squirrel::SqClassObject classobj( CHS( _T( "Hoge" ) ) , CHS( _T( "HogeBase" ) ) );	//Hogeというクラスを作る（HogeBaseを継承する）
	classobj.AddMethod( CHS( _T( "memberfunc1" ) ) , menberfunc1 );									//memberfunc1というメソッドをつける
	classobj.AddMember( CHS( _T( "member1" ) ) , sky::lib::squirrel::SqParamObject( (s32)1 ) );		//member1という変数をつける

	//クラスを登録する
	spSqVm->RegisterClass( classobj );

	//関数を呼びだす
	sky::lib::squirrel::SqFuncParam inParam;		//引数
	sky::lib::squirrel::SqFuncParam outParam;		//戻り値
	spSqVm->ExecuteFuncSync( CHS( _T( "foo" ) ), inParam , outParam ); //関数実行

	SqVmFactory_->Delete( spSqVm );

#endif

	SKY_BASE_LIB_TERM();
}

static const skyString *TEST6_NUTFILE_PATH = _T( "data/test/script/ClassParam2.nut" );

//関数のパラメーターにクラスのインスタンスを使用する
TEST( Script , 6 ) 
{
	SKY_BASE_LIB_INIT();

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );
	spSqVm->RegisterScriptFile( CHS( TEST6_NUTFILE_PATH ) );

	//クラスを作る
	sky::lib::squirrel::SqClassObject classobj( CHS( _T( "Hoge" ) ) , CHS( _T( "HogeBase" ) ) );	//Hogeというクラスを作る（HogeBaseを継承する）
	classobj.AddMethod( CHS( _T( "memberfunc1" ) ) , menberfunc1 );									//memberfunc1というメソッドをつける
	classobj.AddMember( CHS( _T( "member1" ) ) , sky::lib::squirrel::SqParamObject( (s32)1 ) );		//member1という変数をつける

	//関数を呼びだす
	sky::lib::squirrel::SqFuncParam inParam;		//引数
	sky::lib::squirrel::SqFuncParam outParam;		//戻り値

	//クラスを引数として渡す
	inParam.SetParam( &classobj );

	spSqVm->ExecuteFuncSync( CHS( _T( "foo" ) ) , inParam , outParam ); //関数実行

	SqVmFactory_->Delete( spSqVm );

#endif

	SKY_BASE_LIB_TERM();
}

static const skyString *TEST7_NUTFILE_PATH_1 = _T( "data/test/script/CallFoo1.nut" );
static const skyString *TEST7_NUTFILE_PATH_2 = _T( "data/test/script/CallFoo2.nut" );


//foo関数の書かれたファイルをロードし、実行し、違うfoo関数が書かれたファイルをロードし実行する（動的リロードの実験）
TEST( Script , 7 )
{
	SKY_BASE_LIB_INIT();

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );

	//ファイル読む
	spSqVm->RegisterScriptFile( CHS( TEST7_NUTFILE_PATH_1 ) );

	//関数を呼びだす
	sky::lib::squirrel::SqFuncParam inParam;		//引数
	sky::lib::squirrel::SqFuncParam outParam;		//戻り値
	spSqVm->ExecuteFuncSync( CHS( _T( "foo" ) ), inParam , outParam ); //関数実行

	//ファイル読む
	spSqVm->RegisterScriptFile( CHS( TEST7_NUTFILE_PATH_2 ) );

	//関数を呼びだす
	spSqVm->ExecuteFuncSync( CHS( _T( "foo" ) ), inParam , outParam ); //関数実行
	
	SqVmFactory_->Delete( spSqVm );

#endif

	SKY_BASE_LIB_TERM();
}

//nutファイルリロードクラス
class NutReLoad : public sky::lib::reload::AbsReLoadObject
{

public:

	sky::lib::squirrel::SqVmObjectPtr m_spVm;
	sky::lib::hash::CHashStringPtr	  m_filePath;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	NutReLoad( sky::lib::squirrel::SqVmObjectPtr &spVm , const sky::lib::hash::CHashStringPtr &filePath )
	{
		m_spVm = spVm;
		m_filePath = filePath;
		AddFilePath( filePath );
	}

	/// <summary>
	/// ロード関数
	/// </summary>
	virtual void Load()
	{
		m_spVm->RegisterScriptFile( CHS( m_filePath->GetString() ) );
	}

};

static const skyString *TEST8_NUTFILE_PATH = _T( "data/test/script/ReLoad.nut" );

//スクリプトを実行中に書きかえる。
TEST( Script , 8 ) 
{
	SKY_BASE_LIB_INIT();

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );
	spSqVm->RegisterScriptFile( CHS( TEST8_NUTFILE_PATH ) );

	//リロードオブジェクトの作成
	sky::lib::reload::IReLoadObjectPtr nutReLoad( NEW NutReLoad( spSqVm , CHS( TEST8_NUTFILE_PATH ) ) );

	CReLoadObjectManager_::Add( nutReLoad );

	//クラスを登録する
	spSqVm->RegisterClass( GetSqInputManager() );

	sky::lib::squirrel::SqFuncParam inParam;		//引数
	sky::lib::squirrel::SqFuncParam outParam;		//戻り値

	//FPSコントローラー作成
	sky::lib::fps::CFpsController fpsCtrl;

	SKY_PRINTF( _T( "please push key 'A' \n" ) );

	//ゲームループ
	GAME_LOOP_START
	{
		//フレーム開始
		fpsCtrl.StartFrame();

		//入力管理更新
		Ckeyboard_->Update( fpsCtrl.GetPassageTime() );

		//リロードオブジェクト更新
		CReLoadObjectManager_::Update( fpsCtrl.GetStartTime() );

		//終了判定
		{ sky::lib::time::CTimerObject callTime( _T( "script call" ) );

		//実行
		spSqVm->ExecuteFuncSync( CHS( _T( "foo_ReLoad" ) ), inParam , outParam );

		if ( ( skyBool )outParam[ 0 ] != skyFalse )
		{
			GAME_LOOP_EXIT();
		}

		}

		//フレームレート維持のため、時間が余ったら待つ
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	SqVmFactory_->Delete( spSqVm );

#endif

	SKY_BASE_LIB_TERM();
}

static const skyString *TEST9_NUTFILE_PATH_1 = _T( "data/test/script/ReLoadMain.nut" );
static const skyString *TEST9_NUTFILE_PATH_2 = _T( "data/test/script/ReLoadFunc.nut" );


//２つのnutファイルを読む。一つは制御がかかれていて、毎フレーム中断をする、もうひとつは処理がかかれていて、ファイルを書きなおすとリロードする
TEST( Script , 9 ) 
{
	SKY_BASE_LIB_INIT();

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );
	spSqVm->RegisterScriptFile( CHS( TEST9_NUTFILE_PATH_1 ) );
	spSqVm->RegisterScriptFile( CHS( TEST9_NUTFILE_PATH_2 ) );
	spSqVm->InitExecuteFuncASync();

	//リロードオブジェクトの作成
	sky::lib::reload::IReLoadObjectPtr nutReLoad( NEW NutReLoad( spSqVm , CHS( TEST9_NUTFILE_PATH_2 ) ) );

	CReLoadObjectManager_::Add( nutReLoad );

	//クラスを登録する
	spSqVm->RegisterClass( GetSqInputManager() );

	sky::lib::squirrel::SqFuncParam inParam;		//引数
	sky::lib::squirrel::SqFuncParam outParam;		//戻り値

	//FPSコントローラー作成
	sky::lib::fps::CFpsController fpsCtrl;

	SKY_PRINTF( _T( "please push key 'A' \n" ) );

	//ゲームループ
	GAME_LOOP_START
	{
		//フレーム開始
		fpsCtrl.StartFrame();

		//入力管理更新
		Ckeyboard_->Update( fpsCtrl.GetPassageTime() );

		//リロードオブジェクト更新
		CReLoadObjectManager_::Update( fpsCtrl.GetStartTime() );

		//終了判定
		{ sky::lib::time::CTimerObject callTime( _T( "script call." ) );

		if ( spSqVm->ExecuteFuncASync( CHS( _T( "foo_ReLoadMain" ) ), inParam , outParam ) == sky::lib::squirrel::SqVmObject::eThreadStatus_End )
		{
			GAME_LOOP_EXIT();
		}

		}

		//フレームレート維持のため、時間が余ったら待つ
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	SqVmFactory_->Delete( spSqVm );

#endif

	SKY_BASE_LIB_TERM();
}


static const skyString *TEST10_COMPILE_FILE_SRC_PATH = _T( "data/test/script/CompileSrc.nut" );
static const skyString *TEST10_COMPILE_FILE_DEST_PATH = _T( "data/test/script/CompileDest.nut" );

//スクリプトをコンパイルしてファイルに出力する。そしてそのファイルを実行する
TEST( Script , 10 ) 
{
	SKY_BASE_LIB_INIT();

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );

	//ファイルを読む
	sky::lib::stream::IStreamPtr spSrcStream( NEW sky::lib::file::CDiscFileStream( TEST10_COMPILE_FILE_SRC_PATH , sky::lib::file::eOpenMode_Binary_R_NoCreate ) );
	sky::lib::stream::IStreamPtr spDestStream( NEW sky::lib::file::CDiscFileStream( TEST10_COMPILE_FILE_DEST_PATH , sky::lib::file::eOpenMode_Binary_RW_Create ) );

	//ストリームを開く
	spSrcStream->vOpen();
	spDestStream->vOpen();

	//コンパイルしてみる。
	spSqVm->CompileOutStream( spSrcStream , spDestStream );

	//ストリームを閉じる
	spSrcStream->vClose();
	spDestStream->vClose();

	//ファイルをロードする。
	spSqVm->RegisterScriptFile( CHS( TEST10_COMPILE_FILE_SRC_PATH ) );
//	spSqVm->RegisterScriptFile( CHS( TEST10_COMPILE_FILE_DEST_PATH ) );

	//コンパイル後のファイルを実行
	sky::lib::squirrel::SqFuncParam inParam;		//引数
	sky::lib::squirrel::SqFuncParam outParam;		//戻り値

	spSqVm->ExecuteFuncSync( CHS( _T( "foo" ) ), inParam , outParam ); //関数実行

	SqVmFactory_->Delete( spSqVm );

	SKY_BASE_LIB_TERM();
}

static const skyString *TEST11_CALLBACK_TEST_SQ_PATH = _T( "data/test/script/OneLineCallBackTest.nut" );

void CallBackFunc( HSQUIRRELVM v , SQInteger type , const SQChar * sourcename , SQInteger line , const SQChar * funcname )
{
	v;
	SKY_PRINTF( _T( "type=%d , sourcename=%s , line=%d , funcname=%s \n" ) , type , sourcename , line , funcname );
}

//実行１行ごとにコールバックを受ける
TEST( Script , 11 ) 
{
	SKY_BASE_LIB_INIT();

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );

	spSqVm->RegisterNativeDebugHook( CallBackFunc );
	spSqVm->SetDebug( skyTrue ); //これをしないと行ごとのコールバックがこない（正確には自動で付加されたmain関数のものだけくる）

	//ファイルをロードする。
	spSqVm->RegisterScriptFile( CHS( TEST11_CALLBACK_TEST_SQ_PATH ) );

	sky::lib::squirrel::SqFuncParam inParam;		//引数
	sky::lib::squirrel::SqFuncParam outParam;		//戻り値

	spSqVm->ExecuteFuncSync( CHS( _T( "foo" ) ), inParam , outParam ); //関数実行

	SqVmFactory_->Delete( spSqVm );

	SKY_BASE_LIB_TERM();
}

static const skyString *TEST12_DEBUG_API_TEST_SCRIPT_FILE_PATH = _T( "data/test/script/DebugApiTest.nut" );

//デバッグＡＰＩを実行する
TEST( Script , 12 ) 
{
	SKY_BASE_LIB_INIT();

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );

	SqDebugApi_::RegisterApi(spSqVm->Vm());

	//ファイルをロードする。
	spSqVm->RegisterScriptFile( CHS( TEST12_DEBUG_API_TEST_SCRIPT_FILE_PATH ) );

	sky::lib::squirrel::SqFuncParam inParam;		//引数
	sky::lib::squirrel::SqFuncParam outParam;		//戻り値

	spSqVm->ExecuteFuncSync( CHS( _T( "foo" ) ), inParam , outParam ); //関数実行

	SqVmFactory_->Delete( spSqVm );

	SKY_BASE_LIB_TERM();
}

static const skyString *TEST13_INCLUDE_TEST = _T( "data/test/script/IncludeTest.nut" );

//スクリプトでincludeを実現する
TEST( Script , 13 ) 
{
	SKY_BASE_LIB_INIT();

	sky::lib::squirrel::SqVmObjectPtr spSqVm = SqVmFactory_->Create( 1024 );

	//SkyLibBaseのApiを登録
	CSqUtil_::RegisterSkyBaseLibApi( spSqVm->Vm() );

	//ファイルをロードする。
	spSqVm->RegisterScriptFile( CHS( TEST13_INCLUDE_TEST ) );

	sky::lib::squirrel::SqFuncParam inParam;		//引数
	sky::lib::squirrel::SqFuncParam outParam;		//戻り値

	spSqVm->ExecuteFuncSync( CHS( _T( "foo" ) ), inParam , outParam ); //関数実行

	SqVmFactory_->Delete( spSqVm );

	SKY_BASE_LIB_TERM();
}

//スクリプトからの戻り値をスクリプトの関数の引数に使用する
TEST( Script , 14 ) 
{
}


#endif