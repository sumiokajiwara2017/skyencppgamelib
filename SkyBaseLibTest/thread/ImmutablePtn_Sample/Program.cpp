#include "SkyBaseLib/Controller.h"
#include "SkyBaseLib/Thread.h"

/// <summary>
/// メイン関数
/// </summary>
int _tmain( int argc , _TCHAR *argv[] )
{
argc;
argv;

	//ライブラリ初期化
	SkyLibController_->Init();

	//テストコード特になし。
	//

	//ライブラリ末期化
	SkyLibController_->Term();
}