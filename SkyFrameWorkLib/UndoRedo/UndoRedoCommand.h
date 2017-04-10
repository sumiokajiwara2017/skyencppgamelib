#pragma once

namespace sky { namespace lib { namespace framework {

/// <summary>
/// UndoRedo�R�}���h
/// ���̃N���X���p�����č쐬�����N���X�͕K��SkyTypeDefRTTI��`���s���A�X�}�[�g�|�C���^�ň����Ă��������B
/// </summary>
class CUndoRedoCommand : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    virtual ~CUndoRedoCommand(){};

    /// <summary>
    /// ���s
    /// </summary>
    virtual void Execute() = 0;

    /// <summary>
    /// Undo
    /// </summary>
    virtual void Undo() = 0;

    /// <summary>
    /// Redo
    /// </summary>
    virtual void Redo() = 0;

protected:

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    CUndoRedoCommand(){};

};

} } }
