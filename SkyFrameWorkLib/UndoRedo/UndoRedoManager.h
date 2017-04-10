#pragma once

namespace sky { namespace lib { namespace framework {

static const u32 MAX_UNDO_STACK_NUM = 50;

/// <summary>
/// UndoRedo管理
/// </summary>
class CUndoRedoManager : public base::SkyRefObject
{

public:

    /// <summary>
    /// デストラクタ
    /// </summary>
    virtual ~CUndoRedoManager();

    /// <summary>
    /// コンストラクタ
    /// </summary>
    CUndoRedoManager( u32 maxUndoCmdStackNum );

    /// <summary>
    /// UndoRedoコマンドのExecを実行し実行済みスタックに積む。未実行スタックはクリアする。
    /// </summary>
    void ExecuteCommand( const CUndoRedoCommandPtr &spCommand );

    /// <summary>
    /// 実行済みスタックの一番上のコマンドのRedoを実行し、未実行スタックに積む
    /// </summary>
    skyBool RedoCommand();

    /// <summary>
    /// 未実行スタックの一番上のコマンドのUndoを実行し、実行済みスタックに積む
    /// </summary>
    skyBool UndoCommand();

    /// <summary>
    /// 未実行スタック,実行済みスタックをクリアする（セーブ等の時はクリア）
    /// </summary>
    void Clear();

    /// <summary>
    /// インスタンスを作成する
    /// </summary>
    static CUndoRedoManagerPtr CreateInstance( u32 maxUndoCmdStackNum  );

private:

    /// <summary>
    /// 最大ＵＮＤＯ蓄積数
    /// </summary>
	u32 m_MaxUndoCmdStackNum;

    /// <summary>
    /// 実行済みスタック
    /// </summary>
    CUndoRedoCommandPtrStack  m_BeforeExecuteStack;

    /// <summary>
    /// 未実行スタック
    /// </summary>
    CUndoRedoCommandPtrStack  m_AfterExecuteStack;
};

} } }

//インスタンスアクセス文字列
#define CUndoRedoManagerCreate_()	sky::lib::framework::CUndoRedoManager::Singleton.CreateInstance()
#define CUndoRedoManager_			sky::lib::framework::CUndoRedoManager::Singleton.GetInstance()
#define CUndoRedoManagerDelete_()	sky::lib::framework::CUndoRedoManager::Singleton.DeleteInstance()
