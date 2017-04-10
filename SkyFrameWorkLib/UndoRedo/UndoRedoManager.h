#pragma once

namespace sky { namespace lib { namespace framework {

static const u32 MAX_UNDO_STACK_NUM = 50;

/// <summary>
/// UndoRedo�Ǘ�
/// </summary>
class CUndoRedoManager : public base::SkyRefObject
{

public:

    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    virtual ~CUndoRedoManager();

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    CUndoRedoManager( u32 maxUndoCmdStackNum );

    /// <summary>
    /// UndoRedo�R�}���h��Exec�����s�����s�ς݃X�^�b�N�ɐςށB�����s�X�^�b�N�̓N���A����B
    /// </summary>
    void ExecuteCommand( const CUndoRedoCommandPtr &spCommand );

    /// <summary>
    /// ���s�ς݃X�^�b�N�̈�ԏ�̃R�}���h��Redo�����s���A�����s�X�^�b�N�ɐς�
    /// </summary>
    skyBool RedoCommand();

    /// <summary>
    /// �����s�X�^�b�N�̈�ԏ�̃R�}���h��Undo�����s���A���s�ς݃X�^�b�N�ɐς�
    /// </summary>
    skyBool UndoCommand();

    /// <summary>
    /// �����s�X�^�b�N,���s�ς݃X�^�b�N���N���A����i�Z�[�u���̎��̓N���A�j
    /// </summary>
    void Clear();

    /// <summary>
    /// �C���X�^���X���쐬����
    /// </summary>
    static CUndoRedoManagerPtr CreateInstance( u32 maxUndoCmdStackNum  );

private:

    /// <summary>
    /// �ő�t�m�c�n�~�ϐ�
    /// </summary>
	u32 m_MaxUndoCmdStackNum;

    /// <summary>
    /// ���s�ς݃X�^�b�N
    /// </summary>
    CUndoRedoCommandPtrStack  m_BeforeExecuteStack;

    /// <summary>
    /// �����s�X�^�b�N
    /// </summary>
    CUndoRedoCommandPtrStack  m_AfterExecuteStack;
};

} } }

//�C���X�^���X�A�N�Z�X������
#define CUndoRedoManagerCreate_()	sky::lib::framework::CUndoRedoManager::Singleton.CreateInstance()
#define CUndoRedoManager_			sky::lib::framework::CUndoRedoManager::Singleton.GetInstance()
#define CUndoRedoManagerDelete_()	sky::lib::framework::CUndoRedoManager::Singleton.DeleteInstance()
