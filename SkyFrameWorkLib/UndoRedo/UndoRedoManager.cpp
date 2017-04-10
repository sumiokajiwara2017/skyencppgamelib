#include "StdAfx.h"
#include "SkyFrameWorkLib/UndoRedo.h"

namespace sky { namespace lib { namespace framework {

CUndoRedoManager::~CUndoRedoManager()
{
    Clear();
}

void CUndoRedoManager::ExecuteCommand( const CUndoRedoCommandPtr &spCommand )
{
    spCommand->Execute();

    m_BeforeExecuteStack.Push( spCommand );
    m_AfterExecuteStack.Clear();

	if ( m_MaxUndoCmdStackNum < m_BeforeExecuteStack.GetNum() )
	{
		m_BeforeExecuteStack.PopBottom();
	}
}

skyBool CUndoRedoManager::RedoCommand()
{
	if ( m_AfterExecuteStack.GetNum() == 0 )
	{
		return skyFalse;
	}

    CUndoRedoCommandPtr spCommand;
    m_AfterExecuteStack.Pop( spCommand );

    spCommand->Redo();

    m_BeforeExecuteStack.Push( spCommand );

	return skyTrue;
}

skyBool CUndoRedoManager::UndoCommand()
{
	if ( m_BeforeExecuteStack.GetNum() == 0 )
	{
		return skyFalse;
	}

    CUndoRedoCommandPtr spCommand;
    m_BeforeExecuteStack.Pop( spCommand );

    spCommand->Undo();

    m_AfterExecuteStack.Push( spCommand );

	return skyTrue;
}

void CUndoRedoManager::Clear()
{
    m_BeforeExecuteStack.Clear();
    m_AfterExecuteStack.Clear();
}

CUndoRedoManagerPtr CUndoRedoManager::CreateInstance( u32 maxUndoCmdStackNum )
{
    return CUndoRedoManagerPtr( NEW__( CUndoRedoManager , CUndoRedoManager( maxUndoCmdStackNum ) ) );
}

CUndoRedoManager::CUndoRedoManager( u32 maxUndoCmdStackNum ) :
    m_BeforeExecuteStack( undoredo::STACK_SIZE ) ,
    m_AfterExecuteStack( undoredo::STACK_SIZE ) ,
	m_MaxUndoCmdStackNum( maxUndoCmdStackNum )
{

}

} } }
