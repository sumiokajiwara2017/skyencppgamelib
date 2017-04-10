#pragma once

#ifdef SW_SKYLIB_SCRIPT_USE_SQUIRREL

/*
	参考URL
	http://muffin.cias.osakafu-u.ac.jp/~matumoto/cgi-bin/xt.cgi?prog/squirrel
	http://monsho.blog63.fc2.com/blog-entry-20.html
*/

#define SW_USE_SQ_STD_LIB //標準ライブラリを使用するというスイッチ
#define SW_DISPLAY_SQ_LOG //Squirrelの動作ログを出す。
#define SW_USE_SKYALOCATOR_USE //SkyLibのアロケーターを使用する

#include "SquirrelTypes.h"

#undef type //GoogleTest等と競合するのでdefineを取り消す


/// <summary>
/// Ａｐｉ情報
/// </summary>
struct ApiInfo
{
	const skyString *pName;
	SQFUNCTION		funcAdr;
};

#define SQAPI( funcname )	 SQInteger funcname( HSQUIRRELVM v )		//関数宣言用
#define SQAPITBL( funcname ) { _T( #funcname ) , funcname },			//テーブル宣言用

/*
	====メモ====
---------------------------------------------------------------------------------------------
	
	Squirrelはテーブルとスロットという概念があるが、テーブルを構成するのはスロットで、
	スロットとは (キーと値) のペアのことです。Null 以外のすべての値がキーの役割を果たす
	ことが可能で、スロットには任意の値を割り当てることができます。

---------------------------------------------------------------------------------------------

	◇Squirrelから呼び出したい関数は、次のような型でなければならない(fnが呼び出したい関数)。
	『 SQInteger (*SQFUNCTION)(HSQUIRRELVM) 』 (Squirrelのヘッダに定義されている）

---------------------------------------------------------------------------------------------

	◇SquirrelとC++の処理制御１フレームにおけるシーケンス

		C++				Squirrel			Squirrel（必要に応じて）
		 ｜				    
		 ｜	  制御移動→
		 ＋---------------→＋
		 				    ｜
		 				    ｜    制御移動→ 
		 				    ＋-----------------＋
							 				　 ｜
		 									   ｜<<更新>>
		 									   ｜
		 				    ＋-----------------＋
		 				    ｜   ←制御移動
		 				    ｜
		 ＋←---------------＋
		 ｜	  ←制御移動
		 ｜				    
 <<描画>>｜				    
		 ｜				    
		 ｜				    
		 ▽					

		 基本ゲームの更新処理制御はSquirrelで実行する（その過程でC++側のコードをコールすることはあるだろう）
		 描画制御はC++側で行う。


*/

#endif