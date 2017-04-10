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
			//�R�}���h�쐬
			pCommand = NEW DrawCommand();

			//�`��f�[�^�쐬
			IStringAdptr_->Sprintf( buf , 128 , L"%d �Ԗڂ̃f�[�^�ł�%d \n" , i , j);
			pCommand->m_Data = buf;

			//�`��f�[�^�̓o�^
			DrawCommandManager_->GetUpdateList()->AddTail( pCommand );
		}

		//�`�悪��������܂ő҂�
		while( m_spEvent->Is() ) { ThreadWin32::GetThread()->Sleep( 2 ); } //�����œ���

		//�X�C�b�`����
		DrawCommandManager_->SwitchList();

		//�`��\
		m_spEvent->On();
	}

	//�I��
	m_spEvent->End();
}