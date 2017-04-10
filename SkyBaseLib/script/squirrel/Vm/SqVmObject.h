#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

//ノードスタックの型
class SqVmObject; //リスト宣言のための先行宣言
typedef SmartPointer< SqVmObject >			SqVmObjectPtr;		//スマートポインタ版定義
typedef CList< u32 , SqVmObjectPtr >		SqVmObjectPtrList;	//リスト定義
typedef CStack< SqVmObjectPtr >				SqVmObjectPtrStack;	

class AbsBindClass;

/// <summary>
/// Squirrelバーチャルマシンクラス（Compositeパターン）
/// </summary>
class SqVmObject : public base::SkyRefObject
{
public:
	/// <summary>
	/// コルーチンステータス
	/// </summary>
	typedef enum
	{
		eThreadStatus_Wait = 0 ,	//待機
		eThreadStatus_Init ,		//初期化
		eThreadStatus_Execute , 	//実行中
		eThreadStatus_Term ,		//末期化
		eThreadStatus_End ,			//実行終了
	} eThreadStatus;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SqVmObject							( u32 stackSize );
	SqVmObject							( SqVmObjectPtr &psParent , u32 stackSize );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SqVmObject					();

	/// <summary>
	/// getter/setter
	/// </summary>
	eThreadStatus	GetASyncStatus	();

	/// <summary>
	/// 子のＶＭ（実はスレッド）を作成して、親のＶＭのスタックに積んで返す。
	/// </summary>
	SqVmObjectPtr &CreateAndRegisterChildVm	( u32 stackSize , SqVmObjectPtr &spParent );

	/// <summary>
	/// 子の削除＆StackからPop（孫が居るとPopしない）
	/// </summary>
	void ChildPop					();
	void ChildAllPop				();

	/// <summary>
	/// 子が全て孫が居ずかつ実行を終了しているか？、子は居ないか？
	/// </summary>
	skyBool IsAllChildEnd			();

	/// <summary>
	/// 親の取得
	/// </summary>
	SqVmObjectPtr &Parent					();

	/// <summary>
	/// nutファイルのスクリプトをＶＭに登録する。（おそらく関数はクロージャーとして登録され、変数宣言はグローバル変数として登録され、関数実行は実行される）
	/// </summary>
	void RegisterScriptFile				( const hash::CHashStringPtr &pPath );

	/// <summary>
	/// function同期実行（最後まで１回で実行しきるのでスタックの位置は実行前と実行後で同じ。）
	/// </summary>
	eThreadStatus ExecuteFuncSync	( const hash::CHashStringPtr &spFunc, SqFuncParam &inParam , SqFuncParam &outParam );

	/// <summary>
	/// function非同期実行(suspendの戻り値もoutParamに入ってくる）
	/// </summary>
	void InitExecuteFuncASync		();
	eThreadStatus ExecuteFuncASync	( const hash::CHashStringPtr &pFunc, SqFuncParam &inParam ,  SqFuncParam &outParam );
	skyBool	IsExecuteFuncASyncEnd	(){ return ( m_Status == eThreadStatus_End ); };

	/// <summary>
	/// classの設定
	/// </summary>
	void RegisterClass					( const SqClassObject &classObj );

	/// <summary>
	/// C++関数の設定
	/// </summary>
	void RegisterNativeFunc( SQFUNCTION f , const skyString *fname );

	/// <summary>
	/// スクリプトの実行（文字列としてスクリプトを受け取り実行する）
	/// </summary>
	void RegisterScriptString( const hash::CHashStringPtr &pScript );

	/// <summary>
	/// SquirrelVmの取得
	/// </summary>
	HSQUIRRELVM &Vm();

	/// <summary>
	/// 子のVM取得
	/// </summary>
	HSQUIRRELVM GetChildVm( u32 childNo );

	/// <summary>
	/// SkyBaseLibのクラスを登録する
	/// </summary>
	void SetSkyBaseLibClass();

	/// <summary>
	/// 指定のnutストリームをコンパイルして結果をオブジェクトで返す
	/// </summary>
	SqObjectInterfacePtr Compile( const stream::IStreamPtr &pSrc );

	/// <summary>
	/// 指定のnutストリームをコンパイルして結果をストリームに出力する
	/// </summary>
	void CompileOutStream( const stream::IStreamPtr &pSrc , const stream::IStreamPtr &pDest );

	/// <summary>
	/// １行ごとにコールバックを返すデバッグ機能をＯＮ／ＯＦＦする
	/// </summary>
	void RegisterNativeDebugHook( SQDEBUGHOOK debughook );  //Ｃ言語の関数を１行ごとにコールバックする
	void RegisterDebugHook();							   //スタックの一番上がクロージャーであることを前提に、そのクロージャーを１行毎にコールバックする
	void SetDebug( skyBool isOn );				     	//デバッグをＯＦＦにする

private:

	/// <summary>
	/// 親の参照
	/// </summary>
	SqVmObjectPtr							m_spParent;

	/// <summary>
	/// 子の参照スタック
	/// </summary>
	SqVmObjectPtrStack					m_ChildStack;

	/// <summary>
	/// バーチャルマシン
	/// </summary>
	HSQUIRRELVM						m_Vm;

	/// <summary>
	/// 関数呼び出し前のＶＭスタックＴｏｐ
	/// </summary>
	u32								m_FuncStartVmStackTop;

	/// <summary>
	/// 非同期実行時の実行ステータス
	/// </summary>
	eThreadStatus					m_Status;

	/// <summary>
	/// 関数スタートスタック位置のSave/Load（公開の必要なし）
	/// </summary>
	void SaveFuncStartVmStackTop();
	void LoadFuncStartVmStackTop();

	/// <summary>
	/// ステータス設定
	/// </summary>
	void	SetASyncStatus( eThreadStatus eStatus );
};

} } }

#endif