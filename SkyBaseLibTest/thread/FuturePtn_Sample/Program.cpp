#include "SkyBaseLib/Controller.h"
#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"

#include "Data/Data.h"
#include "Helper/Helper.h"
#include "Host/Host.h"

/// <summary>
/// メイン関数
/// </summary>
int _tmain( int argc , _TCHAR *argv[] )
{
	argc;
	argv;

	//データ作成
	DataSmPtr	data( NEW Data() );

	//ホスト作成
	Host host;

	//仕事実行
	host.DoWork( L"仕事を依頼しました。\n" , data );

	SKY_PRINTF( L"応答はすぐに返る\n" );

	//データを得る（ここで停止）
	const skyWString *pData = data->GetData();

	SKY_PRINTF( L"仕事結果= %s \n" , pData );

	IThreadManager_->WaitAllThreadEnded(); //全てのスレッドの終了を待つ
}