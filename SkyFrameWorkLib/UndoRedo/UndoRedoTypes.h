#pragma once

namespace sky { namespace lib { namespace framework {

    class CUndoRedoCommand;
    typedef SmartPointer< CUndoRedoCommand >    CUndoRedoCommandPtr;		//�X�}�[�g�|�C���^�^��`
    typedef CList< u32 , CUndoRedoCommandPtr >  CUndoRedoCommandPtrList;	//�X�}�[�g�|�C���^���X�g�^
    typedef CStack< CUndoRedoCommandPtr >		CUndoRedoCommandPtrStack;    //Stack��`

    class CUndoRedoManager;
    typedef SmartPointer< CUndoRedoManager >  CUndoRedoManagerPtr;			//�X�}�[�g�|�C���^�^��`

    namespace undoredo
    {
        /// <summary>
        /// �f�t�H���g�X�^�b�N�T�C�Y
        /// </summary>
        static const u32 STACK_SIZE = 512;
    }

} } }
