#include "SkyBaseLib/String.h"
#include "SkyBaseLib/Thread.h"
#include "../DrawCommand/DrawCommand.h"
#include "../DrawCommand/DrawCommandManager.h"
#include "../DrawExecEvent/DrawExecEvent.h"
#include "DrawThread.h"

DrawThread::DrawThread( DrawExecEventPtr spEvent ) :
m_spEvent( spEvent )
{
}

DrawThread::~DrawThread( void )
{
}

void DrawThread::ThreadMain( void )
{
	skyBool isDraw = skyTrue;
	DrawCommandPtrList *pList;
	while ( isDraw )
	{
		if ( m_spEvent->IsEnd() )
		{
			break;
		}

		//�X�V����������܂ő҂i�����j
		while( !m_spEvent->Is() ) 
		{ 
			ThreadWin32::GetThread()->Sleep( 2 );
		}

		//�`�惊�X�g�擾
		pList = DrawCommandManager_->GetDrawList();

		DrawCommandPtrList::iterator it = pList->Begin();
		while( it != pList->End() )
		{
			SKY_PRINTF( L"%s" , ( * it )->m_Data.Get() ); //�`��

			it = pList->Erases( it );
		}

		m_spEvent->Off();
	}
}