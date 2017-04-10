#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_GAMELOOP

#include "SkyBaseLib/GameLoop.h"
#include "SkyBaseLib/Input.h"
#include "SkyBaseLib/Fps.h"

static const u32 FRAME_RATE = 60;

TEST( gameloop , 1 ) 
{
	SKY_BASE_LIB_INIT();

	//FPSコントローラー作成
	sky::lib::fps::CFpsController fpsCtrl;

	//フレームレートの設定
	fpsCtrl.SetFrameRate( FRAME_RATE );

	SKY_PRINTF( _T( "エスケープキーを押下してください。 \n" ) );

	//ゲームループ
	GAME_LOOP_START
	{
		//フレーム開始
		fpsCtrl.StartFrame();

		//入力管理更新
		Ckeyboard_->Update( fpsCtrl.GetPassageTime() );

		//入力受付
		if ( Ckeyboard_->IsKeyPress( KEY_ENTER ) )
		{
			//現在のフレーム間の経過時間とフレームレートから算出した1フレームあたりの経過時間を出力する。この値はイコールであるのが理想。
			SKY_PRINTF( _T( "PassTime=[ %f ms ] OneFrameTime=[ %f ms ] \n" ) ,  fpsCtrl.GetPassageTime() , fpsCtrl.GetOneFrameTime() );
		}

		//終了判定
		if ( Ckeyboard_->IsKeyPress( KEY_ESCAPE ) )
		{
			GAME_LOOP_EXIT();
		}

		//フレームレート維持のため、時間が余ったら待つ
		fpsCtrl.EndFrame();
	}
	GAME_LOOP_END

	SKY_BASE_LIB_TERM();
}

#endif