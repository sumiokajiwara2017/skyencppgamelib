#include "StdAfx.h"
#include "GoogleTestInclude.h"
#include "SkyLibInclude.h"
#include "SkyBaseLib/Base.h"

//#include "glut/glut.h"

/// <summary>
/// メイン関数
/// </summary>
int _tmain( int argc , _TCHAR *argv[] )
{
	testing::InitGoogleTest( &argc, argv ); 
	RUN_ALL_TESTS();

	SKY_LIB_GRAPHIC_INIT();

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

	SKY_LIB_GRAPHIC_TERM();

	return 0;
}
