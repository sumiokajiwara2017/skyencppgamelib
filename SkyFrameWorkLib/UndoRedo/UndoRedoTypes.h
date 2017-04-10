#pragma once

namespace sky { namespace lib { namespace framework {

    class CUndoRedoCommand;
    typedef SmartPointer< CUndoRedoCommand >    CUndoRedoCommandPtr;		//スマートポインタ型定義
    typedef CList< u32 , CUndoRedoCommandPtr >  CUndoRedoCommandPtrList;	//スマートポインタリスト型
    typedef CStack< CUndoRedoCommandPtr >		CUndoRedoCommandPtrStack;    //Stack定義

    class CUndoRedoManager;
    typedef SmartPointer< CUndoRedoManager >  CUndoRedoManagerPtr;			//スマートポインタ型定義

    namespace undoredo
    {
        /// <summary>
        /// デフォルトスタックサイズ
        /// </summary>
        static const u32 STACK_SIZE = 512;
    }

} } }
