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
	//Windowsのメモリリークチェック初期化
//	CrtLeakCheckInit();
	{

	SKY_PRINTF( _T( "< =================  SkyBaseLib Test Start !! =========================== >\n" ) );

	testing::InitGoogleTest( &argc, argv ); 
	RUN_ALL_TESTS();

	SKY_PRINTF( _T("< =================  SkyBaseLib Test End !! =========================== >\n" ) );

	}
	//Windowのメモリリークチェック
//	CrtLeakCheck();

	SKY_BASE_LIB_INIT();

	//FPSコントローラー作成
	sky::lib::fps::CFpsController fpsCtrl;

	SKY_PRINTF( _T( "====> Please Push Enter \n" ) );

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

	SKY_BASE_LIB_TERM();

	return 0;
}

