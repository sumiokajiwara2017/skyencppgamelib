#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#include "SkyFrameWorkLib/UndoRedo.h"

#ifdef TEST_UNDOREDO

using namespace sky::lib;

#if ON_

class CHogeCommand;
typedef SmartPointer< CHogeCommand >    CHogeCommandPtr;		//スマートポインタ型定義
class CHogeCommand : public framework::CUndoRedoCommand
{
    SkyTypeDefRTTI;

public:

	u32 m_No;

	CHogeCommand( u32 no )
	{
		m_No = no;
	}

	static CHogeCommandPtr CreateInstance( u32 no )
	{
		return CHogeCommandPtr( NEW CHogeCommand( no ) );
	}
	virtual void Execute()
	{
		SKY_PRINTF( _T( "Execute%d(); \n" ) , m_No );
	};
    virtual void Undo()
	{
		SKY_PRINTF( _T( "Undo%d(); \n" ) , m_No );
	};
    virtual void Redo()
	{
		SKY_PRINTF( _T( "Redo%d(); \n" ) , m_No );
	};
};
SkyImplementRTTI( CHogeCommand , sky::lib::framework::CUndoRedoCommand );

TEST( undoredo , 1 ) 
{
	SKY_LIB_GRAPHIC_INIT();

	CHogeCommandPtr spHoge1 = CHogeCommand::CreateInstance( 1 );
	CHogeCommandPtr spHoge2 = CHogeCommand::CreateInstance( 2 );
	CHogeCommandPtr spHoge3 = CHogeCommand::CreateInstance( 3 );
	CHogeCommandPtr spHoge4 = CHogeCommand::CreateInstance( 4 );

	CUndoRedoManager_->ExecuteCommand( spHoge1 );
	CUndoRedoManager_->ExecuteCommand( spHoge2 );
	CUndoRedoManager_->ExecuteCommand( spHoge3 );
	CUndoRedoManager_->ExecuteCommand( spHoge4 );

	CUndoRedoManager_->RedoCommand();
	CUndoRedoManager_->RedoCommand();
	CUndoRedoManager_->RedoCommand();
	CUndoRedoManager_->RedoCommand();
	CUndoRedoManager_->RedoCommand();

	CUndoRedoManager_->UndoCommand();
	CUndoRedoManager_->UndoCommand();
	CUndoRedoManager_->UndoCommand();
	CUndoRedoManager_->UndoCommand();
	CUndoRedoManager_->UndoCommand();

	SKY_LIB_GRAPHIC_TERM();
}

#endif

#endif