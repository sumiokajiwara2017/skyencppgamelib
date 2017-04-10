#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "../DrawCommand/DrawCommand.h"
#include "../DrawCommand/DrawCommandManager.h"
#include "../DrawExecEvent/DrawExecEvent.h"
#include "UpdateThread.h"

UpdateThread::UpdateThread( DrawExecEventPtr spEvent ) :
m_spEvent( spEvent )
{
}

UpdateThread::~UpdateThread( void )
{
}

void UpdateThread::ThreadMain( void )
{
	DrawCommand *pCommand = skyNull;
	skyWString buf[ 128 ];
	for ( s32 i = 0 ; i < 10 ; i++ )
	{
		for ( s32 j = 0 ; j < 5 ; j++ )
		{
			//コマンド作成
			pCommand = NEW DrawCommand();

			//描画データ作成
			IStringAdptr_->Sprintf( buf , 128 , L"%d 番目のデータです%d \n" , i , j);
			pCommand->m_Data = buf;

			//描画データの登録
			DrawCommandManager_->GetUpdateList()->AddTail( pCommand );
		}

		//描画が完了するまで待つ
		while( m_spEvent->Is() ) { ThreadWin32::GetThread()->Sleep( 2 ); } //ここで同期

		//スイッチする
		DrawCommandManager_->SwitchList();

		//描画可能
		m_spEvent->On();
	}

	//終了
	m_spEvent->End();
}