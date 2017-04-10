#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

/// <summary>
/// 定数
/// </summary>
namespace squtil_const
{
	static const u32 SQ_PRINTF_MSG_LENGHT_WCHAR = 256;
	static const u32 SQ_PRINTF_MSG_LENGHT_SCHAR = SQ_PRINTF_MSG_LENGHT_WCHAR * sizeof( skyString );
	static const u32 STACK_SIZE			= 1024;
	static const u32 RET_STRING_LENGTH	= 512;
}

//-------------------------------------< 実装 >---------------------------------------------//

class CSqUtil : public base::SkyObject
{

public:

	/// <summary>
	/// デフォルトＳＱ出力
	/// </summary>
	static void DefaultPrint( HSQUIRRELVM v , const skyString *s , ... );

	/// <summary>
	/// VMを作成する
	/// </summary>
	static HSQUIRRELVM CreateSQVM( u32 stackSize );

	/// <summary>
	/// VMを破棄する
	/// </summary>
	static void DestroySQVM( HSQUIRRELVM v );

	/// <summary>
	/// ファイルを読み込み最後まで実行する（同期）
	/// </summary>
	static void RegisterScriptFileAndExecuteFunc( HSQUIRRELVM v , const hash::CHashStringPtr &pFilePath , const hash::CHashStringPtr &pFuncName , SqFuncParam &param , SqFuncParam &outParam ); //関数を指定できる

	/// <summary>
	/// コルーチンを作成する（コルーチンは親のスタックにプッシュされている）
	/// </summary>
	static HSQUIRRELVM CreateNewThread( HSQUIRRELVM v , u32 stackSize );

	/// <summary>
	/// ファイルを読み込む
	/// </summary>
	static void RegisterScriptFile( HSQUIRRELVM &v , const hash::CHashStringPtr &pFilePath );

	/// <summary>
	/// ファイル読込済みのVMを受け取り関数を実行する
	/// </summary>
	static void ExecuteFunc( HSQUIRRELVM &v , const hash::CHashStringPtr &pFuncName , SqFuncParam &inParam , SqFuncParam &outParam );

	/// <summary>
	/// Squirrel標準関数のPush（sq_pushroottableをスタックのTopに設定した直後（もしくは独自のテーブルを）にCallして下さい）
	/// </summary>
	static void RegisterStdLib( HSQUIRRELVM v);

	/// <summary>
	/// SkyBaseLibのSquirrelApiをVmに登録
	/// </summary>
	static void RegisterSkyBaseLibApi( HSQUIRRELVM v);

	/// <summary>
	/// SkyBaseLibのSquirrelバインドクラスをVmに登録
	/// </summary>
	static void RegisterSkyBaseLibClass( HSQUIRRELVM v);

	/// <summary>
	/// 関数の登録
	/// </summary>
	static void RegisterNativeFunc( HSQUIRRELVM v , SQFUNCTION f , const skyString *fname );

	/// <summary>
	/// スクリプトの実行
	/// </summary>
	static void RegisterScriptString( HSQUIRRELVM v , const hash::CHashStringPtr &pScript );

	/// <summary>
	/// クラスの登録
	/// </summary>
	static void RegisterClass( HSQUIRRELVM v , const SqClassObject &classObj );

	/// <summary>
	/// パラメーターの設定
	/// </summary>
	static void PushParam( HSQUIRRELVM v , const SqParamObject &param  );

	/// <summary>
	/// パラメーターの取得
	/// </summary>
	static void GetParam( HSQUIRRELVM v , SqParamObject &param );

	/// <summary>
	/// 指定のnutストリームをコンパイルして結果をオブジェクトで返す
	/// </summary>
	static SqObjectInterfacePtr Compile( HSQUIRRELVM v , const stream::IStreamPtr &pSrc );

	/// <summary>
	/// 指定のnutストリームをコンパイルして結果をストリームに出力する
	/// </summary>
	static void CompileOutStream( HSQUIRRELVM v , const stream::IStreamPtr &pSrc , const stream::IStreamPtr &pDest );

	/// <summary>
	/// １行ごとにコールバックを返すデバッグ機能をＯＮ／ＯＦＦする
	/// </summary>
	static void RegisterNativeDebugHook( HSQUIRRELVM v, SQDEBUGHOOK debughook );
	static void RegisterDebugHook( HSQUIRRELVM v );
	static void SetDebug( HSQUIRRELVM v , skyBool isOn );

private:

	//各機能封印
	CSqUtil				();										//コンストラクタ
	CSqUtil				( const CSqUtil& ){};					//コピーコンストラクタ
	virtual ~CSqUtil	();										//デストラクタ
	CSqUtil &operator=	(const CSqUtil& ){ return *this; };		//代入演算子オーバーロード
};

} } }

//インスタンスアクセス文字列
#define CSqUtil_			sky::lib::squirrel::CSqUtil

#endif

//バインドクラス定義情報取得
extern sky::lib::squirrel::SqClassObject GetSqInputManager();
extern sky::lib::squirrel::SqClassObject GetSqCDosUtil();
