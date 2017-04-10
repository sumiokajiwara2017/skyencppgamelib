#include "StdAfx.h"
#include "SkyBaseLib/Script.h"

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

namespace sky { namespace lib { namespace squirrel {

SqVmObject::SqVmObject( u32 stackSize ) :
m_Status( eThreadStatus_Wait ) ,
m_spParent( skyNull ) ,
m_ChildStack( stackSize )
{
	//バーチャルマシンの作成
	m_Vm = CSqUtil_::CreateSQVM( stackSize );

	//標準ライブラリをPush
	CSqUtil_::RegisterStdLib( m_Vm );
}

SqVmObject::SqVmObject( SqVmObjectPtr &spParent , u32 stackSize ) :
m_Status( eThreadStatus_Wait ) ,
m_spParent( spParent ) ,
m_ChildStack( stackSize )
{
	m_spParent = spParent;

	//子スレッドの作成（親のスタック上にVmを作成する。すなわち、Popすることでメモリ領域も開放される）
	m_Vm = CSqUtil_::CreateNewThread( spParent->Vm() , stackSize );
}

SqVmObject::~SqVmObject()
{
	//子をすべて破棄
	ChildAllPop();

	//自分より親が居なければ自分で消えていい
	if ( m_spParent.IsEmpty() )
	{
		//バーチャルマシンの破棄
		CSqUtil_::DestroySQVM( m_Vm );
	}
}

SqVmObject::eThreadStatus	SqVmObject::GetASyncStatus()
{
	return m_Status;
}

SqVmObjectPtr &SqVmObject::CreateAndRegisterChildVm( u32 stackSize , SqVmObjectPtr &spParent )
{
	m_ChildStack.Push( SqVmObjectPtr( NEW__( SqVmObject , SqVmObject( spParent , stackSize ) ) ) );
	return m_ChildStack.GetTop();
}

void SqVmObject::ChildPop()
{
	if ( m_ChildStack.GetNum() > 0 )
	{
		if ( m_ChildStack.GetTop()->GetASyncStatus() != eThreadStatus_End )
		{
            SKY_PRINTF( _T( "This child is alive.  \n" ) );
		}

		//親からPop(親のスタックに居たので、Popされることで領域も開放されているようだ）
		sq_pop( m_Vm , 1 );

		//スタックの中のスマートポインタ内包のインスタンスを削除（これやるとメモリ破壊を起こす）
		m_ChildStack.GetTop().Delete();

		//スタックの中から一個取り出す。
		m_ChildStack.Pop();
	}
}

void SqVmObject::ChildAllPop()
{
	//全子を削除
	while( m_ChildStack.GetNum() > 0 )
	{
		ChildPop();
	}
}

SqVmObjectPtr &SqVmObject::Parent()
{
	return m_spParent;
}

skyBool SqVmObject::IsAllChildEnd()
{
	//子が居なければtrue
	if ( m_ChildStack.GetNum() > 0 )
	{
		//子の全ての実行を確認する
		FOREACH( SqVmObjectPtrStack , it , m_ChildStack )
		{
			if ( ( *it )->GetASyncStatus() != eThreadStatus_End )
			{
				return skyFalse;
			}
		}
	}

	return skyTrue;
}

HSQUIRRELVM &SqVmObject::Vm()
{
	return m_Vm;
}

void SqVmObject::RegisterScriptFile( const hash::CHashStringPtr &pPath )
{
	CSqUtil_::RegisterScriptFile( m_Vm , pPath );
}

SqVmObject::eThreadStatus SqVmObject::ExecuteFuncSync( const hash::CHashStringPtr &pFunc, SqFuncParam &inParam , SqFuncParam &outParam )
{
	CSqUtil_::ExecuteFunc( m_Vm , pFunc, inParam , outParam );
	return eThreadStatus_End;
}

void SqVmObject::InitExecuteFuncASync()
{
	m_Status = eThreadStatus_Init;
}

SqVmObject::eThreadStatus SqVmObject::ExecuteFuncASync( const hash::CHashStringPtr &pFunc, SqFuncParam &inParam , SqFuncParam &outParam )
{
	switch ( m_Status )
	{
		case eThreadStatus_Wait:

			SKY_PANIC_MSG( _T( "ExecuteFuncASync() had been called before InitExecuteFuncASync() was called. " ) ); //InitExecuteFuncASync()が呼ばれる前にExecuteFuncASyncが呼ばれました。必ず呼んでからにしてくださいね。
			break;

		case eThreadStatus_Init:	//初期化------------------------▽
		{
			// 呼び出し前のスタックサイズを保存
			SaveFuncStartVmStackTop();		

			// 関数をスタックに乗せる
			sq_pushstring(m_Vm , pFunc->GetString(), -1);

			// 関数をルートテーブルから取得
			if ( SQ_SUCCEEDED( sq_get(m_Vm , -2) ) )
			{					
				//第一引数
				sq_pushroottable( m_Vm );				// "this"をpush (ルートテーブルに)

				//第二引数以降（可変数）
				inParam.PushSquirrelStack( m_Vm );
			}
			else
			{
                SKY_PANIC_MSG( _T( "It failed in the registration of the function.funcName=[ %s ]" ) , pFunc );
			}

			//最初のサスペンドまで実行
			if ( !SQ_SUCCEEDED( sq_call( m_Vm , inParam.GetParamNum() + 1 , SQTrue , SQTrue ) ) ) //実行終了かサスペンドまでここに居るだろう。
			{
                SKY_PANIC_MSG( _T( "It failed in the registration of the function.funcName=[ %s ]" ) , pFunc );
			}

			//現在のVMの状態を取得する
			outParam.PopSquirrelStack( m_Vm );

			//中断状態ならば永続更新へ、処理が終了していたら末期化へ行く
			if ( sq_getvmstate( m_Vm ) == SQ_VMSTATE_SUSPENDED )
			{
				//実行状態へ
				m_Status = eThreadStatus_Execute;
			}
			else
			{
				//末期化処理
				m_Status = eThreadStatus_Term;
			}

			break;
		}
		case eThreadStatus_Execute:		//実行中------------------------▽
		{
			//実行再開
			//第二引数の意味がわからんが、勝手にPopしておかしくなるのでSQFalseをつけておきます。何か便利なのかな？
			if ( !SQ_SUCCEEDED( sq_wakeupvm( m_Vm , SQFalse , SQTrue , SQTrue , SQFalse ) ) ) //実行終了かサスペンドまでここに居るだろう。
			{
                SKY_PANIC_MSG( _T( "The execution of the function failed.\n" ) );
			}

			//戻り値受け取りタイミング
			outParam.PopSquirrelStack( m_Vm );

			switch ( sq_getvmstate( m_Vm ) )
			{
			case SQ_VMSTATE_IDLE:		//待機中

				//末期化
				m_Status = eThreadStatus_Term;

				break;
			case SQ_VMSTATE_RUNNING:	//実行中

				//ここ何すんだろう・・。

				break;
			case SQ_VMSTATE_SUSPENDED:	//中断中

				//なんもしないが。

				break;
			}
			break;
		}
		case eThreadStatus_Term:		//末期化------------------------▽
		{
			// 元のスタックサイズに戻す
			LoadFuncStartVmStackTop();

			//実行終了
			m_Status = eThreadStatus_End;

			//no brea;
		}
		case eThreadStatus_End:			//実行終了------------------------▽
		{

			//何もしない。終わったことに気づいて！

			break;
		}
	}
	return m_Status;
}

HSQUIRRELVM SqVmObject::GetChildVm( u32 childNo )
{
	return m_ChildStack.GetItem( childNo )->Vm();
}

void SqVmObject::SaveFuncStartVmStackTop()
{
	// 呼び出し前のスタックサイズを保存
	m_FuncStartVmStackTop = sq_gettop( m_Vm );
}

void SqVmObject::LoadFuncStartVmStackTop()
{
	// 元のスタックサイズに戻す
	sq_settop(m_Vm , m_FuncStartVmStackTop);
}

void SqVmObject::SetASyncStatus( eThreadStatus eStatus )
{
	m_Status = eStatus;
}

void SqVmObject::RegisterNativeFunc( SQFUNCTION f , const skyString *fname )
{
	CSqUtil_::RegisterNativeFunc( m_Vm , f , fname );
}

void SqVmObject::RegisterScriptString( const hash::CHashStringPtr &pScript )
{
	CSqUtil_::RegisterScriptString( m_Vm , pScript );
}

void SqVmObject::RegisterClass( const SqClassObject &classObj )
{
	CSqUtil_::RegisterClass( m_Vm , classObj );
}

void SqVmObject::SetSkyBaseLibClass()
{
	//クラスを登録する
	RegisterClass( GetSqInputManager() );
	RegisterClass( GetSqCDosUtil() );
}

SqObjectInterfacePtr SqVmObject::Compile( const stream::IStreamPtr &pSrc )
{
	return CSqUtil_::Compile( m_Vm , pSrc );
}

void SqVmObject::CompileOutStream( const stream::IStreamPtr &pSrc , const stream::IStreamPtr &pDest )
{
	CSqUtil_::CompileOutStream( m_Vm , pSrc , pDest );
}

void SqVmObject::RegisterNativeDebugHook( SQDEBUGHOOK debughook )
{
	CSqUtil_::RegisterNativeDebugHook( m_Vm , debughook );
}
void SqVmObject::RegisterDebugHook()
{
	CSqUtil_::RegisterDebugHook( m_Vm );
}
void SqVmObject::SetDebug( skyBool isOn )
{
	CSqUtil_::SetDebug( m_Vm , isOn );
}

} } }

#endif
