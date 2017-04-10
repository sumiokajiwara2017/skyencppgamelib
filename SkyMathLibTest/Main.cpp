#include "StdAfx.h"
#include "GoogleTestInclude.h"
#include "SkyLibInclude.h"
#include "SkyBaseLib/SkyLibCommon.h"

#include "SkyBaseLib/Memory.h"
#include "SkyBaseLib/GameLoop.h"
#include "SkyBaseLib/Input.h"
#include "SkyBaseLib/Fps.h"

/// <summary>
/// メイン関数
/// </summary>
int _tmain( int argc , _TCHAR *argv[] )
{

	_tsetlocale( LC_ALL , L"japanese"); //これを宣言すると_tprintfで日本語が文字化けしなくなる。

	_tprintf( _T( "< =================  SkyBaseLib Test Start !! =========================== >\n" ) );

	testing::InitGoogleTest( &argc, argv ); 
	RUN_ALL_TESTS();
	_tprintf( _T( "< =================  SkyBaseLib Test End !! =========================== >\n" ) );

	SKY_MATH_LIB_INIT();

	//FPSコントローラー作成
	sky::lib::fps::CFpsController fpsCtrl;

	_tprintf( _T( "====> Please Push Enter \n" ) );

	//ゲームループ
	GAME_LOOP_START
	{
		//フレーム開始
		fpsCtrl.StartFrame();

		//入力管理更新
		Ckeyboard_->Update( fpsCtrl );

		//入力受付
		if ( Ckeyboard_->IsKeyPress( KEY_ENTER ) )
		{
			GAME_LOOP_EXIT();
		}

		//フレームレート維持のため、時間が余ったら待つ
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	SKY_MATH_LIB_TERM();

	return 0;
}